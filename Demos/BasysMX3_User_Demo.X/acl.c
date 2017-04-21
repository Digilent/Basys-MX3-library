/* ************************************************************************** */
/** Descriptive File Name

  @Company
    Company Name

  @File Name
    filename.c

  @Summary
    Brief description of the file.

  @Description
    Describe the purpose of this file.
 */
/* ************************************************************************** */

/* ************************************************************************** */
/* ************************************************************************** */
/* Section: Included Files                                                    */
/* ************************************************************************** */
#include <xc.h>
#include <sys/attribs.h>
#include "config.h"
#include "acl.h"

/* ************************************************************************** */

#define I2C_WAIT_TIMEOUT 0x0FFF
float fGRangeLSB;
void ACL_ConfigureI2C()
{
    ACL_I2C_Init(150000);
    ACL_SetRange(0); // +/- 2g
   ACL_SetRegister(ACL_CTRL_REG1, 1);        // Output data rate at 800Hz, no auto wake, no auto scale adjust, no fast read mode
}

void ACL_Init()
{
    ACL_ConfigurePins();
    ACL_ConfigureI2C();
}

void ACL_ConfigurePins()
{
    // Configure ACL signals as digital outputs.
    tris_ACL_INT2 = 0;
}

/***************************************************************************//**
 * @brief Initializes the I2C communication peripheral.
 *
 * @param clockFreq - I2C clock frequency (Hz).
 *                    Example: 100000 - I2C clock frequency is 100 kHz.
 * @return status   - Result of the initialization procedure.
 *                    Example:  0 - if initialization was successful;
 *                             -1 - if initialization was unsuccessful.
*******************************************************************************/
unsigned char ACL_I2C_Init(unsigned long clockFreq)
{
    unsigned char   status      = 0;
    unsigned long   pbFrequency = 80000000;
    unsigned short  brgValue    = 0;

    I2C1CON = 0;            /*!< Clear the content of I2C1CON register */
    /*!< Fsck = Fpb / ((I2CxBRG + 2) * 2) */
    brgValue = pbFrequency / (2 * clockFreq) - 2;
    I2C1BRG = brgValue;
    I2C1CONbits.ON = 1;     /*!< Enable the I2C module and configure the SDA and */
                            /*!< SCL pins as serial port pins */
    I2C1CONbits.ACKEN = 1;
    return status;
}

/***************************************************************************//**
 * @brief Writes data to a slave device.
 *
 * @param slaveAddress - Address of the slave device.
 * @param dataBuffer   - Pointer to a buffer storing the transmission data.
 * @param bytesNumber  - Number of bytes to write.
 * @param stopBit      - Stop condition control.
 *                       Example: 0 - A stop condition will not be sent;
 *                                1 - A stop condition will be sent.
 *
* @return status       - Number of write bytes
           - 0xFF if the slave address was not acknowledged by the device.
           - 0xFE timeout error
*******************************************************************************/
unsigned char ACL_I2C_Write(unsigned char slaveAddress,
                        unsigned char* dataBuffer,
                        unsigned char bytesNumber,
                        unsigned char stopBit)
{
    unsigned char status = 0;
    unsigned char acknowledge = 0;
    unsigned char byte = 0;
    int cnt_timeout;
    I2C1CONbits.SEN = 1;            /*!< Initiate a start condition */
    cnt_timeout = 0;
    while(I2C1CONbits.SEN && (++cnt_timeout < I2C_WAIT_TIMEOUT));         /*!< Wait for start condition to complete */
    if(cnt_timeout >= I2C_WAIT_TIMEOUT)
    {
        return 0xFE;    // timeout error
    }            
    I2C1TRN = slaveAddress << 1;    /*!< RW bit set to 0 */
    cnt_timeout = 0;
    while(I2C1STATbits.TRSTAT && (++cnt_timeout < I2C_WAIT_TIMEOUT));     /*!< Wait for transmition to complete */
    if(cnt_timeout >= I2C_WAIT_TIMEOUT)
    {
        return 0xFE;    // timeout error
    }
    acknowledge = I2C1STATbits.ACKSTAT;
    if(acknowledge == 0)            /*!< Acknowledge was received */
    {
        for(byte = 0; byte < bytesNumber; byte++)
        {
            I2C1TRN = dataBuffer[byte];
            cnt_timeout = 0;
            while(I2C1STATbits.TRSTAT && (++cnt_timeout < I2C_WAIT_TIMEOUT)); /*!< Wait for transmition to complete */
            if(cnt_timeout >= I2C_WAIT_TIMEOUT)
            {
                return 0xFE;    // timeout error
            }        
        }
    }
    else
    {
        status = 0xFF;
    }
    if(stopBit)
    {
        I2C1CONbits.PEN = 1;            /*!< Initiate a stop condition */
        cnt_timeout = 0;
        while(I2C1CONbits.PEN && (++cnt_timeout < I2C_WAIT_TIMEOUT));         /*!< Wait for stop condition to copmlete */
        if(cnt_timeout >= I2C_WAIT_TIMEOUT)
        {
            return 0xFE;    // timeout error
        }
    }

    return status;
}

