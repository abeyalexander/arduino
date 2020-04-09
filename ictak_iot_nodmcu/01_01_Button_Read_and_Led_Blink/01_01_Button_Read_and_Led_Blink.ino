#define buttonPin D2
#define ledPin D1

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
    delay(500);
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
