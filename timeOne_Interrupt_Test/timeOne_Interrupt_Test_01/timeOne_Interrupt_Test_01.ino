/*
 *  Timer1 library example
 *  June 2008 | jesse dot tane at gmail dot com
 */
 
#include "TimerOne.h"
long time_value; 
void setup()
{
  Serial.begin(115200);
  Timer1.initialize(100000);         // initialize timer1, and set a 1 second period
  Timer1.attachInterrupt(callback);  // attaches callback() as a timer overflow interrupt
}
 
void callback()
{
  time_value = millis();
}
 
void loop()
{
  Serial.println(time_value);
}
