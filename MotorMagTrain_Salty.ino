// MotorMagTrain_Salty.ino
// Tristan Whisenant 7/9/2018
// Communicate with the Magnetometer, Drive the Train, Stop the Train
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//LIBRARIES
#include <Wire.h>
#include <SPI.h>
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

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// *** NEW USER INPUT HERE ***
// CHANGE HOW LONG SALTY RUNS (in milliseconds)
int runtime = 3000;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// DEFINE CONSTANTS
int breakboi = 50;
int saltyrun = 12;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Use function in sensor library to setup sensor and set magnetic range
void setupSensor()
{
  // 2.) Set the magnetometer sensitivity
  lsm.setupMag(lsm.LSM9DS1_MAGGAIN_16GAUSS);
}


void setup() 
{
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // Begin Serial Communication
       Serial.begin(9600);
    
    // Optional Print Output (Disabled in other sketches as it makes it more difficult to compute with later) 
       Serial.println("LSM9DS1 Magnetic Data (Gauss)");
      
    // Intialize Chip
      lsm.begin();
    
    // start Salty's Motor Pin
      pinMode(saltyrun, OUTPUT); 
    
    // Run the Setup Sensor Func.
      setupSensor(); 
}

void loop() 
{  

// The Main Measurement Loop
    while(millis() <=  runtime)   // Stop taking data after specified amount of time
    {

      // Measure the Data 
      lsm.read();  
    
      // Tell Magnetometer to be ready for more data in the future 
      sensors_event_t a, m, g, temp;
    
      lsm.getEvent(&a, &m, &g, &temp);

    
      // Print Magnetic Data to Serial Window
      Serial.print(millis());     Serial.print(","); 
      Serial.print(m.magnetic.x); Serial.print(","); 
      Serial.print(m.magnetic.y); Serial.print(",");
      Serial.print(m.magnetic.z); 
      Serial.println("");
      
      // Keep Salty Chugging Along
      digitalWrite(saltyrun, HIGH);
      delay(breakboi);
     }


     // Stop Salty from running off the track
     digitalWrite(saltyrun, LOW);



        // Terminating Line (To Be Discarded)
        Serial.print(9999); Serial.print(","); 
        Serial.print(9999); Serial.print(","); 
        Serial.print(9999); Serial.print(",");
        Serial.print(9999); 
        Serial.println("");

        // Break the Loop
        for (;;);     


  }

