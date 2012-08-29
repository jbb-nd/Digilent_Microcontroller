int count;

void setup()
{
  // initialize serial communications at 9600 bits per second
  Serial.begin(9600);
}

void loop()
{
  for (count = 0;  count <= 10;  count++) {
    Serial.print("count = ");
    Serial.println(count);
    delay(100);
  }
}
    
  
