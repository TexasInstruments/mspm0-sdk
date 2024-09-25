## Example Summary

This demo shows how to program a MSPM0 MCU using a LP-MSPM0G3507 as host by BSL SPI interface.

## Peripherals & Pin Assignments
                LP-MSPM0G3507
                   Host
             -----------------
      NRST -|         PB8/PICO|---->
            |         PB7/POCI|<----
            |          PB6/CS0|---->
       <----|PB9/SCLK         |
            |              PB0|-->NRST
       S3-->|PB21         PB16|-->Invoke GPIO
            |                 |
            |              PA0|-->LED
            |             PB27|-->LED
            |                 |

## BoosterPacks, Board Resources & Jumper Settings

- Board resources:
    + PB6 (CS0 SPI Chip Select)
    + PB9 (SPI Serial CLK)
    + PB7 (SPI POCI)
    + PB8 (SPI PICO)
	+ PB0 (GPIO output function to generate NRST signal)
	+ PB16 (GPIO output function to generate BSL invoke signal)
	+ S3 - PB21 (Push button to start the program)
	+ LEDs - PA0/PB27 show state of the code


### Device Migration Recommendations
This project was developed for a superset device included in the LP_MSPM0G3507 LaunchPad. Please
visit the [CCS User's Guide](https://software-dl.ti.com/msp430/esd/MSPM0-SDK/latest/docs/english/tools/ccs_ide_guide/doc_guide/doc_guide-srcs/ccs_ide_guide.html#non-sysconfig-compatible-project-migration)
for information about migrating to other MSPM0 devices.

## Example Usage

Connect the hardware that descriped in the document. Compile, load and run the example.
Push the S3 button to start program MSPM0G3507.
