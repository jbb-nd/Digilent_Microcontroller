// Infrared Reflective Sensor Test
// On Cerebot MX4cK install PmodLS on JA top

const int sensorPin = 0;

void setup()
{
  pinMode(sensorPin, INPUT);
  Serial.begin(9600);
}

void loop()
{
  int sensorValue;
  
  sensorValue = digitalRead(sensorPin);
  Serial.println(sensorValue);
}
  
