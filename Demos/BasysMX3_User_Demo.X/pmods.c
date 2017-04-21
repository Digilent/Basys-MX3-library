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
#include "pmods.h"

/*
 *  bPmod = 
 *          1 - JA
 *          2 - JB
 *  bPos =
 *          1  - JA1, JB1
 *          2  - JA2, JB2
 *          3  - JA3, JB3
 *          4  - JA4, JB4
 *          7  - JA7, JB7
 *          8  - JA8, JB8
 *          9  - JA9, JB9
 *         10  - JA10, JB10
 *  bDir = 
 *          0  - Output
 *          1  - Input
 *  */

void PMODS_InitPin(unsigned char bPmod, unsigned char bPos, unsigned char bDir, unsigned char pullup, unsigned char pulldown)
{
    switch (bPmod)
    {
        case 1: 
            // JA
        {
            switch (bPos)
            {
                case 1: 
                // JA1
                    rp_PMODS_JA1 = 0;   // default pin function (no remapable)
                    tris_PMODS_JA1 = bDir;
                    cnpu_PMODS_JA1 = pullup;        
                    cnpd_PMODS_JA1 = pulldown;                            
                    break;
                break;
                case 2: 
                // JA2
                    rp_PMODS_JA2 = 0;   // default pin function (no remapable)
                    tris_PMODS_JA2 = bDir;
                    cnpu_PMODS_JA2 = pullup;        
                    cnpd_PMODS_JA2 = pulldown;        
                break;
                case 3: 
                // JA3
                    rp_PMODS_JA3 = 0;   // default pin function (no remapable)
                    tris_PMODS_JA3 = bDir;
                    cnpu_PMODS_JA3 = pullup;        
                    cnpd_PMODS_JA3 = pulldown;        
                break;
                case 4: 
                // JA4
                    ansel_PMODS_JA4 = 0;    // set pis as digital
                    rp_PMODS_JA4 = 0;   // default pin function (no remapable)
                    tris_PMODS_JA4 = bDir;
                    cnpu_PMODS_JA4 = pullup;        
                    cnpd_PMODS_JA4 = pulldown;        
                break;
                case 7: 
                // JA7
                    rp_PMODS_JA7 = 0;   // default pin function (no remapable)
                    tris_PMODS_JA7 = bDir;
                    cnpu_PMODS_JA7 = pullup;        
                    cnpd_PMODS_JA7 = pulldown;        
                break;
                case 8:
                // JA8
                    ansel_PMODS_JA8 = 0;    // set pis as digital
                    rp_PMODS_JA8 = 0;   // default pin function (no remapable)
                    tris_PMODS_JA8 = bDir;
                    cnpu_PMODS_JA8 = pullup;        
                    cnpd_PMODS_JA8 = pulldown;        
                break;
                case 9: 
                // JA9
                    ansel_PMODS_JA9 = 0;    // set pis as digital
                    rp_PMODS_JA9 = 0;   // default pin function (no remapable)
                    tris_PMODS_JA9 = bDir;
                    cnpu_PMODS_JA9 = pullup;        
                    cnpd_PMODS_JA9 = pulldown;        
                break;
                case 10: 
                // JA10
                    ansel_PMODS_JA10 = 0;    // set pis as digital
                    rp_PMODS_JA10 = 0;   // default pin function (no remapable)
                    tris_PMODS_JA10 = bDir;
                    cnpu_PMODS_JA10 = pullup;        
                    cnpd_PMODS_JA10 = pulldown;        
                break;            
            }
        }
        break;        
        case 2: 
            // JB
        {
            switch (bPos)
            {
                case 1: 
                // JB1
                    rp_PMODS_JB1 = 0;   // default pin function (no remapable)
                    tris_PMODS_JB1 = bDir;
                    cnpu_PMODS_JB1 = pullup;        
                    cnpd_PMODS_JB1 = pulldown;        
                break;
                case 2: 
                // JB2
                    rp_PMODS_JB2 = 0;   // default pin function (no remapable)
                    tris_PMODS_JB2 = bDir;
                    cnpu_PMODS_JB2 = pullup;        
                    cnpd_PMODS_JB2 = pulldown;        
                break;
                case 3: 
                // JB3
                    rp_PMODS_JB3 = 0;   // default pin function (no remapable)
                    tris_PMODS_JB3 = bDir;
                    cnpu_PMODS_JB3 = pullup;        
                    cnpd_PMODS_JB3 = pulldown;        
                break;
                case 4: 
                // JB4
                    ansel_PMODS_JA4 = 0;    // set pis as digital
                    rp_PMODS_JB4 = 0;   // default pin function (no remapable)
                    tris_PMODS_JB4 = bDir;
                    cnpu_PMODS_JB4 = pullup;        
                    cnpd_PMODS_JB4 = pulldown;        
                break;
                case 7: 
                // JB7
                    OSCCONbits.SOSCEN = 0;
                    rp_PMODS_JB7 = 0;   // default pin function (no remapable)
                    tris_PMODS_JB7 = bDir;
                    cnpu_PMODS_JB7 = pullup;        
                    cnpd_PMODS_JB7 = pulldown;        
                break;
                case 8: 
                // JB8
                    rp_PMODS_JB8 = 0;   // default pin function (no remapable)
                    tris_PMODS_JB8 = bDir;
                    cnpu_PMODS_JB8 = pullup;        
                    cnpd_PMODS_JB8 = pulldown;        
                break;
                case 9: 
                // JB9
                    ansel_PMODS_JB9 = 0;    // set pis as digital
                    rp_PMODS_JB9 = 0;   // default pin function (no remapable)
                    tris_PMODS_JB9 = bDir;
                    cnpu_PMODS_JB9 = pullup;        
                    cnpd_PMODS_JB9 = pulldown;        
                break;
                case 10: 
                // JB10
                    OSCCONbits.SOSCEN = 0;
                    rp_PMODS_JB10 = 0;   // default pin function (no remapable)
                    tris_PMODS_JB10 = bDir;
                    cnpu_PMODS_JB10 = pullup;        
                    cnpd_PMODS_JB10 = pulldown;        
                break;            
            }
        }
    }
}

