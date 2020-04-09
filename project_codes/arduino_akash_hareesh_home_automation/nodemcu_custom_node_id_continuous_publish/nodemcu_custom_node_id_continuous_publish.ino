#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <EEPROM.h>

int eeprom_address = 0;
byte eeprom_data;

#define read_pin D5
#define sense_pin D1
#define relay_pin_1 D0
#define relay_pin_2 D3
#define BUILTIN_LED D4


// Update these with values suitable for your network.

//const char* ssid = "hacker";
//const char* password = "sreejamadhu";
const char* ssid = "asus_a007";
const char* password = "012345678910";
const char* mqtt_server = "abraham-VirtualBox";
String node_id = "null";
String default_node_id_string = "9,9";
long tick_1 = 0, tick_2 = 0;
boolean relay_state = false, publish_flag = false, sensor_state = false, current_node_flag_state = false, dest_node_flag_state = false, next_node_flag_state = false;
int max_grid_coordinate = 9;

WiFiClient espClient;
PubSubClient client(espClient);

long lastMsg = 0;
char msg[4];
int node_grid_number = 0;
String msg_received = "";
boolean sensor_state_current = 0, sensor_state_prev = 0;
long max_wait_for_node_id;
int blink_delay = 50;
boolean blink_state = false;

void setup() {
  pinMode(BUILTIN_LED, OUTPUT);     // Initialize the BUILTIN_LED pin as an output
  pinMode(sense_pin, INPUT);
  pinMode(read_pin, INPUT);
  pinMode(relay_pin_1, OUTPUT);
  pinMode(relay_pin_2, OUTPUT);
  Serial.begin(115200);
  EEPROM.begin(512);
  setup_wifi();
  node_id = get_node_id();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
}

String get_node_id_from_eeprom_backup()
{
  String node_id_string = "";
  Serial.println("Node ID selection key press not detected!!!, Checking EEPROM for Node ID backup...");
  int node_id_backup_status = eeprom_read(0);
  if (node_id_backup_status == 170)
  {
    node_id_string  = (String)eeprom_read(1) + (String)eeprom_read(2);
    Serial.println("EEPROM Backup available... Node ID is set to backed up value: " + node_id_string);
  }
  else
  {
    node_id_string = default_node_id_string;
    Serial.println("Node ID backup not available, setting Node ID to default value: " + node_id_string);
  }
  return node_id_string;
}

String get_node_id()
{
  boolean node_id_set_flag = true;
  int node_id_x_coordinate = get_node_id_coordinate(read_pin, max_grid_coordinate);
  if (node_id_x_coordinate < 0)
  {
    return get_node_id_from_eeprom_backup();
  }
  Serial.println("Got X coordinate: " + (String)node_id_x_coordinate);
  Serial.println("Press again to set Y coordinate...");
  delay_with_second_blink(3000);
  int node_id_y_coordinate = get_node_id_coordinate(read_pin, max_grid_coordinate);
  if (node_id_y_coordinate < 0)
  {
    return get_node_id_from_eeprom_backup();
  }
  Serial.println("Got X and Y coordinates!!! :" + (String)node_id_x_coordinate + ", " + (String)node_id_y_coordinate);
  String node_id = (String)node_id_x_coordinate + "," + (String)node_id_y_coordinate;

  Serial.println("Writing Node ID to EEPROM...");

  if (eeprom_write(1, node_id_x_coordinate) & eeprom_write(2, node_id_y_coordinate) & eeprom_write(0, 170))
  {
    Serial.println("Node ID: " + (String)node_id + " written to EEPROM, Success!!!");
  }
  else
    Serial.println("EEPROM write failed!!!");

  return node_id;
}

