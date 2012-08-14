// Finite State Machine example

#include <Bounce.h>

#define START  0
#define ONE    1
#define TWO    2
#define THREE  3

#define BTN    42 //PIN_BTN1
#define BIT0   64 //PIN_LED1
#define BIT1   65 //PIN_LED2
#define BIT2   66 //PIN_LED3
#define BIT3   67 //PIN_LED4

Bounce debounced_btn = Bounce(BTN, 5);
int state = 0;

void setup()
{
  pinMode(BTN, INPUT);
  pinMode(BIT0, OUTPUT);
  pinMode(BIT1, OUTPUT);
  pinMode(BIT2, OUTPUT);
  pinMode(BIT3, OUTPUT);
  displayBinary(state);
}

void loop()
{
  int buttonPress;
  
  debounced_btn.update();
  buttonPress = debounced_btn.risingEdge();
  displayBinary(state);
  
  switch (state) {
    case START:
      if (buttonPress)
        state = ONE;
      else
        state = START;
      break;
    case ONE:
      if (buttonPress)
        state = TWO;
      else
        state = ONE;
      break;
    case TWO:
      if (buttonPress)
        state = THREE;
      else
        state = TWO;
      break;
    case THREE:
      if (buttonPress)
        state = START;
      else
        state = THREE;
      break;
  }
}

void displayBinary(int num)
{
  if (num & 1)
    digitalWrite(BIT0, HIGH);
  else
    digitalWrite(BIT0, LOW);
  
  if (num & 2)
    digitalWrite(BIT1, HIGH);
  else
    digitalWrite(BIT1, LOW);
  
  if (num & 4)
    digitalWrite(BIT2, HIGH);
  else
    digitalWrite(BIT2, LOW);
  
  if (num & 8)
    digitalWrite(BIT3, HIGH);
  else
    digitalWrite(BIT3, LOW);
}
  
