#define ledPin D1
#define ldrPin A0

int IntensityThreshold = 500;

void setup()
{
  Serial.begin(115200);
  pinMode(ledPin, OUTPUT);
  pinMode(ldrPin, INPUT);
}

void loop()
{
  int ldrValue = analogRead(ldrPin);
  
  Serial.println("Current Light Intensity Level (Min. 0 - Max. 1023): " + String(ldrValue));
  
  if (ldrValue < IntensityThreshold)
  {
    digitalWrite(ledPin, HIGH);
  }
  else
  {
    digitalWrite(ledPin, LOW);
  }
  delay (200);
}
