/************************************************************************/
/*																		*/
/*	config.h -- Project Configuration Declarations                      */
/*																		*/
/************************************************************************/
/*	Author: Michael T. Alexander										*/
/*	Copyright 2009, Digilent Inc.										*/
/************************************************************************/
/*  Module Description: 												*/
/*																		*/
/*	This header contains declarations used to configure the project     */
/*  based on the target platform.                                       */
/*																		*/
/************************************************************************/
/*  Revision History:						                			*/
/*											                        	*/
/*	05/21/2009 (MichaelA): created			                			*/
/*											                        	*/
/************************************************************************/

#if !defined(_CONFIG_INC)
#define _CONFIG_INC


/* ------------------------------------------------------------ */
/*					     Pin Definitions   		    			*/
/* ------------------------------------------------------------ */

/*	Onboard LEDs
*/
#define	trisLed1			TRISB
#define	trisLed1Set			TRISBSET
#define	trisLed1Clr			TRISBCLR
#define	prtLed1				PORTB
#define	prtLed1Set			PORTBSET
#define	prtLed1Clr			PORTBCLR
#define	prtLed1Inv			PORTBINV
#define	bnLed1				10

#define	trisLed2			TRISB
#define	trisLed2Set			TRISBSET
#define	trisLed2Clr			TRISBCLR
#define	prtLed2				PORTB
#define	prtLed2Set			PORTBSET
#define	prtLed2Clr			PORTBCLR
#define	prtLed2Inv			PORTBINV
#define	bnLed2				11

#define	trisLed3			TRISB
#define	trisLed3Set			TRISBSET
#define	trisLed3Clr			TRISBCLR
#define	prtLed3				PORTB
#define	prtLed3Set			PORTBSET
#define	prtLed3Clr			PORTBCLR
#define	prtLed3Inv			PORTBINV
#define	bnLed3				12

#define	trisLed4			TRISB
#define	trisLed4Set			TRISBSET
#define	trisLed4Clr			TRISBCLR
#define	prtLed4				PORTB
#define	prtLed4Set			PORTBSET
#define	prtLed4Clr			PORTBCLR
#define	prtLed4Inv			PORTBINV
#define	bnLed4				13

/*	Onboard Buttons
*/

#define	trisBtn1			TRISA
#define	trisBtn1Set			TRISASET
#define	trisBtn1Clr			TRISACLR
#define	prtBtn1				PORTA
#define	prtBtn1Set			PORTASET
#define	prtBtn1Clr			PORTACLR
#define	bnBtn1				6

#define	trisBtn2			TRISA
#define	trisBtn2Set			TRISASET
#define	trisBtn2Clr			TRISACLR
#define	prtBtn2				PORTA
#define	prtBtn2Set			PORTASET
#define	prtBtn2Clr			PORTACLR
#define	bnBtn2				7


/*	Pan and tilt
*/
#define trisPanSet	TRISASET
#define trisPanClr	TRISACLR
#define prtPanSet	PORTASET
#define prtPanClr	PORTACLR
#define	bnPan		6

#define trisTiltSet TRISASET
#define trisTiltClr	TRISACLR
#define	prtTiltSet	PORTASET
#define prtTiltClr	PORTACLR
#define	bnTilt		7

/*	Headlights
*/
#define trisHlightsSet	TRISESET
#define trisHlightsClr	TRISECLR
#define prtHlightsSet	PORTESET
#define	prtHlightsClr	PORTECLR
#define bnHlights		0

/*	Left motor
*/
#define	trisMtrLeftEnSet	TRISDSET
#define	trisMtrLeftEnClr	TRISDCLR
#define	prtMtrLeftEnSet		PORTDSET
#define	prtMtrLeftEnClr		PORTDCLR
#define	bnMtrLeftEn			1

#define	trisMtrLeftDirSet	TRISDSET
#define	trisMtrLeftDirClr	TRISDCLR
#define	prtMtrLeftDirSet	PORTDSET
#define	prtMtrLeftDirClr	PORTDCLR
#define	bnMtrLeftDir		7

/*	Right motor
*/
#define trisMtrRightEnSet	TRISDSET
#define	trisMtrRightEnClr	TRISDCLR
#define prtMtrRightEnSet	PORTDSET
#define prtMtrRightEnClr	PORTDCLR
#define	bnMtrRightEn		2

#define trisMtrRightDirSet	TRISDSET
#define	trisMtrRightDirClr	TRISDCLR
#define prtMtrRightDirSet	PORTDSET
#define prtMtrRightDirClr	PORTDCLR
#define	bnMtrRightDir		6

/*	PmodRF1
*/
#define		trisRF1IrqSet		TRISBSET
#define		trisRF1IrqClr		TRISBCLR
#define		prtRF1Irq			PORTB
#define		prtRF1IrqSet		PORTBSET
#define		prtRF1IrqClr		PORTBCLR
#define		bnRF1Irq			15

#define		trisRF1RstSet		TRISDSET
#define		trisRF1RstClr		TRISDCLR
#define		prtRF1Rst			PORTD
#define		prtRF1RstSet		PORTDSET
#define		prtRF1RstClr		PORTDCLR
#define		bnRF1Rst			5

#define		trisRF1SlpTrSet		TRISDSET
#define		trisRF1SlpTrClr		TRISDCLR
#define		prtRF1SlpTr			PORTD
#define		prtRF1SlpTrSet		PORTDSET
#define		prtRF1SlpTrClr		PORTDCLR
#define		bnRF1SlpTr			4

#define		trisRF1ClkmSet		TRISBSET
#define		trisRF1ClkmClr		TRISBCLR
#define		prtRF1Clkm			PORTB
#define		prtRF1ClkmSet		PORTBSET
#define		prtRF1ClkmClr		PORTBCLR
#define		bnRF1Clkm			14


/* ------------------------------------------------------------ */
/*					Miscellaneous Declarations					*/
/* ------------------------------------------------------------ */

/*	PmodRF1 Configuration
*/
#define	rfadrRobot				0x39	// robot address for wireless comm.
#define	rfadrController			0x38	// controller address for wireless comm.

#define	rfchan					5		// IEEE 802.15.4 channel assignment
										// This selects the channel (center frequency)
										// used for wireless communication.
										
										// Valid channels and their corresponding
										// center frequency:
										//  1 - 906 MHz
										//  2 - 908 MHz
										//  3 - 910 MHz
										//  4 - 912 MHz
										//  5 - 914 MHz (DEFAULT)
										//  6 - 916 MHz
										//  7 - 918 MHz
										//  8 - 920 MHz
										//  9 - 922 MHz
										// 10 - 924 MHz

#define	OPT_HWSPI	2		//use SPI2 controller for SPI interface

// Define constants for joystick positions.
#define	posXMin		300
#define	posXMax		700
#define	posXCenter	500
#define	posYMin		300
#define posYMax		700
#define posYCenter	500

/* ------------------------------------------------------------ */
/*					General Type Declarations					*/
/* ------------------------------------------------------------ */



/* ------------------------------------------------------------ */
/*					Object Class Declarations					*/
/* ------------------------------------------------------------ */



/* ------------------------------------------------------------ */
/*					Variable Declarations						*/
/* ------------------------------------------------------------ */



/* ------------------------------------------------------------ */
/*					Procedure Declarations						*/
/* ------------------------------------------------------------ */



/* ------------------------------------------------------------ */

#endif
