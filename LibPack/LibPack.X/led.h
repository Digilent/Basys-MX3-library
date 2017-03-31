/* ************************************************************************** */
/** Descriptive File Name

  @Company
    Digilent

  @File Name
    led.h

  @Description
        This file groups the declarations of the functions that implement
        the LED library (defined in led.c).
        Include the file in the project when this library is needed.
        Use #include "led.h" in the source files where the functions are needed.
 */
/* ************************************************************************** */

#ifndef _LED_H    /* Guard against multiple inclusion */
#define _LED_H

void LED_Init();

void LED_SetValue(unsigned char bNo, unsigned char bVal);
void LED_ToggleValue(unsigned char bNo);
void LED_SetGroupValue(unsigned char bVal);



//private functions:
void LED_ConfigurePins();    


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
