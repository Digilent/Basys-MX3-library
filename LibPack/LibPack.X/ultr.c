/* ************************************************************************** */
/** Descriptive File Name

  @Company
    University of Delaware

  @File Name
    ultr.c

  @Description
        This file groups the functions that implement the ULTR library.
        The library implements functionality for measuring distance using an
        ultrasonic sensor, compatible with the HC-SR04 sensor.
        NOTE: Measurements returned will be inconsistent when measuring relatively small
        objects at long range (i.e. a 1' wide object at 8' away)

  @Author
    Theo Fleck
 */
/* ************************************************************************** */

/* ************************************************************************** */
/* ************************************************************************** */
/* Section: Included Files                                                    */
/* ************************************************************************** */
#include <xc.h>
#include <sys/attribs.h>
#include "config.h"
#include "ultr.h"
#include "pmods.h"

int echoPinPmod;
int echoPin;
int triggerPinPmod;
int triggerPin;
/* ************************************************************************** */

/* ------------------------------------------------------------ */
/***	ULTR_Init
**
**	Parameters:
**		int ePinPmod - the PMOD port the echo pin is attached to
 *              0 - PMOD A
 *              1 - PMOD B
 *      int ePin - the pin of the PMOD the echo pin is attached to
 *      int tPinPmod - the PMOD port the trigger pin is attached to
 *              0 - PMOD A
 *              1 - PMOD B
 *      int tPin - the pin of the PMOD the trigger pin is attached to
 *      
**
**	Return Value:
**		
**
**	Description:
**		This function initializes the hardware involved in the ULTR module: 
**      The echo and trigger pin positions are recorded
**      The echo pin is initialized as an input, and the trigger pin as an output 
**      Timer 4 is configured with a 1:16 prescale value
**          
*/
void ULTR_Init(int ePinPmod, int ePin, int tPinPmod, int tPin)
{
    echoPinPmod = ePinPmod;
    echoPin = ePin;
    triggerPinPmod = tPinPmod;
    triggerPin = tPin;
    PMODS_InitPin(echoPinPmod,echoPin,1,1,0);
    PMODS_InitPin(triggerPinPmod,triggerPin,0,0,0);
    T4CONbits.TCKPS = 4;     //1:16 pre scale value
    T4CONbits.TGATE = 0;     //not gated input (the default)
    T4CONbits.TCS = 0;       //PCBLK input (the default)
    T4CONbits.ON = 1;        //turn on Timer4
}

/* ------------------------------------------------------------ */
/***	ULTR_MeasureDist
**
**	Parameters:
**		
**
**	Return Value:
**		int - length of echo return pulse, returns -1 if pulse is out of range
**
**	Description:
**		This function measures the distance to the nearest object using
**      the attached ultrasonic sensor.
**      The trigger pin is held high for ~10 microseconds
**      The pulse width is measured on the echo pin
**          
*/
int ULTR_MeasureDist(){
    int counter;
    TMR4 = 0;
    PMODS_SetValue(triggerPinPmod,triggerPin,0);
    for (counter = 0; counter < 3; counter++){}
    PMODS_SetValue(triggerPinPmod,triggerPin,1);
    for (counter = 0; counter < 15; counter++){}
    PMODS_SetValue(triggerPinPmod,triggerPin,0);
    while(PMODS_GetValue(echoPinPmod,echoPin) == 0){
        if(TMR4 >= 65500){ // sensor probably unplugged
            return -1;
        }
    }
    TMR4 = 0;
    unsigned int t1 = TMR4;
    while(PMODS_GetValue(echoPinPmod,echoPin) == 1){
        if((TMR4-t1) >= 58500){ //Timer has exceeded max range of sensor
            return -1;
        }
    }
    unsigned int t2 = TMR4;
    int pulse = ((t2-t1)/(PB_FRQ/1E6))*16;
    if(pulse > 23400){ //23323.62 us is max range of sensor
        return -1;
    }
    return pulse;
}

/* ------------------------------------------------------------ */
/***	ULTR_Close
**
**	Parameters:
** 
**
**	Return Value:
**      
**
**	Description:
**		This functions releases the hardware involved in the ULTR library: 
**      it turns off the Timer 4 module.
**      
**          
*/
void SRV_Close()
{ 
    T4CONbits.ON = 0;        // turn off Timer2
}
/* *****************************************************************************
 End of File
 */