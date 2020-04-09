#define input_pin D2
#define LED_pin D3


void setup()
{
  Serial.begin(9600);
  pinMode(input_pin, INPUT);
  pinMode(LED_pin, OUTPUT);
}
void loop()
{
  if (digitalRead(input_pin) == HIGH)
  {
    digitalWrite(LED_pin, HIGH);
    Serial.println("Motion detected ");
    delay(1000);
  }
  else
  {
    digitalWrite(LED_pin, LOW);
    Serial.println("No motion ");
    delay(200);
  }
}
