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
**      Timer 4 is configured with a 1:256 prescale value
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
    T4CON = 0x8060; //1000000001110000    
}

/* ------------------------------------------------------------ */
/***	ULTR_MeasureCm
**
**	Parameters:
**		
**
**	Return Value:
**		float - the distance from the nearest object in centimeters
 *              returns -1 if out of range (>400 cm)
**
**	Description:
**		This function measures the distance to the nearest object using
**      the attached ultrasonic sensor.
**      The trigger pin is held high for 10 microseconds
**      The pulse width is measured on the echo pin, and the distance is calculated
**          
*/
float ULTR_MeasureCm(){
    int counter;
    TMR4 = 0;
    PMODS_SetValue(triggerPinPmod,triggerPin,0);
    for (counter = 0; counter < 3; counter++){}
    PMODS_SetValue(triggerPinPmod,triggerPin,1);
    for (counter = 0; counter < 15; counter++){}
    PMODS_SetValue(triggerPinPmod,triggerPin,0);
    while(PMODS_GetValue(echoPinPmod,echoPin) == 0);
    unsigned int t1 = TMR4;
    while(PMODS_GetValue(echoPinPmod,echoPin) == 1);
    unsigned int t2 = TMR4;
    float cm = ((t2-t1)/(PB_FRQ/1E6))/58.0*64;
    if(cm > 400){
        return -1;
    }
    else{
        return cm;
    }
}

/* ------------------------------------------------------------ */
/***	ULTR_MeasureIn
**
**	Parameters:
**		
**
**	Return Value:
**		float - the distance from the nearest object in inches
 *              returns -1 if out of range (>157 in)
**
**	Description:
**		This function measures the distance to the nearest object using
**      the attached ultrasonic sensor.
**      The trigger pin is held high for 10 microseconds
**      The pulse width is measured on the echo pin, and the distance is calculated
**          
*/
float ULTR_MeasureIn(){
    int counter;
    TMR4 = 0;
    PMODS_SetValue(triggerPinPmod,triggerPin,0);
    for (counter = 0; counter < 3; counter++);
    PMODS_SetValue(triggerPinPmod,triggerPin,1);
    for (counter = 0; counter < 15; counter++);
    PMODS_SetValue(triggerPinPmod,triggerPin,0);
    while(PMODS_GetValue(echoPinPmod,echoPin) == 0);
    unsigned int t1 = TMR4;
    while(PMODS_GetValue(echoPinPmod,echoPin) == 1);
    unsigned int t2 = TMR4;
    float in = ((t2-t1)/(PB_FRQ/1E6))/148.0*64;
    if(in > 157){
        return -1;
    }
    else{
        return in;
    } 
}

/* *****************************************************************************
 End of File
 */
