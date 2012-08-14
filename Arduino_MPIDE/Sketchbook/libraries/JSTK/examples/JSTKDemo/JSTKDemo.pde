/************************************************************************/
/*									*/
/*  JSTKDemo.pde	-- Example Sketch for JSTK			*/
/*									*/
/************************************************************************/
/*  Author:	Oliver Jones						*/
/*  Copyright (c) 2011, Digilent Inc.  	    				*/
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
/*  Module Description:							*/
/*									*/
/*  This sketch is an example on on how to read the status of a joystick*/
/* with the JSTK library.                                               */
/************************************************************************/
/*  Revision History:							*/
/*									*/
/*  10/21/2011(Oliver J): Created                                       */
/*									*/
/************************************************************************/
/************************************************************************/
/*  Board Support:							*/
/*									*/
/*  chipKit Uno with Pmod Shield:     Header JC	(Top)                	*/
/*   (Note: Select Cerebot MX3ck in Tools | Board to use this           */
/*                    configuration)                                    */              
/*  Cerebot Mx3ck:                    Header JE (Top)                   */
/*  Cerebot Mx4ck:                    Header JB (Top)                   */
/*  Cerebot Mx7ck:                    Header JD (Top)                   */
/*                                              			*/
/************************************************************************/

#include <DSPI.h>
#include <JSTK.h>

uint16_t xAxis = 0;
uint16_t yAxis = 0;
uint8_t btnSt = 0;

JSTK myJSTK;

void setup() {
  Serial.begin(9600);
  Serial.println("Joystick Demo");
  
  myJSTK.begin();
}

uint8_t recv[5];

void loop() {
  xAxis = myJSTK.getX();
  yAxis = myJSTK.getY();
  btnSt = myJSTK.getBtn();
  
  Serial.print("xAxis: ");
  Serial.println(xAxis,DEC);
  Serial.print("yAxis: ");
  Serial.println(yAxis,DEC);
  Serial.print("Button State: ");
  Serial.println(btnSt,HEX);
  
  //if both button is pressed 
  if((btnSt & JSTK_BTN1) && (btnSt & JSTK_BTN2))
  {
    //turn on LED1 and LED2
    myJSTK.setLeds(JSTK_LED1 | JSTK_LED2);
  } 
  //if only button 1 is pressed
  else if(btnSt & JSTK_BTN1)
  {
    //turn on LED1
    myJSTK.setLeds(JSTK_LED1);
  }
  //if only button 2 is pressed
  else if(btnSt & JSTK_BTN2)
  {
    //turn on LED2
    myJSTK.setLeds(JSTK_LED2);
  }
  else
  {
    //All LEDs off
    myJSTK.setLeds(0);
  }
  
  delay(1000);
}
  
  
  