/***************************************************************************//**
 * @brief Reads data from a slave device.
 *
 * @param slaveAddress - Address of the slave device.
 * @param dataBuffer   - Pointer to a buffer that will store the received data.
 * @param bytesNumber  - Number of bytes to read.
 * @param stopBit      - Stop condition control.
 *                       Example: 0 - A stop condition will not be sent;
 *                                1 - A stop condition will be sent.
 *
* @return status       - Number of read bytes
           - 0xFF if the slave address was not acknowledged by the device.
           - 0xFE timeout error
*******************************************************************************/
unsigned char ACL_I2C_Read(unsigned char slaveAddress,
                    unsigned char* dataBuffer,
                    unsigned char bytesNumber)
{
    unsigned char status = bytesNumber;
    unsigned char acknowledge = 0;
    unsigned char byte = 0;
    int cnt_timeout;
    I2C1CONbits.RSEN = 1;            /*!< Initiate a start condition */
            cnt_timeout = 0;
    while(I2C1CONbits.RSEN && (++cnt_timeout < I2C_WAIT_TIMEOUT));         /*!< Wait for start condition to complete */
            if(cnt_timeout >= I2C_WAIT_TIMEOUT)
            {
                return 0xFE;    // timeout error
            }
            I2C1TRN = (slaveAddress << 1) + 1;
            cnt_timeout = 0;
    while(I2C1STATbits.TRSTAT && (++cnt_timeout < I2C_WAIT_TIMEOUT));     /*!< Wait for transmition to complete */
            if(cnt_timeout >= I2C_WAIT_TIMEOUT)
            {
                return 0xFE;    // timeout error
            }
    acknowledge = I2C1STATbits.ACKSTAT;
    if(acknowledge == 0)            /*!< Acknowledge was received */
    {
        for(byte = 0; byte < bytesNumber; byte++)
        {
           I2C1CONbits.RCEN = 1;       /*!< Enable receive mode for I2C */
            if(byte == (bytesNumber - 1))
            {
                I2C1CONbits.ACKDT = 1;
            }
            else
            {
                I2C1CONbits.ACKDT = 0;
            }
            cnt_timeout = 0;
            while(I2C1CONbits.RCEN && (++cnt_timeout < I2C_WAIT_TIMEOUT));    /*!< Wait for reception to complete */
            if(cnt_timeout >= I2C_WAIT_TIMEOUT)
            {
                return 0xFE;    // timeout error
            }
            dataBuffer[byte] = I2C1RCV;
            I2C1CONbits.ACKEN = 1;
            cnt_timeout = 0;
            while(I2C1CONbits.ACKEN && (++cnt_timeout < I2C_WAIT_TIMEOUT));
            if(cnt_timeout >= I2C_WAIT_TIMEOUT)
            {
                return 0xFE;    // timeout error
            }
        }
    }
    else
    {
        status = 0xFF;
    }
    I2C1CONbits.ACKEN = 1;          // Initiate Acknowledge sequence on SDAx and SCLx pins and transmit ACKDT data bit.
//    while(I2C1CONbits.ACKEN);         /*!< Wait for Acknowledge sequence to copmlete */
    I2C1CONbits.PEN = 1;            /*!< Initiate a stop condition */
            cnt_timeout = 0;
    while(I2C1CONbits.PEN && (++cnt_timeout < I2C_WAIT_TIMEOUT));         /*!< Wait for stop condition to copmlete */
            if(cnt_timeout >= I2C_WAIT_TIMEOUT)
            {
                return 0xFE;    // timeout error
            }

    return status;
}

