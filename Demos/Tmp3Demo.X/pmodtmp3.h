/* ************************************************************************** */
/** Descriptive File Name

  @Company
    Digilent

  @File Name
    acl.h

  @Description
        This file groups the declarations of the functions that implement
        the PMODTMP3 functionality (defined in pmodtmp3.c).
        Include the file in the project when this functionality is needed.
        Use #include "pmodtmp3.h" in the source files where the functions are needed.
 */
/* ************************************************************************** */

#ifndef _PMODTMP3_H    /* Guard against multiple inclusion */
#define _PMODTMP3_H

#define TMP3Addr	0x48	//based on jumpers JP1, JP2, and JP3
							//a table explaining the various
							//address configurations is available
							//in the PmodTMP3 Reference Manual
//Configuration settings
#define TMP3_ONESHOT	0x80 //One Shot mode
#define TMP3_RES9		0x00 //9-bit resultion
#define TMP3_RES10		0x20 //10-bit resolution
#define TMP3_RES11		0x40 //11-bit resolution
#define TMP3_RES12		0x60 //12-bit resolution
#define TMP3_FAULT1		0x00 //1 fault queue bits
#define TMP3_FAULT2		0x08 //2 fault queue bits
#define TMP3_FAULT4		0x10 //4 fault queue bits
#define TMP3_FAULT6		0x18 //6 fault queue bits
#define TMP3_ALERTLOW	0x00 //Alert bit active-low
#define TMP3_ALERTHIGH	0x04 //Alert bit active-high
#define TMP3_CMPMODE	0x00 //comparator mode
#define TMP3_INTMODE	0x02 //interrupt mode
#define TMP3_SHUTDOWN	0x01 //Shutdown enabled
#define	TMP3_STARTUP	0x00 //Shutdown Disabled
#define TMP3_CONF_DEFAULT	(TMP3_RES9 | TMP3_FAULT1 | TMP3_ALERTLOW | TMP3_CMPMODE)



void PMODTMP3_Config(unsigned char bCfg);
float PMODTMP3_GetTemp();


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
#endif /* _PMODTMP3_H */

/* *****************************************************************************
 End of File
 */
