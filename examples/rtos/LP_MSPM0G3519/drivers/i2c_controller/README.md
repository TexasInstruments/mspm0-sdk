## Example Summary

Sample application that implements a simple command interface to an I2C target (see [cmdinterface.h]).

This example is intended for use together with the `i2ctarget` example.

## Peripherals & Pin Assignments

| Peripheral | Pin | Function |
| --- | --- | --- |
| SYSCTL |  |  |
| UART0 | PA11 | RX Pin |
| UART0 | PA10 | TX Pin |
| DEBUGSS | PA20 | Debug Clock |
| DEBUGSS | PA19 | Debug Data In Out |
| GPIOA | PA0 | Open-Drain Output |
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

* Example output is generated through use of Display driver APIs.

* Open a serial session (for example [`PuTTY`][putty-homepage]) to the appropriate COM port.
    * The COM port can be determined via Device Manager in Windows or via `ls /dev/tty*` in Linux.

The connection will have the following settings:

```text
Baud-rate:     115200
Data bits:          8
Stop bits:          1
Parity:          None
Flow Control:    None
```

Run the example.

1. __`CONFIG_GPIO_LED_0`__ turns ON to indicate the I2C driver initialization is complete.

    ```text
    Starting the i2ccontroller example
    ```

2. The I2C controller sends the `CMD_CTL_GET_STATUS` command until a valid response is received.

    ```text
    Sending GET_STATUS until target responds
    ```

3. When a valid response is received, the example displays this over UART

    ```text
    Target responded!
    ```

4. The I2C controller now enters a loop where it will loop through 4 commands.
    1. `CMD_CTL_SET_STATUS`: Writes a status byte to the target (the status value is incremented for each iteration).
    2. `CMD_CTL_GET_STATUS`: Reads status byte from the target.
    3. `CMD_CTL_WRITE_BLOCK`: Write data block to the target.
    4. `CMD_CTL_READ_BLOCK`: Read data block from the target

    For each command, the controller displays the result over UART and the task sleeps for 1 second.

    ```text
    Sending SET_STATUS = 0
    Sending GET_STATUS
    GET_STATUS returned 0
    Sending WRITE_BLOCK 'Hi, I am the controller'
    Sending READ_BLOCK
    READ_BLOCK returned 'Hello, I am the target!'
    ```

* If the I2C communication fails, an error message describing
the failure is displayed.

    ```text
    I2C target address 0x48 not acknowledged!
    ```

## Application Design Details

This application uses one task:

`'mainThread'` - performs the following actions:

1. Opens and initializes an I2C driver object.

2. Uses the I2C driver in blocking mode to send commands over the defined command interface

3. Prints command information via the UART.

4. The task sleeps for 1 second between each command.

FreeRTOS:

* Please view the `FreeRTOSConfig.h` header file for example configuration
information.

[putty-homepage]: http://www.putty.org "PuTTY's homepage"
[cmdinterface.h]: ./cmdinterface.h
