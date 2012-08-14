/************************************************************************/
/*																		*/
/*	ClpLib.c -- Common Utility Procedures for the PIC32MX                 */
/*																		*/
/************************************************************************/
/*	Author: Cristian Fatu												*/
/*	Copyright 2011, Digilent Inc.										*/
/************************************************************************/
/*  Module Description: 												*/
/*																		*/
/*	This module contains implementations for common functions needed to */
/*	manage a CLP PMOD.													*/
/*	This file must be included in MPLAB project whenever CLP functions 	*/
/*  are needed. No modifications in this file are required.				*/
/*  This file includes ClpLib.h (CLP definitions) and					*/
/*  ClpLib_config.h (CLP pins definitions)								*/
/************************************************************************/
/*  Revision History:						                			*/
/*											                        	*/
/*	10/05/2011 (CristianF): created			                			*/
/*											                        	*/
/************************************************************************/

/* ------------------------------------------------------------ */
/*				Include File Definitions						*/
/* ------------------------------------------------------------ */

#include <plib.h>
#include "stdtypes.h"
#include "ClpLib_config.h"
#include "ClpLib.h"
#include "util.h"



/* ------------------------------------------------------------ */
/*				Local Type Definitions							*/
/* ------------------------------------------------------------ */


/* ------------------------------------------------------------ */
/*				Global Variables								*/
/* ------------------------------------------------------------ */


/* ------------------------------------------------------------ */
/*				Local Variables									*/
/* ------------------------------------------------------------ */

/* ------------------------------------------------------------ */
/*				Forward Declarations							*/
/* ------------------------------------------------------------ */


/* ------------------------------------------------------------ */
/*				Procedure Definitions							*/
/* ------------------------------------------------------------ */


/* ------------------------------------------------------------ */
/*        ClpPinsConfigure
**
**        Synopsis:
**				ClpPinsConfigure();
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
**			Configures pins involved in CLP management as outputs. 
**				Data pins are not configured here. They will be configured as output / input in the corresponding write / read functions.
**
**
*/
void ClpPinsConfigure()
{
	// set RS as output
	trisClpRSClr = ( 1 << bnClpRS );

	// set RW as output
	trisClpRWClr = ( 1 << bnClpRW );

	// set E as output
	trisClpEClr = ( 1 << bnClpE );

	// set BL as output
	trisClpBLClr = ( 1 << bnClpBL );

	// data pins will be set as output or input in the corresponding write and read functions
}





/* ------------------------------------------------------------ */
/*        ClpSetBackLight
**
**        Synopsis:
**				ClpSetBackLight(fBackLight);
**        Parameters:
**				BOOL fBl - Backlight option
**						- fTrue in order to set the backlight ON	
**						- fFalse in order to set the backlight OFF
**
**
**        Return Values:
**                void 
**
**        Errors:
**
**
**        Description:
**				Sets the backlight. 
**					Note that there are CLP PMODS that do not have a backlight functionality.
**
**
*/
void ClpSetBackLight(BOOL fBl)
{
	if(fBl)
	{
		prtClpBLSet = 1 << bnClpBL;
	}
	else
	{
		prtClpBLClr = 1 << bnClpBL;
	}
}





