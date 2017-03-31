/* 
 * File:   main.c
 * Author: Monica Ignat
 *
 * Created on March 31, 2017
 * 
 */

#include <stdio.h>
#include <stdlib.h>
#include <xc.h>
#include <sys/attribs.h>
#include "config.h"

#include "lcd.h"
#include "ssd.h"
#include "btn.h"


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

void SSDDemo();

int main(int argc, char** argv) 
{
    SSDDemo();   
    return (1);
}

/***	SSDDemo
**
**	Parameters:
**		
**
**	Return Value:
**		
**
**	Description:
**		This function initializes the SSD pins and calls specific functions for initializing LCD, BTN and SSD modules.
**      The 4 digits display a counter values and the BTNC button resets the counter
**          
*/
void SSDDemo()
{
    unsigned int wCnt = 0, baseCnt = 0;
    LCD_Init(); 
    SSD_Init();
    BTN_Init();
    LCD_WriteStringAtPos("SSD Demo", 0, 0);
    LCD_WriteStringAtPos("Digilent", 1, 0);

    while(1)
    {
       if(++baseCnt == 1000000)
        {
            //display the counter values on the 4 digits available
            SSD_WriteDigitsGrouped(wCnt++, 0);
            baseCnt = 0;        
            //if button C is pressed, reset the counter
            if (BTN_GetValue(2))
            {
                wCnt=0;
            }
        }
    }    
}
