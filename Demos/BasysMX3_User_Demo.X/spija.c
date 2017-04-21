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
#include "spija.h"

/* ************************************************************************** */

void SPIJA_ConfigureSPI()
{
    SPI2BRG = 10;
    // configures SPI2
    SPI2CONbits.CKP = 0;    // Clock Polarity
    SPI2CONbits.SMP = 0;    // SPI Data Input Sample Phase 
    SPI2CONbits.MSTEN = 1;  // Master
    SPI2CONbits.MODE16 = 0; // 8 bit transfer
    SPI2CONbits.MODE32 = 0; // 8 bit transfer
    SPI2CON2bits.AUDEN = 0; // Audio protocol is disabled
    SPI2CONbits.ON = 1; // enable SPI
    

 //   SPI2CON=0x8220; // SPI ON, 8 bits transfer, SMP=1, Master mode
}

void SPIJA_Init()
{
    SPIJA_ConfigurePins();
    SPIJA_ConfigureSPI();
}

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
//void inline SPIJA_WaitForDataByte()
//{
//    while(!SPI2STATbits.SPITBE);	// wait for RX buffer to fill 
////    while(!SPI2STATbits.SPIRBF);	// wait for RX buffer to fill 
////    while((SPI2STATbits.SPITBF == 1) || (SPI2STATbits.SPIRBF == 0));
//}
void SPIJA_WriteByte(unsigned char bData)
{
    lat_SPIJA_CE = 0; // Activate CS
    while(!SPI2STATbits.SPITBE);	// wait for TX buffer to be empty
    SPI2BUF = bData;
    while(!SPI2STATbits.SPIRBF);	// wait for TX buffer to be empty
//    SPIJA_WaitForDataByte();
//    DelayAprox10Us(1);
    lat_SPIJA_CE = 1; // Deactivate CS
}

void SPIJA_TrasferBytes(int nBytes, unsigned char *pbRdData, unsigned char *pbWrData)
{
    int i;
    lat_SPIJA_CE = 0; // Activate CS
    for(i = 0; i< nBytes; i++)
    {
        while(!SPI2STATbits.SPITBE);	// wait for TX buffer to be empty
        SPI2BUF = pbWrData[i];
        while(!SPI2STATbits.SPIRBF);	// wait for TX buffer to be empty
        pbRdData[i] = SPI2BUF;
    }
    lat_SPIJA_CE = 1; // Deactivate CS
}
/* *****************************************************************************
 End of File
 */
