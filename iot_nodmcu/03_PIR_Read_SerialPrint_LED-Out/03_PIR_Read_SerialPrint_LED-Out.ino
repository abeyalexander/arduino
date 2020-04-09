#define ledPin D0
#define pirPin D2
#define pirDelay 7

long tick = millis();

void setup()
{
  Serial.begin(115200);
  pinMode(ledPin, OUTPUT);
  pinMode(pirPin, INPUT);
}

void loop()
{
  int pirValue = digitalRead(pirPin);
  if (pirValue)
  {
    digitalWrite(ledPin, LOW);
    Serial.println("Motion Detected!!!...");

    for (int i = 0; i <= pirDelay; i++)
    {
      Serial.print(String(pirDelay - i) + " ");
      delay(1000);
    }
    Serial.println();
  }
  else
  {
    digitalWrite(ledPin, HIGH);
    if ((millis() - tick) > 1000)
    {
      Serial.println("Scanning...");
      tick = millis();
    }
  }
}
