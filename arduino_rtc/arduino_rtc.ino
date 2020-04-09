
// CONNECTIONS:
// DS1302 CLK/SCLK --> 6
// DS1302 DAT/IO --> 7
// DS1302 RST/CE --> 8
// DS1302 VCC --> 3.3v - 5v
// DS1302 GND --> GND

#include <ThreeWire.h>
#include <RtcDS1302.h>

ThreeWire myWire(7, 6, 8); // IO, SCLK, CE
RtcDS1302<ThreeWire> Rtc(myWire);

#define lenArray(a) (sizeof(a) / sizeof(a[0]))

void setup ()
{
  Serial.begin(9600);
  Rtc.Begin();
  Rtc.SetDateTime(RtcDateTime(2020, 4, 8, 11, 20, 0));

}

void loop ()
{
  RtcDateTime now = Rtc.GetDateTime();

  printDateTime(now);
  Serial.println();
  delay(1000); // ten seconds
}

void printDateTime(const RtcDateTime& dt)
{
  char datestring[20];

  snprintf_P(datestring,
             lenArray(datestring),
             PSTR("%02u/%02u/%04u %02u:%02u:%02u"),
             dt.Day(),
             dt.Month(),
             dt.Year(),
             dt.Hour(),
             dt.Minute(),
             dt.Second() );
  Serial.print(datestring);
}
