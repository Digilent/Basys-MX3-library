/* ************************************************************************** */
/** Descriptive File Name

  @Company
    Company Name

  @File Name
    filename.h

  @Summary
    Brief description of the file.

  @Description
    Describe the purpose of this file.
 */
/* ************************************************************************** */

#ifndef _UART_H    /* Guard against multiple inclusion */
#define _UART_H

#define	cchRxMax	0xFF//64	// maximum number of characters a command
						// can contain including the CRLF ("\r\n")
void UART_Init();
void UART_InitRXInt();
void UART_ConfigurePins();
void UART_ConfigureUart();
void UART_CloseUART();


void UART_PutString(char szData[]);
unsigned char UART_GetChar();
unsigned char UART_AvaliableRx();
unsigned char UART_GetString( char* pchBuff, int cchBuff );
unsigned char UART_GetStringPoll(unsigned char *pText);
unsigned char UART_GetString1( char* pchBuff, int cchBuff );
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
