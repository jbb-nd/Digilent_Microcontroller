
/************************************************************************/
/*									*/
/*  OLEDDemo  --  OLED Display demo                                     */
/*									*/
/************************************************************************/
/*  Author: 	Oliver Jones						*/
/*  Copyright 2011, Digilent Inc.					*/
/************************************************************************/
/*
  This program is free software; you can redistribute it and/or
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
/*  Module Description: 						*/
/*									*/
/*  This program demonstrates the basic operation of the OLED graphics  */
/*  display.  It illustrates the  initialization of the display and     */
/*  some basic character and graphic operations.                        */
/*									*/
/************************************************************************/
/*  Revision History:							*/
/*									*/
/*  06/01/2011(GeneA): created						*/
/*  08/04/2011(GeneA): prepare first release				*/
/*									*/
/************************************************************************/

/************************************************************************/
/*  Board Support:							*/
/*									*/
/*  chipKit Uno with Pmod Shield:     Header JC	                	*/
/*   (Note: To use this, download and deploy the Board Variant for      */
/*          chipKIT UNO32 W/Pmod Shield from the Digilent website)      */              
/*  Cerebot Mx3ck:                    Header JE                         */
/*  Cerebot Mx4ck:                    Header JB                         */
/*  Cerebot Mx7ck:                    Header JD                         */
/************************************************************************/

#include <DSPI.h>
#include <OLED.h>

OledClass OLED;

void setup()
{
  OLED.begin();
}

void loop()
{
  int irow;
  int ib;

  //Clear the virtual buffer
  OLED.clearBuffer();
  
  //Chosing Fill pattern 0
  OLED.setFillPattern(OLED.getStdPattern(0));
  //Turn automatic updating off
  OLED.setCharUpdate(0);
  
  //Draw a rectangle over wrting then slide the rectagle
  //down slowly displaying all writing
  for (irow = 0; irow < OLED.rowMax; irow++)
  {
    OLED.clearBuffer();
    OLED.setCursor(0, 0);
    OLED.putString("chipKIT");
    OLED.setCursor(0, 1);
    OLED.putString("Basic I/O Shield");
    OLED.setCursor(0, 2);
    OLED.putString("by Digilent");
    
    OLED.moveTo(0, irow);
    OLED.drawFillRect(127,31);
    OLED.moveTo(0, irow);
    OLED.drawLine(127,irow);
    OLED.updateDisplay();
    delay(100);
  }
  
  delay(1000);
  
  // Blink the display a bit.
  OLED.displayOff();
  delay(500);
  OLED.displayOn();
  delay(500);
  
  OLED.displayOff();
  delay(500);
  OLED.displayOn();
  delay(500);

  OLED.displayOff();
  delay(500);
  OLED.displayOn();
  delay(500);

  delay(2000);
  
  // Now erase the characters from the display
  for (irow = OLED.rowMax-1; irow >= 0; irow--) {
    OLED.setDrawColor(1);
    OLED.setDrawMode(OLED.modeSet);
    OLED.moveTo(0,irow);
    OLED.drawLine(127,irow);
    OLED.updateDisplay();
    delay(25);
    OLED.setDrawMode(OLED.modeXor);
    OLED.moveTo(0, irow);
    OLED.drawLine(127, irow);
    OLED.updateDisplay();
  }
  
  delay(1000);  

  // Draw a rectangle in center of screen
  // Display the 8 different patterns availible
  OLED.setDrawMode(OLED.modeSet);

  for(ib = 1; ib < 8; ib++)
  {
    OLED.clearBuffer();
    
    OLED.setFillPattern(OLED.getStdPattern(ib));
    OLED.moveTo(55, 1);
    OLED.drawFillRect(75, 27);
    OLED.drawRect(75, 27);
    OLED.updateDisplay();
    
    delay(1000);
  }
}
