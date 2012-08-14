/************************************************************************/
/*																		*/
/*	LCDP.cpp		--	Definition for LCDP library 	    			*/
/*																		*/
/************************************************************************/
/*	Author:		Cristian Fatu											*/
/*	Copyright 2011, Digilent Inc.										*/
/************************************************************************/
/*  File Description:													*/
/*		This file defines functions for LCDP							*/
/*																		*/
/************************************************************************/
/*  Revision History:													*/
/*																		*/
/*	12/10/2011(CristianF): created										*/
/*																		*/
/************************************************************************/


/* ------------------------------------------------------------ */
/*				Include File Definitions						*/
/* ------------------------------------------------------------ */
#include <stdio.h>
#include <string.h>
#include "LCDP.h"
#include "WProgram.h"
/* ------------------------------------------------------------ */
/*				Procedure Definitions							*/
/* ------------------------------------------------------------ */


/* ------------------------------------------------------------ */
/*        LCDP::LCDP
**
**        Synopsis:
**				
**        Parameters:
**
**
**
**        Return Values:
**                void 
**
**        Errors:
**
**
**        Description:
**			Class constructor. Performs variables initialization tasks
**
**
*/
LCDP::LCDP()
{
	m_bDisplayMode = 0;
}

/* ------------------------------------------------------------ */
/*        LCDP::begin
**
**        Synopsis:
**				
**        Parameters:
**				uint8_t bkl	- the pin corresponding to Backlight pin
**				uint8_t rs	- the pin corresponding to RS pin
**				uint8_t rw	- the pin corresponding to RW pin
**				uint8_t enable	- the pin corresponding to Enable pin
**				uint8_t d0	- the pin corresponding to D0 pin
**				uint8_t d1	- the pin corresponding to D1 pin
**				uint8_t d2	- the pin corresponding to D2 pin
**				uint8_t d3	- the pin corresponding to D3 pin
**				uint8_t d4	- the pin corresponding to D4 pin
**				uint8_t d5	- the pin corresponding to D5 pin
**				uint8_t d6	- the pin corresponding to D6 pin
**				uint8_t d7	- the pin corresponding to D7 pin
**
**        Return Values:
**
**        Errors:
**
**
**        Description:
**				This function saves the pins corresponding to the CLP and performs the required LCDP initialization tasks.
**
**
*/
void LCDP::begin(uint8_t bkl, uint8_t rs, uint8_t rw, uint8_t enable, uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3, uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7)
{
	m_bkl_pin = bkl;
	m_rs_pin = rs;
	m_rw_pin = rw;
	m_enable_pin = enable;
	if(d0 == 0 && d1 == 1 && d2 == 2 && d3 == 3 && d4 == 4 && d5 == 5 && d6 == 6 && d7 == 7)
	{
		m_fDirectPortJA = true;
	}
	else
	{
		m_data_pins[0] = d0;
		m_data_pins[1] = d1;
		m_data_pins[2] = d2;
		m_data_pins[3] = d3; 
		m_data_pins[4] = d4;
		m_data_pins[5] = d5;
		m_data_pins[6] = d6;
		m_data_pins[7] = d7; 		
	}
	// declare command pins as output
	pinMode(m_bkl_pin, OUTPUT);
	pinMode(m_rs_pin, OUTPUT);
	pinMode(m_rw_pin, OUTPUT);
	pinMode(m_enable_pin, OUTPUT);	
	
	// perform initialization sequence, according to datasheet

	//	wait 20 ms
	delayMicroseconds(20*1000);
	// Set function
	WriteCommand(LCDP_CMD_LcdFcnInit);
	// Wait 37 us
	delayMicroseconds(37);

	// display on, no cursor, no blinking
	SetDisplay(true);

	// Wait 37 us
	delayMicroseconds(37);
	// Display Clear
	DisplayClear();
	// Wait 1.52 ms
	delayMicroseconds(1520);
}


