extern "C" {
#include "em_device.h"
#include "em_chip.h"
#include "em_cmu.h"

#include "hal-config.h"
}

#include "i2c_driver.h"

#include "OTV0P2BASE_Serial_IO.h"
#include "OTV0P2BASE_Util.h"
#include "OTV0P2BASE_Sleep.h"
#include "OTV0P2BASE_SensorSHT21.h"

#include "util.h"



#define DEBUG_BREAK __asm__("BKPT #0");

// Default speed is 38 MHz so div2 must be enabled.
constexpr auto F_CPU = 19000000U;
// extern OTV0P2BASE::EFR32Print Serial;

constexpr auto SI7021_CE_PORT = gpioPortD;
constexpr auto SI7021_CE_PIN = 15;
OTV0P2BASE::RoomTemperatureC16_SHT21 tempC16;
OTV0P2BASE::HumiditySensorSHT21 humidity;

extern "C" {
// SysTick emulates subcycleTime
void SysTick_Handler(void) {
    OTV0P2BASE::tickSubCycle();
}
}

int main(void)
{
    I2CSPM_Init_TypeDef i2cInit = I2CSPM_INIT_DEFAULT;  // I2C settings
    /* Chip errata */
    CHIP_Init();

    if(OTV0P2BASE::setupEmulated2sSubCycle()) { DEBUG_BREAK; }

    // Setup clocks
    OTPORT::setupClocks();
    
    // LED setup
    OTPORT::setupGPIO();
    auto ledFlashed = false;
    auto ledState = true;
    OTPORT::setPin<gpioPortF, 4>(ledState);
    OTPORT::setPin<gpioPortF, 5>(!ledState);

    // Setup UART
    OTV0P2BASE::Serial.setup(9600);

    i2c0.init(i2cInit);


    /* Infinite loop */
    while (1) {
        
        // Poll subCycleTime and delay 1 ms
        const auto subCycleTime = OTV0P2BASE::getSubCycleTime();
        if (0U == subCycleTime) { ledFlashed = false; }
        else if((255U == subCycleTime) && (!ledFlashed)) {
            ledFlashed = true;
            ledState = !ledState;
            OTPORT::setPin<gpioPortF, 4>(ledState);
            OTPORT::setPin<gpioPortF, 5>(!ledState);

            // test i2c
            {
                const auto value = tempC16.read();
                OTV0P2BASE::Serial.print(value);
            }
            {
                const auto value = humidity.read();
                OTV0P2BASE::Serial.print(value);
            }
        }

    }
}
