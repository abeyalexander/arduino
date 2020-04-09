#define buttonPin D2
#define ledPin D4

void setup()
{
  pinMode(ledPin, OUTPUT);
  pinMode(buttonPin, INPUT);
}

void loop()
{
  bool buttonStatus = digitalRead(buttonPin);
  if (buttonStatus)
  {
    digitalWrite(ledPin, HIGH);
    delay(100);
    digitalWrite(ledPin, LOW);
    delay(200);
  }
  else
  {
    digitalWrite(ledPin, LOW);
  }
}

void led_blink(int delay_ms)
{
  digitalWrite (ledPin, HIGH);
  delay(delay_ms);
  digitalWrite (ledPin, LOW);
  delay(delay_ms);
}
