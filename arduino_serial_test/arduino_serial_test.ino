void setup() 
{
  Serial.begin(9600);

}

void loop() 
{
  char read_data, clock_string[100];
  int i = 0;
  String data = "";
  Serial.println("ATE0");
  delay(200);
  while(Serial.available())
  {
    read_data = Serial.read();
    data += (String)read_data;
  }
  Serial.println(data);
  delay(2000);

}
