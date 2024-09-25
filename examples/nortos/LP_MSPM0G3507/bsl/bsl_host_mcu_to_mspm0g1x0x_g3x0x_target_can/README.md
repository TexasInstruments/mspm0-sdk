## Example Summary

This demo shows how to program a MSPM0 MCU using a LP-MSPM0G3507 as host by BSL CAN interface.

## Peripherals & Pin Assignments

| Peripheral | Pin | Function |
| --- | --- | --- |
| GPIOA | PA0 | Open-Drain Output |
| GPIOB | PB21 | Standard Input with internal pull-up |
| GPIOB | PB0 | Standard Output(To control NRST pin) |
| GPIOB | PB16 | Standard Output(To control invoke pin) |
| GPIOB | PB27 | Standard Output |
| SYSCTL | PA5 | HFXIN |
| SYSCTL | PA6 | HFXOUT |
| EVENT |  |  |
| CANFD0 | PA13 | RX Pin |
| CANFD0 | PA12 | TX Pin |
| DEBUGSS | PA20 | Debug Clock |
| DEBUGSS | PA19 | Debug Data In Out |

## BoosterPacks, Board Resources & Jumper Settings

Visit [LP_MSPM0G3507](https://www.ti.com/tool/LP-MSPM0G3507) for LaunchPad information, including user guide and hardware files.

| Pin | Peripheral | Function | LaunchPad Pin | LaunchPad Settings |
| --- | --- | --- | --- | --- |
| PA0 | GPIOA | PA0 | J27_9 | <ul><li>PA0 is 5V tolerant open-drain so it requires pull-up<br><ul><li>`J19 1:2` Use 3.3V pull-up<br><li>`J19 2:3` Use 5V pull-up</ul><br><li>PA0 can be connected to LED1<br><ul><li>`J4 ON` Connect to LED1<br><li>`J4 OFF` Disconnect from LED1</ul></ul> |
| PB21 | GPIOB | PB21 | J27_4 | <ul><li>PB21 is connected to S2 button to GND with no external pull resistor<br><ul><li>Press `S2` button to connect pin to GND<br><li>Don't use `S2` button if not needed by application</ul></ul> |
| PB0 | GPIOB | PB0 | J2_12 | N/A |
| PB16 | GPIOB | PB16 | J2_11 | N/A |
| PB27 | GPIOB | PB27 | J27_10 | <ul><li>PB27 can be connected to LED2 Green<br><ul><li>`J7 ON` Connect to LED2 Green<br><li>`J7 OFF` Disconnect from LED2 Green</ul></ul> |
| PA5 | SYSCTL | HFXIN | J28_4 | <ul><li>PA5 is used for HFXT functionality by default in Launchpad<br><ul><li>`Y2` crystal is soldered and connected by default</ul><br><li>PA5 can be connected to J28_4<br><ul><li>`R15` is not soldered soldered by default<br><li>Solder `R15` to connect pin to J28_4</ul> |
| PA6 | SYSCTL | HFXOUT | J28_5 | <ul><li>PA6 is used for HFXT functionality by default in Launchpad<br><ul><li>`Y2` crystal is soldered and connected by default</ul><br><li>PA6 can be connected to J28_5<br><ul><li>`R17` is not soldered soldered by default<br><li>Solder `R17` to connect pin to J28_5</ul> |
| PA13 | CANFD0 | CANRX | J4_32/J26_3 | <ul><li>PA13 can be connected to CAN/LIN connector in addition to boosterpack connector:<br><ul><li>To use on J26 CAN/LIN connector:<br>  `R65` is populated by default and connects pin to `J26_3`</ul></ul> |
| PA12 | CANFD0 | CANTX | J4_32/J26_1 | <ul><li>PA12 can be connected to CAN/LIN connector in addition to boosterpack connector:<br><ul><li>To use on J26 CAN/LIN connector:<br>  `R64` is populated by default and connects pin to `J26_1`</ul></ul> |
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
The device CAN_RX and CAN_TX are to be connected to a CAN transceiver which will route the lines to the CAN Bus.

Make the following connections, which can Invoke BSL through GPIO togggling,
- Controller NRST -> Peripheral NRST
- Controller BSL Invoke -> Peripheral BSL Invoke.

Compile, load and run the example.
Set up Initial CAN configurations as
    * CAN Mode : Classical CAN (BRS disabled)
    * Nominal Speed : 1 Mbps
    * Sampling point : 87.5
Push the S2 button to start program MSPM0G3507.

# Note
* * The Example is configured to work in CAN mode initially at 1 Mbps to get response from CAN plugin.
* To change the bitrate of communication based on the configuration obtained from host through Change Baudrate Command
    * The Data section in Change Baudrate Command is expected to match the format shown below

|     Padding      (5)    |     DRP      (5)    |     DSJW     (4)    |     DTSEG2      (4)    |     DTSEG1      (5)    |     NRP      (9)    |     NSJW      (7)    |     NSEG2      (7)    |     NTSEG1      (8)    |     BRS      (1)    |     FD      (1)    |
|------------------|-------------------------|---------------------|---------------------|------------------------|------------------------|---------------------|----------------------|-----------------------|------------------------|---------------------|

* An arbitrary CAN frame is injected into CAN bus, on changing the CAN Mode to CAN FD to calibrate the transmission delay compensation attribute values. The Identity value can be modified as required.
* Message Identifier accepted by BSL CAN Host is 0x004
* Message Identifier sent from BSL CAN HOST to BSL CAN PLUGIN is 0x003
