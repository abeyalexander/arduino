#include <Wire.h>
#include "RTClib.h"

#include <LiquidCrystal.h>
LiquidCrystal lcd(9, 8, 7, 6, 5, 4);
RTC_DS1307 RTC;

#include <FlowMeter.h>                  // https://github.com/sekdiy/FlowMeter
FlowMeter Meter = FlowMeter(2);         // connect a flow meter to an interrupt pin (see notes on your Arduino model for pin numbers)
const unsigned long period = 1000;      // set the measurement update period to 1s (1000 ms)

void MeterISR() {
  Meter.count();  // let our flow meter count the pulses
}                 // define an 'interrupt service handler' (ISR) for every interrupt pin you use

unsigned char warning_LED=13, alarm_LED=12, Buzzer=11;
double peak_flow_rate= 10.00, odd_flow_rate= 05.00;  // flow rates in Litres/ minutes
unsigned int sms_count=0;

void sendSMS(void); ///this function is to send a sms

//----------------------SET UP BEGINS--------------------------------------------------------------
void setup () {
    Wire.begin();
    RTC.begin();
     lcd.begin(16, 2);
     lcd.clear();

   pinMode(warning_LED, OUTPUT);
   pinMode(alarm_LED, OUTPUT);
   pinMode(Buzzer, OUTPUT);
      
  attachInterrupt(INT0, MeterISR, RISING);  // enable a call to the 'interrupt service handler' (ISR) on every rising edge at the interrupt pin
  // do this setup step for every ISR you have defined, depending on how many interrupts you use
  Meter.reset();// sometimes initializing the gear generates some pulses that we should ignore

  Serial.begin(9600);// prepare serial communication
  delay(500);
  }
//---------------------SET UP ENDS-------------------------------------------------------------------


//---------------------LOOP BEGINS-------------------------------------------------------------------
void loop () {
   lcd.setCursor(1, 0);
    delay(period);
    Meter.tick(period);// process the (possibly) counted ticks
    //Serial.println("Currently " + String(Meter.getCurrentFlowrate()) + " l/min, " + String(Meter.getTotalVolume())+ " l total.");   // output some measurement result
    lcd.print(String(Meter.getCurrentFlowrate())+ " Litres/Min" );

     lcd.setCursor(3, 1); 
           
    DateTime now = RTC.now();
    if (now.hour()<10)
      lcd.print('0');
    if(now.hour()>12){
      lcd.print(now.hour()-12, DEC);
       }
    else{
     lcd.print(now.hour(), DEC);
       } 
    
    lcd.print(':');
     if (now.minute()<10)
    lcd.print('0');
    lcd.print(now.minute(), DEC);
    lcd.print(':');
    if (now.second()<10)
    lcd.print('0');
    lcd.print(now.second(), DEC);

    if(now.hour()>11){
      lcd.print(" PM"); 
      }
   else{
      lcd.print(" AM"); 
      }

    //--------------- Time slot comparison starts -----------------------------------------------
    unsigned int Hrs= now.hour();
    unsigned int Minutes= now.minute();
    unsigned int current_time = ((Hrs*100 + Minutes));
    
    double current_flow_rate = Meter.getCurrentFlowrate();
    
    if ((current_time >= 830) && (current_time <= 1830))
      {
       if (current_flow_rate >= peak_flow_rate)
          { 
          //Serial.println("Flow rate exeeded\n");   
          digitalWrite(warning_LED, HIGH);
          digitalWrite(alarm_LED, LOW);
          digitalWrite(Buzzer, HIGH);
          sms_count++;
          //delay(950);
          if (sms_count == 60  || sms_count == 360 )
            {
              noInterrupts();
              SendSMS();
              interrupts();
              if (sms_count == 360)
                sms_count= 60;
             }
          }
       else                       
          {
          digitalWrite(warning_LED, LOW);
          digitalWrite(alarm_LED, LOW);
          sms_count= 0;
          }
      }
    else if ((current_time <= 830) || (current_time >= 1830))
      {
       if (current_flow_rate >= odd_flow_rate)
          {
          //Serial.println("Flow rate exeeded\n");   
          digitalWrite(warning_LED, LOW);
          digitalWrite(alarm_LED, HIGH);
          digitalWrite(Buzzer, HIGH);
          sms_count++;
          //delay(950);
          if (sms_count == 60  || sms_count == 360 )
            {
              noInterrupts();
              SendSMS();
              interrupts();
              if (sms_count == 360)
                sms_count= 60;
             }
          }
       else                       
          {
          digitalWrite(warning_LED, LOW);
          digitalWrite(alarm_LED, LOW);
          sms_count= 0;
          }    
      }
}//---------------------LOOP ENDS-------------------------------------------------------------------


//------------------- Function for SMS sending-----------------------------------------------------
 void SendSMS(void)
{
  Serial.println("AT+CMGF=1");    //Because we want to send the SMS in text mode
  delay(100);
  Serial.println("AT+CMGS=\"+918547393655\"\r"); // change to the phone number you using 
  delay(100);
  Serial.println("ALERT: Water flow rate exeeded in ur college..High water consumption..!!!!");//the content of the message
  delay(100);
  
  Serial.println((char)26);//the ASCII code of the ctrl+z is 26
  delay(100);
}

