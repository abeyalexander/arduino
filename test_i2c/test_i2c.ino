/* @file EventSerialKeypad.pde
|| @version 1.0
|| @author Alexander Brevig
|| @contact alexanderbrevig@gmail.com
||
|| @description
|| | Demonstrates using the KeypadEvent.
|| #

   Modified to use I2C i/o - G. D. (Joe) Young Feb 28/12
*/
#include <Keypad_I2C.h>
#include <Keypad.h>          // GDY120705
#include <Wire.h>

#define I2CADDR 0x20

const byte ROWS = 4; //four rows
const byte COLS = 4; //three columns
char keys[ROWS][COLS] = {
	{'7','8','9','/'},
	{'4','5','6', 'x'},
	{'1','2','3', '-'},
	{'C','0','=', '+'}
	};
byte rowPins[ROWS] = {0, 1, 2, 3}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {4, 5, 6, 7}; //connect to the column pinouts of the keypad
	
Keypad_I2C keypad = Keypad_I2C( makeKeymap(keys), rowPins, colPins, ROWS, COLS, I2CADDR );
byte ledPin = 13; 

boolean blink = false;

void setup(){
  Serial.begin(1000000);
  while( !Serial ){/*wait*/}
  Wire.begin( );
  keypad.begin( );          // GDY120705
  pinMode(ledPin, OUTPUT);      // sets the digital pin as output
  digitalWrite(ledPin, HIGH);   // sets the LED on
  keypad.addEventListener(keypadEvent); //add an event listener for this keypad
}
  
void loop(){
  char key = keypad.getKey();
  
  if (key) {
    Serial.println(key);
  }
  if (blink){
    digitalWrite(ledPin,!digitalRead(ledPin));
    delay(100);
  }
}

//take care of some special events
void keypadEvent(KeypadEvent key){
  switch (keypad.getState()){
    case PRESSED:
      switch (key){
        case '8': digitalWrite(ledPin,!digitalRead(ledPin)); break;
        case '9': 
          digitalWrite(ledPin,!digitalRead(ledPin));
        break;
      }
    break;
    case RELEASED:
      switch (key){
        case '8': 
          digitalWrite(ledPin,!digitalRead(ledPin));
          blink = false;
        break;
      }
    break;
    case HOLD:
      switch (key){
        case '8': blink = true; break;
      }
    break;
  }
}
