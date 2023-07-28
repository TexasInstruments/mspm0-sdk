## Example Summary

The following examples configures the TRNG, then goes into STOP mode. The device
stays in STOP mode for 1 second and then is woken up by the Timer. The device
restores the configuration of the TRNG, and then the TRNG fills a buffer with
random data.

Some MSPM0G peripherals residing in PD1 domain don't retain register contents when
entering STOP or STANDBY modes, refer to the datasheet for more details.
TRNG is a peripheral in PD1 which exhibits this behavior.
Developers can decide to re-initialize the peripheral using the default
initialization (i.e. SYSCFG_DL_TRNG_init) from SysConfig. This approach
is more memory-efficient.
Alternatively, this example demonstrates using DriverLib APIs to save and
restore the register configuration of the peripheral before and after entering
low-power mode. This approach is recommended if the TRNG configuration is
modified at runtime.

## Peripherals & Pin Assignments

| Peripheral | Pin | Function |
| --- | --- | --- |
| GPIOA | PA0 | Open-Drain Output |
| SYSCTL |  |  |
| TIMG0 |  |  |
| EVENT |  |  |
| TRNG |  |  |
| DEBUGSS | PA20 | Debug Clock |
| DEBUGSS | PA19 | Debug Data In Out |

## BoosterPacks, Board Resources & Jumper Settings

Visit [LP_MSPM0G3507](https://www.ti.com/tool/LP-MSPM0G3507) for LaunchPad information, including user guide and hardware files.

| Pin | Peripheral | Function | LaunchPad Pin | LaunchPad Settings |
| --- | --- | --- | --- | --- |
| PA0 | GPIOA | PA0 | J27_9 | This pin is 5V tolerant open-drain and requires pull-up.<br>J4 ON/OFF: Connect/Disconnect LED1<br>J19 1:2 3.3V Pull-up<br>J19 2:3 5V Pull-up |
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
After configuring the peripherals, the application will go into STOP mode.
In 1 second, the timer interrupt will fire and wake up the device, and restore
the SW configuration of the TRNG. The application will turn on an LED, and then
the TRNG will fill gTRNGBuffer with random data.
The application will hit a SW breakpoint to allow the user to inspect the
contents of gTRNGBuffer.

## Important Note
Early experimental samples have a known issue where the user is unable to
reconnect their debugger connection after the device has gone into STOP or
STANDBY mode. Please review advisory DEBUGSS_02 to see the steps to regain
the ability to connect the device to the debugger. This information is located
in the "MSPM0G and MSPM0L Families: Functional Differences With Early Samples"
document.
