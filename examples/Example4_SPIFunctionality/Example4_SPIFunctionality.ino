#include "SparkFun_STHS34PF80_Arduino_Library.h"
#include <SPI.h>

STHS34PF80_SPI mySensor; 

// Presence and Motion variables to fill
int16_t presenceVal = 0;
float temperatureVal = 0;

// Set your chip select pin according to your setup
uint8_t chipSelect = 12;

void setup()
{
    Serial.begin(115200);
    Serial.println("STHS34PF80 Example 4: SPI Functionality");

    // Configure the chip select pin
    pinMode(chipSelect, OUTPUT);
	  digitalWrite(chipSelect, HIGH);

    // Begin SPI
    SPI.begin();

    // Establish communication with device 
    if(mySensor.begin(chipSelect) != 0)
    {
      Serial.println("Error setting up device - please check wiring.");
      while(1);
    }

    delay(500);
}

void loop() 
{
  sths34pf80_tmos_drdy_status_t dataReady;
  mySensor.getDataReady(&dataReady);
  Serial.println(dataReady.drdy);

  // Check whether sensor has new data - run through loop if data is ready
  if(dataReady.drdy == 1)
  {
    sths34pf80_tmos_func_status_t status;
    mySensor.getStatus(&status);
    
    // If presence flag is high, then print data
    if(status.pres_flag == 1)
    {
      // Presence Units: cm^-1
      mySensor.getPresenceValue(&presenceVal);
      Serial.print("Presence: ");
      Serial.print(presenceVal);
      Serial.println(" cm^-1");
    }

    if(status.mot_flag == 1)
    {
      Serial.println("Motion Detected!");
    }

    if(status.tamb_shock_flag == 1)
    {
      mySensor.getTemperatureData(&temperatureVal);
      Serial.print("Temperature: ");
      Serial.print(temperatureVal);
      Serial.println(" °C");
    }
  }
}