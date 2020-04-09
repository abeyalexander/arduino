#define vt Serial
unsigned int i = 0, j = 0;
String data[] = {"AT+CCLK?"};
void setup()
{
  Serial.begin(9600);
  Serial.println("ATE0");
  delay(200);
  Serial.println("AT+CCLK?");
  delay(200);
  Serial.println("AT+CCLK?");
}

void loop()
{

  char read_data[100];
  j = 0;
  for (; j < (sizeof(data) / sizeof(String));)
  {
    vt.print("TX: ");
    Serial.println(data[j]);
    i = 0;
    delay(2000);
    while (Serial.available())
    {
      read_data[i] = Serial.read();
      i++;
    }
    String rx_data = (String)read_data;
    vt.print("RX: ");
    vt.print (read_data);
    String Year = "20" + rx_data.substring(10, 12);
    String Month = rx_data.substring(13, 15);
    String Day = rx_data.substring(16, 18);
    String Hour = rx_data.substring(19, 21);
    String Minute = rx_data.substring(22, 24);
    String Second = rx_data.substring(25, 27);
    vt.print(Year);
    vt.print("/");
    vt.print(Month);
    vt.print("/");
    vt.print(Day);
    vt.print(" ");
    vt.print(Hour);
    vt.print(":");
    vt.print(Minute);
    vt.print(":");
    vt.println(Second);
    j++;
    delay(1000);
  }
}

