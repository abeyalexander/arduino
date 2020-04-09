/* Copyright (C) 2012 Kristian Lauszus, TKJ Electronics. All rights reserved.
 
 This software may be distributed and modified under the terms of the GNU
 General Public License version 2 (GPL2) as published by the Free Software
 Foundation and appearing in the file GPL2.TXT included in the packaging of
 this file. Please note that GPL2 Section 2[b] requires that all works based
 on this software must also be made publicly available under the terms of
 the GPL2 ("Copyleft").
 
 Contact information
 -------------------
 
 Kristian Lauszus, TKJ Electronics
 Web      :  http://www.tkjelectronics.com
 e-mail   :  kristianl@tkjelectronics.com
 */

#include <Wire.h>
#include "Kalman.h" // Source: https://github.com/TKJElectronics/KalmanFilter

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
double Ref_Angle=269;



// Motor controller pins
const int PWM_A = 9; // (pwm) pin 3 connected to pin AIN1 
const int PWM_B = 10;
const int BRAKE_A = 5;
const int DIR_A=6;// (pwm) pin 9 connected to pin AIN2 
const int DIR_B = 7; // (pwm) pin 10 connected to pin BIN1  
const int BRAKE_B = 8;  // (pwm) pin 11 connected to pin BIN2 
int pot_val,Kp_val;
 
int speed;
 
// PID
const float Kp = 0.04;
const float Ki = 0;
const float Kd = 0;
float pTerm, iTerm, dTerm, integrated_error, last_error, error;
const float K = 250;   //1.9*1.12;
#define   GUARD_GAIN   10.0
 
#define runEvery(t) for (static typeof(t) _lasttime;(typeof(t))((typeof(t))millis() - _lasttime) > (t);_lasttime += (t))

void setup() 
{  
  for (int thisReading = 0; thisReading < numReadings; thisReading++)
    readings[thisReading] = 0;  
  pinMode(PWM_A, OUTPUT); // set pins to output
  pinMode(DIR_A, OUTPUT);
  pinMode(BRAKE_A,OUTPUT);
  pinMode(DIR_B, OUTPUT);
  pinMode(BRAKE_B, OUTPUT);
  Serial.begin(9600);
  Wire.begin();
  TWBR=((F_CPU/400000L)-16)/2;
  i2cData[0] = 7; // Set the sample rate to 1000Hz - 8kHz/(7+1) = 1000Hz
  i2cData[1] = 0x00; // Disable FSYNC and set 260 Hz Acc filtering, 256 Hz Gyro filtering, 8 KHz sampling
  i2cData[2] = 0x00; // Set Gyro Full Scale Range to ±250deg/s
  i2cData[3] = 0x00; // Set Accelerometer Full Scale Range to ±2g
  while(i2cWrite(0x19,i2cData,4,false)); // Write to all four registers at once
  while(i2cWrite(0x6B,0x01,true)); // PLL with X axis gyroscope reference and disable sleep mode 
  
  while(i2cRead(0x75,i2cData,1));
  if(i2cData[0] != 0x68) 
  { // Read "WHO_AM_I" register
    Serial.print(F("Error reading sensor"));
    while(1);
  }
  delay(100); // Wait for sensor to stabilize
  
  /* Set kalman and gyro starting angle */
  while(i2cRead(0x3B,i2cData,6));
  accX = ((i2cData[0] << 8) | i2cData[1]);
  accY = ((i2cData[2] << 8) | i2cData[3]);
  accZ = ((i2cData[4] << 8) | i2cData[5]);
  // atan2 outputs the value of -p to p (radians) - see http://en.wikipedia.org/wiki/Atan2
  // We then convert it to 0 to 2p and then from radians to degrees
  accYangle = (atan2(accX,accZ)+PI)*RAD_TO_DEG;
  accXangle = (atan2(accY,accZ)+PI)*RAD_TO_DEG;
  
  kalmanX.setAngle(accXangle); // Set starting angle
  kalmanY.setAngle(accYangle);
  gyroXangle = accXangle;
  gyroYangle = accYangle;
  compAngleX = accXangle;
  compAngleY = accYangle;
  timer = micros();
  
}

