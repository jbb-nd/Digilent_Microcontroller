/************************************************************************/
/*																		*/
/*	MtrCtrl.h	--  Motor Control Routines				                */
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

#if !defined(_MTRCTRL_INC)
#define _MTRCTRL_INC

#include "config.h"
#include "stdtypes.h"

/* ------------------------------------------------------------ */
/*					General Type Declarations					*/
/* ------------------------------------------------------------ */

/*	Motor duty cycles.
*/
#define dtcMtrMin		50
#define dtcMtrMax		1950

#define	dtcMtrSlow		8000
#define	dtcMtrMedium	4000
#define	dtcMtrFast		2500
#define	dtcMtrStopped	0

/*	Motor feedback timing.
*/
#define	tusMtrFbSlow	8000
#define	tusMtrFbMedium	4000
#define	tusMtrFbFast	2500   

/*	Motor direction.
*/
#define	dirMtrLeftFwd	1
#define	dirMtrLeftBwd	0
#define	dirMtrRightFwd	0
#define	dirMtrRightBwd	1

/* ------------------------------------------------------------ */
/*					Variable Declarations						*/
/* ------------------------------------------------------------ */

/*	Motor control variables.
*/
volatile	HWORD	tusMtrLeftFb;
volatile	HWORD	tusMtrRightFb;
volatile	BOOL	fMtrLeftFb;
volatile	BOOL	fMtrRightFb;
volatile	BOOL	fMtrLeft;
volatile	BOOL	fMtrRight;
			HWORD	dtcMtrLeft;
			HWORD	dtcMtrRight;
			BYTE	dirMtrLeft;
			BYTE	dirMtrRight;

/* ------------------------------------------------------------ */
/*					        Macros		        				*/
/* ------------------------------------------------------------ */

#define MtrCtrlFwd()		dirMtrLeft			= dirMtrLeftFwd;\
							dirMtrRight			= dirMtrRightFwd;\
							dtcMtrLeft			= dtcMtrFast;\
							dtcMtrRight			= dtcMtrFast;\
							tusMtrLeftFbNew		= tusMtrFbFast;\
							tusMtrRightFbNew	= tusMtrFbFast
							
#define MtrCtrlFwdLeft()	dirMtrLeft			= dirMtrLeftFwd;\
							dirMtrRight			= dirMtrRightFwd;\
							dtcMtrLeft			= dtcMtrMedium;\
							dtcMtrRight			= dtcMtrFast;\
							tusMtrLeftFbNew		= tusMtrFbMedium;\
							tusMtrRightFbNew	= tusMtrFbFast
						
#define MtrCtrlFwdRight()	dirMtrLeft			= dirMtrLeftFwd;\
							dirMtrRight			= dirMtrRightFwd;\
							dtcMtrLeft			= dtcMtrFast;\
							dtcMtrRight			= dtcMtrMedium;\
							tusMtrLeftFbNew		= tusMtrFbFast;\
							tusMtrRightFbNew	= tusMtrFbMedium
							
#define MtrCtrlBwd()		dirMtrLeft			= dirMtrLeftBwd;\
							dirMtrRight			= dirMtrRightBwd;\
							dtcMtrLeft			= dtcMtrFast;\
							dtcMtrRight			= dtcMtrFast;\
							tusMtrLeftFbNew		= tusMtrFbFast;\
							tusMtrRightFbNew	= tusMtrFbFast
							
#define MtrCtrlBwdLeft()	dirMtrLeft			= dirMtrLeftBwd;\
							dirMtrRight			= dirMtrRightBwd;\
							dtcMtrLeft			= dtcMtrMedium;\
							dtcMtrRight			= dtcMtrFast;\
							tusMtrLeftFbNew		= tusMtrFbMedium;\
							tusMtrRightFbNew	= tusMtrFbFast
						
#define MtrCtrlBwdRight()	dirMtrLeft			= dirMtrLeftBwd;\
							dirMtrRight			= dirMtrRightBwd;\
							dtcMtrLeft			= dtcMtrFast;\
							dtcMtrRight			= dtcMtrMedium;\
							tusMtrLeftFbNew		= tusMtrFbFast;\
							tusMtrRightFbNew	= tusMtrFbMedium
							
#define MtrCtrlLeft()		dirMtrLeft			= dirMtrLeftBwd;\
							dirMtrRight			= dirMtrRightFwd;\
							dtcMtrLeft			= dtcMtrSlow;\
							dtcMtrRight			= dtcMtrSlow;\
							tusMtrLeftFbNew		= tusMtrFbSlow;\
							tusMtrRightFbNew	= tusMtrFbSlow
							
#define MtrCtrlRight()		dirMtrLeft			= dirMtrLeftFwd;\
							dirMtrRight			= dirMtrRightBwd;\
							dtcMtrLeft			= dtcMtrSlow;\
							dtcMtrRight			= dtcMtrSlow;\
							tusMtrLeftFbNew		= tusMtrFbSlow;\
							tusMtrRightFbNew	= tusMtrFbSlow
							
#define	MtrCtrlStop()		dtcMtrLeft			= dtcMtrStopped;\
							dtcMtrRight			= dtcMtrStopped


/* ------------------------------------------------------------ */
/*					Procedure Declarations						*/
/* ------------------------------------------------------------ */

void UpdateMotorControl( HWORD posX, HWORD posY );

/* ------------------------------------------------------------ */

#endif

/************************************************************************/
