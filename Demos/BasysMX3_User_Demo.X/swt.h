/* ************************************************************************** */
/** Descriptive File Name

  @Company
    Digilent

  @File Name
    swt.h

  @Description
        This file groups the declarations of the functions that implement
        the SWT library (defined in swt.c).
        Include the file in the project when this library is needed, 
        and use #include "swt.h" in the source files where the functions are needed.
 */
/* ************************************************************************** */

#ifndef _SWT_H    /* Guard against multiple inclusion */
#define _SWT_H

void SWT_Init();
unsigned char SWT_GetValue(unsigned char bNo);
unsigned char SWT_GetGroupValue();


//private functions:
void SWT_ConfigurePins();
#endif /* _SWT_H */

/* *****************************************************************************
 End of File
 */
