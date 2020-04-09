#include "TimerOne.h"
#include <Math.h>
#define pin 9

float time_per_unit = 0.0001; // PWM period in seconds
unsigned long time_per_unit_us = time_per_unit * 1000000;
unsigned long time_per_unit_ms = time_per_unit * 1000;
char message[] = "abcdefghijklmnopqrstuvwxyzzzz";
int i = 0, message_length;
int on_time, off_time;
volatile boolean flag = true;

void setup()
{
  pinMode(pin, OUTPUT);
  message_length = sizeof(message) - 4;
  Serial.begin(115200);
  //Timer1.initialize(time_per_unit_us);
  //Timer1.attachInterrupt(generate_pulse, time_per_unit_us);
}

void loop()
{
  float message_unit_normalized;
  String print_string;
  unsigned long tick;
  while (i < message_length)
  {
    message_unit_normalized = (float)(message[i % message_length] - 96) / 30;
    on_time = (float)message_unit_normalized * time_per_unit_us;
    digitalWrite(pin, HIGH);
    delayMicroseconds(on_time);
    digitalWrite(pin, LOW);
    
    print_string =  (String)i + ", " + (String)message_unit_normalized + ", " + (String)message[i % message_length];
    Serial.println(print_string);
    i++;
  }
}

void generate_pulse()
{
  if (!flag)
  {
    unsigned long tick = Timer1.read();
    digitalWrite(pin, HIGH);
    while (Timer1.read() - tick < on_time);
    digitalWrite(pin, LOW);
    flag = true;
  }
}

