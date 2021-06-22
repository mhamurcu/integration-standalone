/*
  SoftwareI2C.cpp
  2012 Copyright (c) Seeed Technology Inc.  All right reserved.

  Author:Loovee
  Author:Loovee

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

#include <Arduino.h>

#include "SoftwareI2C.h"

void SoftwareI2C::setClock(uint32_t frequency_Hz)
{
    if (frequency_Hz == 0)
    {
        _half_bit_delay_us = 0;
        return;
    }
    else if (frequency_Hz <= 50000)
    {
      frequency_Hz = 500000;
    }
    else if (frequency_Hz <= 100000)
    {
      frequency_Hz = 100000;
    }
    else if (frequency_Hz <= 250000)
    {
      frequency_Hz = 250000;
    }
    else
    {
      frequency_Hz = 400000;
    }


    // The half bit delay is half the period
    _half_bit_delay_us = (1000000 / frequency_Hz)/2;
 }

/*************************************************************************************************
 * Function Name: begin
 * Description:  config IO
 * Parameters: Sda: Scl:
 * Return: None
*************************************************************************************************/
void SoftwareI2C::begin()
{
    digitalWrite(_pinScl, HIGH);
    digitalWrite(_pinSda, HIGH);

    pinMode(_pinScl, OUTPUT);
    pinMode(_pinSda, OUTPUT);
}
/*************************************************************************************************
 * Function Name: sdaGet
 * Description:  get sda digital value, toggling between INPUT and OUTPUT
 * Parameters: None
 * Return: HIGH or LOW
*************************************************************************************************/
uint8_t SoftwareI2C::sdaGet()
{
    pinMode(_pinSda, INPUT);
    uint8_t value = digitalRead(_pinSda);
    pinMode(_pinSda, OUTPUT);
    return value;
}

/*************************************************************************************************
 * Function Name: quickSdaGet
 * Description:  get sda digital value
 * Parameters: None
 * Return: HIGH or LOW
*************************************************************************************************/
uint8_t SoftwareI2C::quickSdaGet()
{
    return digitalRead(_pinSda);
}

/*************************************************************************************************
 * Function Name: sdaSet
 * Description:  set sda digital value
 * Parameters: ucDta: HIGH or LOW
 * Return: None
*************************************************************************************************/
void SoftwareI2C::sdaSet(uint8_t ucDta)
{
    digitalWrite(_pinSda, ucDta);
}

/*************************************************************************************************
 * Function Name: sclSet
 * Description:  set scl digital value
 * Parameters: ucDta: HIGH or LOW
 * Return: None
*************************************************************************************************/
void SoftwareI2C::sclSet(uint8_t ucDta)
{
    digitalWrite(_pinScl, ucDta);
}

/*************************************************************************************************
 * Function Name: getAck
 * Description:  get ack
 * Parameters: None
 * Return: 0 – Nak; 1 – Ack
*************************************************************************************************/
uint8_t SoftwareI2C::getAck(void)
{
    sclSet(HIGH);
    tunedDelay(_half_bit_delay_us/2); // sample in the middle of the pulse
    uint8_t Ack = sdaGet(); // get ack
    sclSet(LOW);
    
    return Ack;
}

/*************************************************************************************************
 * Function Name: sendStart
 * Description:   send start clock
 * Parameters: None
 * Return: None
*************************************************************************************************/
void SoftwareI2C::sendStart(void)
{
    if (sdaGet() == LOW)
    {
        sclSet(HIGH);
        tunedDelay(_half_bit_delay_us);
        sdaSet(HIGH);
        //tunedDelay(_half_bit_delay_us);
    }
    sdaSet(LOW);
    tunedDelay(_half_bit_delay_us);
    sclSet(LOW);
    //tunedDelay(_half_bit_delay_us);
}

/*************************************************************************************************
 * Function Name: sendStop
 * Description:  send stop clock
 * Parameters: None
 * Return: None
*************************************************************************************************/
void SoftwareI2C::sendStop(void)
{
    sdaSet(LOW);
    sclSet(LOW);
    tunedDelay(_half_bit_delay_us);
    sclSet(HIGH);
    tunedDelay(_half_bit_delay_us);
    sdaSet(HIGH);
    //tunedDelay(_half_bit_delay_us);
}

