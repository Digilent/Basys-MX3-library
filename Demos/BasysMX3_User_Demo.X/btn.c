/* ************************************************************************** */
/** Descriptive File Name

  @Company
    Digilent

  @File Name
    btn.c


  @Description
        This file groups the functions that implement the BTN library.
        The functions implement basic digital input functionality needed for the onboard buttons.
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
#include "btn.h"


/* ************************************************************************** */

/* ------------------------------------------------------------ */
/***	BTN_Init
**
**	Parameters:
**		
**
**	Return Value:
**		
**
**	Description:
**		This function initializes the hardware involved in the BTN module. 
**      The pins corresponding to buttons are initialized as digital inputs.
**          
*/
void BTN_Init()
{
    BTN_ConfigurePins();
}

/* ------------------------------------------------------------ */
/***	BTN_ConfigurePins
**
**	Parameters:
**		
**
**	Return Value:
**		
**
**	Description:
**		This function configures the IO pins used in the BTN module as digital input pins. 
**      The function uses pin related definitions from config.h file.
**      This is a low-level function called by BTN_Init(), so user should avoid calling it directly.        
**          
*/
void BTN_ConfigurePins()
{
    // Configure BTNs as digital inputs.
    tris_BTN_BTNU = 1;
    tris_BTN_BTNL = 1;
    tris_BTN_BTNC = 1;
    tris_BTN_BTNR = 1;
    tris_BTN_BTND = 1;

    // disable analog (set pins as digital))
    ansel_BTN_BTNU = 0;
    ansel_BTN_BTNL = 0;
    ansel_BTN_BTNR = 0;
}

/* ------------------------------------------------------------ */
/***	BTN_GetValue
**
**	Parameters:
**		unsigned char btn   - the identification of button whose value will be read.  
**                              The value can be the number or the letter associated to a button:
**                                  0, 'U', 'u': BTNU
**                                  1, 'L', 'l': BTNL
**                                  2, 'C', 'c': BTNC
**                                  3, 'R', 'r': BTNR
**                                  4, 'D', 'd': BTND
**
**	Return Value:
**		unsigned char   - the value corresponding to the specified button:
**                                  0 when corresponding button is not pressed
**                                  1 when corresponding button is pressed
**                      - 0xFF if btn is not between 0 and 4, or one of 'U', 'u', 'L', 'l', 'C', 'c', 'R', 'r', 'D', 'd' 
**
**	Description:
**		This function gets the value of the BTN specified by btn parameter.
**      If the value provided for btn is not between 0 and 4, 
**      or one of 'U', 'u', 'L', 'l', 'C', 'c', 'R', 'r', 'D', 'd', then the function returns 0xFF.
**          
*/
unsigned char BTN_GetValue(unsigned char btn)
{
    unsigned bResult = 0xFF;
    
    switch (btn)
    {
        case 'U': 
        case 'u': 
        case 0:
            bResult = prt_BTN_BTNU;
            break;
        case 'L': 
        case 'l': 
        case 1:
            bResult = prt_BTN_BTNL;
            break;
        case 'C': 
        case 'c': 
        case 2:
            bResult = prt_BTN_BTNC;
            break;
        case 'R': 
        case 'r': 
        case 3:
            bResult = prt_BTN_BTNR;
            break;
        case 'D': 
        case 'd': 
        case 4:
            bResult = prt_BTN_BTND;
            break;
    }

    return bResult;
}

/* ------------------------------------------------------------ */
/***	BTN_GetGroupValue
**
**	Parameters:
**
**	Return Value:
**		unsigned char   - the 8 bit value 0 0 0 B4 B3 B2 B1 B0 where bit Bi corresponds to a button:
**                              B0 has the value 0 when BTNU is not pressed
**                              B0 has the value 1 when BTNU is pressed
**                              B1 has the value 0 when BTNL is not pressed
**                              B1 has the value 1 when BTNL is pressed
**                              B2 has the value 0 when BTNC is not pressed
**                              B2 has the value 1 when BTNC is pressed
**                              B3 has the value 0 when BTNR is not pressed
**                              B3 has the value 1 when BTNR is pressed
**                              B4 has the value 0 when BTND is not pressed
**                              B4 has the value 1 when BTND is pressed
**		
**
**	Description:
**		This function gets the value of the all 5 buttons as a single value represented on 8 bits.  
**      The 5 LSB bits from returned value byte correspond to a button: 
**      bit 0 corresponds to BTNU,
**      bit 1 corresponds to BTNL, 
**      bit 2 corresponds to BTNC, 
**      bit 3 corresponds to BTNR, 
**      bit 4 corresponds to BTND (see return value description).          
*/
unsigned char BTN_GetGroupValue()
{
    unsigned char bResult = 0;
    int i;
    for(i = 0; i < 5; i++)
    {
        bResult |= (BTN_GetValue(i) << i);
    }
    return bResult;
}


/* *****************************************************************************
 End of File
 */
