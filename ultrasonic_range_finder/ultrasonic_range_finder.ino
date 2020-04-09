long duration;
long cm;
void setup() {
  pinMode(D1, OUTPUT); //Trigger
  pinMode(D2, INPUT); //Echo
  Serial.begin(9600);
}
void loop() {
  digitalWrite(D1, LOW);
  delay(2 / 1000);
  digitalWrite(D1, HIGH);
  delay(10 / 1000);
  digitalWrite(D1, LOW);


  duration = pulseIn(D2, HIGH);
  cm = microsecondsToCentimeters(duration);
  Serial.print(cm);
  Serial.println("cm");
  delay(1000);
}

long microsecondsToCentimeters(long microseconds) {
  return (microseconds * .034) / 2;
}
