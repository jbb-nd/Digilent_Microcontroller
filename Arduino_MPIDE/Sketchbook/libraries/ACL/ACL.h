/************************************************************************/
/*																		*/
/*	ACL.h	--	Declaration for ACL library 	    					*/
/*																		*/
/************************************************************************/
/*	Author:		Cristian Fatu											*/
/*	Copyright 2011, Digilent Inc.										*/
/************************************************************************/
/*  File Description:													*/
/*		This file declares ACL library functions and constants involved	*/
/*																		*/
/************************************************************************/
/*  Revision History:													*/
/*																		*/
/*	09/09/2011(CristianF): created										*/
/*																		*/
/************************************************************************/
#if !defined(ACL_H)
#define ACL_H


/* ------------------------------------------------------------ */
/*				Include File Definitions						*/
/* ------------------------------------------------------------ */
#include <inttypes.h>
#include <DSPI.h>

/* ------------------------------------------------------------ */
/*					Definitions									*/
/* ------------------------------------------------------------ */
#define ACL_NO_BITS		10
#define ACL_I2C_ADDR	0x1D

#define ACL_SPI_FREQ	1000000 // 1 MHz - normal spi freq

#define ACL_CONV_TRESH_G_LSB (62.5 * 0.001)	// convert treshold (g) to LSB, 62.5 mg/LSB
#define ACL_CONV_DURATION_S_LSB (625*0.000001) 	// convert duration (s) to LSB, 625 us/LSB
#define ACL_CONV_LATENT_S_LSB (1.25 * 0.001) 	// convert latent (s) to LSB, 1.25 ms/LSB
#define ACL_CONV_WINDOW_S_LSB (1.25 * 0.001) 	// convert window (s) to LSB, 1.25 ms/LSB
#define ACL_CONV_TIMEFREEFALL_S_LSB (5 * 0.001) 	// convert freefall time (s) to LSB, 5 ms/LSB
#define ACL_CONV_OFFSET_G_LSB (15.6 * 0.001) 	// convert offset (g) to LSB, 15.6 mg/LSB

/* ------------------------------------------------------------ */
/*					Errors Definitions							*/
/* ------------------------------------------------------------ */

#define ACL_ERR_SUCCESS				0	// The action completed successfully
#define ACL_ERR_ARG_RANGE_PM2G		1	// The argument is not within -2g, 2g range
#define ACL_ERR_ARG_RANGE_016G		2	// The argument is not within 0, 16g range
#define ACL_ERR_ARG_RANGE_0160MS	3	// The argument is not within 0, 160ms range
#define ACL_ERR_ARG_RANGE_0320MS	4	// The argument is not within 0, 320ms range
#define ACL_ERR_ARG_RANGE_0128S		5	// The argument is not within 0, 1.28s range
#define ACL_ERR_ARG_RANGE_015		6	// The argument is not within 0, 15 range
#define ACL_ERR_ARG_RANGE_DF_03		7	// The argument is not within 0, 3 range
#define ACL_ERR_INVALID_THRESH		8	// The argument is not within 0, 3 range
#define ACL_ERR_ARG_RANGE_WU_03		9	// The argument is not within 0, 3 range
#define ACL_ERR_ARG_RANGE_05		10	// The argument is not within 0, 5 range
#define ACL_ERR_INVALID_BIT_SET		11	// The argument is not within 0, 11 range
#define ACL_ERR_INVALID_BIT_GET		12	// The argument is not within 0, 12 range
#define ACL_ERR_ARG_RANGE_FIFO_03	13	// The argument is not within 0, 3 range
#define ACL_ERR_INVALID_TIME		14	// The argument is not within 0, 3 range
#define ACL_ERR_INVALID_AXIS		16	// The argument is not within 0, 3 range
#define ACL_ERR_ARG_AINTNO01_BIT	32	// The argument is not within 0, 2 range (ACL Interrupt no)
#define ACL_ERR_ARG_EINTNO04_BIT	64	// The argument is not within 0, 4 range (External Interrupt no)
#define ACL_ERR_ARG_ACT_BIT			128	// The argument is not within 0, 1 range (Active)

/* ------------------------------------------------------------ */
/*		Register addresses Definitions							*/
/* ------------------------------------------------------------ */

