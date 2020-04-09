#include "UbidotsMicroESP8266.h"

#define DEVICE  "home_automation"  // Put here your Ubidots device label
#define VARIABLE  "temperature"  // Put here your Ubidots variable label
#define TOKEN  "BBFF-cUZQOc2P5OQPZ56jLX5t6NpY5n4n5Q"  // Put here your Ubidots TOKEN
#define WIFISSID "SSIDName" // Put here your Wi-Fi SSID
#define PASSWORD "password" // Put here your Wi-Fi password

#define ledPin D4

Ubidots client(TOKEN);

void setup()
{
  Serial.begin(115200);
  client.wifiConnection(WIFISSID, PASSWORD);
  pinMode(ledPin, OUTPUT);
}

void loop()
{
  float temperatureAtRemote = client.getValueWithDevice(DEVICE, VARIABLE);
  if (temperatureAtRemote != ERROR_VALUE)
  {
    Serial.print(F("value obtained -> Temperature at Remote Location :"));
    Serial.println(temperatureAtRemote);
  }
  else
  {
    Serial.println(F("Error getting value"));
  }
  if (temperatureAtRemote > 27)
  {
    digitalWrite(ledPin, LOW);
  }
  else
  {
    digitalWrite(ledPin, HIGH);
  }
  delay(1000);
}
