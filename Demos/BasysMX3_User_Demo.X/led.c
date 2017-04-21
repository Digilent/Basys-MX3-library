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
#include "led.h"

/* ************************************************************************** */

void LED_Init()
{
    LED_ConfigurePins();
}

void LED_ConfigurePins()
{
    // Configure LEDs as digital outputs.
    tris_LEDS_GRP &= ~msk_LEDS_GRP;
}

void LED_SetValue(unsigned char bNo, unsigned char bVal)
{
    switch (bNo)
    {
        case 0:
            LEDS_Led0SetValue(bVal);
            break;
        case 1:
            LEDS_Led1SetValue(bVal);
            break;
        case 2:
            LEDS_Led2SetValue(bVal);
            break;
        case 3:
            LEDS_Led3SetValue(bVal);
            break;
        case 4:
            LEDS_Led4SetValue(bVal);
            break;
        case 5:
            LEDS_Led5SetValue(bVal);
            break;
        case 6:
            LEDS_Led6SetValue(bVal);
            break;
        case 7:
            LEDS_Led7SetValue(bVal);
            break;
    }
}

unsigned char LED_GetValue(unsigned char bNo)
{
    return (prt_LEDS_GRP & (1<<bNo)) ? 1: 0;
}

void LED_ToggleValue(unsigned char bNo)
{
    latinv_LEDS_GRP = (1<<bNo);
}
void LED_SetGroupValue(unsigned char bVal)
{
//    lat_LEDS_GRP = (LATA & ~msk_LEDS_GRP) | (bVal & msk_LEDS_GRP);
    
//    int i;
//    for(i = 0; i++; i<8)
//    {
//        LED_SetValue(i, (bVal & (1<<i))? 1: 0);
//    }
    unsigned char *pLedData = (unsigned char *)lat_LEDS_GRP_ADDR;
    *pLedData = bVal;
//    unsigned int *pLedData = (unsigned int *)(0xBF886030);
//    *pLedData = 0xFFFFFF00;
    
}

unsigned char LED_GetGroupValue()
{
    return prt_LEDS_GRP & msk_LEDS_GRP;
}


/* *****************************************************************************
 End of File
 */