/* ------------------------------------------------------------ */
/*        ReadByte
**
**        Synopsis:
**
**        Parameters:
**
**
**        Return Values:
**                uint8_t - the byte that was read
**
**        Errors:
**
**
**        Description:
**				The function implements a CLP read sequence. 
**				The function is used to read data (RS set before calling this function) or to read status (RS cleared before calling this function).
**				The function implements two approaches of handling data pins.
**
*/
uint8_t LCDP::ReadByte()
{
	uint8_t bData = 0;
	// Set RW
	digitalWrite(m_rw_pin, HIGH);
	// Set Enable
	digitalWrite(m_enable_pin, HIGH);

	if(m_fDirectPortJA)
	{
		// configure data pins as inputs
		trisLCDPDatSet = mskLCDPDat07;

		// wait some time before port operation
		delayMicroseconds(3);		
		// read data
		// access data as contiguous 8 bits
		bData = (uint8_t)(prtLCDPDat & mskLCDPDat07);						
		}
	else
	{
		for (int i = 0; i < 8; i++)
		{
			pinMode(m_data_pins[i], INPUT);
			bData += ((uint8_t)digitalRead(m_data_pins[i]) << i);
		}	
	}
	// Clear Enable
	digitalWrite(m_enable_pin, LOW);
	// Clear RW
	digitalWrite(m_rw_pin, LOW);
	return bData;
}
/* ------------------------------------------------------------ */
/*        WriteByte
**
**        Synopsis:
**
**        Parameters:
**				uint8_t bData - data to be written to display
**
**
**        Return Values:
**                void 
**
**        Errors:
**
**
**        Description:
**				The function implements a CLP write sequence. 
**				The function is used to write data (RS set before calling this function) or to write commands (RS cleared before calling this function).
**				When writing data it writes in DDRAM or CGRAM according to the last set write position.
*/
void LCDP::WriteByte(uint8_t bData)
{
	// Clear RW
	digitalWrite(m_rw_pin, LOW);
	
	// Set Enable
	digitalWrite(m_enable_pin, HIGH);		
	if(m_fDirectPortJA)
	{
		// configure data pins as outputs
		trisLCDPDatClr = mskLCDPDat07;
		
		// wait some time before port operation
		delayMicroseconds(3);

		// write byte to the port
		// access data as contiguous 8 bits
		prtLCDPDat = prtLCDPDat & ~(mskLCDPDat07) | bData;
	}
	else
	{
		for (int i = 0; i < 8; i++)
		{
			pinMode(m_data_pins[i], OUTPUT);
			digitalWrite(m_data_pins[i], (bData >> i) & 0x01);
		}	
	}
	// Clear Enable
	digitalWrite(m_enable_pin, LOW);
	
	// Set RW
	digitalWrite(m_rw_pin, HIGH);		
}
/* ------------------------------------------------------------ */
/*        ReadStatus
**
**        Synopsis:
**
**        Parameters:
**
**
**        Return Values:
**                uint8_t - the byte that was read.
**
**        Errors:
**
**
**        Description:
**				Reads the status of the CLP. It clears the RS and calls ReadByte() function.
**
**
*/
uint8_t LCDP::ReadStatus()
{
	uint8_t bStatus;
	// clear RS
	digitalWrite(m_rs_pin, LOW);
	
	// read status byte
	bStatus = ReadByte();
	return bStatus;
}
/* ------------------------------------------------------------ */
/*        WaitUntilNotBusy
**
**        Synopsis:
**				WaitUntilNotBusy()
**        Parameters:
**		
**
**        Return Values:
**                void 
**
**        Errors:
**
**
**        Description:
**				Waits until the status of the CLP is not busy. This function relies on ReadStatus().
**
**
*/
void LCDP::WaitUntilNotBusy()
{
	uint8_t bStatus;
	bStatus = ReadStatus();
	while (bStatus & LCDP_MSK_BStatus)
	{
		delayMicroseconds(10);
		bStatus = ReadStatus();
	}
}
/* ------------------------------------------------------------ */
/*        WriteCommand
**
**        Synopsis:
**				WriteCommand(cmdLcdClear);
**        Parameters:
**				uint8_t bCmd	- the command code byte
**
**
**        Return Values:
**                void 
**
**        Errors:
**
**
**        Description:
**				Writes the specified byte as command. When the device is ready it clears the RS and writes byte.
**
**
*/
void LCDP::WriteCommand(uint8_t bCmd)
{
	// wait until LCD is not busy
	WaitUntilNotBusy();

	// Clear RS
	digitalWrite(m_rs_pin, LOW);

	// Write command byte
	WriteByte(bCmd);
}


