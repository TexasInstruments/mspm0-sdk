## Example Summary
The following example configures the UART at 115200bps waiting to echo the characters received over USB.
The default USB port is shared with the tinyusb's UART port. If a different UART port is required change either UARTs in their
respective configuration files (ti_msp_dl_config for driverlib's UART)


## Peripherals, Pin Functions, MCU Pins, Launchpad Pins
| Peripheral | Function | Pin | Launchpad Pin |
| --- | --- | --- | --- |
| GPIOA | Open-Drain Output | PA0 | Red LED1 |
| UART0 | RX Pin | PA11 | J4_33 |
| UART0 | TX Pin | PA10 | J4_34 |
| USB   | USB DM | PA26 | USB2 |
| USB   | USB DP | PA27 | USB2 |
| BOARD | Debug Clock | PA20 | J19_16 |
| BOARD | Debug Data In Out | PA19 | J19_14 |

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
MSPM0 LaunchPad, please visit the LP-MSPM0G5517 User's Guide.

## Example Usage
Compile, load and run the example.
When the example is run your PC will create a COM port for the USB connection (Labeled USB Serial Device).
Connect to the COM port via a terminal program. Type in the terminal and observe the echoed data on the UART TX pin.
