#include <TimeLib.h>
#include <TinyGPS.h>
#include <SoftwareSerial.h>
#include "TimerOne.h"

#define rxPin 4
#define txPin 5

SoftwareSerial SerialGPS = SoftwareSerial(rxPin, txPin);
TinyGPS gps; 

// Offset hours from gps time (UTC)
const float offset = 5.5;   // Indian Standard Time (IST)

time_t prevDisplay = 0; // when the digital clock was displayed

int8_t answer;
int onModulePin = 2, i=0;
int spd = 10;
char aux_str[50];
String http_packet_string;
String timestamp_string, lat_string, lon_string;
time_t t_epoch;
float lat=10.7680700, lon=76.2737120;
String id = "426035";
char temp[100];
int packet_string_length=200;
long tick = 0;
int errorCount = 0;
char* getSerialInput();

void setup() 
{
  Serial.begin(9600);
  SerialGPS.begin(9600);
  //Timer1.initialize(100000);         // initialize timer1, and set a 100ms period
  //Timer1.attachInterrupt(timerCall);  // attaches callback() as a timer overflow interrupt
  Serial.println("Starting...");
  Serial.println("Waiting for GPS time ... ");
  initialize_gsm_gprs();
}
void(* resetFunc) (void) = 0;
void loop()
{
  setGPSTime();
  if (timeStatus()==timeSet)
  {
    t_epoch = now();
    if ((millis() - tick) >= 5000)
    {
      send_gprs_packet_to_traccar();
      tick = millis();
    }

  }
}

void setGPSTime()
{
  while (SerialGPS.available()) 
  {
    if (gps.encode(SerialGPS.read())) 
    {
      unsigned long age;
      int Year;
      byte Month, Day, Hour, Minute, Second;
      gps.crack_datetime(&Year, &Month, &Day, &Hour, &Minute, &Second, NULL, &age);
      if (age < 500) 
      {
        setTime(Hour, Minute, Second, Day, Month, Year);
        adjustTime(offset * SECS_PER_HOUR);
      }
    }
  }
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

void send_gprs_packet_to_traccar()
{
  String timestamp_string = (String)t_epoch;
  String lat_string = dtostrf(lat, 2, 7, temp);
  String lon_string = dtostrf(lon, 2, 7, temp); 
  String speed_string = String(spd);
  
  while (sendATcommand2("AT+HTTPINIT", "OK", "ERROR", 3000) != 1);
  while (sendATcommand2("AT+HTTPPARA=\"CID\",1", "OK", "ERROR", 3000) != 1);
  String http_packet_string  = "AT+HTTPPARA=\"URL\",\"http://io.adafruit.com/api/groups/weather/send.json?x-aio-key=938f74dec434480b8cd1138c9a49ba8b&timestamp=" + timestamp_string +"\"\"";
  //String http_packet_string  = "AT+HTTPPARA=\"URL\",\"http://demo5.traccar.org\"\"";
  packet_string_length = http_packet_string.length();
  char ip_data[200];
  memset(ip_data, 0, packet_string_length);
  http_packet_string.toCharArray(ip_data, packet_string_length); 
  while (sendATcommand2(ip_data, "OK", "ERROR", 3000) != 1);
  while (sendATcommand2("AT+HTTPACTION=0", "OK", "ERROR", 3000) != 1);
  delay(3000);
  Serial.println(getSerialInput());
  http_packet_string  = "AT+HTTPPARA=\"URL\",\"http://demo3.traccar.org:5055/?&id=" + id + "&timestamp=" + timestamp_string + "&lat=" + lat_string + "&lon=" + lon_string + "&speed=" + speed_string + "\"\"";
  packet_string_length = http_packet_string.length();
  memset(ip_data, 0, packet_string_length);
  http_packet_string.toCharArray(ip_data, packet_string_length); 
  while (sendATcommand2(ip_data, "OK", "ERROR", 3000) != 1);
  while (sendATcommand2("AT+HTTPACTION=0", "OK", "ERROR", 3000) != 1);
  delay(3000);
  Serial.println(getSerialInput());
  while (sendATcommand2("AT+HTTPTERM", "OK", "ERROR", 3000) != 1);
  lat += 0.001111;
  lon += 0.001111;
  spd += 1;
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

void digitalClockDisplay()
{
  // digital clock display of the time
  Serial.print(year()); 
  Serial.print("/");
  Serial.print(month());
  Serial.print("/");
  Serial.print(day());
  Serial.print(", ");
  Serial.print(hour());
  printDigits(minute());
  printDigits(second());
  Serial.println(); 
}

void printDigits(int digits) 
{
  // utility function for digital clock display: prints preceding colon and leading 0
  Serial.print(":");
  if(digits < 10)
    Serial.print('0');
  Serial.print(digits);
}

