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
#include "btn.h"


#include "led.h"

/* ************************************************************************** */
unsigned char (*pcallbackFuncBtn)();
//void __ISR(_CHANGE_NOTICE_VECTOR, ipl2) CN_Int_ISR(void)

// -----------------------------------------------------------------------------
//  ISR handlers
// -----------------------------------------------------------------------------

/*
 * Although I couldn't get the ISR called, the status bits are set, so this works:
        if(CNSTATBbits.CNSTATB8 == 1)
        {
            LED_SetValue(3, 1);
        }
        if(CNSTATAbits.CNSTATA15 == 1)
        {
            LED_SetValue(4, 1);
        }
 */
// Implement interrupt handlers

void __ISR(_CHANGE_NOTICE_VECTOR, ipl5SOFT) CN_Int_ISR(void)
{
   unsigned int thisb = PORTB; // Read PORTB to clear mismatch condition
    //LATASET = 1;                     // toggle RA1
   LED_SetValue(0, 1);
    
//    LATGINV = (1<<6);   //debug - toggle RG6 (JA4)
   
    
//    IFS1CLR = 0x0001; // Be sure to clear the CN interrupt status
                    // flag before exiting the service routine.
   IFS1bits.CNBIF = 0;                  // clear interrupt flag for PORTB Input Change Interrupt
}

void BTN_SetCallbackFunction(unsigned char (*pfIntHandler)())
{
    pcallbackFuncBtn = pfIntHandler;
}


 void BTN_InitChangeNotification()
{
    unsigned char bDummmy;
    ANSELB = 0 ;	//set all pins that share analog and make them digital
    PORTB = 0;    
    PORTA = 0;    
    //INTDisableInterrupts();             // INT step 2: disable interrupts at CPU    
    CNENBbits.CNIEB0 = 1; // RB0 (BTNU) produce change notification
    CNENBbits.CNIEB1 = 1; // RB1 (BTNL) produce change notification
    CNENBbits.CNIEB8 = 1; // RB8 (BTNR) produce change notification

    CNPDBbits.CNPDB0 = 1;     //change notice pull=up
    CNPDBbits.CNPDB1 = 1;     //change notice pull=up
    CNPDBbits.CNPDB8 = 1;     //change notice pull=up


    CNCONBbits.ON = 1; // ON

   bDummmy = PORTB; // Read port(s) to clear mismatch on change notice pins
    
    
    
    IEC1bits.CNBIE = 0;   //disable interrupts IEC0<14> = PORTB change p87
    IEC1bits.CNAIE = 0;   //disable interrupts IEC0<14> = PORTB change p87
    IFS1bits.CNBIF = 0;                  // clear interrupt flag for PORTB Input Change Interrupt
    IFS1bits.CNAIF = 0;                  // clear interrupt flag for PORTB Input Change Interrupt

    IEC1bits.CNBIE = 1; // Enable CN on port B interrupts 

     INTEnableSystemMultiVectoredInt(); // avoid single vectror mode

 }
/*
void BTN_InitChangeNotification()
{
// Setup the change notice options
    ANSELB = 0 ;
    PORTB = 0;
  CNCONCbits.ON = 1;        //CN is enabled
//  CNCONCbits.SIDL = 0;    //CPU Idle does not affect CN operation
  CNENBbits.CNIEB1 = 1;    //Enable RB1 change notice
  CNENBbits.CNIEB8 = 1;    //Enable RB8 change notice
  CNENBbits.CNIEB0 = 1;    //Enable RB0 change notice
 
  // Read port C to clear mismatch condition
  unsigned char tmp = PORTB;

    INTEnableSystemMultiVectoredInt();
    
        IEC1bits.CNBIE = 0;
  // Clear CN interrupt flag, set CN interrupt priority, enable CN interrupt
  IFS1bits.CNBIF = 0;           // clear status register for port B
//  IPC8CLR = _IPC8_CNIP_MASK;                //clear priority
//  IPC8SET = (2 << _IPC8_CNIP_POSITION);    //set priority (2)
  IEC1bits.CNBIE = 1; // enable CN interrupts on port C
 
  // Pulldown enable
//  CNPDBbits.CNPDB1 = 0;
  CNPDBbits.CNPDB8 = 0;
//  CNPDBbits.CNPDB0 = 0;
  
  
//  CNPUBbits.CNPUB4 = 0;
//  CNPUBbits.CNPUB5 = 0;
//  CNPUBbits.CNPUB6 = 0;
//  CNPUBbits.CNPUB7 = 0;  
 
  // Enable interrupts

}
*/
void BTN_Init()
{
    BTN_ConfigurePins();
    // not finalized
//    BTN_InitChangeNotification();
}

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


unsigned char BTN_GetValue(unsigned char btn)
{
    unsigned bResult = 0;
    
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
