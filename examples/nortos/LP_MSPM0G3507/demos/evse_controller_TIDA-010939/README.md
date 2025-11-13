## Example Summary

Firmware for the MSPM0G3507 on the TIDA-010939 reference design acting as an electric vehicle supply equipment front-end controller.
Handles the handshaking with the connected EV to negotiate with and safely charge the vehicle.

## Peripherals & Pin Assignments

| Peripheral | Pin | Function |
| --- | --- | --- |
| GPIOA | PA18 | Standard with Wake Input |
| GPIOA | PA7 | Standard Output |
| GPIOA | PA12 | High-Speed Output |
| GPIOA | PA13 | High-Speed Input |
| GPIOA | PA22 | Standard Output |
| GPIOA | PA31 | High-Drive Input |
| GPIOA | PA4 | Standard Input |
| GPIOA | PA3 | Standard Input |
| GPIOB | PB15 | Standard Output |
| GPIOB | PB13 | Standard Output |
| GPIOB | PB16 | Standard Input |
| GPIOB | PB18 | Standard Input |
| GPIOB | PB17 | Standard Input |
| GPIOB | PB20 | Standard Output |
| GPIOB | PB21 | Standard Output |
| GPIOB | PB22 | Standard Input |
| GPIOB | PB23 | Standard Output |
| GPIOB | PB24 | Standard Output |
| GPIOB | PB25 | Standard Output |
| GPIOB | PB26 | Standard Output |
| GPIOB | PB27 | Standard Output |
| GPIOB | PB4 | Standard Output |
| GPIOB | PB8 | Standard Output |
| GPIOB | PB1 | Standard Input |
| GPIOB | PB19 | Standard Output |
| SYSCTL | PA5 | HFXIN |
| SYSCTL | PA6 | HFXOUT |
| SYSCTL | PA2 | ROSC |
| TIMA1 | PB0 | Capture/Compare Pin 0 |
| TIMG7 | PA28 | Capture/Compare Pin 0 |
| TIMG0 | PB10 | Capture/Compare Pin 0 |
| TIMG8 |  |  |
| TIMG6 |  |  |
| TIMA0 |  |  |
| TIMG12 |  |  |
| I2C0 | PA0 | I2C Serial Data line (SDA) |
| I2C0 | PA1 | I2C Serial Clock line (SCL) |
| UART0 | PA11 | RX Pin |
| UART0 | PA10 | TX Pin |
| UART1 | PA9 | RX Pin |
| UART1 | PA8 | TX Pin |
| ADC0 | PA27 | ADC12 Channel 0 Pin |
| ADC0 | PA25 | ADC12 Channel 2 Pin |
| ADC0 | PA26 | ADC12 Channel 1 Pin |
| ADC0 | PA24 | ADC12 Channel 3 Pin |
| ADC1 | PA15 | ADC12 Channel 0 Pin |
| ADC1 | PA16 | ADC12 Channel 1 Pin |
| VREF | PA23 | VREF Positive (VREF+) |
| VREF | PA21 | VREF Negative (VREF-) |
| EVENT |  |  |
| DMA |  |  |
| CRC |  |  |
| SYSTICK |  |  |
| MATHACL |  |  |
| DEBUGSS | PA20 | Debug Clock |
| DEBUGSS | PA19 | Debug Data In Out |

## BoosterPacks, Board Resources & Jumper Settings

