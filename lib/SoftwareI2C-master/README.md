### SoftwareI2C

This is an Arduino software (master) I2C Library, written by @Seeed-Studio and (heavily) modified by @micooke to provide a closer interface to hardware Wire libraries.

**SDA and SCL pin allocation are set using the constructor or init() function**

Note : no communications throttling is applied, however there is scope via the included tunedDelay function (from SoftwareSerial) to add this in the future. An AtMega32u4 will run at around 31kHz, a nRF52832 will run at around 400kHz.

Example

```c++
SoftwareI2C softwarei2c(0,1); // initialise as sda,scl = 0,1 
softwarei2c.init(10,11); // sda,scl = 10,11
softwarei2c.begin(); // sda, scl are now set
// ... i2c code ...
softwarei2c.init(14,15); // reallocate sda,scl = 14,15
softwarei2c.begin(); // sda, scl are now set


```

This library includes the following functions:
```c++
SoftwareI2C(uint8_t pinSda = 7, uint8_t pinScl = 8); // constructor
void init(uint8_t pinSda = 7, uint8_t pinScl = 8);
void begin(); 
void beginTransmission(uint8_t addr);
uint8_t endTransmission(bool stopBit = false);
void setClock(uint32_t baudrate);
void end();

uint8_t write(uint8_t dta);
uint8_t write(uint8_t *dta, uint8_t len);
uint8_t requestFrom(uint8_t addr, uint8_t len);
uint8_t read();
uint8_t available();
```

