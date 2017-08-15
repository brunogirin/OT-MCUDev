
/**
 * Trying to make i2c work with Si7021. This is supposedly software compatible with the SHT21.
 * Steps:
 * - Init i2c peripheral.
 * - Init Si7021
 * - Make temperature reading
 * - Write to serial? (debug semihosting)
 */

extern "C" {  // Simplicity Studio headers. These are all C files.
#include "em_device.h"
#include "em_chip.h"
#include "em_cmu.h"
//#include "em_i2c.h"

//#include "bsp.h"
//#include "i2cspm.h"
//#include "bspconfig.h"

#include "InitDevice.h"  // Not included by default for some reason
}
#include <my_i2cspm.h>

#if 0
#include "sensors.h"

OTV0P2BASE::RoomTemperatureC16_SHT21 si7021;
#endif

void SysTick_Handler()
{

}

static constexpr uint8_t SHT21_I2C_ADDR = 0x40;
//static constexpr uint8_t SHT21_I2C_CMD_TEMP_HOLD   = 0xe3;
//static constexpr uint8_t SHT21_I2C_CMD_TEMP_NOHOLD = 0xf3;
//static constexpr uint8_t SHT21_I2C_CMD_RH_HOLD     = 0xe5;
//static constexpr uint8_t SHT21_I2C_CMD_RH_NOHOLD   = 0xf5;
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
////    if(SHT21_USE_REDUCED_PRECISION) {
//        // Soft reset in order to sample at reduced precision.
//        uint8_t cmd_ureg = SHT21_I2C_CMD_USERREG;
//        I2C_TransferSeq_TypeDef msg;
//        msg.addr = (SHT21_I2C_ADDR << 1);
//        msg.buf[0].data = &cmd_ureg;
//        msg.buf[0].len = 1;
//        msg.flags = I2C_FLAG_WRITE;
//        auto write1 = I2CSPM_transfer(i2c, msg); // initMsg
//
//        // ????
//        uint8_t rxMsg = 0;
//        msg.buf[0].data = &rxMsg;
//        msg.buf[0].len = 1;
//        msg.flags = I2C_FLAG_READ;
//        auto read = I2CSPM_transfer(i2c, msg);
//        const uint8_t curUR = rxMsg;
//
//        // Preserve reserved bits (3, 4, 5) and sample 8-bit RH (for for 1%) and 12-bit temp (for 1/16C).
//        const uint8_t newUR = (curUR & 0x38) | 3;
//        uint8_t configMsg[2] = { SHT21_I2C_CMD_USERREG, newUR};
////        auto write2 = I2C::i2c.write(SHT21_I2C_ADDR, configMsg, sizeof(configMsg)); // setupMsg
//    }
//    uint8_t SI7021_CMD_READ_ID_BYTE2[2] =  {0xFC, 0xC9};
    uint8_t cmd[1] = { SHT21_I2C_CMD_USERREG };
    uint8_t result[8];
    I2C_TransferSeq_TypeDef seq;
    I2C_TransferReturn_TypeDef ret;

    seq.addr = SHT21_I2C_ADDR << 1;

//    if( cmdLen > 0 ) {

       seq.flags = I2C_FLAG_WRITE;

       seq.buf[0].data = cmd;
       seq.buf[0].len  = sizeof(cmd);
       seq.buf[1].data = result;
       seq.buf[1].len  = sizeof(result);
//    }
//    else {
//
//       seq.flags = I2C_FLAG_READ;
//
//       seq.buf[0].data = result;
//       seq.buf[0].len  = resultLen;
//    }

 ret = I2CSPM_Transfer( i2c, &seq );


    SHT21_initialised = true;
}

int main(void)
{
    I2CSPM_Init_TypeDef i2cInit = I2CSPM_INIT_DEFAULT;  // I2C settings
  /* Chip errata */
    enter_DefaultMode_from_RESET(); // This calls CHIP_Init() anyway

    SysTick_Config(CMU_ClockFreqGet(cmuClock_CORE) / 10);  // ???

#if 0
    I2C::i2c.init(i2cInit);  // Initialise I2C peripheral
#endif
    GPIO_PinOutSet(SI7021_CE_PORT, SI7021_CE_PIN);  // Power up Si7021

//    for (auto i = 0; i < 100000; ++i) {}
#if 0
    const int readVal = si7021.read();
#endif

    I2CSPM_Init(&i2cInit);
    for (auto i = 0; i < 1000000; ++i) {}
    SHT21_init(i2cInit.port);

  /* Infinite loop */
  while (1) {

  }
}
