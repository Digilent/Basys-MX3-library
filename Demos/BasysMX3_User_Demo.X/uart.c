/* ************************************************************************** */
/** Descriptive File Name

  @Company
    Company Name

  @File Name
    filename.c

  @Summary
    Brief description of the file.

  @Description
    Describe the purpose of this file.
 */
/* ************************************************************************** */

/* ************************************************************************** */
/* ************************************************************************** */
/* Section: Included Files                                                    */
/* ************************************************************************** */
#include <xc.h>
#include <sys/attribs.h>
#include "config.h"
#include "uart.h"

volatile unsigned char fRxDone;
volatile unsigned char ichRx;
char rgchRx[cchRxMax];
/* ************************************************************************** */

void __ISR(_UART_4_VECTOR, ipl7) Uart4AHandler (void)
{
	unsigned char bVal;
	
//	LED_SetValue(7, 1);
	//Read the Uart1 rx buffer while data is available
	while(U4STAbits.URXDA)
	{
		bVal = (unsigned char)U4RXREG;
		
		if(!fRxDone)
	    {
	        // Do we have space to store another character?
	        if (cchRxMax > ichRx)
	        {
	            // Yes.
	    
                rgchRx[ichRx] = bVal;
	
	            // Is this the last character of the command?
	            if(('\n' == rgchRx[ichRx] ) && ('\r' == rgchRx[ichRx-1]))
	            {
	                // Yes.
	
	                fRxDone = 1;
	            }   // end if
	
	            ichRx++;
	        }   // end if
	        else
	        {
	            // No we don't have space to store anymore characters.
	            // Mark the command as complete.
	
	            fRxDone = 1;
	        }   // end else
    	}  	// end if
	}  // end while	
	//Clear the Uart1 interrupt flag.
	IFS2bits.U4RXIF = 0;
}

void UART_ConfigureUart()
{
    // 9600 baud, PBFreq = 80MHz
    U4MODEbits.ON     = 0;
    U4MODEbits.SIDL   = 0;
    U4MODEbits.IREN   = 0; 
    U4MODEbits.RTSMD  = 0;
    U4MODEbits.UEN0   = 0; 
    U4MODEbits.UEN1   = 0;
    U4MODEbits.WAKE   = 0;
    U4MODEbits.LPBACK = 0; 
    U4MODEbits.ABAUD  = 0;
    U4MODEbits.RXINV  = 0; 
    U4MODEbits.BRGH   = 0; 
    U4MODEbits.PDSEL1 = 0; 
    U4MODEbits.PDSEL0 = 0; 
    U4MODEbits.STSEL  = 0;  

    
    U4BRG = 43; //Baudrate = 115200
    //U4BRG = 519;         // BAUDRATE = 80MHz/16/9600-1 = 0d259  Ś9600BPS(40MHz)
//    U4BRG = 4152;   //1200

    U4STAbits.UTXEN    = 1;
    U4STAbits.URXEN    = 1;
    U4MODEbits.ON      = 1; 
    
}

void UART_ConfigureUartRXInt()
{
    INTDisableInterrupts();             // disable interrupts 

    UART_ConfigureUart();

    IPC9bits.U4IP = 7;
    IPC9bits.U4IS = 3;

	IFS2bits.U4RXIF = 0;    //Clear the Uart4 interrupt flag.
    IEC2bits.U4RXIE = 1; // enable rx interrupt
    

    INTEnableSystemMultiVectoredInt();  // enable interrupts 
}

void UART_Init()
{
    UART_ConfigurePins();
    UART_ConfigureUart();
}

void UART_InitRXInt()
{
    UART_ConfigurePins();
    UART_ConfigureUartRXInt();
}
// the opposite of UART_ConfigureUart
void UART_CloseUART()
{
    U4MODEbits.ON     = 0;    
}

void UART_ConfigurePins()
{
    tris_UART_TX = 0;   //TX digital output
    rp_UART_TX = 2;     // 0010 U4TX
    
    tris_UART_RX = 1;   //RX digital input
    rp_UART_RX = 9;     // 1001 RF13}
}

void UART_PutChar(char ch)
{
    while(U4STAbits.UTXBF == 1);
    U4TXREG = ch;
}

void UART_PutString(char szData[])
{
    char *pData = szData;
    while(*pData)
    {
        UART_PutChar((*(pData++)));
    }
}

unsigned char UART_GetChar() 
{
    while (!UART_AvaliableRx());
    return U4RXREG;
}

unsigned char UART_AvaliableRx()
{
    return U4STAbits.URXDA;
}

unsigned char UART_GetStringPoll(unsigned char *pText)
{
    int idx = 0;
    while(UART_AvaliableRx())
    {
        pText[idx++] = UART_GetChar(0);
    }
    if(idx != 0)
    {
        pText[idx] = 0; // terminator
    }
    return idx != 0;
}

/* ------------------------------------------------------------ */
/***	CchGetUart1Sz
**
**	Parameters:
**		pchBuff - pointer to a char buffer to hold the received sz
**		cchBuff - size of the buffer to hold the sz
**
**	Return Value:
**		> 0 - the number of characters contained in the command
**		 0	- a command hasn't been received
**		-1	- a buffer overrun occured
**		-2	- a buffer underrun occured (user buffer not large enough)
**		-3	- an invalid (0 char count) command was received  
**
**	Errors:
**		see above
**
**	Description:
**		This routine can be used to poll and a receive a command
**		via UART1. A properly formatted command is sent to UART1
**		with up to cchRxMax - 2 characters, followed by a carrage
**		return and a line feed ("\r\n", CRLF). The CRLF is stripped
**		from the command and a NUL character ('\0') is appended.
**		The number of characters contained in the zero terminated
**		string is returned as a value greater than 0.
*/

unsigned char UART_GetString( char* pchBuff, int cchBuff )
{
	unsigned char ich;
	
	// Have we finished receiving a command via UART1?
	if(!fRxDone)
	{
		return 0;
	}
	
	// Did a buffer overrun occur?
	if((cchRxMax == ichRx) && (('\r' != rgchRx[ichRx-2]) || ( '\n' != rgchRx[ichRx-1] )))
	{
		// A buffer overrun occured.
		
		INTDisableInterrupts();
		fRxDone = 0;
		ichRx = 0;
		INTEnableInterrupts();
		
		return -1;
	}
	
	// Does the user buffer contain enough space to hold the command?
	if(cchBuff < ichRx - 1)
	{
		// A buffer underrun occured.
		INTDisableInterrupts();
		fRxDone = 0;
		ichRx = 0;
		INTEnableInterrupts();
		
		return -2;
	}
	
	// Was a 0 character command received?
	if(2 == ichRx )
	{
		// A zero character length command was received.
		INTDisableInterrupts();
		fRxDone = 0;
		ichRx = 0;
		INTEnableInterrupts();
		
		return -3;
	}
	
    // copy the received chars to the destination location.
	for(ich = 0; ich < ichRx - 2; ich++)
	{
		*pchBuff = rgchRx[ich]; 
		pchBuff++;
	}
	*pchBuff = '\0';
	
	INTDisableInterrupts();
	fRxDone = 0;
	ichRx = 0;
	INTEnableInterrupts();
	
	return ich;
}

/* *****************************************************************************
 End of File
 */
