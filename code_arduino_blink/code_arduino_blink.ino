int pin = 13;
volatile int state = LOW;
volatile int count = 0;
void setup() 
{
    pinMode(pin, OUTPUT);
    Serial.begin(9600);
    attachInterrupt(digitalPinToInterrupt(3), increment, CHANGE);
    attachInterrupt(digitalPinToInterrupt(2), decrement, CHANGE);
}

void loop() 
{
    digitalWrite(pin, state);
    Serial.println("Count = %d");
    Serial.print(count);
    Serial.println("");
}

void increment() 
{
    state = !state;
    count += 1;
}

void decrement() 
{
    state = !state;
    count -= 1;
}
