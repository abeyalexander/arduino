#include <Math.h>
#define pin 9


void setup() 
{
  pinMode(pin, OUTPUT);
}

void loop() 
{
  float duty_ratio = 0.5; // normalised value
  float pwm_period = 0.00001; // PWM period in seconds
  float pwm_period_in_us = pwm_period * 1000000;
  float on_time = duty_ratio * pwm_period_in_us; 
  float off_time = (1-duty_ratio) * pwm_period_in_us;
  int pin_no = pin;
  
  while(true)
  {
    
    write_square_wave_to_pin_us(pin, on_time, off_time);
  }
}

void write_square_wave_to_pin_us(int pin_no, long on_time, long off_time)
{ 
  digitalWrite(pin_no, HIGH);
  delayMicroseconds(on_time);
  digitalWrite(pin_no, LOW);
  delayMicroseconds(off_time);
}

