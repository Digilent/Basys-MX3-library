/**************/
/**Descriptive File Name

  @Company
    Digilent

  @File Name
    pmods.c

  @Description
        This file groups the functions that implement the PMODS library.
        Pins from PMODA and PMODB can be initialized as digital input / output pins, 
        their value can be accessed using set / get functions
        Include the file in the project, together with config.h when this library is needed.
 
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
#include "pmods.h"


/**------------------------------------------------------------ */
/**	PMODS_InitPin
**
**	Parameters:
**		unsigned char bPmod     - the PMOD where the pin is located
**                                 0 - PMODA
**                                 1 - PMODB
**     unsigned char bPos      - the pin position in the Pmod (allowed values 1-4, 7-10)
**                                 1  - JA1 (if bPmod = 0), JB1 (if bPmod = 1)
**                                 2  - JA2 (if bPmod = 0), JB2 (if bPmod = 1)
**                                 3  - JA3 (if bPmod = 0), JB3 (if bPmod = 1)
**                                 4  - JA4 (if bPmod = 0), JB4 (if bPmod = 1)
**                                 7  - JA7 (if bPmod = 0), JB7 (if bPmod = 1)
**                                 8  - JA8 (if bPmod = 0), JB8 (if bPmod = 1)
**                                 9  - JA9 (if bPmod = 0), JB9 (if bPmod = 1)
**                                10  - JA10(if bPmod = 0), JB10(if bPmod = 1)
**     unsigned char bDir      - the pin direction 
**                                 0  - Output
**                                 1  - Input
**     unsigned char pull-up    - the pull-up property of the pin
**                                 0  - No pull-up
**                                 1  - Pull-up
**     unsigned char pull-down  - the pull-down property of the pin
**                                 0  - No pull-down
**                                 1  - Pull-down
**
**	Return Value:
**		
**
**	Description:
**      This function configures the pins located in the PMODA and PMODB connectors 
**      to be used as digital input / output, also allowing pullup and pulldown properties to be specified.
**      This function uses pin related definitions from config.h file.  
**      If the bPmod and bPos do not specify a valid pin, nothing happens.
*/
void PMODS_InitPin(unsigned char bPmod, unsigned char bPos, unsigned char bDir, unsigned char pullup, unsigned char pulldown)
{
    switch (bPmod)
    {
        case 0: 
            // JA
        {
            switch (bPos)
            {
                case 1: 
                // JA1
                    rp_PMODS_JA1 = 0;   // default pin function (no remapable)
                    tris_PMODS_JA1 = bDir;
                    cnpu_PMODS_JA1 = pullup;        
                    cnpd_PMODS_JA1 = pulldown;                            
                    break;
                break;
                case 2: 
                // JA2
                    rp_PMODS_JA2 = 0;   // default pin function (no remapable)
                    tris_PMODS_JA2 = bDir;
                    cnpu_PMODS_JA2 = pullup;        
                    cnpd_PMODS_JA2 = pulldown;        
                break;
                case 3: 
                // JA3
                    rp_PMODS_JA3 = 0;   // default pin function (no remapable)
                    tris_PMODS_JA3 = bDir;
                    cnpu_PMODS_JA3 = pullup;        
                    cnpd_PMODS_JA3 = pulldown;        
                break;
                case 4: 
                // JA4
                    ansel_PMODS_JA4 = 0;    // set pin as digital
                    rp_PMODS_JA4 = 0;   // default pin function (no remapable)
                    tris_PMODS_JA4 = bDir;
                    cnpu_PMODS_JA4 = pullup;        
                    cnpd_PMODS_JA4 = pulldown;        
                break;
                case 7: 
                // JA7
                    rp_PMODS_JA7 = 0;   // default pin function (no remapable)
                    tris_PMODS_JA7 = bDir;
                    cnpu_PMODS_JA7 = pullup;        
                    cnpd_PMODS_JA7 = pulldown;        
                break;
                case 8:
                // JA8
                    ansel_PMODS_JA8 = 0;    // set pin as digital
                    rp_PMODS_JA8 = 0;   // default pin function (no remapable)
                    tris_PMODS_JA8 = bDir;
                    cnpu_PMODS_JA8 = pullup;        
                    cnpd_PMODS_JA8 = pulldown;        
                break;
                case 9: 
                // JA9
                    ansel_PMODS_JA9 = 0;    // set pin as digital
                    rp_PMODS_JA9 = 0;   // default pin function (no remapable)
                    tris_PMODS_JA9 = bDir;
                    cnpu_PMODS_JA9 = pullup;        
                    cnpd_PMODS_JA9 = pulldown;        
                break;
                case 10: 
                // JA10
                    ansel_PMODS_JA10 = 0;    // set pin as digital
                    rp_PMODS_JA10 = 0;   // default pin function (no remapable)
                    tris_PMODS_JA10 = bDir;
                    cnpu_PMODS_JA10 = pullup;        
                    cnpd_PMODS_JA10 = pulldown;        
                break;            
            }
        }
        break;        
        case 1: 
            // JB
        {
            switch (bPos)
            {
                case 1: 
                // JB1
                    rp_PMODS_JB1 = 0;   // default pin function (no remapable)
                    tris_PMODS_JB1 = bDir;
                    cnpu_PMODS_JB1 = pullup;        
                    cnpd_PMODS_JB1 = pulldown;        
                break;
                case 2: 
                // JB2
                    rp_PMODS_JB2 = 0;   // default pin function (no remapable)
                    tris_PMODS_JB2 = bDir;
                    cnpu_PMODS_JB2 = pullup;        
                    cnpd_PMODS_JB2 = pulldown;        
                break;
                case 3: 
                // JB3
                    rp_PMODS_JB3 = 0;   // default pin function (no remapable)
                    tris_PMODS_JB3 = bDir;
                    cnpu_PMODS_JB3 = pullup;        
                    cnpd_PMODS_JB3 = pulldown;        
                break;
                case 4: 
                // JB4
                    rp_PMODS_JB4 = 0;   // default pin function (no remapable)
                    tris_PMODS_JB4 = bDir;
                    cnpu_PMODS_JB4 = pullup;        
                    cnpd_PMODS_JB4 = pulldown;        
                break;
                case 7: 
                // JB7
                    OSCCONbits.SOSCEN = 0;
                    rp_PMODS_JB7 = 0;   // default pin function (no remapable)
                    tris_PMODS_JB7 = bDir;
                    cnpu_PMODS_JB7 = pullup;        
                    cnpd_PMODS_JB7 = pulldown;        
                break;
                case 8: 
                // JB8
                    rp_PMODS_JB8 = 0;   // default pin function (no remapable)
                    tris_PMODS_JB8 = bDir;
                    cnpu_PMODS_JB8 = pullup;        
                    cnpd_PMODS_JB8 = pulldown;        
                break;
                case 9: 
                // JB9
                    ansel_PMODS_JB9 = 0;    // set pis as digital
                    rp_PMODS_JB9 = 0;   // default pin function (no remapable)
                    tris_PMODS_JB9 = bDir;
                    cnpu_PMODS_JB9 = pullup;        
                    cnpd_PMODS_JB9 = pulldown;        
                break;
                case 10: 
                // JB10
                    OSCCONbits.SOSCEN = 0;
                    rp_PMODS_JB10 = 0;   // default pin function (no remapable)
                    tris_PMODS_JB10 = bDir;
                    cnpu_PMODS_JB10 = pullup;        
                    cnpd_PMODS_JB10 = pulldown;        
                break;            
            }
        }
    }
}

