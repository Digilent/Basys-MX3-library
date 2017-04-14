/* 
 * File:   main.c
 * Author: Cristian Fatu
 *
 * Created on March 10, 2017
 */

#include <stdio.h>
#include <stdlib.h>
#include <xc.h>
#include <sys/attribs.h>


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

#define SPIFLASH_PROG_SIZE  256
#define SPIFLASH_PROG_ADDR  0x100

void MOT_Demo()
{
    MOT_Init(1);
    LCD_Init(); 
    LCD_WriteStringAtPos("Motor Demo PH/EN", 0, 0);    
    LCD_WriteStringAtPos("1/128      0/255", 1, 0);    
    MOT_SetPhEnMotor1(1, 128);
    MOT_SetPhEnMotor2(0, 255); 
}

int main(int argc, char** argv) 
{
    MOT_Demo();
    return (1);
}