/*
   ESP8266 NodeMCU Real Time Graphs Demo
   Updates and Gets data from webpage without page refresh
   https://circuits4you.com
*/
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include "DHT.h"

#include "index.h" //Our HTML webpage contents with javascripts

#define LED D4  //On board LED
#define DHTPIN D1
#define DHTTYPE DHT11

//SSID and Password of your WiFi router
const char* ssid = "JioFiAbraham";
const char* password = "0110401611";

ESP8266WebServer server(80); //Server on port 80
DHT dht(DHTPIN, DHTTYPE);
//===============================================================
// This routine is executed when you open its IP in browser
//===============================================================
void handleRoot() {
  String s = MAIN_page; //Read HTML contents
  server.send(200, "text/html", s); //Send web page
}

void handleADC() {
  int a = analogRead(A0);

  float h = dht.readHumidity();
  float t = dht.readTemperature();

  // Check if any reads failed and exit early (to try again).
  if (isnan(h) || isnan(t)) {
    Serial.println(F("Failed to read from DHT sensor!"));
    return;
  }
  String data = "{\"ADC\":\"" + String(a) + "\", \"Temperature\":\"" + String(t) + "\", \"Humidity\":\"" + String(h) + "\"}";
  digitalWrite(LED, !digitalRead(LED)); //Toggle LED on data request ajax
  server.send(200, "text/plane", data); //Send ADC value only to client ajax request
  Serial.println("Humidity: " + String(h) + "%, Temperature: " + String(t) + "°C ");
}
//==============================================================
//                  SETUP
//==============================================================
void setup(void) {
  Serial.begin(115200);
  dht.begin();

  WiFi.begin(ssid, password);     //Connect to your WiFi router
  Serial.println("");

  //Onboard LED port Direction output
  pinMode(LED, OUTPUT);

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  //If connection successful show IP address in serial monitor
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());  //IP address assigned to your ESP

  server.on("/", handleRoot);      //Which routine to handle at root location. This is display page
  server.on("/readADC", handleADC); //This page is called by java Script AJAX

  server.begin();                  //Start server
  Serial.println("HTTP server started");
}
//==============================================================
//                     LOOP
//==============================================================
void loop(void) {
  server.handleClient();          //Handle client requests
}
