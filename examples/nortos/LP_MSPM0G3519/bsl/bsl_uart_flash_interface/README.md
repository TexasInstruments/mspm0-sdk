## Example Summary

UART interface Flash plugin handles the data transaction between the
BSL host and the ROM BSL through the following 4 API Hooks.
- Init
- Receive
- Send
- Deinit

UART Flash plugin is primarily used to override the ROM BSL UART
interface with custom implementation when there is a need.

Once this plugin image is loaded, ROM UART interface will become inactive.
To revert the device to use ROM UART use SWD_Factory_Reset.

For more details on the Flash plugin refer to BSL User Guide.

This example can be used to customize the UART interface implementation.

NOTE:
---
* While creating Custom Flash plugin make sure that, Flash region in which the
Flash plugin resides and Non-main Flash memory are Static write protected in the BCR configuration.
* Otherwise there are chances for device to get locked during the Bootloading process.
* This BSL example uses a provided ti_msp_dl_config.h file that is not generated
by SysConfig, and it's recommended to use this provided file. 
* If it's necessary to generate Driverlib configuration files (ti_msp_dl_config.h/c), copy the content of the generated ti_msp_dl_config.h file into the provided version.

* The following SRAM memory are marked as reserved for the ROM BSL execution
    * 0x20000000 to 0x20000160
    * 0x2000FE00 to 0x2000FFFF
* The Plugin used for SRAM is allotted from 0x20000160

TIPS:
---
* If SRAM size for plugin(0xFF) configured in Non main is not sufficient, 
  users can consume required space at the end of first Buffer in SRAM, 
  but this will impact the maximum BSL packet size.
  The plugin SRAM size in BSL user config can be updated to 0 and free that space for buffers.
 
## Peripherals & Pin Assignments

| Peripheral | Pin | Function |
| --- | --- | --- |
| SYSCTL |  |  |
| UART0 | PA11 | RX Pin |
| UART0 | PA10 | TX Pin |
| DEBUGSS | PA20 | Debug Clock |
| DEBUGSS | PA19 | Debug Data In Out |

## BoosterPacks, Board Resources & Jumper Settings

Visit [LP_MSPM0G3519](https://www.ti.com/tool/LP-MSPM0G3519) for LaunchPad information, including user guide and hardware files.

| Pin | Peripheral | Function | LaunchPad Pin | LaunchPad Settings |
| --- | --- | --- | --- | --- |
| PA11 | UART0 | RX | J3_29 | N/A |
| PA10 | UART0 | TX | J3_30 | N/A |
| PA20 | DEBUGSS | SWCLK | N/A | <ul><li>PA20 is used by SWD during debugging<br><ul><li>`J101 15:16 ON` Connect to XDS-110 SWCLK while debugging<br><li>`J101 15:16 OFF` Disconnect from XDS-110 SWCLK if using pin in application</ul></ul> |
| PA19 | DEBUGSS | SWDIO | N/A | <ul><li>PA19 is used by SWD during debugging<br><ul><li>`J101 13:14 ON` Connect to XDS-110 SWDIO while debugging<br><li>`J101 13:14 OFF` Disconnect from XDS-110 SWDIO if using pin in application</ul></ul> |

### Device Migration Recommendations
This project was developed for a superset device included in the LP_MSPM0G3519 LaunchPad. Please
visit the [CCS User's Guide](https://software-dl.ti.com/msp430/esd/MSPM0-SDK/latest/docs/english/tools/ccs_ide_guide/doc_guide/doc_guide-srcs/ccs_ide_guide.html#manual-migration)
for information about migrating to other MSPM0 devices.

### Low-Power Recommendations
TI recommends to terminate unused pins by setting the corresponding functions to
GPIO and configure the pins to output low or input with internal
pullup/pulldown resistor.

SysConfig allows developers to easily configure unused pins by selecting **Board**→**Configure Unused Pins**.

For more information about jumper configuration to achieve low-power using the
MSPM0 LaunchPad, please visit the [LP_MSPM0G3519 User's Guide](https://www.ti.com/tool/LP-MSPM0G3519#tech-docs).

## Example Usage

Connect UART_RX and UART_TX with the BSL Host (any microcontroller with UART).

Compile, load the example.

Create BSL invocation condition using BSL Invoke pin or any other invocation methods.
Send Connection command from the host. BSL Acknowledgement should be received.
Send GetDeviceInfo command from the host.
BSL should respond back with the UART interface Flash plugin version information.

Similarly Send erase, program, verification commands to program data in the memory.
