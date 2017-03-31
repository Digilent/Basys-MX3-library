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

#ifndef _SRV_H    /* Guard against multiple inclusion */
#define _SRV_H

void SRV_Init();
//void SRV_SetPhEnMotor1(unsigned char bDir, unsigned char bSpeed);

//Local functions
void SRV_ConfigureOCs();
void SRV_ConfigurePins();
void SRV_SetPulseMicroseconds1(unsigned short usVal);
void SRV_SetPulseMicroseconds2(unsigned short usVal);
void SRV_Close();

#endif /* _SRV_H */

/* *****************************************************************************
 End of File
 */
