/* ************************************************************************** */
/** Descriptive File Name

  @Company
    Digilent

  @File Name
    ssd.c

  @Description
        This file groups the functions that implement the SSD library.
        This library deals with seven segment display modules.
        As digits share the cathodes, in order to be able to display different information on each digit, 
        periodically each digit is refreshed, while the other are disabled (Timer1 is used to generate an interrupt).
        The library provides functions for setting the information to be displayed.
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
#include "ssd.h"


/* ************************************************************************** */

// the array that contains the LEDs configuration for each value to be displayed
// Bit 0 corresponds to SSD_CA, 0 for segment ON, 1 for segment off
// Bit 1 corresponds to SSD_CB, 0 for segment ON, 1 for segment off
// Bit 2 corresponds to SSD_CC, 0 for segment ON, 1 for segment off
// Bit 3 corresponds to SSD_CD, 0 for segment ON, 1 for segment off
// Bit 4 corresponds to SSD_CE, 0 for segment ON, 1 for segment off
// Bit 5 corresponds to SSD_CF, 0 for segment ON, 1 for segment off
// Bit 6 corresponds to SSD_CG, 0 for segment ON, 1 for segment off
// Bit 7 corresponds to SSD_CH, 0 for segment ON, 1 for segment off
const unsigned char digitSegments[]= {
    0b1000000, // 0
    0b1111001, // 1
    0b0100100, // 2
    0b0110000, // 3
    0b0011001, // 4
    0b0010010, // 5
    0b0000010, // 6
    0b1111000, // 7
    0b0000000, // 8
    0b0010000, // 9
    0b0001000, // A
    0b0000011, // b
    0b1000110, // C
    0b0100001, // d
    0b0000110, // E
    0b0001110,  // F
    0b0001001   // H
};

#define NO_DIGITS sizeof(digitSegments)/sizeof(digitSegments[0])
unsigned char digits[4];
#define TMR_TIME    0.003 // 3000 us for each tick

/***	Timer1ISR
**
**	Description:
**		This is the interrupt handler for Timer1. It is used to implement proper SSD display.
**      Every time the interrupt occurs, the next of the 4 digits is displayed (in a circular manner).
**      First it deactivates all digits (anodes), then it drives the segments (cathodes) according to the digit that is displayed,
**      then the current digit is activated. 
**      This happens faster than the human eye can notice.
**          
*/
void __ISR(_TIMER_1_VECTOR, ipl7) Timer1ISR(void) 
{  
    static unsigned char idxCurrDigit = 0;
    unsigned char currDigit, idx;

    idx = (idxCurrDigit++) & 3;
    currDigit = digits[idx];
//     1. deactivate all digits (anodes)
    lat_SSD_AN1 = 1; // deactivate digit 1;
    lat_SSD_AN2 = 1; // deactivate digit 2;    
    lat_SSD_AN3 = 1; // deactivate digit 3;   
    lat_SSD_AN0 = 1; // deactivate digit 0;
    
    // 2. drive all the segments (cathodes)
    lat_SSD_CA = currDigit & 0x01;
    lat_SSD_CB = (currDigit & 0x02) >> 1;
    lat_SSD_CC = (currDigit & 0x04) >> 2;
    lat_SSD_CD = (currDigit & 0x08) >> 3;    
    lat_SSD_CE = (currDigit & 0x10) >> 4;
    lat_SSD_CF = (currDigit & 0x20) >> 5;
    lat_SSD_CG = (currDigit & 0x40) >> 6;
    lat_SSD_DP = (currDigit & 0x80) >> 7;     
    
    // 3. activate the current digit (anodes)
    
    switch(idx)
    {
        case 0:
            lat_SSD_AN0 = 0; // activate digit 0;        
            break;
        case 1:
           lat_SSD_AN1 = 0; // activate digit 1;
            break;
        case 2:
           lat_SSD_AN2 = 0; // activate digit 2;
            break;    
        case 3:
            lat_SSD_AN3 = 0; // activate digit 3;   
            break; 
    }    
    IFS0bits.T1IF = 0;       // clear interrupt flag
}

