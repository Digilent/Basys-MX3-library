/* 
 * File:   main.c
 * Author: Cristian Fatu
 *
 * Created on April 12, 2017
 */

#include <stdio.h>
#include <stdlib.h>
#include <xc.h>
#include <sys/attribs.h>
#include "config.h"

#include "lcd.h"
#include "ssd.h"
#include "spiflash.h"


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

/***	SPIFLASH_Demo
**
**	Parameters:
**		
**
**	Return Value:
**		
**
**	Description:
**		This demo function initializes the SPIFLASH, LCD, SSD modules.
**      It erases all SPIFLASH, writes one page of 256 bytes at a specific address, 
**      reads the same page into a buffer.
**      In a loop, it displays on LCD the address inside the page and the data that was read.
**      It also displays on SSD the address inside the page, as a hexadecimal value.
**          
*/
void SPIFLASH_Demo()
{
    unsigned char spiFlashPageW[SPIFLASH_PROG_SIZE]; // 256 bytes
    unsigned char spiFlashPageR[SPIFLASH_PROG_SIZE]; // 256 bytes
    unsigned char valSpiFlash;
    float rgACLGVals[3];  
    char strMsg[80];    
    unsigned int wCnt = 0, baseCnt = 0, valPot, addrSpiFlash, addrInPage;
    LCD_Init(); 
    LCD_WriteStringAtPos("SPI Flash test", 0, 0);
    LCD_WriteStringAtPos("Erase Flash ...", 1, 0);
    SPIFLASH_Init();
        // erase all flash
    SPIFLASH_EraseAll();
    // initialize the write and read buffers 
    for(addrInPage = 0; addrInPage < SPIFLASH_PROG_SIZE; addrInPage++)
    {
        spiFlashPageW[addrInPage] = addrInPage << 1; // address multiplied by 2
        spiFlashPageR[addrInPage] = 0xFF;           // this should be overwritten by read values
    }
    LCD_WriteStringAtPos("Write buffer ...", 1, 0);
    SPIFLASH_ProgramPage(SPIFLASH_PROG_ADDR, spiFlashPageW, SPIFLASH_PROG_SIZE);

    LCD_WriteStringAtPos("Read buffer ...", 1, 0);
    SPIFLASH_Read(SPIFLASH_PROG_ADDR, spiFlashPageR, SPIFLASH_PROG_SIZE);
    SSD_Init();    

    while(1)
    {
        if(++baseCnt == 500000)
        {

            baseCnt = 0;        
//            addrInPage = wCnt & 0xFF;                         // available for SPIFLASH_PROG_SIZE = 256
//            addrInPage = wCnt & (SPIFLASH_PROG_SIZE - 1);     // available for SPIFLASH_PROG_SIZE power of 2
            addrInPage = wCnt % SPIFLASH_PROG_SIZE;                     
            sprintf(strMsg, "ADDR:%02d  Val:%03d", addrInPage, spiFlashPageR[addrInPage]);        
            LCD_WriteStringAtPos(strMsg, 1, 0);     
            SSD_WriteDigitsGrouped(wCnt++, 0);
        }
    }    
}

int main(int argc, char** argv) 
{
    SPIFLASH_Demo();
    return (1);
}