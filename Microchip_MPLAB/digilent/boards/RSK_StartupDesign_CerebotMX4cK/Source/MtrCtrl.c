/************************************************************************/
/*																		*/
/*	MtrCtrl.c	--  Motor Control Routines				                */
/*																		*/
/************************************************************************/
/*	Author: Michael T. Alexander										*/
/*	Copyright 2009, Digilent Inc.										*/
/************************************************************************/
/*  File Description:													*/
/*																		*/
/*  This module contains declarations and definitions for controlling   */
/*  the robot motors using a combination of pulse width modulation and  */
/*  pulse width timing of the feedback signals provided by the DC 		*/
/*	motors.																*/
/*																		*/
/************************************************************************/
/*  Revision History:													*/
/*																		*/
/*  06/03/2009 (MichaelA): created                                      */
/*																		*/
/************************************************************************/

/* ------------------------------------------------------------ */
/*				Include File Definitions						*/
/* ------------------------------------------------------------ */

#include <plib.h>
#include "config.h"
#include "stdtypes.h"
#include "MtrCtrl.h"

/* ------------------------------------------------------------ */
/*				Local Type Definitions							*/
/* ------------------------------------------------------------ */


/* ------------------------------------------------------------ */
/*				Global Variables								*/
/* ------------------------------------------------------------ */


/* ------------------------------------------------------------ */
/*				Local Variables									*/
/* ------------------------------------------------------------ */

const		HWORD	rgtusMtr[] = { 1, 10, 50, 100, 500, 1000, 2000 };
const		HWORD	rgdtcMtr[] = { 0,  1,  5,  10,  20,   30,   40 };

/* ------------------------------------------------------------ */
/*				Forward Declarations							*/
/* ------------------------------------------------------------ */


/* ------------------------------------------------------------ */
/*				Interrupt Service Routines	            	    */
/* ------------------------------------------------------------ */

void __ISR(_TIMER_1_VECTOR, ipl7) _Timer1Handler(void)
{
	HWORD	dtcMtr;
	
	IFS0CLR = ( 1 << 4 );
	
	INTDisableInterrupts();
	if ( ! fMtrLeft ) {
		// Left motor feedback has timed out.
		
		INTEnableInterrupts();
		
		dtcMtr = OC1R + 20;
		if ( dtcMtrMax >= dtcMtr ) {
			OC1RS=  dtcMtr;
		}
		else {
			OC1RS = dtcMtrMax;
		}
	}
	else {
		fMtrLeft = fFalse;
		INTEnableInterrupts();
	}
	
	INTDisableInterrupts();
	if ( ! fMtrRight ) {
		// Right motor feedback has timed out.
		
		INTEnableInterrupts();
		
		dtcMtr = OC4R + 20;
		if ( dtcMtrMax >= dtcMtr ) {
			OC4RS = dtcMtr;
		}
		else {
			OC4RS = dtcMtrMax;
		}
	}
	else {
		fMtrRight = fFalse;
		INTEnableInterrupts();
	}
}

void __ISR(_TIMER_4_VECTOR, ipl7) _Timer4Handler(void)
{	
	HWORD	tusMtr;
	HWORD	dtcMtr;
	BYTE	idtcMtr;
	
	fMtrRight	= fTrue;
	tusMtr		= TMR4;
	TMR4		= 0;
	IFS0CLR		= ( 1 << 16 );
	
	if ( fMtrRightFb ) {
		// Feedback is enabled for the right motor.
		
		dtcMtr = OC4R;	// get current motor duty cycle
		if ( tusMtr < tusMtrRightFb ) {
			// Right motor is running too fast.
			
			// Determine new motor duty cycle.
			tusMtr = tusMtrRightFb - tusMtr;
			idtcMtr = 6;
			while (( 0 < idtcMtr ) && ( rgtusMtr[idtcMtr] > tusMtr )) {
				idtcMtr--;
			}
			dtcMtr = dtcMtr - rgdtcMtr[idtcMtr]; // POSSIBLE CHANCE OF OVERFLOW	
		}
		else if ( tusMtr > tusMtrRightFb ) {
			// Right motor is running too slow.
			
			// Determine the new motor duty cycle.
			tusMtr	= tusMtr - tusMtrRightFb;
			idtcMtr	= 6;
			while (( 0 < idtcMtr ) && ( rgtusMtr[idtcMtr] > tusMtr )) {
				idtcMtr--;
			}
			dtcMtr = dtcMtr + rgdtcMtr[idtcMtr]; // POSSIBLE CHANCE OF OVERFLOW
		}
		
		if (( dtcMtrMin <= dtcMtr ) && ( dtcMtrMax >= dtcMtr )) {
			OC4RS = dtcMtr;
		}
	}
}

