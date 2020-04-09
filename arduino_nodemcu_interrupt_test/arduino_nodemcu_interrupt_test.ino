#include <Bounce2.h> // Used for "debouncing" the pushbutton

uint8_t GPIO_Pin = D2;
byte state = HIGH;
// Initialise the Pushbutton Bouncer object
Bounce bouncer = Bounce();
  
void setup() {
 Serial.begin(2000000);
 pinMode(D4, OUTPUT);
 attachInterrupt(digitalPinToInterrupt(GPIO_Pin), IntCallback, RISING);
   // Setup pushbutton Bouncer object
  bouncer.attach(GPIO_Pin);
  bouncer.interval(5);
}

void loop() {
  Serial.println("Loop Running");
  if(bouncer.rose())
  {
    Serial.print("In Loop Button Rose @ Stamp(ms): ");
    Serial.println(millis());
  }
  digitalWrite(D4, state);
  delay(1000);
  state = !state;
}

void IntCallback(){
  // Update button state
  // This needs to be called so that the Bouncer object can check if the button has been pressed
    bouncer.update();
        Serial.print("Button Rose @ Stamp(ms): ");
    Serial.println(millis());
}
