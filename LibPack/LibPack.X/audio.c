
/* ************************************************************************** */
/** Descriptive File Name

  @Company
    Digilent

  @File Name
    audio.c

  @Description
        This file groups the functions that implement the AUDIO library.
        The library implements generating audio using a sine of a certain frequency,
        recording and playback functionality.
        In order to generate audio using a sine of a certain frequency, the sine values 
        are produced using PWM on OC1.
        Include the file in the project. together with config.h when this library is needed.	

  @Author
    Cristian Fatu 
    cristian.fatu@digilent.ro
 */
/* ************************************************************************** */

/* ************************************************************************** */
/* ************************************************************************** */
/* Section: Included Files                                                    */
/* ************************************************************************** */
#include <xc.h>
#include <sys/attribs.h>
#include "config.h"
#include "audio.h"
#include "mic.h"
#include "led.h"


#define RECORD_SIZE 2*30720
unsigned short rgAudioBuf[RECORD_SIZE];
unsigned short *pAudioSamples;

int cntAudioBuf, idxAudioBuf;

//#define PWM_FREQ    48000
//#define DUTY_CYCLE  10

//#define T3PER   1667 // 80000000/48000
//#define T3PER   417 // 40000000/192000


// http://www.daycounter.com/Calculators/Sine-Generator-Calculator.phtml

unsigned short rgPlay2[] = {
256,320,379,431,472,499,511,507,488,453,
406,350,288,224,162,106,59,24,5,1,
13,40,81,133,192
};
#define RGSIN_SIZE  (sizeof(rgPlay2) / sizeof(rgPlay2[0]))


#define TMR_FREQ_VOICE    20000 // 20 kHz
#define TMR_FREQ_SIN    48000 // 48 kHz

//#define RGSIN_SIZE  (sizeof(rgPlay2) / sizeof(unsigned int))
//#define T3PER   830 // 80000000/96kHz - 10.35 us -> 3.8 kHz sin
//#define T3PER_SIN   208 //10000
//#define T3PER_VOICE 625 //10000

//#define T3PER   1660 //1.93kHz

unsigned char bAudioMode = -1;
//  0   PlaySinus       T3PER_SIN
//  1   Mirror          T3PER_SIN
//  2   Record voice    T3PER_VOICE
//  3   Play voice      T3PER_VOICE

unsigned char GetAudioMode()
{
    return bAudioMode;
}

//void __ISR(_TIMER_3_VECTOR, ipl7AUTO) Timer3ISR(void) 
void __ISR(_TIMER_3_VECTOR, ipl7) Timer3ISR(void) 
{  
    unsigned short v;
//    LATDINV = (1<<9);   //debug - toggle RD9 (JB1)
//    if(++idxBuf == cntBuf)
//    {
//        idxBuf = 0;
//    }
    
    
    if(bAudioMode == 0)
    {   // play sinus
        // load sample values into OC register
        OC1RS = 4*pAudioSamples[(++idxAudioBuf) % cntAudioBuf];
    }
    if(bAudioMode == 1)
    {
        // mirror, get sample from MIC and throw it to speaker
        OC1RS = (MIC_Val()*PR3) >> 10;
//        OC1RS = MIC_GetSample();
    }
    if(bAudioMode == 2)
    {
        // record voice
//        pAudioSamples[(idxAudioBuf++) % cntAudioBuf] = (MIC_GetSample()*842) >> 10;
        pAudioSamples[(idxAudioBuf++) % cntAudioBuf] = MIC_Val();
    }
    if(bAudioMode == 3)
    {
        // play voice
        v = pAudioSamples[(++idxAudioBuf) % cntAudioBuf];
//v >>= 1;
//        LED_SetGroupValue(v);
v <<= 2;
        OC1RS = v;

//        OC1RS = 0;
    }
    
    IFS0bits.T3IF = 0;      // clear Timer3 interrupt flag
}
/*
void __ISR(_TIMER_3_VECTOR, ipl7AUTO) Timer3ISR(void) 
{  
    LATDINV = (1<<9);   //debug - toggle RD9 (JB1)
    if(--cntBuf_HoldSample == 0)
    {
        // move to the next sample
        if(++idxBuf == cntBuf)
        {
            idxBuf = 0;
        }
//        if(fPlay)
        {
            // load values
//           LED_SetValue(3, 1);
            OC1RS = pAudioSamples[idxBuf]* 834 / 1024;
//            OC1RS = pAudioSamples[idxBuf % RECORD_SIZE];
//            cntBuf_HoldSample = pPlay_CntHoldSamples[idxBuf];
            cntBuf_HoldSample = HLD;
        }
        if(fRecord)
        {
            // acquire sample
            rgBuf[idxBuf % RECORD_SIZE] = MIC_GetSample();
            //LED_SetGroupValue(rgBuf[idxBuf % RECORD_SIZE] & 0xFF);
            LED_SetValue(2, 1);
            cntBuf_HoldSample = HLD;
        }
    }
    IFS0bits.T3IF = 0;      // clear Timer3 interrupt flag
}
*/
void AUDIO_InitPlayBack(unsigned short *pAudioSamples1, int cntBuf1)
{
        // init playback

    idxAudioBuf = 0;
    cntAudioBuf = cntBuf1;
    pAudioSamples = pAudioSamples1;

    // load first value
    OC1RS = pAudioSamples[0];

}