/* ------------------------------------------------------------ */
/***	Timer1Setup
**
**	Parameters:
**		
**
**	Return Value:
**		
**
**	Description:
**		This function configures the Timer1 to be used by SSD module.
**      The timer will generate interrupts every 3 ms.
**      The period constant is computed using TMR_TIME definition (located in this source file)
**      and peripheral bus frequency definition (PB_FRQ, located in config.h).
**      This is a low-level function called by SSD_Init(), so user should avoid calling it directly. 
**          
*/
void SSD_Timer1Setup()
{
  PR1 = (int)(((float)(TMR_TIME * PB_FRQ) / 256) + 0.5); //set period register, generates one interrupt every 3 ms
  TMR1 = 0;                           //    initialize count to 0
  T1CONbits.TCKPS = 2;                //    1:64 prescale value
  T1CONbits.TGATE = 0;                //    not gated input (the default)
  T1CONbits.TCS = 0;                  //    PCBLK input (the default)
  T1CONbits.ON = 1;                   //    turn on Timer1
  IPC1bits.T1IP = 7;                  //    priority
  IPC1bits.T1IS = 3;                  //    subpriority
  IFS0bits.T1IF = 0;                  //    clear interrupt flag
  IEC0bits.T1IE = 1;                  //    enable interrupt
  macro_enable_interrupts();          //    enable interrupts at CPU
}
/* ************************************************************************** */
/* ************************************************************************** */
// Section: Local Functions                                                   */
/* ************************************************************************** */
/* ************************************************************************** */

/* ------------------------------------------------------------ */
/***	SSD_Init
**
**	Parameters:
**		
**
**	Return Value:
**		
**
**	Description:
**		This function initializes the hardware involved in the SSD module:
**      The pins corresponding to SSD module are initialized as digital outputs.
**      The Timer1 is initialized to generate interrupts every approx. 3 ms.
**      
**          
*/
void SSD_Init()
{
    SSD_ConfigurePins();
    SSD_Timer1Setup();  
}

/* ------------------------------------------------------------ */
/***	SSD_ConfigurePins
**
**	Parameters:
**		
**
**	Return Value:
**		
**
**	Description:
**		This function configures the IO pins involved in the SSD module as digital output pins. 
**      The function uses pin related definitions from config.h file.
**      This is a low-level function called by SSD_Init(), so user should avoid calling it directly.       
**          
*/
void SSD_ConfigurePins()
{
    // set pins as digital outputs.
    tris_SSD_CA = 0;
    tris_SSD_CB = 0;
    tris_SSD_CC = 0;
    tris_SSD_CD = 0;  
    tris_SSD_CE = 0;
    tris_SSD_CF = 0;
    tris_SSD_CG = 0;
    tris_SSD_DP = 0;  
    
    tris_SSD_AN0 = 0;
    tris_SSD_AN1 = 0;
    tris_SSD_AN2 = 0;
    tris_SSD_AN3 = 0;    
    
    // disable analog (set pins as digital))    
    ansel_SSD_AN0 = 0;
    ansel_SSD_AN1 = 0;
    
    PMCONbits.ON = 0;   // turn PM off
}


/* ------------------------------------------------------------ */
/***	SSD_GetDigitSegments
**
**	Parameters:
**		unsigned char d - the value to be represented on a digit of SSD. Should be between 0 and 16:
**                               0 -  9     - corresponds to 0-9 letter
**                              10 - 15     - corresponds to A-F letters
**                              16          - corresponds to H letter
**
**	Return Value:
**       unsigned char - value containing segments configuration. Bits 0-6 correspond to the seven segments:
**                          Bit 0 corresponds to SSD_CA, 0 for segment ON, 1 for segment off
**                          Bit 1 corresponds to SSD_CB, 0 for segment ON, 1 for segment off
**                          Bit 2 corresponds to SSD_CC, 0 for segment ON, 1 for segment off
**                          Bit 3 corresponds to SSD_CD, 0 for segment ON, 1 for segment off
**                          Bit 4 corresponds to SSD_CE, 0 for segment ON, 1 for segment off
**                          Bit 5 corresponds to SSD_CF, 0 for segment ON, 1 for segment off
**                          Bit 6 corresponds to SSD_CG, 0 for segment ON, 1 for segment off
**                      - 0xFF if d is outside 0 - 16
**
**	Description:
**		This function returns the byte containing the segments configuration for the digit received as parameter.
**      The segments configuration digits are defined in digitSegments.
**      If d is outside 0 - 16, then 0xFF is returned.
**          
*/
unsigned char SSD_GetDigitSegments(unsigned char d)
{
    unsigned char bResult = -1;
    if(d < NO_DIGITS)
    {
        bResult = digitSegments[d];
    }
    return bResult;
}


