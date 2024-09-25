## Example Summary

CAN interface Flash plugin handles the data transaction between the
BSL host and the ROM BSL through the following 4 API Hooks.
- Init
- Receive
- Send
- Deinit

CAN plug-in is added as a new interface since there is no BSL CAN ROM implementation available.

CAN flash plugin image shall be built like any other application and will be loaded to main flash.
Once this plugin image is loaded, the CAN interface will become active in interface list
along with UART, I2C during BSL ROM Auto detection phase.
To revert the device to disable ROM CAN interface, use SWD_Factory_Reset.

For more details on the Flash plugin refer to [MSPM0 Bootloader User's Guide](https://www.ti.com/lit/slau887).

This example can be used to register new CAN interface implementation.

NOTE:
* While creating Custom Flash plugin make sure that, Flash region in which the
Flash plugin resides and Non-main Flash memory are Static write protected in the BCR configuration.
* Otherwise there are chances for device to get locked during the Bootloading process.
* Refer to "boot_config.c" and "boot_config.h" to understand the configuration used for Non Main.
* This BSL example uses "bsl_mcan_flash_interface.syscfg" for the initalization and configuration of CAN modules



## Peripherals & Pin Assignments

| Peripheral | Pin | Function |
| --- | --- | --- |
| SYSCTL | PA5 | HFXIN |
| SYSCTL | PA6 | HFXOUT |
| CANFD0 | PA13 | RX Pin |
| CANFD0 | PA12 | TX Pin |
| DEBUGSS | PA20 | Debug Clock |
| DEBUGSS | PA19 | Debug Data In Out |

## BoosterPacks, Board Resources & Jumper Settings

Visit [LP_MSPM0G3507](https://www.ti.com/tool/LP-MSPM0G3507) for LaunchPad information, including user guide and hardware files.

| Pin | Peripheral | Function | LaunchPad Pin | LaunchPad Settings |
| --- | --- | --- | --- | --- |
| PA5 | SYSCTL | HFXIN | J28_4 | <ul><li>PA5 is used for HFXT functionality by default in Launchpad<br><ul><li>`Y2` crystal is soldered and connected by default</ul><br><li>PA5 can be connected to J28_4<br><ul><li>`R15` is not soldered soldered by default<br><li>Solder `R15` to connect pin to J28_4</ul> |
| PA6 | SYSCTL | HFXOUT | J28_5 | <ul><li>PA6 is used for HFXT functionality by default in Launchpad<br><ul><li>`Y2` crystal is soldered and connected by default</ul><br><li>PA6 can be connected to J28_5<br><ul><li>`R17` is not soldered soldered by default<br><li>Solder `R17` to connect pin to J28_5</ul> |
| PA13 | CANFD0 | CANRX | J4_32/J26_3 | <ul><li>PA13 can be connected to CAN/LIN connector in addition to boosterpack connector:<br><ul><li>To use on J26 CAN/LIN connector:<br>  `R65` is populated by default and connects pin to `J26_3`</ul></ul> |
| PA12 | CANFD0 | CANTX | J4_32/J26_1 | <ul><li>PA12 can be connected to CAN/LIN connector in addition to boosterpack connector:<br><ul><li>To use on J26 CAN/LIN connector:<br>  `R64` is populated by default and connects pin to `J26_1`</ul></ul> |
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
The device CAN_RX and CAN_TX are to be connected to a CAN transceiver which will route the lines to the CAN Bus.
The CAN-FD packages on the bus can be monitored and transmitted using a host example or using any CAN Bus Analyzer tool.

Make the following connections, while using MCAN host example which can Invoke BSL through GPIO togggling,
- Controller NRST -> Peripheral NRST
- Controller BSL Invoke -> Peripheral BSL Invoke.

Compile, load the example.
Set up Initial CAN configurations as
    * CAN Mode : Classical CAN (BRS disabled)
    * Nominal Speed : 1 Mbps
    * Sampling point : 87.5

Create BSL invocation condition using BSL Invoke pin or any other invocation methods.
Send Connection command from the host. BSL Acknowledgement should be received.
Send GetDeviceInfo command from the host.
BSL should respond back with the CAN interface Flash plugin version information.
Similarly Send erase, program, verification commands to program data in the memory.

# Note

* The Example is configured to work in CAN mode initially at 1 Mbps.
* To change the bitrate of communication based on the configuration obtained from host through Change Baudrate Command
    * The Data section in Change Baudrate Command is expected to match the format shown below

|     Padding      (5)    |     DRP      (5)    |     DSJW     (4)    |     DTSEG2      (4)    |     DTSEG1      (5)    |     NRP      (9)    |     NSJW      (7)    |     NSEG2      (7)    |     NTSEG1      (8)    |     BRS      (1)    |     FD      (1)    |
|------------------|-------------------------|---------------------|---------------------|------------------------|------------------------|---------------------|----------------------|-----------------------|------------------------|---------------------|

* An arbitrary CAN frame is injected into CAN bus, on changing the CAN Mode to CAN FD to calibrate the transmission delay compensation attribute values. The Identity value can be modified as required.
* Message Identifier accepted by BSL Plugin is 0x003
* Message Identifier sent from BSL Plugin is 0x004
