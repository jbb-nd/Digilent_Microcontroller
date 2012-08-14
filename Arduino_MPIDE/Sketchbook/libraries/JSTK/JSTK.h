/************************************************************************/
/*																		*/
/*	JSTK.h	--	Interface Declarations for JSTK.cpp			        	*/
/*																		*/
/************************************************************************/
/*	Author:		Oliver Jones											*/
/*	Copyright (c) 2011, Digilent Inc. All rights reserved.				*/
/************************************************************************/
/*  File Description:													*/
/*																		*/
/*																		*/
/************************************************************************/
/*  Revision History:													*/
/*																		*/
/*	10/17/2011(OliverJ): created										*/
/*																		*/
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

#if !defined(JSTK_H)
#define JSTK_H

#include "DSPI.h"

extern "C" {
  #include <stdint.h>
}

/* ------------------------------------------------------------ */
/*					Miscellaneous Declarations					*/
/* ------------------------------------------------------------ */

/*	Led States
*/
#define JSTK_LED1 0x01
#define JSTK_LED2 0x02

/*  Button States
*/
#define JSTK_BTN1 0x02
#define JSTK_BTN2 0x04

/* ------------------------------------------------------------ */
/*					Object Class Declarations					*/
/* ------------------------------------------------------------ */

class JSTK
{
	public:
		JSTK();
		void begin();
		void end();
		void setLeds(uint8_t ledSt);
		uint16_t getX();
		uint16_t getY();
		uint8_t getBtn();
		void getAll(uint8_t ledSt, uint8_t *recv);

	private:
		DSPI0 spiCon;
};

#endif