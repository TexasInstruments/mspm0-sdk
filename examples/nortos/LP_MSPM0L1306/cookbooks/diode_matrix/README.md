## Example Summary

Uses 6 GPIO pins to control a 3 by 3 common row cathode diode matrix. See block diagram for setup and signal connections.

## Peripherals & Pin Assignments

| Peripheral | Pin | Function |
| --- | --- | --- |
| GPIOA | PA5 | High-Speed Output |
| GPIOA | PA4 | Standard Output |
| GPIOA | PA3 | Standard Output |
| GPIOA | PA12 | Standard Output |
| GPIOA | PA13 | Standard Output |
| GPIOA | PA27 | Standard Output |
| SYSCTL |  |  |
| TIMG0 |  |  |
| EVENT |  |  |
| DEBUGSS | PA20 | Debug Clock |
| DEBUGSS | PA19 | Debug Data In Out |

## BoosterPacks, Board Resources & Jumper Settings

Visit [LP_MSPM0L1306](https://www.ti.com/tool/LP-MSPM0L1306) for LaunchPad information, including user guide and hardware files.

| Pin | Peripheral | Function | LaunchPad Pin | LaunchPad Settings |
| --- | --- | --- | --- | --- |
| PA5 | GPIOA | PA5 | J2_15 | N/A |
| PA4 | GPIOA | PA4 | J2_14 | N/A |
| PA3 | GPIOA | PA3 | J2_19 | N/A |
| PA12 | GPIOA | PA12 | J4_40 | N/A |
| PA13 | GPIOA | PA13 | J4_39 | J3 ON/OFF: Connect/Disconnect LED2 Green |
| PA27 | GPIOA | PA27 | J4_37 | J13 ON/OFF: Connect/Disconnect LED2 Blue |
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
Connect column and row pins to diode matrix. Compile, load and run the example.
"T" and "I" will flash on repeat.