unsigned char ACL_GetDeviceID()
{
    unsigned char bResult, bVal = ACL_DEVICE_ID;
 
    
    ACL_I2C_Write(ACL_I2C_ADDR, &bVal, 1, 0);
    ACL_I2C_Read(ACL_I2C_ADDR, &bResult, 1);

    return bResult;
}

unsigned char ACL_SetRegister(unsigned char bAddress, unsigned char bValue)
{
    unsigned char rgVals[2], bResult;
    rgVals[0] = bAddress; // register address
    rgVals[1] = bValue;         // register valued
    
    bResult = ACL_I2C_Write(ACL_I2C_ADDR, rgVals, 2, 1);

    return bResult;
}

unsigned char ACL_GetRegister(unsigned char bAddress)
{
    unsigned char bResult;
    ACL_I2C_Write(ACL_I2C_ADDR, &bAddress, 1, 0);
    ACL_I2C_Read(ACL_I2C_ADDR, &bResult, 1);

    return bResult;
}

unsigned char ACL_SetRange(unsigned char bRange)
{
    unsigned char bResult, rgVals[2];
    bRange &= 3;    // only 2 lsb bits from bRange are used
    rgVals[0] = ACL_XYZDATACFG; // register address
    rgVals[1] = bRange;         // register valued
    
    bResult = ACL_SetRegister(ACL_XYZDATACFG, bRange);

    // set fGRangeLSB according to the selected range
    unsigned char bValRange = 1<<(bRange + 2);
    fGRangeLSB = ((float)bValRange)/(1<<12);     // the range is divided to the resolution corresponding to number of bits (12)
    return bResult;
}

void ACL_ReadRawValues(unsigned char *rgRawVals)
{
    unsigned char bVal = ACL_OUT_X_MSB;
    
    ACL_I2C_Write(ACL_I2C_ADDR, &bVal, 1, 0);
    ACL_I2C_Read(ACL_I2C_ADDR, rgRawVals, 6);
}

float ACL_ConvertRawToValueG(unsigned char *rgRawVals)
{
	//Convert the accelerometer value to G's. 
  //With 12 bits measuring over a +/- ng range we can find how to convert by using the equation:
  // Gs = Measurement Value * (G-range/(2^12))
  // fGRangeLSB is pre-computed in ACL_SetRange
    unsigned short usReading = (((unsigned short)rgRawVals[0]) << 4) + (rgRawVals[1] >> 4);
    // extend sign from bit 12 bits to 16 bits
    if(usReading & (1<<11))
    {
        usReading |= 0xF000;
    }
    float fResult = ((float)((short)usReading)) * fGRangeLSB;
    return fResult;
}

void ACL_ReadGValues(float *rgGVals)
{
    unsigned char rgRawVals[6];
    ACL_ReadRawValues(rgRawVals);
    rgGVals[0] = ACL_ConvertRawToValueG(rgRawVals);
    rgGVals[1] = ACL_ConvertRawToValueG(rgRawVals + 2);
    rgGVals[2] = ACL_ConvertRawToValueG(rgRawVals + 4);
}

/* *****************************************************************************
 End of File
 */
