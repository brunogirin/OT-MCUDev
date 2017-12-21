extern "C" {
#include "em_device.h"
#include "em_chip.h"
#include "em_cmu.h"
#include "em_gpio.h"
#include "em_adc.h"


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

// Pin/port to connect serial to USB VCOM on dev board.
constexpr auto VCOM_ENABLE_PORT = gpioPortA;
constexpr auto VCOM_ENABLE_PIN = 5;

// SI7021 is drop in replacement for SHT21 (ie has electrical and protocal compatibility).
constexpr auto SI7021_CE_PORT = gpioPortD;
constexpr auto SI7021_CE_PIN = 15;
OTV0P2BASE::RoomTemperatureC16_SHT21 tempC16;
OTV0P2BASE::HumiditySensorSHT21 humidity;

extern "C" {
// SysTick emulates subcycleTime
void SysTick_Handler(void) {
    OTV0P2BASE::tickSubCycle();
}

// void ADC0_IRQHandler(void)
// {
//   ADC_IntClear(ADC0, ADC_IF_SINGLE);
// }
}


void setupADC()
{
    //// ADC
    // HFPERCLK 
    CMU_ClockEnable(cmuClock_ADC0, true);

    /* Base the ADC configuration on the default setup. */
    ADC_Init_TypeDef init = ADC_INIT_DEFAULT;
    ADC_InitSingle_TypeDef sInit = ADC_INITSINGLE_DEFAULT;

    /* Initialize timebases */
    init.timebase = ADC_TimebaseCalc(0);
    init.prescale = ADC_PrescaleCalc(400000, 0);
    ADC_Init(ADC0, &init);

    /* Set input to temperature sensor. Reference must be 1.25V */
    sInit.reference = adcRefVDD;
    //sInit.posSel = adcPosSelTEMP;
    sInit.posSel = adcPosSelAPORT1YCH19;
    sInit.negSel = adcNegSelVSS;
    ADC_InitSingle(ADC0, &sInit);

    /* Setup interrupt generation on completed conversion. */
    //ADC_IntEnable(ADC0, ADC_IF_SINGLE);
    //NVIC_EnableIRQ(ADC0_IRQn);
}

uint32_t adcRead()
{
    // Start a conversion
    ADC_Start(ADC0, adcStartSingle);
    // wait for conversion to finish
    while ((ADC0->STATUS & ADC_STATUS_SINGLEDV) == 0);
    return (ADC_DataSingleGet(ADC0));
}


int main(void)
{
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

    // Engage VCOM on dev board
    // GPIO_PinModeSet(VCOM_ENABLE_PORT, VCOM_ENABLE_PIN, gpioModePushPull, 1);  // Couldn't get this to work.

    i2c0.init();

    // Power up SI7021 (it's connected via an analogue switch on the dev board)
    GPIO_PinModeSet(SI7021_CE_PORT, SI7021_CE_PIN, gpioModePushPull, 1);

    setupADC();


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

            // // test i2c
            // {
            //     const auto value = tempC16.read();
            //     OTV0P2BASE::Serial.print(value);
            // }
            // {
            //     const auto value = humidity.read();
            //     OTV0P2BASE::Serial.print(value);
            // }


            OTV0P2BASE::Serial.print(adcRead());
		    
        }

    }
}