#define ACL_BIT_BW_RATE_LOWPOWER		0	// LOW_POWER bit of the BW_RATE register
#define ACL_BIT_POWER_CTL_LINK			1   // Link bit of the POWER_CTL register
#define ACL_BIT_POWER_CTL_AUTOSLEEP		2   // AUTO_SLEEP bit of the POWER_CTL register
#define ACL_BIT_POWER_CTL_MEASURE 		3   // Measure bit of the POWER_CTL register
#define ACL_BIT_POWER_CTL_SLEEP			4   // Sleep bit of the POWER_CTL register
#define ACL_BIT_DATA_FORMAT_SELFTEST	5   // SELF_TEST bit of the DATA_FORMAT register
#define ACL_BIT_DATA_FORMAT_SPI			6   // SPI bit of the DATA_FORMAT register
#define ACL_BIT_DATA_FORMAT_INTINVERT	7   // INT_INVERT bit of the DATA_FORMAT register
#define ACL_BIT_DATA_FORMAT_AUTOSLEEP	8   // AUTO_SLEEP bit of the DATA_FORMAT register
#define ACL_BIT_DATA_FORMAT_FULLRES 	9   // FULL_RES bit of the DATA_FORMAT register
#define ACL_BIT_DATA_FORMAT_JUSTIFY 	10  // Justify bit of the DATA_FORMAT register
#define ACL_BIT_FIFO_CTL_TRIGGER		11  // Trigger bit of the FIFO_CTL register
#define ACL_BIT_FIFO_STATUS_FIFOTRIG	12  // FIFO_TRIG bit of the FIFO_STATUS register
/* ------------------------------------------------------------ */
/*		Register addresses Definitions							*/
/* ------------------------------------------------------------ */

#define	ACL_ADR_DEVID 			0x00
#define	ACL_ADR_THRESH_TAP 		0x1D
#define	ACL_ADR_OFSX			0x1E
#define	ACL_ADR_OFSY 			0x1F
#define	ACL_ADR_OFSZ 			0x20
#define	ACL_ADR_DUR 			0x21
#define	ACL_ADR_LATENT			0x22
#define	ACL_ADR_WINDOW			0x23
#define	ACL_ADR_THRESH_ACT 		0x24
#define	ACL_ADR_THRESH_INACT 	0x25
#define	ACL_ADR_TIME_INACT 		0x26
#define	ACL_ADR_ACT_INACT_CTL 	0x27
#define	ACL_ADR_THRESH_FF 		0x28
#define	ACL_ADR_TIME_FF 		0x29
#define	ACL_ADR_TAP_AXES 		0x2A
#define	ACL_ADR_ACT_TAP_STATUS 	0x2B
#define	ACL_ADR_BW_RATE 		0x2C
#define	ACL_ADR_POWER_CTL 		0x2D
#define	ACL_ADR_INT_ENABLE 		0x2E
#define	ACL_ADR_INT_MAP 		0x2F
#define	ACL_ADR_INT_SOURCE 		0x30
#define	ACL_ADR_DATA_FORMAT 	0x31
#define	ACL_ADR_DATAX0			0x32
#define	ACL_ADR_DATAX1			0x33
#define	ACL_ADR_DATAY0			0x34
#define	ACL_ADR_DATAY1			0x35
#define	ACL_ADR_DATAZ0			0x36
#define	ACL_ADR_DATAZ1			0x37
#define	ACL_ADR_FIFO_CTL 		0x38
#define	ACL_ADR_FIFO_STATUS		0x39

