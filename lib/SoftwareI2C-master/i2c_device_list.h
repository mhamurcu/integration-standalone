// original source:
// https://learn.adafruit.com/i2c-addresses/the-list
//
// additions have been made as they have been found
//
// PCA9685 can be used by any address between 0x40 to 0x7F
// 0x00 - 0x07 and 0x78 - 0x7F are reserved I2C addresses
//

#ifndef I2C_DEVICE_LIST_H
#define I2C_DEVICE_LIST_H
String i2c_device_list(uint8_t device_address)
{
   switch (device_address)
   {
      // 0x00 - 0x0F
      case 0x00:
         return "Reserved - General Call Address";
      case 0x01:
         return "Reserved for CBUS Compatibility";
      case 0x02:
         return "Reserved for I2C-compatible Bus Variants";
      case 0x03:
         return "Reserved for Future Use";
      case 0x04:
      case 0x05:
      case 0x06:
      case 0x07:
         return "Reserved for Hs-mode Master";
      case 0x0E:
         return "MAG3110 3-Axis Magnetometer (0x0E only)";
      // 0x10 - 0x1F
      case 0x11:
         return "Si4713 FM Transmitter with RDS (0x11 or 0x63)";
      case 0x13:
         return "VCNL40x0 proximity sensor (0x13 only)";
      case 0x18:
         return "MCP9808 temp sensor (0x18 - 0x1F)\n"
                "LIS3DH 3-axis accelerometer (0x18 or 0x19)\n"
                "LSM303 Accelerometer & Magnetometer (0x19 for acc, 0x1E for mag)\n";
      case 0x19:
         return "MCP9808 temp sensor (0x18 - 0x1F)\n"
                "LIS3DH 3-axis accelerometer (0x18 or 0x19)";
      case 0x1A:
      case 0x1B:
         return "MCP9808 temp sensor (0x18 - 0x1F)";
      case 0x1C:
         return "MCP9808 temp sensor (0x18 - 0x1F)\n"
                "MMA845x 3-axis Accelerometer (0x1C or 0x1D)\n"
                "FXOS8700 Accelerometer/Magnetometer (0x1C, 0x1D, 0x1E or 0x1F)\n"
                "MMA7455L (0x1C or 0x1D)";
      case 0x1D:
         return "MCP9808 temp sensor (0x18 - 0x1F)\n"
                "MMA845x 3-axis Accelerometer (0x1C or 0x1D)\n"
                "FXOS8700 Accelerometer/Magnetometer (0x1C, 0x1D, 0x1E or 0x1F)\n"
                "LSM9DS0 9-axis IMU (0x1D or 0x1E for Accel/Mag, 0x6A or 0x6B for Gyro)\n"
                "ADXL345 3-axis accelerometer (0x1D or 0x53)\n"
                "MMA7455L (0x1C or 0x1D)";
      case 0x1E:
         return "Kx022-1020 (0x1E or 0x1F)\n"
                "MCP9808 temp sensor (0x18 - 0x1F)\n"
                "FXOS8700 Accelerometer/Magnetometer (0x1C, 0x1D, 0x1E or 0x1F)\n"
                "LSM303 Accelerometer & Magnetometer (0x19 for acc, 0x1E for mag)\n"
                "LSM9DS0 9-axis IMU (0x1D or 0x1E for Accel/Mag, 0x6A or 0x6B for Gyro)\n"
                "HMC5883 Magnetometer (0x1E only";
      case 0x1F:
         return "Kx022-1020 Accelerometer (0x1E or 0x1F)\n"
                "MCP9808 temp sensor (0x18 - 0x1F)\n"
                "FXOS8700 Accelerometer/Magnetometer (0x1C, 0x1D, 0x1E or 0x1F)";
      // 0x20-0x2
      case 0x20:
         return "FXAS21002 Gyroscope (0x20 or 0x21)\n"
                "Chirp !Water sensor(0x20)\n"
                "MCP23008 I2C GPIO expander (0x20 - 0x27)\n"
                "MCP23017 I2C GPIO expander (0x20 - 0x27)";
      case 0x21:
         return "FXAS21002 Gyroscope (0x20 or 0x21)\n"
                "MCP23008 I2C GPIO expander (0x20 - 0x27)\n"
                "MCP23017 I2C GPIO expander (0x20 - 0x27)";
      case 0x22:
      case 0x23:
      case 0x24:
      case 0x25:
      case 0x26:
      case 0x27:
         return "MCP23008 I2C GPIO expander (0x20 - 0x27)\n"
                "MCP23017 I2C GPIO expander (0x20 - 0x27)";
      case 0x28:
         return "BNO055 IMU (0x28 or 0x29)\n"
                "CAP1188 8-channel Capacitive Touch (0x28 - 0x2D)";
      case 0x29:
         return "BNO055 IMU (0x28 or 0x29)\n"
                "TCS34725 color sensor (0x29 only)\n"
                "TSL2561 light sensor (0x29, 0x39 or 0x49)\n"
                "TSL2591 light sensor (0x29 only)\n"
                "VL53L0x ToF distance (0x29, software selectable)\n"
                "VL6180X ToF sensor (0x29)\n"
                "CAP1188 8-channel Capacitive Touch (0x28 - 0x2D)";
      case 0x2A:
      case 0x2B:
      case 0x2C:
      case 0x2D:
         return "CAP1188 8-channel Capacitive Touch (0x28 - 0x2D)";
      // 0x30 - 0x3F
      case 0x38:
         return " VEML6070 UV Index(0x38 and 0x39)\n"
                "FT6x06 Capacitive Touch Driver (0x38 only)";
      case 0x39:
         return "TSL2561 light sensor (0x29, 0x39 or 0x49)\n"
                "VEML6070 UV Index (0x38 and 0x39)\n"
                "APDS-9960 IR/Color/Proximity Sensor (0x39 only";
      case 0x3C:
      case 0x3D:
         return "SSD1305 monochrome OLED (0x3C or 0x3D)\n"
                "SSD1306 monochrome OLED (0x3C or 0x3D)";
      // 0x40 - 0x4
      case 0x40:
         return "Si7021 Humidity/Temp sensor (0x40 only)\n"
                "HTU21D-F Humidity/Temp Sensor (0x40 only)\n"
                "HDC1008 Humidity/Temp sensor (0x40, 0x41, 0x42 or 0x43)\n"
                "TMP007 IR Temperature sensor (0x40 - 0x47)\n"
                "TMP006 IR Temperature sensor (0x40 - 0x47)\n"
                "PCA9685 16-channel PWM driver default address (0x40 - 0x7F)\n"
                "INA219 High-Side DC Current/Voltage sensor (0x40 - 0x4F)";
      case 0x41:
         return "HDC1008 Humidity/Temp sensor (0x40, 0x41, 0x42 or 0x43)\n"
                "TMP007 IR Temperature sensor (0x40 - 0x47)\n"
                "TMP006 IR Temperature sensor (0x40 - 0x47)\n"
                "INA219 High-Side DC Current/Voltage sensor (0x40 - 0x4F)\n"
                "STMPE610/STMPE811 Resistive Touch controller (0x41 or 0x44)";
      case 0x42:
      case 0x43:
         return "HDC1008 Humidity/Temp sensor (0x40, 0x41, 0x42 or 0x43)\n"
                "TMP007 IR Temperature sensor (0x40 - 0x47)\n"
                "TMP006 IR Temperature sensor (0x40 - 0x47)\n"
                "INA219 High-Side DC Current/Voltage sensor (0x40 - 0x4F)";
      case 0x44:
         return "SHT31 Humidity/Temp sensor (0x44 or 0x45 selectable)\n"
                "TMP007 IR Temperature sensor (0x40 - 0x47)\n"
                "TMP006 IR Temperature sensor (0x40 - 0x47)\n"
                "ISL29125 Color Sensor (0x44 only)\n"
                "INA219 High-Side DC Current/Voltage sensor (0x40 - 0x4F)\n"
                "STMPE610/STMPE811 Resistive Touch controller (0x41 or 0x44)";
      case 0x45:
         return "SHT31 Humidity/Temp sensor (0x44 or 0x45 selectable)\n"
                "TMP007 IR Temperature sensor (0x40 - 0x47)\n"
                "TMP006 IR Temperature sensor (0x40 - 0x47)\n"
                "INA219 High-Side DC Current/Voltage sensor (0x40 - 0x4F)";
      case 0x46:
      case 0x47:
         return "TMP007 IR Temperature sensor (0x40 - 0x47)\n"
                "TMP006 IR Temperature sensor (0x40 - 0x47)\n"
                "INA219 High-Side DC Current/Voltage sensor (0x40 - 0x4F)";
      case 0x48:
         return "TMP102 Temperature sensor (0x48 0x49 0x4A or 0x4B)\n"
                "PN532 NFC/RFID reader (0x48 only)\n"
                "ADS1115 4-channel 16-bit ADC (0x48 0x49 0x4A or 0x4B)\n"
                "INA219 High-Side DC Current/Voltage sensor (0x40 - 0x4F)";
      case 0x49:
         return "TSL2561 light sensor (0x29, 0x39 or 0x49)\n"
                "TMP102 Temperature sensor (0x48 0x49 0x4A or 0x4B)\n"
                "ADS1115 4-channel 16-bit ADC (0x48 0x49 0x4A or 0x4B)\n"
                "INA219 High-Side DC Current/Voltage sensor (0x40 - 0x4F)";
      case 0x4A:
      case 0x4B:
         return "TMP102 Temperature sensor (0x48 0x49 0x4A or 0x4B)\n"
                "ADS1115 4-channel 16-bit ADC (0x48 0x49 0x4A or 0x4B)\n"
                "INA219 High-Side DC Current/Voltage sensor (0x40 - 0x4F)";
      case 0x4C:
      case 0x4D:
      case 0x4E:
      case 0x4F:
         return "INA219 High-Side DC Current/Voltage sensor (0x40 - 0x4F)";
      // 0x50 - 0x5F
      case 0x50:
      case 0x51:
         return "MB85RC I2C FRAM (0x50 - 0x57)";
      case 0x52:
         return "MB85RC I2C FRAM (0x50 - 0x57)\n"
                "Nintendo Nunchuck controller (0x52 only";
      case 0x53:
         return "ADXL345 3-axis accelerometer (0x1D or 0x53)\n"
                "MB85RC I2C FRAM (0x50 - 0x57)";
      case 0x54:
      case 0x55:
      case 0x56:
         return "MB85RC I2C FRAM (0x50 - 0x57)";
      case 0x57:
         return "MB85RC I2C FRAM (0x50 - 0x57)\n"
                "MAX3010x Pulse & Oximetry sensor (0x57";
      case 0x58:
         return "TPA2016 I2C-controlled Amplifier (0x58 only";
      case 0x5A:
         return "Si1141/42/43 proximity/ambiant light sensor (used for HRM)\n"
                "MPR121 12-point capacitive touch sensor (0x5A, 0x5B, 0x5C, 0x5D)\n"
                "CCS811 VOC sensor(0x5A or 0x5B)\n"
                "MLX9061x IR temperature sensor (0x5A only)\n"
                "DRV2605 Haptic Motor Driver (0x5A only";
      case 0x5B:
         return "MPR121 12-point capacitive touch sensor (0x5A, 0x5B, 0x5C, 0x5D)\n"
                "CCS811 VOC sensor(0x5A or 0x5B)";
      case 0x5C:
         return "AM2315 Humidity/Temp sensor (0x5C only)\n"
                "MPR121 12-point capacitive touch sensor (0x5A, 0x5B, 0x5C, 0x5D";
      case 0x5D:
         return "MPR121 12-point capacitive touch sensor (0x5A, 0x5B, 0x5C, 0x5D)";
      // 0x60 - 0x6F
      case 0x60:
         return "MPL115A2 Barometric Pressure (0x60 only)\n"
                "MPL3115A2 Barometric Pressure (0x60 only)\n"
                "Si5351A Clock Generator (0x60 or 0x61)\n"
                "Si1145 Light/IR Sensor (0x60 only)\n"
                "MCP4725A0 12-bit DAC (0x60 or 0x61)\n"
                "TEA5767 Radio receiver (0x60 only";
      case 0x61:
         return "Si5351A Clock Generator (0x60 or 0x61)\n"
                "MCP4725A0 12-bit DAC (0x60 or 0x61)";
      case 0x62:
         return "MCP4725A1 12-bit DAC (0x62 or 0x63)";
      case 0x63:
         return "MCP4725A1 12-bit DAC (0x62 or 0x63)\n"
                "Si4713 FM Transmitter with RDS (0x11 or 0x63)";
      case 0x64:
      case 0x65:
         return "MCP4725A2 12-bit DAC (0x64 or 0x65)";
      case 0x66:
      case 0x67:
         return "MCP4725A3 12-bit DAC (0x66 or 0x67)";
      case 0x68:
         return "Almost all RTC use 0x68!\n"
                "AMG8833 IR Thermal Camera Breakout (0x68 or 0x69)\n"
                "DS1307 RTC (0x68 only)\n"
                "PCF8523 RTC (0x68 only)\n"
                "DS3231 RTC (0x68 only)\n"
                "MPU-9250 9-DoF IMU (0x68 or 0x69)\n"
                "MPU-60X0 Accel+Gyro (0x68 or 0x69)\n"
                "ITG3200 Gyro (0x68 or 0x69)";
      case 0x69:
         return "AMG8833 IR Thermal Camera Breakout (0x68 or 0x69)\n"
                "MPU-9250 (0x68 or 0x69)\n"
                "MPU-60X0 Accel+Gyro (0x68 or 0x69)\n"
                "ITG3200 Gyro (0x68 or 0x69)";
      case 0x6A:
         return "L3GD20H gyroscope (0x6A or 0x6B)\n"
                "LSM9DS0 9-axis IMU (0x1D or 0x1E for Accel/Mag, 0x6A or 0x6B for Gyro";
      case 0x6B:
         return "L3GD20H gyroscope (0x6A or 0x6B)\n"
                "LSM9DS0 9-axis IMU (0x1D or 0x1E for Accel/Mag, 0x6A or 0x6B for Gyro)";
      // 0x70 - 0x7F
      case 0x70:
      case 0x71:
      case 0x72:
      case 0x73:
         return "TCA9548 1-to-8 I2C Multiplexer (0x70 - 0x77)\n"
                "HT16K33 LED Matrix Driver (0x70 - 0x77)";
      case 0x74:
         return "IS31FL3731 144-LED CharliePlex driver (0x74 0x75 0x66 or 0x77)\n"
                "TCA9548 1-to-8 I2C Multiplexer (0x70 - 0x77)\n"
                "HT16K33 LED Matrix Driver (0x70 - 0x77)";
      case 0x75:
         return "IS31FL3731 144-LED CharliePlex driver (0x74 0x75 0x66 or 0x77)\n"
                "TCA9548 1-to-8 I2C Multiplexer (0x70 - 0x77)\n"
                "HT16K33 LED Matrix Driver (0x70 - 0x77)";
      case 0x76:
         return "BME280 Temp/Barometric/Humidity (0x76 or 0x77)\n"
                "BMP280 Temp/Barometric (0x76 or 0x77)\n"
                "IS31FL3731 144-LED CharliePlex driver (0x74 0x75 0x66 or 0x77)\n"
                "TCA9548 1-to-8 I2C Multiplexer (0x70 - 0x77)\n"
                "HT16K33 LED Matrix Driver (0x70 - 0x77)\n"
                "MS5607/MS5611 Barometric Pressure (0x76 or 0x77)";
      case 0x77:
         return "BME280 Temp/Barometric/Humidity (0x76 or 0x77)\n"
                "BMP280 Temp/Barometric (0x76 or 0x77)\n"
                "BMP180 Temp/Barometric (0x77 only)\n"
                "BMP085 Temp/Barometric (0x77 only)\n"
                "TCA9548 1-to-8 I2C Multiplexer (0x70 - 0x77)\n"
                "IS31FL3731 144-LED CharliePlex driver (0x74 0x75 0x66 or 0x77)\n"
                "HT16K33 LED Matrix Driver (0x70 - 0x77)\n"
                "BMA180 Accelerometer (0x77 only)\n"
                "MS5607/MS5611 Barometric Pressure (0x76 or 0x77)";
      case 0x78:
      case 0x79:
      case 0x7A:
      case 0x7B:
         return "Reserved for 10-bit I2C Addressing";
      case 0x7C:
      case 0x7D:
      case 0x7E:
      case 0x7F:
         return "Reserved for Future Use";
   }
}
#endif