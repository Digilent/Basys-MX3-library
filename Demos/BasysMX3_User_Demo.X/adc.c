/* ************************************************************************** */
/** Descriptive File Name

  @Company
    Company Name

  @File Name
    filename.c

  @Summary
    Brief description of the file.

  @Description
    Describe the purpose of this file.
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

void ADC_ConfigureAnalogInput(unsigned char channelNumber)
{
	AD1CHS = (channelNumber & 0xFFFF) << 16;
	AD1CON1	=	0; 
//    AD1CON1bits.SSRC = 0;   // Clearing SAMP bit ends sampling and starts conversion
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

void ADC_adcConfigureManual(){
//	AD1CHS = (2 & 0xFFFF) << 16;
	AD1CON1	=	0; 
//    AD1CON1bits.SSRC = 0;   // Clearing SAMP bit ends sampling and starts conversion
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
} // END adcConfigureManual()
void ADC_Init()
{
//    AD1CSSLbits.CSSL2 = 1;
    ADC_ConfigurePins();
    ADC_adcConfigureManual();
}

void ADC_ConfigurePins()
{
    // Configure ADC signals as digital inputs
    
    tris_ADC_AN2 = 1;     // set AN2 (RB2) as analog input pin 
    ansel_ADC_AN2 = 1;   // enable analog (set pins as analog)
    
    TRISGbits.TRISG9 = 1;
    ANSELGbits.ANSG9 = 1;
}

unsigned int POT_Val()
{
    return ADC_analogRead(2);    // Read the ADC Value
}



int ADC_analogRead(char analogPIN)
{
    int adc_val = 0;
    
//    INTDisableInterrupts();             // INT step 2: disable interrupts at CPU
    IEC0bits.T2IE = 0;
    AD1CHS = analogPIN << 16;       // AD1CHS<16:19> controls which analog pin goes to the ADC
 
    AD1CON1bits.SAMP = 1;           // Begin sampling
    while( AD1CON1bits.SAMP );      // wait until acquisition is done
    while( ! AD1CON1bits.DONE );    // wait until conversion done
 
    adc_val = ADC1BUF0;
    
//    INTEnableSystemMultiVectoredInt();  // INT step 7: enable interrupts at CPU
    IEC0bits.T2IE = 1;
    return adc_val;
}

/* *****************************************************************************
 End of File
 */
