/* ************************************************************************** */
/** Descriptive File Name

  @Company
    Company Name

  @File Name
    filename.c

  @Summary
    Brief description of the file.

  @Description    Describe the purpose of this file.
 */
/* ************************************************************************** */

/* ************************************************************************** */
/* ************************************************************************** */
/* Section: Included Files                                                    */
/* ************************************************************************** */
#include <xc.h>
#include <sys/attribs.h>
#include "config.h"
#include "spiflash.h"

/* ************************************************************************** */

unsigned char rd[10], wr[10];
void SPIFLASH_Init()
{
    SPIFLASH_ConfigurePins();
    SPIFLASH_ConfigureSPI();
}

void SPIFLASH_ConfigureSPI()
{
//    SPI1BRG = 10; // 900 kHz
    SPI1BRG = 1;    // 5 Mhz
    // configures SPI1
    SPI1CONbits.CKP = 0;    // Clock Polarity
    SPI1CONbits.SMP = 0;    // SPI Data Input Sample Phase 
    SPI1CONbits.MSTEN = 1;  // Master
    SPI1CONbits.MODE16 = 0; // 8 bit transfer
    SPI1CONbits.MODE32 = 0; // 8 bit transfer
    SPI1CON2bits.AUDEN = 0; // Audio protocol is disabled
    SPI1CONbits.ON = 1; // enable SPI

    SPI1CONbits.CKE = 1;
//    SPI1CON=0x8220; // SPI ON, 8 bits transfer, SMP=1, Master mode
}

void SPIFLASH_ConfigurePins()
{
    // Configure SPIFLASH signals as digital outputs.
    tris_SPIFLASH_CE = 0;
    tris_SPIFLASH_SCK = 0;
    tris_SPIFLASH_SI = 0;
    
    // Configure SPIFLASH signals as digital inputs.
    tris_SPIFLASH_SO = 1;
    
    // configure remapable pins
    rp_SPIFLASH_SI = 0x08; // RPF2R = 1000 = SDO1 
    rp_SPIFLASH_SO = 0x0F;// SDI1R = 1111 = RPF7

}


void SPIFLASH_TransferBytes(int nBytes, unsigned char *pbRdData, unsigned char *pbWrData)
{
    int i;
    lat_SPIFLASH_CE = 0; // Activate CS
    for(i = 0; i< nBytes; i++)
    {
        pbRdData[i] = SPIFLASH_TransferByte(pbWrData[i]);
    }
    lat_SPIFLASH_CE = 1; // Deactivate CS
}

unsigned char SPIFLASH_ReleasePowerDownGetDeviceID()
{
    wr[0] = SPIFLASH_CMD_PWRDWN_DEVID;
    wr[1] = 0;
    wr[2] = 0;
    wr[3] = 0;
    wr[4] = 0;
    SPIFLASH_TransferBytes(5, rd, wr);
    return rd[4];
}

unsigned char SPIFLASH_TransferByte(unsigned char bVal)
{
    while(!SPI1STATbits.SPITBE);	// wait for TX buffer to be empty
    SPI1BUF = bVal;
    while(!SPI1STATbits.SPIRBF);	// wait for TX buffer to be empty
    return SPI1BUF;
}

void SPIFLASH_SendOneByteCmd(unsigned char bCmd)
{
    lat_SPIFLASH_CE = 0; // Activate CS
    SPIFLASH_TransferByte(bCmd);
    lat_SPIFLASH_CE = 1; // Deactivate CS
}

unsigned char SPIFLASH_GetStatus()
{
    unsigned char bResult;
    lat_SPIFLASH_CE = 0; // Activate CS
    SPIFLASH_TransferByte(SPIFLASH_CMD_RDSR);
    bResult = SPIFLASH_TransferByte(0);
    lat_SPIFLASH_CE = 1; // Deactivate CS
    return bResult;
}

