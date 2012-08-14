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
/*	07/06/2009 (MichaelA): created			                			*/
/*	07/24/2009 (MichaelA): updated to use Rev C pin layout				*/
/*	08/04/2009 (MichaelA): added pin definitions for Pmod8LD			*/
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

/*	Pmod8LD LEDs
*/
#define trisLd0				TRISF
#define	trisLd0Set			TRISFSET
#define	trisLd0Clr			TRISFCLR
#define	prtLd0				PORTF
#define	prtLd0Set			PORTFSET
#define	prtLd0Clr			PORTFCLR
#define	bnLd0				12

#define trisLd1				TRISF
#define	trisLd1Set			TRISFSET
#define	trisLd1Clr			TRISFCLR
#define	prtLd1				PORTF
#define	prtLd1Set			PORTFSET
#define	prtLd1Clr			PORTFCLR
#define	bnLd1				13

#define trisLd2				TRISF
#define	trisLd2Set			TRISFSET
#define	trisLd2Clr			TRISFCLR
#define	prtLd2				PORTF
#define	prtLd2Set			PORTFSET
#define	prtLd2Clr			PORTFCLR
#define	bnLd2				4

#define trisLd3				TRISF
#define	trisLd3Set			TRISFSET
#define	trisLd3Clr			TRISFCLR
#define	prtLd3				PORTF
#define	prtLd3Set			PORTFSET
#define	prtLd3Clr			PORTFCLR
#define	bnLd3				5

#define trisLd4				TRISE
#define	trisLd4Set			TRISESET
#define	trisLd4Clr			TRISECLR
#define	prtLd4				PORTE
#define	prtLd4Set			PORTESET
#define	prtLd4Clr			PORTECLR
#define	bnLd4				8

#define trisLd5				TRISD
#define	trisLd5Set			TRISDSET
#define	trisLd5Clr			TRISDCLR
#define	prtLd5				PORTD
#define	prtLd5Set			PORTDSET
#define	prtLd5Clr			PORTDCLR
#define	bnLd5				0

#define trisLd6				TRISD
#define	trisLd6Set			TRISDSET
#define	trisLd6Clr			TRISDCLR
#define	prtLd6				PORTD
#define	prtLd6Set			PORTDSET
#define	prtLd6Clr			PORTDCLR
#define	bnLd6				8


#define trisLd7				TRISE
#define	trisLd7Set			TRISESET
#define	trisLd7Clr			TRISECLR
#define	prtLd7				PORTE
#define	prtLd7Set			PORTESET
#define	prtLd7Clr			PORTECLR
#define	bnLd7				9


/* ------------------------------------------------------------ */
/*					Miscellaneous Declarations					*/
/* ------------------------------------------------------------ */



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
