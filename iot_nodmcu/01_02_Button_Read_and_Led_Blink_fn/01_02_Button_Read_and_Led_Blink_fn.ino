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
    led_blink(200);
  }
  else
  {
    led_blink(500);
  }
}

void led_blink(int delay_ms)
{
  digitalWrite (ledPin, HIGH);
  delay(delay_ms);
  digitalWrite (ledPin, LOW);
  delay(delay_ms);
}
