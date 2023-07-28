## Example Summary

This example implements an SMBus controller using the MSPM0 SMBus software library. This example will send a variety of SMBus commands to an SMBus target (see smb_target00_all_protocols) and record the response. 

## Peripherals & Pin Assignments

| Peripheral | Pin | Function |
| --- | --- | --- |
| GPIOB | PB13 | Standard Output |
| SYSCTL |  |  |
| I2C0 | PA10 | I2C Serial Data line (SDA) |
| I2C0 | PA11 | I2C Serial Clock line (SCL) |
| EVENT |  |  |
| DEBUGSS | PA20 | Debug Clock |
| DEBUGSS | PA19 | Debug Data In Out |

## BoosterPacks, Board Resources & Jumper Settings

Visit [LP_MSPM0G3507](https://www.ti.com/tool/LP-MSPM0G3507) for LaunchPad information, including user guide and hardware files.

| Pin | Peripheral | Function | LaunchPad Pin | LaunchPad Settings |
| --- | --- | --- | --- | --- |
| PB13 | GPIOB | PB13 | J4_35 | N/A |
| PA10 | I2C0 | SDA | J4_34/J26_5 | J21 2:3 Connect to J4_34<br>R62 connects J26_5 (default) |
| PA11 | I2C0 | SCL | J4_33/J26_6 | J22 2:3 Connect to J4_33<br>R63 connects J26_6 (default) |
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

Compile, load and run the example on an LP-MSPM0G3507.

Connect SMBCLK (SCL) and SMBDAT (SDA) to the corresponding SMBCLK and SMBDAT pins on an LP-MSPM0G3507 running the smb_target00_all_protocols example. Ensure that the LaunchPads share a common ground, by connecting a jumper between GND on both LaunchPads.

Each command is sent to target one-by-one. Check response using logic analyzer or set breakpoints to debug.