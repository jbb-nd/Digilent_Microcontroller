/************************************************************************/
/*										                        		*/
/*	main.c	-- HBMotor Demo									            */
/*											                        	*/
/*											                        	*/
/************************************************************************/
/*	Author: Cristian Fatu												*/
/*	Copyright 2011, Digilent Inc.										*/
/************************************************************************/
/*  Module Description: 						                		*/
/*											                        	*/
/*	This module implements a simple demo application that demonstrates  */
/*	how to use HB_Motor and CLP functions. 								*/
/*	It also uses buttons and on-board leds.								*/
/*																		*/
/*	Functionality:														*/
/*																		*/
/* The main loop contains a loop procesing buttons actions and display.	*/
/* Buttons are read and debounced at a rate of 100 us, using Timer5.	*/
/* Using buttons, duty is raised and lowered (single button), or 		*/
/* direction is changed (double buttons).								*/
/*  This file includes clp_config.h (defining the pins corresponding to */
/*  CLP, to be edited accordingly) and clp.h (CLP functions definitions)*/
/*  In order to use other motors, define them in HB_MotorDemo.h.                                                                    */
/*	Required Hardware:													*/
/*		1. Cerebot 32MX4												*/
/*		2. PmodCLP - plugged into JA (J1) and JB (J2)					*/
/*		3. Pmod HB5 - plugged into JH (Motor1)							*/
/* Resources used:														*/
/*	1. Timer5 - for button reading and debouncing - programmed at 100 us*/
/*	2. Timer2 - used for Input Capture and Output Compare modules.		*/
/* 				- programmed at 8.182 ms								*/ 
/*	3. Input Capture 1 - for reading direction and motor speed.			*/
/* 				< the other motors use corresponding IC2, IC3, IC4 >	*/ 
/*	4. Output Compare 1 - for generating PWM.							*/
/* 				< the other motors use corresponding OC2, OC3, OC4 >	*/ 
/* 	Configuring the timers and modules is done using PLib macros. 		*/ 
/*	Still, some examples of SFRs approach are shown in the commented 	*/ 
/*	code that follows.													*/
/************************************************************************/
/*  Revision History:						                			*/
/*											                        	*/
/*	06/04/2011 (CristianF): created			                			*/
/*											                        	*/
/************************************************************************/

/* ------------------------------------------------------------ */
/*				Include File Definitions	            	    */
/* ------------------------------------------------------------ */

#include <plib.h>
#include "stdtypes.h"
#include "config.h"
#include "util.h"
#include "HB_MotorLib.h"
#include "ClpLib.h"


/* ------------------------------------------------------------ */
/*						Configuration Bits		                */
/* ------------------------------------------------------------ */

#ifndef OVERRIDE_CONFIG_BITS

#pragma config ICESEL   = ICS_PGx2		// ICE/ICD Comm Channel Select
#pragma config BWP      = OFF			// Boot Flash Write Protect
#pragma config CP       = OFF			// Code Protect
#pragma config FNOSC    = PRIPLL		// Oscillator Selection
#pragma config FSOSCEN  = OFF			// Secondary Oscillator Enable
#pragma config IESO     = OFF			// Internal/External Switch-over
#pragma config POSCMOD  = HS			// Primary Oscillator
#pragma config OSCIOFNC = OFF			// CLKO Enable
#pragma config FPBDIV   = DIV_8			// Peripheral Clock divisor
#pragma config FCKSM    = CSDCMD		// Clock Switching & Fail Safe Clock Monitor
#pragma config WDTPS    = PS1			// Watchdog Timer Postscale
#pragma config FWDTEN   = OFF			// Watchdog Timer 
#pragma config FPLLIDIV = DIV_2			// PLL Input Divider
#pragma config FPLLMUL  = MUL_16		// PLL Multiplier
#pragma config UPLLIDIV = DIV_2			// USB PLL Input Divider
#pragma config UPLLEN   = OFF			// USB PLL Enabled
#pragma config FPLLODIV = DIV_1			// PLL Output Divider
#pragma config PWP      = OFF			// Program Flash Write Protect
#pragma config DEBUG    = ON			// Debugger Enable/Disable
// finnaly SYSCLK = 8MHz / 2 * 16  = 64 MHz    
#endif


/* ------------------------------------------------------------ */
/*				Local Type Definitions			                */
/* ------------------------------------------------------------ */

#define	stPressed	1
#define	stReleased	0

#define cntPWM
#define	cstMaxCnt	10 // number of consecutive reads required for
					   // the state of a button to be updated
