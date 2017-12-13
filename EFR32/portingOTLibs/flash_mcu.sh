#! /bin/bash

# Script for flashing firmware to an ARM device with a Segger J-Link.
#
# Notes on the jlink script:
# - It should be located in the folder that this script is run from.
# - The script will:
#   1. Halt the MCU.
#   2. Erase the MCU and flash a binary.
#   3. Reset the MCU.
#   4. Start the MCU
#   5. Exit JLinkExe
#   
# The binary can be changed by setting the string after the loadbin command in
# the .jlink script.
# i.e. loadbin <path to binary>,0


DEVICE=EFR32FG1PXXXF256 # device name. 
LINK=SWD  # Link type. JTAG or SWD. SWD is prefered.
SPEED=4000  # The default.
SCRIPT=flash_mcu.jlink 

JLinkExe -device $DEVICE -if $LINK -speed $SPEED -CommanderScript $SCRIPT