/* ------------------------------------------------------------ */
/*				Bit masks Definitions							*/
/* ------------------------------------------------------------ */
#define	MSK_ALLBITS						0xFF
#define	MSK_INT_DATA_READY				1<<7
#define	MSK_INT_SINGLE_TAP				1<<6
#define	MSK_INT_DOUBLE_TAP				1<<5
#define	MSK_INT_ACTIVITY				1<<4
#define	MSK_INT_INACTIVITY				1<<3
#define	MSK_INT_FREE_FALL				1<<2
#define	MSK_INT_WATERMARK				1<<1
#define	MSK_INT_OVERRUN					1<<0
#define	MSK_ACT_INACT_ACTACDC			1<<7
#define	MSK_ACT_INACT_ACTXEN			1<<6
#define	MSK_ACT_INACT_ACTYEN			1<<5
#define	MSK_ACT_INACT_ACTZEN			1<<4
#define	MSK_ACT_INACT_INACTACDC			1<<3
#define	MSK_ACT_INACT_INACTXEN			1<<2
#define	MSK_ACT_INACT_INACTYEN			1<<1
#define	MSK_ACT_INACT_INACTZEN			1<<0
#define	MSK_ACT_TAP_STATUS_ACTXSOURCE	1<<6
#define	MSK_ACT_TAP_STATUS_ACTYSOURCE	1<<5
#define	MSK_ACT_TAP_STATUS_ACTZSOURCE	1<<4
#define	MSK_ACT_TAP_STATUS_ASLEEP		1<<3
#define	MSK_ACT_TAP_STATUS_INACTXSOURCE	1<<2
#define	MSK_ACT_TAP_STATUS_INACTYSOURCE	1<<1
#define	MSK_ACT_TAP_STATUS_INACTZSOURCE	1<<0
#define	MSK_TAP_AXES_SUPPRESS			1<<3
#define	MSK_TAP_AXES_TAPXENABLE			1<<2
#define	MSK_TAP_AXES_TAPYENABLE			1<<1
#define	MSK_TAP_AXES_TAPZENABLE			1<<0
#define	MSK_BW_RATE_LOWPOWER			1<<4
#define	MSK_BW_RATE_RATE				0x0F
#define	MSK_POWER_CTL_LINK				1<<5
#define	MSK_POWER_CTL_AUTOSLEEP			1<<4
#define	MSK_POWER_CTL_MEASURE			1<<3
#define	MSK_POWER_CTL_SLEEP				1<<2
#define	MSK_POWER_CTL_WAKEUP			3
#define	MSK_DATA_FORMAT_SELFTEST		1<<7
#define	MSK_DATA_FORMAT_SPI				1<<6
#define	MSK_DATA_FORMAT_INTINVERT		1<<5
#define	MSK_DATA_FORMAT_AUTOSLEEP		1<<4
#define	MSK_DATA_FORMAT_FULLRES			1<<3
#define	MSK_DATA_FORMAT_JUSTIFY			1<<2
#define	MSK_DATA_FORMAT_RANGE			3
#define	MSK_FIFO_CTL_FIFOMODE			0xC0
#define	MSK_FIFO_CTL_TRIGGER			1<<5
#define	MSK_FIFO_CTL_SAMPLES			0x1F
#define	MSK_FIFO_STATUS_FIFOTRIG		1<<5
#define	MSK_FIFO_STATUS_ENTRIES			0x3F

/* ------------------------------------------------------------ */
/*				Parameters Definitions							*/
/* ------------------------------------------------------------ */
#define	PAR_INT_ACTIVEHIGH		0
#define	PAR_INT_ACTIVELOW		1
#define	PAR_ACL_INT1			0
#define	PAR_ACL_INT2			1
#define	PAR_EXT_INT0			0
#define	PAR_EXT_INT1			1
#define	PAR_EXT_INT2			2
#define	PAR_EXT_INT3			3
#define	PAR_EXT_INT4			4
#define	PAR_ACCESS_SPI0			0
#define	PAR_ACCESS_SPI1			1
#define	PAR_ACCESS_I2C			2	
#define	PAR_OUTPUTRATE0_10Hz	0
#define	PAR_OUTPUTRATE0_20Hz	1
#define	PAR_OUTPUTRATE0_39Hz	2
#define	PAR_OUTPUTRATE0_78Hz	3
#define	PAR_OUTPUTRATE1_56Hz	4
#define	PAR_OUTPUTRATE3_13Hz	5
#define	PAR_OUTPUTRATE6_25Hz	6
#define	PAR_OUTPUTRATE12_50Hz	7
#define	PAR_OUTPUTRATE25_00Hz	8
#define	PAR_OUTPUTRATE50_00Hz	9
#define	PAR_OUTPUTRATE100_00Hz	10
#define	PAR_OUTPUTRATE200_00Hz	11
#define	PAR_OUTPUTRATE400_00Hz	12
#define	PAR_OUTPUTRATE800_00Hz	13
#define	PAR_OUTPUTRATE1600_00Hz	14
#define	PAR_OUTPUTRATE3200_00Hz	15
#define	PAR_WAKEUP_FREQ8Hz		0
#define	PAR_WAKEUP_FREQ4Hz		1
#define	PAR_WAKEUP_FREQ2Hz		2
#define	PAR_WAKEUP_FREQ1Hz		3
#define	PAR_DATAFORMAT_PM2G		0
#define	PAR_DATAFORMAT_PM4G		1
#define	PAR_DATAFORMAT_PM8G		2
#define	PAR_DATAFORMAT_PM16G	3
#define	FIFO_MODE_BYPASS		0
#define	FIFO_MODE_FIFO			1
#define	FIFO_MODE_STREAM		2
#define	FIFO_MODE_TRIGGER		3
#define	PAR_AXIS_XP				0
#define	PAR_AXIS_XN				1
#define	PAR_AXIS_YP				2
#define	PAR_AXIS_YN				3
#define	PAR_AXIS_ZP				4
#define	PAR_AXIS_ZN				5
#define	PAR_AXIS_X				0
#define	PAR_AXIS_Y				1
#define	PAR_AXIS_Z				2
#define	PAR_THRESH_TAP			0
#define	PAR_THRESH_ACT			1
#define	PAR_THRESH_INACT		2
#define	PAR_THRESH_FF			3
#define	PAR_TIME_DUR			0
#define	PAR_TIME_LATENT			1
#define	PAR_TIME_WINDOW			2
#define	PAR_TIME_FF				3

