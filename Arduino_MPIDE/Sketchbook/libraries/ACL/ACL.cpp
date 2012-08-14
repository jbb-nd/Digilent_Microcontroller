/************************************************************************/
/*																		*/
/*	ACL.cpp		--		Definition for ACL library 	    				*/
/*																		*/
/************************************************************************/
/*	Author:		Cristian Fatu											*/
/*	Copyright 2011, Digilent Inc.										*/
/************************************************************************/
/*  File Description:													*/
/*		This file defines functions for ACL							*/
/*																		*/
/************************************************************************/
/*  Revision History:													*/
/*																		*/
/*	09/09/2011(CristianF): created										*/
/*																		*/
/************************************************************************/


/* ------------------------------------------------------------ */
/*				Include File Definitions						*/
/* ------------------------------------------------------------ */
#include "ACL.h"
#include <Dspi.h>
#include <Wire.h>

/* ------------------------------------------------------------ */
/*				Procedure Definitions							*/
/* ------------------------------------------------------------ */


/* ------------------------------------------------------------ */
/*        ACL::ACL
**
**        Synopsis:
**				
**        Parameters:
**
**
**
**        Return Values:
**                void 
**
**        Errors:
**
**
**        Description:
**			Class constructor. Performs variables initialization tasks
**
**
*/
ACL::ACL()
{
	pdspi = NULL;
	m_bAccessType = -1;
	m_dGRangeLSB = 0;
}

/* ------------------------------------------------------------ */
/*        ACL::ACLWriteBytesSPI
**
**        Synopsis:
**				ACLWriteBytesSPI(bRegisterAddress, bCntBytes,  rgbValues);
**        Parameters:
**				uint8_t bAddress – the address where the values will be written
**				uint8_t bCntBytes – the number of bytes that will be written
**				uint8_t *rgbValues - the array of values to be written
**
**
**        Return Values:
**                void 
**
**        Errors:
**			If module is not initialized (using begin), the function does nothing
**
**        Description:
**			This function writes the values from the buffer to the specified number of registers starting from a specified address value. 
**				It performs the SPI write cycle for the specified array of values to the specified address.
**
**
*/
void ACL::WriteBytesSPI(uint8_t bAddress, uint8_t bCntBytes, uint8_t *rgbValues)
{
	if(pdspi != NULL)
	{
		// make SS active
		digitalWrite(m_SSPin, LOW);		
		// As requested by documentation, first byte contains:
		//	bit 7 = 0 because is a write operation
		//	bit 6 = 1 if more than one bytes is written, 0 if a single byte is written
		// 	bits 5-0 - the address
		uint8_t bFirstByte = (bCntBytes > 1 ? 0x40: 0) | (bAddress & 0x3F);
		pdspi->transfer(bFirstByte);
		// transfer the rest of the bytes
		int nIdxBytes;
		for(nIdxBytes = 0; nIdxBytes < bCntBytes; nIdxBytes++)
		{
			pdspi->transfer(rgbValues[nIdxBytes]);
		}
		
		// make SS inactive
		digitalWrite(m_SSPin, HIGH);
	}
}

/* ------------------------------------------------------------ */
/*        ACL::ReadBytesSPI
**
**        Synopsis:
**				ACLReadBytesSPI(bRegisterAddress, bCntBytes, rgbValues);
**        Parameters:
**				uint8_t bAddress – the address from where the values will be read
**				uint8_t bCntBytes – the number of bytes that will be read
**				uint8_t *rgbValues - the array where values will be read
**
**
**        Return Values:
**                void 
**
**        Errors:
**			If module is not initialized (using begin), the function does nothing
**
**        Description:
**				This function will read the specified number of registers starting from a specified address and store their values in the buffer. 
**					It performs the SPI read cycle from the specified address into the specified array of values.
**
**
*/
void ACL::ReadBytesSPI(uint8_t bAddress, uint8_t bCntBytes, uint8_t *rgbValues)
{
	if(pdspi != NULL)
	{
		// make SS active
		digitalWrite(m_SSPin, LOW);

		// As requested by documentation, first byte contains:
		//	bit 7 = 1 because is a read operation
		//	bit 6 = 1 if more than one bytes is written, 0 if a single byte is written
		// 	bits 5-0 - the address
		uint8_t bFirstByte = (bCntBytes > 1 ? 0xE0: 0x80) | (bAddress & 0x3F);
		pdspi->transfer(bFirstByte);

		// transfer the rest of the bytes
		int nIdxBytes;
		for(nIdxBytes = 0; nIdxBytes < bCntBytes; nIdxBytes++)
		{
			rgbValues[nIdxBytes] = pdspi->transfer(0);
		}
		
		// make SS inactive
		digitalWrite(m_SSPin, HIGH);
	}
}



/* ------------------------------------------------------------ */
/*        ACL::WriteBytesI2C
**
**        Synopsis:
**				ACLWriteBytesI2C(bRegisterAddress, bCntBytes,  rgbValues);
**        Parameters:
**				uint8_t bAddress – the address where the values will be written
**				uint8_t bCntBytes – the number of bytes that will be written
**				uint8_t *rgbValues - the array of values to be written
**
**
**        Return Values:
**                void 
**
**        Errors:
**
**
**        Description:
**			This function writes the values from the buffer to the specified number of registers starting from a specified address value. 
**				It performs the I2C write cycle for the specified array of values to the specified address.
**
**
*/
void ACL::WriteBytesI2C(uint8_t bAddress, uint8_t bCntBytes, uint8_t *rgbValues)
{
    Wire.beginTransmission(ACL_I2C_ADDR); //start transmission to device 
    Wire.send(bAddress);        // send register address
	int nIdxBytes;
	for(nIdxBytes = 0; nIdxBytes < bCntBytes; nIdxBytes++)
	{
		Wire.send(rgbValues[nIdxBytes]); // send value to write
	}	
	Wire.endTransmission(); //end transmission
}





/* ------------------------------------------------------------ */
/*        ACL::ReadBytesI2C
**
**        Synopsis:
**				ACLReadBytesI2C(bRegisterAddress, bCntBytes, rgbValues);
**        Parameters:
**				uint8_t bAddress – the address from where the values will be read
**				uint8_t bCntBytes – the number of bytes that will be read
**				uint8_t *rgbValues - the array where values will be read
**
**
**        Return Values:
**                void 
**
**        Errors:
**
**
**        Description:
**				This function will read the specified number of registers starting from a specified address and store their values in the buffer. 
**					It performs the I2C read cycle from the specified address into the specified array of values.
**
**
*/
void ACL::ReadBytesI2C(uint8_t bAddress, uint8_t bCntBytes, uint8_t *rgbValues)
{
	Wire.beginTransmission(ACL_I2C_ADDR); 	//start transmission to idDevice 
	Wire.send(bAddress);        			//send address to read from
	Wire.endTransmission(); 				//end transmission
                 		
	Wire.beginTransmission(ACL_I2C_ADDR); 	//start transmission to idDevice (initiate again)
	Wire.requestFrom(ACL_I2C_ADDR, (int)bCntBytes);    // request bCntBytes bytes from idDevice
                	
	int nIdxBytes = 0;
	while(Wire.available())    //idDevice may send less than requested (abnormal)
	{ 
		rgbValues[nIdxBytes] = Wire.receive(); // receive a byte
		nIdxBytes++;
	}
	Wire.endTransmission(); //end transmission
}



/* ------------------------------------------------------------ */
/*        ACL::WriteRegisters
**
**        Synopsis:
**				ACLWriteRegisters(bRegisterAddress, 1, &bRegValue);
**        Parameters:
**				uint8_t bRegisterAddress – the address where the values will be written
**				uint8_t bCntBytes – the number of bytes that will be written
**				uint8_t *rgbValues - the array of values to be written**
**
**        Return Values:
**                void 
**
**        Errors:
**
**
**        Description:
**			This function writes the values from the buffer to the specified number of registers starting from a specified address value. 
**			It is a low level function and will be used by a lot of higher level functions. According to the chosen access platform (SPI or I2C), 
**			it will direct the call to the specific function (ACLWriteBytesSPI or ACLWriteBytesI2C).
**
**
*/
void ACL::WriteRegisters(uint8_t bRegisterAddress, uint8_t bCntBytes, uint8_t *rgbValues)
{
	if(m_bAccessType == PAR_ACCESS_I2C)
	{
		WriteBytesI2C(bRegisterAddress, bCntBytes,  rgbValues);
	}
	else
	{
		WriteBytesSPI(bRegisterAddress, bCntBytes,  rgbValues);
	}
}


/* ------------------------------------------------------------ */
/*        ACL::ReadRegisters
**
**        Synopsis:
**				ReadRegisters(bRegisterAddress, 1, &bRegValue);
**        Parameters:
**				uint8_t bRegisterAddress – the address from where the values will be read
**				uint8_t bCntBytes – the number of bytes that will be read
**				uint8_t *rgbValues - the array where values will be read
**
**
**        Return Values:
**                void 
**
**        Errors:
**
**
**        Description:
**			This function will read the specified number of registers starting from a specified address and store their values in the buffer. 
**			It is a low level function and will be used by a lot of higher level functions. 
**			According to the chosen access platform (SPI or I2C), it will direct the call to the specific function (ReadBytesSPI or ReadBytesI2C).
**
**
*/
void ACL::ReadRegisters(uint8_t bRegisterAddress, uint8_t bCntBytes, uint8_t *rgbValues)
{
	if(m_bAccessType == PAR_ACCESS_I2C)
	{
		rgbValues[0] = 0x12;
		ReadBytesI2C(bRegisterAddress, bCntBytes, rgbValues);
	}
	else
	{
		ReadBytesSPI(bRegisterAddress, bCntBytes, rgbValues);
	}
}

/* ------------------------------------------------------------ */
/*        ACL::SetRegisterBits
**
**        Synopsis:
**				ACLSetRegisterBits(ACL_ADR_POWER_CTL, bPowerControlMask, fValue);
**        Parameters:
**				uint8_t bRegisterAddress 	- the address of the register whose bits are set
**				uint8_t bMask				- the mask indicating which bits are affected
**				bool fValue					- 1 if the bits are set or 0 if their bits are reset
**
**
**        Return Values:
**                void 
**
**        Errors:
**
**
**        Description:
**				This function sets the value of some bits (corresponding to the bMask) of a register (indicated by bRegisterAddress) to 1 or 0 (indicated by fValue).
**
**
*/
void ACL::SetRegisterBits(uint8_t bRegisterAddress, uint8_t bMask, bool fValue)
{
	uint8_t bRegValue;
	ReadRegisters(bRegisterAddress, 1, &bRegValue);
	if(fValue)
	{
		// set 1 value to the values that are 1 in the mask
		bRegValue |= bMask;
	}
	else
	{
		// set 0 value to the values that are 1 in the mask
		bRegValue &= ~bMask;
	}
	WriteRegisters(bRegisterAddress, 1, &bRegValue);
}


