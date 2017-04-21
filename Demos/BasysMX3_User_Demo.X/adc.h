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

#ifndef _ADC_H    /* Guard against multiple inclusion */
#define _ADC_H

void ADC_Init();
void ADC_ConfigurePins();
void ADC_ConfigureAnalogInput(unsigned char channelNumber);
unsigned int POT_Val();
int ADC_analogRead(char analogPIN);
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
