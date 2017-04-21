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
#include "mot.h"



#define ROR( val, steps, noBits ) ( ( val >> steps ) | ( val << (noBits - steps ) ) ) 

unsigned char bMode = -1, bStepCfg = 0, bStepCfgShiftPos = 1, bStepCfgNoBits = 1;

#define SYSCLK  64000000L
#define PWM_DIV    14000
#define DUTY_CYCLE  10

#define T2PER   0xFF00


void MOT_ConfigureOCs()
{
   // configure Timer2
    T2CONbits.TCKPS = 3;                //            1:64 prescale value
    T2CONbits.TGATE = 0;                //             not gated input (the default)
    T2CONbits.TCS = 0;                  //             PCBLK input (the default)
    T2CONbits.ON = 1;                   //             turn on Timer2

//    PR2 = (SYSCLK / PWM_DIV) - 1; // aprox 100 Hz
    PR2 = T2PER;

//    OC5RS = (PR2 + 1) * ((float)DUTY_CYCLE / 100);

    // Configure Output Compare Module 2
   OC2CONbits.ON = 0;       // Turn off OC2 while doing setup.
   OC2CONbits.OCM = 6;      // PWM mode on OC2; Fault pin is disabled
   OC2CONbits.OCTSEL = 0;   // Timer2 is the clock source for this Output Compare module
   OC2CONbits.ON = 1;       // Start the OC2 module
   
    // Configure Output Compare Module 3
   OC3CONbits.ON = 0;       // Turn off OC3 while doing setup.
   OC3CONbits.OCM = 6;      // PWM mode on OC3; Fault pin is disabled
   OC3CONbits.OCTSEL = 0;   // Timer2 is the clock source for this Output Compare module
   OC3CONbits.ON = 1;       // Start the OC3 module
    
}


void MOT_Init(unsigned char bMode1)
{
    bMode = bMode1;
    MOT_ConfigurePins();
    lat_MOT_MODE = bMode ? 1: 0;
    
    if(bMode)
    {
        // PH/EN mode
        rp_MOT_AIN2 = 0x0B; // AIN2 is set on OC2
        rp_MOT_BIN2 = 0x0B; // BIN2 is set on OC3
        MOT_ConfigureOCs();

    }
    else
    {
        // IN/IN mode
        rp_MOT_AIN2 = 0; // no connect
        rp_MOT_BIN2 = 0; // no connect

        // Step motor  IN In mode
        MOT_InInInitStep (0x0C, 1, 4); // 0b1100 rotated one value right over 4 bits
    }
}

void MOT_ConfigurePins()
{

    // Configure MOTs digital outputs.
    tris_MOT_MODE = 0;
    tris_MOT_AIN1 = 0;
    tris_MOT_AIN2 = 0;
    tris_MOT_BIN1 = 0;
    tris_MOT_BIN2 = 0;

    RPF1R = 0;
    // disable analog (set pins as digital))
    ansel_MOT_AIN1 = 0;
    ansel_MOT_BIN2 = 0;
}

void MOT_SetPhEnMotor1(unsigned char bDir, unsigned char bSpeed)
{
    if(bMode)
    {
        // PH/EN mode
        lat_MOT_AIN1 =(bDir ? 1: 0);
        OC2RS = (unsigned short) (bSpeed << 8);
    }
}

void MOT_SetPhEnMotor2(unsigned char bDir, unsigned char bSpeed)
{
    if(bMode)
    {
        // PH/EN mode
        lat_MOT_BIN1 = (bDir ? 1: 0);
        OC3RS = (unsigned short) (bSpeed << 8);
    }
}

void MOT_InInInitStep(unsigned char bInitVal, unsigned char bShiftPos, unsigned char noBits)
{
    if(!bMode)
    {
        // IN/IN mode
        bStepCfg = bInitVal;
        bStepCfgShiftPos = bShiftPos;
        bStepCfgNoBits = noBits;
    }
}

void MOT_InInPerformStep(int noSteps, unsigned char bDir)
{
    int i;
    if(!bMode)
    {
        // IN/IN mode
        for(i = 0; i< noSteps; i++)
        {
            // apply the value from bStepCfg to A1, B1, A2, B2
            lat_MOT_AIN1 = (bStepCfg & 8) ? 1: 0;
            lat_MOT_BIN1 = (bStepCfg & 4) ? 1: 0;
            lat_MOT_AIN2 = (bStepCfg & 2) ? 1: 0;
            lat_MOT_BIN2 = (bStepCfg & 1) ? 1: 0;
            // keep values for some time
    //        DelayAprox10Us(2000);
     //       DelayAprox10Us(10);

    //i = i+1; // tmp
            // shift right the cfg value
    //        bStepCfg = ROR( bStepCfg, bStepCfgShiftPos, bStepCfgNoBits);
    //( val >> steps ) | ( val << (noBits - steps ) )
            if(bDir)
            {
                // rotate right
                bStepCfg = (( bStepCfg >> 1 ) | ( bStepCfg << (4 - 1 ) )) & ((1<<4) - 1);
            }
            else
            {
                // rotate left
                bStepCfg = (( bStepCfg << 1 ) | ( bStepCfg >> (4 - 1 ) )) & ((1<<4) - 1);
            }
        }
    }
    
}

void MOT_Close()
{
    OC2CONbits.ON = 0;       // Stop the OC2 module
    OC3CONbits.ON = 0;       // Stop the OC3 module   
}
/* *****************************************************************************
 End of File
 */
