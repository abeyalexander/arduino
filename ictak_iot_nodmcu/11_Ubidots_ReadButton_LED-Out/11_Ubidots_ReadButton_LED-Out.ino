#include "UbidotsMicroESP8266.h"

#define DEVICE  "home_automation"  // Put here your Ubidots device label
#define VARIABLE  "cloudSwitch"  // Put here your Ubidots variable label
#define TOKEN  "BBFF-CTRSpeSd93nRjEFCKZHRVbrMoVLfiB"  // Put here your Ubidots TOKEN
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
  int switchAtRemote = client.getValueWithDevice(DEVICE, VARIABLE);
  if (switchAtRemote != ERROR_VALUE)
  {
    Serial.print(F("value obtained -> Switch at Remote Location :"));
    Serial.println(switchAtRemote);
  }
  else
  {
    Serial.println(F("Error getting value"));
  }
  if (switchAtRemote > 0)
  {
    digitalWrite(ledPin, LOW);
  }
  else
  {
    digitalWrite(ledPin, HIGH);
  }
  delay(1000);
}
