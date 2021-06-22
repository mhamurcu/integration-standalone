/*
  SoftwareI2C.h
  2012 Copyright (c) Seeed Technology Inc.  All right reserved.

  This is a Software I2C Library, can act as I2c master mode.
  
  @author  Loovee
  @version 1.0
  @date    01-Nov-2013

  @author  Mark Cooke
  @version 1.1
  @date    01-Sep-2017
  @brief   uchar -> uint8_t, standardise interface to better match Wire
  
  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/

#ifndef _SOFTWAREI2C_H_
#define _SOFTWAREI2C_H_
#include <stdint.h>
#include <Arduino.h>
#ifdef __AVR__
#include <Arduino.h>
#include <util/delay_basic.h>
// tunedDelay for AVR uses _delay_loop_2() which delays for 4 clock cycles.
// We pass the delay in us, so we need to scale this by 4*F_CPU in seconds, multiplied by 1e6 to get us
// The only problem, is that normally F_CPU is 16MHz, so we get 0.25 which is truncated to 0 with the #define
// The solution is to invert and divide the required delay by this value
#define _avr_bit_delay_scaling (F_CPU/4*1000000)
#endif

class SoftwareI2C
{
private:
    uint8_t _pinSda;
    uint8_t _pinScl;

    RingBuffer rxBuffer;
    RingBuffer txBuffer;

    uint16_t _half_bit_delay_us;

    uint8_t _i2c_address;

    bool _transmissionBegun;
    
    inline uint8_t sdaGet();
    inline uint8_t quickSdaGet();

    inline void sdaSet(uint8_t ucDta);
    inline void sclSet(uint8_t ucDta);

    inline void sendStart(void);
    inline void sendStop(void);
    inline uint8_t getAck(void);
    inline void sendByte(uint8_t ucDta);
    inline uint8_t sendByteAck(uint8_t ucDta);  // send byte and get ack
    // Note : tunedDelay should be used to throttle comms by appropriately delaying the SCL toggling
    //        However the I2C devices i work with are tolerant to non-standard rates, so this is not implemented
#ifdef __AVR__
    inline void tunedDelay(uint16_t delay_us) { _delay_loop_2(delay_us / _avr_bit_delay_scaling); }
#else
    inline void tunedDelay(uint16_t delay_us)
    {
      // TODO: Measure how long digitalWrite, pinMode etc take and compensate by this
      // maybe use ___attribute__( ( always_inline ) ) static __INLINE void __NOP(void) { __ASM volatile ("nop"); }
      // to create a _delay_loop_2() ?
      delayMicroseconds(delay_us);
    }
#endif
    
public:
    SoftwareI2C(uint8_t pinSda = 7, uint8_t pinScl = 8) : _pinScl(pinScl), _pinSda(pinSda), _transmissionBegun(false), _i2c_address(0x3C) { setClock(100000); }
    void init(uint8_t pinSda = 7, uint8_t pinScl = 8) { _pinSda = pinSda; _pinScl = pinScl; }
    void begin();
    void beginTransmission(uint8_t addr);
    uint8_t endTransmission(bool stopBit = false);
    void setClock(uint32_t frequency_Hz);
    void end() {}
    
    size_t write(uint8_t dta);
    size_t write(uint8_t *dta, size_t len);
    uint8_t requestFrom(uint8_t register_address, size_t len);
    int read() { return rxBuffer.read_char(); }
    int available() { return rxBuffer.available(); }
    int peek() { return rxBuffer.peek(); }
    void flush() {} // Do nothing
};

#endif
/*********************************************************************************************************
  END FILE
*********************************************************************************************************/
