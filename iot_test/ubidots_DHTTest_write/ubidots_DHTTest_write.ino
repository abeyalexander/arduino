#include "UbidotsMicroESP8266.h"
#include "DHT.h"

#define TOKEN  "BBFF-cUZQOc2P5OQPZ56jLX5t6NpY5n4n5Q"  // Put here your Ubidots TOKEN
#define WIFISSID "ICT-KORATY-OFFICE" // Put here your Wi-Fi SSID
#define PASSWORD "Ict#Koraty$543" // Put here your Wi-Fi password

#define DHTPIN D2

#define DHTTYPE DHT11

DHT dht(DHTPIN, DHTTYPE);

Ubidots client(TOKEN);

void setup() {
  Serial.begin(115200);
  Serial.println(F("DHTxx test!"));

  dht.begin();
  client.wifiConnection(WIFISSID, PASSWORD);
}

void loop() {
  // Wait a few seconds between measurements.
  delay(2000);

  float h = dht.readHumidity();
  float t = dht.readTemperature();

  // Check if any reads failed and exit early (to try again).
  if (isnan(h) || isnan(t)) {
    Serial.println(F("Failed to read from DHT sensor!"));
    return;
  }
  
  client.add("Temperature", t);
  client.add("Humidity", h);
  client.sendAll(true);

  Serial.println();
  Serial.print(F("Humidity: "));
  Serial.print(h);
  Serial.print(F("%  Temperature: "));
  Serial.print(t);
  Serial.print(F("°C "));
  delay(1000);
}
