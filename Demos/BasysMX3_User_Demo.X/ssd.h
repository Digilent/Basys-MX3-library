/* ************************************************************************** */
/** Descriptive File Name

  @Company
    Digilent

  @File Name
    ssd.h

  @Description
    This file groups the declarations of the functions that implement
        the SSD library (defined in ssd.c).
        Include the file in the project when this library is needed.
        Use #include "ssd.h" in the source files where the functions are needed.
 */
/* ************************************************************************** */

#ifndef _SSD_H    /* Guard against multiple inclusion */
#define _SSD_H

//#ifdef __cplusplus
//extern "C" {
//#endif



    // *****************************************************************************
    // *****************************************************************************
    // Section: Interface Functions
    // *****************************************************************************
    // *****************************************************************************

void SSD_Init();
void SSD_WriteDigits(unsigned char d0, unsigned char d1, unsigned char d2, unsigned char d3, \
            unsigned char dp1, unsigned char dp2, unsigned char dp3, unsigned char dp4);
void SSD_WriteDigitsGrouped(unsigned int val, unsigned char dp);
unsigned char SSD_GetDigitSegments(unsigned char d);

// private functions
void SSD_ConfigurePins();
void SSD_Timer1Setup();


    /* Provide C++ Compatibility */
//#ifdef __cplusplus
//}
//#endif
#endif /* _SSD_H */

/* *****************************************************************************
 End of File
 */