/* ------------------------------------------------------------ */
/*        ACL::SetRegisterBitsFromByte
**
**        Synopsis:
**				ACLSetRegisterBitsFromByte(ACL_ADR_BW_RATE, MSK_BW_RATE_RATE, bOutputRatePar);
**        Parameters:
**				uint8_t bRegisterAddress 	- the address of the register whose bits are set
**				uint8_t bMask				- the mask indicating which bits are affected
**				uint8_t bValue				- the byte containing bits values 
**
**        Return Values:
**                void 
**
**        Errors:
**
**
**        Description:
**				This function sets the value of some bits (correspoding to the bMask) of a register (indicated by bRegisterAddress) to the value of the corresponding bits from another byte (indicated by bValue).
**
**
*/
void ACL::SetRegisterBitsFromByte(uint8_t bRegisterAddress, uint8_t bMask, uint8_t bValue)
{
	uint8_t bRegValue;
	ReadRegisters(bRegisterAddress, 1, &bRegValue);
	// register value: mask out the bits from the mask
	bRegValue &= ~bMask;
	
	// value: mask out the values outside the mask
	bValue &= bMask;
	
	// combine the value with the masked register value
	bRegValue |= (bValue & bMask);
	
	WriteRegisters(bRegisterAddress, 1, &bRegValue);
}


/* ------------------------------------------------------------ */
/*        ACL::GetRegisterBits
**
**        Synopsis:
**				return ACLGetRegisterBits(ACL_ADR_BW_RATE, MSK_BW_RATE_RATE);
**        Parameters:
**				uint8_t bRegisterAddress 	- the address of the register whose bits are read
**				uint8_t bMask				- the mask indicating which bits are read
**
**
**        Return Values:
**                uint8_t 					- a byte containing only the bits correspoding to the mask.
**
**        Errors:
**
**
**        Description:
**				Returns a byte containing only the bits from a register (indicated by bRegisterAddress), correspoding to the bMask mask.
**
**
*/
uint8_t ACL::GetRegisterBits(uint8_t bRegisterAddress, uint8_t bMask)
{
	uint8_t bRegValue;
	ReadRegisters(bRegisterAddress, 1, &bRegValue);
	return bRegValue & bMask;
}

/* ------------------------------------------------------------ */
/*        ACL::ConvertReadingToValueG
**
**        Synopsis:
**				*pdAclXg = ACLConvertReadingToValueG(rgwRegVals[0]);
**        Parameters:
**				int16_t uiReading	- the 2 bytes containing the reading (in fact only 10 bits are used).
**
**
**        Return Values:
**                float - the value of the acceleration in "g" corresponding to the 10 bits reading and the current g range
**
**        Errors:
**
**
**        Description:
**				Converts the value from the 10 bits reading to the float value (in g) corresponding to the acceleration, considering the current selected g range.
**	
**
**
*/
float ACL::ConvertReadingToValueG(int16_t uiReading)
{
	//Convert the accelerometer value to G's. 
  //With 10 (ACL_NO_BITS) bits measuring over a +/- ng range we can find how to convert by using the equation:
  // Gs = Measurement Value * (G-range/(2^10))
  // m_dGRangeLSB is pre-computed in ACLSetDataFormatGRangePar
  float dResult = ((float)uiReading) * m_dGRangeLSB;
  return dResult;
}

/* ------------------------------------------------------------ */
/*        ACL::begin
**
**        Synopsis:
**				myACL.begin(PAR_ACCESS_I2C);
**        Parameters:
**				uint8_t bAccessType	- the type of the communication with the PMod. Can be one of:
**					0	PAR_ACCESS_SPI0	- indicates SPI access, port 1
**					1	PAR_ACCESS_SPI1	- indicates SPI access, port 2
**					2	PAR_ACCESS_I2C	- indicates I2C access
**
**
**        Return Values:
**                void 
**
**        Errors:
**
**
**        Description:
**				This function performs the required ACL initialization tasks:
**					-	Initializes and configures the SPI or I2C communication instance. It sets the default SPI frequency to 1 MHz.
**					-	Configures the required signals as outputs
**					-	Other initialization tasks 
**
**
*/
void ACL::begin(uint8_t bAccessType)
{
	m_bAccessType = bAccessType;
	m_dGRangeLSB = GetGRangeLSB(PAR_DATAFORMAT_PM2G);	// the startup range for the ADXL345 accelerometer is +/- 2g
	if(m_bAccessType == PAR_ACCESS_I2C)
	{
		Wire.begin();
	}
	else
	{
		if(m_bAccessType == PAR_ACCESS_SPI0)
		{
			pdspi = new DSPI0();
			m_SSPin = PIN_DSPI0_SS;	// default SS pin for SPI0
		}
		if(m_bAccessType == PAR_ACCESS_SPI1)
		{
			pdspi = new DSPI1();
			m_SSPin = PIN_DSPI1_SS;	// default SS pin for SPI1
		}
		pdspi->begin(m_SSPin);	// this defines SS pin as output, sets SS as high
		pdspi->setMode(DSPI_MODE3);
		pdspi->setSpeed(ACL_SPI_FREQ);
	}
}


/* ------------------------------------------------------------ */
/*        ACL::end
**
**        Synopsis:
**				myACL.end();
**        Parameters:
**
**
**
**
**        Return Values:
**                void 
**
**        Errors:
**
**
**        Description:
**				This function performs the required ACL deinitialization tasks.
**
**
*/
void ACL::end()
{
	if(pdspi != NULL)
	{
		delete pdspi;
		pdspi = NULL;
	}
	m_bAccessType = -1;
	m_dGRangeLSB = 0;
}



/* ------------------------------------------------------------ */
/*        ACL::ReadAccelG
**
**        Synopsis:
**				myACL.ReadAccelG(dX, dY, dZ);
**        Parameters:
**				float &dAclXg	- the output parameter that will receive acceleration on X axis (in "g")
**				float &dAclYg	- the output parameter that will receive acceleration on Y axis (in "g")
**				float &dAclZg	- the output parameter that will receive acceleration on Z axis (in "g")
**
**
**        Return Values:
**                void 
**
**        Errors:
**
**
**        Description:
**				This function is the main function used for acceleration reading, providing the 3 current acceleration values in “g”. 
**					-	It reads simultaneously the acceleration on three axes in a buffer of 6 bytes using the ReadRegister
**					-	For each of the three values, combines the two bytes in order to get a 10-bit value
**					-	For each of the three values, converts the 10-bit value to the value expressed in “g”, considering the currently selected g range

**
**
*/
void ACL::ReadAccelG(float &dAclXg, float &dAclYg, float &dAclZg) 
{
	
	uint16_t rgwRegVals[3];
	ReadRegisters(ACL_ADR_DATAX0, 6, (uint8_t *)rgwRegVals);
	dAclXg = ConvertReadingToValueG(rgwRegVals[0]);
	dAclYg = ConvertReadingToValueG(rgwRegVals[1]);
	dAclZg = ConvertReadingToValueG(rgwRegVals[2]);
}


/* ------------------------------------------------------------ */
/*        ACL::ReadAccelGOneAxis
**
**        Synopsis:
**				dX = myACL.ReadAccelGOneAxis(PAR_AXIS_X);
**					or
**				dX = myACL.ReadAccelGOneAxis(PAR_AXIS_X, &err);
**
**        Parameters:
**				uint8_t bAxisParam - byte indicating the axis whose acceleration will be read. Can be one of:
**					PAR_AXIS_X		- indicating X axis				
**                  PAR_AXIS_Y 		- indicating Y axis
**                  PAR_AXIS_Z 		- indicating Z axis
**
**				 uint8_t *pErr 	- optional parameter - pointer to a value that receives error value. User may chose not to provide this parameter.
**					- ACL_ERR_SUCCESS (0) 	- The action completed successfully 
**					- ACL_ERR_INVALID_AXIS (16)- The argument is not within 0 - 3 range
**
**
**        Return Values:
**                float - the value of the acceleration on specified axis (in "g")
**
**        Errors:
**
**
**        Description:
**				This function is an alternate function used for acceleration reading (the main function for this is ReadAccelG), providing the current acceleration values in “g” on the specified axis. 
**					-	It reads the acceleration on specified axis in a buffer of 2 bytes (2 bytes variable) using the ReadRegister 
**					-	Combines the two bytes in order to get a 10-bit value  
**					-	Converts the 10-bit value to the value in “g”, considering the currently selected g range.
**			When using this function, you must know that it only reads acceleration on one axis. Thus, advancing position in FIFO buffers is done unequally for the 3 axes.  
**			The recommended way is to use the ReadAccelG function.
**			If optional parameter pErr is provided, it will point to the error value: ACL_ERR_SUCCESS if success or ACL_ERR_INVALID_AXIS if bAxisParam is not within 0 - 3 range. 
**
**
*/
float ACL::ReadAccelGOneAxis(uint8_t bAxisParam, uint8_t *pErr)
{
	uint16_t wValue;
	float dResult = 0;
	uint8_t bErr = ACL_ERR_SUCCESS;
	if(bAxisParam >= PAR_AXIS_X && bAxisParam < PAR_AXIS_Z)
	{
		ReadRegisters(ACL_ADR_DATAX0 + 2*bAxisParam, 2, (uint8_t *)&wValue);
		dResult = ConvertReadingToValueG(wValue);
		bErr = ACL_ERR_SUCCESS;
	}
	else
	{
		bErr = ACL_ERR_INVALID_AXIS;
	}
	if(pErr != NULL)
	{
		*pErr = bErr;
	}
	return dResult;
}

/* ------------------------------------------------------------ */
/*        ACL::GetGRangeLSB
**
**        Synopsis:
**				m_dGRangeLSB = GetGRangeLSB(bDataFormatGRangePar);
**        Parameters:
**				uint8_t bDataFormatGRangePar	- the parameter specifying the g range. Can be one of the parameters in the following list
**					0	PAR_DATAFORMAT_PM2G	Parameter g range : +/- 2g
**					1	PAR_DATAFORMAT_PM4G	Parameter g range : +/- 4g
**					2	PAR_DATAFORMAT_PM8G	Parameter g range : +/- 8g
**					3	PAR_DATAFORMAT_PM16G Parameter g range : +/- 16g
**
**        Return Values:
**                float - the value in "g" corresponding to the G range parameter, that corresponds to 1 accelerometer LSB 
**
**        Errors:
**
**
**        Description:
**				Converts the parameter indicating the G range into the value that corresponds to 1 1 accelerometer LSB. 
**					For ex PAR_DATAFORMAT_PM8G: Range is 16g, accelerometer is on 10 bits, that corresponds to 16/(2^10). This constant is later used in converting readings to acceleration values in g.  
**					(for ex converts PAR_DATAFORMAT_PM8G into 8).
**
**
*/
float ACL::GetGRangeLSB(uint8_t bDataFormatGRangePar)
{
	float dGMaxValue = 0;
	float dResult;
	switch(bDataFormatGRangePar)
	{
		case PAR_DATAFORMAT_PM2G:
			dGMaxValue = 2;
			break;
		case PAR_DATAFORMAT_PM4G:
			dGMaxValue = 4;
			break;
		case PAR_DATAFORMAT_PM8G:
			dGMaxValue = 8;
			break;
		case PAR_DATAFORMAT_PM16G:
			dGMaxValue = 16;
			break;
	}
	dResult = 2 * dGMaxValue / (float)(1 << ACL_NO_BITS);
	return dResult;
}

