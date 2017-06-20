/* ************************************************************************** */
/** Descriptive File Name

  @Company
    Digilent

  @File Name
    acl.c

  @Description
        This file groups the functions that implement the PmodTMP3 functionality.
        The PmodTMP3 is accessed using I2C1 access using the I2C library, 
        delivered in BasysMX3 library pack. 
 
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
#include "pmodtmp3.h"

/* ************************************************************************** */


/* ------------------------------------------------------------ */
/***	PMODTMP3_Init
**
**	Parameters:
**		unsigned char bCfg - One byte containing combinations (OR-ed) 
**                                  of predefined configuration setting
**
**	Return Value:
**		none
**
**	Description:
**		Configures temperature sensor, using a nbyte containing combinations 
**      (OR-ed) of predefined configuration setting defined in "pmodtmp3.h".
**          
*/
void PMODTMP3_Init(unsigned char bCfg)
{
	unsigned char rgCfg[2] = {0};
	I2C_Init(400000);   // configures I2C to work at 400 kHz.
    rgCfg[0] = 0x01;
	rgCfg[1] = bCfg;
	I2C_Write(TMP3Addr, rgCfg, 2, 1);
}


/* ------------------------------------------------------------ */
/***	PMODTMP3_GetTemp
**
**	Parameters:
**		none
**
**	Return Value:
**		float	- The resulted scaled temperature data from the
**					device.
**
**	Description:
**		Retrieves data from temp sensor and returns the corresponding value
**		as a floating point value in Celsius. 
**          
*/
float PMODTMP3_GetTemp()
{
	float fResult;
	unsigned short wResult = 0;
	unsigned char rgVals[2] = {0, 0};
	I2C_Write(TMP3Addr, rgVals, 1, 1); // send 0 as the pointer of the temperature register

	I2C_Read(TMP3Addr, rgVals, 2); // requests 2 bytes over I2C

	
	// Convert the result
	wResult = (rgVals[1] | (rgVals[0] << 8)) >> 4;  // to correctly process the (max) 12 bits of the reading
	fResult = wResult/16.0;             // according to the equation from TCN75A datasheet
	
	return fResult;
}



/* *****************************************************************************
 End of File
 */
