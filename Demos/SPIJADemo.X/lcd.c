/* ************************************************************************** */
/** Descriptive File Name

  @Company
    Digilent

  @File Name
    lcd.c

  @Description
        This file groups the functions that implement the LCD library.
        The library implements control of LCD device. 
        It is accessed in a "parallel like" approach. 
 *      Library provides functions for simple commands, displaying characters, handling user characters.
        Include the file together with config.h, utils.c and utils.h in the project when this library is needed.	
 
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
#include <string.h>
#include "config.h"
#include "lcd.h"
/* ************************************************************************** */

/* ------------------------------------------------------------ */
/***	LCD_Init
**
**	Parameters:
**		
**
**	Return Value:
**		
**
**	Description:
**		This function initializes the hardware involved in the LCD module: 
**      The following digital pins are configured as digital outputs: LCD_DISP_RS, LCD_DISP_RW, LCD_DISP_EN
**      The following digital pins are configured as digital inputs: LCD_DISP_RS.
**      After initialization, the LCD is turned on.
**          
*/
void LCD_Init()
{
    LCD_ConfigurePins();
    LCD_InitSequence(displaySetOptionDisplayOn);
}

/* ------------------------------------------------------------ */
/***	LCD_ConfigurePins
**
**	Parameters:
**		
**
**	Return Value:
**		
**
**	Description:
**		This function configures the digital pins involved in the SPIFLASH module: 
**      The following digital pins are configured as digital outputs: LCD_DISP_RS, LCD_DISP_RW, LCD_DISP_EN
**      The following digital pins are configured as digital inputs: LCD_DISP_RS.
**      The function uses pin related definitions from config.h file.
**      
**          
*/
void LCD_ConfigurePins()
{
    // set control pins as digital outputs.
    tris_LCD_DISP_RS = 0;
    tris_LCD_DISP_RW = 0;
    tris_LCD_DISP_EN = 0;
    
    // disable analog (set pins as digital))
    ansel_LCD_DISP_RS = 0;
    
    // default (IO) function for remapable pins
    rp_LCD_DISP_RS = 0;
    rp_LCD_DISP_RW = 0;
    rp_LCD_DISP_EN = 0;
    
    // data pins will be set as output or input in the corresponding write and read functions
    
    // make data pins digital (disable analog)
    ansel_LCD_DB2 = 0;
    ansel_LCD_DB4 = 0;
    ansel_LCD_DB5 = 0;
    ansel_LCD_DB6 = 0;
    ansel_LCD_DB7 = 0;
}

/* ------------------------------------------------------------ */
/***	LCD_WriteByte
**
**	Parameters:
**		unsigned char bData - the data to be written to LCD
**
**	Return Value:
**		
**
**	Description:
**		This function writes a byte tot he LCD. 
**      It implements the parallel write using LCD_DISP_RS, LCD_DISP_RW, LCD_DISP_EN, 
**      LCD_DISP_RS pins, and data pins. 
**      For a better performance, the data pins are accessed using a pointer to 
**      the register byte where they are allocated.
**      The function uses pin related definitions from config.h file.
**      
**          
*/
void LCD_WriteByte(unsigned char bData)
{
    DelayAprox10Us(5);  
	// Configure IO Port data pins as output.
   tris_LCD_DATA &= ~msk_LCD_DATA;
    DelayAprox10Us(5);  
	// clear RW
	lat_LCD_DISP_RW = 0;

// access data as contiguous 8 bits, unsing pointer to the LSB byte of LATE register
    unsigned char *pLCDData = (unsigned char *)(0xBF886430);
    *pLCDData = bData;

    DelayAprox10Us(10);   

	// Set E
	lat_LCD_DISP_EN = 1;    

    DelayAprox10Us(5);
	// Clear E
	lat_LCD_DISP_EN = 0;

    DelayAprox10Us(5);
	// Set RW
	lat_LCD_DISP_RW = 1;
}

/* ------------------------------------------------------------ */
/***	LCD_ReadByte
**
**	Parameters:
**		
**
**	Return Value:
**		unsigned char - the data read from LCD, over the parallel interface
**
**	Description:
**		This function reads a byte from the LCD. 
**      It implements the parallel write using LCD_DISP_RS, LCD_DISP_RW, LCD_DISP_EN, 
**      LCD_DISP_RS pins, and data pins. 
**      The function uses pin related definitions from config.h file.
**      
**          
*/
unsigned char LCD_ReadByte()
{
    unsigned char bData;
	// Configure IO Port data pins as input.
    tris_LCD_DATA |= msk_LCD_DATA;
	// Set RW
	lat_LCD_DISP_RW = 1;

	// set RW
	lat_LCD_DISP_RW = 1;    

    
	// Set E
	lat_LCD_DISP_EN = 1;

    DelayAprox10Us(50);   

    // Clear E
	lat_LCD_DISP_EN = 0;
  	bData = (unsigned char)(prt_LCD_DATA & (unsigned int)msk_LCD_DATA);



	return bData;
}

