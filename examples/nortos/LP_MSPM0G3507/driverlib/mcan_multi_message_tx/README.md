## Example Summary
This example demonstrate how to send multiple CAN-FD messages with message IDs
0x3 and 0x4 via GPIO input interrupt.

The least-significant bit (LSB) in the fist data byte of the packet is toggled
every time the messages are transmitted.

This example can be used with the mcan_multi_message_rx example project.

## Peripherals & Pin Assignments

| Peripheral | Pin | Function |
| --- | --- | --- |
| GPIOB | PB21 | Standard Input with internal pull-up |
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
| PB21 | GPIOB | PB21 | J27_4 | Connected to S2 button (no pull resistor) |
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
Evaluation Module User's Guide and monitor and receive CAN-FD packages using
mcan_multi_message_rx example project or using a CAN Bus Analyzer tool of your
choice.

Ensure the MCAN receiver is running.

Compile, load and run the example.

The example has the following Nominal and Data Rate frequencies configured:
- Nominal Bit Rate (250 kbps)
- Data Bit Rate    (2 Mbps)

When S2 button is pressed (or the GPIO detects a falling input edge), the
following CAN-FD packages with IDs 0x4 and 0x3 will be transmitted simultaneously.
Each time the S2 button is pressed, the data (the LSB of the first data payload byte) that each message sends will be toggled.

Message 1:
- ID : 0x04
- BRS: 0x01
- DLC: 0x01
- FDF: 0x01
- DATA[0] = 0x01

Message 2:
- ID : 0x03
- BRS: 0x01
- DLC: 0x01
- FDF: 0x01
- DATA[0] = 0x00

When using this example with mcan_multi_message_rx example, the packages
controls the state of two LEDs.

Note: There is a known limitation where the example might stop transmitting
packages if the application halt execution after hitting a breakpoint. It is
highly recommended to use the free run option while testing the example.
