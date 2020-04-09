#include "TimerOne.h"
#include "Math.h"
#define pin 2

unsigned long width_value;
boolean flag = false;
float time_per_unit = 0.0001; // in seconds
unsigned long time_per_unit_us = time_per_unit*1000000;
byte i = 0;
float temp[26];
char message_char[26];
void setup() 
{
  pinMode(pin, INPUT);
  Serial.begin(115200);
  attachInterrupt(digitalPinToInterrupt(pin), isr, CHANGE);
  Timer1.initialize(time_per_unit_us);
}

void loop() 
{
  if (flag)
  {
    float message = (float)width_value / (time_per_unit_us); 
    message_char[i%26] = (message * 30) + 96;
    temp[i%26] = message; 
    if (i==29)
    {
      for (int j=0; j<26; j++)
      {
        Serial.println((String)message_char[j] + ", " + (String)temp[j]);
      }
      i = -1; 
    }
    i++;
    flag = false;
  }
}

void isr()
{
  if(!digitalRead(pin))
  {
    width_value = Timer1.read();
    flag = true;
  }
  else
  {
    Timer1.restart();
  }
}

