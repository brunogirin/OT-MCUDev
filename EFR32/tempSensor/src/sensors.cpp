/*
 * sensors.cpp
 *
 *  Created on: 11 Aug 2017
 *      Author: denzo
 */

#include "sensors.h"
#include <my_i2cspm.h>

namespace OTV0P2BASE
{
#if 0

static constexpr uint8_t SHT21_I2C_ADDR = 0x40;
static constexpr uint8_t SHT21_I2C_CMD_TEMP_HOLD   = 0xe3;
static constexpr uint8_t SHT21_I2C_CMD_TEMP_NOHOLD = 0xf3;
static constexpr uint8_t SHT21_I2C_CMD_RH_HOLD     = 0xe5;
static constexpr uint8_t SHT21_I2C_CMD_RH_NOHOLD   = 0xf5;
static constexpr uint8_t SHT21_I2C_CMD_USERREG     = 0xe7; // User register...

// If defined, sample 8-bit RH (for 1%) and 12-bit temp (for 1/16C).
// Reduced precision should save time and energy.
static constexpr bool SHT21_USE_REDUCED_PRECISION = true;

// Set true once SHT21 has been initialised.
static volatile bool SHT21_initialised;

// Initialise/configure SHT21, usually once only.
// TWI must already be powered up.
static void SHT21_init(I2C_TypeDef *i2c)
{
    uint8_t cmd_ureg = SHT21_I2C_CMD_USERREG;
    if(SHT21_USE_REDUCED_PRECISION) {
        I2C_TransferSeq_TypeDef i2cTXMsg;
        i2cTXMsg.addr = SHT21_I2C_ADDR;
        i2cTXMsg.flags = I2C_FLAG_WRITE;
        i2cTXMsg.buf[0].data = &cmd_ureg;
        i2cTXMsg.buf[0].len = 1;

        uint8_t rxMsg;
        I2C_TransferSeq_TypeDef i2cRXMsg;
        i2cRXMsg.addr = SHT21_I2C_ADDR;
        i2cTXMsg.flags = I2C_FLAG_READ;
        i2cTXMsg.buf[0].data = &rxMsg;
        i2cTXMsg.buf[0].len = 1;


        // Soft reset in order to sample at reduced precision.
        i2cTransfer(i2c, i2cTXMsg); // initMsg

        // ????
        i2cTransfer(i2c, i2cRXMsg);
        const uint8_t curUR = rxMsg;

        // Preserve reserved bits (3, 4, 5) and sample 8-bit RH (for for 1%) and 12-bit temp (for 1/16C).
        const uint8_t newUR = (curUR & 0x38) | 3;
        uint8_t configMsg[2] = { SHT21_I2C_CMD_USERREG, newUR};
        i2cTXMsg.buf[0].data = configMsg;
        i2cTXMsg.buf[0].len = 2;
        i2cTransfer(i2c, i2cTXMsg); // setupMsg
    }
    SHT21_initialised = true;
}

// Measure and return the current ambient temperature in units of 1/16th C.
// This may contain up to 4 bits of information to RHS of the fixed binary point.
// This may consume significant power and time.
// Probably no need to do this more than (say) once per minute.
// The first read will initialise the device as necessary
// and leave it in a low-power mode afterwards.
int16_t RoomTemperatureC16_SHT21::read()
  {
#if 0
  const bool neededPowerUp = OTV0P2BASE::powerUpTWIIfDisabled();
#endif

  // Initialise/config if necessary.
  if(!SHT21_initialised) { SHT21_init(); }

  // Max RH measurement time:
  //   * 14-bit: 85ms
  //   * 12-bit: 22ms
  //   * 11-bit: 11ms
  // Use blocking data fetch for now.
  Wire.beginTransmission(SHT21_I2C_ADDR);
  Wire.write((byte) SHT21_I2C_CMD_TEMP_HOLD); // Select control register.
#if 0
  if(SHT21_USE_REDUCED_PRECISION)
    // Should cover 12-bit conversion (22ms).
    { OTV0P2BASE::nap(WDTO_30MS); }
  else
    // Should be plenty for slowest (14-bit) conversion (85ms).
    { OTV0P2BASE::sleepLowPowerMs(90); }
#endif
  Wire.endTransmission();
  Wire.requestFrom(SHT21_I2C_ADDR, 3U);
  while(Wire.available() < 3)
    {
#if 0
      // Wait for data, but avoid rolling over the end of a minor cycle...
    if(OTV0P2BASE::getSubCycleTime() >= OTV0P2BASE::GSCT_MAX - 2)
      { return(DEFAULT_INVALID_TEMP); } // Failure value: may be able to to better.
#endif
    }
  uint16_t rawTemp = (Wire.read() << 8);
  rawTemp |= (Wire.read() & 0xfc); // Clear status ls bits.

#if 0
  // Power down TWI ASAP.
  if(neededPowerUp) { OTV0P2BASE::powerDownTWI(); }
#endif
  // Nominal formula: C = -46.85 + ((175.72*raw) / (1L << 16));
  // FIXME: find a good but faster approximation...
  // FIXME: should the shift/division be rounded to nearest?
  // FIXME: break out calculation and unit test against example in datasheet.
  const int16_t c16 = -750 + int16_t((5623 * int_fast32_t(rawTemp)) >> 17);

#if 0
  // Capture entropy if (transformed) value has changed.
  // Claim one bit of noise in the raw value if the full value has changed,
  // though it is possible that this might be manipulatable by Eve,
  // and nearly all of the raw info is visible in the result.
  if(c16 != value) { addEntropyToPool((uint8_t)rawTemp, 1); }
#endif
  value = c16;
  return(c16);
  }

#endif
}


