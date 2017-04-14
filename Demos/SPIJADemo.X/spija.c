/* ************************************************************************** */
/** Descriptive File Name

  @Company
    Digilent

  @File Name
    spija.c

  @Description
        This file groups the functions that implement the SPIJA library.
        The library implements SPI2 access over pins on PMODA connector (JA).
        SPIJA_CE   ->   JA1 (RC2)
        SPIJA_SI   ->   JA2 (RC1)
        SPIJA_SO   ->   JA3 (RC4)
        SPIJA_SCK  ->   JA4 (RG6)
        
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
#include "spija.h"

/* ************************************************************************** */
/***	SPIJA_Init
**
**	Parameters:
**		
**
**	Return Value:
**		
**
**	Description:
**		This function initializes the hardware involved in the SPIJA module: 
**      The following digital pins are configured as digital outputs: SPIJA_CE (JA1), SPIJA_SCK(JA2), SPIJA_SI(JA3)
**      The following digital pins are configured as digital inputs: SPIJA_SO(JA4).
**      The SPIJA_SI and SPIJA_SO are mapped over the SPI2 interface.
**      The SPI2 module of PIC32 is configured to work at 1 Mhz, polarity 0 and edge 1.
**      
**          
*/
void SPIJA_Init()
{
    SPIJA_ConfigurePins();
    SPIJA_ConfigureSPI(1000000, 0, 1); // configures SPI to work at 1 Mhz, polarity 0 and edge 1.
}

/* ************************************************************************** */
/***	SPIJA_ConfigureSPI
**
**	Parameters:
**		unsigned int spiFreq - SPI clock frequency (Hz).
**                                     for example 1000000 corresponds to 1 MHz
**		unsigned char pol - SPI Clock Polarity, similar to CKP field of SPIxCON
**                  1 = Idle state for clock is a high level; active state is a low level
**                  0 = Idle state for clock is a low level; active state is a high level
**		unsigned char edge - SPI Clock Edge, similar to CKE field of SPIxCON
**                  1 = Serial output data changes on transition from active clock state to Idle clock state (see CKP bit)
**                  0 = Serial output data changes on transition from Idle clock state to active clock state (see CKP bit)
**
**	Return Value:
**		
**
**	Description:
**		This function configures the SPI2 hardware interface of PIC32, according to the provided parameters.
**      In order to compute the baud rate value, it uses the peripheral bus frequency definition (PB_FRQ, located in config.h)
**      This is a low-level function called by initialization functions, so user should avoid calling it directly.
**      
**          
*/
void SPIJA_ConfigureSPI(unsigned int spiFreq, unsigned char pol, unsigned char edge)
{
    SPI2BRG = PB_FRQ / (2 * spiFreq) - 1;
    // configures SPI2
    SPI2CONbits.CKP = pol;    // SPI Clock Polarity
    SPI2CONbits.CKE = edge;   // SPI Clock Edge    
    SPI2CONbits.SMP = 0;      // SPI Data Input Sample Phase 
    SPI2CONbits.MSTEN = 1;    // Master
    SPI2CONbits.MODE16 = 0;   // 8 bit transfer
    SPI2CONbits.MODE32 = 0;   // 8 bit transfer
    SPI2CON2bits.AUDEN = 0;   // Audio protocol is disabled
    SPI2CONbits.ON = 1;       // enable SPI
}

/* ************************************************************************** */
/***	SPIJA_ConfigurePins
**
**	Parameters:
**		
**
**	Return Value:
**		
**
**	Description:
**		This function configures the digital pins involved in the SPIJA module: 
**      The following digital pins are configured as digital outputs: SPIJA_CE (JA1), SPIJA_SCK(JA2), SPIJA_SI(JA3)
**      The following digital pins are configured as digital inputs: SPIJA_SO(JA4).
**      The SPIJA_SI and SPIJA_SO are mapped over the SPI2 interface.
**      The function uses pin related definitions from config.h file.
**      This is a low-level function called by SPIJA_Init(), so user should avoid calling it directly.     
**      
**          
*/
void SPIJA_ConfigurePins()
{
    // Configure SPIJA signals as digital outputs.
    tris_SPIJA_CE = 0;
    tris_SPIJA_SCK = 0;
    tris_SPIJA_SI = 0;
    
    // Configure SPIJA signals as digital inputs.
    tris_SPIJA_SO = 1;
    
    // configure remapable pins
    rp_SPIJA_SI = 0x06; // RPC1R = 0110 = SDO2
    rp_SPIJA_SO = 0x0A;// SDI2R = 1010 = RPC4
    
    ansel_SPIJA_SCK = 0;
    
    PMCONbits.ON = 0;
    RPG6R = 0;
    CM1CONbits.ON = 0;
}

/* ************************************************************************** */
/***	SPIJA_RawTransferByte
**
**	Parameters:
**		unsigned char bVal  - the byte to be transmitted over SPI
**
**	Return Value:
**		unsigned char       - the byte received over SPI	
**
**	Description:
**		This function implements basic byte transfer over SPI2. 
**      It transmits the parameter bVal and returns the received byte.
**      This function does not handle Slave Select (SPIJA_CE) pin. 
**      This is a low-level function called by SPIJA_TransferBytes(), so user should avoid calling it directly.     
 **          
*/
unsigned char SPIJA_RawTransferByte(unsigned char bVal)
{
    while(!SPI2STATbits.SPITBE);	// wait for TX buffer to be empty
    SPI2BUF = bVal;
    while(!SPI2STATbits.SPIRBF);	// wait for RX buffer to be empty
    return SPI2BUF;
}

/* ************************************************************************** */
/***	SPIJA_TransferBytes
**
**	Parameters:
**      int bytesNumber         - Number of bytes to be transfered.
**      unsigned char *pbRdData - Pointer to a buffer storing the received bytes.
**      unsigned char *pbWrData - Pointer to a buffer storing the bytes to be transmitted.
**
**	Return Value:
**
**	Description:
**		This function implements transfer of a number of bytes over SPI2. 
**      It transmits the bytes from pbWrData and receives the bytes in pbRdData.
**      This function properly handles Slave Select (SPIJA_CE) pin.
**      
**          
*/
void SPIJA_TransferBytes(int bytesNumber, unsigned char *pbRdData, unsigned char *pbWrData)
{
    int i;
    lat_SPIJA_CE = 0; // Activate SS
    for(i = 0; i< bytesNumber; i++)
    {
        pbRdData[i] = SPIJA_RawTransferByte(pbWrData[i]);
    }
    lat_SPIJA_CE = 1; // Deactivate SS
}

/* ************************************************************************** */
/***	SPIJA_Close
**
**	Parameters:
** 
**
**	Return Value:
**      
**
**	Description:
**		This functions releases the hardware involved in SPIJA library: 
**      it turns off the SPI2 interface.
**      
**          
*/
void SPIJA_Close()
{
    SPI2CONbits.ON = 0; // disable SPI
}


/* *****************************************************************************
 End of File
 */
