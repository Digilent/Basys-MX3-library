/* ************************************************************************** */
/** Descriptive File Name

  @Company
    Digilent

  @File Name
    swt.c

  @Description
        This file groups the functions that implement the SWT library.
        The functions implement basic digital input functionality.
        Include the file in the project, together with config.h, when this library is needed.

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
#include "swt.h"

/* ************************************************************************** */


/***	SWT_Init
**
**	Parameters:
**		
**
**	Return Value:
**		
**
**	Description:
**		This function initializes the hardware involved in the SWT module: 
**      the pins corresponding to switches are initialized as digital inputs.
**          
*/
void SWT_Init()
{
    SWT_ConfigurePins();
}

/***	SWT_ConfigurePins
**
**	Parameters:
**		
**
**	Return Value:
**		
**
**	Description:
**		This function configures the IO pins involved in the SWT module as digital input pins. 
**      The function uses pin related definitions from config.h file.
**      This is a low-level function called by SWT_Init(), so user should avoid calling it directly.      
**          
*/
void SWT_ConfigurePins()
{
    // Configure SWTs as digital inputs.
    tris_SWT_SWT0 = 1;
    tris_SWT_SWT1 = 1;
    tris_SWT_SWT2 = 1;
    tris_SWT_SWT3 = 1;
    tris_SWT_SWT4 = 1;
    tris_SWT_SWT5 = 1;
    tris_SWT_SWT6 = 1;
    tris_SWT_SWT7 = 1;

    // disable analog (set pins as digital))
    ansel_SWT_SWT5 = 0;
    ansel_SWT_SWT6 = 0;
    ansel_SWT_SWT7 = 0;    
}

/***	SWT_GetValue
**
**	Parameters:
**		unsigned char bNo   - the number of switch  whose value will be read. The value must be between 0 and 7.
**
**	Return Value:
**		unsigned char   - the value corresponding to the specified switch:
**                                  0 when SW<bNo> is turned off
**                                  1 when SW<bNo> is turned on
**                      - 0xFF if bNo is not within 0 - 7.
**		
**
**	Description:
**		This function gets the value of the switch specified by bNo (0 or 1). 
**      If the value provided for bNo is not between 0 and 7, 0xFF is returned.
**          
*/
unsigned char SWT_GetValue(unsigned char bNo)
{
    unsigned bResult = 0xFF;

    switch (bNo)
    {
        case 0: 
            bResult = prt_SWT_SWT0;
            break;
        case 1: 
            bResult = prt_SWT_SWT1;
            break;
        case 2: 
            bResult = prt_SWT_SWT2;
            break;
        case 3: 
            bResult = prt_SWT_SWT3;
            break;
        case 4: 
            bResult = prt_SWT_SWT4;
            break;
        case 5: 
            bResult = prt_SWT_SWT5;
            break;
        case 6: 
            bResult = prt_SWT_SWT6;
            break;
        case 7: 
            bResult = prt_SWT_SWT7;
            break;
    }

    return bResult;
}

/***	SWT_GetGroupValue
**
**	Parameters:
**
**	Return Value:
**		unsigned char   - the 8 bit value B7 B6 B5 B4 B3 B2 B1 B0 where each bit Bi corresponds to LD<i>:
**                                  0 if LD<i> is turned off
**                                  1 if LD<i> is turned on
**		
**
**	Description:
**		This function gets the value of the all 8 switches as a single value on 8 bits.  
**      Each bit from returned value corresponds to a switch: Bit 0 (LSB) corresponds to SW0, bit 7 (MSB) corresponds to SW7.
**          
*/
unsigned char SWT_GetGroupValue()
{
    int i;
    unsigned char bResult = 0;
    for(i = 0; i < 8; i++)
    {
        if(SWT_GetValue(i))
        {
            bResult |= (1 << i);
        }
    }
    return bResult;
}
/* *****************************************************************************
 End of File
 */
