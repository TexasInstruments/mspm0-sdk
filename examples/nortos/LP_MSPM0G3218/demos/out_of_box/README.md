## Example Summary

This demo presents the basic peripherals of MSPM0G3218 (pressing S2 cycles to the next demo)
 (1) Blink_LED: A blinking LED that toggles every second based on a timer
 (2) RGB Cycling: each of the three colors are cycled through, changing every second to the
     next color in the RGB pattern.
 (3) Thermistor ADC Read: The ADC takes a sample of the value of the thermistor circuit. If
     the value falls below 1500, the RGB LED turns Blue, green from 1500-2500, and red for >2500

## Peripherals, Pin Functions, MCU Pins, Launchpad Pins
| Peripheral | Function | MCU Pin | Launchpad Pin | Launchpad Settings |
| --- | --- | --- | --- | --- |
| GPIOA | Open-Drain Output | PA0 | Red LED1 | Populate Jumper(s): J4[1:2] |
| GPIOB | Standard Input with internal pull-up | PB21 | S2 |  |
| GPIOB | Standard Output | PB26 | LED2 Red | Populate Jumper(s): J6[1:2] |
| GPIOB | Standard Output | PB27 | LED2 Green | Populate Jumper(s): J7[1:2] |
| GPIOB | Standard Output | PB22 | LED2 Blue | Populate Jumper(s): J5[1:2] |
| SYSCTL |  |  |  |  |
| TIMA0 |  |  |  |  |
| ADC0 | ADC12 Channel 7 Pin | PA22 | Thermistor |  |
| EVENT |  |  |  |  |

### Device Migration Recommendations
This project was developed for a superset device included in the LP_MSPM0G3218 LaunchPad. Please
visit the [CCS User's Guide](https://software-dl.ti.com/msp430/esd/MSPM0-SDK/latest/docs/english/tools/ccs_ide_guide/doc_guide/doc_guide-srcs/ccs_ide_guide.html#sysconfig-project-migration)
for information about migrating to other MSPM0 devices.

### Low-Power Recommendations
TI recommends to terminate unused pins by setting the corresponding functions to
GPIO and configure the pins to output low or input with internal
pullup/pulldown resistor.

SysConfig allows developers to easily configure unused pins by selecting **Board**→**Configure Unused Pins**.

For more information about jumper configuration to achieve low-power using the
MSPM0 LaunchPad, please visit the [LP-MSPM0G3328 User's Guide](https://www.ti.com/lit/SLVUDA6).

## Example Usage
Compile, load and run the example.
Observe the red LED blink every second
Press user button 2 (S2) once
Observe the RGB LED cycle through each color every 1 second
Press user button 2 (S2) again
Provide a valid voltage to A0_7 between 0 - VCC or connect J2 and J3 for thermistor reading.
Monitor the RGB LED while input voltage changes.

On powerup, the ADC pin(s) used in this example are by default set to
the correct analog mode. Therefore, calls to
DL_GPIO_initPeripheralAnalogFunction for pinmuxing those pins are not needed.
