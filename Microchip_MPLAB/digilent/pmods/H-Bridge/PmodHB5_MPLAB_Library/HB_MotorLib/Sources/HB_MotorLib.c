/************************************************************************/
/*																		*/
/*	HB_Motor.c -- Definitions for HB_Motor functionality           		*/
/*																		*/
/************************************************************************/
/*	Author: Cristian Fatu												*/
/*	Copyright 2011, Digilent Inc.										*/
/************************************************************************/
/*  Module Description: 												*/
/*																		*/
/*	This module contains definitions for common functions needed to 	*/
/*	manage a DC motor using HBx PMODS.									*/
/*	The command of the motor is implemented by PWM generation using		*/
/*  output compare modules.												*/
/*	Reading rotation speed is implemented using Input capture module's	*/
/*  interrupts.															*/
/*  Considering the PMODs pins configuration is possible to use 4 HB5	*/
/*  PMODS. 																*/
/*  For each of these 4 configuration one set of functions is defined.	*/
/*	Edit HB_MotorLib_Defs.h and according to motors configuration
/*  use "#define _MOTORx" definitions . The number of the motor			*/
/*	is the number of the corresponding OC and IC in that PMOD connector.*/
/*	The module must include HB_MotorLib.h file (where functions are 	*/
/*	declared) and HB_MotorLib_Defs.h (where definitions are placed).	*/

/************************************************************************/
/*  Revision History:						                			*/
/*											                        	*/
/*	06/04/2011 (CristianF): created			                			*/
/*											                        	*/
/************************************************************************/

/* ------------------------------------------------------------ */
/*				Include File Definitions						*/
/* ------------------------------------------------------------ */

#include <plib.h>
#include "stdtypes.h"
#include "config.h"
#include "util.h"
#include "HB_MotorLib.h"


/* ------------------------------------------------------------ */
/*				Local Type Definitions							*/
/* ------------------------------------------------------------ */


/* ------------------------------------------------------------ */
/*				Global Variables								*/
/* ------------------------------------------------------------ */

WORD	cntRealTime;				// counter for real time, incremented every 100 us
	// it is incremented either by Timer4Handler (REALTIME_CALLEDBYUSER not defined)
	// or by user calling mIncrementRealTime macro (REALTIME_CALLEDBYUSER defined). 
	// REAL_TIME_FACTOR is defined as the rate (per second) on which real time counter is incremented 
/* ------------------------------------------------------------ */
/*				Local Variables									*/
/* ------------------------------------------------------------ */

/* ------------------------------------------------------------ */
/*				Forward Declarations							*/
/* ------------------------------------------------------------ */


/* ------------------------------------------------------------ */
/*				Procedure Definitions							*/
/* ------------------------------------------------------------ */
#ifndef REALTIME_CALLEDBYUSER
/***	Timer4Handler
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
**		Interrupt service routine for Timer 4 interrupt.
**		Programmed to execute every 100 us.
**		It is used to increment the real time counter
**		
*/

void __ISR(_TIMER_4_VECTOR, ipl7) Timer4Handler(void)
{
	mT4ClearIntFlag();
	
	// increments real time counter
	mIncrementRealTime;
}

#endif	// REALTIME_CALLEDBYUSER
#ifdef _MOTOR1

/* ------------------------------------------------------------ */
/*        __ISR
**
**        Synopsis:
**
**        Parameters:
**				_INPUT_CAPTURE_1_VECTOR 
**				ipl7
**
**
**        Return Values:
**                void 
**
**        Errors:
**
**
**        Description:
**		Interrupt service routine for Input Capture 1 interrupt. 
**		It is triggered by transitions of SA Hall sensor on the motor.
**		It is used to call motor direction / speed detection functionality. 
**		Normally it is not advised to call functions from ISRs. 
**		This decision was taken considering the fact that these interrupts are not very often triggered 
**		and that the motor direction / speed detection functionality is not intensive.
**
*/
void __ISR(_INPUT_CAPTURE_1_VECTOR, ipl7) InputCapture1Interrupt() 
{ 
	HB_MotorReaction1(NULL, 0);
	mIC1ClearIntFlag();
}

