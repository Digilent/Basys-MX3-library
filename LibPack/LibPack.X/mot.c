/* ************************************************************************** */
/** Descriptive File Name

  @Company
    Digilent

  @File Name
    mot.c

  @Description
        This file groups the functions that implement the MOT library.
        There are two modes: PH/EN and IN/IN. After common initialization functions, 
        separate functions are provided for each mode. The PH/EN mmode uses OC2 and OC3 in order to generate PWM.
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
#include "mot.h"
#include "led.h"


#define ROR( val, steps, noBits ) ( ( val >> steps ) | ( val << (noBits - steps ) ) ) 

unsigned char bMode = -1, bStepCfg = 0, bStepCfgNoBits = 1;

/***	MOT_Init
**
**	Parameters:
**		unsigned char bMode1    - mode:
**                                  0 - PH/EN mode
**                                  1 - IN/IN mode
**
**	Return Value:
**		
**
**	Description:
**		This function initializes the hardware involved in the MOT module, in a 
**      specific mode (PH/EN or IN/IN).
**      The motor command pins are initialized as digital outputs.
**      For PH/EN mode, the AIN2 and BIN2 are mapped over OC2 and OC3, OC2 and OC3 are properly initialized.
**      For IN/IN mode, the stepper is initialized to work with 0b1100 rotated one value right over 4 bits, .
**          
*/
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
        MOT_InInInitStep (0x0C, 4); // 0b1100 rotated one value right over 4 bits
    }
}

/* ------------------------------------------------------------ */
/***	MOT_ConfigurePins
**
**	Parameters:
**		
**
**	Return Value:
**		
**
**	Description:
**		This function configures the IO pins involved in the LED module as digital output pins. 
**      
**          
*/
void MOT_ConfigurePins()
{

    // Configure MOTs digital outputs.
    tris_MOT_MODE = 0;
    tris_MOT_AIN1 = 0;
    tris_MOT_AIN2 = 0;
    tris_MOT_BIN1 = 0;
    tris_MOT_BIN2 = 0;

    // disable analog (set pins as digital))
    ansel_MOT_AIN1 = 0;
    ansel_MOT_BIN2 = 0;
}



#define T3PER   2048    // corresponds to approx 20 kHz, FPB 40 MHz
/* ------------------------------------------------------------ */
/***	MOT_ConfigureOCs
**
**	Parameters:
**		
**
**	Return Value:
**		
**
**	Description:
**		This function configures the OC2 and OC3 to work together with Timer3 
**      in order to generate PWM in PH/EN mode, at a frequency of approx 20kHz.
**         
*/
void MOT_ConfigureOCs()
{
   // configure Timer2
    T3CONbits.TCKPS = 0;                //            1:1 prescale value
    T3CONbits.TGATE = 0;                //             not gated input (the default)
    T3CONbits.TCS = 0;                  //             PCBLK input (the default)
    T3CONbits.ON = 1;                   //             turn on Timer2

    PR3 = T3PER;  

    // Configure Output Compare Module 2
   OC2CONbits.ON = 0;       // Turn off OC2 while doing setup.
   OC2CONbits.OCM = 6;      // PWM mode on OC2; Fault pin is disabled
   OC2CONbits.OCTSEL = 1;   // Timer3 is the clock source for this Output Compare module
   OC2CONbits.ON = 1;       // Start the OC2 module
   
    // Configure Output Compare Module 3
   OC3CONbits.ON = 0;       // Turn off OC3 while doing setup.
   OC3CONbits.OCM = 6;      // PWM mode on OC3; Fault pin is disabled
   OC3CONbits.OCTSEL = 1;   // Timer3 is the clock source for this Output Compare module
   OC3CONbits.ON = 1;       // Start the OC3 module
}


/* ------------------------------------------------------------ */
/***	MOT_SetPhEnMotor1
**
**	Parameters:
**		unsigned char bDir      - the rotation direction for DC motor 1
**                                  0 - turn left
**                                  1 - turn right
**		unsigned char bSpeed    - the rotation speed for DC motor 1
**
**	Return Value:
**		
**
**	Description:
**		This function configures the DC motor 1 by providing the rotation direction
**      and the direction speed. The direction speed is provided in values between 0 - 255,
**      which are used to configure the PWM on OC2 accordingly.
**      Naming left / write the direction for rotation speed is a simple convention.
**      The function is supposed to be called in the PH/EN mode, otherwise the function does nothing.
**         
*/
void MOT_SetPhEnMotor1(unsigned char bDir, unsigned char bSpeed)
{
    if(bMode)
    {
        // PH/EN mode
        lat_MOT_AIN1 =(bDir ? 1: 0);
        OC2RS = (unsigned short) (bSpeed << 3);
    }
}

/* ------------------------------------------------------------ */
/***	MOT_SetPhEnMotor2
**
**	Parameters:
**		unsigned char bDir      - the rotation direction for DC motor 2
**                                  0 - turn left
**                                  1 - turn right
**		unsigned char bSpeed    - the rotation speed for DC motor 2
**
**	Return Value:
**		
**
**	Description:
**		This function configures the DC motor 2 by providing the rotation direction
**      and the direction speed. The direction speed is provided in values between 0 - 255,
**      which are used to configure the PWM on OC3 accordingly.
**      Naming left / write the direction for rotation speed is a simple convention.
**      The function is supposed to be called in the PH/EN mode, otherwise the function does nothing.
**         
*/
void MOT_SetPhEnMotor2(unsigned char bDir, unsigned char bSpeed)
{
    if(bMode)
    {
        // PH/EN mode
        lat_MOT_BIN1 = (bDir ? 1: 0);
        OC3RS = (unsigned short) (bSpeed << 3);
    }
}

/* ------------------------------------------------------------ */
/***	MOT_InInInitStep
**
**	Parameters:
**		unsigned char bInitVal  - the byte containing the bits that will be rotated for each step
**		unsigned char noBits    - the number of bits that will be rotated for each step
**
**	Return Value:
**		
**
**	Description:
**		This function configures the Step motor parameters.
**      Performing the steps is done by calling MOT_InInPerformStep.
**      The function is supposed to be called in the IN/IN mode, otherwise the function does nothing.
**         
*/
void MOT_InInInitStep(unsigned char bInitVal, unsigned char noBits)
{
    if(!bMode)
    {
        // IN/IN mode
        bStepCfg = bInitVal;
        bStepCfgNoBits = noBits;
    }
}

/* ------------------------------------------------------------ */
/***	MOT_InInPerformStep
**
**	Parameters:
**		int noSteps  - the byte containing the bits that will be rotated for each step
**		unsigned char noBits    - the number of bits that will be rotated for each step
**
**	Return Value:
**		
**
**	Description:
**		This function configures the Step motor parameters.
**      Performing the steps is done by calling MOT_InInPerformStep.
**      The function is supposed to be called in the IN/IN mode, otherwise the function does nothing.
**         
*/
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
    if(bMode)
    {
        OC2CONbits.ON = 0;       // Stop the OC2 module
        OC3CONbits.ON = 0;       // Stop the OC3 module   
        T2CONbits.ON = 0;        // turn off Timer2
    }
}
/* *****************************************************************************
 End of File
 */
