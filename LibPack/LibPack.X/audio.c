
/* ************************************************************************** */
/** Descriptive File Name

  @Company
    Digilent

  @File Name
    audio.c

  @Description
        This file groups the functions that implement the AUDIO library.
        The library implements functionality for generating audio using a sine of a certain frequency,
        recording and playback functionality.
        The library supports the following modes:
    Mode 0 (Generate sound using sine) - Generates sound using a sine wave. Features:
        - A fix number of sine values, covering one sine period, are stored in an array.
        - Using the sine values, PWM is produced using OC1, at a frequency of 48 kHz given by Timer3.
    Mode 1 (mirror) - Acquires samples from MIC and generates Audio output accordingly, live. Features:
        - MIC is sampled using the MIC library at a frequency of 16 kHz given by Timer3 
        - According to each acquired sample, the audio out is generated using OC1, at a frequency of 16 kHz given by Timer3.
     Mode 2 (record) - Records values acquired from MIC, saving them into a buffer. Features:
        - MIC is sampled using the MIC library at a frequency of 16 kHz given by Timer3 
        - the sampled values are placed in rgAudioBuf (which is a large static character array). 
     Mode 3 (play recorded) - Plays the values from a buffer to the Audio output. Features:
        - This function uses the samples from rgAudioBuf (which is a large static character array), stored with Record mode.
        - According to each acquired sample, the audio out is generated using OC1, at a frequency of 16 kHz given by Timer3.
    
        Include the file in the project. together with config.h, mic.c, adc.c, mic.h, adc.h when this library is needed.
        The library instantiates the rgAudioBuf, which is a large static character array, so including this library in the project uses a lot of memory. 

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


#define RECORD_SIZE 2*30720

// the array of samples, to be stored when recording (Mode 2) and to be played when playing back (Mode 3).
unsigned short rgAudioBuf[RECORD_SIZE];
unsigned short *pAudioSamples;

// global variables that store audio buffer position and size
int cntAudioBuf, idxAudioBuf;

// global variable that stores the mode for the AUDIO library
//  0   Generate sound using sine       TMR_FREQ_SINE
//  1   Mirror         TMR_FREQ_SOUND
//  2   Record sound   TMR_FREQ_SOUND
//  3   Play sound     TMR_FREQ_SOUND
unsigned char bAudioMode = -1;

// This array contains the values that implement one syne period, over 25 samples. 
// They are generated using this site: 
// http://www.daycounter.com/Calculators/Sine-Generator-Calculator.phtml
unsigned short rgSinSamples[] = {
256,320,379,431,472,499,511,507,488,453,
406,350,288,224,162,106, 59, 24,  5,  1,
 13, 40, 81,133,192
};
#define RGSIN_SIZE  (sizeof(rgSinSamples) / sizeof(rgSinSamples[0]))

// the definitions for the SOUND and SINE frequencies, to be used when Timer3 is configured in specific modes. 
#define TMR_FREQ_SOUND   16000 // 16 kHz
#define TMR_FREQ_SINE   48000 // 48 kHz


/* ------------------------------------------------------------ */
/***	Timer3ISR
**
**	Description:
**		This is the interrupt handler for Timer3. According to each mode, it is called at specific frequencies, as initialized in AUDIO_Init.
    Mode 0 (Generate sound using sine) - Advance current index in the sine definition buffer, initialize OC1 with the current sine definition value.
    Mode 1 (mirror)- MIC is sampled using the MIC library. The acquired value is used to initialize OC1. 
    Mode 2 (record) - MIC is sampled using the MIC library. The acquired value is stored in rgAudioBuf. 
    Mode 3 (play recorded) - Advance current index in rgAudioBuf, initialize OC1 with the current value from rgAudioBuf.
**          
*/
void __ISR(_TIMER_3_VECTOR, IPL7AUTO) Timer3ISR(void) 
{  
    unsigned short v;
    
    
    if(bAudioMode == 0)
    {   // play sine
        // load sine value into OC register
        OC1RS = 4*pAudioSamples[(++idxAudioBuf) % cntAudioBuf];
    }
    if(bAudioMode == 1)
    {
        // mirror, get sample from MIC and throw it to speaker
        OC1RS = (MIC_Val()*PR3) >> 10;
    }
    if(bAudioMode == 2)
    {
        // record sound
        // store the value acquired from MIC into the record buffer.
        pAudioSamples[(idxAudioBuf++) % cntAudioBuf] = MIC_Val();
    }
    if(bAudioMode == 3)
    {
        // play sound
        // retrieve value from the record buffer.
        v = pAudioSamples[(++idxAudioBuf) % cntAudioBuf];
        // amplify the value
        v <<= 2;
        // load buffer value into OC register
        OC1RS = v;

    }
    
    IFS0bits.T3IF = 0;      // clear Timer3 interrupt flag
}


