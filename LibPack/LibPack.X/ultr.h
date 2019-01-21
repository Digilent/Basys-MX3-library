/* ************************************************************************** */
/** Descriptive File Name

  @Company
    University of Delaware

  @File Name
    ultr.h

  @Description
        This file groups the declarations of the functions that implement
        the ULTR library (defined in ultr.c).
        Include the file in the project when this library is needed.
        Use #include "ultr.h" in the source files where the functions are needed.
 */
/* ************************************************************************** */


#ifndef _ULTR_H    /* Guard against multiple inclusion */
#define _ULTR_H

void ULTR_Init(int ePinPmod, int ePin, int tPinPmod, int tPin);
int ULTR_MeasureDist();
void ULTR_Close();

#endif /* _ULTR_H */

/* *****************************************************************************
 End of File
 */