/* ------------------------------------------------------------ */
/*        ACL::GetDevID
**
**        Synopsis:
**				bDevID = myACL.ACLGetDevID();
**        Parameters:
**
**	
**        Return Values:
**                uint8_t - the Device ID of the ADXL345 accelerometer
**
**        Errors:
**
**
**        Description:
**				This function returns the Device ID for ADXL345 accelerometer, which is 0xE5 (see datasheet).
**
**
*/
uint8_t ACL::GetDevID()
{
	uint8_t bRegValue;
	bool fResult;
	ReadRegisters(ACL_ADR_DEVID, 1, &bRegValue);
	return bRegValue;
}

/* ------------------------------------------------------------ */
/*        ACL::SetDataFormatGRangePar
**
**        Synopsis:
**					myACL.SetDataFormatGRangePar(PAR_DATAFORMAT_PM2G);
**        Parameters:
**				uint8_t bDataFormatGRangePar	- the parameter specifying the g range. Can be one of the parameters from the following list:
**					0	PAR_DATAFORMAT_PM2G	Parameter g range : +/- 2g
**					1	PAR_DATAFORMAT_PM4G	Parameter g range : +/- 4g
**					2	PAR_DATAFORMAT_PM8G	Parameter g range : +/- 8g
**					3	PAR_DATAFORMAT_PM16G Parameter g range : +/- 16g
**
**
**        Return Value:
**                uint8_t 
**					- ACL_ERR_SUCCESS (0) 	- The action completed successfully 
**					- ACL_ERR_ARG_RANGE_DF_03 (7)	- The argument is not within 0 - 3 range
**
**        Errors:
**				see Return value
**
**        Description:
**				The function sets the appropriate g range bits in the DATA_FORMAT register. The accepted argument values are between 0 and 3.
**				If the argument is within the accepted values range, it sets the g range bits in DATA_FORMAT register and ACL_ERR_SUCCESS status is returned.
**				If value is outside this range, ACL_ERR_ARG_RANGE_DF_03 is returned and no value is set.
**
*/
uint8_t ACL::SetDataFormatGRangePar(uint8_t bDataFormatGRangePar)
{
	uint8_t bResult;
	if(bDataFormatGRangePar >= PAR_DATAFORMAT_PM2G && bDataFormatGRangePar <= PAR_DATAFORMAT_PM16G)
	{
		m_dGRangeLSB = GetGRangeLSB(bDataFormatGRangePar);
		SetRegisterBitsFromByte(ACL_ADR_DATA_FORMAT, MSK_DATA_FORMAT_RANGE, bDataFormatGRangePar);
		bResult = ACL_ERR_SUCCESS;
	}
	else
	{
		bResult = ACL_ERR_ARG_RANGE_DF_03;
	}
	return bResult;	
}

/* ------------------------------------------------------------ */
/*        ACL::GetDataFormatGRangePar
**
**        Synopsis:
**
**        Parameters:
**
**
**        Return Values:
**                uint8_t - the value specifying the g Range parameter. Can be one of the values from the following list
**					0	PAR_DATAFORMAT_PM2G	Parameter g range : +/- 2g
**					1	PAR_DATAFORMAT_PM4G	Parameter g range : +/- 4g
**					2	PAR_DATAFORMAT_PM8G	Parameter g range : +/- 8g
**					3	PAR_DATAFORMAT_PM16G Parameter g range : +/- 16g
**
**        Errors:
**
**
**        Description:
**				The function returns the value specifying the g range parameter. It relies on the data in DATA_FORMAT register. 
`**
*/
uint8_t ACL::GetDataFormatGRangePar()
{
	return GetRegisterBits(ACL_ADR_DATA_FORMAT, MSK_DATA_FORMAT_RANGE);
}

/* ------------------------------------------------------------ */
/*        ACL::SetSFRBit
**
**        Synopsis:
**				  myACL.SetSFRBit(ACL_BIT_POWER_CTL_MEASURE, false);
**        Parameters:
**				uint8_t bSFRBit - byte indicating the SFR bit to be set. Can be one of:
**							ACL_BIT_BW_RATE_LOWPOWER		- LOW_POWER bit of the BW_RATE register
**			                ACL_BIT_POWER_CTL_LINK			- Link bit of the POWER_CTL register
**                          ACL_BIT_POWER_CTL_AUTOSLEEP		- AUTO_SLEEP bit of the POWER_CTL register
**                          ACL_BIT_POWER_CTL_MEASURE 		- Measure bit of the POWER_CTL register
**                          ACL_BIT_POWER_CTL_SLEEP			- Sleep bit of the POWER_CTL register
**                          ACL_BIT_DATA_FORMAT_SELFTEST	- SELF_TEST bit of the DATA_FORMAT register
**                          ACL_BIT_DATA_FORMAT_SPI			- SPI bit of the DATA_FORMAT register
**                          ACL_BIT_DATA_FORMAT_INTINVERT	- INT_INVERT bit of the DATA_FORMAT register
**                          ACL_BIT_DATA_FORMAT_AUTOSLEEP	- AUTO_SLEEP bit of the DATA_FORMAT register
**                          ACL_BIT_DATA_FORMAT_FULLRES 	- FULL_RES bit of the DATA_FORMAT register
**                          ACL_BIT_DATA_FORMAT_JUSTIFY 	- Justify bit of the DATA_FORMAT register
**                          ACL_BIT_FIFO_CTL_TRIGGER		- Trigger bit of the FIFO_CTL register
**						
**				bool fValue	– the boolean value to be set to the specified bit.
**					- true in order to set specified bit
**					- false in order to reset specified bit
**
**
**        Return Values:
**                uint8_t 
**					- ACL_ERR_SUCCESS (0) 	- The action completed successfully 
**					- ACL_ERR_INVALID_BIT_SET (11)- The argument is not within 0 - 11 range
**
**        Errors:
**				See return value
**
**        Description:
**			This function sets the specified bit to the specified value. 
**				If the specified bit is not among the ones from the list above (bSFRBit not in 0 - 11 range), ACL_ERR_INVALID_BIT_SET is returned
**
**
*/
uint8_t ACL::SetSFRBit(uint8_t bSFRBit, bool fValue)
{
	uint8_t bErr = ACL_ERR_SUCCESS;

	switch(bSFRBit)
	{
	case ACL_BIT_BW_RATE_LOWPOWER:
		SetRegisterBits(ACL_ADR_BW_RATE, MSK_BW_RATE_LOWPOWER, fValue);
		break;
	case ACL_BIT_POWER_CTL_LINK:
		SetRegisterBits(ACL_ADR_POWER_CTL, MSK_POWER_CTL_LINK, fValue);
		break;
	case ACL_BIT_POWER_CTL_AUTOSLEEP:
		SetRegisterBits(ACL_ADR_POWER_CTL, MSK_POWER_CTL_AUTOSLEEP, fValue);
		break;
	case ACL_BIT_POWER_CTL_MEASURE:
		SetRegisterBits(ACL_ADR_POWER_CTL, MSK_POWER_CTL_MEASURE, fValue);
		break;
	case ACL_BIT_POWER_CTL_SLEEP:
		SetRegisterBits(ACL_ADR_POWER_CTL, MSK_POWER_CTL_SLEEP, fValue);
		break;
	case ACL_BIT_DATA_FORMAT_SELFTEST:
		SetRegisterBits(ACL_ADR_DATA_FORMAT, MSK_DATA_FORMAT_SELFTEST, fValue);
		break;
	case ACL_BIT_DATA_FORMAT_SPI:
		SetRegisterBits(ACL_ADR_DATA_FORMAT, MSK_DATA_FORMAT_SPI, fValue);
		break;
	case ACL_BIT_DATA_FORMAT_INTINVERT:
		SetRegisterBits(ACL_ADR_DATA_FORMAT, MSK_DATA_FORMAT_INTINVERT, fValue);
		break;
	case ACL_BIT_DATA_FORMAT_AUTOSLEEP:
		SetRegisterBits(ACL_ADR_DATA_FORMAT, MSK_DATA_FORMAT_AUTOSLEEP, fValue);
		break;
	case ACL_BIT_DATA_FORMAT_FULLRES:
		SetRegisterBits(ACL_ADR_DATA_FORMAT, MSK_DATA_FORMAT_FULLRES, fValue);
		break;
	case ACL_BIT_DATA_FORMAT_JUSTIFY:
		SetRegisterBits(ACL_ADR_DATA_FORMAT, MSK_DATA_FORMAT_JUSTIFY, fValue);
		break;
	case ACL_BIT_FIFO_CTL_TRIGGER:
		SetRegisterBits(ACL_ADR_FIFO_CTL, MSK_FIFO_CTL_TRIGGER, fValue);
		break;
	default:
		bErr = ACL_ERR_INVALID_BIT_SET;
	}
	return bErr;
}


