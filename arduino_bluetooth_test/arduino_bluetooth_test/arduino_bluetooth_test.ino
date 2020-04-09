#include <SoftwareSerial.h>
SoftwareSerial bluetooth_serial(4, 5);

#define ledPin 13
int state = 0;
long tick = 0;
void setup() {
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);
  Serial.begin(9600);
  bluetooth_serial.begin(9600); // Default communication rate of the Bluetooth module
}
void loop()
{
  if(millis() - tick >1000)
  {
    bluetooth_serial.println(millis());
    tick = millis();
  }
  if (bluetooth_serial.available() > 0) { // Checks whether data is comming from the serial port
    state = bluetooth_serial.read(); // Reads the data from the serial port
  }
  if (state == '0') {
    digitalWrite(ledPin, LOW); // Turn LED OFF
    Serial.println("LED: OFF"); // Send back, to the phone, the String "LED: ON"
    state = 0;
  }
  else if (state == '1') {
    digitalWrite(ledPin, HIGH);
    Serial.println("LED: ON");;
    state = 0;
  }
}
