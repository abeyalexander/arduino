#include <TimerOne.h>

#include "TimerOne.h"
#define pin 2

unsigned long width_value;
unsigned long time_period_in_us = 100000000; // 100 milliSeconds
boolean flag = false;
void setup()
{
  pinMode(pin, INPUT);
  Serial.begin(2000000);
  attachInterrupt(digitalPinToInterrupt(pin), isr, CHANGE);
  Timer1.initialize(time_period_in_us);
}

void loop()
{
  if (flag)
  {
    Serial.println(width_value);
    flag = false;
  }
}

void isr()
{
  if (digitalRead(pin))
  {
    Timer1.restart();
  }
  else
  {
    width_value = Timer1.read();
    flag = true;
  }
}


