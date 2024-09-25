## Example Summary

Sample application that implements a simple command interface to an I2C target (see [cmdinterface.h]).

This example is intended for use together with the `i2c_target` example.

## Peripherals & Pin Assignments

| Peripheral | Pin | Function |
| --- | --- | --- |
| SYSCTL |  |  |
| UART0 | PA11 | RX Pin |
| UART0 | PA10 | TX Pin |
| DEBUGSS | PA20 | Debug Clock |
| DEBUGSS | PA19 | Debug Data In Out |
| GPIOA | PA23 | Standard Output |
| SCL | PA1 | I2C Serial Clock Line (SCL) |
| SDA | PA0 | I2C Serial Data Line (SDA) |

## BoosterPacks, Board Resources & Jumper Settings

This example requires two LaunchPads.

* Connect the I2C pins and GND pin between two launchpads using the jumper cables.

Visit [LP_MSPM0L2228](https://www.ti.com/tool/LP-MSPM0L2228) for LaunchPad information, including user guide and hardware files.

| Pin | Peripheral | Function | LaunchPad Pin | LaunchPad Settings |
| --- | --- | --- | --- | --- |
| PA11 | UART0 | RX | J4_33/J26_6 | J22 1:2 Connect XDS-110 BackChannel UART on J101<br>J22 2:3 Connect to J4_33<br>R63 connects J26_6 for LIN (default)<br>J101 7:8 Connect XDS-110 BackChannel UART RX |
| PA10 | UART0 | TX | J4_34/J26_5 | J21 1:2 Connect XDS-110 BackChannel UART on J101<br>J21 2:3 Connect to J4_34<br>R62 connects J26_5 for LIN (default)<br>J101 9:10 Connect XDS-110 BackChannel UART TX |
| PA20 | DEBUGSS | SWCLK | N/A | J101 15:16 ON: Connect to XDS-110 SWCLK (debug) |
| PA19 | DEBUGSS | SWDIO | N/A | J101 13:14 ON: Connect to XDS-110 SWDIO (debug) |
| PA23 | GPIOA | PA23 |  | <br>J4 ON/OFF: Connect/Disconnect LED0<br>R23 can be populated to get external 3.3V Pull-up |
| PA1 | I2C1 | SCL | J1_9 | <br>J13 1:2 3.3V Pull-up<br>J13 2:3 5V Pull-up |
| PA0 | I2C1 | SDA | J1_10 | <br>J12 1:2 3.3V Pull-up<br>J12 2:3 5V Pull-up |

### Low-Power Recommendations
TI recommends to terminate unused pins by setting the corresponding functions to
GPIO and configure the pins to output low or input with internal
pullup/pulldown resistor.

SysConfig allows developers to easily configure unused pins by selecting **Board**→**Configure Unused Pins**.

For more information about jumper configuration to achieve low-power using the
MSPM0 LaunchPad, please visit the [LP-MSPM0L2228 web page](https://www.ti.com/tool/LP-MSPM0L2228).


### Device Migration Recommendations
This project was developed for a superset device included in the LP_MSPM0L2228 LaunchPad. Please
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