/* ------------------------------------------------------------ */
/*        HB_MotorConfigure1
**
**        Synopsis:
**				HB_MotorConfigure1(dDuty, fFalse);
**        Parameters:
**				double dDutySubunit - the initial value for the duty factor (value between 0 and 1)
**				BOOL fOnlyPWM		- fTrue if only PWM functionality should be initialized
									- fFalse if all functionality should be initialized
**
**
**        Return Values:
**                void 
**
**        Errors:
**
**
**        Description:
**				Initializes DC motor 1 functionality:
**					- Configures pin directions (if(fOnlyPWM == fFalse))
**					- Configures IC module (if(fOnlyPWM == fFalse))
**					- Configures OC module
**					- If necessary, configures Timer 4 interrupt, used for real time counter
**				
*/
void HB_MotorConfigure1(double dDutySubunit, BOOL fOnlyPWM)
{
	if(fOnlyPWM == fFalse)
	{
		// Configure motor direction pin as output and set default direction.
		trisMtr1DirClr	= ( 1 << bnMtr1Dir );
		prtMtr1DirClr	= ( 1 << bnMtr1Dir );	// forward
		
		// Configure motor enable pin as output.
		trisMtr1EnClr	= ( 1 << bnMtr1En );
	
		// Configure motor SA as input
		trisMtr1SASet	= ( 1 << bnMtr1SA );
	
		// Configure motor SB as input
		trisMtr1SBSet	= ( 1 << bnMtr1SB );

		// Configure Input Capture Module 1, to be used for reading motor 1 speed and direction information.
		// - Capture Every edge
		// - Enable capture interrupts
		// - Use Timer 2 source
		OpenCapture1( IC_EVERY_EDGE | IC_INT_1CAPTURE | IC_TIMER2_SRC | IC_ON );

		// Configure IC2 interrupt
		ConfigIntCapture1(IC_INT_ON | IC_INT_PRIOR_7 |IC_INT_SUB_PRIOR_3);

#ifndef REALTIME_CALLEDBYUSER
	// Configure Timer 4. Used for real time.
	// STEP 1. configure the Timer4
		// Timer Input Clock prescaler 1/8, so the frequency is 1/8 of Periferal bus freq, which is 1/8 of SYSCLK (64 MHz) = 1 MHz.
		// Period = 99, correspondding to 100 us.
		OpenTimer4(T4_ON | T4_SOURCE_INT | T4_PS_1_8, 99);
	
		// STEP 2. set the timer interrupt priority level 7, sub 3
		ConfigIntTimer4(T4_INT_ON | T4_INT_PRIOR_7 | T4_INT_SUB_PRIOR_3);
#endif //REALTIME_CALLEDBYUSER
	}
	// Configure PWM
	HWORD hwPwm = (HWORD)((double)cntPulsePWMMax * dDutySubunit);
	// Configure and start Timer 2. used as clock for PWM and timer for IC2
		// Timer Input Clock prescaler 1, so the frequency is Periferal bus freq, which is 1/8 of SYSCLK (64 MHz) = 8 MHz.
		// Period = FFFF, correspondding to 65536 * 125 ns = 8.192 ms
	// Period = cntPulsePWMMax
	OpenTimer2(T2_ON, cntPulsePWMMax);
	// Configure and start OC2, to generate PWM
	OpenOC1(OC_ON | OC_TIMER_MODE16 | OC_PWM_FAULT_PIN_DISABLE | OC_TIMER2_SRC, hwPwm, hwPwm);
/*
	OC1CON = 0x0000; 	// Turn off OC1 while doing setup.
	OC1CON = 0x0006; // Configure OC1 as simple PWM on 16 bits, Based on Timer 2.
					// Bit 5 = 0 for 16 bit
					// Bit 3 = 0 for Timer 2 as source
					// Bits 2-0 = 110 for PWM mode on OC1, Fault pin disabled

	OC1R = hwPwm; // Initialize primary Compare Register
	OC1RS = hwPwm; // Initialize secondary Compare Register
	PR2 = cntPulsePWMMax; 	// Set period for Timer 2 interrupt.
	T2CONSET = 0x8000; // Start timer2
	OC1CONSET = 0x8000; // Start the OC1 module
*/
}

/* ------------------------------------------------------------ */
/*        HB_MotorCommand1
**
**        Synopsis:
**				HB_MotorCommand1(dDuty > 0 ? dDuty: -dDuty, dDuty < 0);  
**        Parameters:
**				double dDutySubunit - the duty factor (value between 0 and 1)
**				BOOL fDir			- the direction
**
**
**        Return Values:
**                void 
**
**        Errors:
**
**
**        Description:
**				Commands the motor according to the provided duty and direction.
**				The corresponding Output Compare (OC) module is programmed so that it generates a PWM corresponding to the duty.
**				The function also ensures a minimum of 1 ms of low on EN before and after direction changes.
**
*/
void HB_MotorCommand1(double dDutySubunit, BOOL fDir)
{
	static BOOL fOldDir = fFalse;
	if(fDir != fOldDir)
	{
		
		// ensure a minimum period of time EN set to 0
		
		// Set 0 to IO Port pin corresponding to OC1.
		prtMtr1EnClr = ( 1 << bnMtr1En );
		// Configure IO Port pin corresponding to OC1 as output.
		trisMtr1EnClr	= ( 1 << bnMtr1En );
		// Disable OC2
		OpenOC1(OC_OFF, 0, 0);
		// Wait 1 ms
		DelayUs(cntUsForceLowEn); 		// wait 1 ms
	}
	if(fDir == fTrue)
	{
		prtMtr1DirSet = ( 1 << bnMtr1Dir );	
	}
	else
	{
		prtMtr1DirClr = ( 1 << bnMtr1Dir );
	}
	if(fDir != fOldDir)
	{
		DelayUs(cntUsForceLowEn); 		// wait 1 ms

		// Enable OC1
		HB_MotorConfigure1(dDutySubunit, fTrue);

		fOldDir = fDir; // store direction in fOldDir
	}
	// compute PWM from Maximum PWM value and the subunitar duty value (duty value is between 0 and 1).
	HWORD hwPwm = (HWORD)((double)cntPulsePWMMax * dDutySubunit);
	// Set secondary Compare Register with current duty
	SetDCOC1PWM(hwPwm); // Similar to OC1RS = hwPwm; 
}

