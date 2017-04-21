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

unsigned char SSD_GetDigitSegments(unsigned char d);
void SSD_ConfigurePins();
void SSD_Init();
void SSD_DriveDigits();
void Timer1Setup();

void SSD_WriteDigits(unsigned char d0, unsigned char d1, unsigned char d2, unsigned char d3, \
            unsigned char dp1, unsigned char dp2, unsigned char dp3, unsigned char dp4);
void SSD_WriteDigitsGrouped(unsigned int val, unsigned char dp);
void SSD_CheckFlag();

    /* Provide C++ Compatibility */
//#ifdef __cplusplus
//}
//#endif
#endif /* _SSD_H */

/* *****************************************************************************
 End of File
 */
