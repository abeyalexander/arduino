int sensorPin = A0;   
int ledPin_1 = 12;
int ledPin_2 = 11;
int pwmWritePin = 10;
int sensorValue = 0;

void setup() 
{
  // declare the ledPin as an OUTPUT:
  pinMode(ledPin, OUTPUT);
}

void loop() 
{
  digitalWrite(ledPin_1, HIGH);
  sensorValue = analogRead(sensorPin);
  digitalWrite(ledPin_1, LOW);
  digitalWrite(ledPin_2, HIGH);
  analogWrite(pwmWritePin,sensorValue);
  digitalWrite(ledPin_2, LOW);
}