/* ------------------------------------------------------------ */
/*        HB_MotorReaction1
**
**        Synopsis:
**				HB_MotorReaction1(NULL); when called to detect direction / manage counters
				HB_MotorReaction1(&dDCMotorReactionContext, cntRealTime); when called to detect speed / read values
**        Parameters:
**				DCMotorReactionContext *pDCMotorReactionContext 
**											- NULL when function is only called to manage rotation counter 
											- pointer to structure that maintains reaction data (speed, counter, timestamps). Acts as output parameter when speed is read.
**				double dDuty - This will be used to determine the sign of the printed RPM
**
**        Return Values:
**                void 
**
**        Errors:
**
**
**        Description:
**			This function deals with detection of motor direction / speed functionality. It also encapsulates (unsing static variables) the data needed in this process.
**			Every time when IC interrupt occurs, direction is detected and a rotation counter for SA transitions is incremented.
**			Speed (in RPM) is basically computed by dividing the rotation counter difference to the elapsed realtime (refering to last time when speed was computed).
**			The function has two main purposes:
**				1. Detect the direction / manage rotation contor used to detect speed of the motor. 
**				This is called from IC ISR. 
**				No IC buffer data is used, the speed will only use the rotation counter maintained here.
**				When this is called:
**						- direction is detected considering duty, which is based upone DIR pin
**						- based on direction, the rotation counter of SA transitions is incremented
**				2. Read the speed. The speed is computed using the rotation contour, current time and saved rotation counter and saved realtime
**				When this is called:
**						- the speed is detected using the difference between the current rotation counter value and the counter saved value 
**						and the difference between current real time counter and saved realtime counter.
**							- the speed will also consider
**								- the fact that Hall sensors trigger 3 cycles of SA / engine rotation (/ 3)
**								- the fact that both SA transitions are counted (/ 2)
**								- the reduction motor factor (/ reductionMotor4)
**								- the real time factor (/ REAL_TIME_FACTOR) - how many real time values for one second
**						- the real time is saved.
**						- the rotation counter is saved.
**
*/
void HB_MotorReaction1(DCMotorReactionContext *pDCMotorReactionContext, double dDuty)
{
	static DCMotorReactionContext sDCMotorReactionContext = {fFalse, 0, 0, 0.0};
	if(pDCMotorReactionContext == NULL)
	{
		BOOL fSAHigh = ( prtMtr1SA & ( 1 << bnMtr1SA ) ) != 0;
		BOOL fSBHigh = ( prtMtr1SB & ( 1 << bnMtr1SB ) ) != 0;
		BOOL fDir = (fSAHigh == fSBHigh);
		sDCMotorReactionContext.fDir = fDir;
		sDCMotorReactionContext.wRotationCounter += 1;//(fDir == fTrue ? -1: 1);
	}
	else
	{
		// get value mode
		// compute speed (RPM)
		WORD wTimeDelay = cntRealTime - sDCMotorReactionContext.wSavedRealTime;
		// real time is in 100 us 
		WORD wRotationCounterDelay = sDCMotorReactionContext.wRotationCounter - sDCMotorReactionContext.wSavedRotationCounter;

		if(wRotationCounterDelay > 0x80000000)
		{
			// if counter delay is more than half of the maximum value on 32 bits, use the complement value
			wRotationCounterDelay = 1 + ~wRotationCounterDelay; 
		}

		sDCMotorReactionContext.dComputedSpeedRpm = ((float)wRotationCounterDelay *60 * REAL_TIME_FACTOR / 3.0 / 2 / (float)wTimeDelay) / (float)reductionMotor1;
		// real time contains bits of 100 us
		// divide by 3 because there are 3 SA cycles / motor rotation
		// divide by 2 because both raising and falling trigger counter update

		if(sDCMotorReactionContext.dComputedSpeedRpm < 0) sDCMotorReactionContext.dComputedSpeedRpm *= -1;	//makes sure the RPM is positive

		if(dDuty < 0) sDCMotorReactionContext.dComputedSpeedRpm *= -1; //changes the RPM to neg if duty is neg


		sDCMotorReactionContext.wSavedRealTime = cntRealTime;	// stamp the real time
		sDCMotorReactionContext.wSavedRotationCounter = sDCMotorReactionContext.wRotationCounter; // stamp the counter value

		*pDCMotorReactionContext = sDCMotorReactionContext;
	}
}





#endif //_MOTOR1

#ifdef _MOTOR2
/* ------------------------------------------------------------ */
/*        __ISR
**
**        Synopsis:
**
**        Parameters:
**				_INPUT_CAPTURE_2_VECTOR 
**				ipl7
**
**
**        Return Values:
**                void 
**
**        Errors:
**
**
**        Description:
**		Interrupt service routine for Input Capture 2 interrupt. 
**		It is triggered by transitions of SA Hall sensor on the motor.
**		It is used to call motor direction / speed detection functionality. 
**		Normally it is not advised to call functions from ISRs. 
**		This decision was taken considering the fact that these interrupts are not very often triggered 
**		and that the motor direction / speed detection functionality is not intensive.
**
*/
void __ISR(_INPUT_CAPTURE_2_VECTOR, ipl7) InputCapture2Interrupt() 
{ 
	HB_MotorReaction2(NULL);
	mIC2ClearIntFlag();
}

/* ------------------------------------------------------------ */
/*        HB_MotorConfigure2
**
**        Synopsis:
**				HB_MotorConfigure2(dDuty, fFalse);
**        Parameters:
**				double dDutySubunit - the initial value for the duty factor (value between 0 and 1)
**				BOOL fOnlyPWM		- fTrue if only PWM functionality should be initialized
									- fFalse if all functionality should be initialized
**
**
**        Return Values:
**                void 
**
**        Errors:
**
**
**        Description:
**				Initializes DC motor 1 functionality:
**					- Configures pin directions (if(fOnlyPWM == fFalse))
**					- Configures IC module (if(fOnlyPWM == fFalse))
**					- Configures OC module
**					- If necessary, configures Timer 4 interrupt, used for real time counter
**				
*/

