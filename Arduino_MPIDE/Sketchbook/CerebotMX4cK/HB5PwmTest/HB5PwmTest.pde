// PWM speed control of motor through HB5 on Cerebot MX4cK

const int left_dir   = 24;   // JD-01
const int left_en    = 25;   // JD-02
const int right_dir  = 28;   // JD-07
const int right_en   = 29;   // JD-08

int s;

void setup()
{
  s = 1;
  Serial.begin(9600);
  digitalWrite(left_dir, HIGH);
}

void loop()
{
  int motorSpeed;
  
  if (s < 256)
    s = 2 * s;
  else
    s = 1;
    
  motorSpeed = s - 1;
  analogWrite(left_en, motorSpeed);
  Serial.println(motorSpeed);
  delay(500);
}
