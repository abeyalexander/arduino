#include "UbidotsMicroESP8266.h"

#define ledPin D4
#define DEVICE  "807d3a663e55"  // Put here your Ubidots device label
#define VARIABLE  "humidity"  // Put here your Ubidots variable label
#define TOKEN  "BBFF-cUZQOc2P5OQPZ56jLX5t6NpY5n4n5Q"  // Put here your Ubidots TOKEN
#define WIFISSID "ICT-KORATY-OFFICE" // Put here your Wi-Fi SSID
#define PASSWORD "Ict#Koraty$543" // Put here your Wi-Fi password

Ubidots client(TOKEN);

void setup() {
    Serial.begin(115200);
    client.wifiConnection(WIFISSID, PASSWORD);
    pinMode(LED_BUILTIN, OUTPUT);
    digitalWrite(LED_BUILTIN, 1);
    //client.setDebug(true); // Uncomment this line to set DEBUG on
}

void loop() {
    float value = client.getValueWithDevice(DEVICE, VARIABLE);
    if (value != ERROR_VALUE){
      Serial.print(F("value obtained: "));
      Serial.println(value);
    }else{
      Serial.println(F("Error getting value"));
    }

    if (value < 30)
    {
      digitalWrite(ledPin, HIGH);
    }
    else
    {
      digitalWrite(ledPin, LOW);
    }
    delay(1000);
}
