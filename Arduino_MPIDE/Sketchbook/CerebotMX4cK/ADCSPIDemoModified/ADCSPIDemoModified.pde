/************************************************************************/
/*				                        		*/
/*	  ADCSPIDemoProject 		                                */
/*						                  	*/
/*					                        	*/
/************************************************************************/
/*	Author: Cristian Fatu						*/
/*	Copyright 2012, Digilent Inc.					*/
/************************************************************************/
/*  File Description: 			             		        */
/*					                        	*/
/* This file implements a simple demo application that demonstrates     */
/* how to use the ADCSPI library.				        */
/*									*/
/*	Functionality:							*/
/*									*/
/* In the setup() function, the application initializes                 */
/* the ADCSPI library object.                                           */
 /* In the loop() function, the application reads integer and physical  */
/* values from AD convertor and displays them on serial monitor.        */
/*					                        	*/
/*	Required Hardware:						*/
/*	  1. Cerebot MX4cK    					        */
/*	  2. PmodAD1 - plugged into JB connector (SPI0 interface)	*/
/************************************************************************/
/*  Revision History:			        			*/
/*					                        	*/
/*	01/10/2012(CristianF): created	       			        */
/*						                     	*/
/************************************************************************/

/* -------------------------------------------------------------------- */
/*				Include File Definitions  	        */
/* -------------------------------------------------------------------- */
#include <ADCSPI.h>

#include <DSPI.h> // DSPI library header must be included in the sketch

/* -------------------------------------------------------------------- */
/*				Global Variables		        */
/* -------------------------------------------------------------------- */
  ADCSPI myADCSPI; // the library object
  unsigned int wValue; //  unsigned 16 bit variable to store integer value
  float fValue;  // float variable to store physical value
  char sMsg[100];  // character string to keep message that is displayed on serial monitor

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
**		 Performs basic initialization.		
/*** ---------------------------------------------------------- ***/
void setup()
{ 
  //Create a connection to display the data on the serial monitor.
  Serial.begin(9600);
  
  // initialize PmodADCSPI on SPI
myADCSPI.begin(PAR_ACCESS_SPI0); // corresponds to DSPI0 - connector JB
//myADCSPI.begin(PAR_ACCESS_SPI1); // corresponds to DSPI1 - connector J1
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
**		Main program module. Enters the main program loop.
/*** ---------------------------------------------------------- ***/
void loop()
{  
  wValue = myADCSPI.GetIntegerValue();  // read integer value
  fValue = myADCSPI.GetPhysicalValue();  // read physical value
  sprintf(sMsg, "Integer value: %d, Physical Value = %f", wValue, fValue); // format text to be displayed 
  Serial.println(sMsg);  // display text on serial monitor
}