void HB_MotorConfigure2(double dDutySubunit, BOOL fOnlyPWM)
{
	if(fOnlyPWM == fFalse)
	{
		// Configure motor direction pin as output and set default direction.
		trisMtr2DirClr	= ( 1 << bnMtr2Dir );
		prtMtr2DirClr	= ( 1 << bnMtr2Dir );	// forward
		
		// Configure motor enable pin as output.
		trisMtr2EnClr	= ( 1 << bnMtr2En );
	
		// Configure motor SA as input
		trisMtr2SASet	= ( 1 << bnMtr2SA );
	
		// Configure motor SB as input
		trisMtr2SBSet	= ( 1 << bnMtr2SB );

		// Configure Input Capture Module 2, to be used for reading motor 2 speed and direction information.
		// - Capture Every edge
		// - Enable capture interrupts
		// - Use Timer 2 source
		OpenCapture2( IC_EVERY_EDGE | IC_INT_2CAPTURE | IC_TIMER2_SRC | IC_ON );

		// Configure IC2 interrupt
		ConfigIntCapture2(IC_INT_ON | IC_INT_PRIOR_7 |IC_INT_SUB_PRIOR_3);
#ifndef REALTIME_CALLEDBYUSER
	// Configure Timer 4. Used for real time.
	// STEP 1. configure the Timer4
		// Timer Input Clock prescaler 1/8, so the frequency is 1/8 of Periferal bus freq, which is 1/8 of SYSCLK (64 MHz) = 1 MHz.
		// Period = 99, correspondding to 100 us.
		OpenTimer4(T4_ON | T4_SOURCE_INT | T4_PS_1_8, 99);
	
		// STEP 2. set the timer interrupt priority level 7, sub 3
		ConfigIntTimer4(T4_INT_ON | T4_INT_PRIOR_7 | T4_INT_SUB_PRIOR_3);
#endif //REALTIME_CALLEDBYUSER	
}
	// Configure PWM
	HWORD hwPwm = (HWORD)((double)cntPulsePWMMax * dDutySubunit);
	// Configure and start Timer 2. used as clock for PWM and timer for IC2
		// Timer Input Clock prescaler 1, so the frequency is Periferal bus freq, which is 1/8 of SYSCLK (64 MHz) = 8 MHz.
		// Period = FFFF, correspondding to 65536 * 125 ns = 8.192 ms
	// Period = cntPulsePWMMax
	OpenTimer2(T2_ON, cntPulsePWMMax);
	// Configure and start OC2, to generate PWM
	OpenOC2(OC_ON | OC_TIMER_MODE16 | OC_PWM_FAULT_PIN_DISABLE | OC_TIMER2_SRC, hwPwm, hwPwm);
}
/* ------------------------------------------------------------ */
/*        HB_MotorCommand2
**
**        Synopsis:
**				HB_MotorCommand2(dDuty > 0 ? dDuty: -dDuty, dDuty < 0);  
**        Parameters:
**				double dDutySubunit - the duty factor (value between 0 and 1)
**				BOOL fDir			- the direction
**
**
**        Return Values:
**                void 
**
**        Errors:
**
**
**        Description:
**				Commands the motor according to the provided duty and direction.
**				The corresponding Output Compare (OC) module is programmed so that it generates a PWM corresponding to the duty.
**				The function also ensures a minimum of 1 ms of low on EN before and after direction changes.
**
*/
void HB_MotorCommand2(double dDutySubunit, BOOL fDir)
{
	static BOOL fOldDir = fFalse;
	if(fDir != fOldDir)
	{
		// ensure a minimum period of time EN set to 0
		
		// Set 0 to IO Port pin corresponding to OC2.
		prtMtr2EnClr = ( 1 << bnMtr2En );
		// Configure IO Port pin corresponding to OC2 as output.
		trisMtr2EnClr	= ( 1 << bnMtr2En );
		// Disable OC2
		OpenOC2(OC_OFF, 0, 0);
		// Wait 1 ms
		DelayUs(cntUsForceLowEn); 		// wait 1 ms
	}
	if(fDir == fTrue)
	{
		prtMtr2DirSet = ( 1 << bnMtr2Dir );	
	}
	else
	{
		prtMtr2DirClr = ( 1 << bnMtr2Dir );
	}
	if(fDir != fOldDir)
	{
		DelayUs(cntUsForceLowEn); 		// wait 1 ms

		// Enable OC2
		HB_MotorConfigure2(dDutySubunit, fTrue);

		fOldDir = fDir; // store direction in fOldDir
	}
	// compute PWM from Maximum PWM value and the subunitar duty value (duty value is between 0 and 1).
	HWORD hwPwm = (HWORD)((double)cntPulsePWMMax * dDutySubunit);
	// Set secondary Compare Register with current duty
	SetDCOC2PWM(hwPwm); // Similar to OC2RS = hwPwm; 
}

/* ------------------------------------------------------------ */
/*        HB_MotorReaction2
**
**        Synopsis:
**				HB_MotorReaction2(NULL); when called to detect direction / manage counters
				HB_MotorReaction2(&dDCMotorReactionContext, cntRealTime); when called to detect speed / read values
**        Parameters:
**				DCMotorReactionContext *pDCMotorReactionContext 
**											- NULL when function is only called to manage rotation counter 
											- pointer to structure that maintains reaction data (speed, counter, timestamps). Acts as output parameter when speed is read.
**				double dDuty - This will be used to determine the sign of the printed RPM
**
**        Return Values:
**                void 
**
**        Errors:
**
**
**        Description:
**			This function deals with detection of motor direction / speed functionality. It also encapsulates (unsing static variables) the data needed in this process.
**			Every time when IC interrupt occurs, direction is detected and a rotation counter for SA transitions is incremented.
**			Speed (in RPM) is basically computed by dividing the rotation counter difference to the elapsed realtime (refering to last time when speed was computed).
**			The function has two main purposes:
**				1. Detect the direction / manage rotation contor used to detect speed of the motor. 
**				This is called from IC ISR. 
**				No IC buffer data is used, the speed will only use the rotation counter maintained here.
**				When this is called:
**						- direction is detected considering duty, which is based upone DIR pin
**						- based on direction, the rotation counter of SA transitions is incremented
**				2. Read the speed. The speed is computed using the rotation contour, current time and saved rotation counter and saved realtime
**				When this is called:
**						- the speed is detected using the difference between the current rotation counter value and the counter saved value 
**						and the difference between current real time counter and saved realtime counter.
**							- the speed will also consider
**								- the fact that Hall sensors trigger 3 cycles of SA / engine rotation (/ 3)
**								- the fact that both SA transitions are counted (/ 2)
**								- the reduction motor factor (/ reductionMotor4)
**								- the real time factor (/ REAL_TIME_FACTOR) - how many real time values for one second
**						- the real time is saved.
**						- the rotation counter is saved.
**
*/
void HB_MotorReaction2(DCMotorReactionContext *pDCMotorReactionContext, double dDuty)
{
	static DCMotorReactionContext sDCMotorReactionContext = {fFalse, 0, 0, 0.0};
	if(pDCMotorReactionContext == NULL)
	{
		BOOL fSAHigh = ( prtMtr2SA & ( 1 << bnMtr2SA ) ) != 0;
		BOOL fSBHigh = ( prtMtr2SB & ( 1 << bnMtr2SB ) ) != 0;
		BOOL fDir = (fSAHigh == fSBHigh);
		sDCMotorReactionContext.fDir = fDir;
		sDCMotorReactionContext.wRotationCounter += 1;//(fDir == fTrue ? -1: 1);
	}
	else
	{
		// get value mode
		// compute speed (RPM)
		WORD wTimeDelay = cntRealTime - sDCMotorReactionContext.wSavedRealTime;
		// real time is in 100 us 
		WORD wRotationCounterDelay = sDCMotorReactionContext.wRotationCounter - sDCMotorReactionContext.wSavedRotationCounter;

		if(wRotationCounterDelay > 0x80000000)
		{
			// if counter delay is more than half of the maximum value on 32 bits, use the complement value
			wRotationCounterDelay = 1 + ~wRotationCounterDelay; 
		}

		sDCMotorReactionContext.dComputedSpeedRpm = ((float)wRotationCounterDelay *60 * REAL_TIME_FACTOR / 3.0 / 2 / (float)wTimeDelay) / (float)reductionMotor1;
		// real time contains bits of 100 us
		// divide by 3 because there are 3 SA cycles / motor rotation
		// divide by 2 because both raising and falling trigger counter update

		if(sDCMotorReactionContext.dComputedSpeedRpm < 0) sDCMotorReactionContext.dComputedSpeedRpm *= -1;	//makes sure the RPM is positive

		if(dDuty < 0) sDCMotorReactionContext.dComputedSpeedRpm *= -1; //changes the RPM to neg if duty is neg


		sDCMotorReactionContext.wSavedRealTime = cntRealTime;	// stamp the real time
		sDCMotorReactionContext.wSavedRotationCounter = sDCMotorReactionContext.wRotationCounter; // stamp the counter value

		*pDCMotorReactionContext = sDCMotorReactionContext;
	}
}