Visit [LP_MSPM0G3507](https://www.ti.com/tool/LP-MSPM0G3507) for LaunchPad information, including user guide and hardware files.

| Pin | Peripheral | Function | LaunchPad Pin | LaunchPad Settings |
| --- | --- | --- | --- | --- |
| PA18 | GPIOA | PA18 | J3_26/J3_29 | <ul><li>PA18 can be connected to S1 button to VCC with external pull-down<br><ul><li>`J8 ON` Connect S1 button and external pull-down<br><li>`J8 OFF` Disconnect S1 button and external pull-down</ul><br><li>PA18 can be connected to XDS-110 BSL_Invoke<br><ul><li>`J101 17:18 OFF` Disconnect pin to XDS-110 BSL Invoke</ul><br><li>PA18 can be connected to boosterpack pin J3_29<br><ul><li>`J15 2:3` Connect pin to J3_29<br><li>`J15 1:2/OFF` Disconnect pin from J3_29</ul><br><li>PA18 can be connected to boosterpack pin J3_26<br><ul><li>`R58` is soldered and connects pin to J3_26 by default</ul></ul> |
| PA7 | GPIOA | PA7 | J28_6 | N/A |
| PA12 | GPIOA | PA12 | J4_32/J26_1 | <ul><li>PA12 can be connected to CAN/LIN connector in addition to boosterpack connector:<br><ul><li>To use on J26 CAN/LIN connector:<br>  `R64` is populated by default and connects pin to `J26_1`</ul></ul> |
| PA13 | GPIOA | PA13 | J4_32/J26_3 | <ul><li>PA13 can be connected to CAN/LIN connector in addition to boosterpack connector:<br><ul><li>To use on J26 CAN/LIN connector:<br>  `R65` is populated by default and connects pin to `J26_3`</ul></ul> |
| PA22 | GPIOA | PA22 | J3_24 | <ul><li>PA22 can be connected to photodiode D1<br><ul><li>`J16 OFF` Disconnect from photodiode D1</ul>></ul> |
| PA31 | GPIOA | PA31 | J4_37 | N/A |
| PA4 | GPIOA | PA4 | J28_3 | <ul><li>PA4 is used for LFXT functionality by default in Launchpad<br><ul><li>Remove `Y1` crystal to use pin for other functions</ul><br><li>PA4 can be connected to J28_3<br><ul><li>`R11` is not soldered soldered by default<br><li>Solder `R11` to connect pin to J28_3</ul> |
| PA3 | GPIOA | PA3 | J28_2 | <ul><li>PA3 is used for LFXT functionality by default in Launchpad<br><ul><li>Remove `Y1` crystal to use pin for other functions</ul><br><li>PA3 can be connected to J28_2<br><ul><li>`R10` is not soldered soldered by default<br><li>Solder `R10` to connect pin to J28_2</ul> |
| PB15 | GPIOB | PB15 | J2_17 | N/A |
| PB13 | GPIOB | PB13 | J4_35 | N/A |
| PB16 | GPIOB | PB16 | J2_11 | N/A |
| PB18 | GPIOB | PB18 | J3_25 | N/A |
| PB17 | GPIOB | PB17 | J2_18 | N/A |
| PB20 | GPIOB | PB20 | J4_36 | N/A |
| PB21 | GPIOB | PB21 | J27_4 | <ul><li>PB21 is connected to S2 button to GND with no external pull resistor<br><ul><li>Press `S2` button to connect pin to GND<br><li>Don't use `S2` button if not needed by application</ul></ul> |
| PB22 | GPIOB | PB22 | J27_5 | <ul><li>PB22 can be connected to LED2 Blue<br><ul><li>`J5 ON` Connect to LED2 Blue<br><li>`J15 OFF` Disconnect from LED2 Blue</ul></ul> |
| PB23 | GPIOB | PB23 | J1_3/J27_6 | <ul><li>PB23 can be connected to boosterpack connection in addition to J27<br><ul><li>`J14 1:2` Connect pin to J1_3<br><li>`J14 2:3/OFF` Disconnect pin from J1_3</ul></ul> |
| PB24 | GPIOB | PB24 | J1_6 | <ul><li>PB24 can be connected to thermistor RT1<br><ul><li>`J9 2:3/OFF` Disconnect from thermistor RT1</ul></ul> |
| PB25 | GPIOB | PB25 | J27_7/P1 | <ul><li>PB25 can be connected to Active Buffer OPA2365<br><ul><li>OPA2365 is connected by default<br><li><li>Solder `R74` and remove `R72` to bypass OPA2365</ul></ul> |
| PB26 | GPIOB | PB26 | J27_8 | <ul><li>PB26 can be connected to LED2 Red<br><ul><li>`J6 ON` Connect to LED2 Red<br><li>`J6 OFF` Disconnect from LED2 Red</ul></ul> |
| PB27 | GPIOB | PB27 | J27_10 | <ul><li>PB27 can be connected to LED2 Green<br><ul><li>`J7 ON` Connect to LED2 Green<br><li>`J7 OFF` Disconnect from LED2 Green</ul></ul> |
| PB4 | GPIOB | PB4 | J4_40 | N/A |
| PB8 | GPIOB | PB8 | J2_15 | N/A |
| PB1 | GPIOB | PB1 | J4_39 | N/A |
| PB19 | GPIOB | PB19 | J3_23 | N/A |
| PA5 | SYSCTL | HFXIN | J28_4 | <ul><li>PA5 is used for HFXT functionality by default in Launchpad<br><ul><li>`Y2` crystal is soldered and connected by default</ul><br><li>PA5 can be connected to J28_4<br><ul><li>`R15` is not soldered soldered by default<br><li>Solder `R15` to connect pin to J28_4</ul> |
| PA6 | SYSCTL | HFXOUT | J28_5 | <ul><li>PA6 is used for HFXT functionality by default in Launchpad<br><ul><li>`Y2` crystal is soldered and connected by default</ul><br><li>PA6 can be connected to J28_5<br><ul><li>`R17` is not soldered soldered by default<br><li>Solder `R17` to connect pin to J28_5</ul> |
| PA2 | SYSCTL | ROSC | J28_1 | <ul><li>PA2 is used for ROSC functionality by default in Launchpad<br><ul><li>`R8` is connected to GND by default</ul><br><li>PA2 can be connected to J28_1<br><ul><li>`R9` is not soldered soldered by default<br><li>Solder `R9` to connect pin to J28_1</ul> |
| PB0 | TIMA1 | CCP0 | J2_12 | N/A |
| PA28 | TIMG7 | CCP0 | J4_38 | N/A |
| PB10 | TIMG0 | CCP0 | J27_2 | N/A |
| PA0 | I2C0 | SDA | J27_9 | <ul><li>PA0 is 5V tolerant open-drain so it requires pull-up<br><ul><li>`J19 1:2` Use 3.3V pull-up<br><li>`J19 2:3` Use 5V pull-up</ul><br><li>PA0 can be connected to LED1<br><ul><li>`J4 OFF` Disconnect from LED1</ul></ul> |
| PA1 | I2C0 | SCL | J28_9 | <ul><li>PA1 is 5V tolerant open-drain so it requires pull-up<br><ul><li>`J20 1:2` Use 3.3V pull-up<br><li>`J20 2:3` Use 5V pull-up</ul></ul> |
| PA11 | UART0 | RX | J4_33/J26_6 | <ul><li>PA11 can be used as UART RX connected to XDS-110 backchannel, to boosterpack connector or to CAN/LIN connector:<br><ul><li>To use backchannel UART on J101:<br>  `J22 1:2`: Connects XDS-110 backchannel UART RX to `J101`<br>  `J101 7:8` Connects XDS-110 backchannel to UART RX<br><li>To use UART on boosterpack connector:<br>  `J22 2:3`: Connects UART RX to `J4_33`<br><li>To use on J26 CAN/LIN connector:<br>  `R63` is populated by default and connects pin to `J26_6`</ul></ul> |
| PA10 | UART0 | TX | J4_34/J26_5 | <ul><li>PA10 can be used as UART TX connected to XDS-110 backchannel, to boosterpack connector or to CAN/LIN connector:<br><ul><li>To use backchannel UART on J101:<br>  `J21 1:2`: Connects XDS-110 backchannel UART TX to `J101`<br>  `J101 9:10` Connects XDS-110 backchannel to UART TX<br><li>To use UART on boosterpack connector:<br>  `J21 2:3`: Connects UART TX to `J4_34`<br><li>To use on J26 CAN/LIN connector:<br>  `R62` is populated by default and connects pin to `J26_5`</ul></ul> |
| PA9 | UART1 | RX | J1_3 | <ul><li>PA9 can be connected to J1_3<br><ul><li>`J1 2:3` Connect pin to J1_3<br><li>`J1 1:2/OFF` Disconnect pin from J1_3</ul> |
| PA8 | UART1 | TX | J1_4 | N/A |
| PA27 | ADC0 | A0 | J1_8 | <ul><li>PA27 can be connected to photodiode D1<br><ul><li>`J17 OFF` Disconnect from photodiode D1</ul></ul> |
| PA25 | ADC0 | A2 | J1_2 | N/A |
| PA26 | ADC0 | A1 | J1_5 | <ul><li>PA26 can be connected to photodiode D1<br><ul><li>`J18 ON` Connect to photodiode D1<br><li>`J18 OFF` Disconnect from photodiode D1</ul><br><li>PA26 can be connected to thermistor RT1<br><ul><li>`J9 2:3` Connect to thermistor RT1<br><li>`J9 1:2/OFF` Disconnect from thermistor RT1</ul></ul> |
| PA24 | ADC0 | A3 | J3_27 | N/A |
| PA15 | ADC1 | A0 | J3_30 | N/A |
| PA16 | ADC1 | A1 | J3_29 | <ul><li>PA16 can be connected to boosterpack connector<br><ul><li>`J15 1:2` Connect pin to `J3_29`<br><li>`J15 2:3/OFF` Disconnect pin from `J3_29`</ul></ul> |
| PA23 | VREF | VREF+ | N/A | <ul><li>PA23 can be connected to GND using C7 or to VREF- using C8<br><ul><li>`C8` is soldered by default connecting pin to VREF-<br><li>`C7` is not soldered by default and can be used for a custom capacitor to GND</ul><br><li>PA23 can be connected to boosterpack connector<br><ul><li>`R22` is not soldered by default<br><li>Solder `R22` to connect pin to J28_10</ul></ul> |
| PA21 | VREF | VREF- | J28_8 | <ul><li>PA21 can be connected to GND when used for VREF-<br><ul><li>`R3` is soldered by default connecting pin to GND</ul><br><li>PA21 can be connected to boosterpack<br><ul><li>`R10` is not soldered by default<br><li>Solder `R10` to connect pin to `J28_8`</ul></ul> |
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

Compile, load, and run this example of an AC charger on a TIDA-010939 (AM62L-EVSE-DEV-EVM) connected to an AM62L-EVM. The MSPM0 communicates with the host EVerest software running on the AM62L over UART. EVerest is an open-source firmware stack for EV charging stations that supports charging according to ISO 15118, OCPP, Plug & Charge, and more. The MCU detects connected vehicles and negotiates the charging current according to the ISO 15118-20 / IEC 61851 standards, while monitoring system health (e.g., detecting relay welding, GFCI faults, plug motor locking, and plug temperature).

For more information, please visit:
www.ti.com/tool/AM62L-EVSE-DEV-EVM
www.ti.com/tool/TIDA-010939

This software is based on the Yeti firmware developed by ‍Pionix GmbH which is licensed under the Apache License 2.0. Modifications (c) 2025, Texas Instruments Incorporated.
