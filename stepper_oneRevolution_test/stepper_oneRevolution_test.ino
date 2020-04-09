
/*
 Stepper Motor Control - one revolution

 This program drives a unipolar or bipolar stepper motor.
 The motor is attached to digital pins 8 - 11 of the Arduino.

 The motor should revolve one revolution in one direction, then
 one revolution in the other direction.


 Created 11 Mar. 2007
 Modified 30 Nov. 2009
 by Tom Igoe

 */

#include <Stepper.h>

const int stepsPerRevolution = 2048;  // change this to fit the number of steps per revolution
// for your motor

// initialize the stepper library on pins 8 through 11:
Stepper Stepper_1(stepsPerRevolution, 7, 5, 6, 4);
Stepper Stepper_2(stepsPerRevolution, 11, 9, 10, 8);

void setup() {
  // set the rpm:
  Stepper_1.setSpeed(4);
  Stepper_2.setSpeed(4);
  // initialize the serial port:
  Serial.begin(115200);
}

void loop() 
{
  rotate(180);
  delay(1000);
  rotate(-180);
  delay(1000);
}

void rotate(int angle_to_rotate)
{
  float steps_for_angle = 5.688888889 * abs(angle_to_rotate);
  int step_count = 0;
  
  String print_string = "Steps for angle : " + (String)steps_for_angle;
  Serial.println(print_string);
  
  if (angle_to_rotate > 0)
  {
    while (step_count < steps_for_angle)
    {
      Stepper_1.step(1);
      Stepper_2.step(1);
      step_count++;
    }
  }
  else
  {
    while (step_count < steps_for_angle)
    {
      Stepper_1.step(-1);
      Stepper_2.step(-1);
      step_count++;
    }
  }
}

