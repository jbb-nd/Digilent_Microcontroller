/************************************************************************/
/*				                        		*/
/*	  ACLDemoProject 		                                */
/*						                  	*/
/*					                        	*/
/************************************************************************/
/*	Author: Cristian Fatu						*/
/*	Copyright 2011, Digilent Inc.					*/
/************************************************************************/
/*  File Description: 			             		        */
/*					                        	*/
/* This file implements a simple demo application that demonstrates     */
/* how to use the ACL library.				                */
/*									*/
/*	Functionality:							*/
/*									*/
/* In the setup() function, the application instantiates and initializes*/
/* the ACL library object. Then, it performs calibration.               */
/* In the loop() function, the application reads accelerations and      */
/* displays their values on the serial terminal. It also displays tap   */
/* or double tap information.                                           */
/* In the tap() function (called when an accelerometer interrupt occurs)*/
/* the source of the interrupt is detected.                             */
/************************************************************************/
/*  Revision History:			        			*/
/*					                        	*/
/*	11/20/2011 (CristianF): created		       			*/
/*						                     	*/
/************************************************************************/

/************************************************************************/
/*  Board Support for DSPI0:						*/
/*									*/
/*  chipKit Uno with Pmod Shield:     Header JC	                        */
/*   (Note: To use this, download and deploy the Board Variant for      */
/*          chipKIT UNO32 W/Pmod Shield from the Digilent website)      */              
/*  Cerebot Mx3ck:                    Header JE                         */
/*  Cerebot Mx4ck:                    Header JB                         */
/*  Cerebot Mx7ck:                    Header JD                         */
/************************************************************************/




/* -------------------------------------------------------------------- */
/*				Include File Definitions  	        */
/* -------------------------------------------------------------------- */
#include <ACL.h>
// Wire and DSPI libraries headers must be included in the sketch
#include <Wire.h>
#include <DSPI.h>
/* -------------------------------------------------------------------- */
/*				Global Variables		        */
/* -------------------------------------------------------------------- */
ACL myACL; // the library object
byte bTapInfo;    // this variable stores information about tap type (single or double), set in tap() function and used in loop() function

/* -------------------------------------------------------------------- */
/*	               Procedure Definitions	                        */
/* -------------------------------------------------------------------- */
/***	setup
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
**		 Performs basic board initialization.		
/*** ---------------------------------------------------------- ***/
void setup()
{ 
//Create a serial connection to display the data on the terminal.
  Serial.begin(9600);
  // initialize PmodACL on SPI or I2C
  myACL.begin(PAR_ACCESS_SPI0); // corresponds to DSPI0 - connector JB
//  myACL.begin(PAR_ACCESS_SPI1); // corresponds to DSPI1 - connector J1
//  myACL.begin(PAR_ACCESS_I2C); // corresponds to I2C #1

  //Put the device into standby mode to configure it.
  myACL.SetSFRBit(ACL_BIT_POWER_CTL_MEASURE, false);

  
  // set data range to +/- 4g
  myACL.SetDataFormatGRangePar(PAR_DATAFORMAT_PM4G);

  // configure the interrupt related information
  myACL.SetTapAxesBits(MSK_TAP_AXES_TAPZENABLE, true);
  myACL.SetThresholdG(PAR_THRESH_TAP, 3); //Set the Tap Threshold to 3g
  myACL.SetTimeS(PAR_TIME_DUR, 0.01); // duration 10 ms
  myACL.SetTimeS(PAR_TIME_LATENT, 0.1); // latent 100 ms - 100ms Latency before the second tap can occur.
  myACL.SetTimeS(PAR_TIME_WINDOW, 0.318); // window 318 ms 
//  myACL.SetThresholdTapG(3); //Set the Tap Threshold to 3g
//  myACL.SetDurationS(0.01); // duration 10 ms
//  myACL.SetLatentS(0.1); // latent 100 ms - 100ms Latency before the second tap can occur.
//  myACL.SetWindowS(0.318); // window 318 ms 
  
  myACL.ConfigureInterrupt(PAR_ACL_INT1, PAR_EXT_INT2, MSK_INT_SINGLE_TAP | MSK_INT_DOUBLE_TAP, &tap, PAR_INT_ACTIVEHIGH);
  // External INT2 corresponds to pin 36 INT2/RE9
  // set Measure bit to true
  myACL.SetSFRBit(ACL_BIT_POWER_CTL_MEASURE, true);

  // calibrate the accelerometer using Z axis as gravitational
  myACL.CalibrateOneAxisGravitational(PAR_AXIS_ZP);
}

/*** ---------------------------------------------------------- ***/
/***	loop
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
**		Main program module.Enters the main program loop.
/*** ---------------------------------------------------------- ***/
void loop()
{  
  char strMes[150];

  float dX, dY, dZ;

  // read accelerometer values in g
  myACL.ReadAccelG(dX, dY, dZ);
  
  // format the display string
  sprintf(strMes, "X:%.3f, Y:%.3f, Z: %.3f", dX, dY, dZ);  
  
  // send the string to display
  Serial.println(strMes);
  
  // display tap information
  if(bTapInfo > 0)
  {
    if(bTapInfo == 1)
    {
      Serial.println("SINGLE TAP");
    }
    else
    {
      Serial.println("DOUBLE TAP");
    }
    delay(500);
    bTapInfo = 0;    
  }
  delay(100); 
}

/* ------------------------------------------------------------------- */
/** void  tap()
**
**	Parameters:
**
**	Return Value:
**		none
**
**	Errors:
**		none
**
**	Description:
**	  This function is called when accelerometer interrupts occur. It was configurated using  myACL.ConfigureInterrupt() function.
**        The function detects the interrupt source (single or double tap) and sets global variable bTapInfo accordingly.
**
-----------------------------------------------------------------------*/

// 
void tap()
{  // reading the INT_SOURCE register causes interrupt flags to be cleared
  if(myACL.GetInterruptSourceBits(MSK_INT_DOUBLE_TAP))
  {
    // the interrupt is triggered by a double-tap 
    bTapInfo = 2; 
  }
  else
  {
    // the interrupt is triggered by a single-tap 
    bTapInfo = 1;
  }
}


