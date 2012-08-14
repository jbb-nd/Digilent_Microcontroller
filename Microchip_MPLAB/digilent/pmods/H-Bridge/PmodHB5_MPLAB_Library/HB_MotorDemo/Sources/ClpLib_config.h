/************************************************************************/
/*																		*/
/*	ClpLib_config.h -- CLP Configuration Declarations                  	*/
/*																		*/
/************************************************************************/
/*	Author: Cristian Fatu												*/
/*	Copyright 2011, Digilent Inc.										*/
/************************************************************************/
/*  Module Description: 												*/
/*																		*/
/*	This header contains declarations used to configure the CLP     	*/
/*  based on the target platform and PMOD connector used.				*/
/*	Default settings correspond to PmodCLP plugged into JA (J1) and 	*/
/*	JB (J2), _CONTIGUOUS_DATA set.										*/
/*	If _CONTIGUOUS_DATA is not set, default configuration is JC (J1)	*/ 
/*	and JD (J2).														*/
/*  This file is ment to be included only in ClpLib.c.					*/
/*	This file must be included in MPLAB project whenever CLP functions 	*/
/*  are needed. 														*/
/*  Edit this file if CLP is placed on other PMODS connectors than 		*/
/*  default.															*/
/************************************************************************/
/*  Revision History:						                			*/
/*											                        	*/
/*	10/05/2011 (CristianF): created			                			*/
/*											                        	*/
/************************************************************************/

#if !defined(_CLPLIB_CONFIG_INC)
#define _CLPLIB_CONFIG_INC

/* ------------------------------------------------------------ */
/*					     Pin Definitions   		    			*/
/* ------------------------------------------------------------ */

#define _CONTIGUOUS_DATA	// define this if JA-JB connectors are used
#ifdef _CONTIGUOUS_DATA

// There is one configuration of PMOD connectors that allows contiguous data pins
// PmodCLP, connectors J1 = JA and J2 = JB
/*
Data Bus: J1.1, 2, 3, 4, 7, 8, 9, 10
JA-01 PMD0/RE0
JA-02 PMD1/RE1
JA-03 PMD2/RE2
JA-04 PMD3/RE3
JA-07 PMD4/RE4
JA-08 PMD5/RE5
JA-09 PMD5/RE5
JA-10 PMD7/RE7

RS: J2.1
JB-07 PMALL/PMA0/AN15/OCFB/CN12/RB15

R/W: J2.2
JB-08 PMRD/CN14/RD5

E: J2.3
JB-09 PMWR/OC5/CN13/RD4

BL: J2.4
JB-10 PMALH/PMA1/AN14/RB14
*/

// RS - JB-07 PMALL/PMA0/AN15/OCFB/CN12/RB15
#define	trisClpRSSet	TRISBSET
#define	trisClpRSClr	TRISBCLR
#define	prtClpRSSet		PORTBSET
#define	prtClpRSClr		PORTBCLR
#define	bnClpRS			15			


//	RW - JB-08 PMRD/CN14/RD5
#define	trisClpRWSet	TRISDSET
#define	trisClpRWClr	TRISDCLR
#define	prtClpRWSet		PORTDSET
#define	prtClpRWClr		PORTDCLR
#define	bnClpRW			5			

// E - JB-09 PMWR/OC5/CN13/RD4
#define	trisClpESet		TRISDSET
#define	trisClpEClr		TRISDCLR
#define	prtClpESet		PORTDSET
#define	prtClpEClr		PORTDCLR
#define	bnClpE			4			

//	BL - JB-10 PMALH/PMA1/AN14/RB14
#define	trisClpBLSet	TRISBSET
#define	trisClpBLClr	TRISBCLR
#define	prtClpBLSet		PORTBSET
#define	prtClpBLClr		PORTBCLR
#define	bnClpBL			14			

// D0-7 - RE 0-7
#define	trisClpDatSet		TRISESET
#define	trisClpDatClr		TRISECLR
#define	prtClpDatSet		PORTESET
#define	prtClpDatClr		PORTECLR
#define	mskClpDat07			0x000000FF
#define prtClpDat			PORTE

