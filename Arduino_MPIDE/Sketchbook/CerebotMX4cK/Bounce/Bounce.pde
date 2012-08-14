#include <Bounce.h>

// This code turns a led on/off through a debounced button
// Build the circuit indicated here: http://arduino.cc/en/Tutorial/Button

#define BUTTON PIN_BTN1
#define LED PIN_LED1

// Instantiate a Bounce object with a 5 millisecond debounce time
Bounce bouncer = Bounce( BUTTON,5 ); 

void setup() {
  pinMode(BUTTON,INPUT);
  pinMode(LED,OUTPUT);
}

void loop() {
 // Update the debouncer
  bouncer.update ( );
 
 // Get the update value
 int value = bouncer.read();
 
 // Turn on or off the LED
 if ( value == HIGH) {
   digitalWrite(LED, HIGH );
 } else {
    digitalWrite(LED, LOW );
 }
 
}