/**------------------------------------------------------------ */
/**	PMODS_GetValue
**
**	Parameters:
**		unsigned char bPmod     - the PMOD where the pin is located
**                                 0 - PMODA
**                                 1 - PMODB
**     unsigned char bPos      - the pin position in the Pmod (allowed values 1-4, 7-10
**                                 1  - JA1 (if bPmod = 0), JB1 (if bPmod = 1)
**                                 2  - JA2 (if bPmod = 0), JB2 (if bPmod = 1)
**                                 3  - JA3 (if bPmod = 0), JB3 (if bPmod = 1)
**                                 4  - JA4 (if bPmod = 0), JB4 (if bPmod = 1)
**                                 7  - JA7 (if bPmod = 0), JB7 (if bPmod = 1)
**                                 8  - JA8 (if bPmod = 0), JB8 (if bPmod = 1)
**                                 9  - JA9 (if bPmod = 0), JB9 (if bPmod = 1)
**                                10  - JA10(if bPmod = 0), JB10(if bPmod = 1)
**
**	Return Value:
**      unsigned char - the value of the digital pin or error
**              0       - the value of the digital pin 
**              1       - the value of the digital pin 
**              0xFF    - bPmod and bPos do not specify a valid pin 
**
**
**	Description:
**		This function returns the value of a digital pin located in the PMODA and PMODB connectors, 
**      specified by bPmod and bPos
**      If bPmod and bPos do not specify a valid pin, 0xFF is returned.
**      This function uses pin related definitions from config.h file.
**         
*/
unsigned char PMODS_GetValue(unsigned char bPmod, unsigned char bPos)
{
    unsigned bResult = 0xFF;
    
    switch (bPmod)
    {
        case 0: 
            // JA
        {
            switch (bPos)
            {
                case 1: 
                // JA1
                    bResult = prt_PMODS_JA1;
                break;
                case 2: 
                // JA2
                    bResult = prt_PMODS_JA2;
                break;
                case 3: 
                // JA3
                    bResult = prt_PMODS_JA3;
                break;
                case 4: 
                // JA4
                    bResult = prt_PMODS_JA4;
                break;
                case 7: 
                // JA7
                    bResult = prt_PMODS_JA7;
                break;
                case 8: 
                // JA8
                    bResult = prt_PMODS_JA8;
                break;
                case 9: 
                // JA9
                    bResult = prt_PMODS_JA9;
                break;
                case 10: 
                // JA10
                    bResult = prt_PMODS_JA10;
                break;            
            }
        }
        break;
        case 1: 
            // JB
        {
            switch (bPos)
            {
                case 1: 
                // JB1
                    bResult = prt_PMODS_JB1;
                break;
                case 2: 
                // JB2
                    bResult = prt_PMODS_JB2;
                break;
                case 3: 
                // JB3
                    bResult = prt_PMODS_JB3;
                break;
                case 4: 
                // JB4
                    bResult = prt_PMODS_JB4;
                break;
                case 7: 
                // JB7
                    bResult = prt_PMODS_JB7;
                break;
                case 8: 
                // JB8
                    bResult = prt_PMODS_JB8;
                break;
                case 9: 
                // JB9
                    bResult = prt_PMODS_JB9;
                break;
                case 10: 
                // JB10
                    bResult = prt_PMODS_JB10;
                break;            
            }
        }
    }
    return bResult;
}

        

