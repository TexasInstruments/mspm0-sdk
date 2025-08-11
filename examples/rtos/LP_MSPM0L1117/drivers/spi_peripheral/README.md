## Example Summary

Demonstrates how to use SPI driver in peripheral mode to communicate with another
device. To run this example successfully, another LP_MSPM0L1117
device running the `spi_controller` example is required.

## Peripherals & Pin Assignments

| Peripheral | Pin | Function |
| --- | --- | --- |
| SYSCTL |  |  |
| UART0 | PA11 | RX Pin |
| UART0 | PA10 | TX Pin |
| DEBUGSS | PA20 | Debug Clock |
| DEBUGSS | PA19 | Debug Data In Out |
| GPIOA | PA0 | Open-Drain Output |
| SPI0 | PB3 | SPI SCLK (Clock) |
| SPI0 | PB2 | SPI PICO (Peripheral In, Controller Out) |
| SPI0 | PA13 | SPI POCI (Peripheral Out, Controller In) |
| SPI0 | PB6 | SPI CS1 (Chip Select 1) |
| GPIOA | PA7 | Peripheral Ready Standard Output |
| GPIOB | PB24 | Controller Ready Standard Input |

## BoosterPacks, Board Resources & Jumper Settings

This example requires two LaunchPads.

* Connect the SPI pins, GND pin, Controller Ready Pin and Peripheral Ready pin between two launchpads using the jumper cables.

| Pin | Peripheral | Function | LaunchPad Pin | LaunchPad Settings |
| --- | --- | --- | --- | --- |
| PA11 | UART0 | RX | J4_33 | J12 1:2 Connect XDS-110 BackChannel UART on J22<br>J12 2:3 Connect to J4_33<br>J22 7:8 Connect XDS-110 BackChannel UART RX |
| PA10 | UART0 | TX | J4_34 | J10 1:2 Connect XDS-110 BackChannel UART on J22<br>J12 2:3 Connect to J4_34<br>J22 9:10 Connect XDS-110 BackChannel UART TX |
| PA20 | DEBUGSS | SWCLK | N/A | J22 15:16 ON: Connect to XDS-110 SWCLK (debug) |
| PA19 | DEBUGSS | SWDIO | N/A | J22 13:14 ON: Connect to XDS-110 SWDIO (debug) |
| PA0 | GPIOA | PA0 | J1_10 | This pin is 5V tolerant open-drain and requires pull-up.<br>J1 ON/OFF: Connect/Disconnect LED1<br>J9 1:2 3.3V Pull-up<br>J9 2:3 5V Pull-up |
| PB3 | SPI0 | SCLK | J1_7 | N/A |
| PB2 | SPI0 | PICO | J2_15 | N/A |
| PA13 | SPI0 | POCI | J2_14 | N/A |
| PB6 | SPI0 | CS1 | J2_18 | N/A |
| PA7 | GPIOA | PA7 | J2_19 | N/A |
| PB24 | GPIOB | PB24 | J2_17 | N/A |

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
    Starting the SPI peripheral example
    Peripheral SPI initialized

    Peripheral received: Hello from controller, msg#: 0
    Peripheral received: Hello from controller, msg#: 1
    Peripheral received: Hello from controller, msg#: 2
    Peripheral received: Hello from controller, msg#: 3
    Peripheral received: Hello from controller, msg#: 4
    Peripheral received: Hello from controller, msg#: 5
    Peripheral received: Hello from controller, msg#: 6
    Peripheral received: Hello from controller, msg#: 7
    Peripheral received: Hello from controller, msg#: 8
    Peripheral received: Hello from controller, msg#: 9

    Done

    ```

## Application Design Details

This application uses a single thread:

`peripheralThread` - waits for controller to open `CONFIG_SPI_CONTROLLER`, opens
`CONFIG_SPI_PERIPHERAL`, creates the peripheral message & prepares a transfer.  The
`CONFIG_SPI_PERIPHERAL_READY` pin is pulled low to notify the controller it is ready.
Once the transfer complete, peripheral pulls `CONFIG_SPI_PERIPHERAL_READY` high, prepares
the next transaction & pulls `CONFIG_SPI_PERIPHERAL_READY` low again.  A total of
`MAX_LOOP` SPI transactions are performed in this manner.

The `peripheralThread` performs the following actions:

1. Before performing transfers, we must make sure both, `spicontroller` &
`spiperipheral` applications are synchronized with each other.  The peripheral will set
`CONFIG_SPI_PERIPHERAL_READY` to 1 to notify the controller it is ready to synchronize.
The peripheral will then wait for the controller to pull `CONFIG_SPI_CONTROLLER_READY` high in
acknowledgment.

2. Waits until controller has opened `CONFIG_SPI_CONTROLLER`.  Controller will pull
`CONFIG_SPI_CONTROLLER_READY` low once it has opened its SPI peripheral.

3. Opens `CONFIG_SPI_PERIPHERAL` in callback mode.

4. Creates a SPI transaction structure and sets txBuffer to `Hello from
peripheral, msg# n`, where n is the iteration number.

5. Toggles `CONFIG_GPIO_LED_0`.

6. Performs the SPI transfer in callback mode.  If the transfer is successful,
`CONFIG_SPI_PERIPHERAL_READY` is pulled low to notify the controller the peripheral is ready
for a transfer & the peripheral waits until the transfer has completed.  Otherwise, an
error message is printed to the UART.

7. Once the transfer is completed, `CONFIG_SPI_PERIPHERAL_READY` is pulled high to
notify controller peripheral is not ready for another transfer.

8. Repeats from step 4 for `MAX_LOOP` iterations.

9. Closes the SPI driver object and terminates execution.

FreeRTOS:

* Please view the `FreeRTOSConfig.h` header file for example configuration
information.
