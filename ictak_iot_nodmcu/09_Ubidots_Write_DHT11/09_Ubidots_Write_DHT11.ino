#include "UbidotsMicroESP8266.h"
#include "DHT.h"

#define DHTPIN D1     // Digital pin connected to the DHT sensor
#define DHTTYPE DHT11   // DHT 11

#define TOKEN  "BBFF-seb79vW5XCFUmhatBKB6gmSPxoaANE"  // Put here your Ubidots TOKEN
#define temperatureVariableID "5c6a7f521d847231bfa8444c" // Put your variable ID here
#define humidityVariableID "5c71086a1d84723fec62b9bd" // Put your variable ID here
#define locationVariableID "5d0dd2c21d8472636d8f4e75"

#define WIFISSID "POLLAYIL01" // Put here your Wi-Fi SSID
#define PASSWORD "october15" // Put here your Wi-Fi password

Ubidots client(TOKEN);

DHT dht(DHTPIN, DHTTYPE);

void setup()
{
  Serial.begin(115200);
  client.wifiConnection(WIFISSID, PASSWORD);
  Serial.println("DHT11 test!");
  dht.begin();
}
void loop()
{
  delay(2000);
  /*
    float h = dht.readHumidity();
    float t = dht.readTemperature();

    if (isnan(h) || isnan(t))
    {
    Serial.println(F("Failed to read from DHT sensor!"));
    return;
    }

    Serial.println("Humidity: " + String(h) + "%, Temperature: " + String(t) + "°C ");
  */
  char context[40];
  float _lat = 123.5667;
  float _lng = 79.0045;
  char context_string[40] = "lat=%.2f$lng=%.2f";
  //char context_string[40] = "\"context\":{\"lat\":51.5,\"lng\":-0.11}";
  sprintf(context, context_string, _lat, _lng);
  //client.add(humidityVariableID, h);
  //client.add(temperatureVariableID, t);
  client.add("position", 10, context);
  client.sendAll(true);
  delay(5000);
}
