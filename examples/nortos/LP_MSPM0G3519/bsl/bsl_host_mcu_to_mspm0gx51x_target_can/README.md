## Example Summary

This demo shows how to program a MSPM0 MCU using a LP-MSPM0G3519 as host by BSL CAN interface.

## Peripherals & Pin Assignments

| Peripheral | Pin | Function |
| --- | --- | --- |
| GPIOA | PA0 | Open-Drain Output (LED) |
| GPIOB | PB3 | Standard Input with internal pull-up (Button) |
| GPIOB | PB0 | Standard Output (NRST Control) |
| GPIOB | PB16 | Standard Output (BSL Invoke Control) |
| GPIOB | PB27 | Standard Output (Error LED) |
| SYSCTL | PA5 | HFXIN |
| SYSCTL | PA6 | HFXOUT |
| EVENT |  |  |
| CANFD0 | PA13 | RX Pin |
| CANFD0 | PA12 | TX Pin |
| DEBUGSS | PA20 | Debug Clock |
| DEBUGSS | PA19 | Debug Data In Out |

## BoosterPacks, Board Resources & Jumper Settings

Visit [LP_MSPM0G3519](https://www.ti.com/tool/LP-MSPM0G3519) for LaunchPad information, including user guide and hardware files.

| Pin | Peripheral | Function | LaunchPad Pin | LaunchPad Settings |
| --- | --- | --- | --- | --- |
| PA0 | GPIOA | PA0 (LED Output) | Red LED | <ul><li>PA0 is 5V tolerant open-drain so it requires pull-up<br><ul><li>`J16 1:2` Use 3.3V pull-up<br><li>`J16 2:3` Use 5V pull-up</ul><br><li>PA0 can be connected to Red LED<br><ul><li>`J18 ON` Connect to Red LED<br><li>`J18 OFF` Disconnect from Red LED</ul></ul> |
| PB3 | GPIOB | PB3 (Button Input) | S2 Button | <ul><li>PB3 is connected to S2 button to GND with no external pull resistor<br><ul><li>Press `S2` button to connect pin to GND<br><li>`J23 ON` Connect to S2 button<br><li>`J23 OFF` Disconnect from S2 button</ul></ul> |
| PB0 | GPIOB | PB0 (NRST Control) | BoosterPack | N/A |
| PB16 | GPIOB | PB16 (BSL Invoke) | BoosterPack | N/A |
| PB27 | GPIOB | PB27 (Error LED) | RGB LED Green | <ul><li>PB27 can be connected to RGB LED Green<br><ul><li>`J21 ON` Connect to RGB LED Green<br><li>`J21 OFF` Disconnect from RGB LED Green</ul></ul> |
| PA5 | SYSCTL | HFXIN | HFXT Crystal | <ul><li>PA5 is used for HFXT functionality by default in LaunchPad<br><ul><li>`Y2` 40MHz crystal is populated and connected by default</ul><br><li>PA5 can be connected to boosterpack<br><ul><li>`R15` is not populated by default<br><li>Solder `R15` to connect pin to boosterpack</ul></ul> |
| PA6 | SYSCTL | HFXOUT | HFXT Crystal | <ul><li>PA6 is used for HFXT functionality by default in LaunchPad<br><ul><li>`Y2` 40MHz crystal is populated and connected by default</ul><br><li>PA6 can be connected to boosterpack<br><ul><li>`R17` is not populated by default<br><li>Solder `R17` to connect pin to boosterpack</ul></ul> |
| PA13 | CANFD0 | CANRX | J26_3 (CAN Connector) | <ul><li>PA13 can be connected to CAN/LIN connector in addition to boosterpack connector:<br><ul><li>To use on J26 CAN/LIN connector:<br>  `R65` is populated by default and connects pin to `J26_3`</ul></ul> |
| PA12 | CANFD0 | CANTX | J26_1 (CAN Connector) | <ul><li>PA12 can be connected to CAN/LIN connector in addition to boosterpack connector:<br><ul><li>To use on J26 CAN/LIN connector:<br>  `R64` is populated by default and connects pin to `J26_1`</ul></ul> |
| PA20 | DEBUGSS | SWCLK | Debug Interface | <ul><li>PA20 is used by SWD during debugging<br><ul><li>`J101 15:16 ON` Connect to XDS-110 SWCLK while debugging<br><li>`J101 15:16 OFF` Disconnect from XDS-110 SWCLK if using pin in application</ul></ul> |
| PA19 | DEBUGSS | SWDIO | Debug Interface | <ul><li>PA19 is used by SWD during debugging<br><ul><li>`J101 13:14 ON` Connect to XDS-110 SWDIO while debugging<br><li>`J101 13:14 OFF` Disconnect from XDS-110 SWDIO if using pin in application</ul></ul> |

### Required Jumper Settings for BSL Host CAN Example

**Essential Jumper Configuration:**
- **J18**: Populated (ON) - Connects PA0 to Red LED for status indication
- **J21**: Populated (ON) - Connects PB27 to RGB LED Green for error indication
- **J23**: Populated (ON) - Connects PB3 to S2 button for user input
- **J16**: Set to 1:2 position - Provides 3.3V pull-up for PA0 (open-drain output)
- **J101**: All jumpers populated (ON) - Enables XDS110 debug probe connections
- **R64, R65**: Populated by default - Enables CAN TX/RX on J26 CAN connector

**CAN Interface Configuration:**
- **J26**: 3-pin CAN/LIN connector
  - Pin 1: CAN TX (PA12)
  - Pin 3: CAN RX (PA13)
  - Pin 2: GND

**BSL Control Pins:**
- **PB0**: NRST control output (connects to target device NRST)
- **PB16**: BSL invoke control output (connects to target device BSL invoke pin)

### Device Migration Recommendations
This project was developed for a superset device included in the LP_MSPM0G3519 LaunchPad. Please
visit the [CCS User's Guide](https://software-dl.ti.com/msp430/esd/MSPM0-SDK/latest/docs/english/tools/ccs_ide_guide/doc_guide/doc_guide-srcs/ccs_ide_guide.html#sysconfig-project-migration)
for information about migrating to other MSPM0 devices.

### Low-Power Recommendations
TI recommends to terminate unused pins by setting the corresponding functions to
GPIO and configure the pins to output low or input with internal
pullup/pulldown resistor.

SysConfig allows developers to easily configure unused pins by selecting **Board**→**Configure Unused Pins**.

For more information about jumper configuration to achieve low-power using the
MSPM0 LaunchPad, please visit the [LP-MSPM0G3519 User's Guide](https://www.ti.com/lit/slau947).

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
Push the S2 button to start programming the target MSPM0 device.
Note: if use software trigger need the application code(include software invoke) exist on the chip.
# Note
* * The Example is configured to work in CAN mode initially at 1 Mbps to get response from CAN plugin.
* To change the bitrate of communication based on the configuration obtained from host through Change Baudrate Command
    * The Data section in Change Baudrate Command is expected to match the format shown below

|     Padding      (5)    |     DRP      (5)    |     DSJW     (4)    |     DTSEG2      (4)    |     DTSEG1      (5)    |     NRP      (9)    |     NSJW      (7)    |     NSEG2      (7)    |     NTSEG1      (8)    |     BRS      (1)    |     FD      (1)    |
|------------------|-------------------------|---------------------|---------------------|------------------------|------------------------|---------------------|----------------------|-----------------------|------------------------|---------------------|

* An arbitrary CAN frame is injected into CAN bus, on changing the CAN Mode to CAN FD to calibrate the transmission delay compensation attribute values. The Identity value can be modified as required.
* Message Identifier accepted by BSL CAN Host is 0x004
* Message Identifier sent from BSL CAN HOST to BSL CAN PLUGIN is 0x003
