/* ************************************************************************** */
/** Descriptive File Name

  @Company
    Digilent

  @File Name
    filename.h

  @Summary
    Brief description of the file.

  @Description
    Describe the purpose of this file.
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
void Timer5Setup();

#endif /* _RGBLED_H */

/* *****************************************************************************
 End of File
 */