/* ------------------------------------------------------------ */
/*        ClpReadByte
**
**        Synopsis:
**
**        Parameters:
**
**
**        Return Values:
**                BYTE - the byte that was read
**
**        Errors:
**
**
**        Description:
**				Implements a CLP read sequence. 
**				This is a lowest level function that deals with timings of the read cycle.
**				For CEREBOT32MX4 board under the configuration of this project no additional wait time was necessary.
**				The function is used to read data (RS set before calling this function) or to read status (RS cleared before calling this function).
**				The function implements two approaches of handling data pins, according to _CONTIGUOUS_DATA (possible defined in ClpLib_config.h).
**
*/
BYTE ClpReadByte()
{
	// Configure IO Port data pins as input.
#ifdef _CONTIGUOUS_DATA
	trisClpDatSet = mskClpDat07;
#else //_CONTIGUOUS_DATA
	trisClpD0Set = 1 << bnClpD0;
	trisClpD1Set = 1 << bnClpD1;
	trisClpD2Set = 1 << bnClpD2;
	trisClpD3Set = 1 << bnClpD3;
	trisClpD4Set = 1 << bnClpD4;
	trisClpD5Set = 1 << bnClpD5;
	trisClpD6Set = 1 << bnClpD6;
	trisClpD7Set = 1 << bnClpD7;
#endif //_CONTIGUOUS_DATA


	BYTE bData;
	// Set RW
	prtClpRWSet = (1 << bnClpRW);
	// instruction execution time ensures (in this configuration) times conform to read cycle, so no need to wait additional time
//	asm volatile("nop"); // do nothing
	// Set E
	prtClpESet = (1 << bnClpE);

	// instruction execution time ensures (in this configuration) times conform to read cycle, so no need to wait additional time
//	asm volatile("nop"); // do nothing
	// read byte from prtClpDat
#ifdef _CONTIGUOUS_DATA
// access data as contiguous 8 bits
	bData = (BYTE)(prtClpDat & (WORD)mskClpDat07);
#else // not defined _CONTIGUOUS_DATA
	// read each individual pins into corresponding data bits
	bData = 0;
	if(prtClpD0 & ( 1 << bnClpD0 )) 
	{
		bData |= (1 << 0);
	}
	if(prtClpD1 & ( 1 << bnClpD1 )) 
	{
		bData |= (1 << 1);
	}
	if(prtClpD2 & ( 1 << bnClpD2 )) 
	{
		bData |= (1 << 2);
	}
	if(prtClpD3 & ( 1 << bnClpD3 )) 
	{
		bData |= (1 << 3);
	}
	if(prtClpD4 & ( 1 << bnClpD4 )) 
	{
		bData |= (1 << 4);
	}
	if(prtClpD5 & ( 1 << bnClpD5 )) 
	{
		bData |= (1 << 5);
	}
	if(prtClpD6 & ( 1 << bnClpD6 )) 
	{
		bData |= (1 << 6);
	}
	if(prtClpD7 & ( 1 << bnClpD7 )) 
	{
		bData |= (1 << 7);
	}


#endif // _CONTIGUOUS_DATA
	// Clear E
	prtClpEClr = (1 << bnClpE);
	// instruction execution time ensures (in this configuration) times conform to read cycle, so no need to wait additional time
//	asm volatile("nop"); // do nothing
	// clear RW
	prtClpRWClr = (1 << bnClpRW);
	return bData;
}

