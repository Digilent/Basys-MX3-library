/* 
 * File:   main.c
 * Author: Monica Ignat
 *
 * Created on March 30, 2017
 */

#include <stdio.h>
#include <stdlib.h>
#include <xc.h>
#include <sys/attribs.h>
#include "config.h"
#include "lcd.h"
#include "led.h"
#include "btn.h"
#include "swt.h"


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

void BasicIODemo();

int main(int argc, char** argv) 
{
    BasicIODemo();       
    return (1);
}
/***	BasicIODemo
**
**	Parameters:
**		
**
**	Return Value:
**		
**
**	Description:
**		This function initializes the IO basic pins and calls specific functions from the following libs: LED, SWT, BTN, LCD, RGBLed
**      The Init functions for each module are called and bit and group specific functions are used
**          
*/
void BasicIODemo(){
    //libraries initialization for the used IO modules
    LCD_Init(); 
    LED_Init(); 
    SWT_Init();
    BTN_Init();
    RGBLED_Init();
    //welcome message displayed on the LCD screen
    LCD_WriteStringAtPos("BasicIO Demo", 0, 0);
    LCD_WriteStringAtPos("Digilent", 1, 0);
    while(1)
    {    
        //set the LEDs value according to the SWT positions: LED is lit when the corresponding SWT is on
        //usage of the Set and Get group Value functions for both SWT and LED libraries
        LED_SetGroupValue(SWT_GetGroupValue());
        //usage of the GetValue function of the SWT library and the SetValue function of the RGBLed library
        if (BTN_GetValue(0))
        {
            //set Red color if SW0 is on - not the maximum value, 255, but 200, close
            RGBLED_SetValue(200,0,0);
        }
        if (BTN_GetValue(1))
        {
            //set Green color if SW0 is on
            RGBLED_SetValue(0,200,0);
        }
        if (BTN_GetValue(2))
        {
            //set Blue color if SW0 is on
            RGBLED_SetValue(0,0,200);
        }      
    }    
}