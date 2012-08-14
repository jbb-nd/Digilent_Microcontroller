/************************************************************************/
/*																		*/
/*	HB_MotorLib.h -- Declarations used for HB_Motor functions	       	*/
/*																		*/
/************************************************************************/
/*	Author: Cristian Fatu												*/
/*	Copyright 2011, Digilent Inc.										*/
/************************************************************************/
/*  Module Description: 												*/
/*																		*/
/*	This header contains declarations for common functions needed to 	*/
/*	manage a DC motor using HBx PMODS.									*/
/*	The included file HB_MotorLib_Defs.h contains definitions 			*/
/*  "#define _MOTORx" according to motors configuration.				*/
/*																		*/
/************************************************************************/
/*  Revision History:						                			*/
/*											                        	*/
/*	06/04/2011 (CristianF): created			                			*/
/*											                        	*/
/************************************************************************/

#if !defined(_HB_MOTORLIB_INC)
#define	_HB_MOTORLIB_INC

#include "stdtypes.h"
#include "HB_MotorLib_Defs.h"
/* ------------------------------------------------------------ */
/*					     Pin Definitions   		    			*/
/* ------------------------------------------------------------ */

/* OC Summary:
JH-08 	SDO1/OC1/INT0/RD0
JD-02 	OC2/RD1
JD-08	OC3/RD2
JE-08 	OC4/RD3

There will be separate definitions corresponding to PMODs where OC exists fis to EN position on HB5 PMOD.

*/


//	Motor 1 - HB PMOD in JH 7-10

#define	trisMtr1DirSet	TRISESET
#define	trisMtr1DirClr	TRISECLR
#define	prtMtr1DirSet	PORTESET
#define	prtMtr1DirClr	PORTECLR
#define	bnMtr1Dir		8			// JH-07 INT1/RE8


#define	trisMtr1EnSet	TRISDSET
#define	trisMtr1EnClr	TRISDCLR
#define	prtMtr1EnSet	PORTDSET	
#define	prtMtr1EnClr	PORTDCLR
#define	bnMtr1En		0			// JH-08 SDO1/OC1/INT0/RD0


#define	trisMtr1SASet	TRISDSET
#define	trisMtr1SAClr	TRISDCLR
#define	prtMtr1SA		PORTD
#define	prtMtr1SASet	PORTDSET
#define	prtMtr1SAClr	PORTDCLR
#define	bnMtr1SA		8			// JH-09 IC1/RTCC/RD8

#define	trisMtr1SBSet	TRISESET
#define	trisMtr1SBClr	TRISECLR
#define	prtMtr1SB		PORTE
#define	prtMtr1SBSet	PORTESET
#define	prtMtr1SBClr	PORTECLR
#define	bnMtr1SB		9			// JH-10 INT2/RE9

//	Motor 2 - HB PMOD in JD 1-7
	
#define	trisMtr2DirSet	TRISDSDT
#define	trisMtr2DirClr	TRISDCLR
#define	prtMtr2DirSet	PORTDSET
#define	prtMtr2DirClr	PORTDCLR
#define	bnMtr2Dir		7			// JD-01 PMD15/CN16/RD7


#define	trisMtr2EnSet	TRISDSET
#define	trisMtr2EnClr	TRISDCLR
#define	prtMtr2EnSet	PORTDSET	
#define	prtMtr2EnClr	PORTDCLR
#define	bnMtr2En		1			// JD-02 OC2/RD1


#define	trisMtr2SASet	TRISDSET
#define	trisMtr2SAClr	TRISDCLR
#define	prtMtr2SA		PORTD
#define	prtMtr2SASet	PORTDSET
#define	prtMtr2SAClr	PORTDCLR
#define	bnMtr2SA		9			// JD-03 IC2/SS1/RD9

#define	trisMtr2SBSet	TRISCSET
#define	trisMtr2SBClr	TRISCCLR
#define	prtMtr2SB		PORTC
#define	prtMtr2SBSet	PORTCSCT
#define	prtMtr2SBClr	PORTCCLR
#define	bnMtr2SB		1			// JD-04 T2CK/RC1


	//	Motor 3 - HB PMOD in JD 7-10
	
#define	trisMtr3DirSet	TRISDSDT
#define	trisMtr3DirClr	TRISDCLR
#define	prtMtr3DirSet	PORTDSET
#define	prtMtr3DirClr	PORTDCLR
#define	bnMtr3Dir		6			// JD-07 PMD14/CN15/RD6


#define	trisMtr3EnSet	TRISDSET
#define	trisMtr3EnClr	TRISDCLR
#define	prtMtr3EnSet	PORTDSET	
#define	prtMtr3EnClr	PORTDCLR
#define	bnMtr3En		2			// JD-08 OC3/RD2


#define	trisMtr3SASet	TRISDSET
#define	trisMtr3SAClr	TRISDCLR
#define	prtMtr3SA		PORTD
#define	prtMtr3SASet	PORTDSET
#define	prtMtr3SAClr	PORTDCLR
#define	bnMtr3SA		10			// JD-09 IC3/SCK1/PMCS2/PMA15/RD10

