#include <LiquidCrystal.h>
#include "abraham_arduino_functions.h"

const int rs = 12, en = 11, d4 = 7, d5 = 6, d6 = 5, d7 = 4;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

const int inputPin = 2;
const int ledPin = 13;
boolean ledState = LOW;
int count = 0;

void setup()
{
  pinMode(ledPin, OUTPUT);
  pinMode(inputPin, INPUT);
  lcd.begin(16, 2);
  lcd.setCursor(0, 0);
  lcd.print("Count :");

  digitalWrite(ledPin, ledState);
}

void loop()
{
  digitalWrite(ledPin, ledState);
  lcd.setCursor(7, 0);
  lcd.print(count);
  lcd.print("  ");

  delay(1000);
  if(debounce(inputPin))
  {
    ledState = !ledState;
    count++;
  }

}
