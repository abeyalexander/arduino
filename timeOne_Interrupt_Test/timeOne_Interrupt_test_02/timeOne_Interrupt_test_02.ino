/*
 *  Timer1 library example
 *  June 2008 | jesse dot tane at gmail dot com
 */
 
#include "TimerOne.h"

long tick = 0, pin = 9, duty = 0;
void setup()
{
  Timer1.initialize();                     // initialize timer1, and set a duration in microseconds
  Timer1.pwm(pin, 128, 45);
  Serial.begin(115200);
}
 
void loop()
{
  float fade_freq = 0.1;
  float time_instant = 0;
  while(true)
  {
    time_instant = (float)micros()/1000000;
    float x = 0.5+0.48*sin(2*3.14*fade_freq*time_instant);
    float pwm_duty_ratio = x;
    int analog_write_value = 1023*(float)pwm_duty_ratio;
    Timer1.setPwmDuty(pin, analog_write_value);
    if (micros() - tick > 100000)
    {
      Serial.println(pulseInLong(4,HIGH));
      tick = micros();
    }
  }
}
