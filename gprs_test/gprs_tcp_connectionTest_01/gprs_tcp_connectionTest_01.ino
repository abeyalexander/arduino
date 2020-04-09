int8_t answer;
int onModulePin = 2;
char aux_str[50];
String http_packet_string;
String id = "416035";
String timestamp_string, lat_string, lon_string;
long timestamp = 1507718063;
float lat = 10.7670786;
float lon = 76.2727124;
char ip_data[127];
char temp[127];
void setup() {

  pinMode(onModulePin, OUTPUT);
  Serial.begin(9600);
  Serial.println("Starting...");
  power_on();
  delay(3000);
  Serial.println("Connecting to the network...");
  while (sendATcommand2("AT+CREG?", "+CREG: 0,1", "+CREG: 0,5", 1000) == 0 );
}

void loop()
{

  timestamp += 10;
  lat += 0.0000010;
  lon += 0.0000010;
  timestamp_string = dtostrf(timestamp, 10, 0, temp);
  lat_string = dtostrf(lat, 2, 7, temp);
  lon_string = dtostrf(lon, 2, 7, temp);
  http_packet_string = "AT+HTTPPARA=\"URL\",\"http://demo5.traccar.org:5055/?&id=" + id + "&timestamp=" + timestamp_string + "&lat=" + lat_string + "&lon=" + lon_string + "\"";
  http_packet_string.toCharArray(ip_data, http_packet_string.length());

  while (sendATcommand2("AT+HTTPTERM", "OK", "ERROR", 3000) != 2);
  while (sendATcommand2("AT+SAPBR=3,1,\"CONTYPE\",\"GPRS\"", "OK", "ERROR", 3000) != 1);
  while (sendATcommand2("AT+SAPBR=3,1,\"APN\",\"airtelgprs.com\"", "OK", "ERROR", 3000) != 1);
  while (sendATcommand2("AT+SAPBR=3,1,\"USER\",\"\"", "OK",  "ERROR", 3000) != 1);
  while (sendATcommand2("AT+SAPBR=3,1,\"PWD\",\"\"", "OK",  "ERROR", 3000) != 1);
  while (sendATcommand2("AT+HTTPINIT", "OK", "ERROR", 3000) != 1);
  while (sendATcommand2("AT+HTTPPARA=\"CID\",1", "OK", "ERROR", 3000) != 1);
  while (sendATcommand2(ip_data, "OK", "ERROR", 5000) != 1);
  while (sendATcommand2("AT+HTTPACTION=0", "OK", "ERROR", 3000) != 1)
  while (sendATcommand2("AT+HTTPTERM", "OK", "ERROR", 3000) != 1);
}




void power_on()
{

  uint8_t answer = 0;

  // checks if the module is started
  answer = sendATcommand2("AT", "OK", "OK", 2000);
  if (answer == 0)
  {
    // power on pulse
    digitalWrite(onModulePin, HIGH);
    delay(3000);
    digitalWrite(onModulePin, LOW);

    // waits for an answer from the module
    while (answer == 0) 
    {   // Send AT every two seconds and wait for the answer
      answer = sendATcommand2("AT", "OK", "OK", 2000);
    }
  }

}

int8_t sendATcommand2(char* ATcommand, char* expected_answer1, char* expected_answer2, unsigned int timeout)
{

  uint8_t x = 0,  answer = 0;
  char response[100];
  unsigned long previous;

  memset(response, 0, 100);    // Initialize the string

  delay(100);

  while ( Serial.available() > 0) Serial.read();   // Clean the input buffer

  Serial.println(ATcommand);    // Send the AT command
  x = 0;
  previous = millis();

  // this loop waits for the answer
  do {
    // if there are data in the UART input buffer, reads it and checks for the answer
    if (Serial.available() != 0)
    {
      response[x] = Serial.read();
      x++;
      // check if the desired answer 1  is in the response of the module
      if (strstr(response, expected_answer1) != NULL)
      {
        answer = 1;
      }
      // check if the desired answer 2 is in the response of the module
      else if (strstr(response, expected_answer2) != NULL)
      {
        answer = 2;
      }
    }
  }
  // Waits for the asnwer with time out
  while ((answer == 0) && ((millis() - previous) < timeout));

  return answer;
}
