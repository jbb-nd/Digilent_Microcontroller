/************************************************************************/
/*																		*/
/*	IOShieldOled.h	--	Interface Declarations for IOShieldOled.cpp		*/
/*																		*/
/************************************************************************/
/*	Author:		Gene Apperson											*/
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
/*  File Description:													*/
/*																		*/
/*	This header file contains the object class declarations and other	*/
/*	interface declarations need to use the OLED graphics display driver	*/
/*	for the Digilent Basic I/O Shield.									*/
/*																		*/
/************************************************************************/
/*  Revision History:													*/
/*																		*/
/*	04/29/2011(OliverJ): created										*/
/*	08/03/2011(GeneA): added new functions for display control			*/
/*	08/04/2011(GeneA): prepare first release							*/
/*																		*/
/************************************************************************/

#if !defined(OLED_H)
#define OLED_H

/* ------------------------------------------------------------ */
/*					Miscellaneous Declarations					*/
/* ------------------------------------------------------------ */

#include <inttypes.h>

#define BYTE uint8_t

/* ------------------------------------------------------------ */
/*					Global Variable Declarations				*/
/* ------------------------------------------------------------ */

/* ------------------------------------------------------------ */
/*					Object Class Declarations					*/
/* ------------------------------------------------------------ */

class OledClass
{
  private:
     
  public:
    /* Class Constants
	*/
	static const int colMax = 128;	//number of columns in the display
	static const int rowMax = 32;	//number of rows in the display
	static const int pageMax = 4;	//number of display pages

	static const int modeSet = 0;	//set pixel drawing mode
	static const int modeOr  = 1;	//or pixels drawing mode
	static const int modeAnd = 2;	//and pixels drawing mode
	static const int modeXor = 3;	//xor pixels drawing mode

    OledClass();

	/* Basic device control functions.
	*/
    void begin(void);
	void end(void);
	void displayOn(void);
	void displayOff(void);
	void clear(void);
	void clearBuffer(void);
	void updateDisplay(void);

	/* Character output functions.
	*/
	void  setCursor(int xch, int ych);
	void  getCursor(int *pxcy, int *pych);
	int   defineUserChar(char ch, BYTE *pbDef);
	void  setCharUpdate(int f);
	int   getCharUpdate(void);
	void  putChar(char ch);
	void  putString(char *sz);

	/* Graphic output functions.
	*/
	void  setDrawColor(BYTE clr);
	void  setDrawMode(int mod);
	int   getDrawMode();
	BYTE* getStdPattern(int ipat);
	void  setFillPattern(BYTE *pbPat);

	void moveTo(int xco, int yco);
	void getPos(int *pxco, int *pyco);
	void drawPixel(void);
	BYTE getPixel(void);
	void drawLine(int xco, int yco);
	void drawRect(int xco, int yco);
	void drawFillRect(int xco, int yco);
	void getBmp(int dxco, int dyco, BYTE *pbBmp);
	void putBmp(int dxcp, int dyco, BYTE *pbBmp);
	void drawChar(char ch);
	void drawString(char *sz);
};

/* ------------------------------------------------------------ */

#endif

/************************************************************************/