/* ------------------------------------------------------------ */
/***	SSD_WriteDigits
**
**	Parameters:
**		unsigned char d1 - the value to be represented on first (rightmost) digit of SSD. Should be between 0 and 16:
**                               0 -  9     - corresponds to 0-9 letter
**                              10 - 15     - corresponds to A-F letters
**                              16          - corresponds to H letter
**		unsigned char d2 - the value to be represented on second digit of SSD. Should be between 0 and 16:
**                               0 -  9     - corresponds to 0-9 letter
**                              10 - 15     - corresponds to A-F letters
**                              16          - corresponds to H letter
**		unsigned char d3 - the value to be represented on third digit of SSD. Should be between 0 and 16:
**                               0 -  9     - corresponds to 0-9 letter
**                              10 - 15     - corresponds to A-F letters
**                              16          - corresponds to H letter
**		unsigned char d4 - the value to be represented on fourth (leftmost) digit of SSD. Should be between 0 and 16:
**                               0 -  9     - corresponds to 0-9 letter
**                              10 - 15     - corresponds to A-F letters
**                              16          - corresponds to H letter
**		unsigned char dp1 - the value corresponding to the decimal point for the first (rightmost) digit of SSD
**                               0 - digital point is off
**                               1 - digital point is on
**		unsigned char dp1 - the value corresponding to the decimal point for the second digit of SSD
**                               0 - digital point is off
**                               1 - digital point is on
**		unsigned char dp1 - the value corresponding to the decimal point for the third digit of SSD
**                               0 - digital point is off
**                               1 - digital point is on
**		unsigned char dp1 - the value corresponding to the decimal point for the fourth (leftmost) digit of SSD
**                               0 - digital point is off
**                               1 - digital point is on
**
**	Return Value:
**              
**
**	Description:
**		This function sets the 4 values and 4 decimal points to be displayed on the 4 SSD digits.
**      If d1, d2, d3 or d4 is outside 0 - 16, the corresponding SSD digits will display nothing. 
**      (all segments off, corresponding to 0xFF value for segments configuration)
**          
*/
void SSD_WriteDigits(unsigned char d1, unsigned char d2, unsigned char d3, unsigned char d4, \
        unsigned char dp1, unsigned char dp2, unsigned char dp3, unsigned char dp4)
{
      T1CONbits.ON = 0;                   // turn off Timer1
    digits[0] = SSD_GetDigitSegments(d1);
    digits[1] = SSD_GetDigitSegments(d2);
    digits[2] = SSD_GetDigitSegments(d3);
    digits[3] = SSD_GetDigitSegments(d4);
    
    if(!dp1)
    {
        digits[0] |= 0x80;
    }
    if(!dp2)
    {
        digits[1] |= 0x80;
    }
    if(!dp3)
    {
        digits[2] |= 0x80;
    }
    if(!dp4)
    {
        digits[3] |= 0x80;
    }    
  T1CONbits.ON = 1;                   //  turn on Timer1
}

/* ------------------------------------------------------------ */
/***	SSD_WriteDigitsGrouped
**
**	Parameters:
**		unsigned int val - each of the 4 bytes contains the value to be displayed on a SSD digit:
**                              Byte 0 - the value to be displayed on first (rightmost) digit of SSD
**                              Byte 1 - the value to be displayed on second digit of SSD
**                              Byte 2 - the value to be displayed on third digit of SSD
**                              Byte 3 - the value to be displayed on fourth (leftmost) digit of SSD
**      unsigned char dp - each of the 4 LSB bits contains the value corresponding to a decimal point of a SSD digit:
**                              Bit 0 (LSB) - the digital point corresponding to the first (rightmost) digit of SSD
**                              Bit 1 - the digital point corresponding to the second digit of SSD
**                              Bit 2 - the digital point corresponding to the third digit of SSD
**                              Bit 3 - the digital point corresponding to the fourth (leftmost) digit of SSD * 
**
**	Return Value:
**              
**
**	Description:
**		This function sets the 4 values and 4 decimal points to be displayed on the 4 SSD digits.
**      Each of the 4 bytes of val parameter contains the value to be displayed on a SSD digit (as detailed above), and 
**      each of the 4 LSB bits of dp parameter contains the value corresponds to decimal point of a SSD digit.
**      If a value in the 4 bytes of val is outside 0 - 16, then the corresponding SSD digits will display nothing 
 *      (all segments off, corresponding to 0xFF value for segments configuration)
**          
*/
void SSD_WriteDigitsGrouped(unsigned int val, unsigned char dp)
{
    SSD_WriteDigits(val & 0xF, (val & 0xF0) >> 4, (val & 0xF00) >> 8, (val & 0xF000) >> 12,
            dp&1, (dp & 2) >> 1, (dp & 4) >> 2, (dp & 8) >> 3);
}

void SSD_Close()
{
    // stop the timer
    T1CONbits.ON = 0;// turn off Timer1
    // turn off digits
    lat_SSD_AN1 = 1; // deactivate digit 1;
    lat_SSD_AN2 = 1; // deactivate digit 2;    
    lat_SSD_AN3 = 1; // deactivate digit 3;   
    lat_SSD_AN0 = 1; // deactivate digit 0;
}

/* *****************************************************************************
 End of File
 */
