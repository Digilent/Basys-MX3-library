/* ************************************************************************** */
/** Descriptive File Name

  @Company
    Digilent

  @File Name
    lcd.h

  @Description
     This file groups the declarations of the functions that implement
     the LCD library (defined in lcd.c).
     Include the file in the project when this library is needed.
     Use #include "lcd.h" in the source files where the functions are needed.
 */
/* ************************************************************************** */

#ifndef _LCD_H    /* Guard against multiple inclusion */
#define _LCD_H

#define cmdLcdFcnInit 	0x3C        // function set command, (8-bit interface, 2 lines, and 5x8 dots)
#define cmdLcdCtlInit 	0x0C        // display control set command
#define cmdLcdEntryMode 0x06        // Entry Mode Set
#define cmdLcdClear		0x01		// clear display command
#define cmdLcdRetHome	0x02		// return home command
#define cmdLcdDisplayShift 0x18		// shift display command
#define cmdLcdCursorShift  0x10		// shift cursor command
#define cmdLcdSetDdramPos	0x80	// set DDRAM position command
#define cmdLcdSetCgramPos	0x40	// set CGRAM position command

#define mskBStatus 0x80             // bit busy 
#define mskShiftRL 0x04             // bit for direction 
#define displaySetOptionDisplayOn	0x4 // Set Display On option
#define	displaySetOptionCursorOn 	0x2 // Set Cursor On option
#define	displaySetBlinkOn 			0x1 // Set Blink On option


#define posCgramChar0 0		// position in CGRAM for character 0
#define posCgramChar1 8		// position in CGRAM for character 1
#define posCgramChar2 16	// position in CGRAM for character 2
#define posCgramChar3 24	// position in CGRAM for character 3
#define posCgramChar4 32	// position in CGRAM for character 4
#define posCgramChar5 40	// position in CGRAM for character 5
#define posCgramChar6 48	// position in CGRAM for character 6
#define posCgramChar7 56	// position in CGRAM for character 7

#define LCD_SetWriteDdramPosition(bAddr) LCD_WriteCommand(cmdLcdSetDdramPos | bAddr);

void LCD_Init();
void LCD_InitSequence(unsigned char bDisplaySetOptions);
void LCD_WriteStringAtPos(char *szLn, unsigned char idxLine, unsigned char bAdr);
void LCD_DisplaySet(unsigned char bDisplaySetOptions);
void LCD_DisplayClear();
void LCD_DisplayShift(unsigned char fRight);
void LCD_CursorShift(unsigned char fRight);
void LCD_ReturnHome();
void LCD_WriteBytesAtPosCgram(unsigned char *pBytes, unsigned char len, unsigned char bAdr);

// private
unsigned char LCD_ReadByte();
unsigned char LCD_ReadStatus();
void LCD_WriteByte(unsigned char bData);
void LCD_SetWriteCgramPosition(unsigned char bAdr);
void LCD_WriteCommand(unsigned char bCmd);
void LCD_WriteDataByte(unsigned char bData);
void LCD_ConfigurePins();

#endif /* _LCD_H */

/* *****************************************************************************
 End of File
 */