#define	cstSameStBtnMaxCnt	6000 // number of consecutive reads of the same stBtn state required for 
					   // the button to be processed again in the same state

struct btn {
	BYTE	stBtn;	// status of the button (pressed or released)
	BYTE	stCur;  // current read state of the button
	BYTE	stPrev; // previous read state of the button
	BYTE	cst;	// number of consecutive reads of the same button 
	HWORD	cstSameStBtn;	// number of consecutive reads of the same stBtn state.
	HWORD	cstActionLed;	// number of reads until corresponding led will be turned off.
	BOOL 	fProcess;	// true if button needs processing
	BOOL	fReleasedOnDouble;	// true if the release of the button was made while the other btn was pressed
};

/* ------------------------------------------------------------ */
/*				Global Variables				                */
/* ------------------------------------------------------------ */

volatile	struct btn	btnBtn1;
volatile	struct btn	btnBtn2;
volatile	BOOL		fDisplayNeeded;			// flag for display needed


/* ------------------------------------------------------------ */
/*				Local Variables				                    */
/* ------------------------------------------------------------ */


#define cntLedAction  1000;		// counter for led display - 100 ms (1000*100 us)
#define ccntDisplayDelay 2000;	// counter for display - 200 ms (2000*100 us)


/* ------------------------------------------------------------ */
/*				Macros											*/
/* ------------------------------------------------------------ */



/* ------------------------------------------------------------ */
/*				Forward Declarations			                */
/* ------------------------------------------------------------ */

void	DeviceInit( void );
void	AppInit( void );
void 	Display(double dDuty);


/* ------------------------------------------------------------ */
/*				Interrupt Service Routines	            	    */
/* ------------------------------------------------------------ */
/***	Timer5Handler
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
**		Interrupt service routine for Timer 5 interrupt.
**		Programmed to execute every 100 us.
**		It is used to:
**		1. 	to perform software debouncing of the on-board buttons and detect:
				- single button situation: when a button should be processed (being pressed or being kept pressed)
				- double buttons situation: when a button is released while the other is pressed 
**		2.  to increment the real time counter
**		3. 	as a time base for clearing the action leds (LD3 and LD2) 
**		4. 	as a time base for updating the display. 
*/

void __ISR(_TIMER_5_VECTOR, ipl7) Timer5Handler(void)
{
	static WORD oldDisplayRealCounter = 0;
	static WORD cntDisplayDelay = ccntDisplayDelay;
	mT5ClearIntFlag();
	
	// Read the raw state of the button pins.
	btnBtn1.stCur = ( prtBtn1 & ( 1 << bnBtn1 ) ) ? stPressed : stReleased;
	btnBtn2.stCur = ( prtBtn2 & ( 1 << bnBtn2 ) ) ? stPressed : stReleased;
	
	// Update state counts.
	btnBtn1.cst = ( btnBtn1.stCur == btnBtn1.stPrev ) ? btnBtn1.cst + 1 : 0;
	btnBtn2.cst = ( btnBtn2.stCur == btnBtn2.stPrev ) ? btnBtn2.cst + 1 : 0;
	

	// Save the current state.
	btnBtn1.stPrev = btnBtn1.stCur;
	btnBtn2.stPrev = btnBtn2.stCur;
	if(btnBtn1.cstActionLed > 0)
	{
		if(--btnBtn1.cstActionLed == 0)
		{
			prtLed2Clr = ( 1 << bnLed2 ); // Clear LD2, avoid call to SetLeds
		}
	}

	if(btnBtn2.cstActionLed > 0)
	{
		if(--btnBtn2.cstActionLed == 0)
		{
			prtLed3Clr = ( 1 << bnLed3 ); // Clear LD3, avoid call to SetLeds
		}
	}

	// Update the state of button 1 if necessary.
	if ( cstMaxCnt == btnBtn1.cst && btnBtn1.stBtn != btnBtn1.stCur) {
		if(btnBtn1.stBtn == stPressed && btnBtn2.stPrev == stPressed)
		{
			btnBtn1.fReleasedOnDouble = fTrue;
		}
		btnBtn1.stBtn = btnBtn1.stCur;
		btnBtn1.cst = 0;
		btnBtn1.cstSameStBtn = 0;
		btnBtn1.fProcess = (btnBtn1.stBtn == stPressed);	// meaning that the button can be processed in this state

	}
	else
	{
		if(btnBtn1.stBtn == btnBtn1.stCur)
		{
			btnBtn1.cstSameStBtn = btnBtn1.cstSameStBtn + 1;

			if(cstSameStBtnMaxCnt == btnBtn1.cstSameStBtn)
			{
				if(btnBtn1.stBtn == stPressed)
				{
					btnBtn1.fProcess = fTrue; // meaning that the button can be processed once again in this state
				}
				btnBtn1.cstSameStBtn = 0; // restart counter of the same state button	
			}
		}
	}
	// Update the state of button 2 if necessary.
	if ( cstMaxCnt == btnBtn2.cst && btnBtn2.stBtn != btnBtn2.stCur) {
		if(btnBtn2.stBtn == stPressed && btnBtn1.stPrev == stPressed)
		{
			btnBtn2.fReleasedOnDouble = fTrue;
		}
		btnBtn2.stBtn = btnBtn2.stCur;
		btnBtn2.cst = 0;
		btnBtn2.cstSameStBtn = 0;
		btnBtn2.fProcess = (btnBtn2.stBtn == stPressed);	// meaning that the button can be processed in this state

	}
	else
	{
		if(btnBtn2.stBtn == btnBtn2.stCur)
		{
			btnBtn2.cstSameStBtn = btnBtn2.cstSameStBtn + 1;

			if(cstSameStBtnMaxCnt == btnBtn2.cstSameStBtn)
			{
				if(btnBtn2.stBtn == stPressed)
				{
					btnBtn2.fProcess = fTrue; // meaning that the button can be processed once again in this state
				}
				btnBtn2.cstSameStBtn = 0; // restart counter of the same state button	
			}
		}
	}
	// increments real time counter
	// make sure that you define REALTIME_CALLEDBYUSER when you are incrementing the real time cpunter from outside the HB_Motor module
	mIncrementRealTime;
	// deal with display
	if(--cntDisplayDelay == 0)
	{
		fDisplayNeeded = fTrue;
		cntDisplayDelay = ccntDisplayDelay;
	}
}