/* ------------------------------------------------------------ */
/*        ACL::GetSFRBit
**
**        Synopsis:
**				fMeasure = myACL.GetSFRBit(ACL_BIT_POWER_CTL_MEASURE);
**			or
**				fMeasure = myACL.GetSFRBit(ACL_BIT_POWER_CTL_MEASURE, pErr);
**
**        Parameters:
**				uint8_t bSFRBit - byte indicating the SFR bit to be set. Can be one of:
**							ACL_BIT_BW_RATE_LOWPOWER		- LOW_POWER bit of the BW_RATE register
**			                ACL_BIT_POWER_CTL_LINK			- Link bit of the POWER_CTL register
**                          ACL_BIT_POWER_CTL_AUTOSLEEP		- AUTO_SLEEP bit of the POWER_CTL register
**                          ACL_BIT_POWER_CTL_MEASURE 		- Measure bit of the POWER_CTL register
**                          ACL_BIT_POWER_CTL_SLEEP			- Sleep bit of the POWER_CTL register
**                          ACL_BIT_DATA_FORMAT_SELFTEST	- SELF_TEST bit of the DATA_FORMAT register
**                          ACL_BIT_DATA_FORMAT_SPI			- SPI bit of the DATA_FORMAT register
**                          ACL_BIT_DATA_FORMAT_INTINVERT	- INT_INVERT bit of the DATA_FORMAT register
**                          ACL_BIT_DATA_FORMAT_AUTOSLEEP	- AUTO_SLEEP bit of the DATA_FORMAT register
**                          ACL_BIT_DATA_FORMAT_FULLRES 	- FULL_RES bit of the DATA_FORMAT register
**                          ACL_BIT_DATA_FORMAT_JUSTIFY 	- Justify bit of the DATA_FORMAT register
**                          ACL_BIT_FIFO_CTL_TRIGGER		- Trigger bit of the FIFO_CTL register
**                          ACL_BIT_FIFO_STATUS_FIFOTRIG	- FIFO_TRIG bit of the FIFO_STATUS register
**				 uint8_t *pErr 	- optional parameter - pointer to a value that receives error value. User may chose not to provide this parameter.
**					- ACL_ERR_SUCCESS (0) 	- The action completed successfully 
**					- ACL_ERR_INVALID_BIT_GET (11)- The argument is not within 0 - 11 range
**
**        Return Values:
**                bool	– the boolean value of the specified bit.
**
**        Errors:
**				See output parameter pErr
**
**        Description:
**				This function returns the boolean value of the specified bit.
**				If optional parameter pErr is provided, it will point to the error value: ACL_ERR_SUCCESS if success or ACL_ERR_INVALID_BIT_GET  if bSFRBit is not within 0 - 12 range.
**
*/
bool ACL::GetSFRBit(uint8_t bSFRBit, uint8_t *pErr)
{
	uint8_t bErr = ACL_ERR_SUCCESS;
	bool bValue = 0;
	switch(bSFRBit)
	{
	case ACL_BIT_BW_RATE_LOWPOWER:
		bValue = GetRegisterBits(ACL_ADR_BW_RATE, MSK_BW_RATE_LOWPOWER);
		break;
	case ACL_BIT_POWER_CTL_LINK:
		bValue = GetRegisterBits(ACL_ADR_POWER_CTL, MSK_POWER_CTL_LINK);
		break;
	case ACL_BIT_POWER_CTL_AUTOSLEEP:
		bValue = GetRegisterBits(ACL_ADR_POWER_CTL, MSK_POWER_CTL_AUTOSLEEP);
		break;
	case ACL_BIT_POWER_CTL_MEASURE:
		bValue = GetRegisterBits(ACL_ADR_POWER_CTL, MSK_POWER_CTL_MEASURE);
		break;
	case ACL_BIT_POWER_CTL_SLEEP:
		bValue = GetRegisterBits(ACL_ADR_POWER_CTL, MSK_POWER_CTL_SLEEP);
		break;
	case ACL_BIT_DATA_FORMAT_SELFTEST:
		bValue = GetRegisterBits(ACL_ADR_DATA_FORMAT, MSK_DATA_FORMAT_SELFTEST);
		break;
	case ACL_BIT_DATA_FORMAT_SPI:
		bValue = GetRegisterBits(ACL_ADR_DATA_FORMAT, MSK_DATA_FORMAT_SPI);
		break;
	case ACL_BIT_DATA_FORMAT_INTINVERT:
		bValue = GetRegisterBits(ACL_ADR_DATA_FORMAT, MSK_DATA_FORMAT_INTINVERT);
		break;
	case ACL_BIT_DATA_FORMAT_AUTOSLEEP:
		bValue = GetRegisterBits(ACL_ADR_DATA_FORMAT, MSK_DATA_FORMAT_AUTOSLEEP);
		break;
	case ACL_BIT_DATA_FORMAT_FULLRES:
		bValue = GetRegisterBits(ACL_ADR_DATA_FORMAT, MSK_DATA_FORMAT_FULLRES);
		break;
	case ACL_BIT_DATA_FORMAT_JUSTIFY:
		bValue = GetRegisterBits(ACL_ADR_DATA_FORMAT, MSK_DATA_FORMAT_JUSTIFY);
		break;
	case ACL_BIT_FIFO_CTL_TRIGGER:
		bValue = GetRegisterBits(ACL_ADR_FIFO_CTL, MSK_FIFO_CTL_TRIGGER);
		break;
	case ACL_BIT_FIFO_STATUS_FIFOTRIG:
		bValue = GetRegisterBits(ACL_ADR_FIFO_STATUS, MSK_FIFO_STATUS_FIFOTRIG);
		break;
	default:
		bErr = ACL_ERR_INVALID_BIT_GET;		
	}
	if(pErr != NULL)
	{
		*pErr = bErr;
	}
	return bValue != 0;
}
/* ------------------------------------------------------------ */
/*        ACL::SetOffsetG
**
**        Synopsis:
**				myACL.SetOffsetG(PAR_AXIS_Z, 0.5);
**        Parameters:
**				uint8_t bAxisParam - byte indicating the axis whose offset will be set. Can be one of:
**					PAR_AXIS_X		- indicating X axis				
**                  PAR_AXIS_Y 		- indicating Y axis
**                  PAR_AXIS_Z 		- indicating Z axis
**
**				float dOffsetX	– the offset for X axis in “g”
**
**        Return Values:
**                uint8_t 
**					- ACL_ERR_SUCCESS (0) 	- The action completed successfully 
**					- a combination (ORed) of the following errors:
**						- ACL_ERR_ARG_RANGE_PM2G (1)- The dOffset argument is not within +/- 2g range
**						- ACL_ERR_INVALID_AXIS (16)- The bAxisParam argument is not within 0 - 3 range
**
**        Errors:
**				see return value
**
**        Description:
**				This function sets the specified axis offset, the value being given in “g”. The accepted argument values are between -2g and +2g.  
**				If argument is within the accepted values range, its value is quantified in the 8-bit offset register using a scale factor of 15.6 mg/LSB and ACL_ERR_SUCCESS is returned.
**				If value is outside this range, ACL_ERR_ARG_RANGE_PM2G is returned and no value is set.
**				If bAxisParam parameter is outside 0 - 3 range, ACL_ERR_INVALID_AXIS is returned.
**
*/
uint8_t ACL::SetOffsetG(uint8_t bAxisParam, float dOffset)
{
	uint8_t bErr = ACL_ERR_SUCCESS;
	if(dOffset < -2 || dOffset > 2)
	{
		bErr |= ACL_ERR_ARG_RANGE_PM2G;
	}
	if(bAxisParam < PAR_AXIS_X || bAxisParam > PAR_AXIS_Z)
	{
		bErr |= ACL_ERR_INVALID_AXIS;
	}
	if(bErr == ACL_ERR_SUCCESS)
	{
		int8_t bOffsetVal = (uint8_t)(dOffset/(float)ACL_CONV_OFFSET_G_LSB);
		switch (bAxisParam)
		{
			case PAR_AXIS_X:
				WriteRegisters(ACL_ADR_OFSX, 1, (uint8_t *)&bOffsetVal);
				break;
			case PAR_AXIS_Y:
				WriteRegisters(ACL_ADR_OFSY, 1, (uint8_t *)&bOffsetVal);
				break;
			case PAR_AXIS_Z:
				WriteRegisters(ACL_ADR_OFSZ, 1, (uint8_t *)&bOffsetVal);
				break;				
		}
	}
	return bErr;
}


/* ------------------------------------------------------------ */
/*        ACL::GetOffsetXG
**
**        Synopsis:
**				dOffsetX = myACL.GetOffsetG(PAR_AXIS_X);
**					or
**				dOffsetX = myACL.GetOffsetG(PAR_AXIS_X, &err);
**				uint8_t bAxisParam - byte indicating the axis whose acceleration will be read. Can be one of:
**					PAR_AXIS_X		- indicating X axis				
**                  PAR_AXIS_Y 		- indicating Y axis
**                  PAR_AXIS_Z 		- indicating Z axis
**
**				 uint8_t *pErr 	- optional parameter - pointer to a value that receives error value. User may chose not to provide this parameter.
**					- ACL_ERR_SUCCESS (0) 	- The action completed successfully 
**					- ACL_ERR_INVALID_AXIS (16)- The argument is not within 0 - 3 range
**
**
**        Return Values:
**                float 	- the offset for X axis in “g”.
**
**        Errors:
**
**
**        Description:
**				This function returns the offset, in “g”, for the specified axis.  
**				It converts the 8-bit value quantified in the offset register into a value expressed in “g”, using a scale factor of 15.6 mg/LSB.
**				If optional parameter pErr is provided, it will point to the error value: ACL_ERR_SUCCESS if success or ACL_ERR_INVALID_AXIS if bAxisParam is not within 0 - 3 range. 
**
**
*/
float ACL::GetOffsetG(uint8_t bAxisParam, uint8_t *pErr)
{
	int8_t bOffsetVal; 
	float dResult;
	ReadRegisters(ACL_ADR_OFSX, 1, (uint8_t *)&bOffsetVal);
	dResult = (float)bOffsetVal * (float)ACL_CONV_OFFSET_G_LSB;
	return dResult;
}


//* ------------------------------------------------------------ */
/*        ACL::SetBWRateOutputRatePar
**
**        Synopsis:
**				myACL.ACLSetOutputRatePar(PAR_OUTPUTRATE100_00Hz);
**        Parameters:
**				uint8_t bOutputRatePar - a parameter selecting one of the options from the following list
**					0	PAR_OUTPUTRATE0_10Hz	Parameter Output Data Rate : 0.10 Hz
**					1	PAR_OUTPUTRATE0_20Hz	Parameter Output Data Rate : 0.20 Hz
**					2	PAR_OUTPUTRATE0_39Hz	Parameter Output Data Rate : 0.39 Hz
**					3	PAR_OUTPUTRATE0_78Hz	Parameter Output Data Rate : 0.78 Hz
**					4	PAR_OUTPUTRATE1_56Hz	Parameter Output Data Rate : 1_56 Hz
**					5	PAR_OUTPUTRATE3_13Hz	Parameter Output Data Rate : 3.13 Hz
**					6	PAR_OUTPUTRATE6_25Hz	Parameter Output Data Rate : 6.25 Hz
**					7	PAR_OUTPUTRATE12_50Hz	Parameter Output Data Rate : 12.5 Hz
**					8	PAR_OUTPUTRATE25_00Hz	Parameter Output Data Rate : 25  Hz
**					9	PAR_OUTPUTRATE50_00Hz	Parameter Output Data Rate : 50 Hz
**					10	PAR_OUTPUTRATE100_00Hz	Parameter Output Data Rate : 100 Hz
**					11	PAR_OUTPUTRATE200_00Hz	Parameter Output Data Rate : 200 Hz
**					12	PAR_OUTPUTRATE400_00Hz	Parameter Output Data Rate : 400 Hz
**					13	PAR_OUTPUTRATE800_00Hz	Parameter Output Data Rate : 800 Hz
**					14	PAR_OUTPUTRATE1600_00Hz	Parameter Output Data Rate : 1600 Hz
**					15	PAR_OUTPUTRATE3200_00Hz	Parameter Output Data Rate : 3200 Hz
**
**        Return Value:
**                uint8_t 
**					- ACL_ERR_SUCCESS (0) 	- The action completed successfully 
**					- ACL_ERR_ARG_RANGE_015 (6)	- The argument is not within 0 - 15 range
**
**        Errors:
**				.
**
**        Description:
**				The accepted argument values are between 0 and 15. If the argument is within the accepted values range, the function sets the Rate bits in the BW_RATE register 
**				according to the parameter and ACL_ERR_SUCCESS is returned. If the value is outside this range, ACL_ERR_ARG_RANGE_015 is returned and no value is set.
**
**
*/
uint8_t ACL::SetBWRateOutputRatePar(uint8_t bOutputRatePar)
{
	uint8_t bResult;
	if(bOutputRatePar >= PAR_OUTPUTRATE0_10Hz && bOutputRatePar <= PAR_OUTPUTRATE3200_00Hz)
	{
		SetRegisterBitsFromByte(ACL_ADR_BW_RATE, MSK_BW_RATE_RATE, bOutputRatePar);
		bResult = ACL_ERR_SUCCESS;
	}
	else
	{
		bResult = ACL_ERR_ARG_RANGE_0160MS;
	}
	return bResult;	
}


