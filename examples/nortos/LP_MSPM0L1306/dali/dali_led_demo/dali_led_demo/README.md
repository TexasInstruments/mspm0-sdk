## Example Summary

MSP-DALI LED Demo
This example shows how to utilize the MSP DALI Library to create a simple
LED Demo.  This demo allows you to utilize the various DALI commands to
control an LED on the launchpad and keep track of pertinent DALI
information.

The Texas Instruments® MSP DALI Library is a collection of functions to enable the implementation of DALI Lighting Applications using the MSPM0L devices.  This library provides functionality for most basic functions
and also allows users to modify this implementation to fit their needs.


## Peripherals & Pin Assignments

| Peripheral | Pin | Function |
| --- | --- | --- |
| SYSCTL |  |  |
| TIMG1 | PA0 | Counter Compare Pin 0 |
| TIMG1 | PA1 | Counter Compare Pin 1 |
| TIMG4 |  |  |
| TIMG2 |  |  |
| TIMG0 |  |  |
| UART0 | PA9 | RX Pin |
| UART0 | PA17 | TX Pin |
| EVENT |  |  |
| CRC |  |  |
| DEBUGSS | PA20 | Debug Clock |
| DEBUGSS | PA19 | Debug Data In Out |

## BoosterPacks, Board Resources & Jumper Settings

Visit [LP_MSPM0L1306](https://www.ti.com/tool/LP-MSPM0L1306) for LaunchPad information, including user guide and hardware files.

| Pin | Peripheral | Function | LaunchPad Pin | LaunchPad Settings |
| --- | --- | --- | --- | --- |
| PA0 | TIMG1 | CCP0 | J1_10 | This pin is 5V tolerant open-drain and requires pull-up.<br>J2 ON/OFF: Connect/Disconnect LED1<br>J10 1:2 5V Pull-up<br>J10 2:3 3.3V Pull-up |
| PA1 | TIMG1 | CCP1 | J1_9 | This pin is 5V tolerant open-drain and requires pull-up.<br>J9 1:2 5V Pull-up<br>J9 2:3 3.3V Pull-up |
| PA9 | UART0 | RX | J1_3 | J17 1:2 Connect XDS-110 BackChannel UART on J101<br>J17 2:3 Connect to J1_3<br>J101 7:8 Connect XDS-110 BackChannel UART RX |
| PA17 | UART0 | TX | J3_25 | N/A |
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

Compile, load and run the example.
