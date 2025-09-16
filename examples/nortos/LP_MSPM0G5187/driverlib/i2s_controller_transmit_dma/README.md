## Example Summary

I2S Controller example which uses DMA to output a sine wave onto an audio data
pin. When connected to an audio amplifier board (TAS5805EVM for example), which
has a speaker hooked up, a continuous tone will play from the speaker.

This example can be used to validate I2S communication, as a starting point to
enable I2S functionality, or to create custom drivers.

## Peripherals, Pin Functions, MCU Pins, Launchpad Pins
| Peripheral | Function | MCU Pin | Launchpad Pin | Launchpad Settings |
| --- | --- | --- | --- | --- |
TODO

### Device Migration Recommendations
This project was developed for a superset device included in the MSPM0 LaunchPad. Please
visit the [CCS User's Guide](https://software-dl.ti.com/msp430/esd/MSPM0-SDK/latest/docs/english/tools/ccs_ide_guide/doc_guide/doc_guide-srcs/ccs_ide_guide.html#sysconfig-project-migration)
for information about migrating to other MSPM0 devices.

### Low-Power Recommendations
TI recommends to terminate unused pins by setting the corresponding functions to
GPIO and configure the pins to output low or input with internal
pullup/pulldown resistor.

SysConfig allows developers to easily configure unused pins by selecting **Board**→**Configure Unused Pins**.

For more information about jumper configuration to achieve low-power using the
MSPM0 LaunchPad, please visit the LP-MSPM0G5187 User's Guide.

## Example Usage
TODO: The Example Usage section is a rough draft intended only for developers.
It will go over connecting the LaunchPad to the TAS5805EVM and how to use the
TAS5805EVM configuration GUI, also known as PurePath Console 3. Again:

FOR INTERNAL TEAM USE ONLY!!

See this Confluence page: https://confluence.itg.ti.com/display/MSPsw/Using+TAS5805EVM+for+A1_USB+I2S+Audio
