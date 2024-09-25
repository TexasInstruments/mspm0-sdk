## Example Summary
* Flash BSL is developed to provide BSL features in a device not having ROM BSL.
* The Memory Section is altered to achieve this.(refer mspm0c1104.cmd)
* Flash Bootloader supports programming / verifying data in the memory.
* It can be invoked in any of the three way as follows:
	* No Application at APP START
	* BSL PIN Inovation
	* Application based invocation
* It supports the following major functions
	* Program data
	* Get Device info
	* Flash memory erase
	* Readback data
	* CRC verification
	* Start Application
* One of UART/I2C interface can be used to communicate with the Host.
* By default flash BSL occupies the first 6 KB of flash.
* This region is static write protected using bootcode user configuration. ( refer "boot_config.c" )
* Once this image is loaded to the device, the FLash Bootloader will be active. To revert the device to default state SWD_Factory_Reset command has to be used.
* Users can alter the interface pins/ invoke pin in bootloader user configuration (refer boot_config.c)
* The user application is expected to start at 0x1800 by default.

## Flash BSL feature selection
* The following features are Macro based and can be enabled/disabled to save memory consumption (refer "flashBSL_modules.h")
	* UART_INTERFACE
	* I2C_INTERFACE
	* BSL_PIN_INVOKE
	* BSL_BLANK_FLASH_INVOKE
	* BSL_CMD_FLASH_RANGE_ERASE
	* BSL_CMD_MASS_ERASE
	* BSL_CMD_MEMORY_READ_BACK
	* BSL_CMD_STANDALONE_VERIFICATION
	* BSL_CMD_CHANGE_BAUDRATE
	* BSL_CMD_GET_IDENTITY
	* BSL_TIMEOUT

NOTE:
* Always make sure to perform factory reset on device before loading FlashBSL example to avoid *Lockout scenarios*
* By enabling/ disabling features, the RAM buffer size for BSL communication and Flash start address for Application are altered. The values are to be updated in `flash_bsl_defines.h` accordingly
* While creating flash bootloader make sure that, flash bootloader is static write protected. Otherwise there are chances for device to get locked during the boot loading process.

For more details on BSL communication interface refer to BSL User Guide.

## Peripherals & Pin Assignments

| Peripheral | Pin | Function |
| --- | --- | --- |
| DEBUGSS | PA20 | Debug Clock |
| DEBUGSS | PA19 | Debug Data In Out |
| UART0 | PA24 | RX Pin |
| UART0 | PA27 | TX Pin |
| I2C0 | PA0 | I2C Serial Data line (SDA) |
| I2C0 | PA11 | I2C Serial Clock line (SCL) |



## BoosterPacks, Board Resources & Jumper Settings
Visit [LP_MSPM0C1104](https://www.ti.com/tool/LP-MSPM0C1104) for LaunchPad information, including user guide and hardware files.

| Pin | Peripheral | Function | LaunchPad Pin | LaunchPad Settings |
| --- | --- | --- | --- | --- |
| PA20 | DEBUGSS | SWCLK | N/A | <ul><li>PA20 is used by SWD during debugging<br><ul><li>`J101 15:16 ON` Connect to XDS-110 SWCLK while debugging<br><li>`J101 15:16 OFF` Disconnect from XDS-110 SWCLK if using pin in application</ul></ul> |
| PA19 | DEBUGSS | SWDIO | N/A | <ul><li>PA19 is used by SWD during debugging<br><ul><li>`J101 13:14 ON` Connect to XDS-110 SWDIO while debugging<br><li>`J101 13:14 OFF` Disconnect from XDS-110 SWDIO if using pin in application</ul></ul> |
| PA0 | I2C0 | SDA | J1_10 | <ul><li>PA0 is 5V tolerant open-drain so it requires pull-up<br><ul><li>`J20 1:2` Use 3.3V pull-up<br><li>`J20 2:3` Use 5V pull-up</ul></ul> |
| PA11 | I2C0 | SCL | J1_9 | <ul><li>PA11 can be connected to an external 3.3V pull-up<br><ul><li>`J6 OFF` Disconnect 3.3V pull-up<br><li>`J6 ON` Connect 3.3V pull-up</ul></ul> |
| PA26 | UART0 | RX | J1_3 | <ul><li>PA26 can be used as UART RX connected to XDS-110 backchannel or to boosterpack connector:<br><ul><li>To use backchannel UART on J101:<br>  `J101 5:6 ON` Connects XDS-110 backchannel to UART RX<br><li>To use UART on boosterpack connector:<br>  `J101 5:6 OFF`: Disconnects XDS-110 backchannel</ul></ul> |
| PA27 | UART0 | TX | J1_4 | <ul><li>PA27 can be used as UART TX connected to XDS-110 backchannel or to boosterpack connector:<br><ul><li>To use backchannel UART on J101:<br>  `J101 7:8 ON` Connects XDS-110 backchannel to UART TX<br><li>To use UART on boosterpack connector:<br>  `J101 7:8 OFF`: Disconnects XDS-110 backchannel</ul></ul> |

### Device Migration Recommendations
This project was developed for a superset device included in the LP_MSPM0C1104 LaunchPad. Please
visit the [CCS User's Guide](https://software-dl.ti.com/msp430/esd/MSPM0-SDK/latest/docs/english/tools/ccs_ide_guide/doc_guide/doc_guide-srcs/ccs_ide_guide.html#non-sysconfig-compatible-project-migration)
for information about migrating to other MSPM0 devices.

## Example Usage

Enable only the required BSL functionalities in `flash_bsl_modules.h` 
Connect UART_RX and UART_TX or I2C_SDA and I2C SCL with the BSL Host (any microcontroller with UART/I2C respectively).
Compile, load the example.
Create BSL invocation condition using BSL Invoke pin or any other invocation methods.
Send GetDeviceInfo command from the host.
Device should respond back with the version information and SRAM buffer space available.
Similarly Send erase, program, verification commands to program data in the memory.
