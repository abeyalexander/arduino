#include <EEPROM.h>

int address = 0;
byte value;

void setup()
{
  Serial.begin(9600);
  randomSeed(analogRead(0));
}

void loop()
{
  /*
  for (int i = 0; i < 10; i++)
  {
    int data = random(100, 200);
    Serial.println("EEPROM writing @"+(String)i+": "+data);
    if(!eeprom_write(i, data))
    {
      Serial.println("EEPROM Data Write Verification Error!!!");
    }
    delay(500);
  }
  */
  for (int i = 0; i < 10; i++)
  {
    Serial.println("EEPROM Reading @"+(String)i + ": " + eeprom_read(i));
    delay(500);
  }
}


boolean eeprom_write(int address, int data)
{
  int rd_data;
  address = address % EEPROM.length();
  EEPROM.write(address, data);
  rd_data = EEPROM.read(address);
  if (rd_data != data)
    return false;
  else
    return true;
}


int eeprom_read(int address)
{
  address = address % EEPROM.length();
  int rd_data = EEPROM.read(address);
  return rd_data;
}



