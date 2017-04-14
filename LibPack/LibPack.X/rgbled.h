/* ************************************************************************** */
/** Descriptive File Name

  @Company
    Digilent

  @File Name
    rgbled.h

  @Description
    This file groups the declarations of the functions that implement
        the RGBLed library (defined in rgbled.c).
        Include the file in the project when this library is needed.
        Use #include "rgbled.h" in the source files where the functions are needed.
 */
/* ************************************************************************** */

#ifndef _RGBLED_H    /* Guard against multiple inclusion */
#define _RGBLED_H

void RGBLED_Init();
void RGBLED_SetValue(unsigned char bValR, unsigned char bValG, unsigned char bValB);
void RGBLED_SetValueGrouped(unsigned int uiValRGB);
void RGBLED_Close();

//private functions:
void RGBLED_ConfigurePins();
void RGBLED_Timer5Setup();

#endif /* _RGBLED_H */

/* *****************************************************************************
 End of File
 */
