## Example Summary

This example implements an SMBus target using the MSPM0 SMBus software library. This example will respond to a variety of SMBus commands to an SMBus target (see smb_controller00_all_protocols).

## Peripherals & Pin Assignments

| Peripheral | Pin | Function |
| --- | --- | --- |
| GPIOA | PA0 | Open-Drain Output |
| SYSCTL |  |  |
| TIMG0 |  |  |
| I2C0 | PA10 | I2C Serial Data line (SDA) |
| I2C0 | PA11 | I2C Serial Clock line (SCL) |
| ADC0 | PA27 | ADC12 Channel 0 Pin |
| EVENT |  |  |
| DEBUGSS | PA20 | Debug Clock |
| DEBUGSS | PA19 | Debug Data In Out |

## BoosterPacks, Board Resources & Jumper Settings

Visit [LP_MSPM0G3507](https://www.ti.com/tool/LP-MSPM0G3507) for LaunchPad information, including user guide and hardware files.

| Pin | Peripheral | Function | LaunchPad Pin | LaunchPad Settings |
| --- | --- | --- | --- | --- |
| PA0 | GPIOA | PA0 | J27_9 | This pin is 5V tolerant open-drain and requires pull-up.<br>J4 ON/OFF: Connect/Disconnect LED1<br>J19 1:2 3.3V Pull-up<br>J19 2:3 5V Pull-up |
| PA10 | I2C0 | SDA | J4_34/J26_5 | J21 2:3 Connect to J4_34<br>R62 connects J26_5 (default) |
| PA11 | I2C0 | SCL | J4_33/J26_6 | J22 2:3 Connect to J4_33<br>R63 connects J26_6 (default) |
| PA27 | ADC0 | A0 | J1_8 | J17 OFF Disconnect from Light Sensor D1 |
| PA20 | DEBUGSS | SWCLK | N/A | J101 15:16 ON: Connect to XDS-110 SWCLK (debug) |
| PA19 | DEBUGSS | SWDIO | N/A | J101 13:14 ON: Connect to XDS-110 SWDIO (debug) |

### Low-Power Recommendations
TI recommends to terminate unused pins by setting the corresponding functions to
GPIO and configure the pins to output low or input with internal
pullup/pulldown resistor.

SysConfig allows developers to easily configure unused pins by selecting **Board**→**Configure Unused Pins**.

For more information about jumper configuration to achieve low-power using the
MSPM0 LaunchPad, please visit the [LP-MSPM0G3507 User's Guide](https://www.ti.com/lit/slau846).

## Example Usage

Compile, load and run the example on an LP-MSPM0L1306.

Connect SMBCLK (SCL) and SMBDAT (SDA) to the corresponding SMBCLK and SMBDAT pins on an LP-MSPM0L1306 running the smb_controller00_all_protocols example. Ensure that the LaunchPads share a common ground, by connecting a jumper between GND on both LaunchPads.

Waits for command from the SMBus Controller and processes it.