/* ------------------------------------------------------------ */
/*        ClpWriteByte
**
**        Synopsis:
**
**        Parameters:
**				BYTE bData
**
**
**        Return Values:
**                void 
**
**        Errors:
**
**
**        Description:
**				Implements a CLP write sequence. 
**				This is a lowest level function that deals with timings of the write cycle.
**				For CEREBOT32MX4 board under the configuration of this project no additional wait time was necessary.
**				The function is used to write data (RS set before calling this function) or to write commands (RS cleared before calling this function).
**				The function implements two approaches of handling data pins, according to _CONTIGUOUS_DATA (possible defined in ClpLib_config.h).
**				When writting data it writes in DDRAM or CGRAM according to the last set write position.
*/
void ClpWriteByte(BYTE bData)
{
	// Configure IO Port data pins as output.
#ifdef _CONTIGUOUS_DATA
	trisClpDatClr = mskClpDat07;
#else //_CONTIGUOUS_DATA
	trisClpD0Clr = 1 << bnClpD0;
	trisClpD1Clr = 1 << bnClpD1;
	trisClpD2Clr = 1 << bnClpD2;
	trisClpD3Clr = 1 << bnClpD3;
	trisClpD4Clr = 1 << bnClpD4;
	trisClpD5Clr = 1 << bnClpD5;
	trisClpD6Clr = 1 << bnClpD6;
	trisClpD7Clr = 1 << bnClpD7;
#endif //_CONTIGUOUS_DATA
	// clear RW
	prtClpRWClr = (1 << bnClpRW);

	// Set E
	prtClpESet = (1 << bnClpE);

	// instruction execution time ensures (in this configuration) times conform to write cycle, so no need to wait additional time
//	asm volatile("nop"); // do nothing


	// put byte to prtClpDat
#ifdef _CONTIGUOUS_DATA
// access data as contiguous 8 bits
	prtClpDat = prtClpDat & ~((WORD)mskClpDat07) | bData;
#else // not defined _CONTIGUOUS_DATA
	// write each individual bits into corresponding port pins
	if ( bData & ( 1 << 0 ) ) 
	{
		prtClpD0Set = ( 1 << bnClpD0 );
	}
	else 
	{
		prtClpD0Clr = ( 1 << bnClpD0 );
	}
	if ( bData & ( 1 << 1 ) ) 
	{
		prtClpD1Set = ( 1 << bnClpD1 );
	}
	else 
	{
		prtClpD1Clr = ( 1 << bnClpD1 );
	}
	if ( bData & ( 1 << 2 ) ) 
	{
		prtClpD2Set = ( 1 << bnClpD2 );
	}
	else 
	{
		prtClpD2Clr = ( 1 << bnClpD2 );
	}
	if ( bData & ( 1 << 3 ) ) 
	{
		prtClpD3Set = ( 1 << bnClpD3 );
	}
	else 
	{
		prtClpD3Clr = ( 1 << bnClpD3 );
	}
	if ( bData & ( 1 << 4 ) ) 
	{
		prtClpD4Set = ( 1 << bnClpD4 );
	}
	else 
	{
		prtClpD4Clr = ( 1 << bnClpD4 );
	}
	if ( bData & ( 1 << 5 ) ) 
	{
		prtClpD5Set = ( 1 << bnClpD5 );
	}
	else 
	{
		prtClpD5Clr = ( 1 << bnClpD5 );
	}
	if ( bData & ( 1 << 6 ) ) 
	{
		prtClpD6Set = ( 1 << bnClpD6 );
	}
	else 
	{
		prtClpD6Clr = ( 1 << bnClpD6 );
	}
	if ( bData & ( 1 << 7 ) ) 
	{
		prtClpD7Set = ( 1 << bnClpD7 );
	}
	else 
	{
		prtClpD7Clr = ( 1 << bnClpD7 );
	}

#endif // _CONTIGUOUS_DATA


	// instruction execution time ensures (in this configuration) times conform to write cycle, so no need to wait additional time
//	asm volatile("nop"); // do nothing

	// Clear E
	prtClpEClr = (1 << bnClpE);

	// instruction execution time ensures (in this configuration) times conform to write cycle, so no need to wait additional time
//	asm volatile("nop"); // do nothing
	// Set RW
	prtClpRWSet = (1 << bnClpRW);
	// instruction execution time ensures (in this configuration) times conform to write cycle, so no need to wait additional time
//	asm volatile("nop"); // do nothing

}

/* ------------------------------------------------------------ */
/*        ClpReadStatus
**
**        Synopsis:
**
**        Parameters:
**
**
**        Return Values:
**                BYTE - the byte rhat was read.
**
**        Errors:
**
**
**        Description:
**				Reads the status of the CLP. It clears the RS and calls ClpReadByte() function.
**
**
*/
BYTE ClpReadStatus()
{
	// clear RS
	prtClpRSClr = (1 << bnClpRS);
	BYTE bStatus = ClpReadByte();
	return bStatus;
}


/* ------------------------------------------------------------ */
/*        ClpWaitUntilNotBusy
**
**        Synopsis:
**				ClpWaitUntilNotBusy()
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
**				Waits until the status of the CLP is not busy. This function relies on ClpReadStatus().
**
**
*/
void ClpWaitUntilNotBusy()
{
	BYTE bStatus;
	bStatus = ClpReadStatus();
	while (bStatus & mskBStatus)
	{
		DelayUs(10);
		bStatus = ClpReadStatus();
	}
}

