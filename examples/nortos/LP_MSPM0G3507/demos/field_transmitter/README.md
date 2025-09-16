## Example Summary

This example demonstrates a field transmitter implementation. It reads out an ADC for a pressure sensor, compensates and scales the values. To output the readings, a current loop of 4-20 mA is used, so the software controls an DAC to set the coresponding values.


## Peripherals & Pin Assignments

| Peripheral | Pin | Function |
| --- | --- | --- |
| GPIOA | PA3 | Standard Output |
| GPIOA | PA26 | Standard Output |
| GPIOA | PA22 | Standard Input with internal pull-up |
| GPIOA | PA16 | Standard Output |
| GPIOA | PA17 | Standard with Wake Output |
| GPIOA | PA18 | Standard with Wake Input |
| GPIOA | PA26 | Standard Output |
| GPIOA | PA21 | Standard Output |
| GPIOA | PA15 | Standard Input |
| GPIOA | PA2 | Standard Output |
| GPIOA | PA25 | Standard Output |
| SYSCTL |  |  |
| TIMG0 |  |  |
| UART0 | PA1 | RX Pin |
| UART0 | PA0 | TX Pin |
| UART2 | PA24 | RX Pin |
| UART2 | PA23 | TX Pin |
| EVENT |  |  |
| DMA |  |  |
| SPI0 | PA11 | SPI SCLK (Clock) |
| SPI0 | PA9 | SPI PICO (Peripheral In, Controller Out) |
| SPI0 | PA10 | SPI POCI (Peripheral Out, Controller In) |
| SPI1 | PA17 | SPI SCLK (Clock) |
| SPI1 | PA18 | SPI PICO (Peripheral In, Controller Out) |
| SPI1 | PA16 | SPI POCI (Peripheral Out, Controller In) |
| SYSTICK |  |  |
| DEBUGSS | PA20 | Debug Clock |
| DEBUGSS | PA19 | Debug Data In Out |

## BoosterPacks, Board Resources & Jumper Settings

