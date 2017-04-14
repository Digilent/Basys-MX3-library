/* 
 * File:   main.c
 * Author: Cristian Fatu
 *
 * Created on April 13, 2017
 */

#include <stdio.h>
#include <stdlib.h>
#include <xc.h>
#include <sys/attribs.h>
#include "config.h"

#include "mot.h"
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

int main(int argc, char** argv) 
{
    LCD_Init(); 
    LCD_WriteStringAtPos("Welcome to ", 0, 0);    
    LCD_WriteStringAtPos("        BasysMX3", 1, 0);    
    return (1);
}