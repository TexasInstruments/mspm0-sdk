## Example Summary

The following example configures the UART instance in internal loopback mode
to transmit and receive data.
Pressing a LaunchPad button will toggle the device between RUN mode and STANDBY
mode. The green LED is used to indicate the current power mode.
The device will send UART packets continuously while in RUN mode while toggling
the red LED.

Some MSPM0G peripherals residing in PD1 domain don't retain register contents when
entering STOP or STANDBY modes.
UART3 is a UART instance in PD1 which exhibits this behavior.
Developers can decide to re-initialize the peripheral using the default
initialization (i.e. SYSCFG_DL_UART_init) from SysConfig. This approach is more
memory-efficient.
Alternatively, this example demonstrates using DriverLib APIs to save and
restore the register configuration of the peripheral before and after entering
low-power mode. This approach is recommended if the UART configuration is
modified at runtime.

## Peripherals, Pin Functions, MCU Pins, Launchpad Pins
| Peripheral | Function | Pin | Launchpad Pin |
| --- | --- | --- | --- |
| GPIOB | Standard Input with internal pull-up | PB3 | S2 |
| GPIOB | Standard Output | PB26 | LED2 Red |
| GPIOB | Standard Output | PB27 | LED2 Green |
| UART3 | RX Pin | PA25 | J1_6 |
| UART3 | TX Pin | PA14 | J1_2 |
| BOARD | Debug Clock | PA20 | J19_16 |
| BOARD | Debug Data In Out | PA19 | J19_14 |

### Device Migration Recommendations
This project was developed for a superset device included in the MSPM0 LaunchPad. Please
visit the [CCS User's Guide](https://software-dl.ti.com/msp430/esd/MSPM0-SDK/latest/docs/english/tools/ccs_ide_guide/doc_guide/doc_guide-srcs/ccs_ide_guide.html#sysconfig-project-migration)
for information about migrating to other MSPM0 devices.

### Low-Power Recommendations
TI recommends to terminate unused pins by setting the corresponding functions to
GPIO and configure the pins to output low or input with internal
pullup/pulldown resistor.

SysConfig allows developers to easily configure unused pins by selecting **Board**→**Configure Unused Pins**.

For more information about jumper configuration to achieve low-power using the
MSPM0 LaunchPad, please visit the LP-MSPM0G3519 User's Guide.

## Example Usage
Compile, load and run the example.
Device will start in RUN mode.

If the device is in RUN:
- Green LED is ON
- Data will be transmitted/received continuously
- Red LED will toggle on each transfer

If the button is pressed while in RUN:
- Device stops transmitting data
- Saves the UART SW configuration
- Enters STANDBY mode

If the device is in STANDBY:
- Green and LED will be OFF
- No transmission occurs

If the button is pressed while in STANDBY:
- Device wakes-up
- Restores the UART SW configuration
- Goes to RUN mode
