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
#include "swt.h"

/* ************************************************************************** */

void SWT_Init()
{
    SWT_ConfigurePins();
}

void SWT_ConfigurePins()
{
    // Configure SWTs as digital inputs.
    tris_SWT_SWT0 = 1;
    tris_SWT_SWT1 = 1;
    tris_SWT_SWT2 = 1;
    tris_SWT_SWT3 = 1;
    tris_SWT_SWT4 = 1;
    tris_SWT_SWT5 = 1;
    tris_SWT_SWT6 = 1;
    tris_SWT_SWT7 = 1;

    // disable analog (set pins as digital))
    ansel_SWT_SWT5 = 0;
    ansel_SWT_SWT6 = 0;
    ansel_SWT_SWT7 = 0;
    
}


unsigned char SWT_GetValue(unsigned char bNo)
{
    unsigned bResult = 0;
    
    switch (bNo)
    {
        case 0: 
            bResult = prt_SWT_SWT0;
            break;
        case 1: 
            bResult = prt_SWT_SWT1;
            break;
        case 2: 
            bResult = prt_SWT_SWT2;
            break;
        case 3: 
            bResult = prt_SWT_SWT3;
            break;
        case 4: 
            bResult = prt_SWT_SWT4;
            break;
        case 5: 
            bResult = prt_SWT_SWT5;
            break;
        case 6: 
            bResult = prt_SWT_SWT6;
            break;
        case 7: 
            bResult = prt_SWT_SWT7;
            break;
    }

    return bResult;
}

unsigned char SWT_GetGroupValue()
{
    int i;
    unsigned char bResult = 0;
    for(i = 0; i < 8; i++)
    {
        if(SWT_GetValue(i))
        {
            bResult |= (1 << i);
        }
    }
    return bResult;
}



/* *****************************************************************************
 End of File
 */
