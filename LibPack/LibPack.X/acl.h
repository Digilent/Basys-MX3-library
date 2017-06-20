/* ************************************************************************** */
/** Descriptive File Name

  @Company
    Digilent

  @File Name
    acl.h

  @Description
        This file groups the declarations of the functions that implement
        the ACL library (defined in acl.c).
        Include the file in the project when this library is needed.
        Use #include "acl.h" in the source files where the functions are needed.
 */
/* ************************************************************************** */

#ifndef _ACL_H    /* Guard against multiple inclusion */
#define _ACL_H



#define ACL_I2C_ADDR        0x1D
#define ACL_OUT_X_MSB       0x01 
#define ACL_CTRL_REG1       0x2A
#define ACL_DEVICE_ID       0x0D 
#define ACL_XYZDATACFG      0x0E 

// function prototypes
void ACL_Init();
void ACL_ReadRawValues(unsigned char *rgRawVals);
void ACL_ReadGValues(float *rgGVals);
unsigned char ACL_SetRange(unsigned char bRange);
float ACL_ConvertRawToValueG(unsigned char *rgRawVals);
unsigned char ACL_SetRegister(unsigned char bAddress, unsigned char bValue);
unsigned char ACL_GetRegister(unsigned char bAddress);
 
//private functions:
void ACL_ConfigurePins();
void I2C_Init(unsigned int clockFreq);
unsigned char I2C_Write(unsigned char slaveAddress,
                        unsigned char* dataBuffer,
                        unsigned char bytesNumber,
                        unsigned char stopBit);
unsigned char I2C_Read(unsigned char slaveAddress,
                    unsigned char* dataBuffer,
                    unsigned char bytesNumber);



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
