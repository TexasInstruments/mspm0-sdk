## Example Summary

This example shows the implementation of SMBus Host Notify protocol for a
controller device using the MSPM0 SMBus software library.
The controller sends a WRITE_BYTE command (0x12) when a LaunchPadbutton is
pressed.
The device will also respond to Host Notify packets sent by the SMBus target. 
This target example is intended to be used with the target example 
smbus_target_host_notify.

## Peripherals, Pin Functions, MCU Pins, Launchpad Pins
| Peripheral | Function | Pin | Launchpad Pin |
| --- | --- | --- | --- |
| GPIOA | Open-Drain Output | PA0 | Red LED1 |
| GPIOB | Standard Input with internal pull-up | PB3 | S2 |
| I2C2 | I2C Serial Data line (SDA) | PC3 | J1_10 |
| I2C2 | I2C Serial Clock line (SCL) | PC2 | J1_9 |
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
MSPM0 LaunchPad running the smbus_target_host_notify example.
Note that I2C requires pull-up resistors. Internal pull-ups can be enabled in
SysConfig (see datasheet for resistance specification), but external pull-ups
might be required based on I2C speed and capacitance. External pull-ups can be
connected or enabled using the LaunchPad.

Compile, load and run the example.

Press button to send command 0x12 to the SMBus target. The data value will
toggle between 0x00 and 0x01 causing the target's LED to toggle.
The controller example will also wait for Host Notify packets and toggle the 
LaunchPad LED if a packet is received.

Check response using logic analyzer or set breakpoints to debug.