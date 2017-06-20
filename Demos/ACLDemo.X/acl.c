/* ************************************************************************** */
/** Descriptive File Name

  @Company
    Digilent

  @File Name
    acl.c

  @Description
        This file groups the functions that implement the ACL library.
        The library implements basic functions to configure the accelerometer 
        and read the accelerometer values (raw values and g values).
        The library uses I2C functions provided by I2C library.
        Include the file as well as i2c.c, i2c.h and config.h in the project 
        when this library is needed.
 
  @Author
    Cristian Fatu 
    cristian.fatu@digilent.ro
*/
/* ************************************************************************** */

/* ************************************************************************** */
/* ************************************************************************** */
/* Section: Included Files                                                    */
/* ************************************************************************** */
#include <xc.h>
#include <sys/attribs.h>
#include "config.h"
#include "i2c.h"
#include "acl.h"

/* ************************************************************************** */

float fGRangeLSB;   // global variable used to pre-compute the value in g corresponding to each count of the raw value

/* ------------------------------------------------------------ */
/***	ACL_Init
**
**	Parameters:
**		
**
**	Return Value:
**		
**
**	Description:
**		This function initializes the hardware involved in the ACL module: 
**      the I2C1 module of PIC32 is configured to work at 400 khz, 
**      the ACL is initialized at ±2g full scale range and 
**      the ACL_INT2 pin is configured as digital input.
**          
*/
void ACL_Init()
{
    ACL_ConfigurePins();
    I2C_Init(400000);
    ACL_SetRange(0); // +/- 2g
    ACL_SetRegister(ACL_CTRL_REG1, 1);        // Output data rate at 800Hz, no auto wake, no auto scale adjust, no fast read mode
}

/* ------------------------------------------------------------ */
/***	ACL_ConfigurePins
**
**	Parameters:
**		
**
**	Return Value:
**		
**
**	Description:
**		This function configures the digital pins involved in the ACL module: 
**      the ACL_INT2 pin is configured as digital input.
**      The function uses pin related definitions from config.h file.
**      This is a low-level function called by ACL_Init(), so user should avoid calling it directly.
**          
*/
void ACL_ConfigurePins()
{
    // Configure ACL signals as digital input.
    tris_ACL_INT2 = 1;
}




/* ------------------------------------------------------------ */
/***	ACL_SetRegister
**
**	Parameters:
**      unsigned char bAddress  - The register address.
**      unsigned char bValue    - The value to be written in the register.
**
**	Return Value:
**      unsigned char   0          success
**                      0xFF       the slave address was not acknowledged by the device.
**                      0xFE       timeout error
**
**	Description:
**		This function writes the specified value to the register specified by its address.
**      It returns the status of the operation: success or I2C errors (the slave address 
**      was not acknowledged by the device or timeout error).
**          
*/
unsigned char ACL_SetRegister(unsigned char bAddress, unsigned char bValue)
{
    unsigned char rgVals[2], bResult;
    rgVals[0] = bAddress;       // register address
    rgVals[1] = bValue;         // register value
    
    bResult = I2C_Write(ACL_I2C_ADDR, rgVals, 2, 1);

    return bResult;
}

/* ------------------------------------------------------------ */
/***	ACL_GetRegister
**
**	Parameters:
**      unsigned char bAddress  - The register address.
**
**	Return Value:
**      unsigned char   - The register value
**
**	Description:
**		This function returns the value of the register specified by its address. 
**      
**          
*/
unsigned char ACL_GetRegister(unsigned char bAddress)
{
    unsigned char bResult;
    I2C_Write(ACL_I2C_ADDR, &bAddress, 1, 0);
    I2C_Read(ACL_I2C_ADDR, &bResult, 1);

    return bResult;
}

/* ------------------------------------------------------------ */
/***	ACL_GetDeviceID
**
**	Parameters:
**
**
**	Return Value:
**      unsigned char   device ID   The device ID obtained  by reading"0x0D: WHO_AM_I Device ID" register
**                      0xFF        the slave address was not acknowledged by the device.
**                      0xFE        timeout error
**
**	Description:
**		This function returns the device ID. It obtains it by reading 
**          "0x0D: WHO_AM_I Device ID" register.
**      If errors occur, it returns the I2C error 
**     (the slave address was not acknowledged by the device or timeout error)
**          
*/
unsigned char ACL_GetDeviceID()
{
    return ACL_GetRegister(ACL_DEVICE_ID);
}

/* ------------------------------------------------------------ */
/***	ACL_SetRange
**
**	Parameters:
**      unsigned char   bRange     - The full scale range identification
**                          0   +/-2g
**                          1   +/-4g
**                          2   +/-8g
**
**
**	Return Value:
**      unsigned char   0           success
**                      0xFF        the slave address was not acknowledged by the device.
**                      0xFE        timeout error
**
**	Description:
**		This function sets the full scale range. It sets the according bits in the 
**      0x0E: XYZ_DATA_CFG register. The function also pre-computes the fGRangeLSB 
**      to be used when converting raw values to g values.
**      It returns the status of the operation: success or I2C errors (the slave address 
**      was not acknowledged by the device or timeout error).
 */
unsigned char ACL_SetRange(unsigned char bRange)
{
    unsigned char bResult, bVal;
    bRange &= 3;    // only 2 least significant bits from bRange are used

    
    bVal = ACL_GetRegister(ACL_XYZDATACFG); // get old value of the register
    bVal &= 0xFC;   // mask out the 2 LSBs
    bVal |= bRange; // set the 2 LSBs according to the range value
    bResult = ACL_SetRegister(ACL_XYZDATACFG, bVal);

    // set fGRangeLSB according to the selected range
    unsigned char bValRange = 1<<(bRange + 2);
    fGRangeLSB = ((float)bValRange)/(1<<12);     // the range is divided to the resolution corresponding to number of bits (12)
    return bResult;
}