/* ------------------------------------------------------------ */
/***	LCD_ReadStatus
**
**	Parameters:
**		
**
**	Return Value:
**		unsigned char - the status byte that was read.
**
**	Description:
**		Reads the status of the LCD.  
**      It clears the RS and calls LCD_ReadByte() function. 
**      
**          
*/
unsigned char LCD_ReadStatus()
{
	// Clear RS
	lat_LCD_DISP_RS = 0;
    
	unsigned char bStatus = LCD_ReadByte();
	return bStatus;
}

/* ------------------------------------------------------------ */
/***	LCD_WriteCommand
**
**	Parameters:
**		unsigned char bCmd -  the command code byte to be written to LCD
**
**	Return Value:
**		
**
**	Description:
**		Writes the specified byte as command. 
**      It clears the RS and writes the byte to LCD. 
**      
**          
*/
void LCD_WriteCommand(unsigned char bCmd)
{ 
	// 1. Clear RS
	lat_LCD_DISP_RS = 0;

	// 2. Write command byte
	LCD_WriteByte(bCmd);
}

/* ------------------------------------------------------------ */
/***	LCD_WriteDataByte
**
**	Parameters:
**		unsigned char bData -  the data byte to be written to LCD
**
**	Return Value:
**		
**
**	Description:
**		Writes the specified byte as data. 
**      It sets the RS and writes the byte to LCD. 
**      
**          
*/
void LCD_WriteDataByte(unsigned char bData)
{
	// 	1. Set RS 
	lat_LCD_DISP_RS = 1;

	// 2. Write data byte
	LCD_WriteByte(bData);
}


/* ------------------------------------------------------------ */
/***	LCD_InitSequence
**
**  Synopsis:
**      LCD_InitSequence(displaySetOptionDisplayOn);
**
**	Parameters:
**		unsigned char bDisplaySetOptions -  display options
**					Possible options (to be OR-ed)
**						displaySetOptionDisplayOn - display ON
**						displaySetOptionCursorOn - cursor ON
**						displaySetBlinkOn - cursor blink ON
**
**	Return Value:
**		
**
**	Description:
**		This function performs the initializing (startup) sequence. 
**      The LCD is initialized according to the parameter bDisplaySetOptions. 
**      
**          
*/
void LCD_InitSequence(unsigned char bDisplaySetOptions)
{
	//	wait 40 ms

	DelayAprox10Us(40000);
	// Function Set
	LCD_WriteCommand(cmdLcdFcnInit);
	// Wait ~100 us
	DelayAprox10Us(10);
	// Function Set
	LCD_WriteCommand(cmdLcdFcnInit);
	// Wait ~100 us
	DelayAprox10Us(10);	// Display Set
	LCD_DisplaySet(bDisplaySetOptions);
	// Wait ~100 us
	DelayAprox10Us(10);
	// Display Clear
	LCD_DisplayClear();
	// Wait 1.52 ms
	DelayAprox10Us(160);
    // Entry mode set
	LCD_WriteCommand(cmdLcdEntryMode);
    	// Wait 1.52 ms
	DelayAprox10Us(160);
}

/* ------------------------------------------------------------ */
/***	LCD_DisplaySet
**
**  Synopsis:
**				LCD_DisplaySet(displaySetOptionDisplayOn | displaySetOptionCursorOn);
**
**	Parameters:
**		unsigned char bDisplaySetOptions -  display options
**					Possible options (to be OR-ed)
**						displaySetOptionDisplayOn - display ON
**						displaySetOptionCursorOn - cursor ON
**						displaySetBlinkOn - cursor blink ON
**
**	Return Value:
**		
**
**	Description:
**      The LCD is initialized according to the parameter bDisplaySetOptions. 
**      
**          
*/
void LCD_DisplaySet(unsigned char bDisplaySetOptions)
{
	LCD_WriteCommand(cmdLcdCtlInit | bDisplaySetOptions);
}

/* ------------------------------------------------------------ */
/***	LCD_DisplaySet
**
**  Synopsis:
**      LCD_DisplayClear();
**
**	Parameters:
**
**	Return Value:
**		
**	Description:
**      Clears the display and returns the cursor home (upper left corner). 
**      
**          
*/
void LCD_DisplayClear()
{
	LCD_WriteCommand(cmdLcdClear);
}

