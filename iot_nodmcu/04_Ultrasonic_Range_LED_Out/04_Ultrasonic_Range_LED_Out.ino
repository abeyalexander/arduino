#define triggerPin D1
#define echoPin D2
#define ledPin D4

#include "UbidotsMicroESP8266.h"

#define TOKEN  "BBFF-wKJfRUIRfv5A8ebD3r0SBKA9tK5JlY"  // Put here your Ubidots TOKEN
#define WIFISSID "hareesh" // Put here your Wi-Fi SSID
#define PASSWORD "hareesha" // Put here your Wi-Fi password

Ubidots client(TOKEN);


void setup() 
{
  Serial.begin(115200);
  pinMode(triggerPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(ledPin, OUTPUT);
  client.wifiConnection(WIFISSID, PASSWORD);
}

void loop() 
{
  long duration;
  int cm;

  digitalWrite(triggerPin, LOW);
  delayMicroseconds(2);
  digitalWrite(triggerPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(triggerPin, LOW);

  duration = pulseIn(echoPin, HIGH);

  centimetre = microsecondsToCentimeters(duration);

  Serial.println(String(centimetre) + " cm");
  client.add("distance", centimetre);
  client.sendAll(true);
  delay (2000);
}

int microsecondsToCentimeters(long microseconds) 
{
  return microseconds / 29 / 2;
}
