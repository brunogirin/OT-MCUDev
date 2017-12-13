#! /bin/bash


# load to memory
DEVICE=EFR32FG1PXXXF256
LINK=SWD
SPEED=4000
SCRIPT=test.jlink

JLinkExe -device $DEVICE -if $LINK -speed $SPEED -CommanderScript $SCRIPT
