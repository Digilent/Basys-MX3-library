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
#include "IrDA.h"

/* ************************************************************************** */


void IRDA_Init()
{
    // Configure SWTs as digital inputs.
    tris_IRDA_PDOWN = 0;
    tris_IRDA_RX = 1;
    tris_IRDA_TX = 0;

    // disable analog (set pins as digital))
    ansel_IRDA_RX = 0;
    ansel_IRDA_TX = 0;
    
    IRDA_set_SMIR_mode();
//    IRDA_set_FIR_mode();
    
    // set the external interrupt 1 for RX pin
//    INT1R = 0x05;           //0b0101 <=> RPB6
    IRDA_ConfigureIRDAOverUART5();
}

void IRDA_set_FIR_mode()
{
    DelayAprox10Us(1);
    IRDA_PDOWN = 1;
    DelayAprox10Us(1);
    IRDA_TX = 1;
    DelayAprox10Us(1);
    
    IRDA_PDOWN = 0;
    DelayAprox10Us(1);
    IRDA_TX = 0;
    DelayAprox10Us(1);
}

void IRDA_set_SMIR_mode()
{
    DelayAprox10Us(1);
    
    IRDA_TX = 0;
    DelayAprox10Us(1);
    
    IRDA_PDOWN = 1;
    DelayAprox10Us(1);
    
    IRDA_PDOWN = 0;
    DelayAprox10Us(1);
}



void IRDA_ConfigureIRDAOverUART5()
{
    // IRDA RX will be connected to 
    // configure IR_RX (B6) -> U5RX
    U5RXR = 5; //0101 = RPB6
    
    // configure IR_TX (B7) -> U5TX
    RPB7R = 4; //0100 = U5TX
    
    
    // configure UART5
    // 9600 baud, PBFreq = 80MHz
    U5MODEbits.ON     = 0;
    U5MODEbits.SIDL   = 0;
    U5MODEbits.RTSMD  = 0;
    U5MODEbits.UEN0   = 0; 
    U5MODEbits.UEN1   = 0;
    U5MODEbits.WAKE   = 0;
    U5MODEbits.LPBACK = 0; 
    U5MODEbits.ABAUD  = 0;
    U5MODEbits.RXINV  = 0; 
    U5MODEbits.BRGH   = 0; 
    U5MODEbits.PDSEL1 = 0; 
    U5MODEbits.PDSEL0 = 0; 
    U5MODEbits.STSEL  = 0;  

    U5BRG = 519;         // BAUDRATE = 80MHz/16/9600-1 = 0d259  Ś9600BPS(40MHz)


    U5MODEbits.ON      = 1; 
    U5STAbits.UTXEN    = 1;
    U5STAbits.URXEN    = 1;    

    // IRDA related
    U5MODEbits.IREN   = 1; // IrDA Encoder and Decoder Enable bit: 1 = IrDA is enabled
    U5STAbits.UTXINV   = 1;    // Transmit Polarity Inversion bit: 1 = IrDA encoded UxTX Idle state is ‘1’
}

void IRDA_UARTPutChar(char ch)
{
    while(U5STAbits.UTXBF == 1);
    U5TXREG = ch;
}

unsigned char IRDA_UART_GetChar(unsigned int timeout, char *error) 
{
    unsigned int timeout_cnt = 0;
    
    if(timeout == 0)
    {
        while (!IRDA_UART_AvaliableRx());
    }
    else
    {
        while ((!IRDA_UART_AvaliableRx()) && timeout_cnt < timeout)
        {
            timeout_cnt++;
        }
        if(timeout_cnt == timeout)
            *error = 1;
        else
            *error = 0;
    }
    
    return U5RXREG;
}

