## Example Summary

This example implements an SMBus controller using the MSPM0 SMBus software 
library. This example will send a variety of SMBus commands to an SMBus target 
(see smb_target00_all_protocols) and record the response. 

## Peripherals & Pin Assignments

| Peripheral | Pin | Function |
| --- | --- | --- |
| SYSCTL |  |  |
| I2C2 | PA24 | I2C Serial Data line (SDA) |
| I2C2 | PA25 | I2C Serial Clock line (SCL) |
| DEBUGSS | PA20 | Debug Clock |
| DEBUGSS | PA19 | Debug Data In Out |

### Device Migration Recommendations
This project was developed for a superset device included in the LP_MSPM0H3216 LaunchPad. Please
visit the [CCS User's Guide](https://software-dl.ti.com/msp430/esd/MSPM0-SDK/latest/docs/english/tools/ccs_ide_guide/doc_guide/doc_guide-srcs/ccs_ide_guide.html#sysconfig-project-migration)
for information about migrating to other MSPM0 devices.

### Low-Power Recommendations
TI recommends to terminate unused pins by setting the corresponding functions to
GPIO and configure the pins to output low or input with internal
pullup/pulldown resistor.

SysConfig allows developers to easily configure unused pins by selecting **Board**→**Configure Unused Pins**.

For more information about jumper configuration to achieve low-power using the
MSPM0 LaunchPad, please visit the [LP-MSPM0H3216 web page](https://www.ti.com/tool/LP-MSPM0H3216).

## Example Usage

Connect SMBCLK (SCL), SMBDAT (SDA) and GND to the corresponding pins on a second
MSPM0 LaunchPad running the smb_target00_all_protocols example.
Note that I2C requires pull-up resistors. Internal pull-ups can be enabled in
SysConfig (see datasheet for resistance specification), but external pull-ups
might be required based on I2C speed and capacitance. External pull-ups can be
connected or enabled using the LaunchPad.

Compile, load and run the example.

Each command is sent to target one-by-one in the following order:
* Quick CMD W
* Send Byte (0x03)
* Receive Byte
* Write Byte (0x12)
* Read Byte (0x22)
* Read Word16 (0x30)
* Read Word32 (0x31)
* Read Word64 (0x32)
* Process Call (0x40)
* Block Write (0x50)
* Block Read (0x51)
* Block Process Call (0x60)

Check response using logic analyzer or set breakpoints to debug.