/* ------------------------------------------------------------ */
/*        ACL::GetBWRateOutputRatePar
**
**        Synopsis:
**				nOutputRate = myACL.ACLGetBWRateOutputRatePar();
**        Parameters:
**
**
**        Return Values:
**                uint8_t - the value specifying the output rate parameter. Can be one of the values from the following list:
**					0	PAR_OUTPUTRATE0_10Hz	Parameter Output Data Rate : 0.10 Hz
**					1	PAR_OUTPUTRATE0_20Hz	Parameter Output Data Rate : 0.20 Hz
**					2	PAR_OUTPUTRATE0_39Hz	Parameter Output Data Rate : 0.39 Hz
**					3	PAR_OUTPUTRATE0_78Hz	Parameter Output Data Rate : 0.78 Hz
**					4	PAR_OUTPUTRATE1_56Hz	Parameter Output Data Rate : 1_56 Hz
**					5	PAR_OUTPUTRATE3_13Hz	Parameter Output Data Rate : 3.13 Hz
**					6	PAR_OUTPUTRATE6_25Hz	Parameter Output Data Rate : 6.25 Hz
**					7	PAR_OUTPUTRATE12_50Hz	Parameter Output Data Rate : 12.5 Hz
**					8	PAR_OUTPUTRATE25_00Hz	Parameter Output Data Rate : 25  Hz
**					9	PAR_OUTPUTRATE50_00Hz	Parameter Output Data Rate : 50 Hz
**					10	PAR_OUTPUTRATE100_00Hz	Parameter Output Data Rate : 100 Hz
**					11	PAR_OUTPUTRATE200_00Hz	Parameter Output Data Rate : 200 Hz
**					12	PAR_OUTPUTRATE400_00Hz	Parameter Output Data Rate : 400 Hz
**					13	PAR_OUTPUTRATE800_00Hz	Parameter Output Data Rate : 800 Hz
**					14	PAR_OUTPUTRATE1600_00Hz	Parameter Output Data Rate : 1600 Hz
**					15	PAR_OUTPUTRATE3200_00Hz	Parameter Output Data Rate : 3200 Hz
**        Errors:
**				
**
**        Description:
**				This function returns a value corresponding to the Output Rate bits in the BW_RATE register.
**
**
*/
uint8_t ACL::GetBWRateOutputRatePar()
{
	return GetRegisterBits(ACL_ADR_BW_RATE, MSK_BW_RATE_RATE);
}	






/* ------------------------------------------------------------ */
/*        ACL::SetPowerControlWakeupFreqPar
**
**        Synopsis:
**				myACL.SetPowerControlWakeupFreqPar(PAR_WAKEUP_FREQ4Hz)
**        Parameters:
**				uint8_t bPowerControlWakeupFreqPar	- the parameter specifying the wakeup frequency. Can be one of the parameters in the following list
**					0	PAR_WAKEUP_FREQ8Hz	- Parameter Wakeup rate : 8 Hz
**					1	PAR_WAKEUP_FREQ4Hz	- Parameter Wakeup rate : 4 Hz
**					2	PAR_WAKEUP_FREQ2Hz	- Parameter Wakeup rate : 2 Hz
**					3	PAR_WAKEUP_FREQ1Hz	- Parameter Wakeup rate : 1 Hz
**
**        Return Value:
**                uint8_t 
**					- ACL_ERR_SUCCESS (0) 	- The action completed successfully  
**					- ACL_ERR_ARG_RANGE_WU_03 (8)	- The argument is not within 0 - 3 range
**
**        Errors:
**				see Return value
**
**        Description:
**				This function sets the appropriate wakeup frequency bits in POWER_CTL register. The accepted argument values are between 0 and 3.
**				If argument is within the accepted values range, it sets the appropriate wakeup frequency bits in the POWER_CTL  register and ACL_ERR_SUCCESS status is returned. 
**				If the value is outside this range, ACL_ERR_ARG_RANGE_DF_03 is returned and no value is set.
**
*/
uint8_t ACL::SetPowerControlWakeupFreqPar(uint8_t bPowerControlWakeupFreqPar)
{
	uint8_t bResult;
	if(bPowerControlWakeupFreqPar >= PAR_WAKEUP_FREQ8Hz && bPowerControlWakeupFreqPar <= PAR_WAKEUP_FREQ1Hz)
	{
		SetRegisterBitsFromByte(ACL_ADR_POWER_CTL, MSK_POWER_CTL_WAKEUP, bPowerControlWakeupFreqPar);
		bResult = ACL_ERR_SUCCESS;
	}
	else
	{
		bResult = ACL_ERR_ARG_RANGE_WU_03;
	}
	return bResult;	
}


/* ------------------------------------------------------------ */
/*        ACL::GetPowerControlWakeupFreqPar
**
**        Synopsis:
**
**        Parameters:
**
**
**        Return Values:
**                uint8_t - the value specifying the wakeup frequency parameter. Can be one of the values from the following list:
**					0	PAR_WAKEUP_FREQ8Hz	- Parameter Wakeup rate : 8 Hz
**					1	PAR_WAKEUP_FREQ4Hz	- Parameter Wakeup rate : 4 Hz
**					2	PAR_WAKEUP_FREQ2Hz	- Parameter Wakeup rate : 2 Hz
**					3	PAR_WAKEUP_FREQ1Hz	- Parameter Wakeup rate : 1 Hz
**
**        Errors:
**
**
**        Description:
**				The function returns the value specifying the wakeup frequency parameter. 
**				It relies on the data from the POWER_CTL register.  
**
*/
uint8_t ACL::GetPowerControlWakeupFreqPar()
{
	return GetRegisterBits(ACL_ADR_POWER_CTL, MSK_POWER_CTL_WAKEUP);
}



/* ------------------------------------------------------------ */
/*        ACL::SetFIFOControlFIFOModePar
**
**        Synopsis:
**				myACL.SetFIFOControlFIFOModePar(FIFO_MODE_FIFO);
**        Parameters:
**				uint8_t bFIFOControlFIFOModePar	- the parameter specifying the FIFO mode. it can be one of the parameters in the following list
**					0	FIFO_MODE_BYPASS	Parameter FIFO Mode : Bypass, used to access FIFO_MODE bits in FIFO_CTL register
**					1	FIFO_MODE_FIFO		Parameter FIFO Mode : FIFO, used to access FIFO_MODE bits in FIFO_CTL register
**					2	FIFO_MODE_STREAM	Parameter FIFO Mode : Stream, used to access FIFO_MODE bits in FIFO_CTL register
**					3	FIFO_MODE_TRIGGER	Parameter FIFO Mode : Trigger, used to access FIFO_MODE bits in FIFO_CTL register
**
**        Return Value:
**                uint8_t 
**					- ACL_ERR_SUCCESS (0) 	- The action completed successfully 
**					- ACL_ERR_ARG_RANGE_FIFO_03 (13)	- The argument is not within 0 - 3 range
**
**        Errors:
**				.
**
**        Description:
**				The function sets the appropriate FIFO mode bits in the FIFO_CTL register. The accepted argument values are between 0 and 3.
**				If the argument is within the accepted values range, it sets the FIFO mode bits in FIFO_CTL register and ACL_ERR_SUCCESS is returned.
**				If the value is outside this range, ACL_ERR_ARG_RANGE_FIFO_03 is returned and no value is set.
**
**
*/
uint8_t ACL::SetFIFOControlFIFOModePar(uint8_t bFIFOControlFIFOModePar)
{
	uint8_t bResult;
	if(bFIFOControlFIFOModePar >= PAR_DATAFORMAT_PM2G && bFIFOControlFIFOModePar <= PAR_DATAFORMAT_PM16G)
	{
		SetRegisterBitsFromByte(ACL_ADR_FIFO_CTL, MSK_FIFO_CTL_FIFOMODE, bFIFOControlFIFOModePar);
		bResult = ACL_ERR_SUCCESS;
	}
	else
	{
		bResult = ACL_ERR_ARG_RANGE_FIFO_03;
	}
	return bResult;	
}


/* ------------------------------------------------------------ */
/*        ACL::GetFIFOControlFIFOModePar
**
**        Synopsis:
**				bValue = myACL.GetFIFOControlFIFOModePar();
**        Parameters:
**
**
**        Return Values:
**                uint8_t - the value specifying the FIFO mode parameter. It can be one of the values from the following list:
**					0	FIFO_MODE_BYPASS	Parameter FIFO Mode : Bypass, used to access FIFO_MODE bits in FIFO_CTL register
**					1	FIFO_MODE_FIFO		Parameter FIFO Mode : FIFO, used to access FIFO_MODE bits in FIFO_CTL register
**					2	FIFO_MODE_STREAM	Parameter FIFO Mode : Stream, used to access FIFO_MODE bits in FIFO_CTL register
**					3	FIFO_MODE_TRIGGER	Parameter FIFO Mode : Trigger, used to access FIFO_MODE bits in FIFO_CTL register**
**        Errors:
**
**
**        Description:
**				The function returns the value specifying the FIFO mode parameter from  FIFO_CTL register.  
**				
**
*/
uint8_t ACL::GetFIFOControlFIFOModePar()
{
	return GetRegisterBits(ACL_ADR_FIFO_CTL, MSK_FIFO_CTL_FIFOMODE);
}


/* ------------------------------------------------------------ */
/*        ACL::SetFIFOControlSamplesVal
**
**        Synopsis:
**				myACL.SetFIFOControlSamplesVal(10);
**        Parameters:
**				uint8_t bFIFOControlSamplesVal	- the value to be assigned to Samples bits in FIFO_CTL register.
**
**
**        Return Values:
**                void 
**
**        Errors:
**
**
**        Description:
**				The function sets the appropriate Samples bits in FIFO_CTL register. 
**
*/
void ACL::SetFIFOControlSamplesVal(uint8_t bFIFOControlSamplesVal)
{
	SetRegisterBitsFromByte(ACL_ADR_FIFO_CTL, MSK_FIFO_CTL_SAMPLES, bFIFOControlSamplesVal);
}


/* ------------------------------------------------------------ */
/*        ACL::GetFIFOControlSamplesVal
**
**        Synopsis:
**				bValue = myACL.GetFIFOControlSamplesVal();
**        Parameters:
**
**
**        Return Values:
**                uint8_t - the value specifying the value of the Samples bits
**
**        Errors:
**
**
**        Description:
**			The function returns the value of the Samples bits. It relies on the data from FIFO_CTL register.
**
*/
uint8_t ACL::GetFIFOControlSamplesVal()
{
	return GetRegisterBits(ACL_ADR_FIFO_CTL, MSK_FIFO_CTL_SAMPLES);
}

/* ------------------------------------------------------------ */
/*        ACL::GetFIFOStatusEntriesVal
**
**        Synopsis:
**				bValue = myACL.GetFIFOStatusEntriesVal();
**        Parameters:
**
**
**        Return Values:
**                uint8_t - the value of the Entries bits from FIFO_STATUS register
**
**        Errors:
**
**
**        Description:
**				The function returns the Entries bits values enabled from FIFO_STATUS register. 
**
*/
uint8_t ACL::GetFIFOStatusEntriesVal()
{
	return GetRegisterBits(ACL_ADR_FIFO_STATUS, MSK_FIFO_STATUS_ENTRIES);
}


