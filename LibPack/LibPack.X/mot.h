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

#ifndef _MOT_H    /* Guard against multiple inclusion */
#define _MOT_H

void MOT_Init(unsigned char bMode1);
void MOT_SetPhEnMotor1(unsigned char bDir, unsigned char bSpeed);
void MOT_SetPhEnMotor2(unsigned char bDir, unsigned char bSpeed);
void MOT_SetMode(unsigned char bMode);
void MOT_InInInitStep(unsigned char bInitVal, unsigned char noBits);
void MOT_InInPerformStep(int noSteps, unsigned char bDir);

//Local functions
void MOT_ConfigureOCs();
void MOT_ConfigurePins();

void MOT_Close();

#endif /* _MOT_H */

/* *****************************************************************************
 End of File
 */
