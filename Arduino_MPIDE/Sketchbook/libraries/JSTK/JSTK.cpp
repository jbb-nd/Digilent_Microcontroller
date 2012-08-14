/************************************************************************/
/*																		*/
/*	JSTK.cpp	--	Joystick Driver for JSTK				        	*/
/*																		*/
/************************************************************************/
/*	Author: 	Oliver Jones											*/
/*	Copyright (c) 2011, Digilent Inc, All rights reserved				*/
/************************************************************************/
/*  Module Description: 												*/
/*																		*/
/*																		*/
/************************************************************************/
/*  Revision History:													*/
/*																		*/
/*	10/26/2011(OliverJ): created										*/
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


/* ------------------------------------------------------------ */
/*				Include File Definitions						*/
/* ------------------------------------------------------------ */

#include "JSTK.h"

extern "C" {
  #include <stdint.h>
}

/* ------------------------------------------------------------ */
/*			Joystick Driver Object Instantiation					*/
/* ------------------------------------------------------------ */
/* Instantiate a single static instance of this object class
*/



JSTK::JSTK()
{
}

void JSTK::begin()
{
	uint8_t temp = 0;

	spiCon.begin();

	spiCon.setSpeed(1000000);

	spiCon.setMode(DSPI_MODE0);
}

void JSTK::end()
{
	spiCon.end();
}

void JSTK::setLeds(uint8_t ledSt)
{
	uint8_t recv[5];
	ledSt |= 0x80;
	getAll(ledSt,recv);
}

uint16_t JSTK::getX()
{
	uint16_t xPos;
	uint8_t recv[5];
	getAll(0,recv);

	xPos = recv[0];
	xPos |= (recv[1] << 8);

	return xPos;	
}

uint16_t JSTK::getY()
{
	uint16_t yPos;
	uint8_t recv[5];
	getAll(0,recv);

	yPos = recv[2];
	yPos |= (recv[3] << 8);

	return yPos;
}

uint8_t JSTK::getBtn()
{
	uint8_t recv[5];
	getAll(0,recv);

	return recv[4];
}

void JSTK::getAll(uint8_t ledSt, uint8_t *recv)
{
	spiCon.setSelect(LOW);
	delay(1);
	*recv++ = spiCon.transfer(ledSt);
	delay(1);
	*recv++ = spiCon.transfer(0);
	delay(1);
	*recv++ = spiCon.transfer(0);
	delay(1);
	*recv++ = spiCon.transfer(0);
	delay(1);
	*recv = spiCon.transfer(0);

	spiCon.setSelect(HIGH);
}


