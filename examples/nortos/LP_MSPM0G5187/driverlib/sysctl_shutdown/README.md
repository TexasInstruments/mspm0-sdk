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

## Peripherals, Pin Functions, MCU Pins, Launchpad Pins
| Peripheral | Function | MCU Pin | Launchpad Pin | Launchpad Settings |
| --- | --- | --- | --- | --- |
| GPIOA | Standard with Wake Input | PA18 | J101_18 |  |
| GPIOA | Standard with Wake Output | PA17 | LED2 Red | Populate Jumper(s): J12[2:1] |
| GPIOA | Standard Output | PA24 | LED2 Green | Populate Jumper(s): J13[2:1] |
| GPIOB | Standard Input with internal pull-up | PB6 | J16_2 |  |
| SYSCTL |  |  |  |  |
| EVENT |  |  |  |  |
| BOARD | Debug Clock | PA20 | J101_16 |  |
| BOARD | Debug Data In Out | PA19 | J101_14 |  |

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
MSPM0 LaunchPad, please visit the [LP-MSPM0G5187 User's Guide](https://www.ti.com/lit/slau967).

## Example Usage

Compile, load and run the example. Rising edge on S1 will wake device from
SHUTDOWN state and LED2 blinks equal to the number of times the device has been
woken up.
