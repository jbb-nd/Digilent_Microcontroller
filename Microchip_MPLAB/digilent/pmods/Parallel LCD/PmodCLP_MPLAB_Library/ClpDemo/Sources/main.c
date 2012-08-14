/************************************************************************/
/*										                        		*/
/*	main.c	-- LED Demo									                */
/*											                        	*/
/*											                        	*/
/************************************************************************/
/*	Author: Cristian Fatu												*/
/*	Copyright 2011, Digilent Inc.										*/
/************************************************************************/
/*	This module implements a simple demo application that demonstrates  */
/*	to manage CLP functions. It also shows how to debounce the on-board */
/*	buttons.															*/
/*																		*/
/*	Functionality:														*/
/*																		*/
/* The main loop contains a set of steps, each demonstrating a specific	*/
/* CLP feature. Buttons are read so that specific actions are performed.*/
/* Buttons are read and debounced at a rate of 100 us, using Timer5.	*/
/* Three actions are detected by reading buttons:						*/
/* 	- BTN1 is pressed													*/
/* 	- BTN2 is pressed													*/
/* 	- double buttons: one button is released while the other is pressed.*/
/* This is a demo of the usage of the CLP functions, that are defined in*/
/* ClpLib.h, ClpLib_config.h  and ClpLib.c files. 						*/
/*                                                                      */
/*	Required Hardware:													*/
/*		1. Cerebot 32MX4												*/
/*		2. PmodCLP - plugged into JA (J1) and JB (J2) (default position)*/
/* Resources used:														*/
/*	1. Timer5 - for button reading and debouncing - programmed at 100 us*/
/* Configuring the Timer5 is done using PLib macros. Still, SFRs 		*/ 
/* approach is shown in the commented code that follows.				*/
/*                                                                      */
/************************************************************************/
/*  Revision History:						                			*/
/*											                        	*/
/*	10/15/2011 (CristianF): created			                			*/
/*											                        	*/
/************************************************************************/

/* ------------------------------------------------------------ */
/*				Include File Definitions	            	    */
/* ------------------------------------------------------------ */

#include <plib.h>
#include "stdtypes.h"
#include "config.h"
#include "util.h"
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
#pragma config DEBUG    = OFF			// Debugger Enable/Disable
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

struct btn {
	BYTE	stBtn;	// status of the button (pressed or released)
	BYTE	stCur;  // current read state of the button
	BYTE	stPrev; // previous read state of the button
	BYTE	cst;	// number of consecutive reads of the same button state
	BOOL 	fProcess;	// true if button needs processing
	BOOL	fReleasedOnDouble;	// true if the release of the button was made while the other btn was pressed
};

/* ------------------------------------------------------------ */
/*				Global Variables				                */
/* ------------------------------------------------------------ */

volatile	struct btn	btnBtn1;
volatile	struct btn	btnBtn2;

/* ------------------------------------------------------------ */
/*				Local Variables				                    */
/* ------------------------------------------------------------ */



/* ------------------------------------------------------------ */
/*				Macros											*/
/* ------------------------------------------------------------ */



/* ------------------------------------------------------------ */
/*				Forward Declarations			                */
/* ------------------------------------------------------------ */

void	DeviceInit( void );
void	AppInit( void );
void WaitUntilBtnPressed(BOOL *pfBtn1Process, BOOL *pfBtn2Process);

#define cntLedAction  1000;	// 100 ms (1000*100 us)

BOOL fDisplayNeeded;
#define ccntDisplayDelay 2000;	// 200 ms (2000*100 us)

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
**		Interrupt service routine for Timer 5 interrupt. Timer 5
**		is used to perform software debouncing of the on-board
**		buttons. 
** 	Using btnBtn1 and btnBtn2 global structure variables, fields are set so that three actions are detected by reading buttons:						
** 		- BTN1 is pressed - when ( stPressed == stBtn1 ) && ( btnBtn1.cst == 0 )											
** 		- BTN2 is pressed - when ( stPressed == stBtn2 ) && ( btnBtn2.cst == 0 )											
** 		- double buttons: one button is released while the other is pressed. - when ( btnBtn1.fReleasedOnDouble == fTrue ) || ( btnBtn2.fReleasedOnDouble == fTrue )
*/

void __ISR(_TIMER_5_VECTOR, ipl7) Timer5Handler(void)
{
	static	WORD tusLeds = 0;
	
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
	
	// Update the state of button 1 if necessary.
	if ( cstMaxCnt == btnBtn1.cst && btnBtn1.stBtn != btnBtn1.stCur) {
		if(btnBtn1.stBtn == stPressed && btnBtn1.stCur == stReleased && btnBtn2.stPrev == stPressed)
		{
			btnBtn1.fReleasedOnDouble = fTrue;
		}
		btnBtn1.stBtn = btnBtn1.stCur;
		btnBtn1.cst = 0;
		btnBtn1.fProcess = (btnBtn1.stBtn == stPressed);	// meaning that the button can be processed in this state
	}
	
	// Update the state of button 2 if necessary.
	if ( cstMaxCnt == btnBtn2.cst && btnBtn2.stBtn != btnBtn2.stCur) {
		if(btnBtn2.stBtn == stPressed && btnBtn2.stCur == stReleased && btnBtn1.stPrev == stPressed)
		{
			btnBtn2.fReleasedOnDouble = fTrue;
		}
		btnBtn2.stBtn = btnBtn2.stCur;
		btnBtn2.cst = 0;
		btnBtn2.fProcess = (btnBtn2.stBtn == stPressed);	// meaning that the button can be processed in this state
	}
	
}



