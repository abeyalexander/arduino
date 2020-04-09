#include <ESP8266WiFi.h>
#include <PubSubClient.h>
 
const char* ssid = "AKASH VAIO";
const char* password =  "9645960937";
const char* mqttServer = "192.168.137.75";
const int mqttPort = 1883;
 
WiFiClient espClient;
PubSubClient client(espClient);
 
void setup() {
  pinMode(D4, OUTPUT);
  pinMode(D0, OUTPUT);
  pinMode(D1, OUTPUT);
  digitalWrite(D4, HIGH);
  
  Serial.begin(115200);
 
  WiFi.begin(ssid, password);
 
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println("Connecting to WiFi..");
  }
  Serial.println("Connected to the WiFi network");
 
  client.setServer(mqttServer, mqttPort);
  client.setCallback(callback);
 
  while (!client.connected()) {
    Serial.println("Connecting to MQTT...");
 
    if (client.connect("ESP8266Client" )) {
 
      Serial.println("connected");  
 
    } else {
 
      Serial.print("failed with state ");
      Serial.print(client.state());
      delay(2000);
 
    }
  }

  client.subscribe("TEST");
 
}
 
void callback(char* topic, byte* payload, unsigned int length) {
 
  Serial.print("Message arrived in topic: ");
  Serial.println(topic);
 
  Serial.print("Message:");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
      if ((char)payload[i]=='1')
      {
        digitalWrite(D4, LOW);        
        digitalWrite(D0, HIGH);
        digitalWrite(D1, HIGH);
        
        
      }
      else if ((char)payload[i]==' ')
      {
        digitalWrite(D4, HIGH);        
        digitalWrite(D0, LOW);
        digitalWrite(D1, LOW);
        
      }
      else
      {
        digitalWrite(D4, HIGH);        
        digitalWrite(D0, LOW);
        digitalWrite(D1, LOW);
        
      }
  }

 
  Serial.println();
  Serial.println("-----------------------");
 
}
 
void loop() {
  client.loop();
}
