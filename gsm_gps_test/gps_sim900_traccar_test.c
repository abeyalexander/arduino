#include <TimeLib.h>
#include <TinyGPS.h>
#include <SoftwareSerial.h>

#define rxPin 4
#define txPin 5

SoftwareSerial SerialGPS = SoftwareSerial(rxPin, txPin);
TinyGPS gps; 

// Offset hours from gps time (UTC)
const float offset = 5.5;   // Indian Standard Time (IST)

time_t prevDisplay = 0; // when the digital clock was displayed

int8_t answer;
int onModulePin = 2, i=0;
char aux_str[50];
String http_packet_string;
String id = "416035";
String timestamp_string, lat_string, lon_string;
long t_epoch;
float lat = 10.76707;
float lon = 76.27271;
char ip_data[300];
char temp[300];
void setup() 
{
	Serial.begin(9600);
	SerialGPS.begin(9600);
	pinMode(onModulePin, OUTPUT);
	Serial.println("Starting...");
	Serial.println("Waiting for GPS time ... ");
	power_on();
	delay(3000);
	Serial.println("Connecting to the GSM network...");
	while (sendATcommand2("AT+CREG?", "+CREG: 0,1", "+CREG: 0,5", 1000) == 0 );
	while (sendATcommand2("AT+HTTPTERM", "OK", "ERROR", 3000) != 2);
	while (sendATcommand2("AT+SAPBR=3,1,\"CONTYPE\",\"GPRS\"", "OK", "ERROR", 3000) != 1);
	while (sendATcommand2("AT+SAPBR=3,1,\"APN\",\"airtelgprs.com\"", "OK", "ERROR", 3000) != 1);
	while (sendATcommand2("AT+SAPBR=3,1,\"USER\",\"\"", "OK",  "ERROR", 3000) != 1);
	while (sendATcommand2("AT+SAPBR=3,1,\"PWD\",\"\"", "OK",  "ERROR", 3000) != 1);
	delay(3000);
}

void loop()
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
	if (timeStatus()!= timeNotSet) 
	{
		if (now() != prevDisplay)	//update the display only if the time has changed
		{ 
			prevDisplay = now();
			digitalClockDisplay();  
		}
	}
  
	t_epoch = now();
	timestamp_string = dtostrf(t_epoch, 10, 0, temp);
	lat_string = dtostrf(lat, 2, 7, temp);
	lon_string = dtostrf(lon, 2, 7, temp);
	http_packet_string = "AT+HTTPPARA=\"URL\",\"http://demo5.traccar.org:5055/?&id=" + id + "&timestamp=" + timestamp_string + "&lat=" + lat_string + "&lon=" + lon_string + "\"\"";
	http_packet_string.toCharArray(ip_data, http_packet_string.length());

	while (sendATcommand2("AT+HTTPINIT", "OK", "ERROR", 3000) != 1);
	while (sendATcommand2("AT+HTTPPARA=\"CID\",1", "OK", "ERROR", 3000) != 1);
	while (sendATcommand2(ip_data, "OK", "ERROR", 3000) != 1);
	while (sendATcommand2("AT+HTTPACTION=0", "OK", "ERROR", 3000) != 1);
	delay(5000);
	while (sendATcommand2("AT+HTTPTERM", "OK", "ERROR", 3000) != 1);
	
	lat += 0.00100;
	lon += 0.00100;
	delay(3000);
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

int8_t sendATcommand2(char* ATcommand, char* expected_answer1, char* expected_answer2, unsigned int timeout)
{

  uint8_t x = 0,  answer = 0;
  char response[200];
  unsigned long previous;

  memset(response, 0, 200);    // Initialize the string

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