/* ------------------------------------------------------------ */
/*					Procedure Declarations						*/
/* ------------------------------------------------------------ */


class ACL {
private: 
	DSPI *pdspi;
	uint8_t m_SSPin;
	float m_dGRangeLSB;
	uint8_t m_bAccessType;

 	void WriteBytesSPI(uint8_t bAddress, uint8_t bCntBytes, uint8_t *rgbValues);
	void ReadBytesSPI(uint8_t bAddress, uint8_t bCntBytes, uint8_t *rgbValues);
	void WriteBytesI2C(uint8_t bAddress, uint8_t bCntBytes, uint8_t *rgbValues);
	void ReadBytesI2C(uint8_t bAddress, uint8_t bCntBytes, uint8_t *rgbValues);
	void SetRegisterBits(uint8_t bRegisterAddress, uint8_t bMask, bool fValue);
	uint8_t GetRegisterBits(uint8_t bRegisterAddress, uint8_t bMask);
	void SetRegisterBitsFromByte(uint8_t bRegisterAddress, uint8_t bMask, uint8_t bValue);
	float GetGRangeLSB(uint8_t nGRangePar);
	float ConvertReadingToValueG(int16_t uiReading);
	void WriteRegisters(uint8_t bRegisterAddress, uint8_t bCntBytes, uint8_t *rgbValues);
	void ReadRegisters(uint8_t bRegisterAddress, uint8_t bCntBytes, uint8_t *rgbValues);

public:
	ACL();
	void begin(uint8_t bAccessType);
	void end();
	void ReadAccelG(float &dAclXg, float &dAclYg, float &dAclZg); 
	float ReadAccelGOneAxis(uint8_t bAxisParam, uint8_t *pErr);
	uint8_t GetDevID();
		
	uint8_t SetDataFormatGRangePar(uint8_t bDataFormatGRangePar);
	uint8_t GetDataFormatGRangePar();

	uint8_t SetSFRBit(uint8_t bSFRBit, bool fValue);
	bool GetSFRBit(uint8_t bSFRBit, uint8_t *pErr = NULL);

	uint8_t SetOffsetG(uint8_t bAxisParam, float dOffset);
	float GetOffsetG(uint8_t bAxisParam, uint8_t *pErr);

	uint8_t SetThresholdG(uint8_t bThreshParam, float dTreshVal);	
	float GetThresholdG(uint8_t bThreshParam, uint8_t *pErr);	

	uint8_t SetTimeS(uint8_t bTimeParam, float dTime);
	float GetTimeS(uint8_t bTimeParam, uint8_t *pErr);
	
	uint8_t SetBWRateOutputRatePar(uint8_t bOutputRatePar);
	uint8_t GetBWRateOutputRatePar();

	uint8_t SetPowerControlWakeupFreqPar(uint8_t bPowerControlWakeupFreqPar);
	uint8_t GetPowerControlWakeupFreqPar();
		
	uint8_t SetFIFOControlFIFOModePar(uint8_t bFIFOControlFIFOModePar);
	uint8_t GetFIFOControlFIFOModePar();
	
	void SetFIFOControlSamplesVal(uint8_t bFIFOControlSamplesVal);
	uint8_t GetFIFOControlSamplesVal();
	
	void SetActiveInactiveControlBits(uint8_t bActiveInactiveControlMask, bool fValue);
	uint8_t GetActiveInactiveControlBits(uint8_t bActiveInactiveControlMask);
	
	uint8_t GetActTapStatusBits(uint8_t bActTapStatusMask);
	
	void SetTapAxesBits(uint8_t bTapAxesMask, bool fValue);
	uint8_t GetTapAxesBits(uint8_t bTapAxesMask);
	
	uint8_t ConfigureInterrupt(uint8_t bParACLIntNo, uint8_t bParExtIntNo, uint8_t bEventMask, void (*pfIntHandler)(), uint8_t bActiveType);
	uint8_t GetInterruptSourceBits(uint8_t bEventMask);
	uint8_t GetIntEnableEventBits(uint8_t bEventMask);
	bool GetIntMapEventBits(uint8_t bParIntNo, uint8_t bEventMask, uint8_t *pErr = NULL);
	
	uint8_t GetFIFOStatusEntriesVal();	

	uint8_t CalibrateOneAxisGravitational(uint8_t bAxisInfo);
};



#endif