/*
  Blink
  Turns on an LED on for one second, then off for one second, repeatedly.
 
  This example code is in the public domain.
 */

// int led_pin = 0;  // JA-01
int led_pin = 71;
// int led_pin = 66;  // LD3 on the board
// int led_pin = PIN_LED2;  // LD2 on the board, pin 65

void setup() {                
  // initialize the digital pin as an output.
  // Pin 13 has an LED connected on most Arduino boards:
  pinMode(led_pin, OUTPUT);     
}

void loop() {
  digitalWrite(led_pin, HIGH);   // set the LED on
  delay(200);              // wait for a second
  digitalWrite(led_pin, LOW);    // set the LED off
  delay(200);              // wait for a second
}
