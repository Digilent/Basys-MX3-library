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

#ifndef _SPIJA_H    /* Guard against multiple inclusion */
#define _SPIJA_H

void SPIJA_Init();
void SPIJA_ConfigurePins();
void SPIFA_ConfigureSPI();
void SPIJA_WriteByte(unsigned char bData);
//void inline SPIJA_WaitForDataByte();
void SPIJA_TrasferBytes(int nBytes, unsigned char *pbRdData, unsigned char *pbWrData);
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
