/***************************************************************************//**
 * @file
 * @brief I2C simple poll-based master mode driver for the DK/STK.
 * @version 5.1.1
 *******************************************************************************
 * @section License
 * <b>Copyright 2015 Silicon Labs, Inc. http://www.silabs.com</b>
 *******************************************************************************
 *
 * This file is licensed under the Silabs License Agreement. See the file
 * "Silabs_License_Agreement.txt" for details. Before using this software for
 * any purpose, you must agree to the terms of that agreement.
 *
 ******************************************************************************/

#include <stddef.h>
#include "my_i2cspm.h"

extern "C" {
#include "em_cmu.h"
#include "em_gpio.h"
#include "em_i2c.h"
#include "em_assert.h"
}
/*******************************************************************************
 **************************   GLOBAL FUNCTIONS   *******************************
 ******************************************************************************/

/***************************************************************************//**
 * @brief
 *   Initalize I2C peripheral
 *
 * @details
 *   This driver supports master mode only, single bus-master. In addition
 *   to configuring the I2C peripheral module, it also configures DK/STK
 *   specific setup in order to use the I2C bus.
 *
 * @param[in] init
 *   Pointer to I2C initialization structure
 ******************************************************************************/
void I2CSPM::init(I2CSPM_Init_TypeDef &config)
{
    i2cConfig = config;
  int i;
  CMU_Clock_TypeDef i2cClock;
  I2C_Init_TypeDef i2cInit;

  CMU_ClockEnable(cmuClock_HFPER, true);

  /* Select I2C peripheral clock */
  if (false)
  {
#if defined( I2C0 )
  }
  else if (i2cConfig.port == I2C0)
  {
    i2cClock = cmuClock_I2C0;
#endif
#if defined( I2C1 )
  }
  else if (i2cConfig.port == I2C1)
  {
    i2cClock = cmuClock_I2C1;
#endif
  }
  else
  {
    /* I2C clock is not defined */
    EFM_ASSERT(false);
    return;
  }
  CMU_ClockEnable(i2cClock, true);

  /* Output value must be set to 1 to not drive lines low. Set
     SCL first, to ensure it is high before changing SDA. */
  GPIO_PinModeSet(i2cConfig.sclPort, i2cConfig.sclPin, gpioModeWiredAndPullUp, 1);
  GPIO_PinModeSet(i2cConfig.sdaPort, i2cConfig.sdaPin, gpioModeWiredAndPullUp, 1);

  /* In some situations, after a reset during an I2C transfer, the slave
     device may be left in an unknown state. Send 9 clock pulses to
     set slave in a defined state. */
  for (i = 0; i < 9; i++)
  {
    GPIO_PinOutSet(i2cConfig.sclPort, i2cConfig.sclPin);
    GPIO_PinOutClear(i2cConfig.sclPort, i2cConfig.sclPin);
  }

  /* Enable pins and set location */
#if defined (_I2C_ROUTEPEN_MASK)
  i2cConfig.port->ROUTEPEN  = I2C_ROUTEPEN_SDAPEN | I2C_ROUTEPEN_SCLPEN;
  i2cConfig.port->ROUTELOC0 = (i2cConfig.portLocationSda << _I2C_ROUTELOC0_SDALOC_SHIFT)
                          | (i2cConfig.portLocationScl << _I2C_ROUTELOC0_SCLLOC_SHIFT);
#else
  i2cConfig.port->ROUTE = I2C_ROUTE_SDAPEN |
                      I2C_ROUTE_SCLPEN |
                      (i2cConfig.portLocation << _I2C_ROUTE_LOCATION_SHIFT);
#endif

  /* Set emlib init parameters */
  i2cInit.enable = true;
  i2cInit.master = true; /* master mode only */
  i2cInit.freq = i2cConfig.i2cMaxFreq;
  i2cInit.refFreq = i2cConfig.i2cRefFreq;
  i2cInit.clhr = i2cConfig.i2cClhr;

  I2C_Init(i2cConfig.port, &i2cInit);
}


/***************************************************************************//**
 * @brief
 *   Perform I2C transfer
 *
 * @details
 *   This driver only supports master mode, single bus-master. It does not
 *   return until the transfer is complete, polling for completion.
 *
 * @param[in] i2c
 *   Pointer to the peripheral port
 *
 * @param[in] seq
 *   Pointer to sequence structure defining the I2C transfer to take place. The
 *   referenced structure must exist until the transfer has fully completed.
 ******************************************************************************/
I2C_TransferReturn_TypeDef I2CSPM::transfer(I2C_TransferSeq_TypeDef &seq)
{
  I2C_TransferReturn_TypeDef ret;
  uint32_t timeout = I2CSPM_TRANSFER_TIMEOUT;
  /* Do a polled transfer */
  ret = I2C_TransferInit(i2cConfig.port, &seq);
  while (ret == i2cTransferInProgress && timeout--)
  {
    ret = I2C_Transfer(i2cConfig.port);
  }
  return ret;
}

/**
 * @brief   Blocking read from an i2c device
 */
I2C_TransferReturn_TypeDef I2CSPM::read(uint16_t addr, uint8_t *buf, uint16_t len)
{
    // Check buf not null, that we will actually write to it.
    if((nullptr == buf) || (0 == len)) { return i2cTransferUsageFault; }
    I2C_TransferSeq_TypeDef seq;
    seq.addr = addr;
    seq.flags = I2C_FLAG_READ;
    seq.buf[0].data = buf;
    seq.buf[0].len = len;
    return transfer(seq);
}
/**
 * @brief   Blocking write to an i2c device
 */
I2C_TransferReturn_TypeDef I2CSPM::write(uint16_t addr, uint8_t *buf, uint16_t len)
{
    // Check buf not null, that we will actually write to it.
    if((nullptr == buf) || (0 == len)) { return i2cTransferUsageFault; }

    I2C_TransferSeq_TypeDef seq;
    seq.addr = addr;
    seq.flags = I2C_FLAG_WRITE;
    seq.buf[0].data = buf;
    seq.buf[0].len = len;
    return transfer(seq);
}
