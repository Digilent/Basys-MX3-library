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
#include "ssd.h"

/* ************************************************************************** */

//    0b1111001, // 1
//    0b0100100, // 2

unsigned char digitSegments[]= {
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

unsigned char fl_SSD = 0;
void __ISR(_TIMER_1_VECTOR, ipl7) Timer1ISR(void) 
{  
    static unsigned char idxCurrDigit = 0;
    unsigned char currDigit, idx;
//     LATDINV = (1<<9);   //debug - toggle RD9 (JB1)  
//    LATDINV = (1<<11);   //debug 
//    fl_SSD = 1;
//    SSD_DriveDigits();
    // begin content of SSD_DriveDigits()
    INTDisableInterrupts();             // INT step 2: disable interrupts at CPU
    idx = (idxCurrDigit++) & 3;
    currDigit = digits[idx];
//currDigit = 0;//tmp    
//     1. deactivate all digits (anods)
//    lat_SSD_AN01 = (prt_SSD_AN01 & ~msk_SSD_AN01) | msk_SSD_AN01;
//    lat_SSD_AN23 = (prt_SSD_AN23 & ~msk_SSD_AN23) | msk_SSD_AN23;

    lat_SSD_AN1 = 1; // deactivate digit 1;
    lat_SSD_AN2 = 1; // deactivate digit 2;    
    lat_SSD_AN3 = 1; // deactivate digit 3;   
    lat_SSD_AN0 = 1; // deactivate digit 0;
/*
    switch(idx)
    {
        case 0:
            lat_SSD_AN3 = 1; // deactivate digit 3;   
            break;
        case 1:
            lat_SSD_AN0 = 1; // deactivate digit 0;
            break;
        case 2:
           lat_SSD_AN1 = 1; // deactivate digit 1;
            break;
        case 3:
            lat_SSD_AN2 = 1; // deactivate digit 2;
    }
*/
    
    // 2. drive all the segments (cathods))
    lat_SSD_CA = currDigit & 0x01;
    lat_SSD_CB = (currDigit & 0x02) >> 1;
    lat_SSD_CC = (currDigit & 0x04) >> 2;
    lat_SSD_CD = (currDigit & 0x08) >> 3;    
    lat_SSD_CE = (currDigit & 0x10) >> 4;
    lat_SSD_CF = (currDigit & 0x20) >> 5;
    lat_SSD_CG = (currDigit & 0x40) >> 6;
    lat_SSD_DP = (currDigit & 0x80) >> 7;      
    // 3. activate the right digit (anods)
//    unsigned int anNew;
//    anNew = ~(1<<idx);    
//    lat_SSD_AN01 = (prt_SSD_AN01 & ~msk_SSD_AN01) | ((anNew & 3) << 12); // the 2 lsb bits of anNew
//    lat_SSD_AN23 = (prt_SSD_AN23 & ~msk_SSD_AN23) | ((anNew & 0x0C) << 7); // the 2 msb bits of anNew

    
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
  INTEnableSystemMultiVectoredInt();  // INT step 7: enable interrupts at CPU
  // end content of SSD_DriveDigits()
    IFS0bits.T1IF = 0;                  // clear interrupt flag
}
/*
 * 
 */
void SSD_CheckFlag()
{
    if(fl_SSD)
    {
        SSD_DriveDigits();
        fl_SSD = 0;
    }
}

#define DEL_DISP    100
void SSD_DriveDigits()
{
    static unsigned char idxCurrDigit = 0;
    unsigned char currDigit, idx;
    unsigned int anNew;
INTDisableInterrupts();             // INT step 2: disable interrupts at CPU
    idx = (idxCurrDigit++) & 3;
    currDigit = digits[idx];

//     1. deactivate all digits      
//    lat_SSD_AN01 = (lat_SSD_AN01 & ~msk_SSD_AN01) | msk_SSD_AN01;
//    lat_SSD_AN23 = (lat_SSD_AN23 & ~msk_SSD_AN23) | msk_SSD_AN23;

    lat_SSD_AN1 = 1; // deactivate digit 1;
    lat_SSD_AN2 = 1; // deactivate digit 2;    
    lat_SSD_AN3 = 1; // deactivate digit 3;   
    lat_SSD_AN0 = 1; // deactivate digit 0;

/*
    switch(idx)
    {
        case 0:
            lat_SSD_AN3 = 1; // deactivate digit 3;   
            break;
        case 1:
            lat_SSD_AN0 = 1; // deactivate digit 0;
            break;
        case 2:
           lat_SSD_AN1 = 1; // deactivate digit 1;
            break;
        case 3:
            lat_SSD_AN2 = 1; // deactivate digit 2;
    }
*/
  


    // deactivate all digits
    // 2. drive all the segments
        lat_SSD_CA = currDigit & 0x01;
        lat_SSD_CB = (currDigit & 0x02) >> 1;
        lat_SSD_CC = (currDigit & 0x04) >> 2;
        lat_SSD_CD = (currDigit & 0x08) >> 3;    
        lat_SSD_CE = (currDigit & 0x10) >> 4;
        lat_SSD_CF = (currDigit & 0x20) >> 5;
        lat_SSD_CG = (currDigit & 0x40) >> 6;

    // 3. activate the right digit
//    anNew = ~(1<<idx);    
//    lat_SSD_AN01 = (prt_SSD_AN01 & ~msk_SSD_AN01) | ((anNew & 3) << 12); // the 2 lsb bits of anNew
//    lat_SSD_AN23 = (prt_SSD_AN23 & ~msk_SSD_AN23) | ((anNew & 0x0C) << 7); // the 2 msb bits of anNew
        
 
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
  INTEnableSystemMultiVectoredInt();  // INT step 7: enable interrupts at CPU
}

void Timer1Setup()
{
  INTDisableInterrupts();             // INT step 2: disable interrupts at CPU
                                      // INT step 3: setup peripheral
  //PR1 = 600;                        //             set period register, generates one interrupt every ~1000 us
//  PR1 = 3000;                        //             set period register, generates one interrupt every ~1000 us
//  PR1 = 6000;                        //             set period register, generates one interrupt every ~1000 us
  PR1 = 3000;                        //             set period register, generates one interrupt every ~1000 us
  TMR1 = 0;                           //             initialize count to 0
  T1CONbits.TCKPS = 2;                //            1:64 prescale value
  T1CONbits.TGATE = 0;                //             not gated input (the default)
  T1CONbits.TCS = 0;                  //             PCBLK input (the default)
  T1CONbits.ON = 1;                   //             turn on Timer1
  IPC1bits.T1IP = 7;                  // INT step 4: priority
  IPC1bits.T1IS = 3;                  //             subpriority
  IFS0bits.T1IF = 0;                  // INT step 5: clear interrupt flag
  IEC0bits.T1IE = 1;                  // INT step 6: enable interrupt
  INTEnableSystemMultiVectoredInt();  // INT step 7: enable interrupts at CPU
}
/* ************************************************************************** */
/* ************************************************************************** */
// Section: Local Functions                                                   */
/* ************************************************************************** */
/* ************************************************************************** */

/*  A brief description of a section can be given directly below the section
    banner.
 */

/* ************************************************************************** */

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
void SSD_Init()
{
    SSD_ConfigurePins();
    Timer1Setup();  
}

unsigned char SSD_GetDigitSegments(unsigned char d)
{
    unsigned char bResult = -1;
    if(d < NO_DIGITS)
    {
        bResult = digitSegments[d];
    }
    return bResult;
}


void SSD_WriteDigits(unsigned char d1, unsigned char d2, unsigned char d3, unsigned char d4, \
        unsigned char dp1, unsigned char dp2, unsigned char dp3, unsigned char dp4)
{
      T1CONbits.ON = 0;                   //             turn off Timer1
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
  T1CONbits.ON = 1;                   //             turn on Timer1
}

/*
 v1 (16 bits) contains the 4 digits in hexa
 dp (8 bits) contains in the 4 LSB bits the 4 values for the decimal points
 */
void SSD_WriteDigitsGrouped(unsigned int val, unsigned char dp)
{
    SSD_WriteDigits(val & 0xF, (val & 0xF0) >> 4, (val & 0xF00) >> 8, (val & 0xF000) >> 12,
            dp&1, (dp & 2) >> 1, (dp & 4) >> 2, (dp & 8) >> 3);
}


/* *****************************************************************************
 End of File
 */
