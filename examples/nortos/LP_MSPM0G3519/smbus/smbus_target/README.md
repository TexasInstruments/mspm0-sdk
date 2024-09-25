## Example Summary

This example implements an SMBus target using the MSPM0 SMBus software library. 
This example will respond to a variety of SMBus commands sent by a SMBus
controller (see smb_controller00_all_protocols).

## Peripherals, Pin Functions, MCU Pins, Launchpad Pins
| Peripheral | Function | Pin | Launchpad Pin |
| --- | --- | --- | --- |
| GPIOB | Standard Output | PB22 | LED2 Blue |
| GPIOB | Standard Output | PB26 | LED2 Red |
| I2C2 | I2C Serial Data line (SDA) | PC3 | J1_10 |
| I2C2 | I2C Serial Clock line (SCL) | PC2 | J1_9 |
| ADC0 | ADC12 Channel 0 Pin | PA27 | J3_23 |
| BOARD | Debug Clock | PA20 | J19_16 |
| BOARD | Debug Data In Out | PA19 | J19_14 |

### Device Migration Recommendations
This project was developed for a superset device included in the LP_MSPM0G3519 LaunchPad. Please
visit the [CCS User's Guide](https://software-dl.ti.com/msp430/esd/MSPM0-SDK/latest/docs/english/tools/ccs_ide_guide/doc_guide/doc_guide-srcs/ccs_ide_guide.html#manual-migration)
for information about migrating to other MSPM0 devices.

### Low-Power Recommendations
TI recommends to terminate unused pins by setting the corresponding functions to
GPIO and configure the pins to output low or input with internal
pullup/pulldown resistor.

SysConfig allows developers to easily configure unused pins by selecting **Board**→**Configure Unused Pins**.

For more information about jumper configuration to achieve low-power using the
MSPM0 LaunchPad, please visit the [LP-MSPM0G3519 web page](https://www.ti.com/tool/LP-MSPM0G3519).

## Example Usage

Connect SMBCLK (SCL), SMBDAT (SDA) and GND to the corresponding pins on a second
MSPM0 LaunchPad running the smb_controller00_all_protocols example.
Note that I2C requires pull-up resistors. Internal pull-ups can be enabled in
SysConfig (see datasheet for resistance specification), but external pull-ups
might be required based on I2C speed and capacitance. External pull-ups can be
connected or enabled using the LaunchPad.

Compile, load and run the example.

The target example waits for each command from the SMBus controller and 
processes it.
A GPIO connected to LED_DEMO will toggle if Quick Command is received.
A GPIO connected to LED_ERROR is used to signal SMBus errors.
Optionally, an I2C sniffer can be used to observe the SMBus protocol.
