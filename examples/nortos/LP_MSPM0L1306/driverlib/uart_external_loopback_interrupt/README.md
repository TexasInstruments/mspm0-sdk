## Example Summary

The following example configures the UART at 9600bps to transmit and receive
data. The device should be configured in external loopback mode.
The device remains in SLEEP mode while waiting for an interrupt.

## Peripherals & Pin Assignments

| Peripheral | Pin | Function |
| --- | --- | --- |
| SYSCTL |  |  |
| UART0 | PA9 | RX Pin |
| UART0 | PA8 | TX Pin |
| DEBUGSS | PA20 | Debug Clock |
| DEBUGSS | PA19 | Debug Data In Out |

## BoosterPacks, Board Resources & Jumper Settings

Visit [LP_MSPM0L1306](https://www.ti.com/tool/LP-MSPM0L1306) for LaunchPad information, including user guide and hardware files.

| Pin | Peripheral | Function | LaunchPad Pin | LaunchPad Settings |
| --- | --- | --- | --- | --- |
| PA9 | UART0 | RX | J1_3 | J17 1:2 Connect XDS-110 BackChannel UART on J101<br>J17 2:3 Connect to J1_3<br>J101 7:8 Connect XDS-110 BackChannel UART RX |
| PA8 | UART0 | TX | J1_4 | J16 1:2 Connect XDS-110 BackChannel UART on J101<br>J16 2:3 Connect to J1_4<br>J101 9:10 Connect XDS-110 BackChannel UART TX |
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
To use this application in external loopback mode, place a jumper between PA9
(UART0_RX) and PA8 (UART0_TX).
Compile, load and run the example.
Through the external loopback, the UART will automatically start to transmit
and receive from itself.
