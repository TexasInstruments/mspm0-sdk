## Example Summary

Control and capture several GPIOs' status through UART, I2C or SPI.

## Peripherals & Pin Assignments

| Peripheral | Pin | Function |
| --- | --- | --- |
| GPIOA | PA2 | Standard Input |
| GPIOA | PA27 | Standard Input |
| GPIOA | PA17 | Standard with Wake Input |
| GPIOA | PA24 | Standard Input |
| GPIOA | PA4 | Standard Input |
| GPIOA | PA6 | Standard Input |
| GPIOA | PA16 | Standard Input |
| GPIOA | PA22 | Standard Input |
| SYSCTL |  |  |
| I2C0 | PA0 | I2C Serial Data line (SDA) |
| I2C0 | PA1 | I2C Serial Clock line (SCL) |
| UART0 | PA18 | RX Pin |
| UART0 | PA23 | TX Pin |
| SPI0 | PA11 | SPI SCLK (Clock) |
| SPI0 | PA25 | SPI PICO (Peripheral In, Controller Out) |
| SPI0 | PA26 | SPI POCI (Peripheral Out, Controller In) |
| EVENT |  |  |
| DEBUGSS | PA20 | Debug Clock |
| DEBUGSS | PA19 | Debug Data In Out |

## BoosterPacks, Board Resources & Jumper Settings

Visit [LP_MSPM0L1306](https://www.ti.com/tool/LP-MSPM0L1306) for LaunchPad information, including user guide and hardware files.

| Pin | Peripheral | Function | LaunchPad Pin | LaunchPad Settings |
| --- | --- | --- | --- | --- |
| PA2 | GPIOA | PA2 | N/A | <ul><li>PA2 is used for ROSC functionality by default in Launchpad<br><ul><li>`R6` is connected to GND by default<br><li>Remove `R6` to use pin for other functions</ul></ul> |
| PA27 | GPIOA | PA27 | J4_37 | <ul><li>PA27 can be connected to LED2 Blue<br><ul><li>`J13 ON` Connect to LED2 Blue<br><li>`J13 OFF` Disconnect from LED2 Blue</ul></ul> |
| PA17 | GPIOA | PA17 | J3_25 | N/A |
| PA24 | GPIOA | PA24 | J1_5 | <ul><li>PA24 can be connected to photodiode D1<br><ul><li>`J5 OFF` Disconnect from photodiode D1</ul></ul> |
| PA4 | GPIOA | PA4 | J2_14 | N/A |
| PA6 | GPIOA | PA6 | J1_7 | N/A |
| PA16 | GPIOA | PA16 | J3_24 | N/A |
| PA22 | GPIOA | PA22 | J1_8 | <ul><li>PA22 can be connected to photodiode D1<br><ul><li>`J6 OFF` Disconnect from photodiode D1</ul></ul> |
| PA0 | I2C0 | SDA | J1_10 | <ul><li>PA0 is 5V tolerant open-drain so it requires pull-up<br><ul><li>`J10 2:3` Use 3.3V pull-up<br><li>`J10 1:2` Use 5V pull-up</ul><br><li>PA0 can be connected to LED1<br><ul><li>`J2 OFF` Disconnect from LED1</ul></ul> |
| PA1 | I2C0 | SCL | J1_9 | <ul><li>PA1 is 5V tolerant open-drain so it requires pull-up<br><ul><li>`J19 2:3` Use 3.3V pull-up<br><li>`J9 1:2` Use 5V pull-up</ul></ul> |
| PA18 | UART0 | RX | J3_26 | <ul><li>PA18 can be connected to S1 button to VCC with external pull-down<br><ul><li>`J11 ON` Connect S1 button and external pull-down<br><li>`J11 OFF` Disconnect S1 button and external pull-down</ul><br><li>PA18 can be connected to XDS-110 BSL_Invoke<br><ul><li>`J101 17:18 OFF` Disconnect pin to XDS-110 BSL Invoke</ul></ul> |
| PA23 | UART0 | TX | J2_12 | N/A |
| PA11 | SPI0 | SCLK | J2_11/J4_35 | <ul><li>PA11 can be connected to two locations in boosterpack connector:<br><ul><li>`J14 1:2`: Connect pin to `J2_11`<br><li>`J14 2:3`: Connect pin to `J4_35`</ul></ul> |
| PA25 | SPI0 | MOSI | J1_2 | <ul><li>PA25 can be connected to photodiode D1<br><ul><li>`J4 OFF` Disconnect from photodiode D1</ul></ul> |
| PA26 | SPI0 | MISO | J4_38 | <ul><li>PA26 can be connected to LED2 Red<br><ul><li>`J12 OFF` Disconnect from LED2 Red</ul></ul> |
| PA20 | DEBUGSS | SWCLK | J2_13 | <ul><li>PA20 is used by SWD during debugging<br><ul><li>`J101 15:16 ON` Connect to XDS-110 SWCLK while debugging<br><li>`J101 15:16 OFF` Disconnect from XDS-110 SWCLK if using pin in application</ul></ul> |
| PA19 | DEBUGSS | SWDIO | J2_17 | <ul><li>PA19 is used by SWD during debugging<br><ul><li>`J101 13:14 ON` Connect to XDS-110 SWDIO while debugging<br><li>`J101 13:14 OFF` Disconnect from XDS-110 SWDIO if using pin in application</ul></ul> |

### Device Migration Recommendations
This project was developed for a superset device included in the LP_MSPM0L1306 LaunchPad. Please
visit the [CCS User's Guide](https://software-dl.ti.com/msp430/esd/MSPM0-SDK/latest/docs/english/tools/ccs_ide_guide/doc_guide/doc_guide-srcs/ccs_ide_guide.html#sysconfig-project-migration)
for information about migrating to other MSPM0 devices.

### Low-Power Recommendations
TI recommends to terminate unused pins by setting the corresponding functions to
GPIO and configure the pins to output low or input with internal
pullup/pulldown resistor.

SysConfig allows developers to easily configure unused pins by selecting **Board**→**Configure Unused Pins**.

For more information about jumper configuration to achieve low-power using the
MSPM0 LaunchPad, please visit the [LP-MSPM0L1306 User's Guide](https://www.ti.com/lit/slau869).

## Example Usage

Compile, load and run the example. Then you can control and capture several GPIOs' status through UART, I2C or SPI. Please refer to the related [app note](https://www.ti.com/lit/slaael9) for more details.
