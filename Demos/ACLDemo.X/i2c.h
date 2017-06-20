/* ************************************************************************** */
/** Descriptive File Name

  @Company
    Digilent

  @File Name
    acl.h

  @Description
        This file groups the declarations of the functions that implement
        the I2C library (defined in acl.c).
        Include the file in the project when this library is needed.
        Use #include "i2c.h" in the source files where the functions are needed.
 */
/* ************************************************************************** */

#ifndef _I2C_H    /* Guard against multiple inclusion */
#define _I2C_H


void I2C_Init(unsigned int clockFreq);
unsigned char I2C_Write(unsigned char slaveAddress,
                        unsigned char* dataBuffer,
                        unsigned char bytesNumber,
                        unsigned char stopBit);
unsigned char I2C_Read(unsigned char slaveAddress,
                    unsigned char* dataBuffer,
                    unsigned char bytesNumber);

void I2C_Close();

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
#endif /* _I2C_H */

/* *****************************************************************************
 End of File
 */
