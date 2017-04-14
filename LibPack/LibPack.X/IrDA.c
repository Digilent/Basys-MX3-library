/* ************************************************************************** */
/** Descriptive File Name

  @Company
    Digilent

  @File Name
    IrDA.c

  @Description
        This file groups the functions that implement the IRDA library.
        The library implements the control of IRDA module. It also provides the UART5
        interface over the IRDA transmit and receive pins.
        So, in order to send a byte over IRDA, the byte is transmitted over UART5, 
        thus it serializes each bit to be transmitted over IRDA. Receiving bits from IRDA are accumulated in a byte in UART5 receive buffer.
        Include the file together with utils.c and utils.h in the project when this library is needed.	

  @Author
    Cristian Ignat 
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

/* ------------------------------------------------------------ */
/***	IRDA_Init
**
**	Parameters:
**          unsigned int baud - the baud rate for the UART interface
**
**	Return Value:
**		
**
**	Description:
**		This function initializes the hardware involved in the IRDA module: 
**      The following digital pins are configured as digital outputs: IRDA_PDOWN, IRDA_TX.
**      The following digital pins are configured as digital inputs: IRDA_RX.
**      IRDA module is set in SMIR mode.
**      The IRDA_TX and IRDA_RX are mapped over UART5 interface, which is configured
**      to work at the specified baud.
**          
*/
void IRDA_Init(unsigned int baud)
{
    IRDA_ConfigurePins();
    IRDA_Set_SMIR_Mode();   
    IRDA_ConfigureIRDAOverUART5(baud);
}


/* ------------------------------------------------------------ */
/***	IRDA_ConfigurePins
**
**	Parameters:
**		
**
**	Return Value:
**		
**
**	Description:
**		This function configures the digital pins involved in the IRDA module: 
**      The following digital pins are configured as digital outputs: IRDA_PDOWN, IRDA_TX.
**      The following digital pins are configured as digital inputs: IRDA_RX.
**      The function uses pin related definitions from config.h file.
**      This is a low-level function called by IRDA_Init(), so user should avoid calling it directly.
**               
*/
void IRDA_ConfigurePins()
{
    // Configure IRDA_PDOWN and IRDA_TX as digital outputs.
    tris_IRDA_PDOWN = 0;
    tris_IRDA_TX = 0;

    // Configure IRDA_RX as digital inputs.
    tris_IRDA_RX = 1;
    
    // disable analog (set pins as digital))
    ansel_IRDA_RX = 0;
    ansel_IRDA_TX = 0;
}

/* ------------------------------------------------------------ */
/***	IRDA_Set_FIR_Mode
**
**	Parameters:
**          
**
**	Return Value:
**		
**
**	Description:
**		This function configures the IRDA module to work in FIR mode.
**          
*/
void IRDA_Set_FIR_Mode()
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

/* ------------------------------------------------------------ */
/***	IRDA_Set_SMIR_Mode
**
**	Parameters:
**          
**
**	Return Value:
**		
**
**	Description:
**		This function configures the IRDA module to work in FIR mode.
**          
*/
void IRDA_Set_SMIR_Mode()
{
    DelayAprox10Us(1);
    
    IRDA_TX = 0;
    DelayAprox10Us(1);
    
    IRDA_PDOWN = 1;
    DelayAprox10Us(1);
    
    IRDA_PDOWN = 0;
    DelayAprox10Us(1);
}


/* ------------------------------------------------------------ */
/***	IRDA_ConfigureIRDAOverUART5
**
**	Parameters:
**          unsigned int baud - the baud rate for the UART interface
**                              for example 9600 corresponds to 9600 baud.
**
**	Return Value:
**		
**
**	Description:
**		This function initializes the UART5 to work in conjuction with IRDA module. 
**      The IRDA_TX and IRDA_RX are mapped over UART5 interface pins.
**      UART5 is configured to work at the specified baud rate
**      In order to compute the baud rate value, it uses the peripheral bus frequency definition (PB_FRQ, located in config.h)
**          
*/
void IRDA_ConfigureIRDAOverUART5(unsigned int baud)
{
    // IRDA RX will be connected to U5RX 
    // configure IR_RX (RB6) -> U5RX
    U5RXR = 5; //0101 = RPB6
    
    // IRDA TX will be connected to U5TX 
    //configure IR_TX (RB7) -> U5TX
    RPB7R = 4; //0100 = U5TX
    // configure UART5
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


    U5BRG = (int)(((float)PB_FRQ/(16*baud) - 1) + 0.5); // add 0.5 just in order to implement the round using the floor approach


    U5MODEbits.ON      = 1; 
    U5STAbits.UTXEN    = 1;
    U5STAbits.URXEN    = 1;    

    // IRDA related
    U5MODEbits.IREN   = 1;     // IrDA Encoder and Decoder Enable bit: 1 = IrDA is enabled
    U5STAbits.UTXINV   = 1;    // Transmit Polarity Inversion bit: 1 = IrDA encoded UxTX Idle state is "1"
}

/* ------------------------------------------------------------ */
/***	IRDA_UARTPutChar
**
**	Parameters:
**          char ch -   the character to be transmitted over IRDA.
**
**	Return Value:
**		
**
**	Description:
**		This function transmits a character over IRDA, by transmitting it over UART5. 
**      
**          
*/
void IRDA_UARTPutChar(char ch)
{
    while(U5STAbits.UTXBF == 1);
    U5TXREG = ch;
}

/* ------------------------------------------------------------ */
/***	IRDA_UART_GetChar
**
**  Synopsis:
 **      irda_rx_val = IRDA_UART_GetChar(60000, &rx_err);
**	Parameters:
**      unsigned int timeout    - the number of times to check for an available received byte
**      char *error             - pointer to a value that returns the error status
**                                      0 - no timeout
**                                      1 - timeout
**
**	Return Value:
**          - unsigned char - the byte received over IRDA, by receiving it from UART5
**		
**
**	Description:
**		This function receives a character over IRDA. 
 *      It waits until a byte is received over UART5. 
**      Then, it returns the byte.
**      When after a number of times (specified by timeout parameter) no value is available on UART5, 
**      timeout condition is met: *error parameter will return 1 (otherwise it returns 0).
**          
*/
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

/* ------------------------------------------------------------ */
/***	IRDA_UART_AvaliableRx
**
**	Parameters:
**          
**
**	Return Value:
**          - unsigned char - Receive Buffer Data Available bit on UART5 
                    1 = Receive buffer has data, at least one more character can be read
                    0 = Receive buffer is empty
**		
**
**	Description:
**		This function returns UART5 Receive Buffer Data Available bit.
**      It returns 1 if the receive buffer has data (at least one more character can be read).
**      It returns 0 if the receive buffer is empty.
**      
**          
*/
unsigned char IRDA_UART_AvaliableRx()
{
    return U5STAbits.URXDA;
}

/* ------------------------------------------------------------ */
/***	IRDA_Close
**
**	Parameters:
** 
**
**	Return Value:
**      
**
**	Description:
**		This functions releases the hardware involved in IRDA library: 
**      it turns off the UART5 interface.
**      
**          
*/
void IRDA_Close()
{
      U5MODEbits.ON = 0;   
}
/* *****************************************************************************
 End of File
 */

 /* This is a demo of IRRDA usage
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