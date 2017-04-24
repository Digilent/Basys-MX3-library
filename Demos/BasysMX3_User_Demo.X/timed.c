#include "config.h"
#include "lcd.h"
#include "led.h"
#include "timed.h"
#include "utils.h"
#include "rgbled.h"
#include "audio.h"
#include "btn.h"
#include <xc.h>  
#include <sys/attribs.h>
#include <string.h>

#define LEDS_THRESHOLD 100
#define RGBLED_THRESHOLD 500
#define LCD_THRESHOLD 2000
#define RECPLAY_INITIALWAIT_THRESHOLD 300
#define RECPLAY_REC_THRESHOLD 2000
#define RECPLAY_PLAY_THRESHOLD 2000
#define PLAY_BEEP_THRESHOLD 1000

unsigned char fAudio = 0, fVis = 0;
unsigned char rgLedVals[8];
unsigned int rgSSDVals[] = 
{
    0x0246,
    0x2468,
    0x4680,
    0x6802,
    0x8024,
};

unsigned char rgSSDDPVals[] = 
{
    0x08,
    0x04,
    0x02,
    0x01,
    0x00,
};
#define cntSSDVals (sizeof(rgSSDVals) / sizeof(rgSSDVals[0]))

unsigned int rgRGBLedVals[] = 
{
    0x00C00000,
    0x0000C000,
    0x000000C0,
    0x00C0C0C0
};


typedef enum {
    RECPLAY_INITIALWAIT,
	RECPLAY_REC,
	RECPLAY_PLAY,
    PLAY_BEEP
} audio_stat;

audio_stat audioStat;

// counters for the interrupt handler
int cntRecPlay;
int cntLeds;
int cntRGBLed;
int cntLCD;
extern volatile unsigned char UPDATE_DISP;
extern volatile unsigned char ACL_POT_EN;

/***	Timer4SR
**
**	Parameters:
**		none
**
**	Return Value:
**		none
**
**	Description:
**		This is the ISR for the Timer4. 
**      It triggers all the BIST functionality that needs some timing:
**          - Audio Record Play test: advance in the state machine (initial wait, record, play) after the required time
**          - Audio Beep test: finish the beep after the required time
**          - Visual controls: 
**              - implement LEDs activity according to the time flow
**              - implement RGBLEDs activity according to the time flow
**              - implement SSD activity according to the time flow
**          
*/
void __ISR(_TIMER_4_VECTOR, ipl2auto) Timer4SR(void) 
{
    static unsigned char idxLedVal = 0, idxRGBLedVal = 0, idxSSDVal = 0, btnVal=0;

    if(fAudio)
    {
        // this is a kind of state machine:
        // RecPlayStarted -> Inital wait -> Record -> Play -> RecPlayCompleted
        if(audioStat == RECPLAY_INITIALWAIT && ++cntRecPlay == RECPLAY_INITIALWAIT_THRESHOLD)
        {
            // move to next state
            audioStat = RECPLAY_REC;
            cntRecPlay = 0;
            LED_SetValue(7, 0);
            LED_SetValue(6, 1); // LD6 ON - REC
            AUDIO_Init(2);  // Init recording
        }
        if(audioStat == RECPLAY_REC && ++cntRecPlay == RECPLAY_REC_THRESHOLD)
        {
            // move to next state
            audioStat = RECPLAY_PLAY;
            cntRecPlay = 0;
            LED_SetValue(6, 0);
            LED_SetValue(5, 1); // LD5 ON - PLAY
            AUDIO_Init(3);  // Init play
        }
        if(audioStat == RECPLAY_PLAY && ++cntRecPlay == RECPLAY_PLAY_THRESHOLD)
        {
            // finished
            LED_SetValue(5, 0);
            AUDIO_Close();
        }
        if(audioStat == PLAY_BEEP && ++cntRecPlay == PLAY_BEEP_THRESHOLD)
        {
            // finished
            //LED_SetValue(4, 0);
            AUDIO_Close();
        }
        
    }
    
    if(fVis)
    {
//        if(++cntLeds == LEDS_THRESHOLD)
//        {
//            cntLeds = 0;
//            // do the LEDs activity
//            LED_SetGroupValue(rgLedVals[(idxLedVal++)& 0x7]); // restrict the index values on 3 bits (0-7)
//        }
        if(++cntLCD == LCD_THRESHOLD){
            cntLCD=0;
            UPDATE_DISP=1;
        }
        if(++cntRGBLed == RGBLED_THRESHOLD)
        {
            cntRGBLed = 0;
            // do the RGBLEDs activity
            RGBLED_SetValueGrouped(rgRGBLedVals[(idxRGBLedVal++)& 0x3]); // restrict the index values on 2 bits (0-3)

            SSD_WriteDigitsGrouped(rgSSDVals[idxSSDVal], rgSSDDPVals[idxSSDVal]); 
            // restrict the index values to the array range 
            if((++idxSSDVal) == cntSSDVals)
            {
                idxSSDVal = 0;
            }
        }
    }
    IFS0bits.T4IF = 0;                  // clear interrupt flag
}