#endif //_MOTOR2


#ifdef _MOTOR3
/* ------------------------------------------------------------ */
/*        __ISR
**
**        Synopsis:
**
**        Parameters:
**				_INPUT_CAPTURE_3_VECTOR 
**				ipl7
**
**
**        Return Values:
**                void 
**
**        Errors:
**
**
**        Description:
**		Interrupt service routine for Input Capture 3 interrupt. 
**		It is triggered by transitions of SA Hall sensor on the motor.
**		It is used to call motor direction / speed detection functionality. 
**		Normally it is not advised to call functions from ISRs. 
**		This decision was taken considering the fact that these interrupts are not very often triggered 
**		and that the motor direction / speed detection functionality is not intensive.
**
*/
void __ISR(_INPUT_CAPTURE_3_VECTOR, ipl7) InputCapture3Interrupt() 
{ 
	HB_MotorReaction3(NULL);
	mIC3ClearIntFlag();
}
/* ------------------------------------------------------------ */
/*        HB_MotorConfigure3
**
**        Synopsis:
**				HB_MotorConfigure3(dDuty, fFalse);
**        Parameters:
**				double dDutySubunit - the initial value for the duty factor (value between 0 and 1)
**				BOOL fOnlyPWM		- fTrue if only PWM functionality should be initialized
									- fFalse if all functionality should be initialized
**
**
**        Return Values:
**                void 
**
**        Errors:
**
**
**        Description:
**				Initializes DC motor 1 functionality:
**					- Configures pin directions (if(fOnlyPWM == fFalse))
**					- Configures IC module (if(fOnlyPWM == fFalse))
**					- Configures OC module
**					- If necessary, configures Timer 4 interrupt, used for real time counter
**				
*/

void HB_MotorConfigure3(double dDutySubunit, BOOL fOnlyPWM)
{
	if(fOnlyPWM == fFalse)
	{
		// Configure motor direction pin as output and set default direction.
		trisMtr3DirClr	= ( 1 << bnMtr3Dir );
		prtMtr3DirClr	= ( 1 << bnMtr3Dir );	// forward
		
		// Configure motor enable pin as output.
		trisMtr3EnClr	= ( 1 << bnMtr3En );
	
		// Configure motor SA as input
		trisMtr3SASet	= ( 1 << bnMtr3SA );
	
		// Configure motor SB as input
		trisMtr3SBSet	= ( 1 << bnMtr3SB );

		// Configure Input Capture Module 3, to be used for reading motor 3 speed and direction information.
		// - Capture Every edge
		// - Enable capture interrupts
		// - Use Timer 2 source
		OpenCapture3( IC_EVERY_EDGE | IC_INT_3CAPTURE | IC_TIMER2_SRC | IC_ON );

		// Configure IC3 interrupt
		ConfigIntCapture3(IC_INT_ON | IC_INT_PRIOR_7 |IC_INT_SUB_PRIOR_3);
#ifndef REALTIME_CALLEDBYUSER
	// Configure Timer 4. Used for real time.
	// STEP 1. configure the Timer4
		// Timer Input Clock prescaler 1/8, so the frequency is 1/8 of Periferal bus freq, which is 1/8 of SYSCLK (64 MHz) = 1 MHz.
		// Period = 99, correspondding to 100 us.
		OpenTimer4(T4_ON | T4_SOURCE_INT | T4_PS_1_8, 99);
	
		// STEP 2. set the timer interrupt priority level 7, sub 3
		ConfigIntTimer4(T4_INT_ON | T4_INT_PRIOR_7 | T4_INT_SUB_PRIOR_3);
#endif //REALTIME_CALLEDBYUSER	
	}
	// Configure PWM
	HWORD hwPwm = (HWORD)((double)cntPulsePWMMax * dDutySubunit);
	// Configure and start Timer 2. used as clock for PWM and timer for IC3
		// Timer Input Clock prescaler 1, so the frequency is Periferal bus freq, which is 1/8 of SYSCLK (64 MHz) = 8 MHz.
		// Period = FFFF, correspondding to 65536 * 125 ns = 8.192 ms
	// Period = cntPulsePWMMax
	OpenTimer2(T2_ON, cntPulsePWMMax);
	// Configure and start OC3, to generate PWM
	OpenOC3(OC_ON | OC_TIMER_MODE16 | OC_PWM_FAULT_PIN_DISABLE | OC_TIMER2_SRC, hwPwm, hwPwm);
}

