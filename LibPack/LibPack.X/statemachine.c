/* 
 * File:   cetest.c
 * Author: Cristian Fatu
 *
 * Created on March 10, 2017
 */
#include "config.h"
#include "ssd.h"
#include "lcd.h"
#include "led.h"
#include "rgbled.h"
#include "statemachine.h"
#include "utils.h"
#include "rgbled.h"
#include "audio.h"
#include "spiflash.h"
#include "IrDA.h"
#include "mot.h"
#include "srv.h"
#include "uart.h"
#include <xc.h>  
#include <sys/attribs.h>
#include <stdio.h>


// the state structure definition
typedef struct s_stateMachineElem{
    unsigned int cntStateTimer;
    unsigned int cntStepTimer;
    void (*pfInitTestFnc)();
    void (*pfStepTestFnc)(unsigned int);
    void (*pfExitTestFnc)();
}StateMachinesElem;


// the state function prototypes
void CETest_InitRGBLed();
void CETest_StepRGBLed(unsigned int idxStep);
void CETest_ExitRGBLed();
void CETest_InitLeds();
void CETest_StepLeds(unsigned int idxStep);
void CETest_ExitLeds();
void CETest_InitSSD();
void CETest_StepSSD(unsigned int idxStep);
void CETest_ExitSSD();
void CETest_InitPMODA();
void CETest_StepPMODA(unsigned int idxStep);
void CETest_ExitPMODA();
void CETest_InitPMODB();
void CETest_StepPMODB(unsigned int idxStep);
void CETest_ExitPMODB();
void CETest_InitAUDIO();
void CETest_ExitAUDIO();
void CETest_InitSPIFlash();
void CETest_ExitSPIFlash();
void CETest_InitACL();
void CETest_ExitACL();
void CETest_InitIRDA();
void CETest_ExitIRDA();
void CETest_InitMOT();
void CETest_ExitMOT();
void CETest_InitSRV();
void CETest_ExitSRV();
void CETest_InitUART();
void CETest_ExitUART();

// the state definitions
StateMachinesElem stateMachinesElems[] = 
{
//    {500, 20, &CETest_InitPMODA, &CETest_StepPMODA, &CETest_ExitPMODA},
//    {500, 20, &CETest_InitPMODB, &CETest_StepPMODB, &CETest_ExitPMODB},
//    {500, 20, &CETest_InitLeds, &CETest_StepLeds, &CETest_ExitLeds},
//    {500, 20, &CETest_InitRGBLed, &CETest_StepRGBLed, &CETest_ExitRGBLed},
//    {500, 0, &CETest_InitSRV, 0, &CETest_ExitSRV},
//    {500, 20, &CETest_InitSSD, &CETest_StepSSD, &CETest_ExitSSD},
//    {500, 0, &CETest_InitAUDIO, 0, &CETest_ExitAUDIO},
//    {100, 0, &CETest_InitACL, 0, &CETest_ExitACL},
//   {100, 0, &CETest_InitSPIFlash, 0, &CETest_ExitSPIFlash},
//    {100, 0, &CETest_InitIRDA, 0, &CETest_ExitIRDA},
    {1000, 0, &CETest_InitMOT, 0, &CETest_ExitMOT},
    {100, 0, &CETest_InitUART, 0, &CETest_ExitUART}
};

#define NO_STATES sizeof(stateMachinesElems)/sizeof(stateMachinesElems[0])

char strMsg[20];

/***	CETest_InitRGBLed
**
**	Parameters:
**		
**
**	Return Value:
**		
**
**	Description:
**		This function implements the RGBLED test Init function. 
**      It is aimed to be called by the state machine engine.
**          
*/
void CETest_InitRGBLed()
{
    LCD_WriteStringAtPos("RGBLed test     ", 1, 0);
    RGBLED_Init();
}


/***	CETest_StepRGBLed
**
**	Parameters:
**		
**
**	Return Value:
**		
**
**	Description:
**		This function implements the RGBLED test step function. 
**      It is aimed to be called by the state machine engine.
**      According the the step number, the base colors are generated, in a circular  manner.
**          
*/
void CETest_StepRGBLed(unsigned int idxStep)
{
    unsigned char bShift = idxStep % 3;
    unsigned int uiVal = 0x0000FF << (bShift << 3);
    RGBLED_SetValueGrouped(uiVal);
}

