#include <Math.h>

int led_pin_1 = 9;
int led_pin_2 = 10;

void setup() 
{
  pinMode(led_pin_1, OUTPUT);
  pinMode(led_pin_2, OUTPUT);
  TCCR1B = (TCCR1B & 0xF8) | 1; // Set PWM Frequency of pin 9, 10 to low frequency (Timer 1).
}


void loop() 
{
  float fade_speed = 0;
  float fade_freq = 1;
  float time_instant = 0;
  long time_tick = 0;
  while(true)
  {
    time_instant = (float)millis()/1000;
    float x = 0.5+0.5*sin(2*3.14*fade_freq*time_instant);
    float pwm_duty_ratio = x;
    int analog_write_value = 255*pwm_duty_ratio;
    analogWrite(led_pin_1, analog_write_value);
    analogWrite(led_pin_2, 255-analog_write_value);
//    if (millis() - time_tick > 1000)
//    {
//      Serial.print(time_instant);
//      Serial.print(", ");
//      Serial.println(x);
//      time_tick = millis();
//    }
  }
}

