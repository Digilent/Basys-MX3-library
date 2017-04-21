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

#ifndef _ACL_H    /* Guard against multiple inclusion */
#define _ACL_H

void ACL_Init();
void ACL_ConfigurePins();
void ACL_ConfigureI2C();

unsigned char ACL_I2C_Init(unsigned long clockFreq);
unsigned char ACL_I2C_Write(unsigned char slaveAddress,
                        unsigned char* dataBuffer,
                        unsigned char bytesNumber,
                        unsigned char stopBit);
unsigned char ACL_I2C_Read(unsigned char slaveAddress,
                    unsigned char* dataBuffer,
                    unsigned char bytesNumber);
unsigned char ACL_SetRegister(unsigned char bAddress, unsigned char bValue);
unsigned char ACL_SetRange(unsigned char bRange);
void ACL_ReadRawValues(unsigned char *rgRawVals);
unsigned char ACL_GetRegister(unsigned char bAddress);
float ACL_ConvertRawToValueG(unsigned char *rgRawVals);
void ACL_ReadGValues(float *rgGVals);

#define ACL_I2C_ADDR    0x1D

#define ACL_OUT_X_MSB       0x01 
#define ACL_CTRL_REG1       0x2A
#define ACL_DEVICE_ID       0x0D 
#define ACL_XYZDATACFG      0x0E 
//#ifdef __cplusplus
//extern "C" {
//#endif



    // *****************************************************************************
    // *****************************************************************************
    // Section: Interface Functions
    // *****************************************************************************
    // *****************************************************************************



    /* Provide C++ Compatibility */
//#ifdef __cplusplus
//}
//#endif
#endif /* _ACL_H */

/* *****************************************************************************
 End of File
 */
