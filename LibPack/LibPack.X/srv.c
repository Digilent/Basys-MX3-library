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




// FPB = 10000000
// Timer period 20 ms = 0.02
// Prescaler 16
//0.02*10000000/16 = 12500

void SRV_Init()
{
    SRV_ConfigurePins();
    SRV_ConfigureOCs();
}



#define TMR_TIME    0.02 // 20 ms 




unsigned int sPR2 = 0;

void SRV_ConfigureOCs()
{
   // configure Timer2
    T2CONbits.TCKPS = 4;                //            1:16 prescale value
    T2CONbits.TGATE = 0;                //             not gated input (the default)
    T2CONbits.TCS = 0;                  //             PCBLK input (the default)
    T2CONbits.ON = 1;                   //             turn on Timer2
    
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

void SRV_SetPulseMicroseconds1(unsigned short usVal)
{
    float fVal = ((float)(sPR2 + 1) * (float)usVal)/ (TMR_TIME * 1000000);
    OC5RS = (unsigned short)fVal; 
}

void SRV_SetPulseMicroseconds2(unsigned short usVal)
{
    float fVal = ((float)(sPR2 + 1) * (float)usVal)/ (TMR_TIME * 1000000);
    OC4RS = (unsigned short)fVal; 
}


void SRV_Close()
{
    OC5CONbits.ON = 0;       // Stop the OC2 module
    OC4CONbits.ON = 0;       // Stop the OC3 module   
}
/* *****************************************************************************
 End of File
 */
