/************************************************************************/
/*																		*/
/*	OledDriver.c	-- Graphics Driver Library for OLED Display			*/
/*																		*/
/************************************************************************/
/*	Author: 	Gene Apperson											*/
/*	Copyright 2011, Digilent Inc.										*/
/************************************************************************/
/*
  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/
/************************************************************************/
/*  Module Description: 												*/
/*																		*/
/*	This is part of the device driver software for the OLED bit mapped	*/
/*	display on the Digilent Basic I/O Shield. This module contains the	*/
/*	initialization functions and basic display control functions.		*/
/*																		*/
/************************************************************************/
/*  Revision History:													*/
/*																		*/
/*	04/29/2011(GeneA): Created											*/
/*	08/03/2011(GeneA): added functions to shut down the display and to	*/
/*		turn the display on and off.									*/
/*																		*/
/************************************************************************/


/* ------------------------------------------------------------ */
/*				Include File Definitions						*/
/* ------------------------------------------------------------ */

#include <WProgram.h>

extern "C" {
	#include <p32xxxx.h>
	#include <sys/attribs.h>
	#include <wiring.h>
	#include <inttypes.h>
}

#include "Board_Defs.h"
#include "OledDriver.h"
#include "OledChar.h"
#include "OledGrph.h"

#include "DSPI.h"

/* ------------------------------------------------------------ */
/*				Local Symbol Definitions						*/
/* ------------------------------------------------------------ */

#define	cmdOledDisplayOn	0xAF
#define	cmdOledDisplayOff	0xAE
#define	cmdOledSegRemap		0xA1	//map column 127 to SEG0
#define	cmdOledComDir		0xC8	//scan from COM[N-1] to COM0
#define	cmdOledComConfig	0xDA	//set COM hardware configuration

/* ------------------------------------------------------------ */
/*				Global Variables								*/
/* ------------------------------------------------------------ */

extern const uint8_t	rgbOledFont0[];
extern uint8_t		rgbOledFontUser[];
extern const uint8_t	rgbFillPat[];

extern int		xchOledMax;
extern int		ychOledMax;

/* Coordinates of current pixel location on the display. The origin
** is at the upper left of the display. X increases to the right
** and y increases going down.
*/
int		xcoOledCur;
int		ycoOledCur;

uint8_t *	pbOledCur;			//address of byte corresponding to current location
int			bnOledCur;			//bit number of bit corresponding to current location
uint8_t		clrOledCur;			//drawing color to use
uint8_t *	pbOledPatCur;		//current fill pattern
int			fOledCharUpdate;

int		dxcoOledFontCur;
int		dycoOledFontCur;

uint8_t *	pbOledFontCur;
uint8_t *	pbOledFontUser;

// Spi 0 Controller object instantiation
DSPI0 spiCon;

/* Setting pins based on DSPI_SS pin plus offset to get to lower 4 pins
** on pmod connector
*/
const int DataCmd	= PIN_DSPI0_SS + 4;
const int Reset		= PIN_DSPI0_SS + 5;
const int VbatCtrl	= PIN_DSPI0_SS + 6;
const int VddCtrl	= PIN_DSPI0_SS + 7;

/* ------------------------------------------------------------ */
/*				Local Variables									*/
/* ------------------------------------------------------------ */

/* This array is the offscreen frame buffer used for rendering.
** It isn't possible to read back frome the OLED display device,
** so display data is rendered into this offscreen buffer and then
** copied to the display.
*/
uint8_t	rgbOledBmp[cbOledDispMax];

/* ------------------------------------------------------------ */
/*				Forward Declarations							*/
/* ------------------------------------------------------------ */

void	OledHostInit();
void	OledHostTerm();
void	OledDevInit();
void	OledDevTerm();
void	OledDvrInit();

void	OledPutBuffer(int cb, uint8_t * rgbTx);
uint8_t	Spi2PutByte(uint8_t bVal);

/* ------------------------------------------------------------ */
/*				Procedure Definitions							*/
/* ------------------------------------------------------------ */
/***	OledInit
**
**	Parameters:
**		none
**
**	Return Value:
**		none
**
**	Errors:
**		none
**
**	Description:
**		Initialize the OLED display subsystem.
*/

void
OledInit()
	{

	/* Init the PIC32 peripherals used to talk to the display.
	*/
	OledHostInit();

	/* Init the memory variables used to control access to the
	** display.
	*/
	OledDvrInit();

	/* Init the OLED display hardware.
	*/
	OledDevInit();

	/* Clear the display.
	*/
	OledClear();

}

/* ------------------------------------------------------------ */
/***	OledTerm
**
**	Parameters:
**		none
**
**	Return Value:
**		none
**
**	Errors:
**		none
**
**	Description:
**		Shut down the OLED display.
*/