/* ------------------------------------------------------------ */
/***	LCD_ReturnHome
**
**  Synopsis:
**      LCD_ReturnHome();
**
**	Parameters:
**
**	Return Value:
**		
**	Description:
**      Returns the cursor home (upper left corner). 
**      
**          
*/
void LCD_ReturnHome()
{
	LCD_WriteCommand(cmdLcdRetHome);
}

/* ------------------------------------------------------------ */
/***	LCD_DisplayShift
**
**  Synopsis:
**      LCD_DisplayShift(1);
**
**	Parameters:
**		unsigned char fRight
**				- 1 in order to shift right
**				- 0 in order to shift left
**
**	Return Value:
**		
**	Description:
**     Shifts the display one position right or left, depending on the fRight parameter.
**      
**          
*/
void LCD_DisplayShift(unsigned char fRight)
{
	unsigned char bCmd = cmdLcdDisplayShift | (fRight ? mskShiftRL: 0);
	LCD_WriteCommand(bCmd);
}

/* ------------------------------------------------------------ */
/***	LCD_CursorShift
**
**  Synopsis:
**      LCD_CursorShift(1);
**
**	Parameters:
**		unsigned char fRight
**				- 1 in order to shift right
**				- 0 in order to shift left
**
**	Return Value:
**		
**	Description:
**     Shifts the cursor one position right or left, depending on the fRight parameter.
**      
**          
*/
void LCD_CursorShift(unsigned char fRight)
{
	unsigned char bCmd = cmdLcdCursorShift | (fRight ? mskShiftRL: 0);
	LCD_WriteCommand(bCmd);
}

/* ------------------------------------------------------------ */
/***	LCD_WriteStringAtPos
**
**  Synopsis:
**      LCD_WriteStringAtPos("Demo", 0, 0);
**
**	Parameters:
**      char *szLn	- string to be written to LCD
**		int idxLine	- line where the string will be displayed
**          0 - first line of LCD
**          1 - second line of LCD
**		unsigned char idxPos - the starting position of the string within the line
**          0 - first position from left
**
**	Return Value:
**		
**	Description:
**		Displays the specified string at the specified position on the specified line. 
**		It sets the corresponding write position and then writes data bytes as the device is ready.
**      Strings longer than 0x27 are trimmed.
**      
**          
*/
void LCD_WriteStringAtPos(char *szLn, unsigned char idxLine, unsigned char idxPos)
{
	// crop string to 0x27 chars
	int len = strlen(szLn);
	if(len > 0x27)
	{
        szLn[0x27] = 0; // trim the string so it contains 40 characters 
		len = 0x27;
	}

	// Set write position
	unsigned char bAddrOffset = (idxLine == 0 ? 0: 0x40) + idxPos;
	LCD_SetWriteDdramPosition(bAddrOffset);

	unsigned char bIdx = 0;
	while(bIdx < len)
	{
		LCD_WriteDataByte(szLn[bIdx]);
		bIdx++;
	}
}

/* ------------------------------------------------------------ */
/***	LCD_SetWriteCgramPosition
**
**  Synopsis:
**      LCD_SetWriteCgramPosition(bAdr);
**
**	Parameters:
**      unsigned char bAdr	- the write location. The position in CGRAM where the next data writes will put bytes.
**
**	Return Value:
**		
**	Description:
**		Sets the DDRAM write position. This is the location where the next data write will be performed.
**		Writing to a location auto-increments the write location.
**      
**          
*/
void LCD_SetWriteCgramPosition(unsigned char bAdr)
{
	unsigned char bCmd = cmdLcdSetCgramPos | bAdr;
	LCD_WriteCommand(bCmd);
}

/* ------------------------------------------------------------ */
/***	LCD_WriteBytesAtPosCgram
**
**  Synopsis:
**      LCD_WriteBytesAtPosCgram(userDefArrow, 8, posCgramChar0);
**
**	Parameters:
**		unsigned char *pBytes	- pointer to the string of bytes
**		unsigned char len		- the number of bytes to be written
**		unsigned char bAdr		- the position in CGRAM where bytes will be written
**
**	Return Value:
**		
**	Description:
**				Writes the specified number of bytes to CGRAM starting at the specified position.
**				It sets the corresponding write position and then writes data bytes as the device is ready.
**      
**          
*/
void LCD_WriteBytesAtPosCgram(unsigned char *pBytes, unsigned char len, unsigned char bAdr)
{
	// Set write position
	LCD_SetWriteCgramPosition(bAdr);

	// Write the string of bytes that define the character to CGRAM
	unsigned char idx = 0;
	while(idx < len)
	{
		LCD_WriteDataByte(pBytes[idx]);
		idx++;
	}
}

/* *****************************************************************************
 End of File
 */

