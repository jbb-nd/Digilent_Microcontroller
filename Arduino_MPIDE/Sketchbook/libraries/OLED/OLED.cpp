/************************************************************************/
/*																		*/
/*	IOShieldOled.c	--	OLED Display Driver for Basic I/O Shield		*/
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
/*	This module contains the implementation of the object class that	*/
/*	forms the chipKIT interface to the graphics driver functions for	*/
/*	the OLED display on the Digilent Basic I/O Shield.					*/
/*																		*/
/************************************************************************/
/*  Revision History:													*/
/*																		*/
/*	06/01/2011(GeneA): created											*/
/*	08/04/2011(GeneA): prepare first release							*/
/*																		*/
/************************************************************************/


/* ------------------------------------------------------------ */
/*				Include File Definitions						*/
/* ------------------------------------------------------------ */

#include "OLED.h"

#include "OledDriver.h"
#include "OledChar.h"
#include "OledGrph.h"

extern "C" {
  #include <stdlib.h>
  #include <string.h>
  #include <inttypes.h>
}

/* ------------------------------------------------------------ */
/*				Local Type Definitions							*/
/* ------------------------------------------------------------ */


/* ------------------------------------------------------------ */
/*				OledClass Definitions					*/
/* ------------------------------------------------------------ */

/* ------------------------------------------------------------ */
/***	void OledClass::OledClass()
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
**		Default constructor.
*/

OledClass::OledClass()
{
}

/* ------------------------------------------------------------ */
/***	void OledClass::begin(void)
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
void OledClass::begin(void)
{
	OledInit();
}

/* ------------------------------------------------------------ */
/***	void OledClass::end(void)
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
**		Shut down the OLED display. This turns the power off to the
**		OLED display, and releases all of the PIC32 resources used
**		by the OLED display driver.
*/

void OledClass::end()
{
	OledTerm();
}

/* ------------------------------------------------------------ */
/***	void OledClass::displayOn(void)
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
**		Turn the display on. This assumes that the display has been
**		initialized and is powered on. It just unblanks the display.
*/

void OledClass::displayOn()
{
	OledDisplayOn();
}

/* ------------------------------------------------------------ */
/***	void OledClass::displayOff(void)
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
**		Turn the display off. This doesn't power the display down.
**		It blanks the display, but does not clear it.
*/

void OledClass::displayOff()
{
	OledDisplayOff();
}

/* ------------------------------------------------------------ */
/***	void OledClass::clear(void)
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
void OledClass::clear(void)
{
	OledClear();
}

/* ------------------------------------------------------------ */
/***	void OledClass::clearBuffer(void)
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
void OledClass::clearBuffer(void)
{
	OledClearBuffer();
}

/* ------------------------------------------------------------ */
/***	void OledClass::updateDisplay(void)
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
void OledClass::updateDisplay(void)
{
	OledUpdate();
}

/* ------------------------------------------------------------ */
/***	void OledClass::setCursor(int xch, int ych);
**
**	Parameters:
**		xch			- horizontal character position
**		ych			- vertical character position
**
**	Return Value:
**		none
**
**	Errors:
**		none
**
**	Description:
**		Set the character cursor position to the specified location.
**		If either the specified X or Y location is off the display, it
**		is clamped to be on the display.
*/
void OledClass::setCursor(int xch, int ych)
{
	OledSetCursor(xch,ych);
}

/* ------------------------------------------------------------ */
/***	void OledClass::getCursor(int *pxcy, int *pych)
**
**	Parameters:
**		pxch		- pointer to variable to receive horizontal position
**		pych		- pointer to variable to receive vertical position
**
**	Return Value:
**		none
**
**	Errors:
**		none
**
**	Description:
**		Fetch the current cursor position
*/
void OledClass::getCursor(int *pxcy, int *pych)
{
	OledGetCursor(pxcy, pych);
}

/* ------------------------------------------------------------ */
/***	int OledClass::defineUserChar(char ch, BYTE *pbDef)
**
**	Parameters:
**		ch		- character code to define
**		pbDef	- definition for the character
**
**	Return Value:
**		none
**
**	Errors:
**		Returns TRUE if successful, FALSE if not
**
**	Description:
**		Give a definition for the glyph for the specified user
**		character code. User definable character codes are in
**		the range 0x00 - 0x1F. If the code specified by ch is
**		outside this range, the function returns false.
*/
int OledClass::defineUserChar(char ch, BYTE *pbDef)
{
	return OledDefUserChar(ch, pbDef);
}

