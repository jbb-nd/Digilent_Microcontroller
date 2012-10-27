/************************************************************************/
/*																		*/
/*	ADCSPI.h	--	Declaration for ADCSPI library 	    				*/
/*																		*/
/************************************************************************/
/*	Author:		Cristian Fatu											*/
/*	Copyright 2012, Digilent Inc.										*/
/************************************************************************/
/*  File Description:													*/
/*	This file declares the ADCSPI library functions and the constants	*/
/*	involved.															*/
/*																		*/
/************************************************************************/
/*  Revision History:													*/
/*																		*/
/*	01/10/2012(CristianF): created										*/
/*																		*/
/************************************************************************/
#if !defined(ADCSPI_H)
#define ADCSPI_H

/* ------------------------------------------------------------ */
/*				Include File Definitions						*/
/* ------------------------------------------------------------ */
#include <inttypes.h>
#include <DSPI.h>

/* ------------------------------------------------------------ */
/*					Definitions									*/
/* ------------------------------------------------------------ */
#define ADCSPI_NO_BITS		12

#define ADCSPI_SPI_FREQ	1000000 // 1 MHz - default spi freq
#define	PAR_ACCESS_SPI0			0
#define	PAR_ACCESS_SPI1			1
#define	PAR_ACCESS_I2C			2	
/* ------------------------------------------------------------ */
/*					Procedure Declarations						*/
/* ------------------------------------------------------------ */


class ADCSPI {
private: 
	DSPI *pdspi;
	uint8_t m_SSPin;

public:
	uint16_t GetIntegerValue();
	float GetPhysicalValue(float dReference = 3.3);

	ADCSPI();
	void begin(uint8_t bAccessType);
};



#endif