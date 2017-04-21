/* ************************************************************************** */
/** Descriptive File Name

  @Company
    Company Name

  @File Name
    filename.h

  @Summary
    Brief description of the file.

  @Description
    Describe the purpose of this file.
 */
/* ************************************************************************** */

#ifndef _SPIFLASH_H    /* Guard against multiple inclusion */
#define _SPIFLASH_H

void SPIFLASH_Init();
void SPIFLASH_ConfigureSPI();
void SPIFLASH_ConfigurePins();
unsigned char SPIFLASH_TransferByte(unsigned char bVal);
void SPIFLASH_TransferBytes(int nBytes, unsigned char *pbRdData, unsigned char *pbWrData);
void SPIFLASH_SendOneByteCmd(unsigned char bCmd);

unsigned char SPIFLASH_ReleasePowerDownGetDeviceID();

unsigned char SPIFLASH_GetStatus();
void SPIFLASH_WaitUntilNoBusy();
void SPIFLASH_WriteEnable();
void SPIFLASH_WriteDisable();

void SPIFLASH_Erase4k(unsigned int addr);
void SPIFLASH_Erase64k(unsigned int addr);
void SPIFLASH_EraseAll();

void SPIFLASH_ProgramPage(unsigned int addr, unsigned char *pBuf, unsigned int len);

void SPIFLASH_Read(unsigned int addr, unsigned char *pBuf, unsigned int len);



//#ifdef __cplusplus
//extern "C" {
//#endif
#define SPIFLASH_CMD_PROGRAMPAGE        0x02    // SPI Flash opcode: Page program
#define SPIFLASH_CMD_READ               0x03    // SPI Flash opcode: Read up up to 25MHz
#define SPIFLASH_CMD_READ_FAST			0x0B    // SPI Flash opcode: Read up to 50MHz with 1 dummy byte
#define SPIFLASH_CMD_ERASE_4K			0x20    // SPI Flash opcode: 4KByte sector erase
#define SPIFLASH_CMD_ERASE_64K          0xD8    // SPI Flash opcode: 64KByte block erase
#define SPIFLASH_CMD_ERASE_ALL			0x60    // SPI Flash opcode: Entire chip erase
#define SPIFLASH_CMD_WRITE				0x02    // SPI Flash opcode: Write one byte (or a page of up to 256 bytes, depending on device)
#define SPIFLASH_CMD_WRITE_WORD_STREAM	0xAD    // SPI Flash opcode: Write continuous stream of 16-bit words (AAI mode); available on SST25VF016B (but not on SST25VF010A)
#define SPIFLASH_CMD_WRITE_BYTE_STREAM	0xAF    // SPI Flash opcode: Write continuous stream of bytes (AAI mode); available on SST25VF010A (but not on SST25VF016B)
#define SPIFLASH_CMD_RDSR				0x05    // SPI Flash opcode: Read Status Register
#define SPIFLASH_CMD_EWSR				0x50    // SPI Flash opcode: Enable Write Status Register
#define SPIFLASH_CMD_WRSR				0x01    // SPI Flash opcode: Write Status Register
#define SPIFLASH_CMD_WREN				0x06    // SPI Flash opcode: Write Enable
#define SPIFLASH_CMD_WRDI				0x04    // SPI Flash opcode: Write Disable / End AAI mode
#define SPIFLASH_CMD_RDID				0x90    // SPI Flash opcode: Read ID
#define SPIFLASH_CMD_JEDEC_ID			0x9F    // SPI Flash opcode: Read JEDEC ID
#define SPIFLASH_CMD_EBSY				0x70    // SPI Flash opcode: Enable write BUSY status on SO pin
#define SPIFLASH_CMD_DBSY				0x80    // SPI Flash opcode: Disable write BUSY status on SO pin
#define SPIFLASH_CMD_PWRDWN_DEVID       0xAB    // SPI Flash opcode: Release from Deep-Power-Down / Device ID


#define SPIFLASH_STATUS_BUSY            0x01    // Busy bit of SR1




    // *****************************************************************************
    // *****************************************************************************
    // Section: Interface Functions
    // *****************************************************************************
    // *****************************************************************************



    /* Provide C++ Compatibility */
//#ifdef __cplusplus
//}
//#endif
#endif /* _SPIFLASH_H */

/* *****************************************************************************
 End of File
 */
