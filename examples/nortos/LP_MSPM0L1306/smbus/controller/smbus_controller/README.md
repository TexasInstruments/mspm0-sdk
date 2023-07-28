## Example Summary

This example implements an SMBus controller using the MSPM0 SMBus software library. This example will send a variety of SMBus commands to an SMBus target (see smb_target00_all_protocols) and record the response. 

## Peripherals & Pin Assignments

| Peripheral | Pin | Function |
| --- | --- | --- |
| SYSCTL |  |  |
| I2C0 | PA10 | I2C Serial Data line (SDA) |
| I2C0 | PA11 | I2C Serial Clock line (SCL) |
| DEBUGSS | PA20 | Debug Clock |
| DEBUGSS | PA19 | Debug Data In Out |

## BoosterPacks, Board Resources & Jumper Settings

Visit [LP_MSPM0L1306](https://www.ti.com/tool/LP-MSPM0L1306) for LaunchPad information, including user guide and hardware files.

| Pin | Peripheral | Function | LaunchPad Pin | LaunchPad Settings |
| --- | --- | --- | --- | --- |
| PA10 | I2C0 | SDA | J4_36 | N/A |
| PA11 | I2C0 | SCL | J2_11/J4_35 | J14 1:2 Connect to J2_11<br>J14 2:3 Connect to J4_35 |
| PA20 | DEBUGSS | SWCLK | J2_13 | J101 15:16 ON: Connect to XDS-110 SWCLK (debug) |
| PA19 | DEBUGSS | SWDIO | J2_17 | J101 13:14 ON: Connect to XDS-110 SWDIO (debug) |

### Low-Power Recommendations
TI recommends to terminate unused pins by setting the corresponding functions to
GPIO and configure the pins to output low or input with internal
pullup/pulldown resistor.

SysConfig allows developers to easily configure unused pins by selecting **Board**→**Configure Unused Pins**.

For more information about jumper configuration to achieve low-power using the
MSPM0 LaunchPad, please visit the [LP-MSPM0L1306 User's Guide](https://www.ti.com/lit/slau869).

## Example Usage

Compile, load and run the example on an LP-MSPM0L1306.

Connect SMBCLK (SCL) and SMBDAT (SDA) to the corresponding SMBCLK and SMBDAT pins on an LP-MSPM0L1306 running the smb_target00_all_protocols example. Ensure that the LaunchPads share a common ground, by connecting a jumper between GND on both LaunchPads.

Each command is sent to target one-by-one. Check response using logic analyzer or set breakpoints to debug.