void avg()
{
  total1= total1 - readings[index];         
  // read from the sensor:  
  readings[index] = analogRead(A0); 
  // add the reading to the total:
  total1= total1 + readings[index];       
  // advance to the next position in the array:  
  index = index + 1;                    

  // if we're at the end of the array...
  if (index >= numReadings)              
    // ...wrap around to the beginning: 
    index = 0;                           

  // calculate the average:
  average1 = total1 / numReadings;         
  // send it to the computer as ASCII digits
  
}
void loop() 
{

  runEvery(250)  // run code @ 40 Hz
  {
    
    dof();
   //avg();
    total= total - readings[index];         
  // read from the sensor:  
  readings[index] = CurrentAngle; 
  
  // add the reading to the total:
  total= total + readings[index];       
  // advance to the next position in the array:  
  index = index + 1;                    

  // if we're at the end of the array...
  if (index >= numReadings)              
    // ...wrap around to the beginning: 
    index = 0;                           

  // calculate the average:
  average = total / numReadings; 
 Pid(); 
    if (CurrentAngle <= (Ref_Angle+0.002*Ref_Angle) && CurrentAngle >= (Ref_Angle-0.002*Ref_Angle))
    {
      stop();
    }
    else
    {
      Pid();
      Motors();
      if (CurrentAngle > (Ref_Angle+0.002*Ref_Angle))
          {
        m_reverse();
      }
      else 
      {
        m_forward();
      }
    }
  }
}

void m_forward()
{
  digitalWrite(12, HIGH); //Establishes forward direction of Channel A
  digitalWrite(9, LOW);  //Disengage the Brake for Channel A
  analogWrite(3, speed);
  digitalWrite(13, LOW); //Establishes forward direction of Channel B
  digitalWrite(8, LOW);   //Disengage the Brake for Channel B
  analogWrite(11, speed);   //Spins the motor on Channel B at full speed 
}

void m_reverse()
{
  digitalWrite(12, LOW); //Establishes backward direction of Channel A
  digitalWrite(9, LOW);  //Disengage the Brake for Channel A
  analogWrite(3, speed);
  digitalWrite(13, HIGH);  //Establishes backward direction of Channel B
  digitalWrite(8, LOW);   //Disengage the Brake for Channel B
  analogWrite(11, speed);
}

void Motors()
{
  if (speed > 0)
  { 
    //forward 
 speed = map(speed,0,255,0,200);
  digitalWrite(12, HIGH); //Establishes forward direction of Channel A
  digitalWrite(9, LOW);  //Disengage the Brake for Channel A
  analogWrite(3, speed);
  digitalWrite(13, LOW); //Establishes forward direction of Channel B
  digitalWrite(8, LOW);   //Disengage the Brake for Channel B
  analogWrite(11, speed);   //Spins the motor on Channel B at full speed 
  //analogWrite(BIN1, speed);
   //analogWrite(BIN2, 0);
  }
  else 
  { 
    // backward
  speed = map(speed,0,-255,0,255);
  digitalWrite(12, LOW); //Establishes backward direction of Channel A
  digitalWrite(9, LOW);  //Disengage the Brake for Channel A
  analogWrite(3, speed);
  digitalWrite(13, HIGH);  //Establishes backward direction of Channel B
  digitalWrite(8, LOW);   //Disengage the Brake for Channel B
  analogWrite(11, speed);  
    //analogWrite(BIN1, 0);
    //analogWrite(BIN2, speed);
  }
}
 
void stop()
{
  analogWrite(PWM_A, 0);
  analogWrite(PWM_B, 0);
  analogWrite(DIR_A, 0);
  analogWrite(DIR_B, 0);
  digitalWrite(9, HIGH);  //Engage the Brake for Channel A
  digitalWrite(8, HIGH);  //Engage the Brake for Channel B
}
 
