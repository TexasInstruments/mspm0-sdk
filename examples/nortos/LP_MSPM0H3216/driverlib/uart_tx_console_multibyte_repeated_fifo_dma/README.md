## Example Summary

This example sets up the UART to continuously transmit data to output to the console via the DMA
using a Timer to write each second. This example is designed to run from the LaunchPad connected
to a PC with a console open, and verify the DMA can be used to output data to the console with
minimal use of the CPU.

## Peripherals, Pin Functions, MCU Pins, Launchpad Pins
| Peripheral | Function | MCU Pin | Launchpad Pin | Launchpad Settings |
| --- | --- | --- | --- | --- |
| GPIOA | Standard Output | PA0 | Red LED1 | Populate Jumper(s): J4[1:2] |
| GPIOA | Standard Output | PA15 | J4_38 |  |
| SYSCTL |  |  |  |  |
| TIMG1 |  |  |  |  |
| UART0 | RX Pin | PB7 | J1_3 |  |
| UART0 | TX Pin | PB6 | J1_4 |  |
| EVENT |  |  |  |  |
| DMA |  |  |  |  |
| BOARD | Debug Clock | PA20 | J13_4 | Populate Component(s): J13 |
| BOARD | Debug Data In Out | PA19 | J13_2 | Populate Component(s): J13 |

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
MSPM0 LaunchPad, please visit the LP-MSPM0H3216 User's Guide.

## Example Usage
Compile, load and run the example.
Connect to terminal if using Launchpad's back-channel UART, or connect to
external device using BoosterPack.
Configure the terminal emulator for:
- 115200 baud rate
- one stop bit
- no parity

The MSPM0 should begin to output a welcome message and an incremental test message every second. LED1 will toggle each transmit.
USER_TEST_PIN GPIO will mimic the behavior of the LED pin on the BoosterPack header and can be used to verify the LED behavior.
