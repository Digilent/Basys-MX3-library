/* ************************************************************************** */
/** Descriptive File Name

  @Company
    Digilent

  @File Name
    uart.c

  @Description
        This file groups the functions that implement the UART library.
        This library implements the UART4 functionality connected to the USB - UART 
        interface labeled UART. It provides basic functions to configure UART and  
        transmit / receive functions. The receive can be done both with interrupts and with 
        polling method. 
        Include the file in the project, together with config.h, when this library is needed.		

  @Author
    Cristian Fatu 
    cristian.fatu@digilent.ro
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

// global variables, to communicate between interrupt handler and other 
volatile unsigned char fRxDone;
volatile unsigned char ichRx;
char rgchRx[cchRxMax];
/* ************************************************************************** */

/* ------------------------------------------------------------ */
/***	Uart4Handler
**
**	Description:
**		This is the interrupt handler for UART4 RX. It is used to receive strings using interrupt method. 
**      The interrupt recognizes a string having up to cchRxMax - 2 characters, followed by a CRLF.
**      When available received bytes are found they are placed in a global string rgchRx.
**      When a carriage return and a line feed ("\r\n", CRLF) sequence is recognized, the interrupt handler 
**      flags the success using the fRxDone global variable and then ignores the received bytes.
**          
*/
void __ISR(_UART_4_VECTOR, ipl6) Uart4Handler (void)
{
	unsigned char bVal;
	
	//Read the Uart4 RX buffer while data is available
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
	            }   
	            ichRx++;
	        }  
	        else
	        {
	            // No, we don't have space to store anymore characters.
	            // Mark the command as complete
	            fRxDone = 1;
	        }   
    	}  	
	}  
	//Clear the Uart4 interrupt flag.
	IFS2bits.U4RXIF = 0;
}

/***	UART_Init
**
**	Parameters:
**		unsigned int baud - UART baud rate.
**                                     for example 115200 corresponds to 115200 baud			
**
**	Return Value:
**		
**
**	Description:
**		This function initializes the hardware involved in the UART module, in 
**      the UART receive with interrupt mode.
**      The UART_TX digital pin is configured as digital output.
**      The UART_RX digital pin is configured as digital input.
**      The UART_TX and UART_RX are mapped over the UART4 interface.
**      The UART4 module of PIC32 is configured to work at the specified baud, no parity and 1 stop bit.
**      
**          
*/
void UART_Init(unsigned int baud)
{
    UART_ConfigurePins();
    UART_ConfigureUartRXInt(baud);
}

/***	UART_InitPoll
**
**	Parameters:
**		unsigned int baud - UART baud rate.
**                                     for example 115200 corresponds to 115200 baud			
**
**	Return Value:
**		
**
**	Description:
**		This function initializes the hardware involved in the UART module, in 
**      the UART receive without interrupts (polling method).
**      The UART_TX digital pin is configured as digital output.
**      The UART_RX digital pin is configured as digital input.
**      The UART_TX and UART_RX are mapped over the UART4 interface.
**      The UART4 module of PIC32 is configured to work at the specified baud, no parity and 1 stop bit.
**      
**          
*/
void UART_InitPoll(unsigned int baud)
{
    UART_ConfigurePins();
    UART_ConfigureUart(baud);
}

/***	UART_ConfigureUart
**
**	Parameters:
**		unsigned int baud - UART baud rate.
**                                     for example 115200 corresponds to 115200 baud
**
**	Return Value:
**		
**
**	Description:
**		This function configures the UART4 hardware interface of PIC32, according 
**      to the provided baud rate, no parity and 1 stop bit, with no interrupts.
**      In order to compute the baud rate value, it uses the peripheral bus frequency definition (PB_FRQ, located in config.h)
**      This is a low-level function called by initialization functions, so user should avoid calling it directly.   
**      
**          
*/
void UART_ConfigureUart(unsigned int baud)
{
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
    U4MODEbits.PDSEL1 = 0; 
    U4MODEbits.PDSEL0 = 0; 
    U4MODEbits.STSEL  = 0;  

    
    U4MODEbits.BRGH   = 0; 

    U4BRG = (int)(((float)PB_FRQ/(16*baud) - 1) + 0.5); // add 0.5 just in order to implement the round using the floor approach

    U4STAbits.UTXEN    = 1;
    U4STAbits.URXEN    = 1;
    U4MODEbits.ON      = 1; 
    
}

/***	UART_ConfigureUartRXInt
**
**	Parameters:
**		unsigned int baud - UART baud rate.
**                                     for example 115200 corresponds to 115200 baud
**
**	Return Value:
**		
**
**	Description:
**		This function configures the UART4 hardware interface of PIC32, according 
**      to the provided baud rate, no parity and 1 stop bit (by calling the UART_ConfigureUart function)
**      and additionally configures the interrupt on RX.
**      This is a low-level function called by initialization functions, so user should avoid calling it directly.   
**      
**          
*/
void UART_ConfigureUartRXInt(unsigned int baud)
{

    UART_ConfigureUart(baud);

    IPC9bits.U4IP = 6;
    IPC9bits.U4IS = 3;

	IFS2bits.U4RXIF = 0;    //Clear the Uart4 interrupt flag.
    IEC2bits.U4RXIE = 1;    // enable RX interrupt
    

    macro_enable_interrupts();  // enable interrupts 
}

