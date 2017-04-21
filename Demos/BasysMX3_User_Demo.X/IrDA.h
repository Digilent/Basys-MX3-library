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

#ifndef IRDA_H
#define	IRDA_H

#include "config.h"

#define IRDA_TX         prt_IRDA_TX
#define IRDA_PDOWN      lat_IRDA_PDOWN
#define IRDA_RX         prt_IRDA_RX

void IRDA_Init();
void IRDA_set_FIR_mode();
void IRDA_set_SMIR_mode();
void IRDA_ConfigureIRDAOverUART5();
void IRDA_UARTPutChar(char ch);

unsigned char IRDA_UART_AvaliableRx();
unsigned char IRDA_UART_GetChar(unsigned int timeout, char *error);
        
#endif	/* IRDA_H */


/* *****************************************************************************
 End of File
 */