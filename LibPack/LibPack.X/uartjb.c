/* ************************************************************************** */
/** Descriptive File Name

  @Company
    Digilent

  @File Name
    uartjb.c

  @Description
        This file groups the functions that implement the UARTJB library, 
        This library  implements UART1 interface over the PMODB pins:
        JB2 (RD11) is mapped as TX1, JB3 (RD10) is mapped as RX1.
        It provides basic functions to configure UART and transmit / receive functions. 
        The receive can be done both with interrupts and with polling method. 
        Include the file in the project, together with config.h, when this library is needed.		

  @Author
    Cristian Fatu 
    cristian.fatu@digilent.ro 
 */
/* ************************************************************************** */
/* Version control
 * 04/13/2017 Initial Version
 * 07/10/2017 Bugfix: Rename global variables so that UARTJB library can be properly used together with UART library
*/
/* ************************************************************************** */
/* ************************************************************************** */
/* Section: Included Files                                                    */
/* ************************************************************************** */
#include <xc.h>
#include <sys/attribs.h>
#include "config.h"
#include "uartjb.h"

volatile unsigned char fRxDoneJB;
volatile unsigned char ichRxJB;
char rgchRxJB[cchRxMax];
/* ************************************************************************** */

/* ------------------------------------------------------------ */
/***	Uart1Handler
**
**	Description:
**		This is the interrupt handler for UART1 RX. It is used to receive strings using interrupt method. 
**      The interrupt recognizes a string having up to cchRxMax - 2 characters, followed by a CRLF.
**      When available received bytes are found they are placed in a global string rgchRxJB.
**      When a carriage return and a line feed ("\r\n", CRLF) sequence is recognized, the interrupt handler 
**      flags the success using the fRxDoneJB global variable and then ignores the received bytes.
**          
*/
void __ISR(_UART_1_VECTOR, IPL7SRS) Uart1Handler (void)
{
	unsigned char bVal;
	
	//Read the Uart1 rx buffer while data is available
	while(U1STAbits.URXDA)
	{
		bVal = (unsigned char)U1RXREG;
		
		if(!fRxDoneJB)
	    {
	        // Do we have space to store another character?
	        if (cchRxMax > ichRxJB)
	        {
	            // Yes.
                rgchRxJB[ichRxJB] = bVal;
	            // Is this the last character of the command?
	            if(('\n' == rgchRxJB[ichRxJB] ) && ('\r' == rgchRxJB[ichRxJB-1]))
	            {
	                // Yes.
	                fRxDoneJB = 1;
	            }   
	
	            ichRxJB++;
	        }   
	        else
	        {
	            // No we don't have space to store anymore characters.
	            // Mark the command as complete.
	
	            fRxDoneJB = 1;
	        }   
    	}  
	}  
	//Clear the Uart1 interrupt flag.
	IFS1bits.U1RXIF = 0;
}

/***	UARTJB_Init
**
**	Parameters:
**		unsigned int baud - UART baud rate.
**                                     for example 115200 corresponds to 115200 baud			
**
**	Return Value:
**		
**
**	Description:
**		This function initializes the hardware involved in the UARTJB module, in 
**      the UART receive with interrupts mode.
**      The JB2 digital pin is configured as digital output, and mapped over U1TX.
**      The JB3 digital pin is configured as digital input, and mapped over U1RX.
**      The UART1 module of PIC32 is configured to work at the specified baud, no parity and 1 stop bit.
**      
**          
*/
void UARTJB_Init(unsigned int baud)
{
    UARTJB_ConfigurePins();
    UARTJB_ConfigureUartRXInt(baud);
}

/***	UARTJB_InitPoll
**
**	Parameters:
**		unsigned int baud - UART baud rate.
**                                     for example 115200 corresponds to 115200 baud			
**
**	Return Value:
**		
**
**	Description:
**		This function initializes the hardware involved in the UARTJB module, in 
**      the UART receive without interrupts (polling method).
**      The JB2 digital pin is configured as digital output, and mapped over U1TX.
**      The JB3 digital pin is configured as digital input, and mapped over U1RX.
**      The UART1 module of PIC32 is configured to work at the specified baud, no parity and 1 stop bit.
**      
**          
*/
void UARTJB_InitPoll(unsigned int baud)
{
    UARTJB_ConfigurePins();
    UARTJB_ConfigureUart(baud);
}