/* ------------------------------------------------------------ */
/***	void OledClass::setCharUpdate(int f)
**
**	Parameters:
**		f		- enable/disable automatic update
**
**	Return Value:
**		none
**
**	Errors:
**		none
**
**	Description:
**		Set the character update mode. This determines whether
**		or not the display is automatically updated after a
**		character or string is drawn. A non-zero value turns
**		automatic updating on.
*/
void OledClass::setCharUpdate(int f)
{
	OledSetCharUpdate(f);
}

/* ------------------------------------------------------------ */
/***	int OledClass::getCharUpdate(void)
**
**	Parameters:
**		none
**
**	Return Value:
**		returns current character update mode
**
**	Errors:
**		none
**
**	Description:
**		Return the current character update mode.
*/
int OledClass::getCharUpdate(void)
{
	return OledGetCharUpdate();
}

/* ------------------------------------------------------------ */
/***	void OledClass::putChar(char ch)
**
**	Parameters:
**		ch			- character to write to display
**
**	Return Value:
**		none
**
**	Errors:
**		none
**
**	Description:
**		Write the specified character to the display at the current
**		cursor position and advance the cursor.
*/
void OledClass::putChar(char ch)
{
	OledPutChar(ch);
}

/* ------------------------------------------------------------ */
/***	void OledClass::putString(char *sz)
**
**	Parameters:
**		sz		- pointer to the null terminated string
**
**	Return Value:
**		none
**
**	Errors:
**		none
**
**	Description:
**		Write the specified null terminated character string to the
**		display and advance the cursor.
*/
void OledClass::putString(char *sz)
{
	OledPutString(sz);
}

/* ------------------------------------------------------------ */
/***	void OledClass::setDrawColor(BYTE clr)
**
**	Parameters:
**		clr		- drawing color to set
**
**	Return Value:
**		none
**
**	Errors:
**		none
**
**	Description:
**		Set the foreground color used for pixel draw operations.
*/
void OledClass::setDrawColor(BYTE clr)
{
	OledSetDrawColor(clr);
}

/* ------------------------------------------------------------ */
/***	void OledClass::setDrawMode(int mod);
**
**	Parameters:
**		mod		- drawing mode to select
**
**	Return Value:
**		none
**
**	Errors:
**		none
**
**	Description:
**		Set the specified mode as the current drawing mode.
*/
void OledClass::setDrawMode(int mod)
{
	OledSetDrawMode(mod);
}

/* ------------------------------------------------------------ */
/***	int OledClass::getDrawMode();
**
**	Parameters:
**		none

**	Return Value:
**		returns current drawing mode
**
**	Errors:
**		none
**
**	Description:
**		Get the current drawing mode
*/
int OledClass::getDrawMode(void)
{
	return OledGetDrawMode();
}

/* ------------------------------------------------------------ */
/***	BYTE* OledClass::getStdPattern(int ipat)
**
**	Parameters:
**		ipat		- index to standard fill pattern
**
**	Return Value:
**		returns a pointer to the standard fill pattern
**
**	Errors:
**		returns pattern 0 if index out of range
**
**	Description:
**		Return a pointer to the byte array for the specified
**		standard fill pattern.
*/
BYTE* OledClass::getStdPattern(int ipat)
{
	return OledGetStdPattern(ipat);
}

/* ------------------------------------------------------------ */
/***	void OledClass::setFillPattern(BYTE *pbPat)
**
**	Parameters:
**		pbPat	- pointer to the fill pattern
**
**	Return Value:
**		none
**
**	Errors:
**		none
**
**	Description:
**		Set a pointer to the current fill pattern to use. A fill
**		pattern is an array of 8 bytes.
*/
void OledClass::setFillPattern(BYTE *pbPat)
{
	OledSetFillPattern(pbPat);
}

/* ------------------------------------------------------------ */
/***	void OledClass::moveTo(int xco, int yco)
**
**	Parameters:
**		xco			- x coordinate
**		yco			- y coordinate
**
**	Return Value:
**		none
**
**	Errors:
**		none
**
**	Description:
**		Set the current graphics drawing position.
*/

void OledClass::moveTo(int xco, int yco)
{
	OledMoveTo(xco, yco);
}

