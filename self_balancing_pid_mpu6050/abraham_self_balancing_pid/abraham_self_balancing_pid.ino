#include <Wire.h>
#include "Kalman.h" // Source: https://github.com/TKJElectronics/KalmanFilter
#include <PID_v1.h>

#define LED_PIN 13

Kalman kalmanX; // Create the Kalman instances
Kalman kalmanY;
const int numReadings = 10;

int readings[numReadings]; // the readings from the analog input
int readings_ca[numReadings];
int index = 0;                  // the index of the current reading
int total = 0;                  // the running total
int average = 0; // the average
int total1 = 0;
int average1 = 0; // the average

/* IMU Data */
int16_t accX, accY, accZ;
int16_t tempRaw;
int16_t gyroX, gyroY, gyroZ;

double accXangle, accYangle; // Angle calculate using the accelerometer
double temp; // Temperature
double gyroXangle, gyroYangle; // Angle calculate using the gyro
double compAngleX, compAngleY; // Calculate the angle using a complementary filter
double kalAngleX, kalAngleY; // Calculate the angle using a Kalman filter

uint32_t timer;
uint8_t i2cData[14]; // Buffer for I2C data
double CurrentAngle;
double InitialAngle;

/*----------------------------------------------PID  VALUES------------------------------------------*/
double Setpoint, Input, Output;
double Kp = 40, Ki = 40, Kd = 0.05;                 //initial PID constants
/*-----------------------------------------------------------------------------------------------------*/


/*------------------------------------MOTOR DRIVE------------------------------------------------------*/
//int IN1,IN2,IN3,IN4,ENA,ENB;
int IN1 = 5;
int IN2 = 6;
int IN3 = 7;
int IN4 = 8;
int ENA = 9;
int ENB = 10;
/*-----------------------------------------------------------------------------------------*/
double CAngle_array[100];
int i = 0, len_array = 100;
long tick = 0;

PID myPID(&Input, &Output, &Setpoint, Kp, Ki, Kd, REVERSE);                 // PID variable initialization

#define runEvery(t) for (static typeof(t) _lasttime;(typeof(t))((typeof(t))millis() - _lasttime) > (t);_lasttime += (t))

void setup()
{
  for (int thisReading = 0; thisReading < numReadings; thisReading++)
    readings[thisReading] = 0;

  Serial.begin(9600);
  Wire.begin();
  TWBR = ((F_CPU / 400000L) - 16) / 2;
  i2cData[0] = 7; // Set the sample rate to 1000Hz - 8kHz/(7+1) = 1000Hz
  i2cData[1] = 0x00; // Disable FSYNC and set 260 Hz Acc filtering, 256 Hz Gyro filtering, 8 KHz sampling
  i2cData[2] = 0x00; // Set Gyro Full Scale Range to ±250deg/s
  i2cData[3] = 0x00; // Set Accelerometer Full Scale Range to ±2g
  while (i2cWrite(0x19, i2cData, 4, false)); // Write to all four registers at once
  while (i2cWrite(0x6B, 0x01, true)); // PLL with X axis gyroscope reference and disable sleep mode

  while (i2cRead(0x75, i2cData, 1));
  if (i2cRead(0x75, i2cData, 1)==0)
  {
    OFF();
  }
  if (i2cData[0] != 0x68)
  { // Read "WHO_AM_I" register
    Serial.print(F("Error reading sensor"));
    while (1);
  }
  delay(100); // Wait for sensor to stabilize

  /* Set kalman and gyro starting angle */
  while (i2cRead(0x3B, i2cData, 6));
  if (i2cRead(0x3B, i2cData, 6)==0)
  {
    OFF();
  }
  accX = ((i2cData[0] << 8) | i2cData[1]);
  accY = ((i2cData[2] << 8) | i2cData[3]);
  accZ = ((i2cData[4] << 8) | i2cData[5]);
  // atan2 outputs the value of -p to p (radians) - see http://en.wikipedia.org/wiki/Atan2
  // We then convert it to 0 to 2p and then from radians to degrees
  accYangle = (atan2(accX, accZ) + PI) * RAD_TO_DEG;
  accXangle = (atan2(accY, accZ) + PI) * RAD_TO_DEG;

  kalmanX.setAngle(accXangle); // Set starting angle
  kalmanY.setAngle(accYangle);
  gyroXangle = accXangle;
  gyroYangle = accYangle;
  compAngleX = accXangle;
  compAngleY = accYangle;
  timer = micros();
  dof();
  Setpoint = 0;
  InitialAngle = CurrentAngle;
  myPID.SetMode(AUTOMATIC);
  //myPID.SetSampleTime(100);
  pinMode(LED_PIN, OUTPUT);
}

void loop()
{
  dof();
  double error = InitialAngle - CurrentAngle;
  Input = abs(error);
  myPID.Compute();
  if (abs(error) < 20)
  {
    if (error > 0) 
    {
      forward(Output);
    }
    else if (error < 0)
    {
      reverse(Output);
    }
    else
      OFF();
  }

  else
  {
    OFF();
  }
  if (millis() - tick > 500)
  {
    String print_string = (String)CurrentAngle + ", " + (String)Input + ", " + (String)error + ", " + (String)Output;
    //String print_string = (String)accX + ", " + (String)accY + ", " + (String)accZ;
    Serial.println(print_string);
    digitalWrite(LED_PIN, !digitalRead(LED_PIN));
    tick = millis();
  }
}

void dof()
{
  /* Update all the values */
  while (i2cRead(0x3B, i2cData, 14));
  accX = ((i2cData[0] << 8) | i2cData[1]);
  accY = ((i2cData[2] << 8) | i2cData[3]);
  accZ = ((i2cData[4] << 8) | i2cData[5]);
  tempRaw = ((i2cData[6] << 8) | i2cData[7]);
  gyroX = ((i2cData[8] << 8) | i2cData[9]);
  gyroY = ((i2cData[10] << 8) | i2cData[11]);
  gyroZ = ((i2cData[12] << 8) | i2cData[13]);

  // atan2 outputs the value of -p to p (radians) - see http://en.wikipedia.org/wiki/Atan2
  // We then convert it to 0 to 2p and then from radians to degrees
  accXangle = (atan2(accY, accZ) + PI) * RAD_TO_DEG;
  accYangle = (atan2(accX, accZ) + PI) * RAD_TO_DEG;

  double gyroXrate = (double)gyroX / 131.0;
  double gyroYrate = -((double)gyroY / 131.0);

  gyroXangle += gyroXrate * ((double)(micros() - timer) / 1000000); // Calculate gyro angle without any filter
  gyroYangle += gyroYrate * ((double)(micros() - timer) / 1000000);
  compAngleY = (0.93 * (compAngleY + (gyroYrate * (double)(micros() - timer) / 1000000))) + (0.07 * accYangle);
  kalAngleX = kalmanX.getAngle(accXangle, gyroXrate, (double)(micros() - timer) / 1000000); // Calculate the angle using a Kalman filter
  kalAngleY = kalmanY.getAngle(accYangle, gyroYrate, (double)(micros() - timer) / 1000000);
  timer = micros();

  temp = ((double)tempRaw + 12412.0) / 340.0;
  CurrentAngle = kalAngleY;
}

void OFF()
{

  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
  analogWrite(ENA, 0);
  analogWrite(ENB, 0);

}
void forward(int pwm_width)
{

  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
  analogWrite(ENA, pwm_width);
  analogWrite(ENB, pwm_width);
}

void reverse(int pwm_width)
{
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
  analogWrite(ENA, pwm_width);
  analogWrite(ENB, pwm_width);

}

