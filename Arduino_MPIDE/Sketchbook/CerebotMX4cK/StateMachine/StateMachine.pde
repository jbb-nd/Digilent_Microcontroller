// Demonstrate finite state machine

#include <Bounce.h>

int L0 = PIN_LED1;
int L1 = PIN_LED2;
int L2 = PIN_LED3;
int L3 = PIN_LED4;

int button = PIN_BTN1;
int state;
Bounce debouncer = Bounce(button, 5);

void setup()
{
  pinMode(L0, OUTPUT);
  pinMode(L1, OUTPUT);
  pinMode(L2, OUTPUT);
  pinMode(L3, OUTPUT);
  pinMode(button, INPUT);
  state = 0;
}

void loop()
{
  int buttonVal;
  
  // buttonVal = digitalRead(button);
  debouncer.update();
  buttonVal = debouncer.read();
  
  switch (state) {
    case 0:
      if (buttonVal == HIGH)
        state = 1;
       else
         state = 0;
      break;
    case 1: 
      if (buttonVal == LOW)
        state = 2;
      else
        state = 1;
      break;
    case 2: 
      if (buttonVal == HIGH)
        state = 3;
      else
        state = 2;
      break;
    case 3: 
      if (buttonVal == LOW)
        state = 0;
      else
        state = 3;
      break;
  }
    
  switch (state) {
    case 0: 
      digitalWrite(L0, HIGH);
      digitalWrite(L1, LOW);
      digitalWrite(L2, LOW);
      digitalWrite(L3, LOW);
      break;
    case 1: 
      digitalWrite(L0, LOW);
      digitalWrite(L1, HIGH);
      digitalWrite(L2, LOW);
      digitalWrite(L3, LOW);
      break;
    case 2: 
      digitalWrite(L0, LOW);
      digitalWrite(L1, LOW);
      digitalWrite(L2, HIGH);
      digitalWrite(L3, LOW);
      break;
    case 3: 
      digitalWrite(L0, LOW);
      digitalWrite(L1, LOW);
      digitalWrite(L2, LOW);
      digitalWrite(L3, HIGH);
      break;
  }
}
