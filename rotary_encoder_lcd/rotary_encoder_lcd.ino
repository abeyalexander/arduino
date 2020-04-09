#include <LiquidCrystal.h>


int pin = 13;
int count_array[10];
float count_average;
volatile boolean state = LOW;
volatile int count = 0;
volatile boolean rotate_r, state_r = LOW;
volatile boolean rotate_l, state_l = LOW;
volatile boolean increment_flag = LOW, decrement_flag = LOW;
int debounce_duration = 0; //Microseconds;
const int rs = 12, en = 11, d4 = 7, d5 = 6, d6 = 5, d7 = 4;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);


void setup() 
{
  pinMode(pin, OUTPUT);
  pinMode(2,INPUT);
  pinMode(3,INPUT);
  Serial.begin(9600);
    // set up the lcd's number of columns and rows:
    lcd.begin(16, 2);
    lcd.setCursor(0, 0);
    lcd.print("Count :");

    attachInterrupt(digitalPinToInterrupt(2), isr_1, RISING);
    attachInterrupt(digitalPinToInterrupt(3), isr_2, RISING);

}

void loop() 
{

  digitalWrite(pin, state);
  if (increment_flag)
  {
    increment();
    increment_flag = LOW;
  }

  else if (decrement_flag)
  {
    decrement();
    decrement_flag = LOW;
  }


  
    lcd.setCursor(7, 0);
    lcd.print(count);
    lcd.print(",");
    lcd.print(count_average);
    lcd.print("  ");
    attachInterrupt(digitalPinToInterrupt(2), isr_1, CHANGE);
    attachInterrupt(digitalPinToInterrupt(3), isr_2, CHANGE);  
}

float average(int *numbers_array)
{
  int sum=0;
  float average=0;
  int numbers_array_length = sizeof(numbers_array);
  for (int i=0; i<numbers_array_length; i++)
  {
    sum = sum + numbers_array[i];
  }
  average = sum/numbers_array_length;
  return average;
}

void isr_1()
{
  detachInterrupt(digitalPinToInterrupt(2));
  //detachInterrupt(digitalPinToInterrupt(3));
  increment_flag = HIGH;
}

void isr_2()
{
  detachInterrupt(digitalPinToInterrupt(3));
  //detachInterrupt(digitalPinToInterrupt(2));
  decrement_flag = HIGH;
}

void increment() 
{
  delayMicroseconds(debounce_duration);
  state_r = digitalRead(2);

  if(!rotate_r)
  {
    rotate_r = HIGH;
  }
  else if(digitalRead(3)==state_r)
  {
    count += 1;
    state = state_r;
    rotate_r = LOW;
  }
  else
  {
    rotate_r = LOW;
  }

}

void decrement() 
{
  delayMicroseconds(debounce_duration);
  state_l = digitalRead(3);
  if(!rotate_l)
  {
    rotate_l = HIGH;
  }
  else if(digitalRead(2)==state_l)
  {
    count -= 1;
    state = state_l;
    rotate_l = LOW;
  }
  else
  {
    rotate_l = LOW;
  }
}