#else	// _CONTIGUOUS_DATA not defined
// The default for not contiguous data pins
// PmodCLP, connectors J1 = JC and J2 = JD
/*
Data Bus: J1.1, 2, 3, 4, 7, 8, 9, 10
JC-01 TRD1/RG12
JC-02 TRD0/RG13
JC-03 TRD2/RG14
JC-04 RG15
JC-07 PMD8/RG0
JC-08 PMD9/RG1
JC-09 PMD11/RF0
JC-10 PMD10/RF1

RS: J2.1
JD-07 PMD14/CN15/RD6

R/W: J2.2
JD-08 OC3/RD2

E: J2.3
JD-09 IC3/SCK1/PMCS2/PMA15/RD10

BL: J2.4
JD-10 T3CK/RC2
*/

// Edit the following definitions if other than JA_JB, JC-JD connectors are used
//	RS - JD-07 PMD14/CN15/RD6
#define	trisClpRSSet	TRISDSET
#define	trisClpRSClr	TRISDCLR
#define	prtClpRSSet		PORTDSET
#define	prtClpRSClr		PORTDCLR
#define	bnClpRS			6			


//	RW - JD-08 OC3/RD2
#define	trisClpRWSet	TRISDSET
#define	trisClpRWClr	TRISDCLR
#define	prtClpRWSet		PORTDSET
#define	prtClpRWClr		PORTDCLR
#define	bnClpRW			2			

//	E - JD-09 IC3/SCK1/PMCS2/PMA15/RD10
#define	trisClpESet		TRISDSET
#define	trisClpEClr		TRISDCLR
#define	prtClpESet		PORTDSET
#define	prtClpEClr		PORTDCLR
#define	bnClpE			10		

//	BL - JD-10 T3CK/RC2
#define	trisClpBLSet	TRISCSET
#define	trisClpBLClr	TRISCCLR
#define	prtClpBLSet		PORTCSET
#define	prtClpBLClr		PORTCCLR
#define	bnClpBL			2	

// D0 - JC-01 TRD1/RG12
#define	trisClpD0Set	TRISGSET
#define	trisClpD0Clr	TRISGCLR
#define	prtClpD0Set		PORTGSET
#define	prtClpD0Clr		PORTGCLR
#define prtClpD0		PORTG
#define	bnClpD0			12

			
// D1 - JC-02 TRD0/RG13
#define	trisClpD1Set	TRISGSET
#define	trisClpD1Clr	TRISGCLR
#define	prtClpD1Set		PORTGSET
#define	prtClpD1Clr		PORTGCLR
#define prtClpD1		PORTG
#define	bnClpD1			13

// D2 - JC-03 TRD2/RG14
#define	trisClpD2Set	TRISGSET
#define	trisClpD2Clr	TRISGCLR
#define	prtClpD2Set		PORTGSET
#define	prtClpD2Clr		PORTGCLR
#define prtClpD2		PORTG
#define	bnClpD2			14

// D3 - JC-04 RG15
#define	trisClpD3Set	TRISGSET
#define	trisClpD3Clr	TRISGCLR
#define	prtClpD3Set		PORTGSET
#define	prtClpD3Clr		PORTGCLR
#define prtClpD3		PORTG
#define	bnClpD3			15

// D4 - JC-07 PMD8/RG0
#define	trisClpD4Set	TRISGSET
#define	trisClpD4Clr	TRISGCLR
#define	prtClpD4Set		PORTGSET
#define	prtClpD4Clr		PORTGCLR
#define prtClpD4		PORTG
#define	bnClpD4			0

// D5 - JC-08 PMD9/RG1
#define	trisClpD5Set	TRISGSET
#define	trisClpD5Clr	TRISGCLR
#define	prtClpD5Set		PORTGSET
#define	prtClpD5Clr		PORTGCLR
#define prtClpD5		PORTG
#define	bnClpD5			1

// D6 - JC-09 PMD11/RF0
#define	trisClpD6Set	TRISFSET
#define	trisClpD6Clr	TRISFCLR
#define	prtClpD6Set		PORTFSET
#define	prtClpD6Clr		PORTFCLR
#define prtClpD6		PORTF
#define	bnClpD6			0

// D7 - JC-10 PMD10/RF1
#define	trisClpD7Set	TRISFSET
#define	trisClpD7Clr	TRISFCLR
#define	prtClpD7Set		PORTFSET
#define	prtClpD7Clr		PORTFCLR
#define prtClpD7		PORTF
#define	bnClpD7			1

#endif	// _CONTIGUOUS_DATA

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

#endif // _CLP_CONFIG_INC