void SPIFLASH_WaitUntilNoBusy()
{
    unsigned char status;
    while(SPIFLASH_GetStatus() & SPIFLASH_STATUS_BUSY);
}

void SPIFLASH_WriteEnable()
{
    SPIFLASH_SendOneByteCmd(SPIFLASH_CMD_WREN);
    SPIFLASH_WaitUntilNoBusy();
}

void SPIFLASH_WriteDisable()
{
    SPIFLASH_SendOneByteCmd(SPIFLASH_CMD_WRDI);
    SPIFLASH_WaitUntilNoBusy();
}

// erase the 4k sector containing the address
void SPIFLASH_Erase4k(unsigned int addr)
{
    SPIFLASH_WaitUntilNoBusy();
    SPIFLASH_WriteEnable();
    wr[0] = SPIFLASH_CMD_ERASE_4K;
    wr[1] = addr >> 16;
    wr[2] = addr >> 8;
    wr[3] = addr & 0xFF;
    SPIFLASH_TransferBytes(4, rd, wr);
    SPIFLASH_WaitUntilNoBusy();
}

// erase the 64k block containing the address
void SPIFLASH_Erase64k(unsigned int addr)
{
    SPIFLASH_WaitUntilNoBusy();
    SPIFLASH_WriteEnable();
    wr[0] = SPIFLASH_CMD_ERASE_64K;
    wr[1] = addr >> 16;
    wr[2] = addr >> 8;
    wr[3] = addr & 0xFF;
    SPIFLASH_TransferBytes(4, rd, wr);
    SPIFLASH_WaitUntilNoBusy();
}

// erase all chip
void SPIFLASH_EraseAll()
{
    SPIFLASH_WaitUntilNoBusy();
    SPIFLASH_WriteEnable();
    SPIFLASH_SendOneByteCmd(SPIFLASH_CMD_ERASE_ALL);
    SPIFLASH_WaitUntilNoBusy();
}

// program one page of memory; for the moment only write full page
void SPIFLASH_ProgramPage(unsigned int addr, unsigned char *pBuf, unsigned int len)
{
    int i;
    SPIFLASH_WaitUntilNoBusy();
    SPIFLASH_WriteEnable();
    
    lat_SPIFLASH_CE = 0; // Activate CS
    
    SPIFLASH_TransferByte(SPIFLASH_CMD_PROGRAMPAGE);
    SPIFLASH_TransferByte(addr >> 16);
    SPIFLASH_TransferByte(addr >> 8);
    SPIFLASH_TransferByte(addr & 0xFF);
    for(i = 0; i< len; i++)
    {
        SPIFLASH_TransferByte(pBuf[i]);
    }
    lat_SPIFLASH_CE = 1; // Deactivate CS
    SPIFLASH_WaitUntilNoBusy();
}

void SPIFLASH_Read(unsigned int addr, unsigned char *pBuf, unsigned int len)
{
    int i;
  //  SPIFLASH_WaitUntilNoBusy();
  //  SPIFLASH_WriteEnable();
    
    lat_SPIFLASH_CE = 0; // Activate CS
    
    SPIFLASH_TransferByte(SPIFLASH_CMD_READ);
    SPIFLASH_TransferByte(addr >> 16);
    SPIFLASH_TransferByte(addr >> 8);
    SPIFLASH_TransferByte(addr & 0xFF);
    for(i = 0; i< len; i++)
    {
        pBuf[i] = SPIFLASH_TransferByte(0);
    }
    lat_SPIFLASH_CE = 1; // Deactivate CS
//    SPIFLASH_WaitUntilNoBusy();
}


/*
uint8_t SPIFLASH_SPISwapByte(uint8_t bVal)
{

    // Clear the Write Collision flag, to allow writing
    SSP1CON1bits.WCOL = 0;
    SSP1BUF = bVal;
    while(!SSP1STATbits.BF);

    return SSP1BUF;
}
*/

/* *****************************************************************************
 End of File
 */