void
OledTerm()
	{

	/* Shut down the OLED display hardware.
	*/
	OledDevTerm();

	/* Release the PIC32 peripherals being used.
	*/
	OledHostTerm();

}

/* ------------------------------------------------------------ */
/***	OledHostInit
**
**	Parameters:
**		none
**
**	Return Value:
**		none
**
**	Errors:
**		none
**
**	Description:
**		Perform PIC32 device initialization to prepare for use
**		of the OLED display.
**		This is currently hard coded for the Cerebot 32MX4 and
**		SPI2. This needs to be generalized.
*/

void
OledHostInit()
	{

	spiCon.begin();
	spiCon.setSpeed(4000000);
	spiCon.setMode(DSPI_MODE3);

	pinMode(DataCmd, OUTPUT);
	pinMode(VbatCtrl, OUTPUT);
	pinMode(VddCtrl, OUTPUT);

	digitalWrite(DataCmd, HIGH);
	digitalWrite(VbatCtrl, HIGH);
	digitalWrite(VddCtrl, HIGH);

	pinMode(Reset, OUTPUT);
	digitalWrite(Reset, HIGH);

	spiCon.setSelect(LOW);
}

/* ------------------------------------------------------------ */
/***	OledHostTerm
**
**	Parameters:
**		none
**
**	Return Value:
**		none
**
**	Errors:
**		none
**
**	Description:
**		Release processor resources used by the library
*/

void
OledHostTerm()
	{

	///* Make the Data/Command select, Reset, and SPI CS pins be inputs.
	//*/
	digitalWrite(DataCmd, HIGH);
	digitalWrite(Reset, HIGH);

	pinMode(DataCmd, INPUT);
	pinMode(Reset, INPUT);

	///* Make power control pins be inputs. The pullup resistors on the
	//** board will ensure that the power supplies stay off.
	//*/
	digitalWrite(VddCtrl, HIGH);
	digitalWrite(VbatCtrl, HIGH);

	pinMode(VddCtrl, INPUT);
	pinMode(VbatCtrl, INPUT);

	spiCon.end();
}

/* ------------------------------------------------------------ */
/***	OledDvrInit
**
**	Parameters:
**		none
**
**	Return Value:
**		none
**
**	Errors:
**		none
**
**	Description:
**		Initialize the OLED software system
*/

void
OledDvrInit()
	{
	int		ib;

	/* Init the parameters for the default font
	*/
	dxcoOledFontCur = cbOledChar;
	dycoOledFontCur = 8;
	pbOledFontCur = (uint8_t*)rgbOledFont0;
	pbOledFontUser = rgbOledFontUser;

	for (ib = 0; ib < cbOledFontUser; ib++) {
		rgbOledFontUser[ib] = 0;
	}

	xchOledMax = ccolOledMax / dxcoOledFontCur;
	ychOledMax = crowOledMax / dycoOledFontCur;

	/* Set the default character cursor position.
	*/
	OledSetCursor(0, 0);

	/* Set the default foreground draw color and fill pattern
	*/
	clrOledCur = 0x01;
	pbOledPatCur = (uint8_t*)rgbFillPat;
	OledSetDrawMode(modOledSet);

	/* Default the character routines to automaticall
	** update the display.
	*/
	fOledCharUpdate = 1;

}

/* ------------------------------------------------------------ */
/***	OledDevInit
**
**	Parameters:
**		none
**
**	Return Value:
**		none
**
**	Errors:
**		none
**
**	Description:
**		Initialize the OLED display controller and turn the display on.
*/

void
OledDevInit()
	{

	/* We're going to be sending commands, so clear the Data/Cmd bit
	*/
	digitalWrite(DataCmd, LOW);

	/* Start by turning VDD on and wait a while for the power to come up.
	*/
	digitalWrite(VddCtrl, LOW);
	delay(1);

	/* Display off command
	*/
	Spi2PutByte(cmdOledDisplayOff);

	/* Bring Reset low and then high
	*/
	digitalWrite(Reset, LOW);
	delay(1);
	digitalWrite(Reset, HIGH);

	/* Send the Set Charge Pump and Set Pre-Charge Period commands
	*/
	Spi2PutByte(0x8D);		//From Univision data sheet, not in SSD1306 data sheet
	Spi2PutByte(0x14);

	Spi2PutByte(0xD9);		//From Univision data sheet, not in SSD1306 data sheet
	Spi2PutByte(0xF1);

	/* Turn on VCC and wait 100ms
	*/
	digitalWrite(VbatCtrl, LOW);
	delay(100);

	/* Send the commands to invert the display.
	*/
	Spi2PutByte(cmdOledSegRemap);		//remap columns
	Spi2PutByte(cmdOledComDir);			//remap the rows

	/* Send the commands to select sequential COM configuration
	*/
	Spi2PutByte(cmdOledComConfig);		//set COM configuration command
	Spi2PutByte(0x20);					//sequential COM, left/right remap enabled

	/* Send Display On command
	*/
	Spi2PutByte(cmdOledDisplayOn);

}