/* ------------------------------------------------------------ */
/*        ClpWriteCommand
**
**        Synopsis:
**				ClpWriteCommand(cmdLcdClear);
**        Parameters:
**				BYTE bCmd	- the command code byte
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
void ClpWriteCommand(BYTE bCmd)
{
	// wait until LCD is not busy
	ClpWaitUntilNotBusy();

	// 1. Clear RS
	prtClpRSClr = (1 << bnClpRS);

	// 2. Write command byte
	ClpWriteByte(bCmd);
}


/* ------------------------------------------------------------ */
/*        ClpWriteDataByte
**
**        Synopsis:
**				ClpWriteDataByte(pBytes[idx]);
**        Parameters:
**				BYTE bData		- the data byte
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
void ClpWriteDataByte(BYTE bData)
{
	// wait until LCD is not busy
	ClpWaitUntilNotBusy();

	// 	1. Set RS 
	prtClpRSSet = (1 << bnClpRS);

	// 2. Write data byte
	ClpWriteByte(bData);
}

/* ------------------------------------------------------------ */
/*        ClpDisplaySet
**
**        Synopsis:
**				ClpDisplaySet(displaySetOptionDisplayOn | displaySetOptionCursorOn);
**        Parameters:
**				BYTE bDisplaySetOptions	- display options
**					Possible options (to be OR-ed)
**						displaySetOptionDisplayOn - display ON
**						displaySetOptionCursorOn - cursor ON
**						displaySetBlinkOn - cursor blink ON
**
**
**        Return Values:
**                void 
**
**        Errors:
**
**
**        Description:
**				Sets the display options. 
**
*/
void ClpDisplaySet(BYTE bDisplaySetOptions)
{
	ClpWriteCommand(cmdLcdCtlInit | bDisplaySetOptions);
}


/* ------------------------------------------------------------ */
/*        ClpDisplayClear
**
**        Synopsis:
**				ClpDisplayClear();
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
void ClpDisplayClear()
{
	ClpWriteCommand(cmdLcdClear);
}



/* ------------------------------------------------------------ */
/*        ClpReturnHome
**
**        Synopsis:
**				ClpReturnHome();
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
void ClpReturnHome()
{
	ClpWriteCommand(cmdLcdRetHome);
}




/* ------------------------------------------------------------ */
/*        ClpInit
**
**        Synopsis:
**				ClpInit(displaySetOptionDisplayOn);
**        Parameters:
**				BYTE bDisplaySetOptions	- display options
**					Possible options (to be OR-ed)
**						displaySetOptionDisplayOn - display ON
**						displaySetOptionCursorOn - cursor ON
**						displaySetBlinkOn - cursor blink ON
**
**        Return Values:
**                void 
**
**        Errors:
**
**
**        Description:
**			Performs the initializing (startup) sequence. It calls ClpDisplaySet providing bDisplaySetOptions argument.
**
**
*/
void ClpInit(BYTE bDisplaySetOptions)
{
	//	wait 20 ms
	DelayUs(20*1000);
	// Set function
	ClpWriteCommand(cmdLcdFcnInit);
	// Wait 37 us
	DelayUs(37);
	// Display Set
	ClpDisplaySet(bDisplaySetOptions);
	// Wait 37 us
	DelayUs(37);
	// Display Clear
	ClpDisplayClear();
	// Wait 1.52 ms
	DelayUs(1520);
}





/* ------------------------------------------------------------ */
/*        ClpSetWriteDdramPosition
**
**        Synopsis:
**				ClpSetWriteDdramPosition(bAddrOffset);
**        Parameters:
**				BYTE bAdr - the write location. The position in DDRAM where the next data writes will put bytes.
**					0x00-0x27 refer to the first row
**					0x40-0x67 refer to the second row
**
**
**        Return Values:
**                void 
**
**        Errors:
**
**
**        Description:
**				Sets the DDRAM write position. This is the location where the next data write will be performed.
**				Be aware that writing to a location auto-increments the write location.
**
*/
void ClpSetWriteDdramPosition(BYTE bAdr)
{
	BYTE bCmd = cmdLcdSetDdramPos | bAdr;
	ClpWriteCommand(bCmd);
}

