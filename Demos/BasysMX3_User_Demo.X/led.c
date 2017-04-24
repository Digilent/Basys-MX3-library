/* ************************************************************************** */
/** Descriptive File Name

  @Company
    Digilent

  @File Name
    led.c

  @Description
        This file groups the functions that implement the LED library.
        The functions implement basic digital output functionality.
        The library also provides a set of fast access SetValue function macros, one for each LED, defined in led.h.
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
#include "led.h"

/* ************************************************************************** */

/* ------------------------------------------------------------ */
/***	LED_Init
**
**	Parameters:
**		
**
**	Return Value:
**		
**
**	Description:
**		This function initializes the hardware involved in the LED module. 
**      The pins corresponding to LEDs are initialized as digital outputs.
**      All the LEDs are turned off.
** 
*/
void LED_Init()
{
    LED_ConfigurePins();
    LED_SetGroupValue(0);    
}

/* ------------------------------------------------------------ */
/***	LED_ConfigurePins
**
**	Parameters:
**		
**
**	Return Value:
**		
**
**	Description:
**		This function configures the IO pins involved in the LED module as digital output pins. 
**      The function uses pin related definitions from config.h file.
**      This is a low-level function called by LED_Init(), so user should avoid calling it directly.
**      
**          
*/
void LED_ConfigurePins()
{
    // Configure LEDs as digital outputs.
    tris_LEDS_GRP &= ~msk_LEDS_GRP;
}

/* ------------------------------------------------------------ */
/***	LED_SetValue
**
**	Parameters:
**		unsigned char bNo   - the number of led whose value will be set. The value must be between 0 and 7.
**      unsigned char bVal  - the value to be assigned to the specific LED: 
**                                  0               : LD<bNo> is turned off
**                                  Other than 0    : LD<bNo> is turned on
**
**	Return Value:
**		
**
**	Description:
**		This function sets the LED specified by bNo to the value specified by bVal. 
**      If the value provided for bNo is not between 0 and 7, the function does nothing.
**          
*/
void LED_SetValue(unsigned char bNo, unsigned char bVal)
{
    bVal = bVal ? 1: 0;
    if(bNo == (bNo & 0x07))
    {
        switch (bNo)
        {
            case 0:
                LEDS_Led0SetValue(bVal);
                break;
            case 1:
                LEDS_Led1SetValue(bVal);
                break;
            case 2:
                LEDS_Led2SetValue(bVal);
                break;
            case 3:
                LEDS_Led3SetValue(bVal);
                break;
            case 4:
                LEDS_Led4SetValue(bVal);
                break;
            case 5:
                LEDS_Led5SetValue(bVal);
                break;
            case 6:
                LEDS_Led6SetValue(bVal);
                break;
            case 7:
                LEDS_Led7SetValue(bVal);
                break;
        }
    }
}


/* ------------------------------------------------------------ */
/***	LED_ToggleValue
**
**	Parameters:
**		unsigned char bNo   - the led whose value will be toggled. The value must be between 0 and 7.
**
**	Return Value:
**		
**
**	Description:
**		This function toggles the value of the LED specified by bNo.  If the LED was off, it is turned on. If the LED was on, it is tuned off. 
**      If the value provided for bNo is not between 0 and 7, the function does nothing.
**          
*/
void LED_ToggleValue(unsigned char bNo)
{
    if(bNo == (bNo & 0x07))
    {
        latinv_LEDS_GRP = (1<<bNo);
    }
}

/* ------------------------------------------------------------ */
/***	LED_SetGroupValue
**
**	Parameters:
**		unsigned char bVal   - the 8 bit value B7 B6 B5 B4 B3 B2 B1 B0  that specifies the values for the 8 LEDs
**                              each bit Bi:
**                                   0   LD<i> is turned off
**                                   1   LD<i> is turned on
**
**	Return Value:
**		
**
**	Description:
**		This function sets the value for all 8 LEDs, according to the value provided in bVal.  
**      Each bit from bVal corresponds to an LED: Bit 0 (LSB) corresponds to LD0, bit 7 (MSB) corresponds to LD7.
**          
*/
void LED_SetGroupValue(unsigned char bVal)
{
    unsigned char *pLedData = (unsigned char *)lat_LEDS_GRP_ADDR;
    *pLedData = bVal;    
}


/* *****************************************************************************
 End of File
 */
