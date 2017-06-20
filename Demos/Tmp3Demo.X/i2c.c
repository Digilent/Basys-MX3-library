/* ************************************************************************** */
/** Descriptive File Name

  @Company
    Digilent

  @File Name
    acl.c

  @Description
        This file groups the functions that implement the I2C library.
        The library implements I2C access hardware interface I2C1. 
        The hardware interface I2C2 is not available on BasysMX3.
        This library is used by ACL library, in order to implement I2C access.
        Include the file in the project when this library is needed.
 
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

/* ************************************************************************** */

#define I2C_WAIT_TIMEOUT 0x0FFF


/* ------------------------------------------------------------ */
/***	I2C_Init
**
**	Parameters:
**		unsigned int i2cFreq - I2C clock frequency (Hz).
**          for example 400000 value sent as parameter corresponds to 400 kHz
**
**	Return Value:
**
**	Description:
**		This function configures the I2C1 hardware interface of PIC32, according to the provided frequency.
**      In order to compute the baud rate value, it uses the peripheral bus frequency definition (PB_FRQ, located in config.h).
**      This is a low-level function called by ACL_Init(), so user should avoid calling it directly.
**          
*/
void I2C_Init(unsigned int i2cFreq)
{
    unsigned char   status      = 0;
    unsigned short  brgValue    = 0;

    I2C1CON = 0;            //Clear the content of I2C1CON register 
    brgValue = PB_FRQ / (2 * i2cFreq) - 2;
    I2C1BRG = brgValue;
    I2C1CONbits.ON = 1;     // Enable the I2C module and configure the SDA and 
                            //SCL pins as serial port pins
    I2C1CONbits.ACKEN = 1;
}


/* ------------------------------------------------------------ */
/***	I2C_Write
**
**	Parameters:
**		unsigned char slaveAddress  - I2C address of the slave device.
**      unsigned char* dataBuffer   - Pointer to a buffer storing the bytes to be transmitted.
**      unsigned char bytesNumber   - Number of bytes to be transmitted.
**      unsigned char stopBit       - Stop condition control.
**                       Example: 0 - A stop condition will not be sent;
**                                1 - A stop condition will be sent.
**
**	Return Value:
**      unsigned char   0          success
**                      0xFF        the slave address was not acknowledged by the device.
**                      0xFE        timeout error
**
**	Description:
**		This function writes a number of bytes to the specified I2C slave.
**      It returns the status of the operation: success or I2C errors (the slave address 
**      was not acknowledged by the device or timeout error).
**      This is a low-level function, so user should avoid calling it directly.
**          
*/
unsigned char I2C_Write(unsigned char slaveAddress,
                        unsigned char* dataBuffer,
                        unsigned char bytesNumber,
                        unsigned char stopBit)
{
    unsigned char status = 0;
    unsigned char acknowledge = 0;
    unsigned char byte = 0;
    int cnt_timeout;
    I2C1CONbits.SEN = 1;            //Initiate a start condition
    cnt_timeout = 0;
    while(I2C1CONbits.SEN && (++cnt_timeout < I2C_WAIT_TIMEOUT));   // Wait for start condition to complete
    if(cnt_timeout >= I2C_WAIT_TIMEOUT)
    {
        return 0xFE;    // timeout error
    }            
    I2C1TRN = slaveAddress << 1;    //RW bit set to 0
    cnt_timeout = 0;
    while(I2C1STATbits.TRSTAT && (++cnt_timeout < I2C_WAIT_TIMEOUT));   //Wait for transmission to complete 
    if(cnt_timeout >= I2C_WAIT_TIMEOUT)
    {
        return 0xFE;    // timeout error
    }
    acknowledge = I2C1STATbits.ACKSTAT;
    if(acknowledge == 0)          //Acknowledge was received 
    {
        for(byte = 0; byte < bytesNumber; byte++)
        {
            I2C1TRN = dataBuffer[byte];
            cnt_timeout = 0;
            while(I2C1STATbits.TRSTAT && (++cnt_timeout < I2C_WAIT_TIMEOUT)); // Wait for transmission to complete 
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
        I2C1CONbits.PEN = 1;            //Initiate a stop condition
        cnt_timeout = 0;
        while(I2C1CONbits.PEN && (++cnt_timeout < I2C_WAIT_TIMEOUT));         //Wait for stop condition to complete
        if(cnt_timeout >= I2C_WAIT_TIMEOUT)
        {
            return 0xFE;    // timeout error
        }
    }

    return status;
}

/* ------------------------------------------------------------ */
/***	I2C_Read
**
**	Parameters:
**		unsigned char slaveAddress  - I2C address of the slave device.
**      unsigned char* dataBuffer   - Pointer to a buffer where received bytes will be placed.
**      unsigned char bytesNumber   - Number of bytes to be read.
**
**	Return Value:
**      unsigned char   0           Success
**                      0xFF        the slave address was not acknowledged by the device.
**                      0xFE        timeout error
**
**	Description:
**		This function reads a number of bytes from the specified I2C slave.
**      It returns the status of the operation: success or I2C errors (the slave address 
**      was not acknowledged by the device or timeout error).
**      This is a low-level function, so user should avoid calling it directly.
**          
*/
unsigned char I2C_Read(unsigned char slaveAddress,
                    unsigned char* dataBuffer,
                    unsigned char bytesNumber)
{
    unsigned char status = 0;
    unsigned char acknowledge = 0;
    unsigned char byte = 0;
    int cnt_timeout;
    I2C1CONbits.RSEN = 1;            // Initiate a start condition
    cnt_timeout = 0;
    while(I2C1CONbits.RSEN && (++cnt_timeout < I2C_WAIT_TIMEOUT));         //Wait for start condition to complete
    if(cnt_timeout >= I2C_WAIT_TIMEOUT)
    {
        return 0xFE;    // timeout error
    }
    I2C1TRN = (slaveAddress << 1) + 1;
    cnt_timeout = 0;
    while(I2C1STATbits.TRSTAT && (++cnt_timeout < I2C_WAIT_TIMEOUT));     // Wait for reception to complete
    if(cnt_timeout >= I2C_WAIT_TIMEOUT)
    {
        return 0xFE;    // timeout error
    }
    acknowledge = I2C1STATbits.ACKSTAT;
    if(acknowledge == 0)            //Acknowledge was received
    {
        for(byte = 0; byte < bytesNumber; byte++)
        {
           I2C1CONbits.RCEN = 1;       //Enable receive mode for I2C
            if(byte == (bytesNumber - 1))
            {
                I2C1CONbits.ACKDT = 1;
            }
            else
            {
                I2C1CONbits.ACKDT = 0;
            }
            cnt_timeout = 0;
            while(I2C1CONbits.RCEN && (++cnt_timeout < I2C_WAIT_TIMEOUT));    //Wait for reception to complete
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
    I2C1CONbits.ACKEN = 1;          //Initiate Acknowledge sequence on SDAx and SCLx pins and transmit ACKDT data bit. Wait for Acknowledge sequence to complete 
    I2C1CONbits.PEN = 1;            //Initiate a stop condition 
    cnt_timeout = 0;
    while(I2C1CONbits.PEN && (++cnt_timeout < I2C_WAIT_TIMEOUT));         //Wait for stop condition to complete
    if(cnt_timeout >= I2C_WAIT_TIMEOUT)
    {
        return 0xFE;    // timeout error
    }

    return status;
}



/* ------------------------------------------------------------ */
/***	I2C_Close
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
void I2C_Close()
{
    I2C1CONbits.ON = 0;     //Disable the I2C module 
}

/* *****************************************************************************
 End of File
 */