/***	UARTJB_ConfigureUart
**
**	Parameters:
**		unsigned int baud - UART baud rate.
**                                     for example 115200 corresponds to 115200 baud
**
**	Return Value:
**		
**
**	Description:
**		This function configures the UART1 hardware interface of PIC32, according 
**      to the provided baud rate, with no interrupts
**      In order to compute the baud rate value, it uses the peripheral bus frequency definition (PB_FRQ, located in config.h)
**      
**          
*/
void UARTJB_ConfigureUart(unsigned int baud)
{
    U1MODEbits.ON     = 0;
    U1MODEbits.SIDL   = 0;
    U1MODEbits.IREN   = 0; 
    U1MODEbits.RTSMD  = 0;
    U1MODEbits.UEN0   = 0; 
    U1MODEbits.UEN1   = 0;
    U1MODEbits.WAKE   = 0;
    U1MODEbits.LPBACK = 0; 
    U1MODEbits.ABAUD  = 0;
    U1MODEbits.RXINV  = 0; 
    U1MODEbits.PDSEL1 = 0; 
    U1MODEbits.PDSEL0 = 0; 
    U1MODEbits.STSEL  = 0;  

    
    U1MODEbits.BRGH   = 0; 

    U1BRG = (int)(((float)PB_FRQ/(16*baud) - 1) + 0.5); // add 0.5 just in order to implement the round using the floor approach

    U1STAbits.UTXEN    = 1;
    U1STAbits.URXEN    = 1;
    U1MODEbits.ON      = 1; 
    
}

/***	UARTJB_ConfigureUartRXInt
**
**	Parameters:
**		unsigned int baud - UART baud rate.
**                                     for example 115200 corresponds to 115200 baud
**
**	Return Value:
**		
**
**	Description:
**		This function configures the UART1 hardware interface of PIC32, according 
**      to the provided baud rate (by calling the UARTJB_ConfigureUart function)
**      and additionally configures the interrupt on RX.
**      
**          
*/
void UARTJB_ConfigureUartRXInt(unsigned int baud)
{
    macro_disable_interrupts;  // disable interrupts 

    UARTJB_ConfigureUart(baud);

    IPC7bits.U1IP = 7;
    IPC7bits.U1IS = 3;

	IFS1bits.U1RXIF = 0;    //Clear the Uart1 interrupt flag.
    IEC1bits.U1RXIE = 1;   // enable RX interrupt
    

    macro_enable_interrupts();  // enable interrupts 
}

/***	UARTJB_ConfigurePins
**
**	Parameters:
**		
**
**	Return Value:
**		
**
**	Description:
**		This function configures the digital pins involved in the UARTJB module: 
**      The JB2 digital pin is configured as digital output, and mapped over U1TX.
**      The JB3 digital pin is configured as digital input, and mapped over U1RX.
**      The function uses pin related definitions from config.h file.
**      This is a low-level function called by UARTJB_Init(), so user should avoid calling it directly. 
 **      
**          
*/
void UARTJB_ConfigurePins()
{
    //JB2
    tris_UARTJB_TX = 0;   //TX digital output
    rp_UARTJB_TX = 3;     // 0011 U1TX
    
    //JB3
    tris_UARTJB_RX = 1;   //RX digital input
    rp_UARTJB_RX = 3;     // 0011 RD10
}

/***	UARTJB_PutChar
**
**	Parameters:
**          char ch -   the character to be transmitted over UART.
**
**	Return Value:
**		
**
**	Description:
**		This function transmits a character over UART1. 
**      
**          
*/
void UARTJB_PutChar(char ch)
{
    while(U1STAbits.UTXBF == 1);
    U1TXREG = ch;
}

/***	UARTJB_PutString
**
**	Parameters:
**          char szData[] -   the zero terminated string containing characters to be transmitted over UART.
**
**	Return Value:
**		
**
**	Description:
**		This function transmits all the characters from a zero terminated string over UART1. The terminator character is not sent.
**      
**          
*/
void UARTJB_PutString(char szData[])
{
    char *pData = szData;
    while(*pData)
    {
        UARTJB_PutChar((*(pData++)));
    }
}

