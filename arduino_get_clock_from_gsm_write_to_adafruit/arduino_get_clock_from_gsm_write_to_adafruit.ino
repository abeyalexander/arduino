int8_t answer;
int onModulePin = 2, i = 0;
char aux_str[50];
String http_packet_string;
String timestamp_string;
char temp[100];
int packet_string_length = 200;
long tick = 0;
int errorCount = 0;
char* getSerialInput();

void setup()
{
  Serial.begin(9600);
  Serial.println("Starting...");
  initialize_gsm_gprs();
}
void(* resetFunc) (void) = 0;

void loop()
{
  if ((millis() - tick) >= 5000)
  {
    send_gprs_packets();
    tick = millis();
  }
}

String getGSMTime()
{
  int i=0;
  char read_data[30];
  Serial.println("ATE0");
  delay(2000);
  while (Serial.available())
  {
    Serial.read();
  }
  Serial.println("AT+CCLK?");
  delay(2000);
  while (Serial.available())
  {
    read_data[i] = Serial.read();
    i++;
  }
  String rx_data = (String)read_data;
  Serial.println(rx_data);
  rx_data = extract_clock_from_gsm_response(rx_data);
  return rx_data;
}


void initialize_gsm_gprs()
{
  power_on();
  Serial.println("Connecting to the GSM network...");
  while (sendATcommand2("AT+CREG?", "+CREG: 0,1", "+CREG: 0,5", 1000) == 0 );
  while (sendATcommand2("AT+HTTPTERM", "OK", "ERROR", 3000) != 2);
  while (sendATcommand2("AT+SAPBR=3,1,\"CONTYPE\",\"GPRS\"", "OK", "ERROR", 3000) != 1);
  while (sendATcommand2("AT+SAPBR=3,1,\"APN\",\"airtelgprs.com\"", "OK", "ERROR", 3000) != 1);
  while (sendATcommand2("AT+SAPBR=3,1,\"USER\",\"\"", "OK",  "ERROR", 3000) != 1);
  while (sendATcommand2("AT+SAPBR=3,1,\"PWD\",\"\"", "OK",  "ERROR", 3000) != 1);
  sendATcommand2("AT+SAPBR=1,1", "OK",  "ERROR", 3000);
  delay(2000);
}

void send_gprs_packets()
{
  String lat_string = "11.7671786";
  String lon_string = "76.2727124";
  String id = "426035";
  String speed_string = "10";
  String timestamp_string = getGSMTime();
  
  sendATcommand2("AT+HTTPINIT", "OK", "ERROR", 3000);
  delay(200);
  sendATcommand2("AT+HTTPPARA=\"CID\",1", "OK", "ERROR", 3000);
  delay(200);
  //String http_packet_string = "AT+HTTPPARA=\"URL\",\"http://io.adafruit.com/api/groups/weather/send.json?x-aio-key=938f74dec434480b8cd1138c9a49ba8b&timestamp=" + timestamp_string + "\"\"";
  String http_packet_string = "AT+HTTPPARA=\"URL\",\"http://demo3.traccar.org:5055/?&id=" + id + "&timestamp=" + timestamp_string + "&lat=" + lat_string + "&lon=" + lon_string + "&speed=" + speed_string + "\"\"";
  packet_string_length = http_packet_string.length();
  char ip_data[200];
  memset(ip_data, 0, packet_string_length);
  http_packet_string.toCharArray(ip_data, packet_string_length);
  sendATcommand2(ip_data, "OK", "ERROR", 3000);
  delay(1000);
  sendATcommand2("AT+HTTPACTION=0", "OK", "ERROR", 3000);
  delay(3000);
  Serial.println(getSerialInput());
  sendATcommand2("AT+HTTPTERM", "OK", "ERROR", 3000);
  delay(1000);
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
    { // Send AT every two seconds and wait for the answer
      answer = sendATcommand2("AT", "OK", "OK", 2000);
    }
  }

}

char* getSerialInput()
{
  char response[200];
  memset (response, 0, packet_string_length);
  int x = 0;
  while (Serial.available() > 0)
  {
    response[x] = Serial.read();
    x++;
  }
  return response;
}

int8_t sendATcommand2(char* ATcommand, char* expected_answer1, char* expected_answer2, unsigned int timeout)
{

  uint8_t x = 0,  answer = 0;
  char response[200];
  unsigned long previous;

  memset(response, 0, packet_string_length);    // Initialize the string

  delay(500);

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

String extract_clock_from_gsm_response(String serial_string)
{
  String Year = "20" + serial_string.substring(10, 12);
  String Month = serial_string.substring(13, 15);
  String Day = serial_string.substring(16, 18);
  String Hour = serial_string.substring(19, 21);
  String Minute = serial_string.substring(22, 24);
  String Second = serial_string.substring(25, 27);
  String data_to_cloud = Year+Month+Day+Hour+Minute+Second;
  return data_to_cloud;
}