/* ------------------------------------------------------------ */
/*        ACL::CalibrateOneAxisGravitational
**
**        Synopsis:
**				myACL.CalibrateOneAxisGravitational(PAR_AXIS_ZP);
**        Parameters:
**				uint8_t bAxisInfo - Parameter specifying axes orientation. Can be one of the following:
**					0	PAR_AXIS_XP - X axis is oriented in the gravitational direction
**					1	PAR_AXIS_XN - X axis is oriented in the opposite gravitational direction
**					2	PAR_AXIS_YP - Y axis is oriented in the gravitational direction
**					3	PAR_AXIS_YN - Y axis is oriented in the opposite gravitational direction
**					4	PAR_AXIS_ZP - Z axis is oriented in the gravitational direction
**					5	PAR_AXIS_ZN - Z axis is oriented in the opposite gravitational direction
**
**        Return Value:
**                uint8_t 
**					- ACL_ERR_SUCCESS (0) 	- The action completed successfully 
**					- ACL_ERR_ARG_RANGE_05 (10)	- The argument is not within 0 - 5 range
**
**        Errors:
**
**
**        Description:
**				The accepted argument values are between 0 and +5.
**				This function performs the calibration of the accelerometer by setting the offset registers in the following manner: 
**				 computes the correction factor that must be loaded in the offset registers so that the acceleration readings are:
**					1 for the gravitational axis, if positive orientation
**					-1 for the gravitational axis, if negative orientation
**					0 for the other axes
**				The accepted argument values are between 0 and 5, when function performs calibration and returns ACL_ERR_SUCCESS.
**				If value is outside this range, ACL_ERR_ARG_RANGE_05 is returned and no calibration is done.
**
*/
uint8_t ACL::CalibrateOneAxisGravitational(uint8_t bAxisInfo)
{
	uint8_t bResult;
	if(bAxisInfo >= PAR_AXIS_XP && bAxisInfo <= PAR_AXIS_ZN)
	{
		// perform calibration
		float dX, dSumX = 0, dY, dSumY = 0, dZ, dSumZ = 0;
		// set the offset registers to 0
		//Put the device into standby mode to configure it.
		SetSFRBit(ACL_BIT_POWER_CTL_MEASURE, false);
		SetOffsetG(PAR_AXIS_X, 0);
		SetOffsetG(PAR_AXIS_Y, 0);
		SetOffsetG(PAR_AXIS_Z, 0);
		SetSFRBit(ACL_BIT_POWER_CTL_MEASURE, true);


		// read average acceleration on the three axes
		int idxAvg;

		int nCntMeasurements = 128;
		// consume some readings
		for(idxAvg = 0; idxAvg < nCntMeasurements; idxAvg++)
		{
			ReadAccelG(dX, dY, dZ);
		}
		for(idxAvg = 0; idxAvg < nCntMeasurements; idxAvg++)
		{
			ReadAccelG(dX, dY, dZ);
			dSumX = dSumX + dX;
			dSumY = dSumY + dY;
			dSumZ = dSumZ + dZ;
		}
		dX = dSumX/nCntMeasurements;
		dY = dSumY/nCntMeasurements;
		dZ = dSumZ/nCntMeasurements;
		// computes the correction that must be put in the offset registers so that the acceleration readings are:
		//	1 (for the gravitational axis, if positive
		//	-1 (for the gravitational axis, if negative
		// 0 (for the other axes)
		switch (bAxisInfo)
		{
			case PAR_AXIS_XP:
				dX = 1.0 - dX;
				dY = 0.0 - dY;
				dZ = 0.0 - dZ;
				break;
			case PAR_AXIS_XN:
				dX = -1.0 - dX;
				dY = 0.0 - dY;
				dZ = 0.0 - dZ;
				break;
			case PAR_AXIS_YP:
				dY = 1.0 - dY;
				dX = 0.0 - dX;
				dZ = 0.0 - dZ;
				break;
			case PAR_AXIS_YN:
				dY = -1.0 - dY;
				dX = 0.0 - dX;
				dZ = 0.0 - dZ;
				break;
			case PAR_AXIS_ZP:
				dZ = 1.0 - dZ;
				dY = 0.0 - dY;
				dX = 0.0 - dX;
				break;
			case PAR_AXIS_ZN:
				dZ = -1.0 - dZ;
				dY = 0.0 - dY;
				dX = 0.0 - dX;
				break;
		}
		//Put the device into standby mode to configure it.
		SetSFRBit(ACL_BIT_POWER_CTL_MEASURE, false);
		// set the offset data to registers
		SetOffsetG(PAR_AXIS_X, dX);
		SetOffsetG(PAR_AXIS_Y, dY);
		SetOffsetG(PAR_AXIS_Z, dZ);
		SetSFRBit(ACL_BIT_POWER_CTL_MEASURE, true);

		bResult = ACL_ERR_SUCCESS;
	}
	else
	{
		bResult = ACL_ERR_ARG_RANGE_05;
	}
	return bResult;	
}



/* ------------------------------------------------------------ */
/*        ACL::ConfigureInterrupt
**
**        Synopsis:
**				
**        Parameters:
**				uint8_t bParACLIntNo	- The parameter indicating the ACL interrupt number. Can be one of the parameters from the following list
**					0		PAR_ACL_INT1
**					1		PAR_ACL_INT2
**
**				uint8_t bParExtIntNo	- The parameter indicating the external interrupt number. Can be one of the parameters from the following list
**					0		PAR_EXT_INT0
**					1		PAR_EXT_INT1
**					2		PAR_EXT_INT2
**					3		PAR_EXT_INT3
**					4		PAR_EXT_INT4
**
**				uint8_t bEventMask	- the events that trigger the interrupt. Can be one or more (OR-ed) parameters from the following list
**					1<<7	MSK_INT_DATA_READY	DATA_READY 
**					1<<6	MSK_INT_SINGLE_TAP	SINGLE_TAP 
**					1<<5	MSK_INT_float_TAP	float_TAP 
**					1<<4	MSK_INT_ACTIVITY	Activity
**					1<<3	MSK_INT_INACTIVITY	Inactivity 
**					1<<2	MSK_INT_FREE_FALL	FREE_FALL 
**					1<<1	MSK_INT_WATERMARK	Watermark 
**					1<<0	MSK_INT_OVERRUN		Overrun
**
**				void (*pfIntHandler)() - pointer to a function that will serve as interrupt handler.
**
**				uint8_t bActiveType	– The parameter indicating the interrupt pin is active high or low. Can be one of the parameters from the following list
**					0		PAR_INT_ACTIVEHIGH
**					1		PAR_INT_ACTIVELOW
**
**        Return Value:
**                uint8_t 
**					- ACL_ERR_SUCCESS (0) 	- The action completed successfully 
**					- a combination (ORed) of the following errors:
**						- ACL_ERR_ARG_AINTNO02_BIT (32) - ACL Interrupt no is not within 0-1 range
**						- ACL_ERR_ARG_EINTNO04_BIT (64) - External Interrupt no is not within 0-4 range 
**						- ACL_ERR_ARG_ACT_BIT (128) - Active is different than 0 or 1 
**
**        Errors:
**
**
**        Description:
**				The function configures the interrupt by 
**					- associating it to a set of events (INT_ENABLE  & INT_MAP registers) 
**					- associating it to an ACL interrupt number (1, 2)
**					- associating it to an external interrupt number (0-4)
**					- associating it to an interrupt handler  
**					- defining the behavior for the interrupt pin
**				Make sure that interrupt pin of the PmodACL corresponding to the bParACLIntNo parameter is physically connected to external interrupt pin number corresponding to bParExtIntNo parameter.
**				The function expects the parameters bParACLIntNo, bParExtIntNo and bActiveType to be in the specified range.
**				For each parameter outside the range, a specific error is set and a combination (OR-ed) of the errors is returned. 
**				If all parameters are within their range, ACL_ERR_SUCCESS is returned. 
**
*/
uint8_t ACL::ConfigureInterrupt(uint8_t bParACLIntNo, uint8_t bParExtIntNo, uint8_t bEventMask, void (*pfIntHandler)(), uint8_t bActiveType)
{
	uint8_t bResult = ACL_ERR_SUCCESS;
	if(bParACLIntNo < PAR_ACL_INT1 || bParACLIntNo > PAR_ACL_INT2)
	{
		bResult |= ACL_ERR_ARG_AINTNO01_BIT;
	}
	if(bParExtIntNo < PAR_EXT_INT0 || bParExtIntNo > PAR_EXT_INT4)
	{
		bResult |= ACL_ERR_ARG_EINTNO04_BIT;
	}
	if(bActiveType < PAR_INT_ACTIVEHIGH || bActiveType > PAR_INT_ACTIVELOW)
	{
		bResult |= ACL_ERR_ARG_ACT_BIT;
	}
	if(bResult == ACL_ERR_SUCCESS)
	{	attachInterrupt(bParExtIntNo, pfIntHandler, RISING);
		SetRegisterBits(ACL_ADR_INT_MAP, bEventMask, bParACLIntNo == PAR_ACL_INT2);
		SetRegisterBits(ACL_ADR_INT_ENABLE, bEventMask, true);
		SetRegisterBits(ACL_ADR_DATA_FORMAT, MSK_DATA_FORMAT_INTINVERT, bActiveType == PAR_INT_ACTIVELOW);
		uint8_t bVal;
		ReadRegisters(ACL_ADR_INT_SOURCE, 1, &bVal);	// reading the INT_SOURCE register causes interrupt flags to be cleared
	}
	return bResult;
}



/* ------------------------------------------------------------ */
/*        ACL::GetInterruptSourceBits
**
**        Synopsis:
**
**        Parameters:
**				uint8_t bEventMask	- the events that are checked if they triggered the interrupt. Can be one or more (OR-ed) parameters from the following list
**					1<<7	MSK_INT_DATA_READY	DATA_READY 
**					1<<6	MSK_INT_SINGLE_TAP	SINGLE_TAP 
**					1<<5	MSK_INT_float_TAP	float_TAP 
**					1<<4	MSK_INT_ACTIVITY		Activity
**					1<<3	MSK_INT_INACTIVITY	Inactivity 
**					1<<2	MSK_INT_FREE_FALL		FREE_FALL 
**					1<<1	MSK_INT_WATERMARK		Watermark 
**					1<<0	MSK_INT_OVERRUN		Overrun
**
**
**        Return Values:
**                uint8_t - the value of the register masked using the specified mask
**
**        Errors:
**
**
**        Description:
**				The function returns the value of INT_SOURCE register masked using the provided mask.
**					When defining more events for an interrupt, this function can be used to determine which is causing the interrupt.
**					Note that, according to datasheet, reading the INT_SOURCE register causes interrupt flags to be cleared.
**
*/
uint8_t ACL::GetInterruptSourceBits(uint8_t bEventMask)
{
	return GetRegisterBits(ACL_ADR_INT_SOURCE, bEventMask);
}

/* ------------------------------------------------------------ */
/*        ACL::GetIntEnableEventBits
**
**        Synopsis:
**
**        Parameters:
**				uint8_t bEventMask	- the events that are checked if they are enabled to generate the interrupt. 
**				Can be one or more (OR-ed) parameters from the following list 
**					1<<7	MSK_INT_DATA_READY	DATA_READY 
**					1<<6	MSK_INT_SINGLE_TAP	SINGLE_TAP 
**					1<<5	MSK_INT_float_TAP	float_TAP 
**					1<<4	MSK_INT_ACTIVITY		Activity
**					1<<3	MSK_INT_INACTIVITY	Inactivity 
**					1<<2	MSK_INT_FREE_FALL		FREE_FALL 
**					1<<1	MSK_INT_WATERMARK		Watermark 
**					1<<0	MSK_INT_OVERRUN		Overrun
**
**
**        Return Values:
**                uint8_t - the value of the register masked using the specified mask
**        Errors:
**
**
**        Description:
**				The function returns the value of the INT_ENABLE register masked using the provided value.
**
**
*/
uint8_t ACL::GetIntEnableEventBits(uint8_t bEventMask)
{
	return GetRegisterBits(ACL_ADR_INT_ENABLE, bEventMask);
}