/**------------------------------------------------------------ */
/**	PMODS_SetValue
**
**	Parameters:
**		unsigned char bPmod     - the PMOD where the pin is located
**                                 0 - PMODA
**                                 1 - PMODB
**     unsigned char bPos      - the pin position in the Pmod (allowed values 1-4, 7-10
**                                 1  - JA1 (if bPmod = 0), JB1 (if bPmod = 1)
**                                 2  - JA2 (if bPmod = 0), JB2 (if bPmod = 1)
**                                 3  - JA3 (if bPmod = 0), JB3 (if bPmod = 1)
**                                 4  - JA4 (if bPmod = 0), JB4 (if bPmod = 1)
**                                 7  - JA7 (if bPmod = 0), JB7 (if bPmod = 1)
**                                 8  - JA8 (if bPmod = 0), JB8 (if bPmod = 1)
**                                 9  - JA9 (if bPmod = 0), JB9 (if bPmod = 1)
**                                10  - JA10(if bPmod = 0), JB10(if bPmod = 1)
**     unsigned char bVal      - the value to be assigned to the digital pin
**                                 0    -   0 value will be assigned to the digital pin
**                                 not 0-   1 value will be assigned to the digital pin
**
**	Return Value:
**		
**
**	Description:
**      This function assigns the digital value specified by bVal to the digital
**      pin located in the PMODA and PMODB connectors, specified by bPmod and bPos.
**      If bPmod and bPos do not specify a valid pin, nothing is performed.
**      This function uses pin related definitions from config.h file.
*/
void PMODS_SetValue(unsigned char bPmod, unsigned char bPos, unsigned char bVal)
{
    unsigned char bVald = bVal ? 1: 0;
    switch (bPmod)
    {
        case 0: 
            // JA
        {
            switch (bPos)
            {
                case 1: 
                // JA1
                    lat_PMODS_JA1 = bVald;
                break;
                case 2: 
                // JA2
                    lat_PMODS_JA2 = bVald;
                break;
                case 3: 
                // JA3
                    lat_PMODS_JA3 = bVald;
                break;
                case 4: 
                // JA4
                    lat_PMODS_JA4 = bVald;
                break;
                case 7: 
                // JA7
                    lat_PMODS_JA7 = bVald;
                break;
                case 8: 
                // JA8
                    lat_PMODS_JA8 = bVald;
                break;
                case 9: 
                // JA9
                    lat_PMODS_JA9 = bVald;
                break;
                case 10: 
                // JA10
                    lat_PMODS_JA10 = bVald;
                break;            
            }
        }
        break;
        case 1: 
            // JB
        {
            switch (bPos)
            {
                case 1: 
                // JB1
                    lat_PMODS_JB1 = bVald;
                break;
                case 2: 
                // JB2
                    lat_PMODS_JB2 = bVald;
                break;
                case 3: 
                // JB3
                    lat_PMODS_JB3 = bVald;
                break;
                case 4: 
                // JB4
                    lat_PMODS_JB4 = bVald;
                break;
                case 7: 
                // JB7
                    lat_PMODS_JB7 = bVald;
                break;
                case 8: 
                // JB8
                    lat_PMODS_JB8 = bVald;
                break;
                case 9: 
                // JB9
                    lat_PMODS_JB9 = bVald;
                break;
                case 10: 
                // JB10
                    lat_PMODS_JB10 = bVald;
                break;            
            }
        }
    }
}