/* ------------------------------------------------------------ */
/*        HB_MotorCommand3
**
**        Synopsis:
**				HB_MotorCommand3(dDuty > 0 ? dDuty: -dDuty, dDuty < 0);  
**        Parameters:
**				double dDutySubunit - the duty factor (value between 0 and 1)
**				BOOL fDir			- the direction
**
**
**        Return Values:
**                void 
**
**        Errors:
**
**
**        Description:
**				Commands the motor according to the provided duty and direction.
**				The corresponding Output Compare (OC) module is programmed so that it generates a PWM corresponding to the duty.
**				The function also ensures a minimum of 1 ms of low on EN before and after direction changes.
**
*/
void HB_MotorCommand3(double dDutySubunit, BOOL fDir)
{
	static BOOL fOldDir = fFalse;
	if(fDir != fOldDir)
	{
		// ensure a minimum period of time EN set to 0
		
		// Set 0 to IO Port pin corresponding to OC3.
		prtMtr3EnClr = ( 1 << bnMtr3En );
		// Configure IO Port pin corresponding to OC3 as output.
		trisMtr3EnClr	= ( 1 << bnMtr3En );
		// Disable OC3
		OpenOC3(OC_OFF, 0, 0);
		// OC3CON = 0;
		// Wait 1 ms
		DelayUs(cntUsForceLowEn); 		// wait 1 ms
	}
	if(fDir == fTrue)
	{
		prtMtr3DirSet = ( 1 << bnMtr3Dir );	
	}
	else
	{
		prtMtr3DirClr = ( 1 << bnMtr3Dir );
	}
	if(fDir != fOldDir)
	{
		DelayUs(cntUsForceLowEn); 		// wait 1 ms

		// Enable OC3
		HB_MotorConfigure3(dDutySubunit, fTrue);

		fOldDir = fDir; // store direction in fOldDir
	}
	// compute PWM from Maximum PWM value and the subunitar duty value (duty value is between 0 and 1).
	HWORD hwPwm = (HWORD)((double)cntPulsePWMMax * dDutySubunit);
	// Set secondary Compare Register with current duty
	SetDCOC3PWM(hwPwm); // Similar to OC3RS = hwPwm; 
}

/* ------------------------------------------------------------ */
/*        HB_MotorReaction3
**
**        Synopsis:
**				HB_MotorReaction3(NULL); when called to detect direction / manage counters
				HB_MotorReaction3(&dDCMotorReactionContext, cntRealTime); when called to detect speed / read values
**        Parameters:
**				DCMotorReactionContext *pDCMotorReactionContext 
**											- NULL when function is only called to manage rotation counter 
											- pointer to structure that maintains reaction data (speed, counter, timestamps). Acts as output parameter when speed is read.
**				double dDuty - This will be used to determine the sign of the printed RPM
**
**        Return Values:
**                void 
**
**        Errors:
**
**
**        Description:
**			This function deals with detection of motor direction / speed functionality. It also encapsulates (unsing static variables) the data needed in this process.
**			Every time when IC interrupt occurs, direction is detected and a rotation counter for SA transitions is incremented.
**			Speed (in RPM) is basically computed by dividing the rotation counter difference to the elapsed realtime (refering to last time when speed was computed).
**			The function has two main purposes:
**				1. Detect the direction / manage rotation contor used to detect speed of the motor. 
**				This is called from IC ISR. 
**				No IC buffer data is used, the speed will only use the rotation counter maintained here.
**				When this is called:
**						- direction is detected considering duty, which is based upone DIR pin
**						- based on direction, the rotation counter of SA transitions is incremented
**				2. Read the speed. The speed is computed using the rotation contour, current time and saved rotation counter and saved realtime
**				When this is called:
**						- the speed is detected using the difference between the current rotation counter value and the counter saved value 
**						and the difference between current real time counter and saved realtime counter.
**							- the speed will also consider
**								- the fact that Hall sensors trigger 3 cycles of SA / engine rotation (/ 3)
**								- the fact that both SA transitions are counted (/ 2)
**								- the reduction motor factor (/ reductionMotor4)
**								- the real time factor (/ REAL_TIME_FACTOR) - how many real time values for one second
**						- the real time is saved.
**						- the rotation counter is saved.
**
*/
void HB_MotorReaction3(DCMotorReactionContext *pDCMotorReactionContext, double dDuty)
{
	static DCMotorReactionContext sDCMotorReactionContext = {fFalse, 0, 0, 0.0};
	if(pDCMotorReactionContext == NULL)
	{
		BOOL fSAHigh = ( prtMtr3SA & ( 1 << bnMtr3SA ) ) != 0;
		BOOL fSBHigh = ( prtMtr3SB & ( 1 << bnMtr3SB ) ) != 0;
		BOOL fDir = (fSAHigh == fSBHigh);
		sDCMotorReactionContext.fDir = fDir;
		sDCMotorReactionContext.wRotationCounter += 1;//(fDir == fTrue ? -1: 1);
	}
	else
	{
		// get value mode
		// compute speed (RPM)
		WORD wTimeDelay = cntRealTime - sDCMotorReactionContext.wSavedRealTime;
		// real time is in 100 us 
		WORD wRotationCounterDelay = sDCMotorReactionContext.wRotationCounter - sDCMotorReactionContext.wSavedRotationCounter;

		if(wRotationCounterDelay > 0x80000000)
		{
			// if counter delay is more than half of the maximum value on 32 bits, use the complement value
			wRotationCounterDelay = 1 + ~wRotationCounterDelay; 
		}

		sDCMotorReactionContext.dComputedSpeedRpm = ((float)wRotationCounterDelay *60 * REAL_TIME_FACTOR / 3.0 / 2 / (float)wTimeDelay) / (float)reductionMotor1;
		// real time contains bits of 100 us
		// divide by 3 because there are 3 SA cycles / motor rotation
		// divide by 2 because both raising and falling trigger counter update

		if(sDCMotorReactionContext.dComputedSpeedRpm < 0) sDCMotorReactionContext.dComputedSpeedRpm *= -1;	//makes sure the RPM is positive

		if(dDuty < 0) sDCMotorReactionContext.dComputedSpeedRpm *= -1; //changes the RPM to neg if duty is neg


		sDCMotorReactionContext.wSavedRealTime = cntRealTime;	// stamp the real time
		sDCMotorReactionContext.wSavedRotationCounter = sDCMotorReactionContext.wRotationCounter; // stamp the counter value

		*pDCMotorReactionContext = sDCMotorReactionContext;
	}
}



