## Example Summary

Demonstrates how to use SPI driver in controller mode to communicate with another
device. To run this example successfully, another LP_MSPM0L2228
device running the `spi_peripheral` example is required.

## Peripherals & Pin Assignments

| Peripheral | Pin | Function |
| --- | --- | --- |
| SYSCTL |  |  |
| UART0 | PA11 | RX Pin |
| UART0 | PA10 | TX Pin |
| DEBUGSS | PA20 | Debug Clock |
| DEBUGSS | PA19 | Debug Data In Out |
| GPIOA | PA0 | Open-Drain Output |
| SPI1 | PA17 | SPI SCLK (Clock) |
| SPI1 | PB8 | SPI PICO (Peripheral In, Controller Out) |
| SPI1 | PB7 | SPI POCI (Peripheral Out, Controller In) |
| SPI1 | PB6 | SPI CS0 (Chip Select 0) |
| GPIOA | PA25 | Controller Ready Standard Input |
| GPIOA | PA26 | Peripheral Ready Standard Output |

## BoosterPacks, Board Resources & Jumper Settings

This example requires two LaunchPads.

* Connect the SPI pins, GND pin, Controller Ready Pin and Peripheral Ready pin between two launchpads using the jumper cables.

| Pin | Peripheral | Function | LaunchPad Pin | LaunchPad Settings |
| --- | --- | --- | --- | --- |
| PA11 | UART0 | RX | J3_29 | J14 7:8 Connect XDS-110 BackChannel UART RX<br>J3:29 UART RX |
| PA10 | UART0 | TX | J3_30 | J14 9:10 Connect XDS-110 BackChannel UART TX<br>J3:30 UART TX |
| PA20 | DEBUGSS | SWCLK | N/A | J101 15:16 ON: Connect to XDS-110 SWCLK (debug) |
| PA19 | DEBUGSS | SWDIO | N/A | J101 13:14 ON: Connect to XDS-110 SWDIO (debug) |
| PA0 | GPIOA | PA0 | J27_9 | This pin is 5V tolerant open-drain and requires pull-up.<br>J2 ON/OFF: Connect/Disconnect LED1<br>J12 1:2 3.3V Pull-up<br>J12 2:3 5V Pull-up |
| PA17 | SPI1 | SCLK | J1_7 | N/A |
| PB8 | SPI1 | PICO | J2_15 | N/A |
| PB7 | SPI1 | POCI | J2_14 | N/A |
| PB6 | SPI1 | CS0 | J2_13 | N/A |
| PA25 | GPIOA | PA25 | J4_31 | N/A |
| PA26 | GPIOA | PA26 | J4_32 | N/A |

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

* Run the example. `CONFIG_GPIO_LED_0` turns ON to show the example is running.

* If the SPI communication is out of sync, ensure that the controller is running before the peripheral. This is because the peripheral needs to be ready to respond to the controller for proper communication to occur.

Once the peripheral is running, controller and peripheral devices will
exchange messages in a loop.  While the SPI transactions are taking place,
`CONFIG_GPIO_LED_0` will toggle on/off indicating transfers are occurring. While
the transfer is going on, the messages are printed via UART. The loop is repeated
`MAX_LOOP` times.

Messages should appear as follows:

```text
    Starting the SPI controller example
    Controller SPI initialized

    Controller received: Hello from peripheral, msg#: 0
    Controller received: Hello from peripheral, msg#: 1
    Controller received: Hello from peripheral, msg#: 2
    Controller received: Hello from peripheral, msg#: 3
    Controller received: Hello from peripheral, msg#: 4
    Controller received: Hello from peripheral, msg#: 5
    Controller received: Hello from peripheral, msg#: 6
    Controller received: Hello from peripheral, msg#: 7
    Controller received: Hello from peripheral, msg#: 8
    Controller received: Hello from peripheral, msg#: 9

    Done

    ```

## Application Design Details

This application uses a single thread:

`controllerThread` - creates the controller SPI message, opens `CONFIG_SPI_CONTROLLER` and
waits for the peripheral to be ready for a SPI transfer.  When the peripheral is ready it
starts the transfer.  Once complete, the controller prepares the next transaction
& waits for the peripheral once more.  A total of `MAX_LOOP` SPI transactions are
performed in this manner.

The `controllerThread` performs the following actions:

1. Before performing transfers, we must make sure both, `spicontroller` &
`spiperipheral` applications are synchronized with each other.  The controller will set
`CONFIG_SPI_CONTROLLER_READY` to 1 to notify the peripheral it is ready to synchronize.
The controller will then wait for the peripheral to pull `CONFIG_SPI_PERIPHERAL_READY` high in
acknowledgment.

2. Opens `CONFIG_SPI_CONTROLLER` & sets `CONFIG_SPI_CONTROLLER_READY` to 0.  Setting
`CONFIG_SPI_CONTROLLER_READY` will notify the peripheral `CONFIG_SPI_CONTROLLER` has been
opened.

3. Waits for the peripheral to be ready for the SPI transfer. The peripheral will pull
`CONFIG_SPI_PERIPHERAL_READY` low when ready for a transfer.

4. Creates a SPI transaction structure and sets txBuffer to `Hello from
controller, msg# n`, where n is the iteration number.

5. Toggles `CONFIG_GPIO_LED_0`.

6. Transfers the message. If the transfer is successful, the message
received from the peripheral SPI is printed. Otherwise, an error message
is printed.

7. Sleeps for 2 seconds.

8. Repeats from step 3 for `MAX_LOOP` iterations.

9. Closes the SPI driver object and terminates execution.

FreeRTOS:

* Please view the `FreeRTOSConfig.h` header file for example configuration
information.