/* ------------------------------------------------------------ */
/*        ClpWriteStringAtPos
**
**        Synopsis:
**				ClpWriteStringAtPos(szInfo1, 0, 0);
**        Parameters:
**				char *szLn	- string to be written
**				int idxLine	- line where the string will be written
**				BYTE idxPos	- the starting position of the string within the line
**
**
**        Return Values:
**                void 
**
**        Errors:
**
**
**        Description:
**				Writes the specified string at the specified position on the specified line. 
**				It sets the corresponding write position and then writes data bytes as the device is ready.
**				Strings longer than 0x27 are trimmed.
**
**
*/
void ClpWriteStringAtPos(char *szLn, int idxLine, BYTE idxPos)
{
	// crop string to 0x27 chars
	int len = strlen(szLn);
	if(len > 0x27)
	{
	szLn[0x27] = 0; // trim the string so it contains 40 characters 
		len = 0x27;
	}

	// Set write position
	BYTE bAddrOffset = (idxLine == 0 ? 0: 0x40) + idxPos;
	ClpSetWriteDdramPosition(bAddrOffset);

	BYTE bIdx = 0;
	while(bIdx < len)
	{
		ClpWriteDataByte(szLn[bIdx]);
		bIdx++;
	}
}

/* ------------------------------------------------------------ */
/*        ClpSetWriteCgramPosition
**
**        Synopsis:
**				ClpSetWriteCgramPosition(bAdr);
**        Parameters:
**				BYTE bAdr	- the write location. The position in CGRAM where the next data writes will put bytes.
**
**
**        Return Values:
**                void 
**
**        Errors:
**
**
**        Description:
**				Sets the DDRAM write position. This is the location where the next data write will be performed.
**				Be aware that writing to a location auto-increments the write location.
**
*/
void ClpSetWriteCgramPosition(BYTE bAdr)
{
	BYTE bCmd = cmdLcdSetCgramPos | bAdr;
	ClpWriteCommand(bCmd);
}




/* ------------------------------------------------------------ */
/*        ClpWriteBytesAtPosCgram
**
**        Synopsis:
**				ClpWriteBytesAtPosCgram(userDefArrow, 8, posCgramChar0);
**        Parameters:
**				BYTE *pBytes	- pointer to the string of bytes
**				BYTE len		- the number of bytes to be written
**				BYTE bAdr		- the position in CGRAM where bytes will be written
**
**
**        Return Values:
**                void 
**
**        Errors:
**
**
**        Description:
**				Writes the specified number of bytes to CGRAM starting at the specified position.
**				It sets the corresponding write position and then writes data bytes as the device is ready.
**
*/
void ClpWriteBytesAtPosCgram(BYTE *pBytes, BYTE len, BYTE bAdr)
{
	// Set write position
	ClpSetWriteCgramPosition(bAdr);

	// Write the string of bytes that define the character to CGRAM
	BYTE idx = 0;
	while(idx < len)
	{
		ClpWriteDataByte(pBytes[idx]);
		idx++;
	}
}




/* ------------------------------------------------------------ */
/*        ClpDisplayShift
**
**        Synopsis:
**				ClpDisplayShift(fBtn1Process);
**        Parameters:
**				BOOL fRight
**						- fTrue in order to shift right
**						- fFalse in order to shift left
**
**        Return Values:
**                void 
**
**        Errors:
**
**
**        Description:
**				Shifts the display one position right or left, depending on the fRight parameter.
**
**
**
*/
void ClpDisplayShift(BOOL fRight)
{
	BYTE bCmd = cmdLcdDisplayShift | (fRight != fFalse ? mskShiftRL: 0);
	ClpWriteCommand(bCmd);
}


/* ------------------------------------------------------------ */
/*        ClpCursorShift
**
**        Synopsis:
**				ClpCursorShift(fBtn1Process);
**        Parameters:
**				BOOL fRight
**						- fTrue in order to shift right
**						- fFalse in order to shift left
**
**        Return Values:
**                void 
**
**        Errors:
**
**
**        Description:
**				Shifts the cursor one position right or left, depending on the fRight parameter.
**
**
*/
void ClpCursorShift(BOOL fRight)
{
	BYTE bCmd = cmdLcdCursorShift | (fRight != fFalse ? mskShiftRL: 0);
	ClpWriteCommand(bCmd);
}

/* ------------------------------------------------------------ */


