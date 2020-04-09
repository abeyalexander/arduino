#include <LiquidCrystal.h>

const int rs = 12, en = 11, d4 = 6, d5 = 7, d6 = 8, d7 = 9;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

const byte LCD_ROWS = 2; //two rows
const byte LCD_COLS = 16; //16 columns

void setup()
{
  Serial.begin(9600);
  lcd.begin(LCD_COLS,LCD_ROWS); // cols, rows
}

void loop()
{ 
  byte row, col;
  row = 0;
  while(!Serial.available());
  lcd.clear();
  while(row < LCD_ROWS)
  {
    col = 0;
    lcd.setCursor(col,row);
    while(col < LCD_COLS)
    {
      while(!Serial.available());
      lcd.print((char)Serial.read());
      col++;
    }
    row++;   
  }
  delay(500);  
}