#endif //_MOTOR3


#ifdef _MOTOR4
/* ------------------------------------------------------------ */
/*        __ISR
**
**        Synopsis:
**
**        Parameters:
**				_INPUT_CAPTURE_4_VECTOR 
**				ipl7
**
**
**        Return Values:
**                void 
**
**        Errors:
**
**
**        Description:
**		Interrupt service routine for Input Capture 4 interrupt. 
**		It is triggered by transitions of SA Hall sensor on the motor.
**		It is used to call motor direction / speed detection functionality. 
**		Normally it is not advised to call functions from ISRs. 
**		This decision was taken considering the fact that these interrupts are not very often triggered 
**		and that the motor direction / speed detection functionality is not intensive.
**
*/
void __ISR(_INPUT_CAPTURE_4_VECTOR, ipl7) InputCapture4Interrupt() 
{ 
	HB_MotorReaction4(NULL);
	mIC4ClearIntFlag();
}

/* ------------------------------------------------------------ */
/*        HB_MotorConfigure4
**
**        Synopsis:
**				HB_MotorConfigure1(dDuty, fFalse);
**        Parameters:
**				double dDutySubunit - the initial value for the duty factor (value between 0 and 1)
**				BOOL fOnlyPWM		- fTrue if only PWM functionality should be initialized
									- fFalse if all functionality should be initialized
**
**
**        Return Values:
**                void 
**
**        Errors:
**
**
**        Description:
**				Initializes DC motor 1 functionality:
**					- Configures pin directions (if(fOnlyPWM == fFalse))
**					- Configures IC module (if(fOnlyPWM == fFalse))
**					- Configures OC module
**					- If necessary, configures Timer 4 interrupt, used for real time counter
**				
*/

void HB_MotorConfigure4(double dDutySubunit, BOOL fOnlyPWM)
{
	if(fOnlyPWM == fFalse)
	{
		// Configure motor direction pin as output and set default direction.
		trisMtr4DirClr	= ( 1 << bnMtr4Dir );
		prtMtr4DirClr	= ( 1 << bnMtr4Dir );	// forward
		
		// Configure motor enable pin as output.
		trisMtr4EnClr	= ( 1 << bnMtr4En );
	
		// Configure motor SA as input
		trisMtr4SASet	= ( 1 << bnMtr4SA );
	
		// Configure motor SB as input
		trisMtr4SBSet	= ( 1 << bnMtr4SB );

		// Configure Input Capture Module 4, to be used for reading motor 4 speed and direction information.
		// - Capture Every edge
		// - Enable capture interrupts
		// - Use Timer 2 source
		OpenCapture4( IC_EVERY_EDGE | IC_INT_4CAPTURE | IC_TIMER2_SRC | IC_ON );

		// Configure IC1 interrupt
		ConfigIntCapture4(IC_INT_ON | IC_INT_PRIOR_7 |IC_INT_SUB_PRIOR_3);
#ifndef REALTIME_CALLEDBYUSER
	// Configure Timer 4. Used for real time.
	// STEP 1. configure the Timer4
		// Timer Input Clock prescaler 1/8, so the frequency is 1/8 of Periferal bus freq, which is 1/8 of SYSCLK (64 MHz) = 1 MHz.
		// Period = 99, correspondding to 100 us.
		OpenTimer4(T4_ON | T4_SOURCE_INT | T4_PS_1_8, 99);
	
		// STEP 2. set the timer interrupt priority level 7, sub 3
		ConfigIntTimer4(T4_INT_ON | T4_INT_PRIOR_7 | T4_INT_SUB_PRIOR_3);
#endif //REALTIME_CALLEDBYUSER	
	}
	// Configure PWM
	HWORD hwPwm = (HWORD)((double)cntPulsePWMMax * dDutySubunit);
	// Configure and start Timer 2. used as clock for PWM and timer for IC1
		// Timer Input Clock prescaler 1, so the frequency is Periferal bus freq, which is 1/8 of SYSCLK (64 MHz) = 8 MHz.
		// Period = FFFF, correspondding to 65536 * 125 ns = 8.192 ms
	// Period = cntPulsePWMMax
	OpenTimer2(T2_ON, cntPulsePWMMax);
	// Configure and start OC4, to generate PWM
	OpenOC4(OC_ON | OC_TIMER_MODE16 | OC_PWM_FAULT_PIN_DISABLE | OC_TIMER2_SRC, hwPwm, hwPwm);
}

