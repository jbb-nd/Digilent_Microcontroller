/************************************************************************/
/*																		*/
/*	main.c	--	Main program module for project							*/
/*																		*/
/************************************************************************/
/*	Author: 	Dion Moses											*/
/*	Copyright 2009, Digilent Inc.										*/
/************************************************************************/
/*  Module Description: 												*/
/*																		*/
/*	This program is an initial startup design for the Digilent			*/
/*	Robotic Starter Kit (RSK) with the Cerebot 32MX4 Microcontroller	*/
/*	board.  It uses two timers to drive two motors with output compare	*/
/*	modules.															*/
/*																		*/
/*	This program is designed so that both PmodHB5 modules that come		*/
/*	standard with the Digilent RSK connect to JD of the Cerebot 32MX4.	*/
/*																		*/
/*	Once the board and RSK are setup properly, BTN2 should activate		*/
/*	the timers to turn the wheels, BTN1 should turn them off.			*/
/*																		*/
/*	Note: This reference design has been made for Rev. C of the			*/
/*	Cerebot 32MX4														*/
/*																		*/
/************************************************************************/
/*  Revision History:													*/
/*																		*/
/*	 10/01/09(DionM): created											*/
/*																		*/
/************************************************************************/

/* ------------------------------------------------------------ */
/*				Include File Definitions						*/
/* ------------------------------------------------------------ */

#include <plib.h>
#include "stdtypes.h"
#include "config.h"
#include "MtrCtrl.h"

/* ------------------------------------------------------------ */
/*				Local Type Definitions							*/
/* ------------------------------------------------------------ */

#define		TCKPS22 			6
#define 	TCKPS21				5
#define 	TCKPS20				4

#define		TCKPS32 			6
#define 	TCKPS31				5
#define 	TCKPS30				4
/* ------------------------------------------------------------ */
/*				Global Variables								*/
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
    
#endif

/* ------------------------------------------------------------ */
/*				Local Variables									*/
/* ------------------------------------------------------------ */

#define	stPressed	1
#define	stReleased	0

#define	cstMaxCnt	10 // number of consecutive reads required for
					   // the state of a button to be updated

struct btn {
	BYTE	stBtn;	// status of the button (pressed or released)
	BYTE	stCur;  // current read state of the button
	BYTE	stPrev; // previous read state of the button
	BYTE	cst;	// number of consecutive reads of the same button 
					// state
};

/* ------------------------------------------------------------ */
/*				Global Variables				                */
/* ------------------------------------------------------------ */

volatile	struct btn	btnBtn1;
volatile	struct btn	btnBtn2;

/* ------------------------------------------------------------ */
/*				Forward Declarations							*/
/* ------------------------------------------------------------ */

void	DeviceInit(void);
void	AppInit(void);
void	Wait_ms(WORD ms);

/* ------------------------------------------------------------ */
/*				Interrupt Service Routines						*/
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
**		buttons. It is also used as a time base for updating
**		the on-board LEDs and the Pmod8LD LEDs at a regular interval.
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
	if ( cstMaxCnt == btnBtn1.cst ) {
		btnBtn1.stBtn = btnBtn1.stCur;
		btnBtn1.cst = 0;
	}
	
	// Update the state of button 2 if necessary.
	if ( cstMaxCnt == btnBtn2.cst ) {
		btnBtn2.stBtn = btnBtn2.stCur;
		btnBtn2.cst = 0;
	}

}

/* ------------------------------------------------------------ */
/*				Procedure Definitions							*/
/* ------------------------------------------------------------ */
/***	main
**
**	Synopsis:
**		st = main()
**
**	Parameters:
**		none
**
**	Return Values:
**		does not return
**
**	Errors:
**		none
**
**	Description:
**		Main program module. Performs basic board initialization
**		and then enters the main program loop.
*/

int main(void) {

	BYTE	stBtn1;
	BYTE	stBtn2;

	DeviceInit();
	AppInit();

	while (fTrue)
	{		
		INTDisableInterrupts();
		stBtn1 = btnBtn1.stBtn;
		stBtn2 = btnBtn2.stBtn;

		INTEnableInterrupts();
		//configure OCR to go forward

		if ((stPressed == stBtn1) && (stReleased == stBtn2)){
			//stop motor of just button 1 pressed
			OC2R	= dtcMtrStopped;
			OC2RS	= dtcMtrStopped;
			OC3R	= dtcMtrStopped;
			OC3RS	= dtcMtrStopped;
		
		}else if((stReleased == stBtn1)&&(stPressed == stBtn2)){
			//start motor if button 2 pressed
			OC2R	= dtcMtrMedium;
			OC2RS	= dtcMtrMedium;

			OC3R	= dtcMtrMedium;
			OC3RS	= dtcMtrMedium;
		}else{
				
		}

	}  //end while
}  //end main

