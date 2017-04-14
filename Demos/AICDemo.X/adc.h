/* ************************************************************************** */
/** Descriptive File Name

  @Company
    Digilent

  @File Name
    adc.h

  @Description
        This file groups the declarations of the functions that implement
        the ADC module (defined in adc.c). This module is used by AIC and MIC libraries.
        Include the file in the project when the AIC and MIC libraries are needed.
 */
/* ************************************************************************** */

#ifndef _ADC_H    /* Guard against multiple inclusion */
#define _ADC_H


void ADC_Init();
unsigned int ADC_AnalogRead(unsigned char analogPIN);
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
#endif /* _ADC_H */

/* *****************************************************************************
 End of File
 */
