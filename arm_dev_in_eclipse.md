# Setting up Eclipse for ARM dev
Based on [Breathing with Oxygen: DIY ARM Cortex-M C/C++ IDE and Toolchain with Eclipse Oxygen](https://mcuoneclipse.com/2017/07/30/breathing-with-oxygen-diy-arm-cortex-m-cc-ide-and-toolchain-with-eclipse-oxygen/)

1. Download:
    - eclipse c/c++
    - [gcc-arm-non-eabi](https://developer.arm.com/open-source/gnu-toolchain/gnu-rm/downloads)
1. Add [GNU MCU Eclipse plugin.](https://gnu-mcu-eclipse.github.io/)
1. Add gcc-arm-non-eabi to tool path.
    - Window>Preferences>MCU>Global ARM Toolchains Path>Build tools folder
    - Should point to bin directory with all the binaries in it.
1. Install "J-Link Software and Documentation Pack" from [J-Link download page.](https://www.segger.com/downloads/jlink/).


# Using JLink
## Connecting to target (e.g. EFR32FG1P133F256GM48)
```
JLinkExe:
J-Link>connect
Device> <device>
TIF>SWD
Speed> <leave blank>

Connecting to target via SWD
Found SW-DP with ID 0x2BA01477
Found SW-DP with ID 0x2BA01477
Scanning AP map to find all available APs
AP[1]: Stopped AP scan as end of AP map has been reached
AP[0]: AHB-AP (IDR: 0x24770011)
Iterating through AP map to find AHB-AP to use
AP[0]: Core found
AP[0]: AHB-AP ROM base: 0xE00FF000
CPUID register: 0x410FC241. Implementer code: 0x41 (ARM)
Found Cortex-M4 r0p1, Little endian.
FPUnit: 6 code (BP) slots and 2 literal slots
CoreSight components:
ROMTbl[0] @ E00FF000
ROMTbl[0][0]: E000E000, CID: B105E00D, PID: 000BB00C SCS-M7
ROMTbl[0][1]: E0001000, CID: B105E00D, PID: 003BB002 DWT
ROMTbl[0][2]: E0002000, CID: B105E00D, PID: 002BB003 FPB
ROMTbl[0][3]: E0000000, CID: B105E00D, PID: 003BB001 ITM
ROMTbl[0][4]: E0040000, CID: B105900D, PID: 003BB923 TPIU-Lite
Cortex-M4 identified.
```

## Loading code
```
J-Link>erase
Erasing device (EFR32FG1PxxxF256)...
Erasing flash     [100%] Done.
Verifying flash   [100%] Done.
J-Link: Flash download: Total time needed: 3.396s (Prepare: 0.052s, Compare: 0.000s, Erase: 3.307s, Program: 0.000s, Verify: 0.000s, Restore: 0.035s)
Erasing done.
J-Link>loadbin <path-to-bin>, 0
Downloading file [path-to-bin]...
Comparing flash   [100%] Done.
Erasing flash     [100%] Done.
Programming flash [100%] Done.
Verifying flash   [100%] Done.
J-Link: Flash download: Bank 0 @ 0x00000000: 1 range affected (10240 bytes)
J-Link: Flash download: Total time needed: 0.132s (Prepare: 0.037s, Compare: 0.003s, Erase: 0.000s, Program: 0.071s, Verify: 0.000s, Restore: 0.020s)
O.K.
J-Link>r
Reset delay: 0 ms
Reset type NORMAL: Resets core & peripherals via SYSRESETREQ & VECTRESET bit.
Reset: Halt core after reset via DEMCR.VC_CORERESET.
Reset: Reset device via AIRCR.SYSRESETREQ.
J-Link>g
J-Link>
```

## Using a script
- JLink.exe on Windows.
```
CLI command:
------------
JLinkExe -device <Device> -if SWD -speed 4000 -CommanderScript=<path-to-file>/test.jlink

test.jlink:
-----------
h
loadbin <path-to-bin>/test.bin,0
r
g
```




