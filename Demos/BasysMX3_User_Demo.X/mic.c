
/* 
 * File:   mic.c
 * Author: cignat
 *
 * Created on May 27, 2016, 11:43 AM
 */

#include <xc.h>
#include "adc.h"
#include "mic.h"



unsigned int MIC_val()
{
    return (unsigned int)(ADC_analogRead(4));
}




