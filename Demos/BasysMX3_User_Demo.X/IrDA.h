/* ************************************************************************** */
/** Descriptive File Name

  @Company
    Digilent

  @File Name
    IrDA.h

  @Description
     This file groups the declarations of the functions that implement
        the IRDA library (defined in IrDA.c).
        Use #include "IrDA.h" in the source files where the functions are needed.
 */
/* ************************************************************************** */

#ifndef IRDA_H
#define	IRDA_H

#include "config.h"

#define IRDA_TX         prt_IRDA_TX
#define IRDA_PDOWN      lat_IRDA_PDOWN
#define IRDA_RX         prt_IRDA_RX

void IRDA_Init(unsigned int baud);
void IRDA_Set_FIR_Mode();
void IRDA_Set_SMIR_Mode();
void IRDA_ConfigureIRDAOverUART5(unsigned int baud);
void IRDA_UARTPutChar(char ch);

unsigned char IRDA_UART_AvaliableRx();
unsigned char IRDA_UART_GetChar(unsigned int timeout, char *error);

//private
void IRDA_ConfigurePins();
        
#endif	/* IRDA_H */


/* *****************************************************************************
 End of File
 */