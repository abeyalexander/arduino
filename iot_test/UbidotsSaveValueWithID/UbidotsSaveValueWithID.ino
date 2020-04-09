#include "UbidotsMicroESP8266.h"

#define TOKEN  "BBFF-cUZQOc2P5OQPZ56jLX5t6NpY5n4n5Q"  // Put here your Ubidots TOKEN
#define ID_1 "5c6a7f521d847231bfa8444c" // Put your variable ID here
#define ID_2 "5c6a82a61d847233a6c20d00" // Put your variable ID here
#define WIFISSID "Pollayil" // Put here your Wi-Fi SSID
#define PASSWORD "64bcf6ba" // Put here your Wi-Fi password

Ubidots client(TOKEN);
float value1 = 0, value2 = 0;
float n = 0;

void setup(){
    Serial.begin(115200);
    client.wifiConnection(WIFISSID, PASSWORD);
    //client.setDebug(true); // Uncomment this line to set DEBUG on
}

void loop(){ 
    value1 = sin(6.28*n);
    value2 = sin(12.56*n);
    n += 0.01;
    //float value2 = analogRead(2)
    client.add(ID_1, value1);
    client.add(ID_2, value2);
    client.sendAll(false);
    delay(1000);
}
