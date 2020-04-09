#include "ThingSpeak.h"
#include "secrets.h"
#include <ESP8266WiFi.h>

#define ledPin D4

char ssid[] = SECRET_SSID;   // your network SSID (name) 
char pass[] = SECRET_PASS;   // your network password

WiFiClient  client;

// My channel details
unsigned long myChannelNumber = SECRET_CH_ID;
const char * myReadAPIKey = SECRET_READ_APIKEY;
unsigned int myFieldNumber = 1;

void setup() 
{
  Serial.begin(115200);  // Initialize serial

  WiFi.mode(WIFI_STA); 
  ThingSpeak.begin(client);  // Initialize ThingSpeak

  pinMode(ledPin, OUTPUT);
}

void loop() 
{
 
  // Connect or reconnect to WiFi
  if(WiFi.status() != WL_CONNECTED)
  {
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(SECRET_SSID);
    while(WiFi.status() != WL_CONNECTED)
    {
      WiFi.begin(ssid, pass);
      Serial.print(".");
      delay(5000);     
    } 
    Serial.println("\nConnected");
  }

  int remoteDistance = ThingSpeak.readIntField(myChannelNumber, myFieldNumber, myReadAPIKey);  

  int statusCode = ThingSpeak.getLastReadStatus();
  
  if(statusCode == 200)
  {
    Serial.println("Distance measured at remote location : " + String(remoteDistance));
  }
  else
  {
    Serial.println("Problem reading channel. HTTP error code " + String(statusCode)); 
  }
  
  if (remoteDistance < 30)
  {
    digitalWrite(ledPin, HIGH);
  }

  else
  {
    digitalWrite(ledPin, LOW);
  }
    
  delay(15000);
}