void Pid()
{
  error = Ref_Angle - CurrentAngle;
  pTerm = Kp * error;
  integrated_error += error;
  iTerm = Ki * constrain(integrated_error, -GUARD_GAIN, GUARD_GAIN);
  dTerm = Kd * (error - last_error);
  last_error = error;
  speed = constrain(K*(pTerm + iTerm + dTerm), -255, 255);

}

void dof() {
  /* Update all the values */  
  while(i2cRead(0x3B,i2cData,14));
  accX = ((i2cData[0] << 8) | i2cData[1]);
  accY = ((i2cData[2] << 8) | i2cData[3]);
  accZ = ((i2cData[4] << 8) | i2cData[5]);
  tempRaw = ((i2cData[6] << 8) | i2cData[7]);  
  gyroX = ((i2cData[8] << 8) | i2cData[9]);
  gyroY = ((i2cData[10] << 8) | i2cData[11]);
  gyroZ = ((i2cData[12] << 8) | i2cData[13]);
  
  // atan2 outputs the value of -p to p (radians) - see http://en.wikipedia.org/wiki/Atan2
  // We then convert it to 0 to 2p and then from radians to degrees
  accXangle = (atan2(accY,accZ)+PI)*RAD_TO_DEG;
  accYangle = (atan2(accX,accZ)+PI)*RAD_TO_DEG;
  
  double gyroXrate = (double)gyroX/131.0;
  double gyroYrate = -((double)gyroY/131.0);
  
//  gyroXangle += gyroXrate*((double)(micros()-timer)/1000000); // Calculate gyro angle without any filter  
//  gyroYangle += gyroYrate*((double)(micros()-timer)/1000000);
//  //gyroXangle += kalmanX.getRate()*((double)(micros()-timer)/1000000); // Calculate gyro angle using the unbiased rate
//  //gyroYangle += kalmanY.getRate()*((double)(micros()-timer)/1000000);
//  
//  compAngleX = (0.93*(compAngleX+(gyroXrate*(double)(micros()-timer)/1000000)))+(0.07*accXangle); // Calculate the angle using a Complimentary filter
//  compAngleY = (0.93*(compAngleY+(gyroYrate*(double)(micros()-timer)/1000000)))+(0.07*accYangle);

//  
  kalAngleX = kalmanX.getAngle(accXangle, gyroXrate, (double)(micros()-timer)/1000000); // Calculate the angle using a Kalman filter
  kalAngleY = kalmanY.getAngle(accYangle, gyroYrate, (double)(micros()-timer)/1000000);
  CurrentAngle = kalmanY.getAngle(accYangle, gyroYrate, (double)(micros()-timer)/1000000);
  timer = micros();
  
  temp = ((double)tempRaw + 12412.0) / 340.0;
  
  
  /* Print Data */
  /*
  Serial.print(accX);Serial.print("\t");
  Serial.print(accY);Serial.print("\t");
  Serial.print(accZ);Serial.print("\t");
  
  Serial.print(gyroX);Serial.print("\t");
  Serial.print(gyroY); Serial.print("\t");
  Serial.print(gyroZ);Serial.print("\t");
  */
//  Serial.print(accXangle);Serial.print("\t");
//  Serial.print(gyroXangle);Serial.print("\t");
//  Serial.print(compAngleX);Serial.print("\t");
//  Serial.print(kalAngleX);Serial.print("\t");
//  
//  Serial.print("\t");
//  
//  Serial.print(accYangle);Serial.print("\t");
//  Serial.print(gyroYangle);Serial.print("\t");
//  Serial.print(compAngleY); Serial.print("\t");
//  Serial.print(kalAngleY);Serial.print("\t");
  
  //Serial.print(temp);Serial.print("\t");
  
  
//Serial.print(CurrentAngle); Serial.print("\t");
//Serial.print("\r\n");
// delay(1);
}
