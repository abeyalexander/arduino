/*
    Timer1 library example
    June 2008 | jesse dot tane at gmail dot com
*/

#include "TimerOne.h"

int tx_pwm_period = 2000; // milliseconds 
long tick = 0, pin = 9, duty = 0;
byte data_byte, i;
char message[5] = {'H', 'e', 'l', 'l', 'o'};
void setup()
{
  Timer1.initialize();
  Timer1.pwm(pin, 0, tx_pwm_period); // output pin, duty_cycle (0 to 1023), time period (milliseconds) 
}

void loop()
{
  //if (millis() - tick > tx_pwm_period)
  //{
    //data_byte = message[i%5];
    Timer1.setPwmDuty(pin, data_byte*4);
    //Serial.println(pulseInLong(4, HIGH));
    i++;
    //tick = millis();
  //}
}

int byte_to_pwm_duty(byte data)
{
  return (data * 4);
}
