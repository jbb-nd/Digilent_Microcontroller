/************************************************************************/
/*																		*/
/*	NetworkProfile.x                                                    */
/*																		*/
/*	Network Hardware vector file for the MRF24WB0MA PmodWiFi            */
/*																		*/
/************************************************************************/
/*	Author: 	Keith Vogel 											*/
/*	Copyright 2011, Digilent Inc.										*/
/************************************************************************/
/*
  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/
/************************************************************************/
/*  Revision History:													*/
/*																		*/
/*	5/2/2012(KeithV): Created											*/
/*																		*/
/************************************************************************/

#ifndef PMODWIFI_NETWORK_PROFILE_X
#define PMODWIFI_NETWORK_PROFILE_X

#define DWIFIcK_WiFi_Hardware
#define DNETcK_MRF24WB0MA

// board specific stuff
#if defined(_BOARD_UNO_)

    #include <Uno32-MRF24WB0MA.x>

#elif defined (_BOARD_MEGA_)

    #include <Max32-MRF24WB0MA.x>

#elif defined (_BOARD_CEREBOT_MX3CK_)

    #include <MX3cK-MRF24WB0MA.x>

#elif defined (_BOARD_CEREBOT_MX4CK_)

    #include <MX4cK-MRF24WB0MA.x>

#elif defined (_BOARD_CEREBOT_MX7CK_)

    #include <MX7cK-MRF24WB0MA.x>

#elif defined (_BOARD_FUBARINO_)

    #include <FubarSD-MRF24WB0MA.x>

#else

    #error Neither the WiFi Shield nor PmodWiFi is supported by this board.

#endif

// Set the SPI interrupt to use; Only defined for INT 1,2,3... other aren't setup to be used.
#if defined(MRF24WB0M_IN_SPI_INT1)
	#define WF_INT_EDGE			(INTCONbits.INT1EP)
	#define WF_INT_IE			(IEC0bits.INT1IE)
	#define WF_INT_IF			(IFS0bits.INT1IF)
	#define WF_INT_BIT			0x00000080		    // INT 1
	#define WF_INT_IPCSET		IPC1SET
	#define WF_INT_IPCCLR		IPC1CLR
#elif defined(MRF24WB0M_IN_SPI_INT2)
	#define WF_INT_EDGE			(INTCONbits.INT2EP)
	#define WF_INT_IE			(IEC0bits.INT2IE)
	#define WF_INT_IF			(IFS0bits.INT2IF)
	#define WF_INT_BIT			0x00000800		    // INT 2
	#define WF_INT_IPCSET		IPC2SET
	#define WF_INT_IPCCLR		IPC2CLR
#elif defined(MRF24WB0M_IN_SPI_INT3)
	#define WF_INT_EDGE			(INTCONbits.INT3EP)
	#define WF_INT_IE			(IEC0bits.INT3IE)
	#define WF_INT_IF			(IFS0bits.INT3IF)
	#define WF_INT_BIT			0x00008000	        // INT 3	
	#define WF_INT_IPCSET		IPC3SET
	#define WF_INT_IPCCLR		IPC3CLR
#else
    #error unsupported MRF24WB0M SPI interrupt
#endif

    // common SPI interrupt stuff; careful only good for INT 1,2,3
    #define WF_INT_IE_CLEAR		IEC0CLR
	#define WF_INT_IF_CLEAR		IFS0CLR
	#define WF_INT_IE_SET		IEC0SET
	#define WF_INT_IF_SET		IFS0SET
	#define WF_INT_IPC_MASK		0xFF000000
	#define WF_INT_IPC_VALUE    0x0C000000

// setup for SPI 1,2,3,4  Only the MX7cK and Max32 support SPI 3 & 4.
#if defined(MRF24WB0M_IN_SPI1)
	#define WF_SSPBUF			(SPI1BUF)
	#define WF_SPISTAT			(SPI1STAT)
	#define WF_SPISTATbits		(SPI1STATbits)
	#define WF_SPICON1			(SPI1CON)
	#define WF_SPICON1bits		(SPI1CONbits)
	#define WF_SPI_BRG			(SPI1BRG)
	#define WF_SPI_INT_BITS		0x03800000  // SPI1 
	#define WF_SPI_IE_CLEAR		IEC0CLR
	#define WF_SPI_IF_CLEAR		IFS0CLR
#elif defined(MRF24WB0M_IN_SPI2)
	#define WF_SSPBUF			(SPI2BUF)
	#define WF_SPISTAT			(SPI2STAT)
	#define WF_SPISTATbits		(SPI2STATbits)
	#define WF_SPICON1			(SPI2CON)
	#define WF_SPICON1bits		(SPI2CONbits)
	#define WF_SPI_BRG			(SPI2BRG)
	#define WF_SPI_INT_BITS		0x000000E0  // SPI2 and I think SPI2A  
	#define WF_SPI_IE_CLEAR		IEC1CLR
	#define WF_SPI_IF_CLEAR		IFS1CLR
#elif defined(MRF24WB0M_IN_SPI3)
	#define WF_SSPBUF			(SPI3BUF)
	#define WF_SPISTAT			(SPI3STAT)
	#define WF_SPISTATbits		(SPI3STATbits)
	#define WF_SPICON1			(SPI3CON)
	#define WF_SPICON1bits		(SPI3CONbits)
	#define WF_SPI_BRG			(SPI3BRG)
	#define WF_SPI_INT_BITS		0x1C000000  // SPI3 or SPI1A  
	#define WF_SPI_IE_CLEAR		IEC0CLR
	#define WF_SPI_IF_CLEAR		IFS0CLR
#elif defined(MRF24WB0M_IN_SPI4)
	#define WF_SSPBUF			(SPI4BUF)
	#define WF_SPISTAT			(SPI4STAT)
	#define WF_SPISTATbits		(SPI4STATbits)
	#define WF_SPICON1			(SPI4CON)
	#define WF_SPICON1bits		(SPI4CONbits)
	#define WF_SPI_BRG			(SPI4BRG)
	#define WF_SPI_INT_BITS		0x00000700  // SPI4 or SPI3A 
	#define WF_SPI_IE_CLEAR		IEC1CLR
	#define WF_SPI_IF_CLEAR		IFS1CLR
#else
    #error unsupported MRF24WB0M SPI port
#endif

#define DNETcKInitNetworkHardware() {WF_CS_IO = 1;WF_CS_TRIS = 0;}

#endif // PMODWIFI_NETWORK_PROFILE_X