/*
 *  bPmod = 
 *          1 - JA
 *          2 - JB
 *  bPos =
 *          1  - JA1, JB1
 *          2  - JA2, JB2
 *          3  - JA3, JB3
 *          4  - JA4, JB4
 *          7  - JA7, JB7
 *          8  - JA8, JB8
 *          9  - JA9, JB9
 *         10  - JA10, JB10
 */
unsigned char PMODS_GetValue(unsigned char bPmod, unsigned char bPos)
{
    unsigned bResult = 0;
    
    switch (bPmod)
    {
        case 0: 
            // JA
        {
            switch (bPos)
            {
                case 1: 
                // JA1
                    bResult = prt_PMODS_JA1;
                break;
                case 2: 
                // JA2
                    bResult = prt_PMODS_JA2;
                break;
                case 3: 
                // JA3
                    bResult = prt_PMODS_JA3;
                break;
                case 4: 
                // JA4
                    bResult = prt_PMODS_JA4;
                break;
                case 7: 
                // JA7
                    bResult = prt_PMODS_JA7;
                break;
                case 8: 
                // JA8
                    bResult = prt_PMODS_JA8;
                break;
                case 9: 
                // JA9
                    bResult = prt_PMODS_JA9;
                break;
                case 10: 
                // JA10
                    bResult = prt_PMODS_JA10;
                break;            
            }
        }
        break;
        case 1: 
            // JB
        {
            switch (bPos)
            {
                case 1: 
                // JB1
                    bResult = prt_PMODS_JB1;
                break;
                case 2: 
                // JB2
                    bResult = prt_PMODS_JB2;
                break;
                case 3: 
                // JB3
                    bResult = prt_PMODS_JB3;
                break;
                case 4: 
                // JB4
                    bResult = prt_PMODS_JB4;
                break;
                case 7: 
                // JB7
                    bResult = prt_PMODS_JB7;
                break;
                case 8: 
                // JB8
                    bResult = prt_PMODS_JB8;
                break;
                case 9: 
                // JB9
                    bResult = prt_PMODS_JB9;
                break;
                case 10: 
                // JB10
                    bResult = prt_PMODS_JB10;
                break;            
            }
        }
    }
    return bResult;
}

        
/*
 *  bPmod = 
 *          1 - JA
 *          2 - JB
 *  bPos =
 *          1  - JA1, JB1
 *          2  - JA2, JB2
 *          3  - JA3, JB3
 *          4  - JA4, JB4
 *          7  - JA7, JB7
 *          8  - JA8, JB8
 *          9  - JA9, JB9
 *         10  - JA10, JB10
 *  bVal = 
 *          0  - 0 value
 *          != 0  - 1 value
 *  */
void PMODS_SetValue(unsigned char bPmod, unsigned char bPos, unsigned char bVal)
{
    unsigned char bVald = bVal ? 1: 0;
    switch (bPmod)
    {
        case 0: 
            // JA
        {
            switch (bPos)
            {
                case 1: 
                // JA1
                    lat_PMODS_JA1 = bVald;
                break;
                case 2: 
                // JA2
                    lat_PMODS_JA2 = bVald;
                break;
                case 3: 
                // JA3
                    lat_PMODS_JA3 = bVald;
                break;
                case 4: 
                // JA4
                    lat_PMODS_JA4 = bVald;
                break;
                case 7: 
                // JA7
                    lat_PMODS_JA7 = bVald;
                break;
                case 8: 
                // JA8
                    lat_PMODS_JA8 = bVald;
                break;
                case 9: 
                // JA9
                    lat_PMODS_JA9 = bVald;
                break;
                case 10: 
                // JA10
                    lat_PMODS_JA10 = bVald;
                break;            
            }
        }
        break;
        case 1: 
            // JB
        {
            switch (bPos)
            {
                case 1: 
                // JB1
                    lat_PMODS_JB1 = bVald;
                break;
                case 2: 
                // JB2
                    lat_PMODS_JB2 = bVald;
                break;
                case 3: 
                // JB3
                    lat_PMODS_JB3 = bVald;
                break;
                case 4: 
                // JB4
                    lat_PMODS_JB4 = bVald;
                break;
                case 7: 
                // JB7
                    lat_PMODS_JB7 = bVald;
                break;
                case 8: 
                // JB8
                    lat_PMODS_JB8 = bVald;
                break;
                case 9: 
                // JB9
                    lat_PMODS_JB9 = bVald;
                break;
                case 10: 
                // JB10
                    lat_PMODS_JB10 = bVald;
                break;            
            }
        }
    }
}



/* *****************************************************************************
 End of File
 */
