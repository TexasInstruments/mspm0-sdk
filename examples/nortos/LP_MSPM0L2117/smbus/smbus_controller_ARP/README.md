## Example Summary

This example implements SMBus controller Host ARP commands using the MSPM0 SMBus software 
library. This example will send a variety of SMBus ARP commands to an SMBus target 
(see smb_target_ARP) and record the response. 

## Peripherals & Pin Assignments

| Peripheral | Pin | Function |
| --- | --- | --- |
| SYSCTL |  |  |
| UC6 | I2C Serial Data line (SDA) | PA10 | J3_30 |
| UC6 | I2C Serial Clock line (SCL) | PA11 | J3_31 |
| DEBUGSS | PA20 | Debug Clock |
| DEBUGSS | PA19 | Debug Data In Out |

## BoosterPacks, Board Resources & Jumper Settings

Visit [LP_MSPM0L2117](https://www.ti.com/tool/LP-MSPM0L2117) for LaunchPad information, including user guide and hardware files.

| Pin | Peripheral | Function | LaunchPad Pin | LaunchPad Settings |
| --- | --- | --- | --- | --- |
| PA10 | UC6 | SDA | J3_30 | <ul><li>PA10 can be connected to an on-board pull-up resistor<br><ul><li>`R60` is not soldered by default<br><li>Solder `R60` to use on-board pull-up</ul></ul> |
| PA11 | UC6 | SCL | J3_31 | <ul><li>PA11 can be connected to an on-board pull-up resistor<br><ul><li>`R59` is not soldered by default<br><li>Solder `R59` to use on-board pull-up</ul></ul> |
| PA20 | DEBUGSS | SWCLK | N/A | <ul><li>PA20 is used by SWD during debugging<br><ul><li>`J101 15:16 ON` Connect to XDS-110 SWCLK while debugging<br><li>`J101 15:16 OFF` Disconnect from XDS-110 SWCLK if using pin in application</ul></ul> |
| PA19 | DEBUGSS | SWDIO | N/A | <ul><li>PA19 is used by SWD during debugging<br><ul><li>`J101 13:14 ON` Connect to XDS-110 SWDIO while debugging<br><li>`J101 13:14 OFF` Disconnect from XDS-110 SWDIO if using pin in application</ul></ul> |

### Device Migration Recommendations
This project was developed for a superset device included in the LP_MSPM0L2117 LaunchPad. Please
visit the [CCS User's Guide](https://software-dl.ti.com/msp430/esd/MSPM0-SDK/latest/docs/english/tools/ccs_ide_guide/doc_guide/doc_guide-srcs/ccs_ide_guide.html#sysconfig-project-migration)
for information about migrating to other MSPM0 devices.

### Low-Power Recommendations
TI recommends to terminate unused pins by setting the corresponding functions to
GPIO and configure the pins to output low or input with internal
pullup/pulldown resistor.

SysConfig allows developers to easily configure unused pins by selecting **Board**→**Configure Unused Pins**.

For more information about_ jumper configuration to achieve low-power using the
MSPM0 LaunchPad, please visit the [LP-MSPM0L2117 User's Guide](https://www.ti.com/lit/slau873).

## Example Usage

Connect SMBCLK (SCL), SMBDAT (SDA) and GND to the corresponding pins on a second
MSPM0 LaunchPad running the smb_target_ARP example.
Note that I2C requires pull-up resistors. Internal pull-ups can be enabled in
SysConfig (see datasheet for resistance specification), but external pull-ups
might be required based on I2C speed and capacitance. External pull-ups can be
connected or enabled using the LaunchPad.

Compile, load and run the example.

Specific ARP command can be sent to the target using `sendARPCommand`. The specific ARP command can be selected using `cmd` variable.

Below are the available commands for testing. All of the command format definitions are implemented as part of SMBus 3.2 specification

- Prepare to ARP
- Reset Device
- Get UDID
- Assign Address
- Get UDID Direct
- Reset Direct

Check response using logic analyzer or set breakpoints to debug.