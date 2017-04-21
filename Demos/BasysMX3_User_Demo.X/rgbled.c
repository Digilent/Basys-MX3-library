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
#include "rgbled.h"

unsigned char bColR, bColG, bColB, bCnt;


// Generate PDM (Pulse Density Modulation) values for each of R, G, B.
// This is done by using an accumulator where the 8 bit color value is added. 
// The Carry bit resulting from adding the 8 value color to the accumulator (the 9's bit) is the PDM value
// Carry gets often for large values and rarely for small values
void __ISR(_TIMER_5_VECTOR, ipl2) Timer5ISR(void) 
{  
   static unsigned short sAccR = 0, sAccG = 0, sAccB = 0;
    
    // add 8 bit color values over the accumulators
    sAccR += bColR;
    sAccG += bColG;
    sAccB += bColB;

    // thake the 9'th bit (addition carry) as the PDM
    lat_LED8_R = (sAccR & 0x100) ? 1: 0;    
    lat_LED8_G = (sAccG & 0x100) ? 1: 0;
    lat_LED8_B = (sAccB & 0x100) ? 1: 0;

    
//    LATDbits.LATD11 = (sAccR & 0x100) ? 1: 0;   // debug, same as red
    
    // filter only 8 bits in the accumulator
    sAccR &= 0xFF;
    sAccG &= 0xFF;
    sAccB &= 0xFF;
    
    
    LATDINV = (1<<9);   //debug 
    IFS0bits.T5IF = 0;                  // clear interrupt flag
}

void Timer5Setup()
{
  INTDisableInterrupts();             // INT step 2: disable interrupts at CPU
                                      // INT step 3: setup peripheral
  PR5 = 3000;                        //             set period register, generates one interrupt every 300 us
  TMR5 = 0;                           //             initialize count to 0
  T5CONbits.TCKPS = 3;                //            1:256 prescale value
  T5CONbits.TGATE = 0;                //             not gated input (the default)
  T5CONbits.TCS = 0;                  //             PCBLK input (the default)
  T5CONbits.ON = 1;                   //             turn on Timer1
  IPC5bits.T5IP = 2;                  // INT step 4: priority
  IPC5bits.T5IS = 0;                  //             subpriority
  IFS0bits.T5IF = 0;                  // INT step 5: clear interrupt flag
  IEC0bits.T5IE = 1;                  // INT step 6: enable interrupt
  INTEnableSystemMultiVectoredInt();  // INT step 7: enable interrupts at CPU
}
void RGBLED_Init()
{
    RGBLED_ConfigurePins();
    bCnt = 0;
    Timer5Setup();
    lat_LED8_R = 0;
    lat_LED8_G = 0;
    lat_LED8_B = 0;
                          
    /*
       // configure Timer2
//    T2CONbits.TCKPS = 0;                //            1:1 prescale value
    T2CONbits.TCKPS = 3;                //            1:64 prescale value
    T2CONbits.TGATE = 0;                //             not gated input (the default)
    T2CONbits.TCS = 0;                  //             PCBLK input (the default)
    T2CONbits.ON = 1;                   //             turn on Timer2

    PR2 = 4096;

    // Configure Output Compare Module 3
   OC3CONbits.ON = 0;       // Turn off OC3 while doing setup.
   OC3CONbits.OCM = 6;      // PWM mode on OC3; Fault pin is disabled
   OC3CONbits.OCTSEL = 0;   // Timer2 is the clock source for this Output Compare module
   OC3CONbits.ON = 1;       // Start the OC3 module
   
    // Configure Output Compare Module 4
   OC4CONbits.ON = 0;       // Turn off OC4 while doing setup.
   OC4CONbits.OCM = 6;      // PWM mode on OC4; Fault pin is disabled
   OC4CONbits.OCTSEL = 0;   // Timer2 is the clock source for this Output Compare module
   OC4CONbits.ON = 1;       // Start the OC4 module

    // Configure Output Compare Module 5
   OC5CONbits.ON = 0;       // Turn off OC5 while doing setup.
   OC5CONbits.OCM = 6;      // PWM mode on OC5; Fault pin is disabled
   OC5CONbits.OCTSEL = 0;   // Timer2 is the clock source for this Output Compare module
   OC5CONbits.ON = 1;       // Start the OC5 module  
     */
}

void RGBLED_ConfigurePins()
{
//  tris_PMODS_JB2 = 0;     // output, debug  
    // Configure RGBLEDs as digital outputs.

//    rp_LED8_R = 0x0B; // LED8_R RPD2 is OC3 
    rp_LED8_R = 0;      // no remapable
    tris_LED8_R = 0;    // output
  
    //RPD12R 1011 = OC5
//   rp_LED8_G = 0x0B; // LED8_G RPD12 is OC5
    rp_LED8_G = 0;      // no remapable
    tris_LED8_G = 0;    // output
 
//    rp_LED8_B = 0x0B; // LED8_B RPD3 is OC4
    rp_LED8_B = 0;      // no remapable
    tris_LED8_B = 0;    // output
    
    // disable analog (set pins as digital))
    ansel_LED8_R = 0;
    ansel_LED8_B = 0;
}


void RGBLED_SetValue(unsigned char bValR, unsigned char bValG, unsigned char bValB)
{
    bColR = bValR;
    bColG = bValG;
    bColB = bValB;
    
}

void RGBLED_SetValueGrouped(unsigned int uiValRGB)
{
    unsigned char *pBCol = (unsigned char *)&uiValRGB;
    RGBLED_SetValue(pBCol[2], pBCol[1], pBCol[0]);
}




/* *****************************************************************************
 End of File
 */
