#include <TimerOne.h>

byte sensor_type_select_pin = 9;
boolean sensor_type = false;

void setup() 
{
  sensor_type = digitalRead(sensor_type_select_pin);
  
  if(!sensor_type)
  {
    attachInterrupt(digitalPinToInterrupt(0),isr_halleffect, RISING);
  }

}

void loop() 
{
  
}


void isr_halleffect()
{
  count++;
}


