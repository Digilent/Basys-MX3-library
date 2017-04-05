/* 
 * File:   main.c
 * Author: Monica Ignat
 *
 * Created on April 3rd, 2017
 */

#include <stdio.h>
#include <stdlib.h>
#include <xc.h>
#include <sys/attribs.h>
#include "config.h"
#include "uart.h"
#include "lcd.h"
#include "uartjb.h"


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

void UARTDemoInt();
void UARTDemoPoll();
void UARTDemoPmod();

int main(int argc, char** argv) 
{
    //use either one of the functions to communicate via UART interface: interrupt based or polling
    //UARTDemoInt();  
    //UARTDemoPoll();
    //use the UARTJB lib to communicate via PmodJB using UART
    UARTDemoPmod();
    return (1);
}
/***	UARTDemoInt
**
**	Parameters:
**		
**
**	Return Value:
**		
**
**	Description:
**		This function initializes the LCD and UART interface and calls specific UART interrupt based functions 
**      to send and receive a string to/from the terminal
**          
*/
void UARTDemoInt(){
    
    char strTxti[cchRxMax];   
    unsigned char cchi;
    //UART and LCD basic initializations
    UART_Init(9600); 
    LCD_Init();
    //send a text to UART terminal on PC
    UART_PutString("UART Demo \n\r");
    UART_PutString("Send a text file\n\r");
    LCD_WriteStringAtPos(" UART Demo Int", 0, 0);
    LCD_DisplayClear();
    //when sending a text file, make sure the content ends with CR+LF and your terminal is also setup accordingly, to transmit both characters
    //otherwise, the file sending will not function correctly
    while(1){    
        cchi = UART_GetString(strTxti, cchRxMax);
        if(cchi > 0)
        {
          //display the received string on the LCD
          LCD_WriteStringAtPos(strTxti, 0, 0);
          LCD_WriteStringAtPos("UART Int Demo", 1, 0);
          //send the content of the file back to UART to be visualized
          UART_PutString(strTxti);
        }  
    }    
}
/***	UARTDemoPoll
**
**	Parameters:
**		
**
**	Return Value:
**		
**
**	Description:
**		This function initializes the LCD and UART interface and calls specific UART polling based functions 
**      to send and receive a character to/from the terminal
**          
*/
void UARTDemoPoll()
{
    char strTxtp[cchRxMax];   
    unsigned char cch;
    //UART and LCD basic initializations
    UART_InitPoll(9600); 
    LCD_Init();
    //send a text to UART terminal on PC
    UART_PutString("UART Demo \n\r");
    UART_PutString("Send a char to UART\n\r");
    //write to the LCD as well
    LCD_WriteStringAtPos(" UART Demo Poll", 0, 0);
    LCD_DisplayClear();
    while(1)
    {    
        LCD_DisplayClear();
        cch = UART_GetCharPoll();
        //use the LCD display to visualize the received chars
        sprintf(strTxtp, " Received %c\n", cch);
        LCD_WriteStringAtPos(strTxtp, 1, 0);
        //send the char back to UART
        UART_PutChar(cch);
    }
}
/***	UARTDemoPmod
**
**	Parameters:
**		
**
**	Return Value:
**		
**
**	Description:
**		This function initializes the LCD and UART interface and calls specific UART polling based functions 
**      to send and receive a character to/from the terminal
**          
*/
void UARTDemoPmod()
{
    char strTxt[cchRxMax];   
    unsigned char cch;
    //UARTJB and LCD basic initializations
    UARTJB_Init(9600); 
    LCD_Init();
    //send a text to UART terminal on PC using UARTJB library
    UARTJB_PutString("UART Demo \n\r");
    UARTJB_PutString("Send a text file\n\r");
    LCD_WriteStringAtPos(" UART Demo Pmod", 0, 0);
    LCD_DisplayClear();
    //when sending a text file, make sure the content ends with CR+LF and your terminal is also setup accordingly, to transmit both characters
    //otherwise, the file sending will not function correctly
    while(1){  
        
        cch = UARTJB_GetString(strTxt, cchRxMax);
        if(cch > 0)
        {
           // LCD_DisplayClear();
          LCD_WriteStringAtPos(strTxt, 0, 0);
          LCD_WriteStringAtPos(" UART Demo Pmod", 1, 0);
          //send the content of the file back to UART to be visualized
          UARTJB_PutString(strTxt);
        }  
    }    
}
//the code below is used as part of the UARTDemoPmod function, when polling mode is needed
//replace the code above with the commented section below
/*char strTxtpmd[cchRxMax];   
    unsigned char cchp;
    //UART and LCD basic initializations
    UARTJB_InitPoll(9600); 
    LCD_Init();
    //send a text to UART terminal on PC
    UARTJB_PutString("UART Demo \n\r");
    UARTJB_PutString("Send a char to UART\n\r");
    //write to the LCD as well
    LCD_WriteStringAtPos(" UART Demo Pmod", 0, 0);
    
    while(1)
    {    
        cchp = UARTJB_GetCharPoll();
        //use the LCD display to visualize the received chars
        sprintf(strTxtpmd, "Received %c", cchp);
        LCD_WriteStringAtPos(strTxtpmd, 1, 0);
        //send the char back to UART
        UARTJB_PutChar(cchp);
    }
    }
     */