/* ------------------------------------------------------------ */
/*        WaitUntilBtnPressed
**
**        Synopsis:
**
**        Parameters:
**				BOOL *pfBtn1Process	- used as output parameter
**					fTrue if the button 1 causes processing
**					fFalse if the button 1 does not cause processing
**				BOOL *pfBtn2Process	- used as output parameter
**					fTrue if the button 2 causes processing
**					fFalse if the button 2 does not cause processing
**
**        Return Values:
**                void 
**
**        Errors:
**
**
**        Description:
**				Waits in a loop until a button action is detected. Then, configures the output parameters and returns.
**				It reads flags set by Timer5Handler, detecting the following actions:
**					- single button: one of BTN1, BTN2 is pressed - *pfBtn1Process, *pfBtn1Process are set to fTrue
**					- double buttons: one of BTN1, BTN2 is released while the other is pressed - both *pfBtn1Process and *pfBtn1Process are set to fTrue
**				This function is called from main program loop, so its loop is the place where other functionality can be placed.	
**
**
*/
void WaitUntilBtnPressed(BOOL *pfBtn1Process, BOOL *pfBtn2Process)
{
	BYTE	stBtn1;
	BYTE	stBtn2;
	*pfBtn1Process = fFalse;
	*pfBtn2Process = fFalse;
	while ((*pfBtn1Process == fFalse) && (*pfBtn2Process == fFalse))
	{
		// Get the current internal state of the buttons.
		INTDisableInterrupts();
		stBtn1 = btnBtn1.stBtn;
		stBtn2 = btnBtn2.stBtn;
		INTEnableInterrupts();	
		if (( btnBtn1.fReleasedOnDouble == fTrue ) || ( btnBtn2.fReleasedOnDouble == fTrue )) 
		{
			*pfBtn1Process = fTrue;
			*pfBtn2Process = fTrue;
			btnBtn1.fReleasedOnDouble = fFalse;
			btnBtn2.fReleasedOnDouble = fFalse;
		}
		else
		{
			if (( stPressed == stBtn2 ) && ( btnBtn2.fProcess == fTrue ))  
			{
				*pfBtn2Process = fTrue;
				btnBtn2.fProcess = fFalse;
			}
			else
			{
				if (( stPressed == stBtn1 ) && ( btnBtn1.fProcess == fTrue )) 
				{
					*pfBtn1Process = fTrue;
					btnBtn1.fProcess = fFalse;
				}
			}	
		}
		// place here any other functionality needed to be executed in the main loop.
	}
}

BYTE userDefArrow[] = {0, 0x4, 0x2, 0x1F, 0x2, 0x4, 0, 0}; // this is used to define a CGRAM character that resembles to an arrow
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
** 		Main loop contains several steps corresponding to some features demo.
** 		At each step buttons are read using WaitUntilBtnPressed function and then processed by calling specific CLP functions.
*/