/***	CETest_ExitRGBLed
**
**	Parameters:
**		
**
**	Return Value:
**		
**
**	Description:
**		This function implements the RGBLED test Exit function. 
**      It is aimed to be called by the state machine engine.
**          
*/
void CETest_ExitRGBLed()
{
    RGBLED_Close(); 
}

/***	CETest_InitLeds
**
**	Parameters:
**		
**
**	Return Value:
**		
**
**	Description:
**		This function implements the LED test Init function. 
**      It is aimed to be called by the state machine engine.
**          
*/
void CETest_InitLeds()
{
    LCD_WriteStringAtPos("LED test        ", 1, 0);
    LED_Init();
}


/***	CETest_StepLeds
**
**	Parameters:
**		
**
**	Return Value:
**		
**
**	Description:
**		This function implements the LED test step function. 
**      It is aimed to be called by the state machine engine.
**      According the the step number, one lighting LED is walked left in a circular manner.
**          
*/
void CETest_StepLeds(unsigned int idxStep)
{
    LED_SetGroupValue(1<<(idxStep & 0x7));    
}


/***	CETest_ExitLeds
**
**	Parameters:
**		
**
**	Return Value:
**		
**
**	Description:
**		This function implements the LED test Exit function. 
**      It is aimed to be called by the state machine engine.
**          
*/
void CETest_ExitLeds()
{
    LED_SetGroupValue(0);  
}

/***	CETest_InitSSD
**
**	Parameters:
**		
**
**	Return Value:
**		
**
**	Description:
**		This function implements the SSD test Init function. 
**      It is aimed to be called by the state machine engine.
**          
*/
void CETest_InitSSD()
{
    LCD_WriteStringAtPos("SSD test        ", 1, 0);
    SSD_Init();
}

/***	CETest_StepSSD
**
**	Parameters:
**		
**
**	Return Value:
**		
**
**	Description:
**		This function implements the SSD test step function. 
**      It is aimed to be called by the state machine engine.
**      The step number is displayed on SSD.
**          
*/
void CETest_StepSSD(unsigned int idxStep)
{
    SSD_WriteDigitsGrouped(idxStep, 0);
}

/***	CETest_ExitSSD
**
**	Parameters:
**		
**
**	Return Value:
**		
**
**	Description:
**		This function implements the SSD test Exit function. 
**      It is aimed to be called by the state machine engine.
**          
*/
void CETest_ExitSSD()
{
    SSD_Close();
}

/***	CETest_InitPMODA
**
**	Parameters:
**		
**
**	Return Value:
**		
**
**	Description:
**		This function implements the PMODA test Init function. 
**      It is aimed to be called by the state machine engine.
**      It initializes the pins as output, 0 value.
*/
void CETest_InitPMODA()
{
    int i, j;
    LCD_WriteStringAtPos("PMODA test      ", 1, 0);
    for(i = 1; i <= 8; i++)
    {
        j = (i >= 5) ? (i+2): i;
        PMODS_InitPin(0, j, 0, 0, 0); // output, no pullup, no pulldown
        PMODS_SetValue(0, j, 0); // initialize to 0;
    }
    
}

/***	CETest_StepPMODA
**
**	Parameters:
**		
**
**	Return Value:
**		
**
**	Description:
**		This function implements the PMODA test step function. 
**      It is aimed to be called by the state machine engine.
**      According the the step number, one lighting LED is walked left in a circular manner.
**          
*/
void CETest_StepPMODA(unsigned int idxStep)
{
    PMODS_SetGroupValue(0, 1<<(idxStep & 0x7));
}

/***	CETest_ExitPMODA
**
**	Parameters:
**		
**
**	Return Value:
**		
**
**	Description:
**		This function implements the PMODA test Exit function. 
**      It is aimed to be called by the state machine engine.
*/
void CETest_ExitPMODA()
{
    int i;
    PMODS_SetGroupValue(0, 0);
    for(i = 1; i <= 8; i++)
    {
        PMODS_InitPin(0, i >= 4 ? (i+2): i, 1, 0, 0); // input, no pullup, no pulldown
    }
}

