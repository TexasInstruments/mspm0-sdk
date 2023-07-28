## Example Summary

Note: The use of "Master" and "Slave", along with "MOSI/MISO" terminology is
being considered obsolete. These terms will be replaced with "Controller" and
"Peripheral", and "PICO/POCI" respectively.

The following example configures the SPI as a Controller in internal loopback
mode to transmit and receive data.

## Peripherals & Pin Assignments

| Peripheral | Pin | Function |
| --- | --- | --- |
| SYSCTL |  |  |
| SPI1 | PA17 | SPI SCLK (Clock) |
| SPI1 | PA18 | SPI PICO (Peripheral In, Controller Out) |
| SPI1 | PA16 | SPI POCI (Peripheral Out, Controller In) |
| SPI1 | PA2 | SPI CS0 (Chip Select 0) |
| DEBUGSS | PA20 | Debug Clock |
| DEBUGSS | PA19 | Debug Data In Out |

## BoosterPacks, Board Resources & Jumper Settings

Visit [LP_MSPM0G3507](https://www.ti.com/tool/LP-MSPM0G3507) for LaunchPad information, including user guide and hardware files.

| Pin | Peripheral | Function | LaunchPad Pin | LaunchPad Settings |
| --- | --- | --- | --- | --- |
| PA17 | SPI1 | SCLK | J3_28 | RC Filter R82/C51 not soldered by default |
| PA18 | SPI1 | MOSI | J3_26/J3_29 | J15 2:3 Connect to J3_29<br>R58 connects to J3_26 (default)<br>R23 connects to J101 and J8 for BSL_Invoke (default)<br>J8 OFF Disconnect S1 for BSL_Invoke (with pull-down)<br>J101 17:18 OFF Disconnect from XDS-110 BSL_Invoke<br>R66/R67 for thermistor RT1 not soldered by default |
| PA16 | SPI1 | MISO | J3_29 | J15 1:2 Connect to J3_29 |
| PA2 | SPI1 | CS0 | J28_1 | R8 ROSC to GND<br>Remove ROSC for other functions<br>Solder R9 to connect to J28_1 |
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
Compile, load and run the example. The SPI will automatically start
to transmit and receive data.
The SPI is initialized with the following configuration:
- SPI Controller
- Motorola 4 Wire with Polarity 0, Phase 0
- No parity
- 8 bits per transfer
- MSB first
A SW breakpoint is set in the application to allow the user to check the value
of the received data.
