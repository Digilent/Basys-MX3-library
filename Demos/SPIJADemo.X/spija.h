/* ************************************************************************** */
/** Descriptive File Name

  @Company
    Digilent

  @File Name
    spija.h

  @Description
        This file groups the declarations of the functions that implement
        the SPIFLASH library (defined in spija.c).
        Include the file in the project when this library is needed.
        Use #include "spija.h" in the source files where the functions are needed.
 */
/* ************************************************************************** */

#ifndef _SPIJA_H    /* Guard against multiple inclusion */
#define _SPIJA_H

void SPIJA_Init();

void SPIJA_ConfigureSPI(unsigned int spiFreq, unsigned char pol, unsigned char edge);
unsigned char SPIJA_TransferByte(unsigned char bVal);
void SPIJA_TransferBytes(int bytesNumber, unsigned char *pbRdData, unsigned char *pbWrData);
void SPIJA_Close();

//private functions
void SPIJA_ConfigurePins();

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
#endif /* _SPIJA_H */

/* *****************************************************************************
 End of File
 */