/*
 ** bPmod = 
 **         0 - JA
 **         1 - JB
 ** bPos =
 **         1  - JA1, JB1
 **         2  - JA2, JB2
 **         3  - JA3, JB3
 **         4  - JA4, JB4
 **         7  - JA7, JB7
 **         8  - JA8, JB8
 **         9  - JA9, JB9
 **        10  - JA10, JB10
 ** bVal = 
 **         0  - 0 value
 **         != 0  - 1 value
 ** */
/**------------------------------------------------------------ */
/**	PMODS_SetGroupValue
**
**	Parameters:
**		unsigned char bPmod     - the PMOD where the pin is located
**                                 0 - PMODA
**                                 1 - PMODB
**     unsigned char bVal      - the value to be assigned to the digital pins: each bit corresponds to a Pmod pin:
**                                 bit 0 (LSB)  - JA1 (if bPmod = 0), JB1 (if bPmod = 1)
**                                 bit 1        - JA2 (if bPmod = 0), JB2 (if bPmod = 1)
**                                 bit 2        - JA3 (if bPmod = 0), JB3 (if bPmod = 1)
**                                 bit 3        - JA4 (if bPmod = 0), JB4 (if bPmod = 1)
**                                 bit 4        - JA7 (if bPmod = 0), JB7 (if bPmod = 1)
**                                 bit 5        - JA8 (if bPmod = 0), JB8 (if bPmod = 1)
**                                 bit 6        - JA9 (if bPmod = 0), JB9 (if bPmod = 1)
**                                 bit 7        - JA10(if bPmod = 0), JB10(if bPmod = 1)
**
**	Return Value:
**		
**
**	Description:
**      This function assigns a digital value to all the digital pins of the PMOD specified by bPmod.
**      Each bit from bVal is assigned to a pin in the PMOD, as shown above.
*/

void PMODS_SetGroupValue(unsigned char bPmod, unsigned char bVal)
{
    int i, j;
    for(i = 1; i <= 8; i++)
    {
        j = (i >= 5) ? (i+2): i;
        PMODS_SetValue(bPmod, j, bVal & (1<<(i-1))); // the bit corresponding to the Pmod pin
    }    
}


/* *****************************************************************************
 End of File
 */
