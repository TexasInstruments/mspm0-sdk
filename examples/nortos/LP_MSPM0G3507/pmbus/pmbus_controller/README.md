## Example Summary

This example acts as a PMBus Controller and sends the below commands to target.

PMBus CMD          | CMD ID |  Write Type | Read Type
-------------------|--------|-------------|-----------
Device ID          | 0xAD   |  -          | Block read
Clear Faults       | 0x03   | Send Byte   | -
Vout Mode          | 0x020  | Write Byte  | Read Byte
Vout OV warn Limit | 0x42   | Write Word  | Read Word
MRF ID             | 0x99   | Block Write | Block Read
Read Vout          | 0x8B   | -           | Read Word
EXT_BYTE_CMD       | 0x01FF | Write Byte  | Read Byte
EXT_WORD_CMD       | 0x02FF | Write Word  | Read Word
Process call Byte  | 0xD0   | Write Word  | Read Word
Process call Block | 0xD1   | Block Write | Block Read

## Peripherals, Pin Functions, MCU Pins, Launchpad Pins
| Peripheral | Function | Pin | Launchpad Pin |
| --- | --- | --- | --- |
| GPIOA | Open-Drain Output | PA0 | Red LED1 |
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

Connect SCL and SDA to the corresponding SCL and SDA pins on the PMBus target.
Note that I2C requires pull-up resistors. Internal pull-ups can be enabled in
SysConfig (see datasheet for resistance specification), but external pull-ups
might be required based on I2C speed and capacitance. External pull-ups can be
connected or enabled using the LaunchPad.

Compile, load and run the example.

Each command is sent to the target one-by-one. Check response using logic analyzer or set breakpoints to debug. If the response from target is not as expected, an LED on launchpad is set. When all commands are sent, an LED on launchpad will toggle.