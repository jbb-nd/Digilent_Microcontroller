/************************************************************************/
/*																		*/
/*	main.c	--	Starter project for Cerebot MX3ck						*/
/*																		*/
/************************************************************************/
/*	Author:		Michelle Yu												*/
/*	Copyright 2011, Digilent Inc.										*/
/************************************************************************/
/*  File Description:													*/
/*		This project cycles through LD4 and LD5              			*/
/*																		*/
/************************************************************************/
/*  Revision History:													*/
/*																		*/
/*	11/16/2011(MichelleY): created										*/
/*																		*/
/************************************************************************/


#include <plib.h>
/* ------------------------------------------------------------ */
/*				Configuration Bits				                */
/* ------------------------------------------------------------ */

// SYSCLK = 80 MHz (8 MHz Crystal/ FPLLIDIV * FPLLMUL / FPLLODIV)
// Primary Osc w/PLL (XT+,HS+,EC+PLL)

#pragma config FNOSC	 = PRIPLL
#pragma config FPLLMUL 	 = MUL_20
#pragma config FPLLIDIV  = DIV_10



/* ------------------------------------------------------------ */
/*				Forward Declarations							*/
/* ------------------------------------------------------------ */

void DeviceInit();
void DelayInit();
void DelayMs(int cms);


/* ------------------------------------------------------------ */
/*				Definitions										*/
/* ------------------------------------------------------------ */

#define	cntMsDelay	10000			//timer 1 delay for 1ms

/* ------------------------------------------------------------ */
/*				Main											*/
/* ------------------------------------------------------------ */

int main()
{
	//set up LD4 and LD5 as digital output
	DeviceInit();
	//initialize timer for delay
	DelayInit();

	/* Perform the main application loop.
	*/
	while (1) 
	{
		//drive LD4 high
		PORTWrite (IOPORT_F, BIT_0);
		DelayMs(1);
		//drive LD5 high
		PORTWrite (IOPORT_F, BIT_1);
		DelayMs(1);
	}
}

/* ------------------------------------------------------------ */
/*  DeviceInit()
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
**		Set LD4 and LD5 as digital output
/* ------------------------------------------------------------ */

void DeviceInit()
{
	//On MX4ck LD4 is on RF0
	//		   LD5 is on RF1
	//Set ports for onboard LEDs to outputs
	PORTSetPinsDigitalOut (IOPORT_F, BIT_0|BIT_1);

}

/* ------------------------------------------------------------ */
/***	DelayInit
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
**		Initialized the hardware for use by delay functions. This
**		initializes Timer 1 to count at 10Mhz.
/* ------------------------------------------------------------ */

void DelayInit()	
{
	unsigned int tcfg;

	/* Configure Timer 1. This sets it up to count a 10Mhz with a period of 0xFFFF
	*/
	tcfg = T1_ON|T1_IDLE_CON|T1_SOURCE_INT|T1_PS_1_8|T1_GATE_OFF|T1_SYNC_EXT_OFF;
	OpenTimer1(tcfg, 0xFFFF);

}

/* ------------------------------------------------------------ */
/***	DelayMs
**
**	Parameters:
**		cms			- number of milliseconds to delay
**
**	Return Value:
**		none
**
**	Errors:
**		none
**
**	Description:
**		Delay the requested number of milliseconds. Uses Timer1.
/* ------------------------------------------------------------ */

void DelayMs(int cms)
{
	int		ims;

	for (ims=0; ims<cms; ims++) {
		WriteTimer1(0);
		while (ReadTimer1() < cntMsDelay);
	}

}		