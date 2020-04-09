#include <LiquidCrystal.h>
const int rs = 12, en = 11, d4 = 7, d5 = 6, d6 = 5, d7 = 4;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

const int buttonPin = 2;
const int ledPin = 13;

int ledState = HIGH;
int buttonState, count=0;
int lastButtonState = LOW;

unsigned long lastDebounceTime = 0;
unsigned long debounceDelay = 50;

void setup() 
{
  pinMode(buttonPin, INPUT);
  pinMode(ledPin, OUTPUT);
  
  lcd.begin(16, 2);
  lcd.setCursor(0, 0);
  lcd.print("Count :");

  digitalWrite(ledPin, ledState);
}

void loop() 
{
  int reading = digitalRead(buttonPin);
  if (reading != lastButtonState) 
  {
    lastDebounceTime = millis();
  }

  if ((millis() - lastDebounceTime) > debounceDelay) 
  {
    if (reading != buttonState) 
    {
      buttonState = reading;
      if (buttonState == HIGH) 
      {
        ledState = !ledState;
        count++;
      }
    }
  }
  digitalWrite(ledPin, ledState);
  lastButtonState = reading;

  lcd.setCursor(7, 0);
  lcd.print(count);
}
