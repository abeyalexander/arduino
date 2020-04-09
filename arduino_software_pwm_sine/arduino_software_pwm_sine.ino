#include "TimerOne.h"
#include <Math.h>
#define pin 9

float pwm_period = 0.00001; // PWM period in seconds
unsigned long pwm_period_us = pwm_period * 1000000;
unsigned long pwm_period_ms = pwm_period * 1000;

void setup()
{
  pinMode(pin, OUTPUT);
  Timer1.initialize(pwm_period_us);
  Timer1.pwm(pin, 512, pwm_period_us);
}

void loop()
{
  float fade_freq = 0.1;
  float time_instant = 0;
  long time_tick = 0;
  while (true)
  {
    time_instant = (float)millis() / 1000;
    float x = 0.5 + 0.5 * sin(2 * 3.14 * fade_freq * time_instant);
    Timer1.setPwmDuty(pin, x*1023);
  }
}


