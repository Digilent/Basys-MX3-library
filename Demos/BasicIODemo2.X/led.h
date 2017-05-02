/* ************************************************************************** */
/** Descriptive File Name

  @Company
    Digilent

  @File Name
    led.h

  @Description
        This file groups the declarations of the functions that implement
        the LED library (defined in led.c).
        It also provides the definitions of the fast access SetValue macros.
        Include the file in the project when this library is needed.
        Use #include "led.h" in the source files where the functions are needed.
 */
/* ************************************************************************** */

#ifndef _LED_H    /* Guard against multiple inclusion */
#define _LED_H

#include "config.h"

//LEDxSetValue fast access function macros

/* ------------------------------------------------------------ */
/***	LEDxSetValue
**
**	Parameters:
**      unsigned char bVal  - the value to be assigned to the specific LED: 
**                                  0    : LD<x> is turned off
**                                  1    : LD<x> is turned on
**
**	Return Value:
**		
**
**	Description:
**		For each LED there is a macro function that provides the fastest way to set its value.  
**      The bVal parameter specifies the value for the LED (0 for off, 1 for on). 
**          
*/
#define LEDS_Led0SetValue(val) lat_LEDS_LED0 = val
#define LEDS_Led1SetValue(val) lat_LEDS_LED1 = val
#define LEDS_Led2SetValue(val) lat_LEDS_LED2 = val
#define LEDS_Led3SetValue(val) lat_LEDS_LED3 = val
#define LEDS_Led4SetValue(val) lat_LEDS_LED4 = val
#define LEDS_Led5SetValue(val) lat_LEDS_LED5 = val
#define LEDS_Led6SetValue(val) lat_LEDS_LED6 = val
#define LEDS_Led7SetValue(val) lat_LEDS_LED7 = val

// function prototypes
void LED_Init();

void LED_SetValue(unsigned char bNo, unsigned char bVal);
void LED_ToggleValue(unsigned char bNo);
void LED_SetGroupValue(unsigned char bVal);

//private functions:
void LED_ConfigurePins();    


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
#endif /* _LED_H */

/* *****************************************************************************
 End of File
 */
