#include <LiquidCrystal.h>
const int rs = 12, en = 11, d4 = 7, d5 = 6, d6 = 5, d7 = 4;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

#define no_of_buttons 2

int buttonPin[no_of_buttons];
const int ledPin = 13;

int reading[no_of_buttons], ledState = HIGH;
int buttonState[no_of_buttons], count = 0;
int lastButtonState[no_of_buttons];

unsigned long lastDebounceTime[no_of_buttons];
unsigned long debounceDelay = 50;

void setup()
{
  for (int i = 0; i < no_of_buttons; i++)
  {
    buttonPin[i] = i;
    pinMode(buttonPin[i], INPUT);
    lastButtonState[i] = LOW;
    lastDebounceTime[i] = 0;
  }

  pinMode(ledPin, OUTPUT);
  lcd.begin(16, 2);
  lcd.setCursor(0, 0);
  lcd.print("Count :");

  digitalWrite(ledPin, ledState);
}

void loop()
{
  for (int i = 0; i < no_of_buttons; i++)
  {
    reading[i] = digitalRead(buttonPin[i]) ;
    if (reading[i] != lastButtonState[i])
    {
      lastDebounceTime[i] = millis();
    }

    if ((millis() - lastDebounceTime[i]) > debounceDelay)
    {
      if (reading[i] != buttonState[i])
      {
        buttonState[i] = reading[i];
        if (buttonState[i] == HIGH)
        {
          ledState = !ledState;
          if (i % 2)
          {
            count++;
          }
          else
          {
            count--;
          }
        }
      }
    }
    lastButtonState[i] = reading[i];
    digitalWrite(ledPin, ledState);


    lcd.setCursor(7, 0);
    lcd.print(count);
    lcd.print("  ");
  }
}