/* ------------------------------------------------------------ */
/*        ACL::GetIntMapEventBits
**
**        Synopsis:
**
**        Parameters:
**				uint8_t bParIntNo	- The parameter indicating ACL the interrupt number. It can be one of the parameters from the following list:
**					0		PAR_INT_INT1
**					1		PAR_INT_INT2
**
**				uint8_t bEventMask	- the events that are verified if mapped to generate the specific interrupt. There can be one or more (OR-ed) parameters from the following list
**					1<<7	MSK_INT_DATA_READY	DATA_READY 
**					1<<6	MSK_INT_SINGLE_TAP	SINGLE_TAP 
**					1<<5	MSK_INT_float_TAP	float_TAP 
**					1<<4	MSK_INT_ACTIVITY	Activity
**					1<<3	MSK_INT_INACTIVITY	Inactivity 
**					1<<2	MSK_INT_FREE_FALL	FREE_FALL 
**					1<<1	MSK_INT_WATERMARK	Watermark 
**					1<<0	MSK_INT_OVERRUN		Overrun
**
**				 uint8_t *pErr 	- optional parameter - pointer to a value that receives error value. User may chose not to provide this parameter.
**					- ACL_ERR_SUCCESS (0) 	- The action completed successfully 
**					- ACL_ERR_ARG_AINTNO02_BIT (32) - bParIntNo (ACL Interrupt no) is not within 0-1 range
**
**        Return Values:
**                bool - the value of the register masked using the specified value
**
**        Errors:
**
**
**        Description:
**			The function returns the value of the INT_MAP register masked using the provided value.
**			If optional parameter pErr is provided, it will point to the error value: ACL_ERR_SUCCESS if success or ACL_ERR_ARG_AINTNO02_BIT if bParIntNo (ACL Interrupt no) is not within 0-1 range. 
**				
**
*/
bool ACL::GetIntMapEventBits(uint8_t bParIntNo, uint8_t bEventMask, uint8_t *pErr)
{
	uint8_t bRegValue;
	bool fResult;
	ReadRegisters(ACL_ADR_INT_MAP, 1, &bRegValue);
	fResult = (bParIntNo && (bRegValue & bEventMask)) || (!bParIntNo && (~bRegValue & bEventMask));
	return fResult;
}

/* ------------------------------------------------------------ */
/*        ACL::SetThresholdG
**
**        Synopsis:
**
**        Parameters:
**				uint8_t bThreshParam - byte indicating the threshold that will be set. Can be one of:
**					PAR_THRESH_TAP			- indicating tap treshold			
**                  PAR_THRESH_ACT	 		- indicating activity treshold
**                  PAR_THRESH_INACT 		- indicating inactivity treshold
**                  PAR_THRESH_FF			- indicating free fall treshold
**
**				float dTreshVal	– parameter containing Threshold in “g”.
**
**
**        Return Values:
**                uint8_t 
**					- ACL_ERR_SUCCESS (0) 	- The action completed successfully 
**					- a combination (ORed) of the following errors:
**						- ACL_ERR_ARG_RANGE_016G (2) - The dTreshVal argument is not within 0 - 16g range
**						- ACL_ERR_INVALID_THRESH (8)- The bThreshParam argument is not within 0 - 3 range
**
**        Errors:
**				see return value
**
**        Description:
**				This function sets the Tap, Activity, Inactivity or Free Fall Threshold, the value being given in “g”. The bThreshParam parameter specify which of the threshold values will be set.
**				The accepted threshold values are between 0 and 16g. If value is outside this range, ACL_ERR_ARG_RANGE_016G is returned and no value is set.
**				If argument is within the accepted values range, its value is quantified in 8-bit threshold register using a scale factor of 62.5 mg/LSB and ACL_ERR_SUCCESS is returned.
**				
**				
**
**
*/
uint8_t ACL::SetThresholdG(uint8_t bThreshParam, float dTreshVal)
{
	uint8_t bErr = ACL_ERR_SUCCESS;
	if(dTreshVal < 0 || dTreshVal > 16)
	{
		bErr |= ACL_ERR_ARG_RANGE_016G;
	}
	if(bThreshParam < PAR_THRESH_TAP || bThreshParam > PAR_THRESH_FF)
	{
		bErr |= ACL_ERR_INVALID_THRESH;
	}
	if(bErr == ACL_ERR_SUCCESS)
	{
		uint8_t bTreshVal = (uint8_t)(dTreshVal/(float)ACL_CONV_TRESH_G_LSB);
		switch(bThreshParam)
		{
			case PAR_THRESH_TAP:
				WriteRegisters(ACL_ADR_THRESH_TAP, 1, &bTreshVal);
				break;
			case PAR_THRESH_ACT:
				WriteRegisters(ACL_ADR_THRESH_ACT, 1, &bTreshVal);
				break;
			case PAR_THRESH_INACT:
				WriteRegisters(ACL_ADR_THRESH_INACT, 1, &bTreshVal);
				break;
			case PAR_THRESH_FF:
				WriteRegisters(ACL_ADR_THRESH_FF, 1, &bTreshVal);
				break;
		}
	}
	return bErr;		
}


/* ------------------------------------------------------------ */
/*        ACL::GetThresholdG
**
**        Synopsis:
**				dThreshTap = myACL.GetThresholdG(PAR_THRESH_TAP);
**					or
**				dThreshTap = myACL.GetThresholdG(PAR_THRESH_TAP, &err);
**        Parameters:
**				uint8_t bThreshParam - byte indicating the threshold that will be returned. Can be one of:
**					PAR_THRESH_TAP			- indicating tap treshold			
**                  PAR_THRESH_ACT	 		- indicating activity treshold
**                  PAR_THRESH_INACT 		- indicating inactivity treshold
**                  PAR_THRESH_FF			- indicating free fall treshold
**
**				 uint8_t *pErr 	- optional parameter - pointer to a value that receives error value. User may chose not to provide this parameter.
**					- ACL_ERR_SUCCESS (0) 	- The action completed successfully 
**					- ACL_ERR_INVALID_THRESH (8)- The bThreshParam argument is not within 0 - 3 range
**
**        Return Value:
**                float – the value of the Activity Threshold in “g”.
**
**        Errors:
**
**
**        Description:
**				This function returns the Tap, Activity, Inactivity or Free Fall Threshold, in “g”. The bThreshParam parameter specify which of the threshold values will be returned.
**				It converts the 8-bit value quantified in the specific threshold register into a value expressed in “g”, using a scale factor of 62.5 mg/LSB.
**				If optional parameter pErr is provided, it will point to the error value: ACL_ERR_SUCCESS if success or ACL_ERR_INVALID_THRESH if bThreshParam is not within 0 - 3 range.
**
**
*/
float ACL::GetThresholdG(uint8_t bThreshParam, uint8_t *pErr)
{
	uint8_t bTreshVal; 
	float dResult;
	uint8_t bErr = ACL_ERR_SUCCESS;
	
	ReadRegisters(ACL_ADR_THRESH_ACT, 1, &bTreshVal);
	switch(bThreshParam)
	{
		case PAR_THRESH_TAP:
			ReadRegisters(ACL_ADR_THRESH_TAP, 1, &bTreshVal);
			break;
		case PAR_THRESH_ACT:
			ReadRegisters(ACL_ADR_THRESH_ACT, 1, &bTreshVal);
			break;
		case PAR_THRESH_INACT:
			ReadRegisters(ACL_ADR_THRESH_INACT, 1, &bTreshVal);
			break;
		case PAR_THRESH_FF:
			ReadRegisters(ACL_ADR_THRESH_FF, 1, &bTreshVal);
			break;
		default:
			bErr = ACL_ERR_INVALID_THRESH;
			dResult = 0;
	}	
	dResult = (float)bTreshVal * (float)ACL_CONV_TRESH_G_LSB;
	if(pErr != NULL)
	{
		*pErr = bErr;
	}
	return dResult;
}

/* ------------------------------------------------------------ */
/*        ACL::SetTimeS
**
**        Synopsis:
**				myACL.SetTimeS(PAR_TIME_DUR, 0.01); // duration 10 ms
**        Parameters:
**				uint8_t bTimeParam - byte indicating the time values that will be set. Can be one of:
**					PAR_TIME_DUR		- indicating duration time			
**                  PAR_TIME_LATENT		- indicating latent time
**                  PAR_TIME_WINDOW		- indicating window time
**                  PAR_TIME_FF			- indicating free fall time
**				float dTime			- parameter containing time expressed in seconds. 
**
**
**        Return Value:
**                uint8_t 
**					- ACL_ERR_SUCCESS (0) 	- The action completed successfully 
**					- ACL_ERR_INVALID_TIME (14) 	- The bTimeParam is not in 0 - 3 range	
**					- Depending on bTimeParam, one of:
**						- ACL_ERR_ARG_RANGE_0160MS (2)	- The argument is not within 0 - 160ms range (for PAR_TIME_DUR)
**						- ACL_ERR_ARG_RANGE_DF_0320MS (4)	- The argument is not within 0 - 320ms range (for PAR_TIME_LATENT, PAR_TIME_WINDOW)
**						- ACL_ERR_ARG_RANGE_0128S (5) - The argument is not within 0- 1.28s range (for PAR_TIME_FF)
**
**        Errors:
**				see return value
**
**        Description:
**				This function sets the Duration, Latent, Window and Free Fall time, the value being given in seconds. The bTimeParam parameter specify which of the time values will be set.
**				If bTimeParam is outside the four time values, ACL_ERR_INVALID_TIME will be returned.
**				The accepted dTime argument values depend on the different times:
**					- if Duration time is not between 0 and 0.16s, ACL_ERR_ARG_RANGE_0160MS is returned. Otherwise a scale factor of 625 μs/LSB is used.
**					- if Latent time is not between 0 and 0.32s, ACL_ERR_ARG_RANGE_DF_0320MS is returned. Otherwise a scale factor of 1.25 ms/LSB is used.
**					- if Window time is not between 0 and 0.32s, ACL_ERR_ARG_RANGE_DF_0320MS is returned. Otherwise a scale factor of 1.25 ms/LSB is used.
**					- if Free Fall time is not between 0 and 1.28s, ACL_ERR_ARG_RANGE_0128S is returned. Otherwise a scale factor of 5 ms/LSB is used.
**				If argument is within the accepted values range, its value is quantified in specific 8-bit register using the scale factor described before and ACL_ERR_SUCCESS is returned.
**				If value is outside this range, specific errors are returned and no value is set.
**
**
*/
uint8_t ACL::SetTimeS(uint8_t bTimeParam, float dTime)
{
	uint8_t bResult;
	uint8_t bRegVal;
	switch (bTimeParam)
	{
		case PAR_TIME_DUR:
			if(dTime >= 0 && dTime <= 0.16)
			{
				bRegVal = (uint8_t)(dTime/(float)ACL_CONV_DURATION_S_LSB);
				WriteRegisters(ACL_ADR_DUR, 1, &bRegVal);
				bResult = ACL_ERR_SUCCESS;
			}
			else
			{
				bResult = ACL_ERR_ARG_RANGE_0160MS;
			}		
			break;
		case PAR_TIME_LATENT:
			if(dTime >= 0 && dTime <= 0.32)
			{
				bRegVal = (uint8_t)(dTime/(float)ACL_CONV_LATENT_S_LSB);
				WriteRegisters(ACL_ADR_LATENT, 1, &bRegVal);
				bResult = ACL_ERR_SUCCESS;
			}
			else
			{
				bResult = ACL_ERR_ARG_RANGE_0320MS;
			}
			break;
		case PAR_TIME_WINDOW:
			if(dTime >= 0 && dTime <= 0.32)
			{
				bRegVal = (uint8_t)(dTime/(float)ACL_CONV_WINDOW_S_LSB);
				WriteRegisters(ACL_ADR_WINDOW, 1, &bRegVal);
				bResult = ACL_ERR_SUCCESS;
			}
			else
			{
				bResult = ACL_ERR_ARG_RANGE_0320MS;
			}			
			break;
		case PAR_TIME_FF:
			if(dTime >= 0 && dTime <= 1.28)
			{
				bRegVal = (uint8_t)(dTime/(float)ACL_CONV_TIMEFREEFALL_S_LSB);
				WriteRegisters(ACL_ADR_TIME_FF, 1, &bRegVal);
				bResult = ACL_ERR_SUCCESS;
			}
			else
			{
				bResult = ACL_ERR_ARG_RANGE_0128S;
			}
			break;
		default:
			bResult = ACL_ERR_INVALID_TIME;
	}
	return bResult;	
}


