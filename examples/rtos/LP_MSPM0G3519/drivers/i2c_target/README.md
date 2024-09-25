## Example Summary

Sample application using the I2CTarget driver to be an I2C target device.
The example implements a simple command interface (see [cmdinterface.h]).

## Peripherals & Pin Assignments

| Peripheral | Pin | Function |
| --- | --- | --- |
| SYSCTL |  |  |
| UART0 | PA11 | RX Pin |
| UART0 | PA10 | TX Pin |
| DEBUGSS | PA20 | Debug Clock |
| DEBUGSS | PA19 | Debug Data In Out |
| GPIOA | PA0 | Open-Drain Output |
| GPIOB | PB26 | Standard Output |
| SCL | PC2 | I2C Serial Clock Line (SCL) |
| SDA | PC3 | I2C Serial Data Line (SDA) |

## BoosterPacks, Board Resources & Jumper Settings

This example requires two LaunchPads.

* Connect the I2C pins and GND pin between two launchpads using the jumper cables.

Visit [LP_MSPM0G3519](https://www.ti.com/tool/LP-MSPM0G3519) for LaunchPad information, including user guide and hardware files.

| Pin | Peripheral | Function | LaunchPad Pin | LaunchPad Settings |
| --- | --- | --- | --- | --- |
| PA11 | UART0 | RX | J4_33 | J15 2:3 Connect XDS-110 BackChannel UART on J19<br>J15 1:2 Connect to J4_33<br>J19 7:8 Connect XDS-110 BackChannel UART RX |
| PA10 | UART0 | TX | J4_34 | J14 2:3 Connect XDS-110 BackChannel UART on J19<br>J14 1:2 Connect to J4_34<br>J19 9:10 Connect XDS-110 BackChannel UART TX |
| PA0 | GPIOA | PA0 | J5_10 | <ul><li>PA0 is 5V tolerant open-drain so it requires pull-up<br><ul><li>`J2 1:2` Use 3.3V pull-up<br><li>`J2 2:3` Use 5V pull-up</ul><br><li>PA0 can be connected to LED1<br><ul><li>`J6 ON` Connect to LED1<br><li>`J6 OFF` Disconnect from LED1</ul></ul> |
| PA20 | DEBUGSS | SWCLK | N/A | J19 15:16 ON: Connect to XDS-110 SWCLK (debug) |
| PA19 | DEBUGSS | SWDIO | N/A | J19 13:14 ON: Connect to XDS-110 SWDIO (debug) |
| PC2 | I2C2 | SCL | J1_9 | J16 Jumper should be added to enable the pull-up |
| PC3 | I2C2 | SDA | J1_10 | J17 Jumper should be added to enable the pull-up |
| PB26 | GPIOB | PB26 | J27_8 | This pin is standard output.<br>J6 ON/OFF: Connect/Disconnect LED1 |

### Low-Power Recommendations
TI recommends to terminate unused pins by setting the corresponding functions to
GPIO and configure the pins to output low or input with internal
pullup/pulldown resistor.

SysConfig allows developers to easily configure unused pins by selecting **Board**→**Configure Unused Pins**.

For more information about jumper configuration to achieve low-power using the
MSPM0 LaunchPad, please visit the [LP-MSPM0G3519 web page](https://www.ti.com/tool/LP-MSPM0G3519).


### Device Migration Recommendations
This project was developed for a superset device included in the LP_MSPM0G3519 LaunchPad. Please
visit the [CCS User's Guide](https://software-dl.ti.com/msp430/esd/MSPM0-SDK/latest/docs/english/tools/ccs_ide_guide/doc_guide/doc_guide-srcs/ccs_ide_guide.html#non-sysconfig-compatible-project-migration)
for information about migrating to other MSPM0 devices.

## Example Usage

Run the example.

1. __`CONFIG_GPIO_LED_0`__ turns ON to indicate the I2CTarget driver initialization is complete.

2. __`CONFIG_GPIO_LED_1`__ toggles when commands are received over the `CONFIG_I2C_TARGET_0` I2C bus.

3. The example implements 4 commands (see [cmdinterface.h])
    * `CMD_CTL_SET_STATUS`: The example receives 1 byte and stores it in `rxBuffer[0]`
    * `CMD_CTL_GET_STATUS`: The example returns the value stored in `rxBuffer[0]`
    * `CMD_CTL_WRITE_BLOCK`: The example receives N bytes and stores it in `rxBuffer`
    * `CMD_CTL_READ_BLOCK`: The example fills `txBuffer` with 'Hello, I am the target!' and returns the N bytes
      requested by the I2C controller.

## Application Design Details

This application uses one task:

`'mainThread'` - performs the following actions:

1. Opens and initializes an I2CTarget driver object.

2. Starts I2CTarget driver to send/receive data over the defined command interface

3. The task wakes up every 1 second.

FreeRTOS:

* Please view the `FreeRTOSConfig.h` header file for example configuration
information.

[cmdinterface.h]: ./cmdinterface.h
