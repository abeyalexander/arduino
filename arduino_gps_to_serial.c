#include <TimeLib.h>
#include <TinyGPS.h>
#include <SoftwareSerial.h>

#define rxPin_GPS 4
#define txPin_GPS 5

SoftwareSerial SerialGPS = SoftwareSerial(rxPin_GPS, txPin_GPS);
TinyGPS gps;

// Offset hours from gps time (UTC)
const float offset = 5.5;   // Indian Standard Time (IST)

float latitude, longitude;
unsigned long fix_age, time, date, speed, course, tick;
int Year;
byte Month, Day, Hour, Minute, Second;

void setup()
{
  Serial.begin(9600);
  SerialGPS.begin(9600);
  Serial.println("Starting...");
  Serial.println("Waiting for GPS time ... ");
}

void loop()
{    
  setDetails_from_GPS();
  if (timeStatus()==timeSet)
  {
    if ((millis() - tick) > 1000)
    {
      String print_string = "Latitiude = " + (String)latitude + ", Longitude = " + (String)longitude + ", Time = " + now();
      Serial.println(print_string);
      tick = millis();
    }
  }
}

void setDetails_from_GPS()
{
  while (SerialGPS.available())
  {
    if (gps.encode(SerialGPS.read()))
    {
      unsigned long chars;
      unsigned short sentences, failed_checksum;

      gps.f_get_position(&latitude, &longitude, &fix_age);

      // time in hhmmsscc, date in ddmmyy
      gps.get_datetime(&date, &time, &fix_age);

      // returns speed in 100ths of a knot
      speed = gps.speed();

      // course in 100ths of a degree
      course = gps.course();

      gps.crack_datetime(&Year, &Month, &Day, &Hour, &Minute, &Second, NULL, &fix_age);
      if (fix_age < 500)
      {
        adjustTime(offset * SECS_PER_HOUR);
        setTime(Hour, Minute, Second, Day, Month, Year);
      }
    }
  }
}
