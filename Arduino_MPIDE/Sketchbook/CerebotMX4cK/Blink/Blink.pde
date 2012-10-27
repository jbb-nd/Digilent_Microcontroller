/*
  Blink
  Turns on an LED on for 200 ms, then off for 200 ms, repeatedly.
 
  This example code is in the public domain.
 */

#define LED_PIN 0          // LD3 on the board
// int led_pin = PIN_LED2;  // LD2 on the board, pin 65

void setup() {                
  // initialize the digital pin as an output.
  pinMode(LED_PIN, OUTPUT);     
}

void loop() {
  digitalWrite(LED_PIN, HIGH);   // set the LED on
  delay(200);                    // wait for 200 ms
  digitalWrite(LED_PIN, LOW);    // set the LED off
  delay(200);                    // wait for 200 ms
}
