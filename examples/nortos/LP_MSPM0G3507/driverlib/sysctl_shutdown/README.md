<span style="color:red">This example has not been fully validated at the time of this release. Functionality is not guaranteed. </span>

## Example Summary

Configures different power policies based on the configuration of two input
pins. This prevents the device from indefinitely staying in SHUTDOWN
mode since the serial wire debug (SWD) remains locked while the device is in
SHUTDOWN mode.

After entering SHUTDOWN mode, the device will exit when a high level is detected
on the wake-up pin. Then it will toggle LED2 for as many times as it has exited
SHUTDOWN mode before (i.e. LED2 will toggle once the first time it wakes up, two
times the second time it wakes-up and so on).

If a different power policy is selected via the configuration input pins. The
device will only remain in the selected power policy but will not wake-up via
wake-up pin

The table below shows the power policy to be configured based on configuration
input configuration.

|config input 0      |config input 1      |Power Policy        |
|--------------------|--------------------|--------------------|
| High (default)     | High (default)     | SHUTDOWN           |
| High (default)     | Low (pin grounded) | STOP0              |
| Low (pin grounded) | High (default)     | STANDBY0           |
| Low (pin grounded) | Low (pin grounded) | RUN0SLEEP0         |

## Peripherals & Pin Assignments

| Peripheral | Pin | Function |
| --- | --- | --- |
| GPIOA | PA18 | Standard with Wake Input |
| GPIOB | PB22 | Standard Output |
| GPIOB | PB26 | Standard Output |
| GPIOB | PB17 | Standard Input with internal pull-down |
| GPIOB | PB12 | Standard Input with internal pull-down |
| SYSCTL |  |  |
| EVENT |  |  |
| DEBUGSS | PA20 | Debug Clock |
| DEBUGSS | PA19 | Debug Data In Out |

## BoosterPacks, Board Resources & Jumper Settings

Visit [LP_MSPM0G3507](https://www.ti.com/tool/LP-MSPM0G3507) for LaunchPad information, including user guide and hardware files.

| Pin | Peripheral | Function | LaunchPad Pin | LaunchPad Settings |
| --- | --- | --- | --- | --- |
| PA18 | GPIOA | PA18 | J3_26/J3_29 | J15 2:3 Connect to J3_29<br>R58 connects to J3_26 (default)<br>R23 connects to J101 and J8 for BSL_Invoke (default)<br>J8 ON/OFF Connect/Disconnect S1 for BSL_Invoke (with pull-down)<br>J101 17:18 Connect/Disconnect from XDS-110 BSL_Invoke<br>R66/R67 for thermistor RT1 not soldered by default |
| PB22 | GPIOB | PB22 | J27_5 | J5 ON/OFF Connect/Disconnect LED2 Blue |
| PB26 | GPIOB | PB26 | J27_8 | J6 ON/OFF Connect/Disconnect LED2 Red |
| PB17 | GPIOB | PB17 | J2_18 | N/A |
| PB12 | GPIOB | PB12 | J2_19 | N/A |
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

Compile, load and run the example. Rising edge on S1 will wake device from
SHUTDOWN state and LED2 blinks equal to the number of times the device has been
woken up.
