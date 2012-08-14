/************************************************************************/
/*																		*/
/*	util.c -- Common Utility Procedures for the PIC32MX                 */
/*																		*/
/************************************************************************/
/*	Author: Michael T. Alexander										*/
/*	Copyright 2009, Digilent Inc.										*/
/************************************************************************/
/*  Module Description: 												*/
/*																		*/
/*	This module contains definitions for common utility functions.      */
/*																		*/
/************************************************************************/
/*  Revision History:						                			*/
/*											                        	*/
/*	04/28/2009 (MichaelA): created			                			*/
/*	07/07/2009 (MichaelA): added routine for setting LEDs				*/
/*	07/10/2009 (MichaelA): added InitLeds()								*/
/*											                        	*/
/************************************************************************/

/* ------------------------------------------------------------ */
/*				Include File Definitions						*/
/* ------------------------------------------------------------ */

#include <plib.h>
#include "stdtypes.h"
#include "config.h"
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
/***    DelayMs
**
**	Synopsis:
**		DelayMs(tmsDelay)
**
**	Parameters:
**		tmsDelay - the number of milliseconds you want to delay
**
**	Return Values:
**      none
**
**	Errors:
**		none
**
**	Description:
**		This procedure delays program execution for the specified number
**      of miliseconds.
*/
void DelayMs( WORD tmsDelay )
{	
	while ( 0 < tmsDelay ) {
		DelayUs(1000);
		tmsDelay--;
	}
}

/* ------------------------------------------------------------ */
/***    DelayUs
**
**	Synopsis:
**		DelayUs(tusDelay)
**
**	Parameters:
**		tusDelay - the amount of time you wish to delay in microseconds
**
**	Return Values:
**      none
**
**	Errors:
**		none
**
**	Description:
**		This procedure delays program execution for the specified number
**      of microseconds. Please note that the minimal delay supported by
**		this routine is 3 microseconds.
**		
**	Note:
**		This routine is written with the assumption that the
**		system clock is 64 MHz.
*/
void DelayUs( WORD tusDelay )
{
	tusDelay -= 2;
	
    while ( 0 < tusDelay )
    {
        tusDelay--;
    }   // end while
}


/* ------------------------------------------------------------ */
/***	InitLeds
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
**		This routine initializes the LEDs by configuring the
**		appropriate pins as digital outputs and setting their
**		state to '0'.
*/

void InitLeds( void )
{
	// Configure LEDs as digital outputs.
	trisLed1Clr = ( 1 << bnLed1 );
	trisLed2Clr = ( 1 << bnLed2 );
	trisLed3Clr = ( 1 << bnLed3 );
	trisLed4Clr = ( 1 << bnLed4 );
	
	// Turn off the LEDs.
	prtLed1Clr	= ( 1 << bnLed1 );
	prtLed2Clr	= ( 1 << bnLed2 );
	prtLed3Clr	= ( 1 << bnLed3 );
	prtLed4Clr	= ( 1 << bnLed4 );
}	


/* ------------------------------------------------------------ */
/***	SetLeds
**
**	Parameters:
**		stLeds - state to set the LEDs to (0 to 15)
**
**	Return Value:
**		none
**
**	Errors:
**		none
**
**	Description:
**		This routine sets the state of the on-board LEDs to the
**		state specified by stLeds. Please note that only the four
**		least significant bits of stLeds are used.
*/

void SetLeds( BYTE stLeds ) {
	// Set the state of Led1.
	if ( stLeds & ( 1 << 0 ) ) {
		prtLed1Set = ( 1 << bnLed1 );
	}
	else {
		prtLed1Clr = ( 1 << bnLed1 );
	}
	
	// Set the state of Led2.
	if ( stLeds & ( 1 << 1 ) ) {
		prtLed2Set = ( 1 << bnLed2 );
	}
	else {
		prtLed2Clr = ( 1 << bnLed2 );
	}
	
	// Set the state of Led3.
	if ( stLeds & ( 1 << 2 ) ) {
		prtLed3Set = ( 1 << bnLed3 );
	}
	else {
		prtLed3Clr = ( 1 << bnLed3 );
	}
	
	// Set the state of Led4.
	if ( stLeds & ( 1 << 3 ) ) {
		prtLed4Set = ( 1 << bnLed4 );
	}
	else {
		prtLed4Clr = ( 1 << bnLed4 );
	}
}

/*************************************************************************************/