/* ------------------------------------------------------------ */
/***	DeviceInit
**
**	Synopsis:
**		DeviceInit()
**
**	Parameters:
**		none
**
**	Return Values:
**		none
**
**	Errors:
**		none
**
**	Description:
**		Initializes on chip peripheral devices to the default
**		state.
*/

void DeviceInit() {

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

	// Configure left motor direction pin and set default direction.
	trisMtrLeftDirClr	= ( 1 << bnMtrLeftDir );
	prtMtrLeftDirClr	= ( 1 << bnMtrLeftDir );	// forward
	
	// Configure right motor diretion pin and set default direction.
	trisMtrRightDirClr	= ( 1 << bnMtrRightDir );	//modify for JD
	prtMtrRightDirSet	= ( 1 << bnMtrRightDir );	// forward

	// Configure Output Compare 2 to drive the left motor.
	OC2CON	= ( 1 << 2 ) | ( 1 << 1 );	// pwm
	OC2R	= dtcMtrStopped;
	OC2RS	= dtcMtrStopped;

	// Configure Output Compare 3.
	OC3CON = ( 1 << 3 ) | ( 1 << 2 ) | ( 1 << 1 );	// pwm
	OC3R	= dtcMtrStopped;
	OC3RS	= dtcMtrStopped;

	// Configure Timer 2.
	TMR2	= 0;									// clear timer 2 count
	PR2		= 9999;

	// Configure Timer 3.
	TMR3	= 0;
	PR3		= 9999;

	// Start timers and output compare units.
	T2CON		= ( 1 << 15 ) | ( 1 << TCKPS20 )|(1 << TCKPS21);		// timer 2 prescale = 8
	OC2CONSET	= ( 1 << 15 );	// enable output compare module 2
	OC3CONSET	= ( 1 << 15 );	// enable output compare module 3
	T3CON		= ( 1 << 15 ) | ( 1 << TCKPS31 ) | ( 1 << TCKPS30); //timer3 prescale = 8

	// Configure Timer 5.
	TMR5	= 0;
	PR5		= 99; // period match every 100 us
	IPC5SET	= ( 1 << 4 ) | ( 1 << 3 ) | ( 1 << 2 ) | ( 1 << 1 ) | ( 1 << 0 ); // interrupt priority level 7, sub 3
	IFS0CLR = ( 1 << 20);
	IEC0SET	= ( 1 << 20);
	
	// Start timers.
	T5CON = ( 1 << 15 ) | ( 1 << 5 ) | ( 1 << 4 ); // fTimer5 = fPb / 8
    
	// Enable multi-vector interrupts.
	INTEnableSystemMultiVectoredInt();


}

/* ------------------------------------------------------------ */
/***	AppInit
**
**	Synopsis:
**		AppInit()
**
**	Parameters:
**		none
**
**	Return Values:
**		none
**
**	Errors:
**		none
**
**	Description:
**		Performs application specific initialization. Sets devices
**		and global variables for application.
*/

void AppInit() {



}

/* ------------------------------------------------------------ */
/***	Wait_ms
**
**	Synopsis:
**		Wait_ms(WORD)
**
**	Parameters:
**		WORD (range from 0 to 65535)
**
**	Return Values:
**		none
**
**	Errors:
**		none
**
**	Description:
**		Will wait for specified number of milliseconds.  Using a 
**		word variable allows for delays up to 65.535 seconds.  The value
**		in the for statement may need to be altered depending on how your
**		compiler translates this code into AVR assembly.  In assembly, it is
**		possible to generate exact delay values by counting the number of clock
**		cycles your loop takes to execute.  When writing code in C, the compiler
**		interprets the code, and translates it into assembly.  This process is 
**		notoriously inefficient and may vary between different versions of AVR Studio
**		and WinAVR GCC.  A handy method of calibrating the delay loop is to write a 
**		short program that toggles an LED on and off once per second using this 
**		function and using a watch to time how long it is actually taking to
**		complete. 
**
*/

void Wait_ms(WORD delay) {

	WORD i;

	while(delay > 0){

		for( i = 0; i < 375; i ++){
			;;
		}
		delay -= 1;
	}
}

/************************************************************************/
