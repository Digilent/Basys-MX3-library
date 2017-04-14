/* ************************************************************************** */
/** Descriptive File Name

  @Company
    Digilent

  @File Name
    mot.h

  @Description
     This file groups the declarations of the functions that implement
        the MOT library (defined in mot.c).
        Include the file in the project when this library is needed.
        Use #include "mot.h" in the source files where the functions are needed.
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
void MOT_Close();

//Private functions
void MOT_ConfigureOCs();
void MOT_ConfigurePins();
unsigned short MOT_PhEnComputeOCFromSpeed(unsigned char bSpeed);


#endif /* _MOT_H */

/* *****************************************************************************
 End of File
 */
