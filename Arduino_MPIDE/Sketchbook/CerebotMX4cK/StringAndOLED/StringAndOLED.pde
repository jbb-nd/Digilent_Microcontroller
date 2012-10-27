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
  String stringOne = String("Value is");
  char charArray[32];
  float pi = 3.14;
  
  //stringOne.toCharArray(charArray, 32);
  sprintf(charArray, "Pi = %4.2f", pi);
  
  OLED.clearBuffer();
  OLED.setCursor(0, 0);
  OLED.putString(charArray);
}