void setup_wifi() {

  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void delay_with_second_blink(long delay_ms)
{
  long tick_1 = millis();
  long tick_2 = millis();
  while (millis() - tick_2 < delay_ms)
  {
    if (millis() - tick_1 > 1000)
    {
      digitalWrite(BUILTIN_LED, LOW);
      delay(blink_delay);
      digitalWrite(BUILTIN_LED, HIGH);
      Serial.println(".");
      tick_1 = millis();
    }
  }
}

int get_node_id_coordinate(int read_pin, int max_grid_coordinate)
{
  int node_id_coordinate_count = -1;
  tick_1 = millis();
  tick_2 = millis();
  if (digitalRead(read_pin))
  {
    for (int i = 0; i < max_grid_coordinate; i++)
    {
      if (digitalRead(read_pin))
      {
        node_id_coordinate_count = i;
        digitalWrite(BUILTIN_LED, LOW);
        delay(blink_delay);
        digitalWrite(BUILTIN_LED, HIGH);
        Serial.println("Blink");
        Serial.println("Read Pin: High, node_id_coordinate_count: " + (String)node_id_coordinate_count);
        delay(1000);
      }
    }
  }
  return node_id_coordinate_count;
}


/*
  String get_node_id()
  {

  int node_id_r_count = -1;
  int node_id_c_count = -1;
  int node_id_count = -1;
  String node_id_string;
  tick_1 = millis();
  tick_2 = millis();
  if (digitalRead(read_pin))
  {
    while (digitalRead(read_pin) && (node_id_r_count <= max_number_grid_rows))
    {
      if (millis() - tick_1 > 1000)
      {
        node_id_r_count += 1;
        digitalWrite(BUILTIN_LED, LOW);
        delay(blink_delay);
        digitalWrite(BUILTIN_LED, HIGH);
        Serial.println("Blink");
        Serial.println("Read Pin: High " + (String)node_id_r_count + ", " + (String)node_id_c_count);
        tick_1 = millis();
      }
    }
  }
  Serial.println("Setting Node ID row value to:" + (String)node_id_r_count);
  Serial.println("Press key for setting Node ID column value...");
  tick_2 = millis();
  tick_1 = millis();
  while (millis() - tick_2 > 3000)
  {
    if (millis() - tick_1 > 1000)
    {
      digitalWrite(BUILTIN_LED, LOW);
      delay(blink_delay);
      digitalWrite(BUILTIN_LED, HIGH);
      tick_1 = millis();
    }
  }
  tick_2 = millis();
  tick_1 = millis();
  if (digitalRead(read_pin))
  {
    while (digitalRead(read_pin) && (node_id_c_count <= max_number_grid_columns))
    {
      if (millis() - tick_1 > 1000)
      {
        node_id_c_count += 1;
        digitalWrite(BUILTIN_LED, LOW);
        delay(blink_delay);
        digitalWrite(BUILTIN_LED, HIGH);
        Serial.println("Blink");
        Serial.println("Read Pin: High " + (String)node_id_r_count + ", " + (String)node_id_c_count);
        tick_1 = millis();
      }
    }
    if (node_id_c_count > max_number_grid_columns)
    {
      node_id = (String)(max_number_nodes - 1);
      Serial.println("Node ID selection key press reached max delay!!!, setting Node ID to default value :" + (String)node_id);
    }
    else if (node_id_count > -1)
    {
      node_id_c_count = (String)node_id_c_count;
      Serial.println("Setting Node ID:" + (String)node_id);
      Serial.println("Writing to EEPROM...");
      if (eeprom_write(1, node_id.toInt()))
      {
        if (eeprom_write(0, 170))
          Serial.println("Node ID: " + (String)node_id + " written to EEPROM, Success!!!");
      }
      else
        Serial.println("EEPROM write failed!!!");
    }
  }
  else
  {
    Serial.println("Node ID selection key press not detected!!!, Checking EEPROM for Node ID backup...");
    int node_id_backup_status = eeprom_read(0);
    if (node_id_backup_status == 170)
    {
      node_id = eeprom_read(1);
      Serial.println("EEPROM Backup available... Node ID is set to backed up value: " + (String)node_id);
    }
    else
    {
      node_id = (String)(max_number_nodes - 1);
      Serial.println("Node ID backup not available, setting Node ID to default value: " + (String)node_id);
    }
  }

  }
*/

boolean set_message_flag(String topic, String message)
{
  if (msg_received == (String)node_id)
  {
    return true;
  }
  else
  {
    return false;
  }
}

void callback(char* topic, byte * payload, unsigned int length) {
  msg_received = "";
  for (int i = 0; i < length; i++)
  {
    msg_received = msg_received + (char)payload[i];
  }
  Serial.println("Message [" + (String)topic + "] received: " + (String)msg_received);

  if ((String)topic == "next")
  {
    next_node_flag_state = set_message_flag((String)topic, msg_received);
  }
  else if ((String)topic == "dest")
  {
    dest_node_flag_state = set_message_flag((String)topic, msg_received);
  }
  else if ((String)topic == "current")
  {
    current_node_flag_state = set_message_flag((String)topic, msg_received);
  }
}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected())
  {
    String temp = "client_id_" + (String)node_id;
    char client_id[20];
    temp.toCharArray(client_id, 20);

    if (client.connect(client_id))
    {
      Serial.println("mqtt Connected!!!, Client ID:" + (String)client_id);
      client.subscribe("current");
      client.subscribe("dest");
      client.subscribe("next");
      Serial.println("Subscribed to Topics: \"current\", \"next\", \"dest\"");
    }
    else
    {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}
void loop()
{
  if (!client.connected())
  {
    reconnect();
  }
  client.loop();
  if (millis() - tick_1 > 1000)
  {
    digitalWrite(BUILTIN_LED, LOW);
    delay(blink_delay);
    digitalWrite(BUILTIN_LED, HIGH);
    sensor_state = digitalRead(sense_pin);

    if (sensor_state)
    {
      if (millis() - tick_2 > 3500)
      {
        publish_flag = true;
        tick_2 = millis();
      }
      blink_state = true;
    }

    relay_state = sensor_state | current_node_flag_state | next_node_flag_state | dest_node_flag_state;
    digitalWrite(relay_pin_1, !relay_state);
    digitalWrite(relay_pin_2, !relay_state);
    digitalWrite(BUILTIN_LED, blink_state);
    blink_state = !blink_state;
    Serial.println(blink_state);
    tick_1 = millis();
  }

  if (publish_flag)
  {
    char message_char_array[10];
    node_id.toCharArray(message_char_array, 10);
    client.publish("outTopic", message_char_array);
    Serial.println("");
    Serial.print("Message published!!!: " + (String)message_char_array);
    Serial.println("");
    publish_flag = false;
  }
}

boolean eeprom_write(int address, int data)
{
  int rd_data;
  EEPROM.write(address, data);
  EEPROM.commit();
  rd_data = EEPROM.read(address);
  if (rd_data != data)
    return false;
  else
    return true;
}


int eeprom_read(int address)
{
  int rd_data = EEPROM.read(address);
  return rd_data;
}