/* ------------------------------------------------------------ */
/*        HB_MotorCommand4
**
**        Synopsis:
**				HB_MotorCommand4(dDuty > 0 ? dDuty: -dDuty, dDuty < 0);  
**        Parameters:
**				double dDutySubunit - the duty factor (value between 0 and 1)
**				BOOL fDir			- the direction
**
**
**        Return Values:
**                void 
**
**        Errors:
**
**
**        Description:
**				Commands the motor according to the provided duty and direction.
**				The corresponding Output Compare (OC) module is programmed so that it generates a PWM corresponding to the duty.
**				The function also ensures a minimum of 1 ms of low on EN before and after direction changes.
**
*/
void HB_MotorCommand4(double dDutySubunit, BOOL fDir)
{
	static BOOL fOldDir = fFalse;
	if(fDir != fOldDir)
	{
		// ensure a minimum period of time EN set to 0
		
		// Set 0 to IO Port pin corresponding to OC4.
		prtMtr4EnClr = ( 1 << bnMtr4En );
		// Configure IO Port pin corresponding to OC4 as output.
		trisMtr4EnClr	= ( 1 << bnMtr4En );
		// Disable OC4
		OpenOC4(OC_OFF, 0, 0);
		// Wait 1 ms
		DelayUs(cntUsForceLowEn); 		// wait 1 ms
	}
	if(fDir == fTrue)
	{
		prtMtr4DirSet = ( 1 << bnMtr4Dir );	
	}
	else
	{
		prtMtr4DirClr = ( 1 << bnMtr4Dir );
	}
	if(fDir != fOldDir)
	{
		DelayUs(cntUsForceLowEn); 		// wait 1 ms

		// Enable OC4
		HB_MotorConfigure4(dDutySubunit, fTrue);

		fOldDir = fDir; // store direction in fOldDir
	}
	// compute PWM from Maximum PWM value and the subunitar duty value (duty value is between 0 and 1).
	HWORD hwPwm = (HWORD)((double)cntPulsePWMMax * dDutySubunit);
	// Set secondary Compare Register with current duty
	SetDCOC4PWM(hwPwm); // Similar to OC4RS = hwPwm; 
}

/* ------------------------------------------------------------ */
/*        HB_MotorReaction4
**
**        Synopsis:
**				HB_MotorReaction4(NULL); when called to detect direction / manage counters
				HB_MotorReaction4(&dDCMotorReactionContext, cntRealTime); when called to detect speed / read values
**        Parameters:
**				DCMotorReactionContext *pDCMotorReactionContext 
**											- NULL when function is only called to manage rotation counter 
											- pointer to structure that maintains reaction data (speed, counter, timestamps). Acts as output parameter when speed is read.
**				double dDuty - This will be used to determine the sign of the printed RPM
**
**        Return Values:
**                void 
**
**        Errors:
**
**
**        Description:
**			This function deals with detection of motor direction / speed functionality. It also encapsulates (unsing static variables) the data needed in this process.
**			Every time when IC interrupt occurs, direction is detected and a rotation counter for SA transitions is incremented.
**			Speed (in RPM) is basically computed by dividing the rotation counter difference to the elapsed realtime (refering to last time when speed was computed).
**			The function has two main purposes:
**				1. Detect the direction / manage rotation contor used to detect speed of the motor. 
**				This is called from IC ISR. 
**				No IC buffer data is used, the speed will only use the rotation counter maintained here.
**				When this is called:
**						- direction is detected considering duty, which is based upone DIR pin
**						- based on direction, the rotation counter of SA transitions is incremented
**				2. Read the speed. The speed is computed using the rotation contour, current time and saved rotation counter and saved realtime
**				When this is called:
**						- the speed is detected using the difference between the current rotation counter value and the counter saved value 
**						and the difference between current real time counter and saved realtime counter.
**							- the speed will also consider
**								- the fact that Hall sensors trigger 3 cycles of SA / engine rotation (/ 3)
**								- the fact that both SA transitions are counted (/ 2)
**								- the reduction motor factor (/ reductionMotor4)
**								- the real time factor (/ REAL_TIME_FACTOR) - how many real time values for one second
**						- the real time is saved.
**						- the rotation counter is saved.
**
*/
void HB_MotorReaction4(DCMotorReactionContext *pDCMotorReactionContext, double dDuty)
{
	static DCMotorReactionContext sDCMotorReactionContext = {fFalse, 0, 0, 0.0};
	if(pDCMotorReactionContext == NULL)
	{
		BOOL fSAHigh = ( prtMtr4SA & ( 1 << bnMtr4SA ) ) != 0;
		BOOL fSBHigh = ( prtMtr4SB & ( 1 << bnMtr4SB ) ) != 0;
		BOOL fDir = (fSAHigh == fSBHigh);
		sDCMotorReactionContext.fDir = fDir;
		sDCMotorReactionContext.wRotationCounter += 1;//(fDir == fTrue ? -1: 1);
	}
	else
	{
		// get value mode
		// compute speed (RPM)
		WORD wTimeDelay = cntRealTime - sDCMotorReactionContext.wSavedRealTime;
		// real time is in 100 us 
		WORD wRotationCounterDelay = sDCMotorReactionContext.wRotationCounter - sDCMotorReactionContext.wSavedRotationCounter;

		if(wRotationCounterDelay > 0x80000000)
		{
			// if counter delay is more than half of the maximum value on 32 bits, use the complement value
			wRotationCounterDelay = 1 + ~wRotationCounterDelay; 
		}

		sDCMotorReactionContext.dComputedSpeedRpm = ((float)wRotationCounterDelay *60 * REAL_TIME_FACTOR / 3.0 / 2 / (float)wTimeDelay) / (float)reductionMotor1;
		// real time contains bits of 100 us
		// divide by 3 because there are 3 SA cycles / motor rotation
		// divide by 2 because both raising and falling trigger counter update

		if(sDCMotorReactionContext.dComputedSpeedRpm < 0) sDCMotorReactionContext.dComputedSpeedRpm *= -1;	//makes sure the RPM is positive

		if(dDuty < 0) sDCMotorReactionContext.dComputedSpeedRpm *= -1; //changes the RPM to neg if duty is neg


		sDCMotorReactionContext.wSavedRealTime = cntRealTime;	// stamp the real time
		sDCMotorReactionContext.wSavedRotationCounter = sDCMotorReactionContext.wRotationCounter; // stamp the counter value

		*pDCMotorReactionContext = sDCMotorReactionContext;
	}
}




#endif //_MOTOR4

/* ------------------------------------------------------------ */


