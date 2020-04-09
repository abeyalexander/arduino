
#include <SoftwareSerial.h>

SoftwareSerial GPSSerial(4,5); //(RX, TX)


void setup() {
  // wait for hardware serial to appear
  while (!Serial);

  // make this baud rate fast enough to we aren't waiting on it
  Serial.begin(9600);

  // 9600 baud is the default rate for the Ultimate GPS
  GPSSerial.begin(9600);
}

     
void loop() 
{
  if (GPSSerial.available()) 
  {
    char c = GPSSerial.read();
    Serial.write(c);
  }
}
