#include <ESP8266WiFi.h>
#include <PubSubClient.h>

const char* ssid = "AKASH VAIO";
const char* password =  "9645960937";
const char* mqttServer = "192.168.137.66";
const int mqttPort = 1883;
boolean sub_data_flag = false;
long t1 = 0;

WiFiClient espClient;
PubSubClient client(espClient);

void setup() {

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

  client.publish("TEST", "Hello from RELAY_NODE");
  client.subscribe("TEST");

}

void callback(char* topic, byte* payload, unsigned int length) {

  Serial.print("Message arrived in topic: ");
  Serial.println(topic);

  Serial.print("Message:");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
    if ((char)payload[i] == '1')
    {
      sub_data_flag = true;
    }
  }


  Serial.println();
  Serial.println("-----------------------");

}

void loop() {
  client.loop();
  if (sub_data_flag)
  {
    digitalWrite(13, HIGH);
    t1 = millis();
    sub_data_flag = false;
  }
  else if (digitalRead(13) && (millis() - t1 > 2000))
  {
    digitalWrite(13, LOW);
  }
}