/***	CETest_InitPMODB
**
**	Parameters:
**		
**
**	Return Value:
**		
**
**	Description:
**		This function implements the PMODB test Init function. 
**      It is aimed to be called by the state machine engine.
**      It initializes the pins as output, 0 value.
*/
void CETest_InitPMODB()
{
    int i, j;
    LCD_WriteStringAtPos("PMODB test      ", 1, 0);
    for(i = 1; i <= 8; i++)
    {
        j = (i >= 5) ? (i+2): i;
        PMODS_InitPin(1, j, 0, 0, 0); // output, no pullup, no pulldown
        PMODS_SetValue(1, j, 0); // initialize to 0;
    }
}

/***	CETest_StepPMODB
**
**	Parameters:
**		
**
**	Return Value:
**		
**
**	Description:
**		This function implements the PMODB test step function. 
**      It is aimed to be called by the state machine engine.
**      According the the step number, one lighting LED is walked left in a circular manner.
**          
*/
void CETest_StepPMODB(unsigned int idxStep)
{
    PMODS_SetGroupValue(1, 1<<(idxStep & 0x7));
}

/***	CETest_ExitPMODB
**
**	Parameters:
**		
**
**	Return Value:
**		
**
**	Description:
**		This function implements the PMODB test Exit function. 
**      It is aimed to be called by the state machine engine.
*/
void CETest_ExitPMODB()
{
    int i;
    PMODS_SetGroupValue(1, 0);
    for(i = 1; i <= 8; i++)
    {
        PMODS_InitPin(1, i >= 4 ? (i+2): i, 1, 0, 0); // input, no pullup, no pulldown
    }
}

/***	CETest_InitAUDIO
**
**	Parameters:
**		
**
**	Return Value:
**		
**
**	Description:
**		This function implements the AUDIO test Init function. 
**      It starts the audio to play a sinus generated sound. 
**      It is aimed to be called by the state machine engine.
*/
void CETest_InitAUDIO()
{
    LCD_WriteStringAtPos("AUDIO test        ", 1, 0);
    AUDIO_Init(0);
}


/***	CETest_ExitAUDIO
**
**	Parameters:
**		
**
**	Return Value:
**		
**
**	Description:
**		This function implements the AUDIO test Exit function. 
**      It is aimed to be called by the state machine engine.
*/
void CETest_ExitAUDIO()
{
    AUDIO_Close();
}


/***	CETest_InitSPIFlash
**
**	Parameters:
**		
**
**	Return Value:
**		
**
**	Description:
**		This function implements the SPI Flash test Init function. 
**      It is aimed to be called by the state machine engine.
**      It performs a read from an address incremented each time the function is called.
**      When address is 0, the flash is erased. 
*/
void CETest_InitSPIFlash()
{
    static unsigned int addr;
    unsigned char cw, cr;
    sprintf(strMsg, "SPIFlash test %2.2X", addr);
    LCD_WriteStringAtPos(strMsg, 1, 0);
    
    SPIFLASH_Init();
    if(!addr)
    {
        // erase all flash
        SPIFLASH_EraseAll();
    }

    cw = (unsigned char)addr;
    SPIFLASH_ProgramPage(addr, &cw, 1);
    SPIFLASH_Read(addr++, &cr, 1);    
}


/***	CETest_ExitSPIFlash
**
**	Parameters:
**		
**
**	Return Value:
**		
**
**	Description:
**		This function implements the SPI Flash test Exit function. 
**      It is aimed to be called by the state machine engine.
*/
void CETest_ExitSPIFlash()
{
    SPIFLASH_Close();
}