/*************************************************************************************************
 * Function Name: sendByte
 * Description:  send a byte
 * Parameters: ucDta: data to send
 * Return: None
*************************************************************************************************/
void SoftwareI2C::sendByte(uint8_t ucDta)
{
    for(uint8_t i=0; i<8; ++i)
    {
        sdaSet((ucDta&0x80)!=0);
        sclSet(HIGH);
        tunedDelay(_half_bit_delay_us);
        sclSet(LOW);
        tunedDelay(_half_bit_delay_us);
        ucDta <<= 1;
    }
}

/*************************************************************************************************
 * Function Name: sendByteAck
 * Description:  send a byte and get ack signal
 * Parameters: ucDta: data to send
 * Return: 0: get ack  1: get nak
*************************************************************************************************/
uint8_t SoftwareI2C::sendByteAck(uint8_t ucDta)
{
    sendByte(ucDta);
    return getAck();
}

/*************************************************************************************************
 * Function Name: beginTransmission
 * Description:  send begin signal
 * Parameters: divider – clock divider
 * Return: None
*************************************************************************************************/
void SoftwareI2C::beginTransmission(uint8_t addr)
{
    _i2c_address = addr;
    _transmissionBegun = true;
    sendStart();                  // start signal
    write(_i2c_address<<1);
}

/*************************************************************************************************
 * Function Name: endTransmission
 * Description:  triggers sending the write commands and sends the stop signal
 * Parameters: None
 * Return: 0: get ack  1: get nak
*************************************************************************************************/
uint8_t SoftwareI2C::endTransmission(bool stopBit)
{
    uint8_t _success = 0;
    if (_transmissionBegun == true)
    {
        while (txBuffer.available())
        {
            _success = sendByteAck(txBuffer.read_char());
        }
        
        sendStop();
        _transmissionBegun = false;
    }
    
    return _success;
}

/*************************************************************************************************
 * Function Name: write
 * Description:  send a byte
 * Parameters: dta: data to send
 * Return: 0: fail  1: success
*************************************************************************************************/  
size_t SoftwareI2C::write(uint8_t dta)
{
    // No writing, without begun transmission or a full buffer
    if ( !_transmissionBegun || txBuffer.isFull() )
    {
        return 0;
    }

    txBuffer.store_char( dta ) ;
    return 1;
}

/*************************************************************************************************
 * Function Name: write
 * Description:  write array
 * Parameters: len - length of the array
               *dta - array to be sent
 * Return: number of bytes stored for write
*************************************************************************************************/
size_t SoftwareI2C::write(uint8_t *dta, size_t len)
{
  //Try to store all the data
  for(size_t i = 0; i < len; ++i)
  {
    //Return the number of data stored, when the buffer is full (if write return 0)
    if(!write(dta[i]))
    {
      return i;
    }
  }

  //All data stored
  return len;
}

/*************************************************************************************************
 * Function Name: requestFrom
 * Description:  request data from slave
 * Parameters: address - slave i2c address
               len  - length of request
 * Return: number of bytes stored for read
*************************************************************************************************/
uint8_t SoftwareI2C::requestFrom(uint8_t address, size_t len)
{
    _i2c_address = address;

    sendStart();                             // start signal
    // send register address, 0x01 = read access, get ack
    uint8_t nak = sendByteAck((_i2c_address<<1) | 0x01);  

    size_t byteRead = 0;

    // read and store the data from the slave
    for (byteRead = 0; byteRead < len; byteRead++)
    {
        pinMode(_pinSda, INPUT);
        uint8_t ucRt = 0;
        for(uint8_t b=0; b<8; ++b)
        {
            sclSet(HIGH);
            tunedDelay(_half_bit_delay_us / 2); // sample in the middle of the clock pulse
            ucRt = (ucRt << 1) + quickSdaGet();
            sclSet(LOW);
            //tunedDelay(_half_bit_delay_us);
        }

        // store the byte
        rxBuffer.store_char(ucRt);

        // if last packet send NAK, otherwise send ACK
        bool last_packet = (byteRead +1 == len);

        sdaSet(last_packet);
        pinMode(_pinSda, OUTPUT);
        //sdaSet(last_packet);
        sclSet(HIGH);
        tunedDelay(_half_bit_delay_us);
        sclSet(LOW);
        //tunedDelay(_half_bit_delay_us);
    }
    sdaSet(LOW);
    sendStop();
    
    // validate the return using the sendByteAck
    return byteRead * (!nak);//ret;
}

/*********************************************************************************************************
  END FILE
*********************************************************************************************************/