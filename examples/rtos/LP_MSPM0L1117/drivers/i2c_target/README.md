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
| GPIOB | PB16 | Standard Output |
| GPIOB | PB17 | Standard Output |
| SCL | PA1 | I2C Serial Clock Line (SCL) |
| SDA | PA0 | I2C Serial Data Line (SDA) |

## BoosterPacks, Board Resources & Jumper Settings

This example requires two LaunchPads.

* Connect the I2C pins and GND pin between two launchpads using the jumper cables.

Visit [LP_MSPM0L1117](https://www.ti.com/tool/LP-MSPM0L1117) for LaunchPad information, including user guide and hardware files.

| Pin | Peripheral | Function | LaunchPad Pin | LaunchPad Settings |
| --- | --- | --- | --- | --- |
| PA11 | UART0 | RX | J4_33 | J12 1:2 Connect XDS-110 BackChannel UART on J22<br>J12 2:3 Connect to J4_33<br>J22 7:8 Connect XDS-110 BackChannel UART RX |
| PA10 | UART0 | TX | J4_34 | J10 1:2 Connect XDS-110 BackChannel UART on J22<br>J12 2:3 Connect to J4_34<br>J22 9:10 Connect XDS-110 BackChannel UART TX |
| PA20 | DEBUGSS | SWCLK | N/A | J22 15:16 ON: Connect to XDS-110 SWCLK (debug) |
| PA19 | DEBUGSS | SWDIO | N/A | J22 13:14 ON: Connect to XDS-110 SWDIO (debug) |
| PB16 | GPIOB | PB16 | J4_32 | This pin is used as standard output<br>J3 ON/OFF: Connect/Disconnect LED0 |
| PB17 | GPIOB | PB17 | J2_11 | This pin is standard output.<br>J4 ON/OFF: Connect/Disconnect LED1 |
| PA1 | I2C0 | SCL | J1_9 | J11 1:2 adds 3.3V pull-up |
| PA0 | I2C0 | SDA | J1_10 | J9 1:2 adds 3.3V pull-up |

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
