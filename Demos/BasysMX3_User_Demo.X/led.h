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

#ifndef _LED_H    /* Guard against multiple inclusion */
#define _LED_H

void LED_Init();
void LED_ConfigurePins();
void LED_SetValue(unsigned char bNo, unsigned char bVal);
unsigned char LED_GetValue(unsigned char bNo);
void LED_ToggleValue(unsigned char bNo);
void LED_SetGroupValue(unsigned char bVal);
unsigned char LED_GetGroupValue();
void LED_SetValueByNo(unsigned char bNo, unsigned char bVal);




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
#endif /* _LED_H */

/* *****************************************************************************
 End of File
 */
