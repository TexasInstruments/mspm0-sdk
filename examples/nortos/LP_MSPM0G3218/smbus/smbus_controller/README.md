## Example Summary

This example implements an SMBus controller using the MSPM0 SMBus software 
library. This example will send a variety of SMBus commands to an SMBus target 
(see smb_target00_all_protocols) and record the response. 

## Peripherals & Pin Assignments

| Peripheral | Pin | Function |
| --- | --- | --- |
| SYSCTL |  |  |
| UC0 | I2C Serial Data line (SDA) | PA10 |
| UC0 | I2C Serial Clock line (SCL) | PA11 |
| DEBUGSS | PA20 | Debug Clock |
| DEBUGSS | PA19 | Debug Data In Out |

## BoosterPacks, Board Resources & Jumper Settings

Visit [LP_MSPM0G3218](https://www.ti.com/tool/LP-MSPM0G3218) for LaunchPad information, including user guide and hardware files.

| Pin | Peripheral | Function | LaunchPad Pin | LaunchPad Settings |
| --- | --- | --- | --- | --- |
| PA10 | UC0 | SDA | J4_34 | <ul><li>PA10 can be connected to XDS-110 backchannel or boosterpack connector:<br><ul><li>To use on boosterpack connector J4_34 and disconnect XDS-110:<br> `J21 2:3 ON` </ul></ul> |
| PA11 | UC0 | SCL | J4_33 | <ul><li>PA11 can be connected to XDS-110 backchannel or boosterpack connector:<br><ul><li>To use on boosterpack connector J4_33 and disconnect XDS-110:<br> `J22 2:3 ON` </ul></ul> |
| PA20 | DEBUGSS | SWCLK | N/A | <ul><li>PA20 is used by SWD during debugging<br><ul><li>`J101 15:16 ON` Connect to XDS-110 SWCLK while debugging<br><li>`J101 15:16 OFF` Disconnect from XDS-110 SWCLK if using pin in application</ul></ul> |
| PA19 | DEBUGSS | SWDIO | N/A | <ul><li>PA19 is used by SWD during debugging<br><ul><li>`J101 13:14 ON` Connect to XDS-110 SWDIO while debugging<br><li>`J101 13:14 OFF` Disconnect from XDS-110 SWDIO if using pin in application</ul></ul> |

### Device Migration Recommendations
This project was developed for a superset device included in the LP_MSPM0G3218 LaunchPad. Please
visit the [CCS User's Guide](https://software-dl.ti.com/msp430/esd/MSPM0-SDK/latest/docs/english/tools/ccs_ide_guide/doc_guide/doc_guide-srcs/ccs_ide_guide.html#sysconfig-project-migration)
for information about migrating to other MSPM0 devices.

### Low-Power Recommendations
TI recommends to terminate unused pins by setting the corresponding functions to
GPIO and configure the pins to output low or input with internal
pullup/pulldown resistor.

SysConfig allows developers to easily configure unused pins by selecting **Board**→**Configure Unused Pins**.

For more information about_ jumper configuration to achieve low-power using the
MSPM0 LaunchPad, please visit the [LP-MSPM0G3218 User's Guide](https://www.ti.com/lit/slvuda6).

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