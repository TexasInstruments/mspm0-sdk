## Example Summary
This example demonstrates how to receive CAN Classic and CAN-FD
messages using standard IDs.

## Peripherals & Pin Assignments

| Peripheral | Pin | Function |
| --- | --- | --- |
| GPIOB | PB22 | Standard Output |
| GPIOB | PB26 | Standard Output |
| GPIOB | PB13 | Standard Output |
| SYSCTL | PA5 | HFXIN |
| SYSCTL | PA6 | HFXOUT |
| EVENT |  |  |
| CANFD0 | PA13 | RX Pin |
| CANFD0 | PA12 | TX Pin |
| DEBUGSS | PA20 | Debug Clock |
| DEBUGSS | PA19 | Debug Data In Out |

## BoosterPacks, Board Resources & Jumper Settings

Visit [LP_MSPM0G3507](https://www.ti.com/tool/LP-MSPM0G3507) for LaunchPad information, including user guide and hardware files.

| Pin | Peripheral | Function | LaunchPad Pin | LaunchPad Settings |
| --- | --- | --- | --- | --- |
| PB22 | GPIOB | PB22 | J27_5 | J5 ON/OFF Connect/Disconnect LED2 Blue |
| PB26 | GPIOB | PB26 | J27_8 | J6 ON/OFF Connect/Disconnect LED2 Red |
| PB13 | GPIOB | PB13 | J4_35 | N/A |
| PA5 | SYSCTL | HFXIN | J28_4 | Connected to HFXT  Y2<br>Solder R15 to connect to J28_4 |
| PA6 | SYSCTL | HFXOUT | J28_5 | Connected to HFXT  Y2<br>Solder R17 to connect to J28_5 |
| PA13 | CANFD0 | CANRX | J4_32/J26_3 | R65 connects J26_3 for CAN (default) |
| PA12 | CANFD0 | CANTX | J4_32/J26_1 | R64 connects J26_1 for CAN (default) |
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
Connect the CAN_RX and CAN_TX to TCAN1042DEVM pins as described in TCAN
Evaluation Module User's Guide and monitor and transmit CAN-FD packages using
mcan_single_message_tx example project or using a CAN Bus Analyzer tool of your
choice.

The example has the following Nominal and Data Rate frequencies configured:
- Nominal Bit Rate (250 KBits/s)
- Data Bit Rate    (2 MBits/s)

Compile, load and run the example.

To turn on the LED_1, the example must receive the following package:
- ID : 0x03
- DATA[0] = 0x01

To turn off the LED_1, the example must receive the following package:
- ID : 0x03
- DATA[0] = 0x00

To turn on the LED_2, the example must receive the following package:
- ID : 0x04
- DATA[0] = 0x01

To turn off the LED_2, the example must receive the following package:
- ID : 0x04
- DATA[0] = 0x00

Note: There is a known limitation where the example might stop receiving
packages if the application halt execution after hitting a breakpoint. It is
highly recommended to use free run option while testing the example.