/***	UART_ConfigurePins
**
**	Parameters:
**		
**
**	Return Value:
**		
**
**	Description:
**		This function configures the digital pins involved in the UART module: 
**      The UART_TX digital pin is configured as digital output.
**      The UART_RX digital pin is configured as digital input.
**      The UART_TX and UART_RX are mapped over the UART4 interface.
**      The function uses pin related definitions from config.h file.
**      This is a low-level function called by UART_Init(), so user should avoid calling it directly.   
**          
*/
void UART_ConfigurePins()
{
    tris_UART_TX = 0;   //TX digital output
    rp_UART_TX = 2;     // 0010 U4TX
    
    tris_UART_RX = 1;   //RX digital input
    rp_UART_RX = 9;     // 1001 RF13
}




/***	UART_PutChar
**
**	Parameters:
**          char ch -   the character to be transmitted over UART.
**
**	Return Value:
**		
**
**	Description:
**		This function transmits a character over UART4. 
**      
**          
*/
void UART_PutChar(char ch)
{
    while(U4STAbits.UTXBF == 1);
    U4TXREG = ch;
}

/***	UART_PutString
**
**	Parameters:
**          char szData[] -   the zero terminated string containing characters to be transmitted over UART.
**
**	Return Value:
**		
**
**	Description:
**		This function transmits all the characters from a zero terminated string over UART4. The terminator character is not sent.
**      
**          
*/
void UART_PutString(char szData[])
{
    char *pData = szData;
    while(*pData)
    {
        UART_PutChar((*(pData++)));
    }
}



/***	UART_AvaliableRx
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
**		This function returns UART4 Receive Buffer Data Available bit.
**      It returns 1 if the receive buffer has data (at least one more character can be read).
**      It returns 0 if the receive buffer is empty.
**      
**          
*/
unsigned char UART_AvaliableRx()
{
    return U4STAbits.URXDA;
}

/***	UART_GetCharPoll
**
**	Parameters:
**          
**
**	Return Value:
**          - unsigned char - the byte received over UART
**		
**
**	Description:
**		This function waits until a byte is received over UART4. 
**      Then, it returns the byte.
**      This implements the polling method of receive one byte.
**      
**          
*/
unsigned char UART_GetCharPoll() 
{
    while (!UART_AvaliableRx());
    return U4RXREG;
}

/***	UART_GetStringPoll
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
**		This function returns a zero terminated string received over UART4, using polling method.    
**      While a received bytes is available, this function calls repeatedly UART_GetCharPoll 
**      until no values are received over UART4.
**      It returns 0 if no received bytes are available, and returns 1 if at least one byte was received.
*/
unsigned char UART_GetStringPoll(unsigned char *pText)
{
    int idx = 0;
    while(UART_AvaliableRx())
    {
        pText[idx++] = UART_GetCharPoll();
    }
    if(idx != 0)
    {
        pText[idx] = 0; // terminator
    }
    return idx != 0;
}

/***	UART_GetString
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
**                  -2	- a buffer underrun occurred (user buffer not large enough)
**                  -3	- an invalid (0 char count) CR+LF terminated string was received  
**
**	Description:
**		This function returns a zero terminated string to be received over UART4,  
**      received using interrupt method. The interrupt recognizes a string 
**		having up to cchRxMax - 2 characters, followed by a carriage
**		return and a line feed ("\r\n", CRLF). The CRLF is stripped
**		from the string and a NULL character ('\0') is appended.
**		The number of characters contained in the zero terminated
**		string is returned as a value greater than 0.
**          
*/
unsigned char UART_GetString( char* pchBuff, int cchBuff )
{
	unsigned char ich;
	
	// Have we finished receiving a CR+LF terminated string via UART4?
	if(!fRxDone)
	{
		return 0;
	}
	// Does the user buffer have enough space to store the CR+LF terminated string?
	if(cchBuff < ichRx - 1)
	{
		// A buffer underrun occured.
		macro_disable_interrupts;
		fRxDone = 0;
		ichRx = 0;
		macro_enable_interrupts();
		
		return -2;
	}
    // Was a 0 character CR+LF terminated string received?
	if(2 == ichRx )
	{
		// A zero character length CR+LF terminated string was received.
		macro_disable_interrupts;
		fRxDone = 0;
		ichRx = 0;
		macro_enable_interrupts();
		return -3;
	}
	
    // copy the received chars to the destination location.
	for(ich = 0; ich < ichRx - 2; ich++)
	{
		*pchBuff = rgchRx[ich]; 
		pchBuff++;
	}
	*pchBuff = '\0';
	
	macro_disable_interrupts;
	fRxDone = 0;
	ichRx = 0;
	macro_enable_interrupts();
	return ich;
}
/***	UART_Close
**
**	Parameters:
** 
**
**	Return Value:
**      
**
**	Description:
**		This functions releases the hardware involved in UART library: 
**      it turns off the UART4 interface.
**      
**          
*/
void UART_Close()
{
      U4MODEbits.ON = 0;   
}
/* *****************************************************************************
 End of File
 */
