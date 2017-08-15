/*
 * i2cspmconfig.h
 *
 *  Created on: 11 Aug 2017
 *      Author: denzo
 */

#ifndef SRC_MY_I2CSPMCONFIG_H_
#define SRC_MY_I2CSPMCONFIG_H_
#if 1
/***************************************************************************//**
 * @file
 * @brief I2C simple poll-based master mode driver for the DK/STK.
 * @version 5.2.2
 *******************************************************************************
 * # License
 * <b>Copyright 2015 Silicon Labs, Inc. http://www.silabs.com</b>
 *******************************************************************************
 *
 * This file is licensed under the Silabs License Agreement. See the file
 * "Silabs_License_Agreement.txt" for details. Before using this software for
 * any purpose, you must agree to the terms of that agreement.
 *
 ******************************************************************************/
extern "C" {
#include "em_gpio.h"
#include "em_i2c.h"
}

/***************************************************************************//**
 * @defgroup kitdrv Kit Drivers
 * @brief Kit support and drivers
 * @details
 *  Drivers and support modules for board components such as displays, sensors and
 *  memories on EFM32, EZR32 and EFR32 kits.
 *
 * @{
 ******************************************************************************/

/***************************************************************************//**
 * @addtogroup I2CSPM
 * @{
 ******************************************************************************/


/*******************************************************************************
 ********************************   STRUCTS   **********************************
 ******************************************************************************/

/** I2C driver instance initialization structure.
    This data structure contains a number of I2C configuration options
    required for driver instance initialization.
    This struct is passed to @ref I2CSPM_Init() when initializing a I2CSPM
    instance. */
typedef struct
{
  I2C_TypeDef           *port;          /**< Peripheral port */
  GPIO_Port_TypeDef     sclPort;        /**< SCL pin port number */
  uint8_t               sclPin;         /**< SCL pin number */
  GPIO_Port_TypeDef     sdaPort;        /**< SDA pin port number */
  uint8_t               sdaPin;         /**< SDA pin number */
#if defined (_I2C_ROUTELOC0_MASK)
  uint8_t               portLocationScl; /**< Port location of SCL signal */
  uint8_t               portLocationSda; /**< Port location of SDA signal */
#else
  uint8_t               portLocation;   /**< Port location */
#endif
  uint32_t              i2cRefFreq;     /**< I2C reference clock */
  uint32_t              i2cMaxFreq;     /**< I2C max bus frequency to use */
  I2C_ClockHLR_TypeDef  i2cClhr;        /**< Clock low/high ratio control */
} I2CSPM_Init_TypeDef;


/** Default config for I2C init structure. The default may be overridden
    by a i2cspmconfig.h file. */
//#if !defined( I2CSPM_INIT_DEFAULT )
/* I2C SPM driver config. This default override only works if one I2C interface
   is in use. If multiple interfaces are in use, define the peripheral setup
   inside the application in a I2CSPM_Init_TypeDef and then pass the initialization
   struct to I2CSPM_Init(). */
// TODO NOTE! the locations seem to be very important. What are they?
#define I2CSPM_INIT_DEFAULT                                                   \
  { I2C0,                      /* Use I2C instance 0 */                       \
    gpioPortC,                 /* SCL port */                                 \
    10,                        /* SCL pin */                                  \
    gpioPortC,                 /* SDA port */                                 \
    11,                        /* SDA pin */                                  \
    14,                        /* Location of SCL */                          \
    16,                        /* Location of SDA */                          \
    0,                         /* Use currently configured reference clock */ \
    I2C_FREQ_STANDARD_MAX,     /* Set to standard rate  */                    \
    i2cClockHLRStandard,       /* Set to use 4:4 low/high duty cycle */       \
  }

//#endif

static constexpr uint32_t I2CSPM_TRANSFER_TIMEOUT = 300000;
/*******************************************************************************
 *****************************   PROTOTYPES   **********************************
 ******************************************************************************/

void I2CSPM_Init(I2CSPM_Init_TypeDef *init);
I2C_TransferReturn_TypeDef I2CSPM_Transfer(I2C_TypeDef *i2c, I2C_TransferSeq_TypeDef *seq);
#endif
#endif /* SRC_MY_I2CSPMCONFIG_H_ */