/* ------------------------------------------------------------ */
/*        WriteDataByte
**
**        Synopsis:
**				WriteDataByte(pBytes[idx]);
**        Parameters:
**				uint8_t bData		- the data byte
**
**
**        Return Values:
**                void 
**
**        Errors:
**
**
**        Description:
**				Writes the specified byte as data. When the device is ready it sets the RS and writes byte.
**
**
*/
void LCDP::WriteDataByte(uint8_t bData)
{
	// wait until LCD is not busy
	WaitUntilNotBusy();

	// Set RS
	digitalWrite(m_rs_pin, HIGH);

	// Write command byte
	WriteByte(bData);
}

/* ------------------------------------------------------------ */
/*        SetWriteCgramPosition
**
**        Synopsis:
**				SetWriteCgramPosition(bAdr);
**        Parameters:
**				uint8_t bAdr	- the write location. The position in CGRAM where the next data writes will put bytes.
**
**
**
**        Errors:
**
**
**        Description:
**				Sets the CGRAM write position. This is the location where the next data write will be performed.
**				Be aware that writing to a location auto-increments the write location.
**
*/
void LCDP::SetWriteCgramPosition(uint8_t bAdr)
{
	uint8_t bCmd = LCDP_CMD_LcdSetCgramPos | bAdr;
	WriteCommand(bCmd);
}

/* ------------------------------------------------------------ */
/*        SetWriteDdramPosition
**
**        Synopsis:
**				SetWriteDdramPosition(bAddrOffset);
**        Parameters:
**				uint8_t bAdr - the write location. The position in DDRAM where the next data writes will put bytes.
**					0x00-0x27 refer to the first row
**					0x40-0x67 refer to the second row
**
**
**        Errors:
**
**
**        Description:
**				Sets the DDRAM write position. This is the location where the next data write will be performed.
**				Be aware that writing to a location auto-increments the write location.
**
*/
void LCDP::SetWriteDdramPosition(uint8_t bAdr)
{
	uint8_t bCmd = LCDP_CMD_LcdSetDdramPos | bAdr;
	WriteCommand(bCmd);
}


/* ------------------------------------------------------------ */
/*        DisplayClear
**
**        Synopsis:
**				DisplayClear();
**        Parameters:
**
**
**        Return Values:
**                void 
**
**        Errors:
**
**
**        Description:
**				Clears the display and returns the cursor home (upper left corner).
**
**
*/
void LCDP::DisplayClear()
{
	WriteCommand(LCDP_CMD_LcdClear);
}
/* ------------------------------------------------------------ */
/*        ReturnHome
**
**        Synopsis:
**				ReturnHome();
**        Parameters:
**
**
**        Return Values:
**                void 
**
**        Errors:
**
**
**        Description:
**				Returns the cursor home (upper left corner).
**
**
*/
void LCDP::ReturnHome()
{
	WriteCommand(LCDP_CMD_LcdRetHome);
}

