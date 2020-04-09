#include <Math.h>
#define pin 9

#define no_of_tones 1        //  number of tones generated per sequence
#define min_tone_freq 100     //  in Hertz
#define max_tone_freq 100    //  in Hertz
#define min_tone_duration 5000 //  minimum duration of a single tone in milliseconds
#define max_tone_duration 5000 //  maximum duration of a single tone in milliseconds

void setup() 
{
  pinMode(pin, OUTPUT);
}

void loop() 
{
  float freq;
  float t_period;
  long t_p_us;
  long tick_1=0, tick_2=0;
  int y=0;
  int z[no_of_tones];
  int i=0;
  randomSeed(analogRead(A0));
  int t_tone = random(min_tone_duration, max_tone_duration);;
  int t_tone_sequence = t_tone*no_of_tones;
  for (y=0; y<no_of_tones; y++)
  {
    z[y] = random(min_tone_freq, min_tone_freq);
  }
  
  freq = random(min_tone_freq, min_tone_freq);
  t_period = 1/freq;
  t_p_us = t_period*500000;
  
  while(true)
  {
    //freq = 1000 + 4000*sin(x);
    if (millis() - tick_1 > t_tone)
    {
      freq = z[(i%no_of_tones)]; 
      i++;
      tick_1 = millis();
      t_period = 1/freq;
      t_p_us = t_period*500000;
    }
    if (millis() - tick_2 > t_tone_sequence)
    {
      for (y=0; y<no_of_tones; y++)
      {
        z[y] = random(min_tone_freq, max_tone_freq);
      }
      tick_2 = millis();
      t_tone = random(min_tone_duration, max_tone_duration);
      t_tone_sequence = t_tone*no_of_tones;
    }
    
    write_square_wave_to_pin_us(pin, t_p_us);
  }
}

void write_square_wave_to_pin_us(int pin_no, float delay_time_us)
{
  digitalWrite(pin_no, HIGH);
  delayMicroseconds(delay_time_us);
  digitalWrite(pin_no, LOW);
  delayMicroseconds(delay_time_us);
}

