#include <EEPROM.h>

int address = 0;
byte value;

void setup() 
{
  Serial.begin(9600);
}

void loop() 
{
  Serial.println((String)address+": "+ (String)value);
}


boolean eeprom_write(int address, int data)
{
	int rd_data;
	address = address%EEPROM.length();
	EEPROM.write(address, data);
	EEPROM.read(address, rd_data);
	if (rd_data != data)
		return true;
	else
		return false;
}


void eeprom_read(int address, int data)
{
	address = address%EEPROM.length(); 
	EEPROM.read(address, rd_data);
	return data;
}


