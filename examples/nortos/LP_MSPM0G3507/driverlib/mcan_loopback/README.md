## Example Summary
This example shows the MCAN Loopback functionality. The module is configured in
internal loopback mode. The sent message should be received by the node.

## Peripherals & Pin Assignments

| Peripheral | Pin | Function |
| --- | --- | --- |
| SYSCTL | PA5 | HFXIN |
| SYSCTL | PA6 | HFXOUT |
| CANFD0 | PA13 | RX Pin |
| CANFD0 | PA12 | TX Pin |
| DEBUGSS | PA20 | Debug Clock |
| DEBUGSS | PA19 | Debug Data In Out |

## BoosterPacks, Board Resources & Jumper Settings

Visit [LP_MSPM0G3507](https://www.ti.com/tool/LP-MSPM0G3507) for LaunchPad information, including user guide and hardware files.

| Pin | Peripheral | Function | LaunchPad Pin | LaunchPad Settings |
| --- | --- | --- | --- | --- |
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
Compile, load and run.
The module is configured in internal loopback mode. The sent messages should be
internally received by the node, and the application is configured to send 10
messages. After 10 messages are sent, gTransmitComplete will be set to true and
a breakpoint will be hit.
A breakpoint will also be hit if an error is encountered in the application, the
status of the error can be inspected in the gError variable.
Otherwise, the application can be paused or a breakpoint can be set at line 125
to inspect the received messages in rxMsg.