/***	CETest_InitACL
**
**	Parameters:
**		
**
**	Return Value:
**		
**
**	Description:
**		This function implements the ACL test Init function. 
**      It is aimed to be called by the state machine engine.
**      It reads 10 times the ID from ACL.
**       
*/
void CETest_InitACL()
{
    unsigned char bID;
    int i;
    ACL_Init();

    for(i = 0; i< 10; i++)
    {
        bID = ACL_GetDeviceID();       
    }
    sprintf(strMsg, "ACL Test:ID 0x%2.2X", bID); // last ID
    LCD_WriteStringAtPos(strMsg, 1, 0);
    
}

/***	CETest_ExitACL
**
**	Parameters:
**		
**
**	Return Value:
**		
**
**	Description:
**		This function implements the ACL test Exit function. 
**      It is aimed to be called by the state machine engine.
**       
*/
void CETest_ExitACL()
{
    ACL_Close();
}

/***	CETest_InitIRDA
**
**	Parameters:
**		
**
**	Return Value:
**		
**
**	Description:
**		This function implements the IRDA test Init function. 
**      It is aimed to be called by the state machine engine.
**      It configures the IRDA to work over UART, and sends 10 times a character over the IRDA.
**      
*/
void CETest_InitIRDA()
{
    int i;
    LCD_WriteStringAtPos("IRDA test       ", 1, 0);
    IRDA_Init(9600);

    for(i = 0; i< 10; i++)
    {
        IRDA_UARTPutChar(0x23);   
    }    
}

/***	CETest_ExitIRDA
**
**	Parameters:
**		
**
**	Return Value:
**		
**
**	Description:
**		This function implements the IRDA test Exit function. 
**      It is aimed to be called by the state machine engine.
**      
*/
void CETest_ExitIRDA()
{
    IRDA_Close();
}

/***	CETest_InitMOT
**
**	Parameters:
**		
**
**	Return Value:
**		
**
**	Description:
**		This function implements the MOTOR test Init function. 
**      It is aimed to be called by the state machine engine.
**      It configures the MOTOR module in the Phase / Enable mode. Then it drives the motors with a 50% duty.
**      
*/
void CETest_InitMOT()
{
    LCD_WriteStringAtPos("Motor test      ", 1, 0);
    MOT_Init(1);
    MOT_SetPhEnMotor1(1, 128);
    MOT_SetPhEnMotor2(1, 128);
}

/***	CETest_ExitMOT
**
**	Parameters:
**		
**
**	Return Value:
**		
**
**	Description:
**		This function implements the MOTOR test Exit function. 
**      It is aimed to be called by the state machine engine.
**      
*/
void CETest_ExitMOT()
{
    MOT_Close();
}

/***	CETest_InitSRV
**
**	Parameters:
**		
**
**	Return Value:
**		
**
**	Description:
**		This function implements the Servo test Init function. 
**      It is aimed to be called by the state machine engine.
**      It outputs a 1.5ms pulse width a 20 ms period.
**      
*/
void CETest_InitSRV()
{
    LCD_WriteStringAtPos("Servos test     ", 1, 0);
    SRV_Init();
    SRV_SetPulseMicroseconds1(1500);
    SRV_SetPulseMicroseconds2(1500);
}

/***	CETest_ExitSRV
**
**	Parameters:
**		
**
**	Return Value:
**		
**
**	Description:
**		This function implements the Servo test Exit function. 
**      It is aimed to be called by the state machine engine.
**      
*/
void CETest_ExitSRV()
{
    SRV_Close();
}

/***	CETest_InitUART
**
**	Parameters:
**		
**
**	Return Value:
**		
**
**	Description:
**		This function implements the UART test Init function. 
**      It is aimed to be called by the state machine engine.
**      It initializes the UART at 115200 baud and outputs some text.
**      
*/
void CETest_InitUART()
{
    LCD_WriteStringAtPos("UART test       ", 1, 0);
    UART_Init(115200);
    UART_PutString("Digilent CE Test");   
}

/***	CETest_ExitUART
**
**	Parameters:
**		
**
**	Return Value:
**		
**
**	Description:
**		This function implements the UART test Exit function. 
**      It is aimed to be called by the state machine engine.
**      
*/
void CETest_ExitUART()
{
    UART_Close();
}


