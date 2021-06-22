#define USE_SOFTWAREI2C
#define SOFTI2C_PIN_SDA 7
#define SOFTI2C_PIN_SCL 8
#ifdef USE_SOFTWAREI2C
#include <SoftwareI2C.h>  // https://github.com/micooke/SoftwareI2C
#include <Arduino.h>
SoftwareI2C _i2c;
#else
#include <Wire.h>
#define _i2c Wire
#endif

#include <i2c_device_list.h> //adds 7952 bytes, so comment it out if you dont need it

bool device_found(uint8_t i2c_address)
{
  _i2c.beginTransmission(i2c_address);
  uint8_t device_found = (_i2c.endTransmission() == 0);

  return device_found;
}

void setup()
{
    Serial.begin(9600);
    Serial.println(__FILE__);
    Serial.println(__TIME__);
    #ifdef USE_SOFTWAREI2C
    Serial.print(F("Using SoftwareI2C. SDA,SCL = "));
    Serial.print(SOFTI2C_PIN_SDA);
    Serial.print(F(","));
    Serial.println(SOFTI2C_PIN_SCL);
    _i2c.init(SOFTI2C_PIN_SDA, SOFTI2C_PIN_SCL); // sda, scl
    #else
    Serial.println(F("Using Wire"));
    #endif
    _i2c.begin();

    Serial.println(F("Scanning..."));
    uint8_t numDevicesFound = 0;

    // if there is something found at 0x74 then there is an error with the sda/scl setup
    // there shouldnt be anything here - its reserved
    if (device_found(0x7F) == false) 
    {
        for(uint8_t address=1; address<128; ++address)
        {
            bool deviceFound = device_found(address);

            if(deviceFound)
            {
                
                Serial.print(F("Found : 0x"));
                if (address < 16) Serial.print(F("0"));
                Serial.println(address, HEX);
                #ifdef I2C_DEVICE_LIST_H
                Serial.println(F("which could be one of the following devices"));
                Serial.println(F("--------"));
                String device_name = i2c_device_list(address);
                Serial.println(device_name.c_str());
                Serial.println(F("--------"));
                #endif
                
                ++numDevicesFound;
            }
        }
    }

    if (!numDevicesFound)
    {
      Serial.println(F("No I2C devices found"));
    }
}

void loop() { yield(); }
