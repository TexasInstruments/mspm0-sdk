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

## Peripherals & Pin Assignments

| Peripheral | Pin | Function |
| --- | --- | --- |
| GPIOB | PB21 | Standard Input with internal pull-up |
| GPIOB | PB26 | Standard Output |
| GPIOB | PB27 | Standard Output |
| SYSCTL |  |  |
| UART3 | PB3 | RX Pin |
| UART3 | PB2 | TX Pin |
| EVENT |  |  |
| DEBUGSS | PA20 | Debug Clock |
| DEBUGSS | PA19 | Debug Data In Out |

## BoosterPacks, Board Resources & Jumper Settings

Visit [LP_MSPM0G3507](https://www.ti.com/tool/LP-MSPM0G3507) for LaunchPad information, including user guide and hardware files.

| Pin | Peripheral | Function | LaunchPad Pin | LaunchPad Settings |
| --- | --- | --- | --- | --- |
| PB21 | GPIOB | PB21 | J27_4 | Connected to S2 button (no pull resistor) |
| PB26 | GPIOB | PB26 | J27_8 | J6 ON/OFF Connect/Disconnect LED2 Red |
| PB27 | GPIOB | PB27 | J27_10 | J7 ON/OFF Connect/Disconnect LED2 Green |
| PB3 | UART3 | RX | J1_10 | R60 pull-up not soldered by default |
| PB2 | UART3 | TX | J1_9 | R59 pull-up not soldered by default |
| PA20 | DEBUGSS | SWCLK | N/A | J101 15:16 ON: Connect to XDS-110 SWCLK (debug) |
| PA19 | DEBUGSS | SWDIO | N/A | J101 13:14 ON: Connect to XDS-110 SWDIO (debug) |

### Low-Power Recommendations
TI recommends to terminate unused pins by setting the corresponding functions to
GPIO and configure the pins to output low or input with internal
pullup/pulldown resistor.

SysConfig allows developers to easily configure unused pins by selecting **Board**→**Configure Unused Pins**.

For more information about jumper configuration to achieve low-power using the
MSPM0 LaunchPad, please visit the [LP-MSPM0G3507 User's Guide](https://www.ti.com/lit/slau846).

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

## Important Note
Early experimental samples have a known issue where the user is unable to
reconnect their debugger connection after the device has gone into STOP or
STANDBY mode. Please review advisory DEBUGSS_02 to see the steps to regain
the ability to connect the device to the debugger. This information is located
in the "MSPM0G and MSPM0L Families: Functional Differences With Early Samples"
document.
