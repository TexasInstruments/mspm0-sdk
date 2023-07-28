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
* This BSL example uses provided bsl_can_config.c and bsl_can_config.h files for the 
initalization and configuration of CAN modules 



## Peripherals & Pin Assignments

| Peripheral | Pin | Function |
| --- | --- | --- |
| SYSCTL | PA5 | HFXIN |
| SYSCTL | PA6 | HFXOUT |
| CANFD0 | PA13 | RX Pin |
| CANFD0 | PA12 | TX Pin |
| DEBUGSS | PA20 | Debug Clock |
| DEBUGSS | PA19 | Debug Data In Out |
| BSL | PA18 | BSL Invoke Pin |
| BSL | NRST | NRST |

## BoosterPacks, Board Resources & Jumper Settings

Visit [LP_MSPM0L1306](https://www.ti.com/tool/LP-MSPM0L1306) for LaunchPad information, including user guide and hardware files.

| Pin | Peripheral | Function | LaunchPad Pin | LaunchPad Settings |
| --- | --- | --- | --- | --- |
| PA5 | SYSCTL | FCC_IN | J28_4 | Connected to HFXT  Y2<br>Remove Y2 for other functions<br>Solder R15 to connect to J28_4 |
| PA5 | SYSCTL | HFXIN | J28_4 | Connected to HFXT  Y2<br>Solder R15 to connect to J28_4 |
| PA6 | SYSCTL | HFCLKIN | J28_5 | Connected to HFXT  Y2<br>Remove Y2 for other functions<br>Solder R17 to connect to J28_5 |
| PA6 | SYSCTL | HFXOUT | J28_5 | Connected to HFXT  Y2<br>Solder R17 to connect to J28_5 |
| PA13 | CANFD0 | CANRX | J4_32/J26_3 | R65 connects J26_3 for CAN (default) |
| PA12 | CANFD0 | CANTX | J4_32/J26_1 | R64 connects J26_1 for CAN (default) |
| PA20 | DEBUGSS | SWCLK | N/A | J101 15:16 ON: Connect to XDS-110 SWCLK (debug) |
| PA19 | DEBUGSS | SWDIO | N/A | J101 13:14 ON: Connect to XDS-110 SWDIO (debug) |

### Low-Power Recommendations
TI recommends to terminate unused pins by setting the corresponding functions to
GPIO and configure the pins to output low or input with internal
pullup/pulldown resistor.

For more information about jumper configuration to achieve low-power using the
MSPM0 LaunchPad, please visit the [LP-MSPM0L1306 User's Guide](https://www.ti.com/lit/slau869).

## Example Usage
The device CAN_RX and CAN_TX are to be connected to a CAN transceiver which will route the lines to the CAN Bus.
The CAN-FD packages on the bus can be monitored and transmitted using a host example or using any CAN Bus Analyzer tool.

Make the following connections, while using MCAN host example which can Invoke BSL through GPIO togggling, 
- Controller NRST -> Peripheral NRST
- Controller BSL Invoke -> Peripheral BSL Invoke.

Compile, load the example.
In case of Loading an updated Non Main
Load Nonmain and main sections, individually

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
* The Transceiver used to test the example is TCAN1462-Q1.
* PCAN USB Pro is used to analyse the CAN traffic on bus.