/* ------------------------------------------------------------ */
/*				Procedure Definitions			                */
/* ------------------------------------------------------------ */
/***	main
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
**		Main program module. Performs basic board initialization
**		and then enters the main program loop.
*/

int main( void )
{	
	BYTE	stBtn1;
	BYTE	stBtn2;
	BOOL 	fUpdLeds = fFalse;
	float dDuty = 0.5;
	float dDutyStep = 0.01;
	DeviceInit();
	AppInit();
	
	HB_MotorConfigure1(dDuty, fFalse);
	ClpInit(displaySetOptionDisplayOn);
	ClpSetBackLight(fTrue);

	while ( fTrue ) {
		
		// Get the current internal state of the buttons.
		INTDisableInterrupts();
		stBtn1 = btnBtn1.stBtn;
		stBtn2 = btnBtn2.stBtn;
		INTEnableInterrupts();
		if (( btnBtn1.fReleasedOnDouble == fTrue ) || ( btnBtn2.fReleasedOnDouble == fTrue ))  
		{ // both buttons pressed, reverse motor direction and maintain duty
			dDuty = -dDuty;
			HB_MotorCommand1(dDuty > 0 ? dDuty: -dDuty, dDuty < 0);  
			btnBtn1.fProcess = fFalse;
			btnBtn2.fProcess = fFalse;
			btnBtn1.fReleasedOnDouble = fFalse;
			btnBtn2.fReleasedOnDouble = fFalse;
		}
		if (( stPressed == stBtn2 ) && ( btnBtn2.fProcess == fTrue )) 
		{
			dDuty += dDutyStep;
			if(dDuty >  1)
			{
				dDuty = 1;
			}

			HB_MotorCommand1(dDuty > 0 ? dDuty: -dDuty, dDuty < 0); 
			prtLed3Set = ( 1 << bnLed3 ); // turn on led 3, avoid call to SetLeds

			btnBtn2.cstActionLed = cntLedAction;
			btnBtn2.fProcess = fFalse;
		}
		else
		{
			if (( stPressed == stBtn1 ) && ( btnBtn1.fProcess == fTrue )) 
			{
				dDuty -= dDutyStep;
				if(dDuty <= -1)
				{
					dDuty = -1;
				}
			
				HB_MotorCommand1(dDuty > 0 ? dDuty: -dDuty, dDuty < 0); 
				prtLed2Set = ( 1 << bnLed2 ); // turn on led 2, avoid call to SetLeds
				btnBtn1.cstActionLed = cntLedAction;
				btnBtn1.fProcess = fFalse;
			}
		}
		if(dDuty >=  1)
		{
			prtLed4Set = ( 1 << bnLed4 ); // turn on led 4, avoid call to SetLeds
		}
		else
		{
			prtLed4Clr = ( 1 << bnLed4 ); // turn off led 4, avoid call to SetLeds
		}
		if(dDuty <= -1)
		{
			prtLed1Set = ( 1 << bnLed1 ); // turn on led 1, avoid call to SetLeds
		}
		else
		{
			prtLed1Clr = ( 1 << bnLed1 ); // turn off led 1, avoid call to SetLeds
		}
		
		// check if display must be performed
		if(fDisplayNeeded == fTrue)
		{
			// compute motor speed
			Display(dDuty);
			
			fDisplayNeeded = fFalse;
		}	
	}
}


