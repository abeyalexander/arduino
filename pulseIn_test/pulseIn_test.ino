#include <TimerOne.h>

#define out_pin 10
#define in_pin 9

unsigned long x, tick = 0;
byte data_byte;

void setup()
{
  pinMode(in_pin, INPUT);
  pinMode(out_pin, OUTPUT);
  Serial.begin(115200);
  Timer1.initialize();
  Timer1.pwm(out_pin, 0, 2000); 
  Timer1.attachInterrupt(timer_routine);
}

void timer_routine()
{
  data_byte += 1;
  Timer1.setPwmDuty(out_pin, data_byte*4);
  if(millis() - tick > 1000)
  {
    tick = millis();
    Serial.println(x);
  }
}

void loop()
{
  x = pulseInLong(in_pin, HIGH, 2000);
}