/* ------------------------------------------------------------ */
/***	OledDevTerm
**
**	Parameters:
**		none
**
**	Return Value:
**		none
**
**	Errors:
**		none
**
**	Description:
**		Shut down the OLED display hardware
*/

void
OledDevTerm()
	{

	/* Send the Display Off command.
	*/
	Spi2PutByte(cmdOledDisplayOff);

	/* Turn off VCC
	*/
	digitalWrite(VbatCtrl, HIGH);
	delay(100);

	/* Turn off VDD
	*/
	digitalWrite(VbatCtrl, LOW);
}

/* ------------------------------------------------------------ */
/***	OledDisplayOn
**
**	Parameters:
**		none
**
**	Return Value:
**		none
**
**	Errors:
**		none
**
**	Description:
**		Turn the display on. This assumes that the display has
**		already been powered on and initialized. All it does
**		is send the display on command.
*/

void
OledDisplayOn()
	{
	digitalWrite(DataCmd, LOW);
	Spi2PutByte(cmdOledDisplayOn);

}

/* ------------------------------------------------------------ */
/***	OledDisplayOff
**
**	Parameters:
**		none
**
**	Return Value:
**		none
**
**	Errors:
**		none
**
**	Description:
**		Turn the display off. This does not power the display
**		down. All it does is send the display off command.
*/

void
OledDisplayOff()
	{
	digitalWrite(DataCmd, LOW);
	Spi2PutByte(cmdOledDisplayOff);

}

/* ------------------------------------------------------------ */
/***	OledClear
**
**	Parameters:
**		none
**
**	Return Value:
**		none
**
**	Errors:
**		none
**
**	Description:
**		Clear the display. This clears the memory buffer and then
**		updates the display.
*/

void
OledClear()
	{

	OledClearBuffer();
	OledUpdate();

}

/* ------------------------------------------------------------ */
/***	OledClearBuffer
**
**	Parameters:
**		none
**
**	Return Value:
**		none
**
**	Errors:
**		none
**
**	Description:
**		Clear the display memory buffer.
*/

void
OledClearBuffer()
	{
	int			ib;
	uint8_t *		pb;

	pb = rgbOledBmp;

	/* Fill the memory buffer with 0.
	*/
	for (ib = 0; ib < cbOledDispMax; ib++) {
		*pb++ = 0x00;
	}

}

/* ------------------------------------------------------------ */
/***	OledUpdate
**
**	Parameters:
**		none
**
**	Return Value:
**		none
**
**	Errors:
**		none
**
**	Description:
**		Update the OLED display with the contents of the memory buffer
*/

void
OledUpdate()
	{
	int		ipag;
	int		icol;
	uint8_t *	pb;

	pb = rgbOledBmp;

	for (ipag = 0; ipag < cpagOledMax; ipag++) {

		digitalWrite(DataCmd, LOW);

		/* Set the page address
		*/
		Spi2PutByte(0x22);		//Set page command
		Spi2PutByte(ipag);		//page number

		/* Start at the left column
		*/
		Spi2PutByte(0x00);		//set low nybble of column
		Spi2PutByte(0x10);		//set high nybble of column

		digitalWrite(DataCmd, HIGH);

		/* Copy this memory page of display data.
		*/
		OledPutBuffer(ccolOledMax, pb);
		pb += ccolOledMax;

	}

}

/* ------------------------------------------------------------ */
/***	OledPutBuffer
**
**	Parameters:
**		cb		- number of bytes to send/receive
**		rgbTx	- pointer to the buffer to send
**
**	Return Value:
**		none
**
**	Errors:
**		none
**
**	Description:
**		Send the bytes specified in rgbTx to the slave and return
**		the bytes read from the slave in rgbRx
*/
void
OledPutBuffer(int cb, uint8_t * rgbTx)
	{
	int		ib;
	uint8_t	bTmp;

	/* Write/Read the data
	*/
	for (ib = 0; ib < cb; ib++) {

		bTmp = spiCon.transfer(*rgbTx++);

	}

}

/* ------------------------------------------------------------ */
/***	Spi2PutByte
**
**	Parameters:
**		bVal		- byte value to write
**
**	Return Value:
**		Returns byte read
**
**	Errors:
**		none
**
**	Description:
**		Write/Read a byte on SPI port 2
*/
uint8_t
Spi2PutByte(uint8_t bVal)
	{
	uint8_t	bRx;

	bRx = spiCon.transfer(bVal);
	
	return bRx;
}

/* ------------------------------------------------------------ */
/***	ProcName
**
**	Parameters:
**
**	Return Value:
**
**	Errors:
**
**	Description:
**
*/

/* ------------------------------------------------------------ */

/************************************************************************/

