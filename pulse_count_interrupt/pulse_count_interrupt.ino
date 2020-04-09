#include "TimerOne.h"
#define interrupt_pin 2

unsigned long pulse_interval, tick;
double rpm;
boolean flag = false;
float time_per_unit = 1; // in seconds
unsigned long time_per_unit_us = time_per_unit * 1000000;
void setup()
{
  Serial.begin(115200);
  attachInterrupt(digitalPinToInterrupt(interrupt_pin), isr, RISING);
  Timer1.initialize(time_per_unit_us);
  pinMode(9, OUTPUT);
  digitalWrite(9, LOW);
}

void loop()
{
  rpm = 2500000/pulse_interval ;
  if (millis() - tick > 500)
  {
    Serial.println("RPM: "+(String)rpm);
    tick = millis();
  }
}

void isr()
{
  pulse_interval = Timer1.read();
  Timer1.restart();
}

