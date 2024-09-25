## Example Summary

SPI interface Flash plugin handles the data transaction between the
BSL host and the ROM BSL through the following 4 API Hooks:
- Init
- Receive
- Send
- Deinit

SPI plugin is added as a new interface since there is no BSL SPI ROM
implementation available.

SPI flash plugin image shall be built like any other application and will be loaded to main flash.
Once this plugin image is loaded, ROM SPI interface will become active in interface list
along with UART, I2C during BSL ROM Auto detection phase.
To revert the device to disable ROM SPI interface, use SWD_Factory_Reset.

For more details on the Flash plugin refer to [BSL User Guide](https://www.ti.com/lit/ug/slau887/slau887.pdf).

This example can be used to register new SPI interface implementation.

NOTE:
While creating custom Flash plugin make sure that, Flash region in which
the Flash plugin resides and NONMAIN Flash memory are static write protected
in the BCR configuration. Otherwise there are chances for device to get locked
during the Bootloading process.

NOTE:
This BSL example uses a provided ti_msp_dl_config.h file that is not generated
by SysConfig, and it's recommended to use this provided file. If it's necessary
to generate Driverlib configuration files (ti_msp_dl_config.h/c), copy the
content of the generated ti_msp_dl_config.h file into the provided version.

## Peripherals & Pin Assignments

| Peripheral | Pin | Function |
| --- | --- | --- |
| SYSCTL |  |  |
| SPI1 | PA17 | SPI SCLK (Clock) |
| SPI1 | PB8 | SPI PICO (Peripheral In, Controller Out) |
| SPI1 | PB7 | SPI POCI (Peripheral Out, Controller In) |
| SPI1 | PB6 | SPI CS0 (Chip Select 0) |
| DEBUGSS | PA20 | Debug Clock |
| DEBUGSS | PA19 | Debug Data In Out |

## BoosterPacks, Board Resources & Jumper Settings

Visit [LP_MSPM0L1306](https://www.ti.com/tool/LP-MSPM0L1306) for LaunchPad information, including user guide and hardware files.

| Pin | Peripheral | Function | LaunchPad Pin | LaunchPad Settings |
| --- | --- | --- | --- | --- |
| PA17 | SPI1 | SCLK | J1_7 | N/A |
| PB8 | SPI1 | PICO | J2_15 | N/A |
| PB7 | SPI1 | POCI | J2_14 | N/A |
| PB6 | SPI1 | CS0 | J2_13 | <ul><li>PA8 can be connected to XDS-110 backchannel or to boosterpack connector:<br><ul><li>To use on boosterpack connector and disconnect XDS-110:<br>  `J16 2:3`: Connect pin to `J1_4`</ul></ul> |
| PA20 | DEBUGSS | SWCLK | N/A | <ul><li>PA20 is used by SWD during debugging<br><ul><li>`J101 15:16 ON` Connect to XDS-110 SWCLK while debugging<br><li>`J101 15:16 OFF` Disconnect from XDS-110 SWCLK if using pin in application</ul></ul> |
| PA19 | DEBUGSS | SWDIO | N/A | <ul><li>PA19 is used by SWD during debugging<br><ul><li>`J101 13:14 ON` Connect to XDS-110 SWDIO while debugging<br><li>`J101 13:14 OFF` Disconnect from XDS-110 SWDIO if using pin in application</ul></ul> |

### Device Migration Recommendations
This project was developed for a superset device included in the LP_MSPM0L1306 LaunchPad. Please
visit the [CCS User's Guide](https://software-dl.ti.com/msp430/esd/MSPM0-SDK/latest/docs/english/tools/ccs_ide_guide/doc_guide/doc_guide-srcs/ccs_ide_guide.html#manual-migration)
for information about migrating to other MSPM0 devices.

### Low-Power Recommendations
TI recommends to terminate unused pins by setting the corresponding functions to
GPIO and configure the pins to output low or input with internal
pullup/pulldown resistor.

SysConfig allows developers to easily configure unused pins by selecting **Board**→**Configure Unused Pins**.

For more information about jumper configuration to achieve low-power using the
MSPM0 LaunchPad, please visit the [LP-MSPM0L1306 User's Guide](https://www.ti.com/lit/slau869).

## Example Usage

Connect target with the BSL Host (any microcontroller with SPI) by making the following connections between
the SPI Controller and SPI Peripheral:
- Controller SCLK -> Peripheral SCLK
- Controller PICO -> Peripheral PICO
- Controller POCI <- Peripheral POCI
- Controller CS -> Peripheral CS
- Controller NRST -> Peripheral NRST
- Controller BSL Invoke -> Peripheral BSL Invoke.

Compile, load the example.

Create BSL invocation condition using BSL Invoke pin or any other invocation methods.
Send Connection command from the host. BSL Acknowledgement should be received.
Send GetDeviceInfo command from the host.
BSL should respond back with the SPI interface Flash plugin version information.

Similarly Send erase, program, verification commands to program data in the memory.

The given example can be paired up with `bsl_host_mcu_spi` example in sdk.
When connections are made and examples are loaded respectively, with each button press
from the host side, commands will be send and the target device will be programmed with demo application.