/* ------------------------------------------------------------ */
/*        SetDisplay
**
**        Synopsis:
**				SetDisplay(true);
**        Parameters:
**				bool fDisplayOn - Display option
**						- true in order to set the display ON	
**						- false in order to set the display OFF
**
**
**        Return Values:
**
**        Errors:
**
**
**        Description:
**				Sets the display option. If true, display is on, if false, the display is off.
**
*/
void LCDP::SetDisplay(bool fDisplayOn)
{
	if(fDisplayOn)
	{
		m_bDisplayMode |= LCDP_OPT_DisplayOn;
	}
	else
	{
		m_bDisplayMode &= ~LCDP_OPT_DisplayOn;		
	}
	WriteCommand(LCDP_CMD_LcdCtlInit | m_bDisplayMode);
}

/* ------------------------------------------------------------ */
/*        SetCursor
**
**        Synopsis:
**				SetCursor(true);
**        Parameters:
**				bool fCursorOn - Cursor option
**						- true in order to set the Cursor ON	
**						- false in order to set the Cursor OFF
**
**
**        Return Values:
**
**        Errors:
**
**
**        Description:
**				Sets the cursor option. If true, Cursor is on, if false, the Cursor is off.
**
*/
void LCDP::SetCursor(bool fCursorOn)
{
	if(fCursorOn)
	{
		m_bDisplayMode |= LCDP_OPT_CursorOn;
	}
	else
	{
		m_bDisplayMode &= ~LCDP_OPT_CursorOn;		
	}
		
	WriteCommand(LCDP_CMD_LcdCtlInit | m_bDisplayMode);
}

/* ------------------------------------------------------------ */
/*        SetBlink
**
**        Synopsis:
**				SetBlink(true);
**        Parameters:
**				bool fBlinkOn - Blink option
**						- true in order to set the Blink ON	
**						- false in order to set the Blink OFF
**
**
**        Return Values:
**
**        Errors:
**
**
**        Description:
**				Sets the Blink option. If true, Blink is on, if false, the Blink is off.
**
*/
void LCDP::SetBlink(bool fBlinkOn)
{
	if(fBlinkOn)
	{
		m_bDisplayMode |= LCDP_OPT_BlinkOn;
	}
	else
	{
		m_bDisplayMode &= ~LCDP_OPT_BlinkOn;		
	}
		
	WriteCommand(LCDP_CMD_LcdCtlInit | m_bDisplayMode);
}

/* ------------------------------------------------------------ */
/*        SetBacklight
**
**        Synopsis:
**				SetBacklight(fBackLight);
**        Parameters:
**				bool fBl - Backlight option
**						- true in order to set the backlight ON	
**						- false in order to set the backlight OFF
**
**
**        Return Values:
**                void 
**
**        Errors:
**
**
**        Description:
**				This function turns the backlight on or off, according to the user's selection.
**				Note that there are CLP Pmods that do not have backlight functionality. Using this function for this type of modules will have no effect.
**
**
*/
void LCDP::SetBacklight(bool fBacklightOn)
{
	digitalWrite(m_bkl_pin, fBacklightOn ? HIGH: LOW);
}

