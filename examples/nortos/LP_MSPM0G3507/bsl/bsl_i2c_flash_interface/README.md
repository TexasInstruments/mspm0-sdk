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
While creating Custom Flash plugin make sure that, Flash region in which
the Flash plugin resides and Non-main Flash memory are Static write protected
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
| I2C0 | PA0 | I2C Serial Data line (SDA) |
| I2C0 | PA1 | I2C Serial Clock line (SCL) |
| DEBUGSS | PA20 | Debug Clock |
| DEBUGSS | PA19 | Debug Data In Out |

## BoosterPacks, Board Resources & Jumper Settings

Visit [LP_MSPM0G3507](https://www.ti.com/tool/LP-MSPM0G3507) for LaunchPad information, including user guide and hardware files.

| Pin | Peripheral | Function | LaunchPad Pin | LaunchPad Settings |
| --- | --- | --- | --- | --- |
| PA0 | I2C0 | SDA | J27_9 | This pin is 5V tolerant open-drain and requires pull-up.<br>J4 OFF: Disconnect LED1<br>J19 1:2 3.3V Pull-up<br>J19 2:3 5V Pull-up |
| PA1 | I2C0 | SCL | J28_9 | This pin is 5V tolerant open-drain and requires pull-up.<br>J20 1:2 3.3V Pull-up<br>J20 2:3 5V Pull-up |
| PA20 | DEBUGSS | SWCLK | N/A | J101 15:16 ON: Connect to XDS-110 SWCLK (debug) |
| PA19 | DEBUGSS | SWDIO | N/A | J101 13:14 ON: Connect to XDS-110 SWDIO (debug) |

### Low-Power Recommendations
TI recommends to terminate unused pins by setting the corresponding functions to
GPIO and configure the pins to output low or input with internal
pullup/pulldown resistor.

SysConfig allows developers to easily configure unused pins by selecting **Board**→**Configure Unused Pins**.

For more information about jumper configuration to achieve low-power using the
MSPM0 LaunchPad, please visit the [LP-MSPM0G3507 User's Guide](https://www.ti.com/lit/slau846).

## Example Usage

Connect I2C_SDA and I2C_SCL with the BSL Host (any microcontroller with I2C).

Compile, load the example.

Create BSL invocation condition using BSL Invoke pin or any other invocation methods.
Send Connection command from the host. BSL Acknowledgement should be received.
Send GetDeviceInfo command from the host.
BSL should respond back with the I2C interface Flash plugin version information.

Similarly Send erase, program, verification commands to program data in the memory.