/***	Timer4Setup
**
**	Parameters:
**		none
**
**	Return Value:
**		none
**
**	Description:
**		This function configures Timer4 to generate in interrupt every 1ms, to be used for BIST functionality that needs timing.
**          
*/
void Timer4Setup()
{
    static int fTimerInitialised = 0;
    if(!fTimerInitialised)
    {
        
                                          //  setup peripheral
        PR4 = 10000;                        //             set period register, generates one interrupt every 1 ms
        TMR4 = 0;                           //             initialize count to 0
        T4CONbits.TCKPS = 3;                //            1:256 prescale value
        T4CONbits.TGATE = 0;                //             not gated input (the default)
        T4CONbits.TCS = 0;                  //             PCBLK input (the default)
        T4CONbits.ON = 1;                   //             turn on Timer1
        IPC4bits.T4IP = 2;                  //             priority
        IPC4bits.T4IS = 0;                  //             subpriority
        IFS0bits.T4IF = 0;                  //             clear interrupt flag
        IEC0bits.T4IE = 1;                  //             enable interrupt
        fTimerInitialised = 1;
    }
}

/***	VisCtrlsTest
**
**	Parameters:
**		szMsg      - the character string that the log / error messages will be written
**
**	Return Value:
**		always return 1 (PASS).
**
**	Description:
**		This function implements the test for visual controls: LEDs, RGBLed, SSD. It is aimed to be called from main bist loop.
**      -	The LEDs will display a shifting value from left to right ? changed at about 100ms
**      - 	The RGBLed will display a sequence of colors ? changed at about 500ms:
**      - 	The SSD will display a sequence of values 
**      The test always returns 1 (PASS).
**          
*/
unsigned char VisCtrlsTest()//char *szMsg)
{
    unsigned char i, ledsVal = 1; 
    fVis = 1;
    fAudio = 0; 
    cntRGBLed = 0;
    //LCD_Init();
    LED_Init();
    RGBLED_Init();
    SSD_Init();
    //LCD_WriteStringAtPos("Digilent", 0, 0);
    //LCD_WriteStringAtPos("Test", 1, 0);
    // prepare the LED values
    for(i = 0; i<8; i++)
    {
        rgLedVals[i] = ledsVal;
        ledsVal <<= 1;
    }
    Timer4Setup(); // this will fire up the LEDs, RGBLed, SSD activity
    //strcpy(szMsg, "Visual test started");
    return 1;
}

/***	BIST_FRecPlayTest
**
**	Parameters:
**		szMsg      - the character string that the log / error messages will be written
**
**	Return Value:
**		always return 1 (PASS).
**
**	Description:
**		This function implements the Record - Play Audio Test.
**      The test will be made of three states:
**          - Initial wait (aprox 300 ms)
**          - Record (aprox 2s)
**          - Play (aprox 2s)
**      The test always returns 1 (PASS).
**          
*/
unsigned char RecPlayTest(char *szMsg)
{
    fVis = 0;
    fAudio = 1; 
    audioStat = RECPLAY_INITIALWAIT; // initial state
    cntRecPlay = 0;
    LED_Init();
    ADC_Init();
    LED_SetGroupValue(0x80); // LD7 ON - INITIALWAIT
    Timer4Setup(); // this will fire up the timer interrupt 
    strcpy(szMsg, "Rec-Play test started");
    return 1;
}

/***	BIST_FRecPlayTest
**
**	Parameters:
**		szMsg      - the character string that the log / error messages will be written
**
**	Return Value:
**		always return 1 (PASS).
**
**	Description:
**		This function implements the Beep Audio Test.
**      The test will generate a beep that will be turned off after aprox 2s:
**      The test always returns 1 (PASS).
**          
*/
unsigned char BeepTest(char *szMsg)
{
    fVis = 0;
    fAudio = 1; 
    audioStat = PLAY_BEEP; 
    cntRecPlay = 0;
    //LED_Init();
    //LED_SetGroupValue(0x10); // LD4 ON - BEEP
    Timer4Setup(); // this will fire up the timer interrupt
    AUDIO_Init(0);
    strcpy(szMsg, "Beep test started");
    return 1;
}