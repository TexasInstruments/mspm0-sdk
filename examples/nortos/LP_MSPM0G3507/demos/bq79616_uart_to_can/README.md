## Example Summary

This example shows the implementation of using MSPM0 to control BQ79616 to detect the battery status. After pressing the button connected to PB21, MSPM0 will start communicating with BQ79616 though the UART port at PA9(RX) and PA8(TX). Then send battery status to CAN interface.

The example uses a BQ79616 which is a battery monitor, but the same framework can be adapted for other similar UART-based battery management ICs from TI. Please refer to [BQ79616 Control Based on MSPM0 Through UART to CAN](https://www.ti.com/lit/slaaep4) app note for more details.

## Peripherals & Pin Assignments

| Peripheral | Pin | Function |
| --- | --- | --- |
| SYSCTL | N/A | N/A |
| GPIO | PB21 | Standard Input with Interrupt enabled |
| GPIO | PB0 | Standard Input |
| MCAN | PA13 | MCAN RX Pin |
| MCAN | PA12 | MCAN TX Pin |
| UART1 | PA9 | UART RX Pin |
| UART1 | PA8 | UART TX Pin |
| DEBUGSS | PA20 | Debug Clock |
| DEBUGSS | PA19 | Debug Data In Out |

## BoosterPacks, Board Resources & Jumper Settings

Visit [LP_MSPM0G3507](https://www.ti.com/tool/LP-MSPM0G3507) for LaunchPad information, including user guide and hardware files.

| Pin | Peripheral | Function | LaunchPad Pin | LaunchPad Settings |
| --- | --- | --- | --- | --- |
| PB21 | GPIOB | PB21 | S2 | <ul><li>PB21 is connected to LaunchPad's S2 Button</ul></ul> |
| PB0 | GPIOB | PB0 | J2_12 | <ul><li>PB0 is connected to BQ79616EVM's J17_3 fault pin.</ul></ul> |
| PA13 | MCAN | RX | J4_31 | <ul><li>PA13 is connected to TCAN1046EVM's J5_RX1.</ul></ul> |
| PA12 | MCAN | TX | J4_30 | <ul><li>PA12 is connected to TCAN1046EVM's J5_TX1.</ul></ul> |
| 3V3 | Power | Power | N/A | <ul><li>3V3 is connected to TCAN1046EVM's J5_VIO.</ul></ul> |
| 5V | Power | Power | N/A | <ul><li>PA12 is connected to TCAN1046EVM's J5_VCC.</ul></ul> |
| GND | Power | Power | N/A | <ul><li>PA12 is connected to TCAN1046EVM's J5_GND.</ul></ul> |
| PA9 | UART1 | RX | J14_3 | <ul><li>PA9 is connected to BQ79616EVM's J17_8.</ul></ul> |
| PA8 | UART1 | TX | J1_4 | <ul><li>PA8 is connected to BQ79616EVM's J17_7.</ul></ul> |
| PA20 | DEBUGSS | SWCLK | N/A | <ul><li>PA20 is used by SWD during debugging<br><ul><li>`J101 15:16 ON` Connect to XDS-110 SWCLK while debugging<br><li>`J101 15:16 OFF` Disconnect from XDS-110 SWCLK if using pin in application</ul></ul> |
| PA19 | DEBUGSS | SWDIO | N/A | <ul><li>PA19 is used by SWD during debugging<br><ul><li>`J101 13:14 ON` Connect to XDS-110 SWDIO while debugging<br><li>`J101 13:14 OFF` Disconnect from XDS-110 SWDIO if using pin in application</ul></ul> |

### Device Migration Recommendations
This project was developed for a superset device included in the LP_MSPM0G3507 LaunchPad. Please
visit the [CCS User's Guide](https://software-dl.ti.com/msp430/esd/MSPM0-SDK/latest/docs/english/tools/ccs_ide_guide/doc_guide/doc_guide-srcs/ccs_ide_guide.html#sysconfig-project-migration)
for information about migrating to other MSPM0 devices.

### Low-Power Recommendations
TI recommends to terminate unused pins by setting the corresponding functions to
GPIO and configure the pins to output low or input with internal
pullup/pulldown resistor.

SysConfig allows developers to easily configure unused pins by selecting **Board**→**Configure Unused Pins**.

For more information about jumper configuration to achieve low-power using the
MSPM0 LaunchPad, please visit the [LP-MSPM0G3507 User's Guide](https://www.ti.com/lit/slau873).

## Example Usage

Compile, load and run the example.
Please refer to [BQ79616 Control Based on MSPM0 Through UART to CAN](https://www.ti.com/lit/slaaep4) app note for more details.
