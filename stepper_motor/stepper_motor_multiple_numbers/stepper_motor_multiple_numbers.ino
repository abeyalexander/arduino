#define no_of_stepper_motors 2

byte stepper_pins[no_of_stepper_motors][4] =
{
  {8, 9, 10, 11},
  {4, 5, 6, 7}
};

int current_Step[no_of_stepper_motors];
boolean Direction[no_of_stepper_motors];
int step_count = 0, angle, steps_for_angle;

void setup()
{
  for (int i = 0; i < no_of_stepper_motors; i++)
  {
    current_Step[i] = 0;
    Direction[i] = true;
    for (int j = 0; j < 4; j++)
    {
      pinMode(stepper_pins[i][j], OUTPUT);
    }
  }
}

void loop()
{
  rotate_stepper_angles(90);
  delay(1000);
}

void stepper_motor_off(int stepper_motor_number)
{
  for (int i = 0; i < 4; i++)
  {
    digitalWrite(stepper_pins[stepper_motor_number][i], LOW);
  }
}

void rotate_stepper_angles(int angle_to_rotate)
{
  float steps_for_angle = 11.37777778 * abs(angle_to_rotate);
  int step_count = 0;
  for (int i = 0; i < no_of_stepper_motors; i++)
  {
    if (angle > 0)
    {
      Direction[i] = 1;
    }
    else
    {
      Direction[i] = 0;
    }
  }

  //String print_string = "Steps for angle : " + (String)steps_for_angle;
  //Serial.println(print_string);
  while (step_count < steps_for_angle)
  {
    rotate_stepper_steps(0, 1);
    delayMicroseconds(1600);
    stepper_motor_off(0);
    //rotate_stepper_steps(1, 1);
    //delay(2);
    //stepper_motor_off(1);
    step_count++;
  }
}

void rotate_stepper_steps(byte stepper_motor_number, int steps_to_rotate)
{
  for (int x = 0; x < steps_to_rotate; x++)
  {
    switch (current_Step[stepper_motor_number])
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
  update_steps(stepper_motor_number);
}

void update_steps(int i)
{
  if (Direction[i] == 1)
  {
    current_Step[i]++;
  }
  if (Direction[i] == 0)
  {
    current_Step[i]--;
  }
  if (current_Step[i] > 7)
  {
    current_Step[i] = 0;
  }
  if (current_Step[i] < 0)
  {
    current_Step[i] = 7;
  }
}

