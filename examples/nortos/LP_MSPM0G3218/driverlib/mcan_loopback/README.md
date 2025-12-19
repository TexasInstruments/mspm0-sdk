## Example Summary
This example shows the MCAN Loopback functionality. The module is configured in
internal loopback mode. The sent message should be received by the node.

## Peripherals, Pin Functions, MCU Pins, Launchpad Pins
| Peripheral | Function | MCU Pin | Launchpad Pin | Launchpad Settings |
| --- | --- | --- | --- | --- |
| GPIOA | Open-Drain Output | PA0 | Red LED1 | Populate Jumper(s): J4[1:2] |
| GPIOA | Standard Output | PA15 | J3_30 |  |
| SYSCTL | HFXIN | PA5 | J1_17 | Populate Component(s): R15 |
| SYSCTL | HFXOUT | PA6 | J1_16 | Populate Component(s): R17 |
| EVENT |  |  |  |  |
| BOARD | Debug Clock | PA20 | J101_16 |  |
| BOARD | Debug Data In Out | PA19 | J101_14 |  |

### Device Migration Recommendations
This project was developed for a superset device included in the MSPM0 LaunchPad. Please
visit the [CCS User's Guide](https://software-dl.ti.com/msp430/esd/MSPM0-SDK/latest/docs/english/tools/ccs_ide_guide/doc_guide/doc_guide-srcs/ccs_ide_guide.html#sysconfig-project-migration)
for information about migrating to other MSPM0 devices.

### Low-Power Recommendations
TI recommends to terminate unused pins by setting the corresponding functions to
GPIO and configure the pins to output low or input with internal
pullup/pulldown resistor.

SysConfig allows developers to easily configure unused pins by selecting **Board**→**Configure Unused Pins**.

For more information about jumper configuration to achieve low-power using the
MSPM0 LaunchPad, please visit the LP-MSPM0G3218 User's Guide.

## Example Usage
Compile, load and run.
The module is configured in internal loopback mode. The sent messages should be
internally received by the node.

A breakpoint will also be hit if an error is encountered in the application, the
status of the error can be inspected in the gError variable.
Otherwise, the application can be paused or a breakpoint can be set at line 127
to inspect the received messages in rxMsg.

txMsg.data[0] will be updated with the current value of "gMsgCount" before
sending a new loopback test message to confirm a new data is being transmitted
and received.
