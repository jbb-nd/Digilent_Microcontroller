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
int buttonState;

const int buttonPin = PIN_BTN1;

void setup()
{
  OLED.begin();
  pinMode(buttonPin, INPUT);
}

void loop()
{
  OLED.clearBuffer();
  OLED.setCursor(0, 0);
  buttonState = digitalRead(buttonPin);
  if (buttonState == HIGH)
    OLED.putString("Button Pressed");
  else
    OLED.putString("Button Open");
}
