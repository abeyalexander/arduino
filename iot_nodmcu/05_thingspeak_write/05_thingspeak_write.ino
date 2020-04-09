#include "ThingSpeak.h"
#include "secrets.h"
#include <ESP8266WiFi.h>

char ssid[] = SECRET_SSID;   // your network SSID (name)
char pass[] = SECRET_PASS;   // your network password

WiFiClient  client;

unsigned long myChannelNumber = SECRET_CH_ID;
const char * myWriteAPIKey = SECRET_WRITE_APIKEY;
unsigned int myFieldNumber = 1;

int testData = 0;

void setup() {
  Serial.begin(115200);  // Initialize serial

  WiFi.mode(WIFI_STA);
  ThingSpeak.begin(client);  // Initialize ThingSpeak
}

void loop() 
{
  // Connect or reconnect to WiFi
  if (WiFi.status() != WL_CONNECTED) 
  {
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(SECRET_SSID);
    
    while (WiFi.status() != WL_CONNECTED) 
    {
      WiFi.begin(ssid, pass);  // Connect to WPA/WPA2 network. Change this line if using open or WEP network
      Serial.print(".");
      delay(5000);
    }
    Serial.println("\nConnected.");
  }

  // Write to ThingSpeak. There are up to 8 fields in a channel, allowing you to store up to 8 different
  // pieces of information in a channel.  Here, we write to field 1.
  int httpConnectionStatusCode = ThingSpeak.writeField(myChannelNumber, myFieldNumber, testData, myWriteAPIKey);

  if (httpConnectionStatusCode == 200) 
  {
    Serial.println("Channel update successful.");
  }
  
  else 
  {
    Serial.println("Problem updating channel. HTTP error code " + String(httpConnectionStatusCode));
  }

  testData++;
  if (testData > 99)
  {
    testData = 0;
  }

  delay(20000); // Wait 20 seconds to update the channel again
}