#define	trisMtr3SBSet	TRISCSET
#define	trisMtr3SBClr	TRISCCLR
#define	prtMtr3SB		PORTC
#define	prtMtr3SBSet	PORTCSCT
#define	prtMtr3SBClr	PORTCCLR
#define	bnMtr3SB		2			// JD-10 T3CK/RC2

//	Motor 4 - HB PMOD in JE 7-10
	
	
#define	trisMtr4DirSet	TRISDSDT
#define	trisMtr4DirClr	TRISDCLR
#define	prtMtr4DirSet	PORTDSET
#define	prtMtr4DirClr	PORTDCLR
#define	bnMtr4Dir		13			// JE-07 PMD13/CN19/RD13


#define	trisMtr4EnSet	TRISDSET
#define	trisMtr4EnClr	TRISDCLR
#define	prtMtr4EnSet	PORTDSET	
#define	prtMtr4EnClr	PORTDCLR
#define	bnMtr4En		3			// JE-08 OC4/RD3


#define	trisMtr4SASet	TRISDSET
#define	trisMtr4SAClr	TRISDCLR
#define	prtMtr4SA		PORTD
#define	prtMtr4SASet	PORTDSET
#define	prtMtr4SAClr	PORTDCLR
#define	bnMtr4SA		11			// JE-09 IC4/PMCS1/PMA14/RD11

#define	trisMtr4SBSet	TRISCSET
#define	trisMtr4SBClr	TRISCCLR
#define	prtMtr4SB		PORTC
#define	prtMtr4SBSet	PORTCSCT
#define	prtMtr4SBClr	PORTCCLR
#define	bnMtr4SB		3			// JE-10 T4CK/RC3


/* ------------------------------------------------------------ */
/*				    	Macro Declarations			    		*/
/* ------------------------------------------------------------ */


#define mIncrementRealTime cntRealTime++;

/* ------------------------------------------------------------ */
/*					General Type Declarations					*/
/* ------------------------------------------------------------ */

typedef struct _DCMotorReactionContext
{
	BOOL fDir;		// read motor direction, filled by ReactionDCMotorx function when called from IC ISR
	WORD wRotationCounter;	// SA transitions counter, incremented or decremented by ReactionDCMotorx function when called from IC ISR
	WORD wSavedRealTime;	// saved real time - saved by ReactionDCMotorx function value when the speed was last read
	WORD wSavedRotationCounter;		// saved rotation counter   - saved by ReactionDCMotorx function value when the speed was last read
	float dComputedSpeedRpm;	// the computed speed in RPM   - computed by ReactionDCMotorx function value when the speed was last read
}	DCMotorReactionContext;
	

/* ------------------------------------------------------------ */
/*					Object Class Declarations					*/
/* ------------------------------------------------------------ */



/* ------------------------------------------------------------ */
/*					Variable Declarations						*/
/* ------------------------------------------------------------ */
extern WORD	cntRealTime;
// this variable must be visible outside HB_MotorLib.c so that real time counter could be incremented by user
#define cntPulsePWMMax  0xFFFF
#define cntUsForceLowEn	1000

#ifdef _MOTOR1
	#define reductionMotor1	19.0
#endif //_MOTOR1

#ifdef _MOTOR2
	#define reductionMotor2	19.0
#endif //_MOTOR2

#ifdef _MOTOR3
	#define reductionMotor3	19.0
#endif //_MOTOR3

#ifdef _MOTOR4
	#define reductionMotor4	19.0
#endif //_MOTOR4


/* ------------------------------------------------------------ */
/*					Procedure Declarations						*/
/* ------------------------------------------------------------ */
#ifdef _MOTOR1

void HB_MotorConfigure1(double dDutySubunit, BOOL fOnlyPWM);
void HB_MotorCommand1( double dDutySubunit, BOOL fDir);
void HB_MotorReaction1(DCMotorReactionContext *pDCMotorReactionContext, double dDuty);
#endif //_MOTOR1

#ifdef _MOTOR2

void HB_MotorConfigure2(double dDutySubunit, BOOL fOnlyPWM);
void HB_MotorCommand2( double dDutySubunit, BOOL fDir);
void HB_MotorReaction2(DCMotorReactionContext *pDCMotorReactionContext, double dDuty);
#endif //_MOTOR2

#ifdef _MOTOR3

void HB_MotorConfigure3(double dDutySubunit, BOOL fOnlyPWM);
void HB_MotorCommand3( double dDutySubunit, BOOL fDir);
void HB_MotorReaction3(DCMotorReactionContext *pDCMotorReactionContext, double dDuty);
#endif //_MOTOR3

#ifdef _MOTOR4

void HB_MotorConfigure4(double dDutySubunit, BOOL fOnlyPWM);
void HB_MotorCommand4( double dDutySubunit, BOOL fDir);
void HB_MotorReaction4(DCMotorReactionContext *pDCMotorReactionContext, double dDuty);
#endif //_MOTOR4


#endif // _HB_MOTORLIB_INC