/***	UARTJB_AvaliableRx
**
**	Parameters:
**          
**
**	Return Value:
**          - unsigned char - Receive Buffer Data Available bit 
                    1 = Receive buffer has data, at least one more character can be read
                    0 = Receive buffer is empty
**		
**
**	Description:
**		This function returns UART1 Receive Buffer Data Available bit.
**      It returns 1 if the receive buffer has data (at least one more character can be read).
**      It returns 0 if the receive buffer is empty.
**      
**          
*/
unsigned char UARTJB_AvaliableRx()
{
    return U1STAbits.URXDA;
}

/***	UARTJB_GetCharPoll
**
**	Parameters:
**          
**
**	Return Value:
**          - unsigned char - the byte received over UART
**		
**
**	Description:
**		This function waits until a byte is received over UART1. 
**      Then, it returns the byte.
**      This implements the polling method of receive one byte.
**      
**          
*/
unsigned char UARTJB_GetCharPoll() 
{
    while (!UARTJB_AvaliableRx());
    return U1RXREG;
}

/***	UARTJB_GetStringPoll
**
**	Parameters:
**          - unsigned char *pText - Pointer to a buffer to store the received bytes.
**          
**
**	Return Value:
**          unsigned char  receive status
**              1 if at least one received byte is available, the received characters are placed in pText buffer
**              0 if no received bytes are available 
**
**	Description:
**		This function returns a zero terminated string to be received over UART1, using polling method.
**      While a received byte is available, this function calls repeatedly UART_GetCharPoll 
**      until no values are received over UART1.
**      It returns 0 if no received bytes are available, and returns 1 if at least one byte was received.
 **          
*/
unsigned char UARTJB_GetStringPoll(unsigned char *pText)
{
    int idx = 0;
    while(UARTJB_AvaliableRx())
    {
        pText[idx++] = UARTJB_GetCharPoll();
    }
    if(idx != 0)
    {
        pText[idx] = 0; // terminator
    }
    return idx != 0;
}

/***	UARTJB_GetString
**
**	Parameters:
**		char* pchBuff - pointer to a char buffer to hold the received zero terminated string 
**		int cchBuff - size of the buffer to hold the zero terminated string
**          
**
**	Return Value:
**          unsigned char  receive status
**                  > 0 - the number of characters contained in the string
**                  0	- a CR+LF terminated string hasn't been received
**                  -3	- an invalid (0 char count) CR+LF terminated string was received  
**
**	Description:
**		This function returns a zero terminated string to be received over UART1,  
**      received using interrupt method. The interrupt recognizes a string 
**		having up to cchRxMax - 2 characters, followed by a carriage
**		return and a line feed ("\r\n", CRLF). The CRLF is stripped
**		from the string and a NULL character ('\0') is appended.
**		The number of characters contained in the zero terminated
**		string is returned as a value greater than 0.
**          
*/
unsigned char UARTJB_GetString( char* pchBuff, int cchBuff )
{
	unsigned char ich;
	
	// Have we finished receiving a CR+LF terminated string via UART1?
	if(!fRxDoneJB)
	{
		return 0;
	}
	// Was a 0 character CR+LF terminated string received?
	if(2 == ichRxJB )
	{
		// A zero character length CR+LF terminated string was received.
		macro_disable_interrupts;
		fRxDoneJB = 0;
		ichRxJB = 0;
		macro_enable_interrupts();
		
		return -3;
	}
	
    // copy the received chars to the destination location.
	for(ich = 0; ich < ichRxJB - 2; ich++)
	{
		*pchBuff = rgchRxJB[ich]; 
		pchBuff++;
	}
	*pchBuff = '\0';
	
	macro_disable_interrupts;
	fRxDoneJB = 0;
	ichRxJB = 0;
	macro_enable_interrupts();
	
	return ich;
}

/***	UARTJB_Close
**
**	Parameters:
** 
**
**	Return Value:
**      
**
**	Description:
**		This functions releases the hardware involved in UART library: 
**      it turns off the UART1 interface.
**      
**          
*/
void UARTJB_Close()
{
      U1MODEbits.ON = 0;   
}/* *****************************************************************************
 End of File
 */
