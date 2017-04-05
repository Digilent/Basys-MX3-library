/* 
 * File:   main.c
 * Author: Cristian Fatu
 *
 * Created on April 5, 2017
 */

#include <stdio.h>
#include <stdlib.h>
#include <xc.h>
#include <sys/attribs.h>
#include "config.h"
#include "spija.h"
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

//constants used to determine the value to be send to the DAC in order to obtain certain voltage values
//reference voltage
const float fRef = 3.3;
//DAC bits number
const int noBits = 8;
//control byte to be sent prior to the data byte in order to configure the two converters
#define DACSPI1_CTRL_BYTE	0x00	// selecting convertor DAC A to be written
void SPIJADemo();

int main(int argc, char** argv) 
{
    SPIJADemo();
    return (1);
}

/***	SPIJADemo
**
**	Parameters:
**		
**
**	Return Value:
**		
**
**	Description:
**		This function initializes the LCD and SPIJA modules, then outputs on the SPI Pmod connector digital values corresponding to
**      voltage values between 0 and 3V, with a constant step defined
**     
*/
void SPIJADemo()
{
    float dMaxValue = 3;
    float dMinValue = 0;
    float dStep = 0.05;
    float dValue;
    unsigned char bVal, bWrBytes[2], bRdBytes[2];
    
    float fConvConst = ((float)(1<<noBits))/ fRef;
    //perform modules initialization
    LCD_Init(); 
    SPIJA_Init(); 
    LCD_WriteStringAtPos("SPIJA Demo",0 ,0);
    while(1)
    {
        for(dValue = dMinValue; dValue < dMaxValue; dValue+= dStep)
        {
            //compute the value to be sent to the DA module. PmodDA1 has been used for this demo
            bVal = (unsigned char)(dValue * fConvConst);
            bWrBytes[0] = DACSPI1_CTRL_BYTE; // selecting convertor DAC A to be written
            bWrBytes[1] = bVal;              // value corresponding to the voltage to be represented
            // send bVal over SPI
            SPIJA_TransferBytes(2, bRdBytes, bWrBytes);
        }
    }
}    