/* ------------------------------------------------------------ */
/***	void OledClass::getPos(int *pxco, int *pyco)
**
**	Parameters:
**		pxco	- variable to receive x coordinate
**		pyco	- variable to receive y coordinate
**
**	Return Value:
**		none
**
**	Errors:
**		none
**
**	Description:
**		Fetch the current graphics drawing position
*/
void OledClass::getPos(int *pxco, int *pyco)
{
	OledGetPos(pxco, pyco);
}

/* ------------------------------------------------------------ */
/***	void OledClass::drawPixel(void)
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
**		Set the pixel at the current drawing location to the
**		specified value.
*/
void OledClass::drawPixel(void)
{
	OledDrawPixel();
}

/* ------------------------------------------------------------ */
/***	BYTE OledClass::getPixel(void)
**
**	Parameters:
**		none
**
**	Return Value:
**		returns pixel value at current drawing location
**
**	Errors:
**		none
**
**	Description:
**		Return the value of the pixel at the current drawing location
*/
BYTE OledClass::getPixel(void)
{
	return OledGetPixel();
}

/* ------------------------------------------------------------ */
/***	void OledClass::drawLine(int xco, int yco)
**
**	Parameters:
**		xco			- x coordinate
**		yco			- y coordinate
**
**	Return Value:
**		none
**
**	Errors:
**		none
**
**	Description:
**		Draw a line from the current position to the specified
**		position.
*/
void OledClass::drawLine(int xco, int yco)
{
	OledLineTo(xco, yco);
}

/* ------------------------------------------------------------ */
/***	void OledClass::drawRect(int xco, int yco)
**
**	Parameters:
**		xco		- x coordinate of other corner
**		yco		- y coordinate of other corner
**
**	Return Value:
**		none
**
**	Errors:
**		none
**
**	Description:
**		Draw a rectangle bounded by the current location and
**		the specified location.
*/
void OledClass::drawRect(int xco, int yco)
{
	OledDrawRect(xco, yco);
}

/* ------------------------------------------------------------ */
/***	void OledClass::drawFillRect(int xco, int yco)
**
**	Parameters:
**		xco		- x coordinate of other corner
**		yco		- y coordinate of other corner
**
**	Return Value:
**		none
**
**	Errors:
**		none
**
**	Description:
**		Fill a rectangle bounded by the current location and
**		the specified location.
*/
void OledClass::drawFillRect(int xco, int yco)
{
	OledFillRect(xco, yco);
}

/* ------------------------------------------------------------ */
/***	void OledClass::getBmp(int dxco, int dyco, BYTE *pbBmp)
**
**	Parameters:
**		dxco		- width of bitmap
**		dyco		- height of bitmap
**		pbBits		- pointer to the bitmap bits	
**
**	Return Value:
**		none
**
**	Errors:
**		none
**
**	Description:
**		This routine will get the bits corresponding to the
**		rectangle implied by the current location and the
**		specified width and height. The buffer specified
**		by pbBits must be large enough to hold the resulting
**		bytes.
*/
void OledClass::getBmp(int dxco, int dyco, BYTE *pbBmp)
{
	OledGetBmp(dxco, dyco, pbBmp);
}

/* ------------------------------------------------------------ */
/***	void OledClass::putBmp(int dxcp, int dyco, BYTE *pbBmp)
**
**	Parameters:
**		dxco		- width of bitmap
**		dyco		- height of bitmap
**		pbBits		- pointer to the bitmap bits	
**
**	Return Value:
**		none
**
**	Errors:
**		none
**
**	Description:
**		This routine will put the specified bitmap into the display
**		buffer at the current location.
*/
void OledClass::putBmp(int dxcp, int dyco, BYTE *pbBmp)
{
	OledPutBmp(dxcp, dyco, pbBmp);
}

/* ------------------------------------------------------------ */
/***	void OledClass::drawChar(char ch)
**
**	Parameters:
**		ch			- character to write to display
**
**	Return Value:
**		none
**
**	Errors:
**		none
**
**	Description:
**		Write the specified character to the display at the current
**		cursor position and advance the cursor.
*/
void OledClass::drawChar(char ch)
{
	OledDrawChar(ch);
}

/* ------------------------------------------------------------ */
/***	void OledClass::drawString(char *sz)
**
**	Parameters:
**		sz		- pointer to the null terminated string
**
**	Return Value:
**		none
**
**	Errors:
**		none
**
**	Description:
**		Write the specified null terminated character string to the
**		display and advance the cursor.
*/
void OledClass::drawString(char *sz)
{
	OledDrawString(sz);
}

/* ------------------------------------------------------------ */

/************************************************************************/

