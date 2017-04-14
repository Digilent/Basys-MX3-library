/* ************************************************************************** */
/** Descriptive File Name

  @Company
    Digilent

  @File Name
    aic.c

  @Description
        This file groups the functions that implement the AIC (analog input 
        control library). 
        The library is used to handle analog input control on thumbwheel potentiometer 
        or analog input connectors labeled AIC. 
        The analog value is converted to digital value using 10 bits analog to digital 
        conversion implemented in ADC library. 
        Include the file as well as adc.c and adc.h in the project when this 
        library is needed.	
 
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
#include "adc.h"
#include "aic.h"


/* ************************************************************************** */

/* ------------------------------------------------------------ */
/***	AIC_Init
**
**	Parameters:
**		
**
**	Return Value:
**		
**
**	Description:
**		This function initializes the hardware involved in the AIC module: 
**      the ADC_AN2 pin is configured as analog input, the ADC module is 
**      configured by calling the ADC configuration function from adc.c.
**          
*/
void AIC_Init()
{
    AIC_ConfigurePins();
    ADC_Init();
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
**		This function configures the ADC_AN2 pin as analog input.
**      The function uses pin related definitions from config.h file.
**      This is a low-level function called by AIC_Init(), so user should avoid calling it directly.
**      
**          
*/
void AIC_ConfigurePins()
{
    // Configure AIC signal as analog input
    tris_ADC_AN2 = 1;     // set AN2 (RB2) as analog input pin 
    ansel_ADC_AN2 = 1;   // enable analog (set pins as analog)
}

/* ------------------------------------------------------------ */
/***	AIC_Val
**
**	Parameters:
**		
**
**	Return Value:
**		unsigned int    - the 16 LSB bits contain the result of analog to digital conversion of AIC value
**
**	Description:
**		This function returns the digital value corresponding to the AIC analog 
**      pin (thumbwheel potentiometer or analog input connectors labeled AIC)
**      as the result of analog to digital conversion performed by the ADC module. 
**      
**          
*/
unsigned int AIC_Val()
{
    return ADC_AnalogRead(2);    // Read the ADC Value for analog pin 2
}

/* *****************************************************************************
 End of File
 */