/* ------------------------------------------------------------ */
/***	DeviceInit
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
**		This routine initializes on-chip and on-board peripheral
**		devices to their default state.
*/

void DeviceInit( void )
{
	// Initialize the on-board LEDs.
	InitLeds();
	
	// Configure onboard buttons as inputs.
	trisBtn1Set = ( 1 << bnBtn1 );
	trisBtn2Set = ( 1 << bnBtn2 );

	// configure CLP command pins ss inputs
	ClpPinsConfigure();
}



/* ------------------------------------------------------------ */
/***	AppInit
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
**		This routine performs application specific initialization.
**		It configures devices and global variables for the application.
*/

void AppInit( void )
{
	// Initialize the state of button 1.
	btnBtn1.stBtn 	= stReleased;
	btnBtn1.stCur 	= stReleased;
	btnBtn1.stPrev 	= stReleased;
	btnBtn1.cst		= 0;
	
	// Initialize the state of button 2.
	btnBtn2.stBtn 	= stReleased;
	btnBtn2.stCur 	= stReleased;
	btnBtn2.stPrev 	= stReleased;
	btnBtn2.cst		= 0;
	
	
	// Configure Timer 5. Used for buttons debouncing.
	// STEP 1. configure the Timer5
		// Timer Input Clock prescaler 1/8, so the frequency is 1/8 of Periferal bus freq, which is 1/8 of SYSCLK (64 MHz) = 1 MHz.
		// Period = 99, correspondding to 100 us.
	OpenTimer5(T5_ON | T5_SOURCE_INT | T5_PS_1_8, 99);
	
	// STEP 2. set the timer interrupt priority level 7, sub 3
	ConfigIntTimer5(T5_INT_ON | T5_INT_PRIOR_7 | T5_INT_SUB_PRIOR_3);
/*
	// this approach configures the same settings whithout the PLib macros, dirrectly to SFRs.
	TMR5	= 0;
	PR5		= 99; // period match every 100 us
	IPC5SET	= ( 1 << 4 ) | ( 1 << 3 ) | ( 1 << 2 ) | ( 1 << 1 ) | ( 1 << 0 ); // interrupt priority level 7, sub 3
	IFS0CLR = (1 << 20);
	IEC0SET	= (1 << 20);

	// Start timer.
	T5CON = ( 1 << 15 ) | ( 1 << 5 ) | ( 1 << 4 ); // fTimer5 = fPb / 8
*/	

	// Enable multi-vector interrupts.
	INTEnableSystemMultiVectoredInt();
}


/* ------------------------------------------------------------ */
/*        Display
**
**        Synopsis:
**
**        Parameters:
**				double dDuty - the applied duty (between -1 and 1).
**					values from 0 to 1 correspond to positive direction
**					values from -1 to 0 correspond to negative direction	
**
**
**        Return Values:
**                void 
**
**        Errors:
**
**
**        Description:
**				The function reads the DCMotorReactionContext structure containing motor information.
**				It display the applied duty and the RPM speed.
**
**
*/
void Display(double dDuty)
{
	char szInfo1[30], szInfo2[30];
	DCMotorReactionContext dDCMotorReactionContext;
	HB_MotorReaction1(&dDCMotorReactionContext, dDuty);
	sprintf(szInfo1, "Duty: %7.3f", dDuty);
//	sprintf(szInfo1, "Cnt: %d", dDCMotorReactionContext.wCounter);
	sprintf(szInfo2, "RPM: %8.3f ", dDCMotorReactionContext.dComputedSpeedRpm);
	ClpWriteStringAtPos(szInfo1, 0, 0);
	ClpWriteStringAtPos(szInfo2, 1, 0);
}


/*************************************************************************************/