void AUDIO_InitRecord(unsigned short *pAudioSamples1, int cntBuf1)
{
    cntAudioBuf = cntBuf1;
    idxAudioBuf = 0;
    pAudioSamples = pAudioSamples1;

    OC1RS = 0;
}
void AUDIO_Init(unsigned char bMode)
{   
    bAudioMode = bMode;
    AUDIO_ConfigurePins();

    INTDisableInterrupts();             // disable interrupts at CPU
  
    // configure Timer3
    
    switch (bAudioMode)
    {
        case 0:
            // play sinus
                PR3 = (int)((float)((float)PB_FRQ/TMR_FREQ_SIN) + 0.5);               
                AUDIO_InitPlayBack(rgPlay2, RGSIN_SIZE);
            break;
        case 1:
            // mirror
                PR3 = (int)((float)((float)PB_FRQ/TMR_FREQ_VOICE) + 0.5);  
            break;
        case 2:
            // record voice
                PR3 = (int)(((float)(float)PB_FRQ/TMR_FREQ_VOICE) + 0.5);  
                AUDIO_InitRecord(rgAudioBuf, RECORD_SIZE);
            break;        
        case 3:
            // playbback voice
                PR3 = (int)((float)((float)PB_FRQ/TMR_FREQ_VOICE) + 0.5);  
                AUDIO_InitPlayBack(rgAudioBuf, RECORD_SIZE);
            break;        
    }
    TMR3 = 0;
    T3CONbits.TCKPS = 0;                //            1:1 prescale value
    T3CONbits.TGATE = 0;                //             not gated input (the default)
    T3CONbits.TCS = 0;                  //             PCBLK input (the default)
    T3CONbits.ON = 1;                   //             turn on Timer3

    
    OC1CONbits.ON = 0;       // Turn off OC1 while doing setup.
    OC1CONbits.OCM = 6;      // PWM mode on OC1; Fault pin is disabled
    OC1CONbits.OCTSEL = 1;   // Timer3 is the clock source for this Output Compare module
    OC1CONbits.ON = 1;       // Start the OC1 module  
   


    IPC3bits.T3IP = 7;      // interrupt priority
    IPC3bits.T3IS = 3;      // interrupt subpriority
    IEC0bits.T3IE = 1;      // enable Timer3 interrupt    
    IFS0bits.T3IF = 0;      // clear Timer3 interrupt flag
    
    INTEnableSystemMultiVectoredInt();  // enable interrupts at CPU
}

// The opposite of AUDIO_Init
void AUDIO_Close()
{
        T3CONbits.ON = 0;       // turn off Timer3
        OC1CONbits.ON = 0;      // Turn off OC1
        
}


void AUDIO_ConfigurePins()
{
    // Configure AUDIO output as digital output.
    tris_A_OUT = 0;    
    
    rp_A_OUT = 0x0C; // 1100 = OC1
//rp_MOT_AIN2 = 0x0C; // AIN2 is set on OC1 TMP DEBUG
    
    // disable analog (set pins as digital)
    ansel_A_OUT = 0;
}


/* *****************************************************************************
 End of File
 */
