/************************************************************************/
/*																		*/
/*	LCDP.h	--	Declaration for LCDP library 	    					*/
/*																		*/
/************************************************************************/
/*	Author:		Cristian Fatu											*/
/*	Copyright 2011, Digilent Inc.										*/
/************************************************************************/
/*  File Description:													*/
/*		This file declares LCDP library functions and constants involved*/
/*																		*/
/************************************************************************/
/*  Revision History:													*/
/*																		*/
/*	12/10/2011(CristianF): created										*/
/*																		*/
/************************************************************************/
#if !defined(LCDP_H)
#define LCDP_H


/* ------------------------------------------------------------ */
/*				Include File Definitions						*/
/* ------------------------------------------------------------ */
#include <inttypes.h>


/* ------------------------------------------------------------ */
/*					Definitions									*/
/* ------------------------------------------------------------ */

/* ------------------------------------------------------------ */
/*					Errors Definitions							*/
/* ------------------------------------------------------------ */

/* ------------------------------------------------------------ */
/*		Command codes Definitions								*/
/* ------------------------------------------------------------ */
#define LCDP_CMD_LcdFcnInit 	0x38	// function set command, (8-bit interface, 2 lines, and 5x8 dots)
#define LCDP_CMD_LcdCtlInit 	0x08	// display control set command
#define LCDP_CMD_LcdClear		0x01	// clear display command
#define LCDP_CMD_LcdRetHome		0x02	// return home command
#define LCDP_CMD_LcdDisplayShift 0x18	// shift display command
#define LCDP_CMD_LcdCursorShift  0x10	// shift cursor command
#define LCDP_CMD_LcdSetDdramPos	0x80	// set DDRAM position command
#define LCDP_CMD_LcdSetCgramPos	0x40	// set CGRAM position command

#define LCDP_MSK_BStatus 0x80		// bit busy 
#define LCDP_MSK_ShiftRL 0x04		// shift direction mask
#define LCDP_OPT_DisplayOn	0x4 	// Set Display On option
#define	LCDP_OPT_CursorOn 	0x2 	// Set Cursor On option
#define	LCDP_OPT_BlinkOn 	0x1 	// Set Blink On option


// bkl, rs, rw, enable, d0, d1, d2, d3, d4, d5, d6, d7

#define LCDP_32MX4CK_JA_JB_ARGS 15, 12, 13, 14,  0,  1,  2,  3,  4,  5,  6,  7
#define LCDP_32MX4CK_JB_JC_ARGS 23, 20, 21, 22,  8,  9, 10, 11, 12, 13, 14, 15
#define LCDP_32MX4CK_JC_JD_ARGS 31, 28, 29, 30, 16, 17, 18, 19, 20, 21, 22, 23
#define LCDP_32MX4CK_JE_JF_ARGS 47, 44, 45, 46, 32, 33, 34, 35, 36, 37, 38, 39
#define LCDP_32MX4CK_JH_JJ_ARGS 63, 60, 61, 62, 48, 49, 50, 51, 52, 53, 54, 55
#define LCDP_32MX4CK_JJ_JK_ARGS 71, 68, 69, 70, 56, 57, 58, 59, 60, 61, 62, 63


/* ------------------------------------------------------------ */
/*				Parameters Definitions							*/
/* ------------------------------------------------------------ */
#define LCDP_DISP_SetOptionDisplayOn	0x4 // Set Display On option
#define	LCDP_DISP_SetOptionCursorOn 	0x2 // Set Cursor On option
#define	LCDP_DISP_SetBlinkOn 			0x1 // Set Blink On option
/* ------------------------------------------------------------ */
/*					Class Declarations							*/
/* ------------------------------------------------------------ */
#define	trisLCDPDatSet		TRISESET
#define	trisLCDPDatClr		TRISECLR
#define	prtLCDPDatSet		PORTESET
#define	prtLCDPDatClr		PORTECLR
#define	mskLCDPDat07		0x000000FF
#define prtLCDPDat			PORTE

#define LCDP_ERR_SUCCESS 				0 		// The action completed successfully 
#define LCDP_ERR_UCHAR_POSITION_INVALID	0x20 	// The user character position is not correct
#define LCDP_ERR_ARG_ROW_RANGE 			0x80 	// The row index is not valid
#define LCDP_ERR_ARG_COL_RANGE 			0x40 	// The column index is not valid 

#define LCDP_NO_ROWS	2
#define LCDP_NO_COLS	40
#define LCDP_NO_UCHARS	8

class LCDP {
private:
	bool m_fDirectPortJA;
	uint8_t m_bkl_pin; // backlight
	uint8_t m_rs_pin; // LOW: command.  HIGH: character.
	uint8_t m_rw_pin; // LOW: write to LCD.  HIGH: read from LCD.
	uint8_t m_enable_pin; // activated by a HIGH pulse.
	uint8_t m_data_pins[8];
	
	uint8_t m_bDisplayMode;

	uint8_t ReadByte();
	void WriteByte(uint8_t bData);

	void WaitUntilNotBusy();
	void WriteCommand(uint8_t bCmd);
	void WriteDataByte(uint8_t bData);

	void SetWriteCgramPosition(uint8_t bAdr);
	void SetWriteDdramPosition(uint8_t bAdr);
	uint8_t ReadStatus();
public:

	LCDP();

	void begin(uint8_t bkl, uint8_t rs, uint8_t rw, uint8_t enable, uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3, uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7);

	void DisplayClear();
	void ReturnHome();
	void SetDisplay(bool fDisplayOn);
	void SetCursor(bool fCursorOn);
	void SetBlink(bool fBlinkOn);	
	void SetBacklight(bool fBacklightOn);
	uint8_t SetPos(uint8_t idxLine, uint8_t idxCol);	
	uint8_t WriteStringAtPos(char *szLn, uint8_t idxLine, uint8_t idxCol);
	void DisplayShift(bool fRight);
	void CursorShift(bool fRight);
	uint8_t DefineUserChar(uint8_t *pBytes, uint8_t bCharNo);
	uint8_t WriteUserCharsAtPos(uint8_t* rgCharPos, uint8_t bNoChars, uint8_t idxLine, uint8_t idxCol) ;
	};



#endif