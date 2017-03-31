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
#include "aic.h"
#include "adc.h"


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

void AICDemo();

int main(int argc, char** argv) 
{
    AICDemo();       
    return (1);
}
/***	AICDemo
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
AICDemo(){
    //libraries initialization for the used IO modules
    char strMsg[80];    
    int valPot;
    LCD_Init(); 
    AIC_Init(); 
    while(1)
    {    
        valPot = AIC_Val();
        sprintf(strMsg, "Pot:%04d", valPot);
        LCD_WriteStringAtPos(strMsg, 0, 0);
        LCD_WriteStringAtPos("AIC Demo", 1, 0);
    }    
}