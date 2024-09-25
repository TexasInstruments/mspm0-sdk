## Example Summary

The following example reads the temperature from a TMP117 sensor via I2C.

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

This example requires a [__BP-BASSENSORSMKII BoosterPack__](https://www.ti.com/tool/BP-BASSENSORSMKII).

* Connect booster pack J1, J2, J3 and J4 on the LaunchPad.
* Connect J1 pin 1 to J3 pin 28 to apply 3.3V to the BP-BASSENSORSMKII.
* Connect TMP117 temperature sensor break away board to the on board TMP117 temperature connector using the ribbon cable provided.

Visit [LP_MSPM0G3507](https://www.ti.com/tool/LP-MSPM0G3507) for LaunchPad information, including user guide and hardware files.

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
MSPM0 LaunchPad, please visit the [LP-MSPM0G3507 web page](https://www.ti.com/tool/LP-MSPM0G3507).


### Device Migration Recommendations
This project was developed for a superset device included in the LP_MSPM0G3507 LaunchPad. Please
visit the [CCS User's Guide](https://software-dl.ti.com/msp430/esd/MSPM0-SDK/latest/docs/english/tools/ccs_ide_guide/doc_guide/doc_guide-srcs/ccs_ide_guide.html#non-sysconfig-compatible-project-migration)
for information about migrating to other MSPM0 devices.

## Example Usage

Example output is generated through use of Display driver APIs.

* Open a serial session (e.g. [`PuTTY`](http://www.putty.org/ "PuTTY's
 Homepage"), etc.) to the appropriate COM port.
  * The COM port can be determined via Device Manager in Windows or via `ls /dev/tty*` in Linux.

* The connection will have the following settings:

```text
Baud-rate:     115200
Data bits:          8
Stop bits:          1
Parity:          None
Flow Control:    None
```

* Run the example.

1. `CONFIG_GPIO_LED_0` turns ON LED1 to indicate the I2C driver initialization is complete.

    ```text
    I2C Initialized!
    ```

2. If required, the TMP sensor is enabled using a GPIO output.

3. The I2C bus is queried for all supported TMP sensors. The output varies depending on
which TMP sensor(s) is found.

    ```text
    I2C target address 0x49 not acknowledged!
    Detected TMP117 sensor with target address 0x48
    ```

4. The last known TMP sensor will be used to read temperature samples. Samples
are converted to celsius and displayed via UART.

    ```text
    Sample 0: 24 C
    Sample 1: 24 C
        .
        .
        .
    Sample 18: 24 C
    Sample 19: 24 C
    ```

5. After a number of samples, the I2C driver instance is closed.

    ```text
    I2C closed!
    ```

* If the I2C communication fails, an error message describing
the failure is displayed.

    ```text
    I2C target address 0x49 not acknowledged!
    ```

## Application Design Details

* This application uses one task:

`'mainThread'` - performs the following actions:

1. Opens and initializes an I2C driver object.

2. Determines which TMP sensor is available. Sensors are described in the
`sensors` data structure at the top of the source.

3. Uses the I2C driver in blocking mode to get data from the TMP sensor.

4. Extracts the temperature (in Celsius) and prints the value via the UART.

5. The task sleeps for 1 second.

### FREERTOS:

* Please view the FreeRTOSConfig.h header file for example configuration information.
