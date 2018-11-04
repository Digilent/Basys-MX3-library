
/* ************************************************************************** */
/** Descriptive File Name

  @Company
 * University of Delaware

  @File Name
    tone.c

  @Description
    This file groups the functions that implement the TONE library.
    The library implements functionality for generating audio using a sine waves and
*   playing them back through the on board speaker.

    Include the file in the project. together with config.h when this library is needed.
*   This library cannot be used at the same time as the Audio library, this
*   is intended as an alternative.

  @Author
 * Theo Fleck
 */
/* ************************************************************************** */

/* ************************************************************************** */
/* ************************************************************************** */
/* Section: Included Files                                                    */
/* ************************************************************************** */
#include <xc.h>
#include <sys/attribs.h>
#include <math.h>
#include "config.h"
#include "tone.h"

//global variable to ensure TONE_Init() is only called once
int initRun = 0;

// This array contains the sine wave samples, and the number of samples stored
// 159 samples at 48kHz = 300Hz (minimum frequency tone)
int RGSIN_SIZE = 160; // # of samples in array to be played
int idxAudioBuf = 0;// current position in array
unsigned short rgSinSamples[160];

// defines the sample rate of the sine wave
#define TMR_FREQ_SINE 48000


/* ------------------------------------------------------------ */
/***	Timer3ISR
**
**	Description:
**		This is the interrupt handler for Timer3.
        Advance current index in the sine definition buffer, initialize OC1 with 
 *      the current sine definition value.
**          
*/
void __ISR(_TIMER_3_VECTOR, IPL7AUTO) Timer3ISR(void) 
{      
    
    // play sine
    // load sine value into OC register
    OC1RS = 4*rgSinSamples[(++idxAudioBuf) % RGSIN_SIZE];
    
    IFS0bits.T3IF = 0;      // clear Timer3 interrupt flag
}


/* ------------------------------------------------------------ */
/***	TONE_Init
**
**	Parameters:
**
**	Return Value:
**		
**	Description:
**		This function initializes the TONE module
**      The output pin corresponding to TONE module (same as the AUDIO, A_OUT)
 *      is configured as digital output and is mapped to OC1.
**      Timer3 is set and specific initialization is performed:
**      Timer3 is initialized at a frequency of 48 kHz.
 *      OC1 module is configured to work with Timer3.
**      The timer period constant is computed using TMR_FREQ_SINE definitions (located in this source file)
**      and peripheral bus frequency definition (PB_FRQ, located in config.h).
 *      Timer3's interrupt is enabled with a priority of 7, and sub priority of 3
*/
void TONE_Init()
{      
    if(initRun == 0){
        TONE_ConfigurePins(); 
        PR3 = (int)((float)((float)PB_FRQ/TMR_FREQ_SINE) + 0.5); 

        TMR3 = 0;
        T3CONbits.TCKPS = 0;     //1:1 pre scale value
        T3CONbits.TGATE = 0;     //not gated input (the default)
        T3CONbits.TCS = 0;       //PCBLK input (the default)
        T3CONbits.ON = 0;        //turn on Timer3

        OC1CONbits.ON = 0;       // Turn off OC1 while doing setup.
        OC1CONbits.OCM = 6;      // PWM mode on OC1; Fault pin is disabled
        OC1CONbits.OCTSEL = 1;   // Timer3 is the clock source for this Output Compare module
        OC1CONbits.ON = 0;       // Start the OC1 module  

        IPC3bits.T3IP = 7;      // interrupt priority
        IPC3bits.T3IS = 3;      // interrupt sub priority
        IEC0bits.T3IE = 1;      // enable Timer3 interrupt    
        IFS0bits.T3IF = 0;      // clear Timer3 interrupt flag

        macro_enable_interrupts();  // enable interrupts at CPU
        initRun = 1;
    }
}

/* ------------------------------------------------------------ */
/***	TONE_ConfigurePins
**
**	Parameters:
**
**	Return Value:
**		
**	Description:
**		This function configures the output pin corresponding to TONE module (A_OUT) 
**      as digital output and maps it to OC1.
**      The function uses pin related definitions from config.h file.
**      This is a low-level function called by TONE_Init(), so user should avoid calling it directly.           
*/
void TONE_ConfigurePins()
{
    // Configure AUDIO output as digital output.
    tris_A_OUT = 0;    
    rp_A_OUT = 0x0C; // 1100 = OC1
    // disable analog (set pins as digital)
    ansel_A_OUT = 0;
}

/* ------------------------------------------------------------ */
/***	TONE_Start
**
**	Parameters:
**      int frequency - the frequency in Hz to be played by the speaker. 
 *      int ms - the number of milliseconds to play the sound for. 
 * 
**	Return Value:
**		
**	Description:
**		This function plays a frequency on the speaker.
**      The frequency is limited to 300-6000Hz because the speaker sounds terrible outside this
 *      range. If using the line out, this can be removed, and the samples array
 *      can be expanded to accommodate lower frequencies.
**      Providing an ms of -1 will play the tone indefinitely until TONE_Close() is called.         
*/
void TONE_Start(int frequency, int ms){
    if(initRun == 0){ // check if init has been run yet
        TONE_Init();
    }
    TONE_Close();
    if(frequency > 6000){ //keep frequency from sounding terrible through on board speaker
        frequency = 6000;
    }
    else if(frequency < 300){
        frequency = 300;
    }
    RGSIN_SIZE = 1.0/((float)frequency/(float)TMR_FREQ_SINE)+1; //number of samples played at 48kHz
    int j = 0;
    for(; j < RGSIN_SIZE; j++){
        //Generate sine wave lookup table assuming played back with an increment of 1 at 48kHz
        //Sine wave values are +/-256 shifted up 256 (+1 to account for rounding error) to 0-512
        //The Timer3 ISR scales up by 4 to increase amplitude
        rgSinSamples[j] = (unsigned short)(256 * sinf(2.0f * M_PI * (float)j / RGSIN_SIZE)+257);
    }
    
    T3CONbits.ON = 1;       // turn on Timer3
    OC1CONbits.ON = 1;      // Turn on OC1
    
    int counter = 0;
    for (; counter < ms; counter++){
        int i = 0;
        for(; i < 1426; i++){}   //software delay 1 millisecond
    }
    if(ms >= 0){
        TONE_Close();
    }
}

/***	TONE_Close
**
**	Parameters:
** 
**	Return Value:
**      
**	Description:
**		This functions releases the hardware involved in TONE library: 
**      it turns off the Timer3 and OC1 modules.        
*/
void TONE_Close()
{
        T3CONbits.ON = 0;       // turn off Timer3
        OC1CONbits.ON = 0;      // Turn off OC1
        
}
/* *****************************************************************************
 End of File
 */
