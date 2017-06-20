/* 
 * File:   main.c
 * Author: Cristian Fatu
 *
 * Created on March 10, 2017
 * Modified by Monica Ignat on March 31st, 2017
 */

#include <stdio.h>
#include <stdlib.h>
#include <xc.h>
#include <sys/attribs.h>
#include "config.h"

#include "lcd.h"
#include "pmodtmp3.h"



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


void Tmp3Demo();

int main(int argc, char** argv) 
{
    Tmp3Demo();   
    return (1);
}



/* ------------------------------------------------------------ */

/***	Tmp3Demo
**
**	Parameters:
**		
**
**	Return Value:
**		
**
**	Description:
**		This function configures the PmodTMP3 and calls specific function for reading the temperature value.
**      The float value of temperature, in Celsius, is displayed on the LCD
**          
*/
void Tmp3Demo(){
    float fVal;  
    char strMsg[80];    
    unsigned int baseCnt = 0;
    LCD_Init(); 
    LCD_WriteStringAtPos("PmodTMP3 Demo", 0, 0);
    LCD_WriteStringAtPos("Digilent", 1, 0);
    PMODTMP3_Init(TMP3_RES12);
    while(1)
    {
       //perform temperature readings only once in a while, to be able to visualize the results
        if(++baseCnt == 400000)
       {
            baseCnt = 0;        
            fVal = PMODTMP3_GetTemp();
            //display on the LCD screen the temperature value, on second row
            sprintf(strMsg, "Temp = %6.3f  C", fVal);
            strMsg[14] = (unsigned char)223;    // extended ASCII code for degree sign
            LCD_WriteStringAtPos(strMsg, 1, 0);     
        }
    }  
}