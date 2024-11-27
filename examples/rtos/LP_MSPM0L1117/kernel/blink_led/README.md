## Example Summary

The blink LED demo is a simple demo that implements a blinking LED with
FreeRTOS's tickless idle mode. The LED blinks every 1 second.

## Peripherals & Pin Assignments

| Peripheral | Pin | Function |
| --- | --- | --- |
| GPIOA | PA0 | Open-Drain Output |
| SYSCTL |  |  |
| EVENT |  |  |
| DEBUGSS | PA20 | Debug Clock |
| DEBUGSS | PA19 | Debug Data In Out |

## BoosterPacks, Board Resources & Jumper Settings

Visit [LP_MSPM0L1117](https://www.ti.com/tool/LP-MSPM0L1117) for LaunchPad information, including user guide and hardware files.

| Pin | Peripheral | Function | LaunchPad Pin | LaunchPad Settings |
| --- | --- | --- | --- | --- |
| PA0 | GPIOA | PA0 | J1_10 | This pin is 5V tolerant open-drain and requires pull-up.<br>J1 ON/OFF: Connect/Disconnect LED1<br>J9 1:2 3.3V Pull-up<br>J9 2:3 5V Pull-up |
| PA20 | DEBUGSS | SWCLK | N/A | J22 15:16 ON: Connect to XDS-110 SWCLK (debug) |
| PA19 | DEBUGSS | SWDIO | N/A | J22 13:14 ON: Connect to XDS-110 SWDIO (debug) |

### Device Migration Recommendations
This project was developed for a superset device included in the LP_MSPM0L1117 LaunchPad. Please
visit the [CCS User's Guide](https://software-dl.ti.com/msp430/esd/MSPM0-SDK/latest/docs/english/tools/ccs_ide_guide/doc_guide/doc_guide-srcs/ccs_ide_guide.html#manual-migration)
for information about migrating to other MSPM0 devices.

### Low-Power Recommendations
TI recommends to terminate unused pins by setting the corresponding functions to
GPIO and configure the pins to output low or input with internal
pullup/pulldown resistor.

SysConfig allows developers to easily configure unused pins by selecting **Board**→**Configure Unused Pins**.

For more information about jumper configuration to achieve low-power using the
MSPM0 LaunchPad, please visit the [LP-MSPM0L1117 User's Guide](https://www.ti.com/lit/slau873).

## Example Usage

* For **CCS**:
    Compile, load and run the example.
* For **Keil**:
    The .uvmpw project workspace file shall be used to bring the example project along with the freertos project to the IDE.
    In Keil uVision,
    * Select **Project** → **Batch Setup**
    * Select all the project targets for the build
    * Select **Project** → **Batch Build**, it will build all the projects in the workspace.
* For **IAR**:
    The .eww project workspace file shall be used to bring the example project along with the freertos project to the IDE.
    In IAR IDE:
    * Select **Project** → **Rebuild All**
    * It should first build Freertos project followed by example project.

LED1 will blink every 1s when the example run.

## Application Design Details

* This example shows how two tasks coordinate to blink LED1 every 1s.
* In order to blink LED1, one task passes a queue message every second to another task.

### FREERTOS:

* Please view the FreeRTOSConfig.h header file for example configuration information.
