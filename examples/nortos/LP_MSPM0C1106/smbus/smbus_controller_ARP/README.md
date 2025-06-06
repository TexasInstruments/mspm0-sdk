## Example Summary

This example implements SMBus controller Host ARP commands using the MSPM0 SMBus software 
library. This example will send a variety of SMBus ARP commands to an SMBus target 
(see smb_target_ARP) and record the response. 

## Peripherals & Pin Assignments

| Peripheral | Pin | Function |
| --- | --- | --- |
| SYSCTL |  |  |
| I2C1 | PA10 | I2C Serial Data line (SDA) |
| I2C1 | PA11| I2C Serial Clock line (SCL) |
| DEBUGSS | PA20 | Debug Clock |
| DEBUGSS | PA19 | Debug Data In Out |

### Device Migration Recommendations
This project was developed for a superset device included in the LP_MSPM0C1106 LaunchPad. Please
visit the [CCS User's Guide](https://software-dl.ti.com/msp430/esd/MSPM0-SDK/latest/docs/english/tools/ccs_ide_guide/doc_guide/doc_guide-srcs/ccs_ide_guide.html#sysconfig-project-migration)
for information about migrating to other MSPM0 devices.

### Low-Power Recommendations
TI recommends to terminate unused pins by setting the corresponding functions to
GPIO and configure the pins to output low or input with internal
pullup/pulldown resistor.

SysConfig allows developers to easily configure unused pins by selecting **Board**→**Configure Unused Pins**.

For more information about jumper configuration to achieve low-power using the
MSPM0 LaunchPad, please visit the [LP-MSPM0C1106 web page](https://www.ti.com/tool/LP-MSPM0C1106).

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