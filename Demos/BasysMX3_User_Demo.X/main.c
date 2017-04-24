/* 
 * File:   main.c
 * Author: Cristian Fatu
 *
 * Created on March 22, 2016, 14:59 
 */

#include <stdio.h>
#include <stdlib.h>
#include <xc.h>
#include <sys/attribs.h>
#include "config.h"

#include "lcd.h"
#include "adc.h"
#include "btn.h"
#include "swt.h"
#include "led.h"
#include "acl.h"
#include "timed.h"


#pragma config JTAGEN = OFF     
#pragma config FWDTEN = OFF      


/* ------------------------------------------------------------ */
/*						Configuration Bits		                */
/* ------------------------------------------------------------ */


// Device Config Bits in  DEVCFG1:	
#pragma config FNOSC =	FRCPLL
#pragma config FSOSCEN =	OFF
#pragma config POSCMOD =	EC
#pragma config OSCIOFNC =	ON
#pragma config FPBDIV =     DIV_1

// Device Config Bits in  DEVCFG2:	
#pragma config FPLLIDIV =	DIV_2
#pragma config FPLLMUL =	MUL_20
#pragma config FPLLODIV =	DIV_1

 #define _XTAL_FREQ 8000000     // 

#define LINES 6
#define LENGTH 16

unsigned int POT_Val();

char LCD_Strings[LINES][LENGTH]={ "Welcome! To get ","started go to:  ",
                                        "digilentinc.com ","/basysmx3       ",
                                        "Press BTNC to   ","toggle display. "};
int LCD_Line=0;

volatile unsigned char UPDATE_DISP=0;
volatile unsigned char ACL_POT_EN=0;
unsigned char btn_down=0;

int main(int argc, char** argv) 
{
    unsigned char bSwtVal;
    float rgACLGVals[3];  
    char strMsg[80];    
    int valPot;
    
    LCD_Init(); 
    LCD_WriteStringAtPos(LCD_Strings[LCD_Line], 0, 0);
    LCD_WriteStringAtPos(LCD_Strings[LCD_Line+1], 1, 0);
    SWT_Init();
    LED_Init();
    ADC_Init();
    ACL_Init();
    BTN_Init();
    VisCtrlsTest();
    while(1)
    {
            bSwtVal = SWT_GetGroupValue();
            LED_SetGroupValue(bSwtVal);
            
            if(ACL_POT_EN==1){
                ACL_ReadGValues(rgACLGVals);
                valPot = POT_Val();
                sprintf(strMsg, "POT:%04dX:%6.3f", valPot, rgACLGVals[0]);
                LCD_WriteStringAtPos(strMsg, 0, 0);
                sprintf(strMsg, "Y:%6.3fZ:%6.3f", rgACLGVals[1], rgACLGVals[2]);            
                LCD_WriteStringAtPos(strMsg, 1, 0);
            }
            else if (UPDATE_DISP){
                UPDATE_DISP=0;
                LCD_Line+=2;
                LCD_Line%=LINES;
                LCD_WriteStringAtPos(LCD_Strings[LCD_Line], 0, 0);
                LCD_WriteStringAtPos(LCD_Strings[LCD_Line+1], 1, 0);
            }
            if(btn_down==0){//If button is released
                if(BTN_GetValue(0)){//U
                    btn_down=1;
                }
                else if(BTN_GetValue(1)){//L
                    btn_down=1;
                }
                else if(BTN_GetValue(2)){//C Toggle between Message and ACL/Potentiometer Display
                    btn_down=1;
                    ACL_POT_EN^=1;
                    if(!ACL_POT_EN){
                        LCD_WriteStringAtPos(LCD_Strings[LCD_Line], 0, 0);
                    }
                }
                else if(BTN_GetValue(3)){//R
                    btn_down=1;
                }
                else if(BTN_GetValue(4)){//D
                    btn_down=1;
                }
            }
            else{
                if (BTN_GetGroupValue()==0){
                    btn_down=0;
                }
            }
    }
    return (1);
}


unsigned int POT_Val()
{
    return ADC_AnalogRead(2);    // Read the ADC Value
}