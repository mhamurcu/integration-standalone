#include <SoftwareI2C.h>  // https://github.com/micooke/SoftwareI2C
SoftwareI2C _i2c;

#if defined(_VARIANT_T28_)
#define DIGITAL_PINS 28
uint8_t DigitalPins[DIGITAL_PINS] = {
  PIN_TOUCH,PIN_HR_ON,PIN_SERIAL1_RX,PIN_SERIAL1_TX,
  PIN_OLED_VPP,PIN_OLED_SW,
  PIN_SPI1_MISO,PIN_SPI1_MOSI,PIN_SPI1_SCK,PIN_SPI1_CS,
  PIN_BUTTON1,PIN_BUTTON2,PIN_VIBRATE,LED_BUILTIN,
  PIN_SERIAL_RX,PIN_SERIAL_TX,
  PIN_SPI_MISO,PIN_SPI_MOSI,PIN_SPI_SCK,
  SS,MOSI,MISO,SCK,
  OLED_RST,OLED_CS,OLED_DC,
  PIN_WIRE_SDA,PIN_WIRE_SCL};
#define ANALOG_PINS 8
uint8_t AnalogPins[ANALOG_PINS] = { PIN_A0, PIN_A1, PIN_A2, PIN_A3, PIN_A4, PIN_A5, PIN_A6, PIN_A7 };
#elif defined (_VARIANT_IDO003_) | defined (_VARIANT_ID100HR_) | defined(_VARIANT_ID107HR_)
#define DIGITAL_PINS 18
uint8_t DigitalPins[DIGITAL_PINS] = {
  PIN_BUTTON1,PIN_BUTTON2,PIN_VIBRATE,LED_BUILTIN,
  PIN_SERIAL_RX,PIN_SERIAL_TX,
  PIN_SPI_MISO,PIN_SPI_MOSI,PIN_SPI_SCK,
  SS,MOSI,MISO,SCK,
  OLED_RST,OLED_CS,OLED_DC,
  PIN_WIRE_SDA,PIN_WIRE_SCL};
#define ANALOG_PINS 6
uint8_t AnalogPins[ANALOG_PINS] = { PIN_A0, PIN_A1, PIN_A2, PIN_A3, PIN_A4, PIN_A5};
#else
#define DIGITAL_PINS 12
uint8_t DigitalPins[DIGITAL_PINS] = {
  LED_BUILTIN,
  PIN_SERIAL_RX,PIN_SERIAL_TX,
  PIN_SPI_MISO,PIN_SPI_MOSI,PIN_SPI_SCK,
  SS,MOSI,MISO,SCK,
  PIN_WIRE_SDA,PIN_WIRE_SCL};
#define ANALOG_PINS 6
uint8_t AnalogPins[ANALOG_PINS] = { PIN_A0, PIN_A1, PIN_A2, PIN_A3, PIN_A4, PIN_A5};
#endif

#include <i2c_device_list.h> //adds 7952 bytes, so comment it out if you dont need it

#include <Vector.h> // https://github.com/zacsketches/Arduino_Vector

Vector<uint8_t> UNUSED_PINS;
uint8_t NUM_UNUSED_PINS = 0;

// the maximum number of i2c devices found before the i2c setup is declared invalid
#define MAX_I2C_DEVICES 10

void populate_UNUSED_PINS()
{
  // Note : AnalogPins arent dedicated to a function so will be used by rev eng tools
  for (uint8_t p=0; p < PINS_COUNT; ++p)
  {
    bool isAllocated = false;
    for (uint8_t idx=0; idx < DIGITAL_PINS; ++idx)
    {
      if (p == DigitalPins[idx])
      {
        isAllocated = true;
        break;
      }
    }

    // If it isnt one of the digital pins, save it as unused
    if (isAllocated == false)
    {
      UNUSED_PINS.push_back(p);
      ++NUM_UNUSED_PINS;
    }
  }
}

bool device_found(uint8_t i2c_address)
{
  _i2c.beginTransmission(i2c_address);
  bool _device_found = (_i2c.endTransmission() == 0);
  return _device_found;
}

void find_devices(uint8_t pin_sda, uint8_t pin_scl)
{
  _i2c.init(pin_sda, pin_scl); // sda, scl
  _i2c.begin();
  
  // iterate through each address
  uint8_t foundI2Cdevice[MAX_I2C_DEVICES];
  uint8_t numDevicesFound = 0;
  for(uint8_t address=1; address<128; ++address)
  {
    if (device_found(address))
    {
      foundI2Cdevice[numDevicesFound++] = address;
      if (numDevicesFound == MAX_I2C_DEVICES)
      {
        return;
      }
    }
  }

  // output the address of valid i2c devices
  for(uint8_t idx=0; idx<numDevicesFound; ++idx)
  {
    const uint8_t address = foundI2Cdevice[idx];
    Serial.print(F("Found : 0x"));
    if (address < 16) Serial.print(F("0"));
    Serial.print(address, HEX);
    Serial.print(F(" @ SDA,SCL = "));
    Serial.print(pin_sda);
    Serial.print(F(","));
    Serial.println(pin_scl);
    #ifdef I2C_DEVICE_LIST_H
    Serial.println(F("which could be one of the following devices"));
    Serial.println(F("--------"));
    String device_name = i2c_device_list(address);
    Serial.println(device_name.c_str());
    Serial.println(F("--------"));
    #endif
  }

  _i2c.end();
}

void setup()
{
    Serial.begin(9600);
    Serial.println(__FILE__);
    Serial.println(__TIME__);

    populate_UNUSED_PINS();

    Serial.println(F("Start I2C scan"));
    for (uint8_t i1 = 0; i1 < NUM_UNUSED_PINS; ++i1)
    {
        for (uint8_t i2 = i1+1; i2 < NUM_UNUSED_PINS; ++i2)
        {
            find_devices(UNUSED_PINS[i1], UNUSED_PINS[i2]);
            find_devices(UNUSED_PINS[i2], UNUSED_PINS[i1]);
        }
    }
    Serial.println(F("Scan complete"));
}

void loop() { yield(); }

