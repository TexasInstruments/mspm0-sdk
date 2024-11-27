## Example Summary

The following example toggles the LED(s) using a GPIO pin interrupt.

## Peripherals & Pin Assignments

| Peripheral | Pin | Function |
| --- | --- | --- |
| SYSCTL |  |  |
| DEBUGSS | PA20 | Debug Clock |
| DEBUGSS | PA19 | Debug Data In Out |
| GPIOA | PA0 | Open-Drain Output |
| GPIOA | PA18 | Standard with Wake Input |
| GPIOB | PA5 | Standard Input with internal pull-up |
| GPIOB | PB16 | Standard Output |

## BoosterPacks, Board Resources & Jumper Settings

Visit [LP_MSPM0L1117](https://www.ti.com/tool/LP-MSPM0L1117) for LaunchPad information, including user guide and hardware files.

| Pin | Peripheral | Function | LaunchPad Pin | LaunchPad Settings |
| --- | --- | --- | --- | --- |
| PA20 | DEBUGSS | SWCLK | N/A | J22 15:16 ON: Connect to XDS-110 SWCLK (debug) |
| PA19 | DEBUGSS | SWDIO | N/A | J22 13:14 ON: Connect to XDS-110 SWDIO (debug) |
| PA0 | GPIOA | PA0 | J1_10 | This pin is 5V tolerant open-drain and requires pull-up.<br>J1 ON/OFF: Connect/Disconnect LED1<br>J9 1:2 3.3V Pull-up<br>J9 2:3 5V Pull-up |
| PA18 | GPIOA | PA18 | J3_26/J3_29 | J15 2:3 Connect to J3_29<br>J2 ON/OFF Connect/Disconnect S1 Button for BSL_Invoke (with pull-down)<br>J22 17:18 Connect/Disconnect from XDS-110 BSL_Invoke |
| PA5 | GPIOA | PA5 | N/A | J6 ON/OFF Connect/Disconnect the S2 button (no pull resistor) |
| PB16 | GPIOB | PB16 | N/A | J3 ON/OFF Connect/Disconnect LED2 Blue |

### Low-Power Recommendations

TI recommends to terminate unused pins by setting the corresponding functions to
GPIO and configure the pins to output low or input with internal
pullup/pulldown resistor.

SysConfig allows developers to easily configure unused pins by selecting **Board**→**Configure Unused Pins**.

For more information about jumper configuration to achieve low-power using the
MSPM0 LaunchPad, please visit the [LP-MSPM0L1117 web page](https://www.ti.com/tool/LP-MSPM0L1117).


### Device Migration Recommendations
This project was developed for a superset device included in the LP_MSPM0L1117 LaunchPad. Please
visit the [CCS User's Guide](https://software-dl.ti.com/msp430/esd/MSPM0-SDK/latest/docs/english/tools/ccs_ide_guide/doc_guide/doc_guide-srcs/ccs_ide_guide.html#non-sysconfig-compatible-project-migration)
for information about migrating to other MSPM0 devices.

## Example Usage

* Run the example. `CONFIG_GPIO_LED_0` turns ON LED1 to indicate driver
initialization is complete.

* LED1 is toggled by pushing S2 Button.
* LED2 is toggled by pushing S1 Button.

## Application Design Details

When this project is built, the SysConfig tool will generate the TI-Driver
configurations into the __ti_drivers_config.c__ and __ti_drivers_config.h__
files. Information on pins and resources used is present in both generated
files. Additionally, the System Configuration file (\*.syscfg) present in the
project may be opened with SysConfig's graphical user interface to determine
pins and resources used.
* `CONFIG_GPIO_LED_0` defines LED1
* `CONFIG_GPIO_LED_1` defines LED2
* `CONFIG_GPIO_BUTTON_0` defines S2 Button
* `CONFIG_GPIO_BUTTON_1` defines S1 Button
* LED1 - Indicates that the board was initialized within
`mainThread()` also toggled by S2 Button.
* LED2 - Toggled by S1 Button

* The `gpioButtonFxn0`/`gpioButtonFxn1` functions are configured in the driver configuration
file. These functions are called in the context of the GPIO interrupt.

* There is no button de-bounce logic in the example.

### FREERTOS:

* Please view the FreeRTOSConfig.h header file for example configuration information.
