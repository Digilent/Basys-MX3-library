/* ************************************************************************** */
/** Descriptive File Name

  @Company
    Digilent

  @File Name
    uart.h

  @Description
        This file groups the declarations of the functions that implement
        the UART library (defined in uart.c).
        Include the file in the project when this library is needed.
        Use #include "uart.h" in the source files where the functions are needed.
 */
/* ************************************************************************** */

#ifndef _UART_H    /* Guard against multiple inclusion */
#define _UART_H

#define	cchRxMax	0xFF//64	// maximum number of characters a command
						// can contain including the CRLF ("\r\n")
void UART_InitPoll(unsigned int baud);
void UART_Init(unsigned int baud);
void UART_ConfigurePins();
void UART_ConfigureUart(unsigned int baud);
void UART_ConfigureUartRXInt(unsigned int baud);
void UART_CloseUART();


void UART_PutString(char szData[]);
unsigned char UART_GetCharPoll();
unsigned char UART_AvaliableRx();
unsigned char UART_GetString( char* pchBuff, int cchBuff );
unsigned char UART_GetStringPoll(unsigned char *pText);
//#ifdef __cplusplus
//extern "C" {
//#endif



    // *****************************************************************************
    // *****************************************************************************
    // Section: Interface Functions
    // *****************************************************************************
    // *****************************************************************************



    /* Provide C++ Compatibility */
//#ifdef __cplusplus
//}
//#endif
#endif /* _UART_H */

/* *****************************************************************************
 End of File
 */
