<span style="color:red">This example has not been fully validated at the time of this release. Functionality is not guaranteed. </span>

## Example Summary

One OPA is configured in as a Transimpedance Amplifier (TIA) with COMP0_DAC8
driving the bias voltage.
A second stage is configured as PGA with COMP1_DAC8 driving bias voltage.
The output of the second stage is connected to ADC.

## Peripherals & Pin Assignments

| Peripheral | Pin | Function |
| --- | --- | --- |
| SYSCTL |  |  |
| VREF | PA23 | VREF Positive (VREF+) |
| VREF | PA21 | VREF Negative (VREF-) |
| OPA0 | PA27 | OPA Inverting input 0 Pin |
| OPA0 | PA22 | OPA Output Pin |
| OPA1 | PA16 | OPA Output Pin |
| ADC1 |  |  |
| COMP0 |  |  |
| EVENT |  |  |
| DEBUGSS | PA20 | Debug Clock |
| DEBUGSS | PA19 | Debug Data In Out |

## BoosterPacks, Board Resources & Jumper Settings

Visit [LP_MSPM0G3507](https://www.ti.com/tool/LP-MSPM0G3507) for LaunchPad information, including user guide and hardware files.

| Pin | Peripheral | Function | LaunchPad Pin | LaunchPad Settings |
| --- | --- | --- | --- | --- |
| PA23 | VREF | VREF+ | N/A | C8 to GND soldered by default<br>C7 to GND can be soldered for custom VREF capacitor<br>Solder R22 to connect to J28_10 |
| PA21 | VREF | VREF- | J28_8 | R3 to GND soldered by default<br>Solder R10 to connect to J28_8 |
| PA27 | OPA0 | IN0- | J1_8 | J17 ON/OFF Connect/Disconnect from Light Sensor D1 |
| PA22 | OPA0 | OUT | J3_24 | J16 ON/OFF: Connect/Disconnect from Light sensor D1<br>RC Filter R83/C52 not soldered by default |
| PA16 | OPA1 | OUT | J3_29 | J15 1:2 Connect to J3_29 |
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

Connect OPA0_IN0- to external RC circuitry and input current to be measured.
Compile, load and run the example.  For an easier setup, keep jumpers J16 and
J17 connected to the device to utilize the light sensor circuit, which can act
as the required current source and RC circuitry for this example.  In addition,
avoid using 1x gain for the second stage programmable gain amplifier, and if no gain
is desired, configure the second stage amplifier into buffer mode.

Use debugger to check ADC result based on input current.

On powerup, the OPA and ADC pin(s) used in this example are by default set to
the correct analog mode. Therefore, calls to
DL_GPIO_initPeripheralAnalogFunction for pinmuxing those pins are not needed.
