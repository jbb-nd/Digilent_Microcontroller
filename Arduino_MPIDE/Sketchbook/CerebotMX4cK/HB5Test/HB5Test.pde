// Two HB5 H-bridges connected on JD of the Cerebot MX4cK
// NOTE:  ** Do not switch directions while motor is on to avoid damaging H-bridge **

const int left_dir   = 24;   // JC-01
const int left_en    = 25;   // JC-02
const int right_dir  = 28;   // JC-07
const int right_en   = 29;   // JC-08

void setup()
{
  pinMode(left_dir, OUTPUT);
  pinMode(left_en, OUTPUT);
  pinMode(right_dir, OUTPUT);
  pinMode(right_en, OUTPUT);
}

void loop()
{
  digitalWrite(left_dir, HIGH);
  digitalWrite(left_en, LOW);
  digitalWrite(right_dir, HIGH);
  digitalWrite(right_en, HIGH);
  delay(500);
  digitalWrite(left_dir, HIGH);
  digitalWrite(left_en, HIGH);
  digitalWrite(right_dir, HIGH);
  digitalWrite(right_en, LOW);
  delay(500);
  digitalWrite(left_dir, LOW);
  digitalWrite(left_en, LOW);
  digitalWrite(right_dir, LOW);
  digitalWrite(right_en, HIGH);
  delay(500);
  digitalWrite(left_dir, LOW);
  digitalWrite(left_en, HIGH);
  digitalWrite(right_dir, LOW);
  digitalWrite(right_en, LOW);
  delay(500);
}
