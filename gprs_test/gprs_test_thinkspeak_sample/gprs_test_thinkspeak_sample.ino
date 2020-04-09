
void setup()
{
  Serial.begin(9600); /* Define baud rate for serial communication */
}

void loop() {
  Serial.println("TCP Send :");
  Serial.print("AT\\r\\n");
  SIM900.println("AT"); /* Check Communication */
  delay(5000);
  ShowSerialData(); /* Print response on the serial monitor */
  delay(5000);
  Serial.print("AT+CIPMODE=0\\r\\n");
  SIM900.println("AT+CIPMODE=0"); /* Non-Transparent (normal) mode for TCP/IP application */
  delay(5000);
  ShowSerialData();
  delay(5000);
  Serial.print("AT+CIPMUX=0\\r\\n");
  SIM900.println("AT+CIPMUX=0");  /* Single TCP/IP connection mode */
  delay(5000);
  ShowSerialData();
  delay(5000);
  Serial.print("AT+CGATT=1\\r\\n");
  SIM900.println("AT+CGATT=1"); /* Attach to GPRS Service */
  delay(5000);
  ShowSerialData();
  delay(5000);
  Serial.print("AT+CREG?\\r\\n");
  SIM900.println("AT+CREG?"); /* Network registration status */
  delay(5000);
  ShowSerialData();
  delay(5000);
  Serial.print("AT+CGATT?\\r\\n");
  SIM900.println("AT+CGATT?");  /* Attached to or detached from GPRS service */
  delay(5000);
  ShowSerialData();
  delay(5000);
  Serial.print("AT+CSTT=\"TATA.DOCOMO.INTERNET\",\"\",\"\"\\r\\n");
  SIM900.println("AT+CSTT=\"TATA.DOCOMO.INTERNET\",\"\",\"\""); /* Start task and set APN */
  delay(5000);
  ShowSerialData();
  delay(5000);
  Serial.print("AT+CIICR\\r\\n");
  SIM900.println("AT+CIICR"); /* Bring up wireless connection with GPRS */
  delay(5000);
  ShowSerialData();
  delay(5000);
  Serial.print("AT+CIFSR\\r\\n");
  SIM900.println("AT+CIFSR"); /* Get local IP address */
  delay(5000);
  ShowSerialData();
  delay(5000);
  Serial.print("AT+CIPSTART=\"TCP\",\"api.thingspeak.com\",\"80\"\\r\\n");
  SIM900.println("AT+CIPSTART=\"TCP\",\"api.thingspeak.com\",\"80\"");  /* Start up TCP connection */
  delay(5000);
  ShowSerialData();
  delay(5000);
  Serial.print("AT+CIPSEND\\r\\n");
  SIM900.println("AT+CIPSEND"); /* Send data through TCP connection */
  delay(2000);
  ShowSerialData();
  delay(2000);
  Serial.print("GET /update?api_key=C7JFHZY54GLCJY38&field1=1\\r\\n");
  delay(10000);
  ShowSerialData();
  delay(5000);
  Serial.print("AT+CIPSHUT\\r\\n");
  delay(5000);
  ShowSerialData();
  delay(5000);
}

void ShowSerialData()
{
  while (Serial.available() != 0) /* If data is available on serial port */
    Serial.write(char (Serial.read())); /* Print character received on to the serial monitor */
}
