#include "SPI.h"
#include "Adafruit_GFX.h"
#include "Adafruit_ILI9341.h"

// For the Adafruit shield, these are the default.
#define TFT_DC 9
#define TFT_CS 10

// Use hardware SPI (on Uno, #13, #12, #11) and the above for CS/DC
Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC);

#include <Keypad_I2C.h>
#include <Keypad.h>          // GDY120705
#include <Wire.h>

#define I2CADDR 0x20

const byte ROWS = 4;
const byte COLS = 4;
char keys[ROWS][COLS] = {
  {'7', '8', '9', '/'},
  {'4', '5', '6', 'x'},
  {'1', '2', '3', '-'},
  {'C', '0', '=', '+'}
};
byte rowPins[ROWS] = {0, 1, 2, 3}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {4, 5, 6, 7}; //connect to the column pinouts of the keypad

Keypad_I2C keypad = Keypad_I2C( makeKeymap(keys), rowPins, colPins, ROWS, COLS, I2CADDR );
byte ledPin = 13;

boolean blink = false;

void setup() {
  Serial.begin(1000000);
  Wire.begin( );
  keypad.begin( );          // GDY120705
  pinMode(ledPin, OUTPUT);      // sets the digital pin as output
  digitalWrite(ledPin, HIGH);   // sets the LED on
  keypad.addEventListener(keypadEvent); //add an event listener for this keypad
  tft.begin();
  tft.setTextColor(ILI9341_WHITE);
  tft.setTextSize(2);
  tft.fillScreen(ILI9341_BLACK);
}

void loop() {
  char key = keypad.getKey();

  if (key) {
    Serial.println(key);
    tft.fillScreen(ILI9341_BLACK);
    tft.setCursor(0, 0);
    tft.println("Key: " + String(key));
  }
  if (blink) {
    digitalWrite(ledPin, !digitalRead(ledPin));
    delay(100);
  }
}

//take care of some special events
void keypadEvent(KeypadEvent key) {
  switch (keypad.getState()) {
    case PRESSED:
      switch (key) {
        case '8':
          digitalWrite(ledPin, !digitalRead(ledPin));
          break;
        case '9':
          digitalWrite(ledPin, !digitalRead(ledPin));
          break;
      }
      break;
    case RELEASED:
      switch (key) {
        case '8':
          digitalWrite(ledPin, !digitalRead(ledPin));
          blink = false;
          break;
      }
      break;
    case HOLD:
      switch (key) {
        case '8':
          blink = true;
          break;
      }
      break;
  }
}
