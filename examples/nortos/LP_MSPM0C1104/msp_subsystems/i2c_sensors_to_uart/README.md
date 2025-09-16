## Example Summary

The following example shows I2C communication between MSPM0 and sensors HDC2010/80 and OPT3001. It demonstrates how to configure a sensor and read measurement data. LED toggles during successful transmission and stays ON if an error is detected. Result message is sent via UART and is possible to read using serial monitor. For evaluation you can use [LP-MSPM0C1104](https://www.ti.com/tool/LP-MSPM0C1104) board and sensor packs [BP-BASSENSORSMKII](https://www.ti.com/tool/BP-BASSENSORSMKII) or [BOOSTXL-BASSENSORS](https://www.ti.com/tool/BOOSTXL-BASSENSORS).

**Note**: Example requires Rev E3 or later of the MSPM0C1104 LaunchPad.

## Peripherals & Pin Assignments

| Peripheral | Pin | Function |
| --- | --- | --- |
| SYSCTL |  |  |
| DEBUGSS | PA20 | Debug Clock |
| DEBUGSS | PA19 | Debug Data In Out |

## BoosterPacks, Board Resources & Jumper Settings

Visit [LP_MSPM0C1104](https://www.ti.com/tool/LP-MSPM0C1104) for LaunchPad information, including user guide and hardware files.

| Pin | Peripheral | Function | LaunchPad Pin | LaunchPad Settings |
| --- | --- | --- | --- | --- |
| PA20 | DEBUGSS | SWCLK | J2_11 | <ul><li>PA20 is used by SWD during debugging<br><ul><li>`J101 13:14 ON` Connect to XDS-110 SWCLK while debugging<br><li>`J101 13:14 OFF` Disconnect from XDS-110 SWCLK if using pin in application</ul></ul> |
| PA19 | DEBUGSS | SWDIO | J2_17 | <ul><li>PA19 is used by SWD during debugging<br><ul><li>`J101 11:12 ON` Connect to XDS-110 SWDIO while debugging<br><li>`J101 11:12 OFF` Disconnect from XDS-110 SWDIO if using pin in application</ul></ul> |

### Device Migration Recommendations
This project was developed for a superset device included in the LP_MSPM0C1104 LaunchPad. Please
visit the [CCS User's Guide](https://software-dl.ti.com/msp430/esd/MSPM0-SDK/latest/docs/english/tools/ccs_ide_guide/doc_guide/doc_guide-srcs/ccs_ide_guide.html#sysconfig-project-migration)
for information about migrating to other MSPM0 devices.

### Low-Power Recommendations
TI recommends to terminate unused pins by setting the corresponding functions to
GPIO and configure the pins to output low or input with internal
pullup/pulldown resistor.

SysConfig allows developers to easily configure unused pins by selecting **Board**→**Configure Unused Pins**.

For more information about jumper configuration to achieve low-power using the
MSPM0 LaunchPad, please visit the [LP-MSPM0C1104 User's Guide](https://www.ti.com/lit/slau908).

## Example Usage

Connect SDA and SCL between I2C Controller and Target. Note that I2C requires pull-up resistors. They are provided already on sensor packs [BP-BASSENSORSMKII](https://www.ti.com/tool/BP-BASSENSORSMKII) and [BOOSTXL-BASSENSORS](https://www.ti.com/tool/BOOSTXL-BASSENSORS).
Compile, load and run the example. LED1 will toggle periodically till next iteration if transmission and reception were successful. LED1 will remain on if there is a problem during data transfers. Check connections and press NRST button then.