/***	Timer4SR
**
**	Parameters:
**		none
**
**	Return Value:
**		none
**
**	Description:
**		This is the ISR for the Timer4. It is trigggered every 100ms. 
 *      It implements the state machine engine:
 *      For every entry in the stateMachinesElems
 *          - the State Init function is called (if defined)
 *          - if the Step Function and step counter value are defined, at every step counter interval the Step Function  is called
 *          - after the time described by state counter value, the State Exit function is called (if defined), and the new state becomes active (in a circular manner)
 **          
*/
void __ISR(_TIMER_4_VECTOR, IPL2SRS) Timer4SR(void) 
{
    static unsigned int idxTimerWithinStep = 0, idxTimerWithinState = 0, idxState = 0, idxStep = 0;
    if(stateMachinesElems[idxState].pfStepTestFnc && 
            (++idxTimerWithinStep >= (stateMachinesElems[idxState].cntStepTimer)))
    {
        // call the step function
        (*stateMachinesElems[idxState].pfStepTestFnc)(idxStep++);
        
        // reset counter within step
        idxTimerWithinStep = 0;
        
    }
            

    if(++idxTimerWithinState >= (stateMachinesElems[idxState].cntStateTimer))
    {

        // move to next state
        
        // exit the old state
        if(stateMachinesElems[idxState].pfExitTestFnc)
        {
            (*stateMachinesElems[idxState].pfExitTestFnc)();
        }
        if(++idxState == NO_STATES)
        {
            // loop within states
            idxState = 0;
        }
        // init the new state
        if(stateMachinesElems[idxState].pfInitTestFnc)
        {
            (*stateMachinesElems[idxState].pfInitTestFnc)();
        }
        

        // reset counter within state
        idxTimerWithinState = 0;
        // reset counter within step
        idxTimerWithinStep = 0;
        idxStep = 0;
    }
    
    
//LATDINV = (1<<9);   //debug - toggle RD9 (JB1)  
    IFS0bits.T4IF = 0;                  // clear interrupt flag
}

#define TMR_TIME    0.01 // 10 ms for each tick
/***	Timer4Setup
**
**	Parameters:
**		none
**
**	Return Value:
**		none
**
**	Description:
**		This function configures Timer4 to generate in interrupt every 10ms, to be used for the state machine timing.
**          
*/
void Timer4Setup()
{
    static int fTimerInitialised = 0;
    if(!fTimerInitialised)
    {
        macro_disable_interrupts;             // INT step 2: disable interrupts at CPU
                                          // INT step 3: setup peripheral
        T4CONbits.TCKPS = 7;                //            1:256 prescale value
        T4CONbits.TGATE = 0;                //             not gated input (the default)
        T4CONbits.TCS = 0;                  //             PCBLK input (the default)
        TMR4 = 0;                           //             initialize count to 0
        PR4 = (int)(((float)(TMR_TIME * PB_FRQ) / 256) + 0.5);                        //             set period register, generates one interrupt every 10 ms
        T4CONbits.ON = 1;                   //             turn on Timer1
        IPC4bits.T4IP = 2;                  // INT step 4: priority
        IPC4bits.T4IS = 0;                  //             subpriority
        IFS0bits.T4IF = 0;                  // INT step 5: clear interrupt flag
        IEC0bits.T4IE = 1;                  // INT step 6: enable interrupt
        macro_enable_interrupts();  // INT step 7: enable interrupts at CPU
        fTimerInitialised = 1;
    }
}

/***	STATEMACHINE_Main
**
**	Parameters:
**		
**
**	Return Value:
**		
**
**	Description:
**		This function implements the CE Test. It is aimed to be called from main.
**      The function firstt initializes the LCD and displays the intro message.
**      Then it fires the state machine by configuring Timer4.
**      Prior to this, the first state is initialized (needed because of the way state machine engine is implemented).
**          
*/
void STATEMACHINE_Main()
{
// TRISDbits.TRISD9 = 0;//output, debug    
    LCD_Init(); 

    LCD_WriteStringAtPos("BasysMX3 LibPack", 0, 0);
    LCD_WriteStringAtPos("", 1, 0);
 
    // init the first state
    (*stateMachinesElems[0].pfInitTestFnc)();

    Timer4Setup();

}