/* ------------------------------------------------------------ */
/*        SetPos
**
**        Synopsis:
**				SetPos(0, 3);
**        Parameters:
**				uint8_t idxLine	- the line where the position will be set
**				uint8_t idxCol	- the column where the position will be set
**
**
**        Return Value:
**                uint8_t 
**					- LCDP_ERR_SUCCESS (0) 	- The action completed successfully 
**					- a combination (ORed) of the following errors:
**						- LCDP_ERR_ARG_ROW_RANGE (0x80) - The row index is not valid
**						- LCDP_ERR_ARG_COL_RANGE (0x40) - The column index is not valid 
**						
**
**        Errors:
**				See Return Value
**
**        Description:
**				This function sets the corresponding LCD position. This is used for write position and cursor position.
**				If position set is invalid (outside the display), errors are returned.
**
**
*/
uint8_t LCDP::SetPos(uint8_t idxLine, uint8_t idxCol)
{
	uint8_t bResult = LCDP_ERR_SUCCESS;

	if (idxLine < 0 || idxLine >= LCDP_NO_ROWS)
	{
		bResult |= LCDP_ERR_ARG_ROW_RANGE;
	}
	if (idxCol < 0 || idxCol >= LCDP_NO_COLS)
	{
		bResult |= LCDP_ERR_ARG_COL_RANGE;
	}
	if (bResult == LCDP_ERR_SUCCESS)
	{
		// Set write position
		uint8_t bAddrOffset = (idxLine == 0 ? 0: 0x40) + idxCol;
		SetWriteDdramPosition(bAddrOffset);
	}	
	return bResult;
	
}
/* ------------------------------------------------------------ */
/*        WriteStringAtPos
**
**        Synopsis:
**				WriteStringAtPos(szInfo1, 0, 0);
**        Parameters:
**				char *szLn	- string to be written to LCD
**				uint8_t idxLine	- the line where the string will be displayed
**				uint8_t idxCol	- the column line where the string will be displayed
**
**
**        Return Value:
**                uint8_t 
**					- LCDP_ERR_SUCCESS (0) 	- The action completed successfully 
**					- a combination (ORed) of the following errors:
**						- LCDP_ERR_ARG_ROW_RANGE (0x80) - The row index is not valid
**						- LCDP_ERR_ARG_COL_RANGE (0x40) - The column index is not valid 
**						
**
**        Errors:
**				See Return Value
**
**        Description:
**				The function writes the specified string at the specified position (line and column). 
**				It sets the corresponding write position and then writes data bytes when the device is ready.
**				Strings that span over the end of line are trimmed so they fit in the line.
**				If position is invalid (outside the display), errors are returned.
**
*/
uint8_t LCDP::WriteStringAtPos(char *szLn, uint8_t idxLine, uint8_t idxCol)
{
	uint8_t bResult = SetPos(idxLine, idxCol);
	if (bResult == LCDP_ERR_SUCCESS)
	{
		// Strings that span over the end of line are trimmed so they fit in the line.
		int len = strlen(szLn);
		if(len + idxCol > LCDP_NO_COLS)
		{
			len = LCDP_NO_COLS - idxCol;
			szLn[len] = 0; // crop the string at this position  
		}

		uint8_t bIdx = 0;
		while(bIdx < len)
		{
			WriteDataByte(szLn[bIdx]);
			bIdx++;
		}
	}	
	return bResult;
	
}



/* ------------------------------------------------------------ */
/*        DisplayShift
**
**        Synopsis:
**				DisplayShift(fBtn1Process);
**        Parameters:
**				bool fRight - parameter indicating the direction of the display shift
**						- true in order to shift the display right
**						- false in order to shift the display left
**
**        Return Values:
**                void 
**
**        Errors:
**
**
**        Description:
**				This function shifts the display one position right or left, depending on the fRight parameter.
**
**
**
*/
void LCDP::DisplayShift(bool fRight)
{
	uint8_t bCmd = LCDP_CMD_LcdDisplayShift | (fRight != false ? LCDP_MSK_ShiftRL: 0);
	WriteCommand(bCmd);
}
/* ------------------------------------------------------------ */
/*        CursorShift
**
**        Synopsis:
**				CursorShift(fBtn1Process);
**        Parameters:
**				bool fRight - parameter indicating the direction of the cursor shift
**						- true in order to shift the cursor right
**						- false in order to shift the cursor left
**
**        Return Values:
**                void 
**
**        Errors:
**
**
**        Description:
**				This function shifts the cursor one position right or left, depending on the fRight parameter.
**
**
*/
void LCDP::CursorShift(bool fRight)
{
	uint8_t bCmd = LCDP_CMD_LcdCursorShift | (fRight != false ? LCDP_MSK_ShiftRL: 0);
	WriteCommand(bCmd);
}

