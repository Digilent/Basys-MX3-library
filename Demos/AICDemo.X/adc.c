/* ************************************************************************** */
/** Descriptive File Name

  @Company
    Digilent

  @File Name
    adc.c

  @Description
        This file groups the functions that implement the ADC module.
        The ADC module implements basic ADC (analog to Digital converter) functionality.
        This module is used by AIC and MIC libraries.
        Include this file in the project when the AIC and MIC libraries are needed.
 
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

/* ************************************************************************** */

/***	ADC_ConfigureAnalogInputManual
**
**	Parameters:
**		
**
**	Return Value:
**		
**
**	Description:
**		This function initializes the analog to digital converter module 
**      in manual sampling mode.
**          
*/
void ADC_ConfigureAnalogInputManual()
{

	AD1CON1	=	0; 
    AD1CON1bits.SSRC = 7;   // Internal counter ends sampling and starts conversion (auto convert)
    AD1CON1bits.FORM = 0;   // Integer 16-bit
	/* Set up for manual sampling
	*/
	AD1CSSL	=	0;
	AD1CON3	=	0x0002;	//Tad = internal 6 Tpb
	AD1CON2	=	0;
    AD1CON2bits.VCFG = 0;   // VREFH = AVDD / VREFL = AVss

	/* Turn on ADC
	*/
    AD1CON1bits.ON = 1;
} 


/***	ADC_AnalogRead
**
**	Parameters:
**		unsigned char analogPIN - the number of the analog pin that must be read
**
**	Return Value:
**		
**
**	Description:
**		This function returns the digital value corresponding to the analog pin, 
**      as the result of analog to digital conversion performed by the ADC module. 
**          
*/
unsigned int ADC_AnalogRead(unsigned char analogPIN)
{
    int adc_val = 0;
    
    IEC0bits.T2IE = 0;
    AD1CHS = analogPIN << 16;       // AD1CHS<16:19> controls which analog pin goes to the ADC
 
    AD1CON1bits.SAMP = 1;           // Begin sampling
    while( AD1CON1bits.SAMP );      // wait until acquisition is done
    while( ! AD1CON1bits.DONE );    // wait until conversion done
 
    adc_val = ADC1BUF0;
    
    IEC0bits.T2IE = 1;
    return adc_val;
}

/* *****************************************************************************
 End of File
 */
