#include <AccelStepper.h>

//AccelStepper Xaxis(1, 2, 5); // pin 2 = step, pin 5 = direction
//AccelStepper Yaxis(1, 3, 6); // pin 3 = step, pin 6 = direction
//AccelStepper Zaxis(1, 4, 7); // pin 4 = step, pin 7 = direction

AccelStepper stepper_01(4, 8, 9, 10, 11);

void setup() 
{
  stepper_01.setMaxSpeed(4096);
  stepper_01.setSpeed(100);
}

void loop() 
{  
   stepper_01.runSpeed();
}
