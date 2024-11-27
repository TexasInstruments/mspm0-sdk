## Example Summary

I2C interface Flash plugin handles the data transaction between the
BSL host and the ROM BSL through the following 4 API Hooks.
- Init
- Receive
- Send
- Deinit

I2C Flash plugin is primarily used to override the ROM BSL I2C
interface with custom implementation when there is a need.

Once this plugin image is loaded, ROM I2C interface will become inactive.
To revert the device to use ROM I2C use SWD_Factory_Reset.

For more details on the Flash plugin refer to BSL User Guide.

This example can be used to customize the I2C interface implementation.

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
    * 0x20007EE0 to 0x20007FFF
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
| I2C0 | PA0 | I2C Serial Data line (SDA) |
| I2C0 | PA1 | I2C Serial Clock line (SCL) |
| DEBUGSS | PA20 | Debug Clock |
| DEBUGSS | PA19 | Debug Data In Out |

## BoosterPacks, Board Resources & Jumper Settings

Visit [LP_MSPM0G3507](https://www.ti.com/tool/LP-MSPM0G3507) for LaunchPad information, including user guide and hardware files.

| Pin | Peripheral | Function | LaunchPad Pin | LaunchPad Settings |
| --- | --- | --- | --- | --- |
| PA0 | I2C0 | SDA | J27_9 | <ul><li>PA0 is 5V tolerant open-drain so it requires pull-up<br><ul><li>`J19 1:2` Use 3.3V pull-up<br><li>`J19 2:3` Use 5V pull-up</ul><br><li>PA0 can be connected to LED1<br><ul><li>`J4 OFF` Disconnect from LED1</ul></ul> |
| PA1 | I2C0 | SCL | J28_9 | <ul><li>PA1 is 5V tolerant open-drain so it requires pull-up<br><ul><li>`J20 1:2` Use 3.3V pull-up<br><li>`J20 2:3` Use 5V pull-up</ul></ul> |
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

Connect I2C_SDA and I2C_SCL with the BSL Host (any microcontroller with I2C).

Compile, load the example.

Create BSL invocation condition using BSL Invoke pin or any other invocation methods.
Send Connection command from the host. BSL Acknowledgement should be received.
Send GetDeviceInfo command from the host.
BSL should respond back with the I2C interface Flash plugin version information.

Similarly Send erase, program, verification commands to program data in the memory.
