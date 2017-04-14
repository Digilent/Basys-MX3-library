/* ************************************************************************** */
/** Descriptive File Name

  @Company
    Digilent

  @File Name
    srv.h

  @Description
        This file groups the declarations of the functions that implement
        the SRV library (defined in srv.c).
        Include the file in the project when this library is needed.
        Use #include "srv.h" in the source files where the functions are needed.
 */
/* ************************************************************************** */

#ifndef _SRV_H    /* Guard against multiple inclusion */
#define _SRV_H

void SRV_Init();
void SRV_SetPulseMicroseconds1(unsigned short usVal);
void SRV_SetPulseMicroseconds2(unsigned short usVal);
void SRV_Close();

// private functions
void SRV_ConfigureOCs();
void SRV_ConfigurePins();

#endif /* _SRV_H */

/* *****************************************************************************
 End of File
 */