/* ------------------------------------------------------------ */
/***	ACL_ReadRawValues
**
**	Parameters:
**      unsigned char *rgRawVals     - Pointer to a buffer where the received bytes will be placed. 
**      It will contain the 6 bytes, one pair for each of to the 3 axes:
**                      rgRawVals[0]   - MSB of X reading (X11 X10 X9 X8 X7 X6 X5 X4)
**                      rgRawVals[1]   - LSB of X reading ( X3  X2 X1 X0  0  0  0  0)
**                      rgRawVals[2]   - MSB of Y reading (Y11 Y10 Y9 Y8 Y7 Y6 Y5 Y4)
**                      rgRawVals[3]   - LSB of Y reading ( Y3  Y2 Y1 Y0  0  0  0  0)
**                      rgRawVals[4]   - MSB of Z reading (Z11 Z10 Z9 Z8 Z7 Z6 Z5 Z4)
**                      rgRawVals[5]   - LSB of Z reading ( Z3  Z2 Z1 Z0  0  0  0  0)
**                                      In the above table, the raw value for each axis is a 12 bits value: 
**                                      X11-X0, Y11-Y0, Z11-Z0, the 0 bit being the LSB bit.
**
**	Return Value:
**
**	Description:
**		This function reads the module raw values for the three axes. 
**      Each raw value is represented on 12 bits, so it will be represented on 2 bytes:
**      The MSB byte contains the 8 MSB bits, while the LSB byte contains the 4 LSB bits, 
**      padded right with 4 bits of 0.
**      The raw values are obtained by reading the 6 consecutive registers starting with "0x01: OUT_X_MSB"
**      
*/
void ACL_ReadRawValues(unsigned char *rgRawVals)
{
    unsigned char bVal = ACL_OUT_X_MSB;
    
    I2C_Write(ACL_I2C_ADDR, &bVal, 1, 0);
    I2C_Read(ACL_I2C_ADDR, rgRawVals, 6);
}

/* ------------------------------------------------------------ */
/***	ACL_ConvertRawToValueG
**
**	Parameters:
**      unsigned char *rgRawVals     - Pointer to a buffer that contains the 2 bytes corresponding to the raw value. 
**                      rgRawVals[0]   - MSB of raw value (V11 V10 V9 V8 V7 V6 V5 V4)
**                      rgRawVals[1]   - LSB of raw value ( V3  V2 V1 V0  0  0  0  0)
**                                      In the above table, the raw value is a 12 bits value: V11-V0, the 0 bit being the LSB bit
**
**
**	Return Value:
**          float   the acceleration value in terms of g
**
**	Description:
**		This function returns the acceleration value in terms of g, computed from
**      the 2 bytes acceleration raw values (2's complement on 12 bites) 
**      Each raw value is represented on 12 bits so it will be represented on 2 bytes:
**      The MSB byte contains the 8 MSB bits of the raw value, while the LSB byte contains the 
**      4 LSB bits of the raw value padded right with 4 bits of 0.
**      Computing the acceleration in terms of g is done with this formula:
**      (<full scale range> / 2^12)*<raw value>. The (<full scale range> / 2^12) term is 
**      pre-computed every time the range is set, using global variable fGRangeLSB.
**      This function involves float values computing, so avoid using it intensively when performance is an issue.
**      
*/
float ACL_ConvertRawToValueG(unsigned char *rgRawVals)
{
    // Convert the accelerometer value to G's. 
    // With 12 bits measuring over a +/- ng range we can find how to convert by using the equation:
    // Gs = Measurement Value * (G-range/(2^12))

    unsigned short usReading = (((unsigned short)rgRawVals[0]) << 4) + (rgRawVals[1] >> 4);
    // extend sign from bit 12 bits to 16 bits
    if(usReading & (1<<11))
    {
        usReading |= 0xF000;
    }
  // fGRangeLSB is pre-computed in ACL_SetRange
    float fResult = ((float)((short)usReading)) * fGRangeLSB;
    return fResult;
}

/* ------------------------------------------------------------ */
/***	ACL_ReadGValues
**
**	Parameters:
**      float *rgGVals     - Pointer to a buffer where the acceleration values in terms of g will be placed.
**                          It will contain the 3 float values, one for each of to the 3 axes:
**                              0 - Acceleration on X axis (in terms of g) 
**                              1 - Acceleration on Y axis (in terms of g) 
**                              2 - Acceleration on Z axis (in terms of g) 
**
**
**	Return Value:
**
**	Description:
**		This function provides the acceleration values for the three axes, as float values in terms of g.
**      The raw values are acquired using ACL_ReadRawValues. Then, for each axis, the 2 bytes
**      are converted to a float value in terms of in g.
**      This function involves float values computing, so avoid using it intensively when performance is an issue.
**      
*/
void ACL_ReadGValues(float *rgGVals)
{
    unsigned char rgRawVals[6];
    ACL_ReadRawValues(rgRawVals);
    rgGVals[0] = ACL_ConvertRawToValueG(rgRawVals);
    rgGVals[1] = ACL_ConvertRawToValueG(rgRawVals + 2);
    rgGVals[2] = ACL_ConvertRawToValueG(rgRawVals + 4);
}


/* ------------------------------------------------------------ */
/***	ACL_Close
**
**	Parameters:
** 
**
**	Return Value:
**      
**
**	Description:
**		This functions releases the hardware involved in the ACL library: 
**      it closes the I2C1 interface.
**      
**          
*/
void ACL_Close()
{
    I2C1CONbits.ON = 0;     //Disable the I2C module 
}

/* *****************************************************************************
 End of File
 */
