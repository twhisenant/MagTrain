// Salty_Driver.ino
// Whisenant 4/3/2018
// Allow Infrared Remote Communication with the Train

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//LIBRARIES
#include <Wire.h>
#include <SPI.h>
#include <IRremote.h>
#include <Adafruit_LSM9DS1.h>
#include <Adafruit_Sensor.h>  // not used in this demo but required!

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// i2c Communication Tag
Adafruit_LSM9DS1 lsm = Adafruit_LSM9DS1();

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// DEFINE PINS
#define LSM9DS1_SCK A5
#define LSM9DS1_MISO 12
#define LSM9DS1_MOSI A4
#define LSM9DS1_XGCS 6
#define LSM9DS1_MCS 5
// You can also use software SPI
//Adafruit_LSM9DS1 lsm = Adafruit_LSM9DS1(LSM9DS1_SCK, LSM9DS1_MISO, LSM9DS1_MOSI, LSM9DS1_XGCS, LSM9DS1_MCS);
// Or hardware SPI! In this case, only CS pins are passed in
//Adafruit_LSM9DS1 lsm = Adafruit_LSM9DS1(LSM9DS1_XGCS, LSM9DS1_MCS);

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// DEFINE CONSTANTS & PINS
int breakboi = 50;
int saltyrun = 12;
int RECV_PIN = 11;
int RESET    = 8;


// For IR Remote
int SaltyState = LOW; 
int GO         = 999;
IRrecv irrecv(RECV_PIN);
decode_results results;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Use function in sensor library to setup sensor and set magnetic range
void setupSensor()
{
  // Set the magnetometer sensitivity
  lsm.setupMag(lsm.LSM9DS1_MAGGAIN_16GAUSS);
}


void setup() 
{
  ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  // Begin Serial Communication
  Serial.begin(9600);
  
  // Intialize Chip
  lsm.begin();

  //Initialize IR
  irrecv.enableIRIn(); // Start the receiver

  // start Salty's Motor Pin
  pinMode(saltyrun, OUTPUT); 

  // Run the Setup Sensor Func.
  setupSensor(); 

  // Set Up Reset Sequence
  pinMode(RESET, INPUT);
  digitalWrite(RESET, LOW); 

}

void loop()
{

// Stop Salty on BackGround
  digitalWrite(saltyrun, LOW);

 // IR Remote Governor
 if (irrecv.decode(&results))
   {
   irrecv.resume();
   while (results.value == 0xFFC23D)
         {
 
         DRIVE();
         delay(breakboi);
         
                if(irrecv.decode(&results))
                  {
                  irrecv.resume();
                  if (results.value != 0xFFC23D & results.value != 0xFFFFFFFF)
                        {
//                        digitalWrite(saltyrun, LOW);
//                        irrecv.resume();
//                        break;
                          loop();
                        }
                  }
                  
         }
    

    if(results.value == 0xFF22DD)
        {
        STOP();
        }

   }

 }

   // Salty Runner
  void DRIVE()
        {
          lsm.read();  /* ask it to read in the data */ 
        
          /* Get a new sensor event */ 
          sensors_event_t a, m, g, temp;
        
          lsm.getEvent(&a, &m, &g, &temp);
        
          // Print Acceleration Data
        
          Serial.print(millis());     Serial.print(","); 
          Serial.print(m.magnetic.x); Serial.print(","); 
          Serial.print(m.magnetic.y); Serial.print(",");
          Serial.print(m.magnetic.z); 
          Serial.println("");

          delay(breakboi);
          
          // Keep Salty Chugging Along
          digitalWrite(saltyrun, HIGH);
        }

  void STOP()
       {
          // Kill Matlab
          Serial.print(9999); Serial.print(","); 
          Serial.print(9999); Serial.print(","); 
          Serial.print(9999); Serial.print(",");
          Serial.print(9999); 
          Serial.println("");
          
          // STOP Salty the Train
          digitalWrite(saltyrun, LOW);

          // Give it time to print the 9999's
          delay(50);

          //Reset Board
          pinMode(RESET, OUTPUT);  
       }
 
 