unsigned char IRDA_UART_AvaliableRx()
{
    return U5STAbits.URXDA;
}
/* *****************************************************************************
 End of File
 */

 /*
const unsigned char irda_tx_values[] = {0xaa, 0x83, 0x27, 0x16, 0xf5, 0x6e, 0xb2, 0x9a};
int main(int argc, char** argv) 
{
   unsigned int wCnt = 0x3456, baseCnt = 0;
    unsigned char bVal1, bVal2, bVal;
    char strMsg[12];    
    unsigned int irda_counter = 0;
    float rgACLGVals[3];        
    unsigned int cnt = 0;
    unsigned char irda_val = 0, irda_lastTransmit;
    int genCnt = 0, valPot, i;
    ADC_Init();
    
    TRISDbits.TRISD9 = 0;   //debug - RD9 (JB1) output
    TRISDbits.TRISD11 = 0;   //debug - RD11 (JB2) output
        
    LCD_Init(); 
    LED_Init();
    SWT_Init();
    IRDA_Init();
    SSD_Init();
   BTN_Init();
    IRDA_ConfigureIRDAOverUART5();
    SSD_WriteDigitsGrouped(wCnt, 0);
    
    unsigned char irda_rx_val = 0;
    const unsigned char irda_max_val = 0xae;
    
    unsigned char master_board = 0, slave_board = 0;
    
    LCD_WriteStringAtPos("irDA mirror test", 0, 0);
    

    char rx_err;
    
    while(IRDA_UART_AvaliableRx())
    {
        IRDA_UART_GetChar(65000, &rx_err);;
    }
    
    while(1)
    {
        
        if(BTN_GetValue('u') == 1)
        {   
            master_board = 1;
            slave_board = 0;
            LCD_WriteStringAtPos("Master          ", 0, 0);
        }
        else if(BTN_GetValue('d') == 1)
        {
            master_board = 0;
            slave_board = 1;
            LCD_WriteStringAtPos("Slave           ", 0, 0);
        }
        
        if(master_board == 1)
        {
            // IRDA_TX
            master_board = 0;
            
            sprintf(strMsg, "= :%2.2X     ", 0x84);
            LCD_WriteStringAtPos(strMsg, 1, 0);
            
            int ir_err = 0;
            
            for(i = 0; i < sizeof(irda_tx_values)/sizeof(irda_tx_values[0]); i++)
            {
                IRDA_UARTPutChar(irda_tx_values[i]);
                DelayAprox10Us(200); 

                // empty receive buffer - needed because of the inside feedback
                while(IRDA_UART_AvaliableRx())
                {
                    IRDA_UART_GetChar(65000, &rx_err);
                }

                DelayAprox10Us(200);
                
                irda_rx_val = IRDA_UART_GetChar(60000, &rx_err);
                
                if(irda_rx_val != irda_tx_values[i] || rx_err == 1)
                {
                    ir_err++;
                }
                
                sprintf(strMsg, "IRDA TX:%2.2X     ", irda_tx_values[i]);
                LCD_WriteStringAtPos(strMsg, 0, 0);
                
                sprintf(strMsg, "IRDA err:%2.2X     ", ir_err);
                LCD_WriteStringAtPos(strMsg, 1, 0);

//                DelayAprox10Us(40000);      //optional
            }
        }
        else if(slave_board == 1)
        {
            DelayAprox10Us(20);
            irda_rx_val = IRDA_UART_GetChar(0, &rx_err);

            DelayAprox10Us(20);
            // empty receive buffer
            while(IRDA_UART_AvaliableRx())
            {
                IRDA_UART_GetChar(0, &rx_err);
            }

            DelayAprox10Us(200);

            IRDA_UARTPutChar(irda_rx_val);

            DelayAprox10Us(200);
            // empty receive buffer - needed because of the inside feedback
            while(IRDA_UART_AvaliableRx())
            {
                IRDA_UART_GetChar(0, &rx_err);
            }

            sprintf(strMsg, "IRDA RX:%2.2X     ", irda_rx_val);
            LCD_WriteStringAtPos(strMsg, 1, 0);

            DelayAprox10Us(20);
            
            if(master_board == 1)
                slave_board = 0;
            
        }
        
        LED_SetGroupValue(irda_val);
    }

    return (1);
}
*/