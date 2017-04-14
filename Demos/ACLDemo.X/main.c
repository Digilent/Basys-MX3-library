/* 
 * File:   main.c
 * Author: Cristian Fatu
 *
 * Created on March 10, 2017
 * Modified by Monica Ignat on March 31st, 2017
 */

#include <stdio.h>
#include <stdlib.h>
#include <xc.h>
#include <sys/attribs.h>
#include "config.h"

#include "lcd.h"
#include "acl.h"



#pragma config JTAGEN = OFF     
#pragma config FWDTEN = OFF      


/* ------------------------------------------------------------ */
/*						Configuration Bits		                */
/* ------------------------------------------------------------ */


// Device Config Bits in  DEVCFG1:	
#pragma config FNOSC =	FRCPLL
#pragma config FSOSCEN =	OFF
#pragma config POSCMOD =	XT
#pragma config OSCIOFNC =	ON
#pragma config FPBDIV =     DIV_2

// Device Config Bits in  DEVCFG2:	
#pragma config FPLLIDIV =	DIV_2
#pragma config FPLLMUL =	MUL_20
#pragma config FPLLODIV =	DIV_1

void ACLDemo();

int main(int argc, char** argv) 
{
    ACLDemo();   
    return (1);
}

/***	ACLDemo
**
**	Parameters:
**		
**
**	Return Value:
**		
**
**	Description:
**		This function initializes the ACL pins and calls specific functions for reading the ACL values
**      The X, Y and Z axes values are then displayed on the LCD
**          
*/
void ACLDemo(){
    float rgACLGVals[3];  
    char strMsg[80];    
    unsigned int baseCnt = 0;
    ACL_Init();
    LCD_Init(); 
    LCD_WriteStringAtPos("ACL Demo", 0, 0);
    LCD_WriteStringAtPos("Digilent", 1, 0);

    while(1)
    {
       //perform ACL readings only once in a while, to be able to visualize the results
        if(++baseCnt == 400000)
       {
            baseCnt = 0;        
            ACL_ReadGValues(rgACLGVals);
            //display on the LCD screen the X values, first row
            sprintf(strMsg, "ACLDemo X:%6.3f", rgACLGVals[0]);
            LCD_WriteStringAtPos(strMsg, 0, 0);
            //display on the LCD screen the Y and Z values, second row
            sprintf(strMsg, "Y:%6.3fZ:%6.3f", rgACLGVals[1], rgACLGVals[2]);            
            LCD_WriteStringAtPos(strMsg, 1, 0);     
        }
    }  
}