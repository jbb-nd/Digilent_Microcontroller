// Using 4 pin LED display
#include <LedDisplay.h>

int d3 = PIN_LED4;
int d2 = PIN_LED3;
int d1 = PIN_LED2;
int d0 = PIN_LED1;

LedDisplay disp(d3, d2, d1, d0);

void setup() 
{
  pinMode(d3, OUTPUT);
  pinMode(d2, OUTPUT);
  pinMode(d1, OUTPUT);
  pinMode(d0, OUTPUT);
}

void loop()
{
  disp.cycle();
}
  



