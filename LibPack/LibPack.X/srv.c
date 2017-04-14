/* ************************************************************************** */
/** Descriptive File Name

  @Company
    Digilent

  @File Name
    srv.c

  @Description
        This file groups the functions that implement the SRV library.
        This library handles the 2 servo modules SERVO 1 and SERVO 2.
        Servo pins are mapped over OC5 and OC4 that are properly configured (period 20 ms) 
        in order to generate PWM for each servo. For each servo, the control function allows 
        to set the pulse duration in microseconds.
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
#include "srv.h"
// FPB = 80000000
// Timer period 20 ms = 0.02
// Prescaler 16
#define TMR_TIME    0.02 // 20 ms 

unsigned int sPR2 = 0;
/* ------------------------------------------------------------ */
/***	SRV_Init
**
**	Parameters:
**		
**
**	Return Value:
**		
**
**	Description:
**		This function initializes the hardware involved in the SRV module: 
**      The Servo 1 PWM and Servo 2 PWM pins are configured as digital out and mapped to OC5 and OC4. 
**      The OC5 and OC4 module of PIC32 are configured with a period of 20 ms given by Timer2.
**          
*/
void SRV_Init()
{
    SRV_ConfigurePins();
    SRV_ConfigureOCs();
}

/* ------------------------------------------------------------ */
/***	SRV_ConfigureOCs
**
**	Parameters:
**		
**
**	Return Value:
**		
**
**	Description:
**		This function configures the output compares and timer involved in the SRV module.
**      The OC5 and OC4 module of PIC32 are configured with a period of 20 ms given by Timer2.
**      This is a low-level function called by SRV_Init(), so user should avoid calling it directly.
**          
*/
void SRV_ConfigureOCs()
{
   // configure Timer2
    T2CONbits.TCKPS = 4;                // 1:16 prescale value
    T2CONbits.TGATE = 0;                // not gated input (the default)
    T2CONbits.TCS = 0;                  // PCBLK input (the default)
    T2CONbits.ON = 1;                   // turn on Timer2
    
    sPR2 = (int)(((float)(TMR_TIME * PB_FRQ) / 16) + 0.5);
    PR2 = sPR2;

    // Configure Output Compare Module 2
   OC5CONbits.ON = 0;       // Turn off OC5 while doing setup.
   OC5CONbits.OCM = 6;      // PWM mode on OC5; Fault pin is disabled
   OC5CONbits.OCTSEL = 0;   // Timer2 is the clock source for this Output Compare module
   OC5CONbits.ON = 1;       // Start the OC5 module
   
    // Configure Output Compare Module 3
   OC4CONbits.ON = 0;       // Turn off OC4 while doing setup.
   OC4CONbits.OCM = 6;      // PWM mode on OC4; Fault pin is disabled
   OC4CONbits.OCTSEL = 0;   // Timer2 is the clock source for this Output Compare module
   OC4CONbits.ON = 1;       // Start the OC4 module
}

/* ------------------------------------------------------------ */
/***	SRV_ConfigurePins
**
**	Parameters:
**		
**
**	Return Value:
**		
**
**	Description:
**		This function configures the digital pins involved in the SRV module:
**      The servo 1 and servo 2 pins are defined as digital out and mapped to OC5 and OC4. 
**      The function uses pin related definitions from config.h file.
**      This is a low-level function called by SRV_Init(), so user should avoid calling it directly.
**          
*/
void SRV_ConfigurePins()
{

    // Configure SRVs digital outputs.
    tris_SRV_S0PWM = 0;
    tris_SRV_S1PWM = 0;
    
    // disable analog (set pins as digital))
    ansel_SRV_S0PWM = 0;
    
    // remap pins to OCs
    rp_SRV_S0PWM = 0x0B; // 1011 = OC5
    rp_SRV_S1PWM = 0x0B; // 1011 = OC4    
}

/* ------------------------------------------------------------ */
/***	SRV_SetPulseMicroseconds1
**
**	Parameters:
**		unsigned short usVal - the pulse width in microseconds
**                              
**	Return Value:
**		
**
**	Description:
**		This function configures the output compare 5 (corresponding to servo 1) 
**      according to the specified pulse width.
**          
*/
void SRV_SetPulseMicroseconds1(unsigned short usVal)
{
    float fVal = ((float)(sPR2 + 1) * (float)usVal)/ (TMR_TIME * 1000000);
    OC5RS = (unsigned short)fVal; 
}

/* ------------------------------------------------------------ */
/***	SRV_SetPulseMicroseconds2
**
**	Parameters:
**		unsigned short usVal - the pulse width in microseconds
**                              
**	Return Value:
**		
**
**	Description:
**		This function configures the output compare 4 (corresponding to servo 2) 
**      according to the specified pulse width.
**          
*/
void SRV_SetPulseMicroseconds2(unsigned short usVal)
{
    float fVal = ((float)(sPR2 + 1) * (float)usVal)/ (TMR_TIME * 1000000);
    OC4RS = (unsigned short)fVal; 
}

/* ------------------------------------------------------------ */
/***	SRV_Close
**
**	Parameters:
** 
**
**	Return Value:
**      
**
**	Description:
**		This functions releases the hardware involved in the SRV library: 
**      it turns off the OC5, OC4 and Timer 2 modules.
**      
**          
*/
void SRV_Close()
{
    OC5CONbits.ON = 0;       // Stop the OC2 module
    OC4CONbits.ON = 0;       // Stop the OC3 module   
    T2CONbits.ON = 0;        // turn off Timer2
}
/* *****************************************************************************
 End of File
 */
