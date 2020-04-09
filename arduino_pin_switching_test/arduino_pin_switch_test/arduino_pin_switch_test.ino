#define pin 9

void setup() 
{
  pinMode(pin, OUTPUT);
}
int z[5] = {100, 200, 300, 400, 500};
int i = 0;

void loop() 
{
  float freq = 100;
  float t_period = 1/freq;
  long t_p_us = t_period*500000;
  long tick=0;
  float x=0;
  while(true)
  {
    freq = 1000 + z[i%5]*sin(x);
    t_period = 1/freq;
    t_p_us = t_period*500000;
    write_square_wave_to_pin_us(pin, t_p_us);
    x = x+0.01; 
    if (millis() - tick >2000)
    {
      i++;  
      tick = millis();
    }
  }
}

void write_square_wave_to_pin_us(int pin_no, float delay_time_us)
{
  digitalWrite(pin_no, HIGH);
  delayMicroseconds(delay_time_us);
  digitalWrite(pin_no, LOW);
  delayMicroseconds(delay_time_us);
}

