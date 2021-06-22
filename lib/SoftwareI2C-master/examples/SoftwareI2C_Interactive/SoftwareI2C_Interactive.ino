//#define USE_SOFTWAREI2C
#include "SerialCommands.h"
/*
Note : Serial Commands requires the following libraries
#include <SerialCommand.h> // https://github.com/kroimon/Arduino-SerialCommand
#include <Vector.h> // https://github.com/zacsketches/Arduino_Vector
*/

void setup(void)
{
   Serial.begin(9600);
   Serial.println(__FILE__);
   Serial.println(__TIME__);
   Serial.println("SDA,SCL = 24,25");

   digitalWrite(13, HIGH);
   pinMode(13, OUTPUT);

#ifdef USE_SOFTWAREI2C
   sCmd.addCommand("P", setI2CPins);
#endif
   sCmd.addCommand("R", readDecCommand);
   sCmd.addCommand("RX", readHexCommand);
   sCmd.addCommand("W", writeDecCommand);
   sCmd.addCommand("WX", writeHexCommand);
   sCmd.addCommand("S", scanCommand);
   sCmd.setDefaultHandler(unrecognized);

   i2c_begin();

   unrecognized(NULL);
}

void loop(void) { sCmd.readSerial(); }
