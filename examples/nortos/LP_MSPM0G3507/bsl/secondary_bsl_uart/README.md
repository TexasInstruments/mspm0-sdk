## Example Summary

Secondary Bootloader supports programming / verifying data in the memory,
with same BSL protocol format as Primary BSL (ROM BSL) in the device.
It can be invoked in the same way as the ROM BSL.

It supports the following major functions
- Program data
- Flash memory erase
- Readback data
- CRC verification
- Start Application

It uses UART interface to communicate with the Host.

This example takes care of secondary Bootloader implementation as well as
the registration of it. Hence once this image is loaded to the device, the
primary Bootloader in the device can't be used. Only the secondary Bootloader
will be active. To revert the device to use primary Bootloader
SWD_Factory_Reset command has to be used.

For more details refer to BSL User Guide.

This example can be used to create custom Bootloader.

NOTE:
While creating Custom Bootloader make sure that, Flash region in which
the custom Bootloader resides is Static write protected in the
BCR configuration. Otherwise there are chances for device to get locked
during the Bootloading process.

NOTE:
This BSL example uses a provided ti_msp_dl_config.h file that is not generated
by SysConfig, and it's recommended to use this provided file. If it's necessary
to generate Driverlib configuration files (ti_msp_dl_config.h/c), copy the
content of the generated ti_msp_dl_config.h file into the provided version.

## Peripherals & Pin Assignments

| Peripheral | Pin | Function |
| --- | --- | --- |
| DEBUGSS | PA20 | Debug Clock |
| DEBUGSS | PA19 | Debug Data In Out |

## BoosterPacks, Board Resources & Jumper Settings

Visit [LP_MSPM0G3507](https://www.ti.com/tool/LP-MSPM0G3507) for LaunchPad information, including user guide and hardware files.

| Pin | Peripheral | Function | LaunchPad Pin | LaunchPad Settings |
| --- | --- | --- | --- | --- |
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

Connect UART_RX and UART_TX with the BSL Host (any microcontroller with UART).

Compile, load the example.

Create BSL invocation condition using BSL Invoke pin or any other invocation methods.
Send GetDeviceInfo command from the host.
Device should respond back with the version information and SRAM buffer space available.

Similarly Send erase, program, verification commands to program data in the memory.