Visit [LP_MSPM0G3507](https://www.ti.com/tool/LP-MSPM0G3507) for LaunchPad information, including user guide and hardware files.

| Pin | Peripheral | Function | LaunchPad Pin | LaunchPad Settings |
| --- | --- | --- | --- | --- |
| PA3 | GPIOA | PA3 | J28_2 | <ul><li>PA3 is used for LFXT functionality by default in Launchpad<br><ul><li>Remove `Y1` crystal to use pin for other functions</ul><br><li>PA3 can be connected to J28_2<br><ul><li>`R10` is not soldered soldered by default<br><li>Solder `R10` to connect pin to J28_2</ul> |
| PA26 | GPIOA | PA26 | J1_5 | <ul><li>PA26 can be connected to photodiode D1<br><ul><li>`J18 OFF` Disconnect from photodiode D1</ul><br><li>PA26 can be connected to thermistor RT1<br><ul><li>`J9 1:2/OFF` Disconnect from thermistor RT1</ul></ul> |
| PA22 | GPIOA | PA22 | J3_24 | <ul><li>PA22 can be connected to photodiode D1<br><ul><li>`J16 OFF` Disconnect from photodiode D1</ul>></ul> |
| PA16 | GPIOA | PA16 | J3_29 | <ul><li>PA16 can be connected to boosterpack connector<br><ul><li>`J15 1:2` Connect pin to `J3_29`<br><li>`J15 2:3/OFF` Disconnect pin from `J3_29`</ul></ul> |
| PA17 | GPIOA | PA17 | J3_28 | N/A |
| PA18 | GPIOA | PA18 | J3_26/J3_29 | <ul><li>PA18 can be connected to S1 button to VCC with external pull-down<br><ul><li>`J8 ON` Connect S1 button and external pull-down<br><li>`J8 OFF` Disconnect S1 button and external pull-down</ul><br><li>PA18 can be connected to XDS-110 BSL_Invoke<br><ul><li>`J101 17:18 OFF` Disconnect pin to XDS-110 BSL Invoke</ul><br><li>PA18 can be connected to boosterpack pin J3_29<br><ul><li>`J15 2:3` Connect pin to J3_29<br><li>`J15 1:2/OFF` Disconnect pin from J3_29</ul><br><li>PA18 can be connected to boosterpack pin J3_26<br><ul><li>`R58` is soldered and connects pin to J3_26 by default</ul></ul> |
| PA26 | GPIOA | PA26 | J1_5 | <ul><li>PA26 can be connected to photodiode D1<br><ul><li>`J18 OFF` Disconnect from photodiode D1</ul><br><li>PA26 can be connected to thermistor RT1<br><ul><li>`J9 1:2/OFF` Disconnect from thermistor RT1</ul></ul> |
| PA21 | GPIOA | PA21 | J28_8 | <ul><li>PA21 can be connected to GND when used for VREF-<br><ul><li>`R3` is soldered by default connecting pin to GND<br><li>Remove `R3` to use pin for other functions</ul><br><li>PA21 can be connected to boosterpack<br><ul><li>`R10` is not soldered by default<br><li>Solder `R10` to connect pin to `J28_8`</ul></ul> |
| PA15 | GPIOA | PA15 | J3_30 | N/A |
| PA2 | GPIOA | PA2 | J28_1 | <ul><li>PA2 is used for ROSC functionality by default in Launchpad<br><ul><li>`R8` is connected to GND by default<br><li>Remove `R8` to use pin for other functions</ul><br><li>PA2 can be connected to J28_1<br><ul><li>`R9` is not soldered soldered by default<br><li>Solder `R9` to connect pin to J28_1</ul> |
| PA25 | GPIOA | PA25 | J1_2 | N/A |
| PA1 | UART0 | RX | J28_9 | <ul><li>PA1 is 5V tolerant open-drain so it requires pull-up<br><ul><li>`J20 1:2` Use 3.3V pull-up<br><li>`J20 2:3` Use 5V pull-up</ul></ul> |
| PA0 | UART0 | TX | J27_9 | <ul><li>PA0 is 5V tolerant open-drain so it requires pull-up<br><ul><li>`J19 1:2` Use 3.3V pull-up<br><li>`J19 2:3` Use 5V pull-up</ul><br><li>PA0 can be connected to LED1<br><ul><li>`J4 OFF` Disconnect from LED1</ul></ul> |
| PA24 | UART2 | RX | J3_27 | N/A |
| PA23 | UART2 | TX | N/A |  |
| PA11 | SPI0 | SCLK | J4_33/J26_6 | <ul><li>PA11 can be connected to XDS-110 backchannel, boosterpack connector or CAN/LIN connector:<br><ul><li>To use on boosterpack connector and disconnect XDS-110:<br>  `J22 2:3`: Connect pin to `J4_33`<br><li>To use on J26 CAN/LIN connector:<br>  `R63` is populated by default and connects pin to `J26_6`</ul></ul> |
| PA9 | SPI0 | MOSI | J1_3 | <ul><li>PA9 can be connected to J1_3<br><ul><li>`J1 2:3` Connect pin to J1_3<br><li>`J1 1:2/OFF` Disconnect pin from J1_3</ul> |
| PA10 | SPI0 | MISO | J4_34/J26_5 | <ul><li>PA10 can be connected to XDS-110 backchannel, boosterpack connector or CAN/LIN connector:<br><ul><li>To use on boosterpack connector and disconnect XDS-110:<br>  `J21 2:3`: Connect pin to `J4_34`<br><li>To use on J26 CAN/LIN connector:<br>  `R62` is populated by default and connects pin to `J26_5`</ul></ul> |
| PA17 | SPI1 | SCLK | J3_28 | N/A |
| PA18 | SPI1 | MOSI | J3_26/J3_29 | <ul><li>PA18 can be connected to S1 button to VCC with external pull-down<br><ul><li>`J8 ON` Connect S1 button and external pull-down<br><li>`J8 OFF` Disconnect S1 button and external pull-down</ul><br><li>PA18 can be connected to XDS-110 BSL_Invoke<br><ul><li>`J101 17:18 OFF` Disconnect pin to XDS-110 BSL Invoke</ul><br><li>PA18 can be connected to boosterpack pin J3_29<br><ul><li>`J15 2:3` Connect pin to J3_29<br><li>`J15 1:2/OFF` Disconnect pin from J3_29</ul><br><li>PA18 can be connected to boosterpack pin J3_26<br><ul><li>`R58` is soldered and connects pin to J3_26 by default</ul></ul> |
| PA16 | SPI1 | MISO | J3_29 | <ul><li>PA16 can be connected to boosterpack connector<br><ul><li>`J15 1:2` Connect pin to `J3_29`<br><li>`J15 2:3/OFF` Disconnect pin from `J3_29`</ul></ul> |
| PA20 | DEBUGSS | SWCLK | N/A | <ul><li>PA20 is used by SWD during debugging<br><ul><li>`J101 15:16 ON` Connect to XDS-110 SWCLK while debugging<br><li>`J101 15:16 OFF` Disconnect from XDS-110 SWCLK if using pin in application</ul></ul> |
| PA19 | DEBUGSS | SWDIO | N/A | <ul><li>PA19 is used by SWD during debugging<br><ul><li>`J101 13:14 ON` Connect to XDS-110 SWDIO while debugging<br><li>`J101 13:14 OFF` Disconnect from XDS-110 SWDIO if using pin in application</ul></ul> |

### Device Migration Recommendations
This project was developed for a superset device included in the LP_MSPM0G3507 LaunchPad. Please
visit the [CCS User's Guide](https://software-dl.ti.com/msp430/esd/MSPM0-SDK/latest/docs/english/tools/ccs_ide_guide/doc_guide/doc_guide-srcs/ccs_ide_guide.html#manual-migration)
for information about migrating to other MSPM0 devices.

### Low-Power Recommendations
TI recommends to terminate unused pins by setting the corresponding functions to
GPIO and configure the pins to output low or input with internal
pullup/pulldown resistor.

SysConfig allows developers to easily configure unused pins by selecting **Board**→**Configure Unused Pins**.

For more information about jumper configuration to achieve low-power using the
MSPM0 LaunchPad, please visit the [LP-MSPM0G3507 User's Guide](https://www.ti.com/lit/slau873).

## Example Usage

See also the documentation of TIDA-010982 here https://www.ti.com/tool/TIDA-010982.