/* ------------------------------------------------------------ */
/***	AUDIO_Init
**
**	Parameters:
**		unsigned char bMode - the mode for the Audio module
**              0 - Generate sound using a sine wave. 
**              1 - Mirror. 
**              2 - Record. 
**              3 - Play recorded. 
**
**	Return Value:
**		
**
**	Description:
**		This function initializes the AUDIO module, in the mode indicated by parameter bMode. 
**      The output pin corresponding to AUDIO module (A_OUT) is configured as digital output and is mapped to OC1.
**      According to the specified mode, Timer3 is set and specific initialization is performed:
**      Mode 0 (Generate sound using a sine wave) - Timer3 is initialized at a frequency of 48 kHz. The playback is initialized with the sine definition string.
**      Mode 1 (mirror) - The MIC module is initialized. Timer3  is initialized at a frequency of 16 kHz. 
**      Mode 2 (record) - The MIC module is initialized. Timer3 is initialized at a frequency of 16 kHz. The record is initialized with the large buffer rgAudioBuf.
**      Mode 3 (play recorded) - Timer3 is initialized at a frequency of 16 kHz. The playback is initialized with the large buffer rgAudioBuf, where eventually MIC values were stored.
**      OC1 module is configured to work with Timer3.
**      The timer period constant is computed using TMR_FREQ_SINE and TMR_FREQ_SOUND definitions (located in this source file)
**      and peripheral bus frequency definition (PB_FRQ, located in config.h).
*/
void AUDIO_Init(unsigned char bMode)
{   
    // close the timer and OC if the AUDIO_Init function is called when the AUDIO is already initialized
    if(bAudioMode != -1)
    {
        AUDIO_Close();
    }
    bAudioMode = bMode;
    AUDIO_ConfigurePins();
    // configuration is specific to each mode: 
    switch (bAudioMode)
    {
        case 0:
            // play sine
            PR3 = (int)((float)((float)PB_FRQ/TMR_FREQ_SINE) + 0.5);               
            AUDIO_InitPlayBack(rgSinSamples, RGSIN_SIZE);
            break;
        case 1:
            // mirror
            MIC_Init();
            PR3 = (int)((float)((float)PB_FRQ/TMR_FREQ_SOUND) + 0.5);  
            break;
        case 2:
            // record sound           
            MIC_Init();
            PR3 = (int)(((float)(float)PB_FRQ/TMR_FREQ_SOUND) + 0.5);  
            AUDIO_InitRecord(rgAudioBuf, RECORD_SIZE);
            break;        
        case 3:
            // playback sound
            PR3 = (int)((float)((float)PB_FRQ/TMR_FREQ_SOUND) + 0.5);  
            AUDIO_InitPlayBack(rgAudioBuf, RECORD_SIZE);
            break;        
    }
    TMR3 = 0;
    T3CONbits.TCKPS = 0;     //1:1 prescale value
    T3CONbits.TGATE = 0;     //not gated input (the default)
    T3CONbits.TCS = 0;       //PCBLK input (the default)
    T3CONbits.ON = 1;        //turn on Timer3
 
    OC1CONbits.ON = 0;       // Turn off OC1 while doing setup.
    OC1CONbits.OCM = 6;      // PWM mode on OC1; Fault pin is disabled
    OC1CONbits.OCTSEL = 1;   // Timer3 is the clock source for this Output Compare module
    OC1CONbits.ON = 1;       // Start the OC1 module  

    IPC3bits.T3IP = 7;      // interrupt priority
    IPC3bits.T3IS = 3;      // interrupt subpriority
    IEC0bits.T3IE = 1;      // enable Timer3 interrupt    
    IFS0bits.T3IF = 0;      // clear Timer3 interrupt flag
    
    macro_enable_interrupts();  // enable interrupts at CPU
}

/* ------------------------------------------------------------ */
/***	AUDIO_ConfigurePins
**
**	Parameters:
**
**	Return Value:
**		
**
**	Description:
**		This function configures the output pin corresponding to AUDIO module (A_OUT) 
**      as digital output and maps it to OC1.
**      The function uses pin related definitions from config.h file.
**      This is a low-level function called by AUDIO_Init(), so user should avoid calling it directly.           
*/
void AUDIO_ConfigurePins()
{
    // Configure AUDIO output as digital output.
    tris_A_OUT = 0;    
    rp_A_OUT = 0x0C; // 1100 = OC1
    // disable analog (set pins as digital)
    ansel_A_OUT = 0;
}

/* ------------------------------------------------------------ */
/***	AUDIO_GetAudioMode
**
**	Parameters:
**
**	Return Value:
**		unsigned char   - the current audio mode
**
**	Description:
**		This function returns the audio mode that was selected when the AUDIO was initialized.
**      
**          
*/
unsigned char AUDIO_GetAudioMode()
{
    return bAudioMode;
}


/* ------------------------------------------------------------ */
/***	AUDIO_InitPlayBack
**
**	Parameters:
**
**	Return Value:
**		unsigned short *pAudioSamples1  - pointer to a buffer where the 2 bytes values to be played are stored.
**		int cntBuf1                     - the buffer dimension (the number of values that are stored in the buffer).
**
**
**	Description:
**		This function initializes the playback buffer, by providing a pointer to a buffer where the 2 bytes values to be played are stored and the buffer dimension.
**      This function is called by by AUDIO_Init(), when Mode 3 is initialized.
**          
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


/* ------------------------------------------------------------ */
/***	AUDIO_InitRecord
**
**	Parameters:
**
**	Return Value:
**		unsigned short *pAudioSamples1  - pointer to a buffer where the recorded 2 bytes values will be stored.
**		int cntBuf1                     - the buffer dimension (the number of values that can be stored in the buffer).
**
**
**	Description:
**		This function initializes the record buffer, by providing a pointer to pointer to a buffer where the recorded 2 bytes values will be stored and the buffer dimension.  
**      This function is called by by AUDIO_Init(), when Mode 2 is initialized.
**      
**          
*/
void AUDIO_InitRecord(unsigned short *pAudioSamples1, int cntBuf1)
{
    cntAudioBuf = cntBuf1;
    idxAudioBuf = 0;
    pAudioSamples = pAudioSamples1;

    OC1RS = 0;
}

/***	AUDIO_Close
**
**	Parameters:
** 
**
**	Return Value:
**      
**
**	Description:
**		This functions releases the hardware involved in AUDIO library: 
**      it turns off the Timer3 and OC1 modules.
**      
**          
*/
void AUDIO_Close()
{
        T3CONbits.ON = 0;       // turn off Timer3
        OC1CONbits.ON = 0;      // Turn off OC1
        
}

/* *****************************************************************************
 End of File
 */