/* ------------------------------------------------------------ */
/*        DefineUserChar
**
**        Synopsis:
**				MyLCDP.DefineUserChar(defChar2, 2);
**        Parameters:
**				uint8_t *pBytes	- pointer to the string that contains the 8 bytes definition of the character. 
**				uint8_t bCharNo	- the position of the user character to be saved in the memory 
**
**
**        Return Values:
**                uint8_t 
**					- LCDP_ERR_SUCCESS (0) 				- The action completed successfully 
**					- LCDP_ERR_UCHAR_POSITION_INVALID	(0x20) - The user character position is not within 0 - 7 
**
**        Errors:
**
**
**        Description:
**				This function writes the specified number of bytes to CGRAM starting at the specified position.
**				It sets the corresponding write position and then writes data bytes when the device is ready.
**				If the user character position is not within 0 - 7 range, error is returned. 
**
*/
uint8_t LCDP::DefineUserChar(uint8_t *pBytes, uint8_t bCharNo)
{
	uint8_t bResult = LCDP_ERR_SUCCESS;

	if (bCharNo >=0 || bCharNo < LCDP_NO_UCHARS)
	{	
		uint8_t bAdr = bCharNo << 3; // multiply by 8
		// Set write position to CGRAM
		SetWriteCgramPosition(bAdr);
		uint8_t len = 8;
		// Write the string of bytes that define the character to CGRAM
		uint8_t bIdx = 0;
		while(bIdx < len)
		{
			WriteDataByte(pBytes[bIdx]);
			bIdx++;
		}
		bResult = LCDP_ERR_SUCCESS;
	}
	else
	{
		bResult = LCDP_ERR_UCHAR_POSITION_INVALID;
	}	
	return bResult;	
}

/* ------------------------------------------------------------ */
/*        WriteUserCharsAtPos
**
**        Synopsis:
**				WriteUserCharsAtPos(szInfo1, 0, 0);
**        Parameters:
**				rgCharPos - an array containing the index (position) of the user characters to be displayed 
**				bNoChars - an array containing the index (position) of the user characters to be displayed
**				uint8_t idxLine	- line where the string will be displayed
**				uint8_t idxCol	- the starting position of the string within the line

**
**        Return Value:
**                uint8_t 
**					- LCDP_ERR_SUCCESS (0) 	- The action completed successfully 
**					- a combination (ORed) of the following errors:
**						- LCDP_ERR_ARG_ROW_RANGE (0x80) - The row index is not valid
**						- LCDP_ERR_ARG_COL_RANGE (0x40) - The column index is not valid 
**						- LCDP_ERR_UCHAR_POSITION_INVALID	(0x20) - The user character position is not within the accepted range (0 – 7)
**
**	Errors:
**		none
**
**	Description:
**		This function displays one or more user defined characters at the specified positions on the LCD. 
**		If the position set or the user character position is not correct, errors are returned.
**		
**
**
-----------------------------------------------------------------------*/
uint8_t LCDP::WriteUserCharsAtPos(uint8_t* rgCharPos, uint8_t bNoChars, uint8_t idxLine, uint8_t idxCol) 
{
	uint8_t bResult = SetPos(idxLine, idxCol);
	if (bResult == LCDP_ERR_SUCCESS)
	{
		// validate the user character positions to be between 0 and 7
		uint8_t bIdx = 0;
		while(bIdx < bNoChars)
		{
			if (rgCharPos[bIdx] < 0 || rgCharPos[bIdx] >= LCDP_NO_UCHARS)
			{
				bResult = LCDP_ERR_UCHAR_POSITION_INVALID;
				bIdx = bNoChars; // force out, no need to continue
			}			
			bIdx++;
		}
		if (bResult == LCDP_ERR_SUCCESS)
		{
			//set the write position of the cursor to the wanted line/column for displaying custom chars
			uint8_t bAddrOffset = (idxLine == 0 ? 0: 0x40) + idxCol;
			SetWriteDdramPosition(bAddrOffset);
			//send the position of the user character to be displayed
			uint8_t bIdx = 0;
			while(bIdx < bNoChars)
			{		
				WriteDataByte(rgCharPos[bIdx]);
				bIdx++;
			}
		}
	}
	return bResult;
}
