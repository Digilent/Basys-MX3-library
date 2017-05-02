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
**      The Init functions for each module are called and bit and group specific functions are used.
**      The LCD displays the status of the buttons.
**      The LEDs are lit according to the switches position.
**      The RGBLed is driven according to the Switches 0-2 position.
**          
*/
void BasicIODemo(){
    char strMsg[80];
    char strPressed[17];
    char strNotPressed[17];   
    const char strLet[6] = "ULCRD";
    unsigned int baseCnt = 0;
    unsigned int valBtn, j;
    
    //libraries initialization for the used IO modules
    LCD_Init(); 
    LED_Init(); 
    SWT_Init();
    BTN_Init();
    RGBLED_Init();
    //welcome message displayed on the LCD screen
    LCD_WriteStringAtPos("BasicIO Demo 2", 0, 0);
    LCD_WriteStringAtPos("Digilent", 1, 0);
    LCD_DisplayClear();
    while(1)
    {
        LED_SetGroupValue(SWT_GetGroupValue());
         //write on the LCD the buttons that are pressed or released
        strcpy(strPressed, "Pressed:        ");
        strcpy(strNotPressed, "Released:       ");
        for(j = 0; j<5; j++)
        {
            //usage for the GetValue function of the BTN library
            valBtn = BTN_GetValue(j);
            if(valBtn)
            {
                strPressed[11+j] = strLet[j];
            }
            else
            {
                strNotPressed[11+j] = strLet[j];
            }                    
        }

        LCD_WriteStringAtPos(strPressed, 0, 0);
        LCD_WriteStringAtPos(strNotPressed, 1, 0);
        //usage of the GetValue function of the SWT library and the SetValue function of the RGBLed library
        if (SWT_GetValue(0))
        {
            //set Red color if SW0 is on - not the maximum value, 255, but 200, close
            RGBLED_SetValue(200,0,0);
        }
        if (SWT_GetValue(1))
        {
            //set Green color if SW0 is on
            RGBLED_SetValue(0,200,0);
        }
         if (SWT_GetValue(2))
        {
            //set Blue color if SW0 is on
             RGBLED_SetValue(0,0,200);
        }      
    }    
}