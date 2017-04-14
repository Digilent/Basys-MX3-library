/* ************************************************************************** */
/** Descriptive File Name

  @Company
    Digilent

  @File Name
    aic.h

  @Description
        This file groups the declarations of the functions that implement
        the AIC library (defined in aic.c). 
        Include the file in the project when the AIC and MIC libraries are needed.
 */
/* ************************************************************************** */

#ifndef _AIC_H    /* Guard against multiple inclusion */
#define _AIC_H

void AIC_Init();
unsigned int AIC_Val();
//private functions:
void AIC_ConfigurePins();

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
#endif /* _AIC_H */

/* *****************************************************************************
 End of File
 */
