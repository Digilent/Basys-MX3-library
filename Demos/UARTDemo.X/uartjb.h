/* ************************************************************************** */
/** Descriptive File Name

  @Company
    Digilent

  @File Name
    filename.h

  @Summary
    Brief description of the file.

  @Description
    Describe the purpose of this file.
 */
/* ************************************************************************** */

#ifndef _UARTJB_H    /* Guard against multiple inclusion */
#define _UARTJB_H

#define	cchRxMax	0xFF//64	// maximum number of characters a command
						// can contain including the CRLF ("\r\n")
void UARTJB_InitPoll(unsigned int baud);
void UARTJB_Init(unsigned int baud);
void UARTJB_ConfigurePins();
void UARTJB_ConfigureUart(unsigned int baud);
void UARTJB_ConfigureUartRXInt(unsigned int baud);
void UARTJB_CloseUART();


void UARTJB_PutString(char szData[]);
unsigned char UARTJB_GetCharPoll();
unsigned char UARTJB_AvaliableRx();
unsigned char UARTJB_GetString( char* pchBuff, int cchBuff );
unsigned char UARTJB_GetStringPoll(unsigned char *pText);
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
#endif /* _UARTJB_H */

/* *****************************************************************************
 End of File
 */
