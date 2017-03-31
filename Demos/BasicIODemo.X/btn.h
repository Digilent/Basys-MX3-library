/* ************************************************************************** */
/** Descriptive File Name

  @Company
    Digilent

  @File Name
    btn.h

  @Description
        This file groups the declarations of the functions that implement
        the BTN library (defined in btn.c).
        Include the file in the project when this library is needed.
        Use #include "btn.h" in the source files where the functions are needed.
 */
/* ************************************************************************** */

#ifndef _BTN_H    /* Guard against multiple inclusion */
#define _BTN_H

void BTN_Init();
void BTN_InitChangeNotification();
unsigned char BTN_GetValue(unsigned char btn);
unsigned char BTN_GetGroupValue();

//private functions:
void BTN_ConfigurePins();

#endif /* _BTN_H */

/* *****************************************************************************
 End of File
 */
