/* Demonstration sketch for PCF8574T I2C LCD Backpack 
Uses library from https://bitbucket.org/fmalpartida/new-liquidcrystal/downloads GNU General Public License, version 3 (GPL-3.0) */
#include <Wire.h>
#include <LCD.h>
#include <LiquidCrystal_I2C.h>

//LiquidCrystal_I2C  lcd(0x27,2,1,0,4,5,6,7); // 0x27 is the I2C bus address for an PCF8574T I2C LCD Backpack
LiquidCrystal_I2C  lcd(0x3F,2,1,0,4,5,6,7); // 0x3F is the I2C bus address for an PCF8574A I2C LCD Backpack

void setup()
{
  // activate LCD module
  lcd.begin (16,2); // for 16 x 2 LCD module
  lcd.setBacklightPin(3,POSITIVE);
  lcd.setBacklight(HIGH);
    // set the cursor to (0,0):
  lcd.setCursor(0, 0);
    // print from 0 to 9:
  for (int thisChar = 0; thisChar < 10; thisChar++) 
  {
    lcd.print(thisChar);
    delay(500);
  }
}

void loop()
{
  lcd.scrollDisplayRight();
  delay(500);
/*
  // set the cursor to (16,1):
  lcd.setCursor(8, 1);
  // set the display to automatically scroll:
  lcd.autoscroll();
  // print from 0 to 9:
  for (int thisChar = 0; thisChar < 10; thisChar++) {
    lcd.print("1");
    delay(500);
  }
  // turn off automatic scrolling
  lcd.noAutoscroll();

  // clear screen for the next loop:
  lcd.clear();
    */
}
