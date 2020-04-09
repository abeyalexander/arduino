#include <SoftwareSerial.h>
#include <SPI.h>
#include <SD.h>
 
SoftwareSerial GPS(4,5);                             /* Software serial - RX, TX pins                           */
File Cordinates;
 
/*........................................ Variables declaration .............................................  */
 
char Rec_data;
char RMC_flag=0,comma=0,i=0,j=0;
char latitude[12],longitude[12];
int smsflag=0,stable=0,finish=0,snd=0;
 
/*............................................... Setup ....................................................... */
 
void setup() 
{
  Serial.begin(9600);                                /* Initialize serial communication at 9600 bits per second */
  GPS.begin(9600);                                   /* Initialize software serial at 9600 bps for GPS          */
 
  while (!SD.begin(9)) 
  {
    Serial.println("SD Card Initialization failed!");
    return;
  }
  Serial.println("SD Card Initialized");
 
  Serial.print("AT\r\n");                            /* Initialization command                                  */
  delay(1000); 
  Serial.print("ATE0\r\n");                          /* Turn echo off                                           */
  delay(1000);
  Serial.print("AT+CMGF=1\r\n");                     /* Text mode                                               */
  delay(1000); 
  Serial.print("AT+CNMI=2,1,0,0,0\r\n");             /* Set message format                                      */
  delay(1000);
}
 
/*...................................................Loop...................................................... */
 
void loop()
{
  if(smsflag==0 && finish==1 && snd==0)              /* Send message after storing GPS cordinates               */
  {
    Serial.print("AT+CMGS=\"+919847148655\"\r\n");   /* Replace xxxxxxxxxx with a valid 10-digit mobile no:     */
    delay(1000); 
  }    
 
  if(smsflag==1 && finish==1 && snd==0)              /* Send the message                                        */
  {
    Serial.print("Latitude:");  
      Serial.print(latitude);                        /* Send latitude                                           */
    Serial.print("\r\n");
    Serial.print("Longitude:");
      Serial.print(longitude);                       /* Send longitude                                          */
    Serial.print('\x1A');                            /* Send Ctrl+Z after the message                           */
 
    Cordinates = SD.open("GPS.txt", FILE_WRITE);     /* Open the file GPS.txt to write cordinates               */
 
    if(Cordinates)                                   /* If the file opened okay, write to it                    */
    {
      Serial.print("Writing to GPS.txt...");
      Cordinates.print("Latitude:");
      Cordinates.println(latitude);                  /* Write latitude to the file                              */
      Cordinates.print("Longitude:");
      Cordinates.println(longitude);                 /* Write longitude to the file                             */
      Cordinates.close();                            /* Close the file                                          */
      Serial.println("done.");
    } 
    else
    {
      Serial.println("Error opening GPS.txt");       /* If the file didn't open, print an error                 */
    }
 
    Cordinates = SD.open("GPS.txt");                 /* Re-open the file for reading                            */
    if(Cordinates)
    {
      Serial.println("GPS.txt");
      while (Cordinates.available())                 /* Read from the file until there's nothing else in it     */
      {
        Serial.write(Cordinates.read());
      }
      Cordinates.close();                            /* Close the file                                          */
 
/*----------- Uncomment these lines if you want to delete the file ---------------------------------------------
 
      Serial.println("Removing GPS.txt...");
      SD.remove("GPS.txt");
      Serial.println("Removed GPS.txt...");
 
----------------------------------------------------------------------------------------------------------------*/
    } 
    else
    {
      Serial.println("Error opening GPS.txt");       /* If the file didn't open, print an error                 */
    }  
 
    i = 0;
    j = 0;
    RMC_flag = 0;
    comma = 0;                                       /* Clear the variables                                     */
    snd = 1;
    smsflag = 0;
    finish = 0;
    stable = 0;
  }
 
  while(GPS.available())                             /* Check if any data has arrived in software UART          */
  {
    Rec_data = GPS.read();                           /* Copy the received charactr to a variable                */
 
    if(Rec_data == 'G')                              /* Check for GPRMC header                                  */
    {
    RMC_flag = 1;
    }
    if(Rec_data == 'P' && RMC_flag==1)               /* Check for GPRMC header                                  */
    {
    RMC_flag = 2;
    }
    if(Rec_data == 'R' && RMC_flag==2)               /* Check for GPRMC header                                  */
    {                                                  
      RMC_flag = 3;
    }
 
    else if(Rec_data=='M' && RMC_flag==3) 
    {
      RMC_flag = 4;
    }
 
    else if(Rec_data=='C' && RMC_flag==4)
    { 
      RMC_flag = 5;
    }
 
    if(RMC_flag == 5)
    {
      if(Rec_data==',') 
         comma++;                                    /* If GPRMC header is received, count the no: of commas    */  
 
      if(comma==2 && Rec_data=='A')                  /* Check if GPS is stable                                  */
      {
         stable=1;
      }  
 
      else if(comma==2 && Rec_data=='V')
      {
         stable=0;
         comma=0;
         RMC_flag=0;
      }  
 
      else if(comma>=3 && comma<5 && Rec_data!=',' && stable==1)
      {   
        latitude[i++]=Rec_data;                      /* Store latitude in an array                              */
      }
 
      else if(comma>=5 && comma<7 && Rec_data!=',' && stable==1)
      {   
        longitude[j++]=Rec_data;                     /* Store longitude in an array                             */
      }
 
      if(Rec_data=='*' && stable==1)
      {
        comma=0; finish=1;
      }
    }
  } 
 
  while (Serial.available())                         /* Check if any data has arrived in hardware UART          */
  {
    Rec_data = Serial.read();                        /* Copy the received character to a variable               */
    if(Rec_data=='>')                                /* Set flag for response to "AT+CMGS=\"+91xxxxxxxxxx\""    */
    {
      smsflag = 1;
    } 
  }
}
