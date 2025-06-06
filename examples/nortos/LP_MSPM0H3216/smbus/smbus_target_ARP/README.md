## Example Summary

This example implements an SMBus target ARP implementation using the MSPM0 SMBus software library. 
This example will respond to a variety of SMBus ARP commands sent by a SMBus
controller (see smb_controller_ARP example).

## Peripherals & Pin Assignments

| Peripheral | Pin | Function |
| --- | --- | --- |
| GPIOA | PA0 | Standard Output |
| GPIOA | PA1 | Standard Output |
| SYSCTL |  |  |
| TIMG0 |  |  |
| I2C2 | PA24 | I2C Serial Data line (SDA) |
| I2C2 | PA25 | I2C Serial Clock line (SCL) |
| ADC0 | PA27 | ADC12 Channel 0 Pin |
| EVENT |  |  |
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
MSPM0 LaunchPad running the smb_controller_ARP example.
Note that I2C requires pull-up resistors. Internal pull-ups can be enabled in
SysConfig (see datasheet for resistance specification), but external pull-ups
might be required based on I2C speed and capacitance. External pull-ups can be
connected or enabled using the LaunchPad.


Compile, load and run the example.

The target example waits for each command from the SMBus controller and 
processes it.

The ARP example uses the Deafult Device address `0x61` to initiate the ARP command.

User can initiate various ARP commands through Controller example as below

- Prepare to ARP
- Reset Device
- Get UDID (General & Directed)
- Assign Address (General & Directed)

User can check the status of ARP Flags Address Resoved & Address Valid in the SMBus->arpStatus structure

Once Target's Own address is set through ARP, user can continue testing the typical SMBUs commands.

A GPIO connected to LED_DEMO will toggle if Quick Command is received.
A GPIO connected to LED_ERROR is used to signal SMBus errors.
Optionally, an I2C sniffer can be used to observe the SMBus protocol.

