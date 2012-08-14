/************************************************************************/
/*																		*/
/*	HB_Moto_Defs.h -- Common Utility Procedures for the PIC32MX         */
/*																		*/
/************************************************************************/
/*	Author: Cristian Fatu												*/
/*	Copyright 2011, Digilent Inc.										*/
/************************************************************************/
/*  Module Description: 												*/
/*																		*/
/*	This header contains definitions needed to manage DC motor(s) 		*/
/*	using HBx PMODS.													*/
/*																		*/
/************************************************************************/
/*  Revision History:						                			*/
/*											                        	*/
/*	06/04/2011 (CristianF): created			                			*/
/*											                        	*/
/************************************************************************/

#if !defined(_HB_MOTORDEFS_INC)
#define	_HB_MOTORDEFS_INC

// define _MOTORX for each used motor
#define _MOTOR1
//#define _MOTOR2
//#define _MOTOR3
//#define _MOTOR4

// define REALTIME_CALLEDBYUSER if user calls real time counter increment macro
#define REALTIME_CALLEDBYUSER

// REAL_TIME_FACTOR is the rate on which real time counter is incfremented (how many times it is incremented in one second).
#define REAL_TIME_FACTOR 10000 // (= 1s /100 us)

#endif //_HB_MOTORDEFS_INC
