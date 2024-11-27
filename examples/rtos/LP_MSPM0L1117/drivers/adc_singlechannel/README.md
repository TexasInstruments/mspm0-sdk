## Example Summary

Example that uses the ADC driver to make a number of samples
and print them via UART.

## Peripherals & Pin Assignments

| Peripheral | Pin | Function |
| --- | --- | --- |
| SYSCTL |  |  |
| UART0 | PA11 | RX Pin |
| UART0 | PA10 | TX Pin |
| DEBUGSS | PA20 | Debug Clock |
| DEBUGSS | PA19 | Debug Data In Out |
| GPIOA | PA0 | Open-Drain Output |
| ADC0 | PA27 | ADC0 Channel 0 Pin |

## BoosterPacks, Board Resources & Jumper Settings

Visit [LP_MSPM0L1117](https://www.ti.com/tool/LP-MSPM0L1117) for LaunchPad information, including user guide and hardware files.

| Pin | Peripheral | Function | LaunchPad Pin | LaunchPad Settings |
| --- | --- | --- | --- | --- |
| PA11 | UART0 | RX | J4_33 | J12 1:2 Connect XDS-110 BackChannel UART on J22<br>J12 2:3 Connect to J4_33<br>J22 7:8 Connect XDS-110 BackChannel UART RX |
| PA10 | UART0 | TX | J4_34 | J10 1:2 Connect XDS-110 BackChannel UART on J22<br>J10 2:3 Connect to J4_34<br>J22 9:10 Connect XDS-110 BackChannel UART TX |
| PA0 | GPIOA | PA0 | J1_10 | This pin is 5V tolerant open-drain and requires pull-up.<br>J1 ON/OFF: Connect/Disconnect LED1<br>J9 1:2 3.3V Pull-up<br>J9 2:3 5V Pull-up |
| PA27 | ADC0 | Channel 0 | J1_8 | N/A |
| PA20 | DEBUGSS | SWCLK | N/A | J22 15:16 ON: Connect to XDS-110 SWCLK (debug) |
| PA19 | DEBUGSS | SWDIO | N/A | J22 13:14 ON: Connect to XDS-110 SWDIO (debug) |

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

* Example output is generated through use of Display driver APIs.

* Connect the ADC channels to the sampling sources.
    * For quick testing, connect ADC channel pin to `GND` and run the test, then connect to `3V3` and run the test.

>__Important:__ Caution should be used when connecting the pins to analog inputs greater than 3VDC.

* Open a serial session (e.g. [`PuTTY`](http://www.putty.org/ "PuTTY's
Homepage"), etc.) to the appropriate COM port.
* The COM port can be determined via Device Manager in Windows or via
`ls /dev/tty*` in Linux.

* The connection will have the following settings:

```text
    Baud-rate:     115200
    Data bits:          8
    Stop bits:          1
    Parity:          None
    Flow Control:    None
```

* Run the example.

* Example snippet of outputs:

```text
        Starting the adcsinglechannel example

        CONFIG_ADC_0 raw result (0): 4095

        CONFIG_ADC_0 convert result (0): 3300000 uV

        CONFIG_ADC_0 raw result (1): 4095

        CONFIG_ADC_0 convert result (1): 3300000 uV
                    .                 .     .
                    .                 .     .
                    .                 .     .
        CONFIG_ADC_0 raw result (9): 4095

        CONFIG_ADC_0 convert result (9): 3300000 uV
```

* The example outputs ten samples from `CONFIG_ADC_0`.

* The actual conversion result values may vary depending on the reference
voltage settings. Please refer to the board specific datasheet for more details.

## Application Design Details

This application uses a single thread:

`threadFxn0` - performs the following actions:

1. Opens an ADC driver object.

2. Uses the ADC driver object to perform 10 samples and output the results.

3. Closes the ADC driver object.

FreeRTOS:

* Please view the `FreeRTOSConfig.h` header file for example configuration
information.
