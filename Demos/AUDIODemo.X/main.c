/* 
 * File:   main.c
 * Author: Cristian Fatu
 *
 * Created on April 04, 2017
 */

#include <stdio.h>
#include <stdlib.h>
#include <xc.h>
#include <sys/attribs.h>
#include "config.h"
#include "audio.h"
#include "led.h"
#include "swt.h"
#include "lcd.h"

#pragma config JTAGEN = OFF     
#pragma config FWDTEN = OFF      


/* ------------------------------------------------------------ */
/*						Configuration Bits		                */
/* ------------------------------------------------------------ */


// Device Config Bits in  DEVCFG1:	
#pragma config FNOSC =	FRCPLL
#pragma config FSOSCEN =	OFF
#pragma config POSCMOD =	XT
#pragma config OSCIOFNC =	ON
#pragma config FPBDIV =     DIV_2

// Device Config Bits in  DEVCFG2:	
#pragma config FPLLIDIV =	DIV_2
#pragma config FPLLMUL =	MUL_20
#pragma config FPLLODIV =	DIV_1


void AudioDemo();

int main(int argc, char** argv) 
{
//    BasicIODemo();       
//    SSDDemo();
    AudioDemo();
    return (1);
}


/***	AudioDemo
**
**	Parameters:
**		
**
**	Return Value:
**		
**
**	Description:
**		This function initializes the LCD, SSD, LED, SWT modules.
**      It reads the switches. The functions allocated to each switch are:
**      Switch 0 - start generating a sin wave on the audio output.
**      Switch 1 - start "mirror": the MIC is read and the value is outputed "live" to the audio output.
**      Switch 2 - start recording from MIC. The samples are stored in a circular manner in a buffer.
**      Switch 3 - start playing the samples recorded from MIC. The samples are read from the buffer in a circular manner, and sent to the Audio output
**      Every time a switch from the ones presented above is turned on, the corresponding LED is turned on.
**      Every time a switch from the ones presented above is turned off, the corresponding LED is turned off and the audio module is turned off.
**          
*/
void AudioDemo()
{
    unsigned int baseCnt = 0;
    LCD_Init(); 
    SWT_Init();
    LED_Init();
    LCD_WriteStringAtPos("Audio Demo", 0, 0);
    LCD_WriteStringAtPos("Digilent", 1, 0);
                
    while(1)
    {
       if(++baseCnt == 500000) // ensure timing by dividing the frequency of while loops
        {
            baseCnt = 0;
            
            // check the switches status
            if(SWT_GetValue(0))
            {
                if(AUDIO_GetAudioMode() != 0)
                {    
                    LED_SetValue(0, 1);
                    AUDIO_Init(0);  // start play sin
                }
            }
            else
            {
                LED_SetValue(0, 0);
                if(AUDIO_GetAudioMode() == 0)
                {    
                    AUDIO_Close();  // close play sin
                }
            }
            if(SWT_GetValue(1))
            {
                if(AUDIO_GetAudioMode() != 1)
                {    
                    LED_SetValue(1, 1);
                    AUDIO_Init(1);  // start mirror
                }
            }
            else
            {
               LED_SetValue(1, 0);
                if(AUDIO_GetAudioMode() == 1)
                {    
                    AUDIO_Close();  // close mirror
                }               
            }
            if(SWT_GetValue(2))
            {
                if(AUDIO_GetAudioMode() != 2)
                {    
                    LED_SetValue(2, 1);
                    AUDIO_Init(2);  // start record
                }
            }
            else
            {
                LED_SetValue(2, 0);
                if(AUDIO_GetAudioMode() == 2)
                {    
                    AUDIO_Close();  // close record
                }              
            }
            if(SWT_GetValue(3))
            {
                if(AUDIO_GetAudioMode() != 3)
                {    
                    LED_SetValue(3, 1);
                    AUDIO_Init(3);  // start play recorded
                }
            }
            else
            {
                LED_SetValue(3, 0);
                if(AUDIO_GetAudioMode() == 3)
                {    
                    AUDIO_Close();  // close play recorded
                }              
            }         
       }
    }
}    