void __ISR(_TIMER_5_VECTOR, ipl7) _Timer5Handler(void)
{
	HWORD	tusMtr;
	HWORD	dtcMtr;
	BYTE	idtcMtr;
	
	fMtrLeft	= fTrue;
	tusMtr		= TMR5;
	TMR5		= 0;
	IFS0CLR		= ( 1 << 20 );
	
	if ( fMtrLeftFb ) {
		// Feedback is enabled for the left motor.
		
		dtcMtr = OC1R;	// get current motor duty cycle
		if ( tusMtr < tusMtrLeftFb ) {
			// Left motor is running too fast.
			
			// Determine new motor duty cycle.
			tusMtr = tusMtrLeftFb - tusMtr;
			idtcMtr = 6;
			while (( 0 < idtcMtr ) && ( rgtusMtr[idtcMtr] > tusMtr )) {
				idtcMtr--;
			}
			dtcMtr = dtcMtr - rgdtcMtr[idtcMtr]; // POSSIBLE CHANCE OF OVERFLOW	
		}
		else if ( tusMtr > tusMtrLeftFb ) {
			// Left motor is running too slow.
			
			// Determine the new motor duty cycle.
			tusMtr	= tusMtr - tusMtrRightFb;
			idtcMtr	= 6;
			while (( 0 < idtcMtr ) && ( rgtusMtr[idtcMtr] > tusMtr )) {
				idtcMtr--;
			}
			dtcMtr = dtcMtr + rgdtcMtr[idtcMtr]; // POSSIBLE CHANCE OF OVERFLOW
		}
		
		if (( dtcMtrMin <= dtcMtr ) && ( dtcMtrMax >= dtcMtr )) {
			OC1RS = dtcMtr;
		}
	}
}