/* ------------------------------------------------------------ */
/*        ACL::GetTimeS
**
**        Synopsis:
**				dLatent = GetTimeS(PAR_TIME_LATENT); // get latent
**					or
**				dLatent = GetTimeS(PAR_TIME_LATENT, &err);
**
**        Parameters:
**				uint8_t bTimeParam - byte indicating the time values that will be returned. Can be one of:
**					PAR_TIME_DUR		- indicating duration time			
**                  PAR_TIME_LATENT		- indicating latent time
**                  PAR_TIME_WINDOW		- indicating window time
**                  PAR_TIME_FF			- indicating free fall time
**				 uint8_t *pErr 	- optional parameter - pointer to a value that receives error value. User may chose not to provide this parameter.
**					- ACL_ERR_SUCCESS (0) 	- The action completed successfully 
**					- ACL_ERR_INVALID_TIME (14) 	- The bTimeParam is not in 0 - 3 range
**
**        Return Value:
**                float – the value of the specified time expressed in seconds.
**
**        Errors:
**
**
**        Description:
**				This function returns the Duration, Latent, Window and Free Fall time expressed in seconds. The bTimeParam parameter specify which of the time values will be set.
**				It converts the value quantified in the 8-bit register into a value expressed in seconds, using a scale factor from the following list:
**					-  625 μs/LSB for PAR_TIME_DUR.
**					- 1.25 ms/LSB for PAR_TIME_LATENT and PAR_TIME_WINDOW.
**					- 5 ms/LSB is used for PAR_TIME_FF.
**				If bTimeParam value is not among the ones from the list above (not in 0 - 3 range), if pErr is specified not null (the pErr parameter is optional), 
**				pErr it will point to the ACL_ERR_INVALID_TIME error.
**
**
*/
float ACL::GetTimeS(uint8_t bTimeParam, uint8_t *pErr)
{
	uint8_t bRegVal; 
	float dResult;
	uint8_t bErr = ACL_ERR_SUCCESS;
	switch (bTimeParam)
	{
		case PAR_TIME_DUR:
			ReadRegisters(ACL_ADR_DUR, 1, &bRegVal);
			dResult = (float)bRegVal * (float)ACL_CONV_DURATION_S_LSB;
			break;
		case PAR_TIME_LATENT:
			ReadRegisters(ACL_ADR_LATENT, 1, &bRegVal);
			dResult = (float)bRegVal * (float)ACL_CONV_LATENT_S_LSB;
			break;
		case PAR_TIME_WINDOW:
			ReadRegisters(ACL_ADR_WINDOW, 1, &bRegVal);
			dResult = (float)bRegVal * (float)ACL_CONV_WINDOW_S_LSB;
			break;
		case PAR_TIME_FF:
			ReadRegisters(ACL_ADR_TIME_FF, 1, &bRegVal);
			dResult = (float)bRegVal * (float)ACL_CONV_TIMEFREEFALL_S_LSB;
			break;
		default:
			bErr = ACL_ERR_INVALID_TIME;
			dResult = 0;
	}
	if(pErr != NULL)
	{
		*pErr = bErr;
	}
	return dResult;
}



/* ------------------------------------------------------------ */
/*        ACL::SetActiveInactiveControlBits
**
**        Synopsis:
**
**        Parameters:
**				uint8_t bActiveInactiveControlMask	- the mask containing the control bits. There can be one or more (OR-ed) parameters from the following list
**					1<<7	MSK_ACT_INACT_ACTACDC	ACT ac/dc 
**					1<<6	MSK_ACT_INACT_ACTXEN	ACT_X enable 
**					1<<5	MSK_ACT_INACT_ACTYEN	ACT_Y enable 
**					1<<4	MSK_ACT_INACT_ACTZEN	ACT_Z enable
**					1<<3	MSK_ACT_INACT_INACTACDC	INACT ac/dc 
**					1<<2	MSK_ACT_INACT_INACTXEN	INACT_X enable 
**					1<<1	MSK_ACT_INACT_INACTYEN	INACT_Y enable 
**					1<<0	MSK_ACT_INACT_INACTZEN	INACT_Z enable
**				bool fValue
**					-	if true, the bits corresponding to 1 values in the mask will get 1 value 
**					-	if false, the bits corresponding to 1 values in the mask will get 0 value
**
**
**        Return Values:
**                void 
**
**        Errors:
**
**
**        Description:
**				The function sets the appropriate (corresponding to the mask) bits in ACT_INACT_CTL register to 0 or 1 value.
**
**
*/
void ACL::SetActiveInactiveControlBits(uint8_t bActiveInactiveControlMask, bool fValue)
{
	SetRegisterBits(ACL_ADR_ACT_INACT_CTL, bActiveInactiveControlMask, fValue);
}



/* ------------------------------------------------------------ */
/*        ACL::GetActiveInactiveControlBits
**
**        Synopsis:
**
**        Parameters:
**				uint8_t bActiveInactiveControlMask	- the mask containing the control bits. There can be one or more (OR-ed) parameters from the following list.
**					1<<7	MSK_ACT_INACT_ACTACDC	ACT ac/dc 
**					1<<6	MSK_ACT_INACT_ACTXEN	ACT_X enable 
**					1<<5	MSK_ACT_INACT_ACTYEN	ACT_Y enable 
**					1<<4	MSK_ACT_INACT_ACTZEN	ACT_Z enable
**					1<<3	MSK_ACT_INACT_INACTACDC	INACT ac/dc 
**					1<<2	MSK_ACT_INACT_INACTXEN	INACT_X enable 
**					1<<1	MSK_ACT_INACT_INACTYEN	INACT_Y enable 
**					1<<0	MSK_ACT_INACT_INACTZEN	INACT_Z enable
**
**
**        Return Values:
**                uint8_t - the value of the register masked using the specified value
**
**        Errors:
**
**
**        Description:
**			The function returns the value of ACT_INACT_CTL register masked using the provided value.
**
**
*/
uint8_t ACL::GetActiveInactiveControlBits(uint8_t bActiveInactiveControlMask)
{
	return GetRegisterBits(ACL_ADR_ACT_INACT_CTL, bActiveInactiveControlMask);
}

/* ------------------------------------------------------------ */
/*        ACL::GetActTapStatusBits
**
**        Synopsis:
**
**        Parameters:
**				uint8_t bActTapStatusMask	- the axes verified if they are the first axes involved in a tap or activity event. There can be one or more (OR-ed) parameters from the following list
**					1<<6	MSK_ACT_TAP_STATUS_ACTXSOURCE	ACT_X source 
**					1<<5	MSK_ACT_TAP_STATUS_ACTYSOURCE	ACT_Y source 
**					1<<4	MSK_ACT_TAP_STATUS_ACTZSOURCE	ACT_Z source 
**					1<<3	MSK_ACT_TAP_STATUS_ASLEEP		Asleep 
**					1<<2	MSK_ACT_TAP_STATUS_INACTXSOURCE	TAP_X source 
**					1<<1	MSK_ACT_TAP_STATUS_INACTYSOURCE	TAP_Y source 
**					1<<0	MSK_ACT_TAP_STATUS_INACTZSOURCE	TAP_Z source**
**
**        Return Values:
**                uint8_t - the value of the register masked using the specified value 	
**
**        Errors:
**
**
**        Description:
**			The function returns the value of the ACT_TAP_STATUS register masked using the provided mask/value. 
**
**
*/
uint8_t ACL::GetActTapStatusBits(uint8_t bActTapStatusMask)
{
	return GetRegisterBits(ACL_ADR_ACT_TAP_STATUS, bActTapStatusMask);
}


/* ------------------------------------------------------------ */
/*        ACL::SetTapAxesBits
**
**        Synopsis:
**
**        Parameters:
**				uint8_t bTapAxesMask	- the mask containing the tap axes bits. There can be one or more (OR-ed) parameters from the list below:
**					1<<3	MSK_TAP_AXES_SUPPRESS	This mask is used in TAP_AXES register for Suppress bit.
**					1<<2	MSK_TAP_AXES_TAPXENABLE	This mask is used in TAP_AXES register for TAP_X bit.
**					1<<1	MSK_TAP_AXES_TAPYENABLE	This mask is used in TAP_AXES register for TAP_Y bit.
**					1<<0	MSK_TAP_AXES_TAPZENABLE	This mask is used in TAP_AXES register for TAP_Z bit.
**				bool fValue
**					-	if true, the bits corresponding to 1 values in the mask will get 1 value 
**					-	if false, the bits corresponding to 1 values in the mask will get 0 value
**
**
**        Return Values:
**                void 
**
**        Errors:
**
**
**        Description:
**				The function sets the appropriate (corresponding to the mask) bits in TAP_AXES register to 0 or 1 value.
**
**
*/
void ACL::SetTapAxesBits(uint8_t bTapAxesMask, bool fValue)
{
	SetRegisterBits(ACL_ADR_TAP_AXES, bTapAxesMask, fValue);
}



/* ------------------------------------------------------------ */
/*        ACL::GetTapAxesBits
**
**        Synopsis:
**
**        Parameters:
**				uint8_t bTapAxesMask	- the tap axes bits verified  if they are enabled. There can be one or more (OR-ed) parameters from the following list:
**					1<<3	MSK_TAP_AXES_SUPPRESS	This mask is used in TAP_AXES register for Suppress bit.
**					1<<2	MSK_TAP_AXES_TAPXENABLE	This mask is used in TAP_AXES register for TAP_X bit.
**					1<<1	MSK_TAP_AXES_TAPYENABLE	This mask is used in TAP_AXES register for TAP_Y bit.
**					1<<0	MSK_TAP_AXES_TAPZENABLE	This mask is used in TAP_AXES register for TAP_Z bit.
**
**
**        Return Values:
**                uint8_t - the value of the register masked using the specified value
**
**        Errors:
**
**
**        Description:
**				The function returns the value of TAP_AXES register masked using the provided mask.
**
**
*/
uint8_t ACL::GetTapAxesBits(uint8_t bTapAxesMask)
{
	return GetRegisterBits(ACL_ADR_TAP_AXES, bTapAxesMask);
}
