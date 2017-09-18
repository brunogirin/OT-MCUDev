/*
The OpenTRV project licenses this file to you
under the Apache Licence, Version 2.0 (the "Licence");
you may not use this file except in compliance
with the Licence. You may obtain a copy of the Licence at

http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing,
software distributed under the Licence is distributed on an
"AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
KIND, either express or implied. See the Licence for the
specific language governing permissions and limitations
under the Licence.

Author(s) / Copyright (s): Deniz Erbilgin 2017
                           Damon Hart-Davis 2017
*/

/**
 * @brief    Simple app testing running cpp code on the EFR wireless dev kit
 *          (SLWSTK6061A).
 *           Flashes LED0 and turns LED1 on when PB0 is pressed.
 *             Pins:
 *             Function | PORT | PIN NAME
 *             LED0     | F    | PF4
 *             LED1     | F    | PF5
 *             PB0      | F    | PF6
 *             PB1      | F    | PF7
 * @note    Creating a HW Configurator project requires at least the following
 *          to be installed:
 *              - MCU 5.x
 *              - Flex SDK
 *              - GCC 4.9+
 *          Additionally, Simplicity Studio needs to be coaxed into downloading
 *          the relevant HW Configurator files for your chip. Not sure how but
 *          it worked eventually...
 * @note    To make the project a cpp project:
 *          - Create a normal HW configurator project.
 *          - Open Project>Convert>Convert to MCU or C/C++ project.
 *          - Select the check box next to the project you wish to convert.
 *          - Select "C++ Project" under "Convert to C/C++ project".
 *          - Click "Finish"
 *          - Rename "main.c" to "main.cpp".
 *          - In "main.cpp" add extern "C" guards around the default #includes.
 *          - Add '#include "InitDevice.h"' to the list of #includes, inside
 *            the guards.
 *          - Replace CHIP_Init() with enter_DefaultMode_from_RESET().
 *
 */

extern "C" {  // Simplicity Studio headers. These are all C files.
#include "em_device.h"
#include "em_chip.h"
#include "InitDevice.h"  // Not included by default for some reason
}

/**
 * @brief   testing turning a pin on/off with templates
 */
template<GPIO_Port_TypeDef port, unsigned int pin>
void setLED(bool on) {
    if(on) GPIO_PinOutClear(port, pin);  // LEDs are connected to Vcc
    else GPIO_PinOutSet(port, pin);
}

auto readPB0() {  // Testing cpp features
    return (bool)GPIO_PinInGet(PB0_PORT, PB0_PIN);
}

int main(void)
{
  /* Chip errata */
    enter_DefaultMode_from_RESET(); // This calls CHIP_Init() anyway

  /* Infinite loop */
  while (1) {
      GPIO_PinOutToggle(LED0_PORT, LED0_PIN);
      for(uint32_t i = 0; i < 100000; ++i) {
          if(readPB0()) setLED<LED1_PORT, LED1_PIN>(true);
          else setLED<LED1_PORT, LED1_PIN>(false);
      }
  }
}
