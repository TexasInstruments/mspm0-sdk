## Example Summary

This example demonstrates UART TX and RX with Manchester encoding. Manchester encoding is a feature only usable with UART Extend.

This example can be used with the uart_extend_manchester_echo example running on another device.

This example enables Manchester encoding and decoding, then transmits a packet of data. The device then blocks on waiting for the packet data to be echoed back.
When the expected number of bytes has been received, it checks to see that the transmitted data matches the received data. If the data does not match, a software breakpoint is set. Otherwise, an LED is toggled to indicate success.

**Note**: Example requires Rev E3 or later of the MSPM0C1104 LaunchPad.

## Peripherals & Pin Assignments

| Peripheral | Pin | Function |
| --- | --- | --- |
| GPIOA | PA22 | Standard Output |
| GPIOA | PA2 | Standard Output |
| SYSCTL |  |  |
| UART0 | PA26 | RX Pin |
| UART0 | PA27 | TX Pin |
| EVENT |  |  |
| DEBUGSS | PA20 | Debug Clock |
| DEBUGSS | PA19 | Debug Data In Out |

## BoosterPacks, Board Resources & Jumper Settings

Visit [LP_MSPM0C1104](https://www.ti.com/tool/LP-MSPM0C1104) for LaunchPad information, including user guide and hardware files.

| Pin | Peripheral | Function | LaunchPad Pin | LaunchPad Settings |
| --- | --- | --- | --- | --- |
| PA22 | GPIOA | PA22 | J1_8 | N/A |
| PA2 | GPIOA | PA2 | J2_13 | N/A |
| PA26 | UART0 | RX | J1_3 | <ul><li>PA26 can be used as UART RX connected to XDS-110 backchannel or to boosterpack connector:<br><ul><li>To use backchannel UART on J101:<br>  `J101 5:6 ON` Connects XDS-110 backchannel to UART RX<br><li>To use UART on boosterpack connector:<br>  `J101 5:6 OFF`: Disconnects XDS-110 backchannel</ul></ul> |
| PA27 | UART0 | TX | J1_4 | <ul><li>PA27 can be used as UART TX connected to XDS-110 backchannel or to boosterpack connector:<br><ul><li>To use backchannel UART on J101:<br>  `J101 7:8 ON` Connects XDS-110 backchannel to UART TX<br><li>To use UART on boosterpack connector:<br>  `J101 7:8 OFF`: Disconnects XDS-110 backchannel</ul></ul> |
| PA20 | DEBUGSS | SWCLK | J2_11 | <ul><li>PA20 is used by SWD during debugging<br><ul><li>`J101 13:14 ON` Connect to XDS-110 SWCLK while debugging<br><li>`J101 13:14 OFF` Disconnect from XDS-110 SWCLK if using pin in application</ul></ul> |
| PA19 | DEBUGSS | SWDIO | J2_17 | <ul><li>PA19 is used by SWD during debugging<br><ul><li>`J101 11:12 ON` Connect to XDS-110 SWDIO while debugging<br><li>`J101 11:12 OFF` Disconnect from XDS-110 SWDIO if using pin in application</ul></ul> |

### Device Migration Recommendations
This project was developed for a superset device included in the LP_MSPM0C1104 LaunchPad. Please
visit the [CCS User's Guide](https://software-dl.ti.com/msp430/esd/MSPM0-SDK/latest/docs/english/tools/ccs_ide_guide/doc_guide/doc_guide-srcs/ccs_ide_guide.html#sysconfig-project-migration)
for information about migrating to other MSPM0 devices.

### Low-Power Recommendations
TI recommends to terminate unused pins by setting the corresponding functions to
GPIO and configure the pins to output low or input with internal
pullup/pulldown resistor.

SysConfig allows developers to easily configure unused pins by selecting **Board**→**Configure Unused Pins**.

For more information about jumper configuration to achieve low-power using the
MSPM0 LaunchPad, please visit the [LP-MSPM0C1104 User's Guide](https://www.ti.com/lit/slau908).

## Example Usage
Connect the RX and TX between two UART devices.
Ensure that the uart_extend_manchester_echo is running first on another device.
Compile, load and run the example.
The LED should toggle after every packet received correctly after an echo from the other device.