/* ------------------------------------------------------------ */
/*				Procedure Definitions							*/
/* ------------------------------------------------------------ */
/***	UpdateMotorControl
**
**	Synopsis:
**		PutChUart1(ch)
**
**	Parameters:
**		posX - x position of the right joystick
**		posY - y position of the right joystick
**
**	Return Values:
**		none
**
**	Errors:
**		none
**
**	Description:
**		This routine will update the motor controller based on the
**		current X and Y position of the right joystick of the controller.
*/
void UpdateMotorControl( HWORD posX, HWORD posY )
{
	static	HWORD	tusMtrLeftDcSav		= dtcMtrStopped;	// stopped
	static	HWORD	tusMtrRightDcSav	= dtcMtrStopped;	// stopped
	static	BYTE	dirMtrLeftSav		= dirMtrLeftFwd;
	static	BYTE	dirMtrRightSav		= dirMtrRightFwd;
			HWORD	tusMtrLeftFbNew;
			HWORD	tusMtrRightFbNew;
	
	// Determine motor direction and speed based on joystick position.
	if ( posYMax < posY ) {
		if ( posXMax < posX ) {
			// Go forward and left.
			MtrCtrlFwdLeft();
		}
		else if ( posXMin > posX ) {
			// Go backward and left.
			MtrCtrlBwdLeft();
		}
		else {
			// Go left.
			MtrCtrlLeft();
		}
	}
	else if ( posYMin > posY ) {
		if ( posXMax < posX ) {
			// Go forward and right.
			MtrCtrlFwdRight();
		}
		else if ( posXMin > posX ) {
			// Go backward and right.
			MtrCtrlBwdRight();
		}
		else {
			// Go right.
			MtrCtrlRight();
		}
	}
	else {
		if ( posXMax < posX ) {
			// Go forward.
			MtrCtrlFwd();
		}
		else if ( posXMin > posX ) {
			// Go backward.
			MtrCtrlBwd();
		}
		else {
			MtrCtrlStop();
			// Stop.
		}
	}
	
	// Update left motor speed if necessary.
	if ( tusMtrLeftDcSav != dtcMtrLeft ) {
		
		// 1. disable left motor feedback.
		INTDisableInterrupts();
		fMtrLeftFb	= fFalse;
		INTEnableInterrupts();
		
		if ( dtcMtrStopped != dtcMtrLeft ) {
			// 3. update left motor feedback
			tusMtrLeftFb = tusMtrLeftFbNew;
			
			// 4. re-enable left motor feedback.
			INTDisableInterrupts();
			fMtrLeft	= fFalse;
			fMtrLeftFb	= fTrue;
			INTEnableInterrupts();
			
			// 5. enable feedback watchdog
			T1CONSET = ( 1 << 15 );
		}
		else {
			
			// 2. update left motor duty cycle.
			OC1RS = dtcMtrLeft;
			
			// Disable feedback watchdog.
			T1CONCLR 	= ( 1 << 15 );
			TMR1		= 0;
			IFS0CLR		= ( 1 << 4 );
		}
		
		tusMtrLeftDcSav = dtcMtrLeft;
	}
	
	// Update right motor speed if necessary.
	if ( tusMtrRightDcSav != dtcMtrRight ) {
		
		// 1. disable right motor feedback.
		INTDisableInterrupts();
		fMtrRightFb	= fFalse;
		INTEnableInterrupts();
		
		if ( dtcMtrStopped != dtcMtrRight ) {
			// 3. update right motor feedback.
			tusMtrRightFb = tusMtrRightFbNew;
			
			// 4. re-enable right motor feedback.
			INTDisableInterrupts();
			fMtrRight	= fFalse;
			fMtrRightFb	= fTrue;
			INTEnableInterrupts();
			
			// 5. enable feedback watchdog
			T1CONSET = ( 1 << 15 );
		}
		else {
			
			// 2. update right motor duty cycle.
			OC4RS = dtcMtrRight;
		}
		
		tusMtrRightDcSav = dtcMtrRight;
	}
	
	// Update left motor direction if necessary.
	if ( dirMtrLeftSav != dirMtrLeft ) {
		
		OC1CONCLR	= ( 1 << 15 );	// Disable output compare module 1
									// to release I/O pin control of enable
									// signal.
									
		trisMtrLeftEnClr	= ( 1 << bnMtrLeftEn );	// Set enable pin as a digital output.
		prtMtrLeftEnClr		= ( 1 << bnMtrLeftEn ); // Drive enable pin low.
									
		if ( dirMtrLeftFwd	== dirMtrLeft ) {
			prtMtrLeftDirSet = ( 1 << bnMtrLeftDir );
		}
		else {
			prtMtrLeftDirClr = ( 1 << bnMtrLeftDir );
		}
									
		OC1CONSET	= ( 1 << 15 );	// Re-enable output compare module 1.
		dirMtrLeftSav = dirMtrLeft;	// Save the current direction of the
									// left motor.
	}
	
	// Update right motor direction if necessary.
	if ( dirMtrRightSav != dirMtrRight ) {
		OC4CONCLR	= ( 1 << 15 );	// Disable output compare module 4
									// to release I/O pin control of enable
									// signal.
									
		trisMtrRightEnClr	= ( 1 << bnMtrRightEn ); // Set enable pin as a digital output.
		prtMtrRightEnClr	= ( 1 << bnMtrRightEn ); // Drive enable pin low.
		
		if ( dirMtrRightFwd == dirMtrRight ) {
			prtMtrRightDirClr = ( 1 << bnMtrRightDir );
		}
		else {
			prtMtrRightDirSet = ( 1 << bnMtrRightDir );
		}
									
		OC4CONSET	= ( 1 << 15 );	// Re-enable output compare module 4.
		
		dirMtrRightSav = dirMtrRight;	// Save the current direciton of the
										// right motor.
	}
}


/*************************************************************************************/
