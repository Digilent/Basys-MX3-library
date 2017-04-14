/* ************************************************************************** */
/** Descriptive File Name

  @Company
    Digilent

  @File Name
    pmods.h

  @Description
     This file groups the declarations of the functions that implement
        the Pmods library (defined in pmods.c).
        Include the file in the project when this library is needed.
        Use #include "pmods.h" in the source files where the functions are needed.
 */
/* ************************************************************************** */

#ifndef _PMODS_H    /* Guard against multiple inclusion */
#define _PMODS_H

void PMODS_InitPin(unsigned char bPmod, unsigned char bPos, unsigned char bDir, unsigned char pullup, unsigned char pulldown);
unsigned char PMODS_GetValue(unsigned char bPmod, unsigned char bPos);
void PMODS_SetValue(unsigned char bPmod, unsigned char bPos, unsigned char bVal);
void PMODS_SetGroupValue(unsigned char bPmod, unsigned char bVal);
#endif /* _PMODS_H */

/* *****************************************************************************
 End of File
 */