int main( void )
{	
	BYTE	stBtn1;
	BYTE	stBtn2;
	HWORD	stLedsBase;
	BYTE	stLeds;
	char szInfo1[0x27], szInfo2[0x27];
	BOOL fBtn1Process, fBtn2Process;
	DeviceInit();
	AppInit();
	ClpInit(displaySetOptionDisplayOn);
	int idxScrollPos = 0;
	BOOL fBackLight = fTrue;
	BOOL fDisplay = fTrue;
	BOOL fCursor = fFalse;
	BOOL fBlink = fFalse;
	ClpSetBackLight(fBackLight);

	while(fTrue)
	{
		// 1. Welcome

		strcpy(szInfo1, "CLP Demo. Press");
		strcpy(szInfo2, "btn to continue");
		ClpWriteStringAtPos(szInfo1, 0, 0);
		ClpWriteStringAtPos(szInfo2, 1, 0);
		fBtn1Process = fFalse;
		fBtn2Process = fFalse;
		while(fBtn1Process == fFalse && fBtn2Process == fFalse)
		{
			WaitUntilBtnPressed(&fBtn1Process, &fBtn2Process);
		}

		// 2. Toggle backlight
		strcpy(szInfo1, "BTN2: Backlight");
		strcpy(szInfo2, "BTN1: Continue.");
		ClpWriteStringAtPos(szInfo1, 0, 0);
		ClpWriteStringAtPos(szInfo2, 1, 0);
		fBtn1Process = fFalse;
		fBtn2Process = fFalse;
		while(fBtn1Process == fFalse)
		{
			WaitUntilBtnPressed(&fBtn1Process, &fBtn2Process);
			if(fBtn2Process == fTrue)
			{
				// if only Btn2 is pressed
				fBackLight =  1 - fBackLight; // toggle fBackLight
				ClpSetBackLight(fBackLight);
			}
		}
		ClpSetBackLight(fTrue); // retsore backlight
		// 3. Display shift
		strcpy(szInfo1, "Btns - L/R scroll long text.");
		strcpy(szInfo2, "Both btns continue.");
		ClpWriteStringAtPos(szInfo1, 0, 0);
		ClpWriteStringAtPos(szInfo2, 1, 0);
		while(fBtn1Process == fFalse || fBtn2Process == fFalse)
		{
			WaitUntilBtnPressed(&fBtn1Process, &fBtn2Process);
			if(fBtn1Process != fBtn2Process)
			{
				// if only one button is pressed
				ClpDisplayShift(fBtn1Process);
			}
		}
		ClpDisplayClear(); // clear display, return cursor home
		// 4. Toggle display
		strcpy(szInfo1, "BTN2: Display");
		strcpy(szInfo2, "BTN1: Continue.");
		ClpWriteStringAtPos(szInfo1, 0, 0);
		ClpWriteStringAtPos(szInfo2, 1, 0);
		fBtn1Process = fFalse;
		fBtn2Process = fFalse;
		while(fBtn1Process == fFalse)
		{
			WaitUntilBtnPressed(&fBtn1Process, &fBtn2Process);
			if(fBtn2Process == fTrue)
			{
				// if only Btn2 is pressed
				fDisplay =  1 - fDisplay; // toggle fDisplay
				ClpDisplaySet(fDisplay == fTrue ? displaySetOptionDisplayOn: 0);
			}
		}
		// restore display on
		ClpDisplaySet(displaySetOptionDisplayOn);
		// 5. Toggle cursor
		strcpy(szInfo1, "BTN2: Cursor ");
		strcpy(szInfo2, "BTN1: Continue.");
		ClpWriteStringAtPos(szInfo1, 0, 0);
		ClpWriteStringAtPos(szInfo2, 1, 0);
		ClpReturnHome();	// return cursor home
		fBtn1Process = fFalse;
		fBtn2Process = fFalse;
		while(fBtn1Process == fFalse)
		{
			WaitUntilBtnPressed(&fBtn1Process, &fBtn2Process);
			if(fBtn2Process == fTrue)
			{
				// if only Btn2 is pressed
				fCursor =  1 - fCursor; // toggle fCursor
				ClpDisplaySet( displaySetOptionDisplayOn | (fCursor == fTrue ? displaySetOptionCursorOn : 0));
			}
		}
		// restore cursor on
		ClpDisplaySet(displaySetOptionDisplayOn | displaySetOptionCursorOn);
		// 6. Toggle blink
		strcpy(szInfo1, "BTN2: Blink ");
		strcpy(szInfo2, "BTN1: Continue.");
		ClpWriteStringAtPos(szInfo1, 0, 0);
		ClpWriteStringAtPos(szInfo2, 1, 0);
		ClpReturnHome();	// return cursor home
		fBtn1Process = fFalse;
		fBtn2Process = fFalse;
		while(fBtn1Process == fFalse)
		{
			WaitUntilBtnPressed(&fBtn1Process, &fBtn2Process);
			if(fBtn2Process == fTrue)
			{
				// if only Btn2 is pressed
				fBlink =  1 - fBlink; // toggle fBlink
				ClpDisplaySet( displaySetOptionDisplayOn | displaySetOptionCursorOn |  (fBlink == fTrue ? displaySetBlinkOn : 0));
			}
		}
		// 7. Shift cursor
		strcpy(szInfo1, "Btns-L/R cursor");
		strcpy(szInfo2, "Both btns continue");
		ClpWriteStringAtPos(szInfo1, 0, 0);
		ClpWriteStringAtPos(szInfo2, 1, 0);
		ClpReturnHome();	// return cursor home
		while(fBtn1Process == fFalse || fBtn2Process == fFalse)
		{
			WaitUntilBtnPressed(&fBtn1Process, &fBtn2Process);
			if(fBtn1Process != fBtn2Process)
			{
				// if only one button is pressed
				ClpCursorShift(fBtn1Process);
			}
		}
		// restore cursor off
		ClpDisplaySet(displaySetOptionDisplayOn);
		// 8. User char.
		// define character in CGRAM at position 0 (corresponding to ASCII code 0)
		ClpWriteBytesAtPosCgram(userDefArrow, 8, posCgramChar0);
		ClpDisplayClear(); // clear display, return cursor home
		strcpy(szInfo1, "User char:");
		ClpWriteStringAtPos(szInfo1, 0, 0);

		ClpWriteDataByte(0); // ASCII code 0
		// write user defined character	
		strcpy(szInfo2, "btn to continue");
		ClpWriteStringAtPos(szInfo2, 1, 0);
		fBtn1Process = fFalse;
		fBtn2Process = fFalse;
		while(fBtn1Process == fFalse && fBtn2Process == fFalse)
		{
			WaitUntilBtnPressed(&fBtn1Process, &fBtn2Process);
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



/*************************************************************************************/
