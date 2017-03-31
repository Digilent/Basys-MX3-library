
/** Descriptive File Name

  @Company
    Digilent

  @File Name
    mmic.c

  @Description
        This file groups the functions that implement the MIC.
        The library is used to handle analog input from the microphone. 
        It relies on the ADC module. 
        Include the file as well as adc.c and adc.h in the project when this 
        library is needed.	
 
   @Author
    Cristian Ignat 
 */

#include <xc.h>
#include "adc.h"
#include "mic.h"
#include "config.h"

/* ------------------------------------------------------------ */
/***	MIC_Init
**
**	Parameters:
**		
**
**	Return Value:
**		
**
**	Description:
**		This function initializes the hardware involved in the MIC module: 
**      the MIC pin is configured as analog input, the ADC module is 
**      configured by calling the ADC configuration function from adc.c.
**          
*/
void MIC_Init()
{
    MIC_ConfigurePins();
    ADC_ConfigureAnalogInputManual();
}

/* ------------------------------------------------------------ */
/***	MIC_ConfigurePins
**
**	Parameters:
**		
**
**	Return Value:
**		
**
**	Description:
**		This function configures the pins involved in the AIC module: 
**      The following pins are configured as analog input: MIC.
**      The function uses pin related definitions from config.h file.
**      
**          
*/
void MIC_ConfigurePins()
{
    // Configure ADC signal as analog input
   
    tris_MIC = 1;     // set AN4 (RB4) as analog input pin 
    ansel_MIC = 1;   // enable analog (set pins as analog)
}

/* ------------------------------------------------------------ */
/***	MIC_Val
**
**	Parameters:
**		
**
**	Return Value:
**		unsigned int    - the result of analog to digital conversion of MIC value
**
**	Description:
**		This function returns the digital value corresponding to the MIC analog 
**      pin. 
**      It can be used to sample the microphone input.
**      
**          
*/
unsigned int MIC_Val()
{
    return ADC_AnalogRead(4);   // Read the ADC Value for analog pin 4
}





