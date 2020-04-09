#define no_of_stepper_motors 2

byte stepper_pins[no_of_stepper_motors][4] =
{
  {8, 9, 10, 11},
  {4, 5, 6, 7}
};

int current_Step = 0;
boolean Direction = true;
int step_count = 0, angle, steps_for_angle;

void setup()
{
  for (int i = 0; i < no_of_stepper_motors; i++)
  {
    for (int j = 0; j < 4; j++)
    {
      pinMode(stepper_pins[i][j], OUTPUT);
    }
  }
}

void loop()
{
  rotate_stepper_angles(1, 10);
  rotate_stepper_angles(2, -10);
  delay(1000);
}

void stepper_motor_off(byte stepper_motor_number)
{
  for (int i = 0; i < 4; i++)
  {
    digitalWrite(stepper_pins[stepper_motor_number][i], LOW);
  }
}

void rotate_stepper_angles(byte stepper_motor_number, int angle_to_rotate)
{
  float steps_for_angle = 11.37777778 * abs(angle_to_rotate);
  int step_count = 0;
  if (angle > 0)
  {
    Direction = 1;
  }
  else
  {
    Direction = 0;
  }
  String print_string = "Steps for angle : " + (String)steps_for_angle;
  Serial.println(print_string);
  while (step_count < steps_for_angle)
  {
    rotate_stepper_steps(stepper_motor_number, 1);
    delayMicroseconds(1600);
    stepper_motor_off(stepper_motor_number);
    step_count++;
  }
}

void rotate_stepper_steps(byte stepper_motor_number, int steps_to_rotate)
{
  for (int x = 0; x < steps_to_rotate; x++)
  {
    switch (current_Step)
    {
      case 0:
        digitalWrite(stepper_pins[stepper_motor_number][0], LOW);
        digitalWrite(stepper_pins[stepper_motor_number][1], LOW);
        digitalWrite(stepper_pins[stepper_motor_number][2], LOW);
        digitalWrite(stepper_pins[stepper_motor_number][3], HIGH);
        break;
      case 1:
        digitalWrite(stepper_pins[stepper_motor_number][0], LOW);
        digitalWrite(stepper_pins[stepper_motor_number][1], LOW);
        digitalWrite(stepper_pins[stepper_motor_number][2], HIGH);
        digitalWrite(stepper_pins[stepper_motor_number][3], HIGH);
        break;
      case 2:
        digitalWrite(stepper_pins[stepper_motor_number][0], LOW);
        digitalWrite(stepper_pins[stepper_motor_number][1], LOW);
        digitalWrite(stepper_pins[stepper_motor_number][2], HIGH);
        digitalWrite(stepper_pins[stepper_motor_number][3], LOW);
        break;
      case 3:
        digitalWrite(stepper_pins[stepper_motor_number][0], LOW);
        digitalWrite(stepper_pins[stepper_motor_number][1], HIGH);
        digitalWrite(stepper_pins[stepper_motor_number][2], HIGH);
        digitalWrite(stepper_pins[stepper_motor_number][3], LOW);
        break;
      case 4:
        digitalWrite(stepper_pins[stepper_motor_number][0], LOW);
        digitalWrite(stepper_pins[stepper_motor_number][1], HIGH);
        digitalWrite(stepper_pins[stepper_motor_number][2], LOW);
        digitalWrite(stepper_pins[stepper_motor_number][3], LOW);
        break;
      case 5:
        digitalWrite(stepper_pins[stepper_motor_number][0], HIGH);
        digitalWrite(stepper_pins[stepper_motor_number][1], HIGH);
        digitalWrite(stepper_pins[stepper_motor_number][2], LOW);
        digitalWrite(stepper_pins[stepper_motor_number][3], LOW);
        break;
      case 6:
        digitalWrite(stepper_pins[stepper_motor_number][0], HIGH);
        digitalWrite(stepper_pins[stepper_motor_number][1], LOW);
        digitalWrite(stepper_pins[stepper_motor_number][2], LOW);
        digitalWrite(stepper_pins[stepper_motor_number][3], LOW);
        break;
      case 7:
        digitalWrite(stepper_pins[stepper_motor_number][0], HIGH);
        digitalWrite(stepper_pins[stepper_motor_number][1], LOW);
        digitalWrite(stepper_pins[stepper_motor_number][2], LOW);
        digitalWrite(stepper_pins[stepper_motor_number][3], HIGH);
        break;
      default:
        digitalWrite(stepper_pins[stepper_motor_number][0], LOW);
        digitalWrite(stepper_pins[stepper_motor_number][1], LOW);
        digitalWrite(stepper_pins[stepper_motor_number][2], LOW);
        digitalWrite(stepper_pins[stepper_motor_number][3], LOW);
        break;
    }
  }
}

void update_steps()
{
  if (Direction == 1)
  {
    current_Step++;
  }
  if (Direction == 0)
  {
    current_Step--;
  }
  if (current_Step > 7)
  {
    current_Step = 0;
  }
  if (current_Step < 0)
  {
    current_Step = 7;
  }
}
