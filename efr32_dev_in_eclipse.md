# Open Source Eclipse Dev Environment for EFR32FG1
Hardware configuration still requires SimplicityStudio.

Before following this guide, you should follow the steps in [arm_dev_in_eclipse](arm_dev_in_eclipse.md), with the following modification:
- Simplicity Studio V4 is bundled with gcc v4.9q3 so you should download and install that instead of the latest arm-non-eabi-gcc package.
- I couldn't find that on the official website and had to download it from the Cannonical Launchpad repo instead.


**NOTE:** PART A lists the steps required to get an existing project compiling. The rest of the steps document the process of making a working project from scratch.

## PART A: Getting the SDKs
- Install the required SDKs in Simplicity Studio:
    - MCU SDK 5.3+ (confusingly, it's refered to as "gecko sdk suite 2.0" after being downloaded.)
    - RAILS SDK (for radio. untested.)
- Copy ARM toolchain into `$ECLIPSE_HOME/toolchain`
    - Need arm-non-eabi-gcc v4.9q3 (20171212)
- Softlink gecko sdk into `$ECLIPSE_HOME/gecko_sdk_2`
    - Original location: `SimplicityStudio/v4/developer/sdks/gecko_sdk_suite/v2.0`

## PART B: Creating a project from scratch.
1. Make a Simplicity Studio MCU Configurator project.
1. Setup peripherals with the hardware configurator and compile.
1. Make a new GNU MCU C++ project on Eclipse.
1. Copy the source files from the Simplicity Studio project.
1. Apply settings.


* Copy <part-name>.ld into Debug dir.
* For OTRadioLink: Create a new config using the cross compiler toolchain
    - Need to exclude CMSIS and emlibs folders from main compilation.

### Eclipse settings for EFR32FG1P133F256GM48
Just getting it to match Simplicity Debug config at this stage.

#### In project properties:
##### Common Settings
- C/C++ General>File Types
    - Check `Use Project Settings`
    - Add `*.s` as an `Assembly Source File`
- C/C++ Build>Settings>Tool Settings
    - Target Processor
        - ARM family: `cortex-m4`
        - Float ABI: `Library with FP` (move to hard float or just disable completely?)
    - Optimization (following checked)
        - Message length
        - 'char' is signed
        - Function sections
        - Data sections#include "em_device.h"
        - Disable builtin
    - Debugging
        - Other debugging flags: `-gdwarf-2`
- C/C++ Build>Settings>Tool Settings>GNU ARM Cross XXX Compiler>Includes
    - C and C++ compiler only.
    - Not required for assembler
```
${eclipse_home}/gecko_sdk_v2/hardware/kit/common/bsp
${eclipse_home}/gecko_sdk_v2/hardware/kit/common/drivers
${eclipse_home}/gecko_sdk_v2/platform/CMSIS/Include
${eclipse_home}/gecko_sdk_v2/platform/emlib/inc
${eclipse_home}/gecko_sdk_v2/platform/Device/SiliconLabs/EFR32FG1P/Include
${eclipse_home}/gecko_sdk_v2/platform/halconfig/inc/hal-config
```

- Defined Symbols:

| Symbol | Assembler | C Compiler | C++ Compiler|
| :--- | | --- | --- | --- |
| HAL_CONFIG=1 | Y | Y | Y |
| EFR32FG1P133F256GM48=1 | Y | Y | Y |
| DEBUG=1 | N | Y | Y |


##### GNU ARM Cross Assembler
- No change


##### GNU ARM Cross C Compiler
- Optimization
    - Language standard: `GNU ISO C11`
- Mischellaneous
    - Other compiler flags: `-mno-sched-prolog`

##### GNU ARM Cross C++ Compiler
- Optimization
    - Language standard:`-std=c++14`
- Mischellaneous
    - Other compiler flags: `-mno-sched-prolog`

##### GNU ARM Cross C++ Linker
- General
    - Script files (-T) `EFR32FG1P133F256GM48.ld`
    - check `Remove unused sections`
- Libraries
    - Any libraries
- Miscellaneous
    - check `Use newlib-nano`
    - check `Do not use syscalls`

**Note:** As an alternative to providing the libraries under the `Libraries` setting, you can pass them in as `user objects` under the `Miscellaneous` settings tab. This will cause Eclipse to try to compile them automatically before compiling the application. See [Managing Project and Library Dependencies with Eclipse CDT](https://mcuoneclipse.com/2017/09/19/managing-project-and-library-dependencies-with-eclipse-cdt/)

##### Build Artifact
- Artifact extension `axf`


