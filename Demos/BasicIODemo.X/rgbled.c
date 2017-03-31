/* ************************************************************************** */
/** Descriptive File Name

  @Company
    Digilent

  @File Name
    filename.c

  @Description
        This file groups the functions that implement the RGBLED library.
        The colors are generated using PDM method, using accumulators updated 
        periodically (Timer5 is used).
        The source file also contains (commented), the PWM implementation, using 
        OC3, OC4, OC5 and Timer2.
        Include the file in the project when this library is needed, together with config.h.
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

// global variables to store R, G, B color values
volatile unsigned char bColR, bColG, bColB;


/***	Timer5ISR
**
**	Description:
**		This is the interrupt handler for Timer5. It is used to implement PDM (Pulse Density Modulation) values for each of R, G, B. 
**      One 16 bits accumulators are used, one for each color: R, G, B
**      The 8 bits component color value is added to the corresponding 16 bits accumulator.
**      The 9th bit of the accumulator is considered the carry bit. 
**      The resulted carry bits are assigned to the digital pins corresponding to each color (LED8_R, LED8_G and LED8_B) 
**      Carry gets often for large values and rarely for small values.
**      Carry bit is cleared in the accumulator.
**          
*/
void __ISR(_TIMER_5_VECTOR, ipl2) Timer5ISR(void) 
{  
   static unsigned short sAccR = 0, sAccG = 0, sAccB = 0;
    
    // add 8 bit color values over the accumulators
    sAccR += bColR;
    sAccG += bColG;
    sAccB += bColB;

    // take the 9'th bit (addition carry) as the PDM
    lat_LED8_R = (sAccR & 0x100) ? 1: 0;    
    lat_LED8_G = (sAccG & 0x100) ? 1: 0;
    lat_LED8_B = (sAccB & 0x100) ? 1: 0;
    
    // filter only 8 bits in the accumulator
    sAccR &= 0xFF;
    sAccG &= 0xFF;
    sAccB &= 0xFF;
    
    IFS0bits.T5IF = 0;                  // clear interrupt flag
}

// Timer period in seconds
#define TMR_TIME    0.0003 // 300 us for each tick

/* ------------------------------------------------------------ */
/***	Timer5Setup
**
**	Parameters:
**		
**
**	Return Value:
**		
**
**	Description:
**		This function configures the Timer5 to be used by RGBLED module.
**      The period constant is computed using TMR_TIME definition (located in this source file)
**      and peripheral bus frequency definition (PB_FRQ, located in config.h).
**          
*/
void Timer5Setup()
{
  INTDisableInterrupts();             // INT step 2: disable interrupts at CPU
                                      // INT step 3: setup peripheral
//  PR5 = 375;                        //             set period register, generates one interrupt every 300 us, for PBFreq 10 Mhz
  PR5 = (int)(((float)(TMR_TIME * PB_FRQ) / 256) + 0.5);                        //             set period register, generates one interrupt every 300 us
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

/* ------------------------------------------------------------ */
/***	RGBLED_Init
**
**	Parameters:
**		
**
**	Return Value:
**		
**
**	Description:
**		This function initializes the hardware involved in the RGBLED module: 
**      the pins corresponding to R, G and B colors are initialized as digital outputs and Timer 5 in configured.
**          
*/
void RGBLED_Init()
{
    RGBLED_ConfigurePins();
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

/* ------------------------------------------------------------ */
/***	RGBLED_ConfigurePins
**
**	Parameters:
**		
**
**	Return Value:
**		
**
**	Description:
**		This function configures the IO pins involved in the RGBLED module as digital output pins 
**      
**          
*/
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

/* ------------------------------------------------------------ */
/***	RGBLED_SetValue
**
**	Parameters:
**		unsigned char bValR   - the value that corresponds to R component of the color
**		unsigned char bValG   - the value that corresponds to G component of the color
**		unsigned char bValB   - the value that corresponds to B component of the color
**
**	Return Value:
**		
**
**	Description:
**		This function sets the color value by providing the values for the 3 components
**          R, G and B, as 3 separate 8 bits values. 
**          
*/
void RGBLED_SetValue(unsigned char bValR, unsigned char bValG, unsigned char bValB)
{
    bColR = bValR;
    bColG = bValG;
    bColB = bValB;
    
}

/* ------------------------------------------------------------ */
/***	RGBLED_SetValueGrouped
**
**	Parameters:
**		unsigned int uiValRGB   - the value that groups in the 3 LSB bytes the color values
**                                  in this pattern xxxxxxxxRRRRRRRRGGGGGGGGBBBBBBBB
**
**	Return Value:
**		
**
**	Description:
**		This function sets the color value by providing the values for the 3 components
**      R, G and B as a 24 bits value, placed in the LSB 3 bytes of the uiValRGB like this:
**      bits 23-16 correspond to color R, bits 15-8 correspond to color G, bits 7-0 (LSB byte) correspond to color B.
**          
*/
void RGBLED_SetValueGrouped(unsigned int uiValRGB)
{
    unsigned char *pBCol = (unsigned char *)&uiValRGB;
    RGBLED_SetValue(pBCol[2], pBCol[1], pBCol[0]);
}

/* ------------------------------------------------------------ */
/***	RGBLED_Close
**
**	Parameters:
**
**	Return Value:
**		
**
**	Description:
**		This function can be called when RGBLED library is no longer needed: 
**      it stops the Timer5 and turns off the RGBLED.
**          
*/
void RGBLED_Close()
{

    // stop the timer
      T5CONbits.ON = 0;                   //             turn off Timer1
  // turn off colors
    lat_LED8_R = 0;
    lat_LED8_G = 0;
    lat_LED8_B = 0;
}


/* *****************************************************************************
 End of File
 */
