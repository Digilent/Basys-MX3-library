/* ************************************************************************** */
/** Descriptive File Name

  @Company
    Digilent

  @File Name
    uartjb.h

  @Description
        This file groups the declarations of the functions that implement
        the UARTJB library (defined in uartjb.c).
        Include the file in the project when this library is needed.
        Use #include "uartjb.h" in the source files where the functions are needed.
 */
/* ************************************************************************** */

#ifndef _UARTJB_H    /* Guard against multiple inclusion */
#define _UARTJB_H

#define	cchRxMax	0xFF	// maximum number of characters a CR+LF terminated string
void UARTJB_InitPoll(unsigned int baud);
void UARTJB_Init(unsigned int baud);

void UARTJB_CloseUART();


void UARTJB_PutString(char szData[]);
unsigned char UARTJB_GetCharPoll();
unsigned char UARTJB_AvaliableRx();
unsigned char UARTJB_GetString( char* pchBuff, int cchBuff );
unsigned char UARTJB_GetStringPoll(unsigned char *pText);

// private functions
void UARTJB_ConfigurePins();
void UARTJB_ConfigureUart(unsigned int baud);
void UARTJB_ConfigureUartRXInt(unsigned int baud);

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
