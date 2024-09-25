## Example Summary
This example demonstrates how to create a DMA ping pong to transfer ADC data between 2 buffers. 
The ADC is using 12-bit resolution mode at 10us sampling time and using the Internal Vref at 2.5V as the ADC reference. 
The example will take 64 samples and transfer from the ADC FIFO to the first buffer via the DMA. 
The DMA starts transfer once the ADC fills all the memory registers and the DMA will transfer 6 32-bit data packets per trigger.
Once all the data is transferred the ADC module will generate the DMA done interrupt. The DMA will then be configured to the other buffer as the destination address. 

Refer to the documentation of this design at www.ti.com/lit/slaae84

## Peripherals & Pin Assignments

| Peripheral | Pin | Function |
| --- | --- | --- |
| SYSCTL |  |  |
| ADC0 | PA25 | ADC12 Channel 2 Pin |
| VREF | PA23 | VREF Positive (VREF+) |
| EVENT |  |  |
| DMA |  |  |
| DEBUGSS | PA20 | Debug Clock |
| DEBUGSS | PA19 | Debug Data In Out |

## BoosterPacks, Board Resources & Jumper Settings

Visit [LP_MSPM0G3507](https://www.ti.com/tool/LP-MSPM0G3507) for LaunchPad information, including user guide and hardware files.

| Pin | Peripheral | Function | LaunchPad Pin | LaunchPad Settings |
| --- | --- | --- | --- | --- |
| PA25 | ADC0 | A2 | J1_2 | N/A |
| PA23 | VREF | VREF+ | N/A | <ul><li>PA23 can be connected to GND using C7 or to VREF- using C8<br><ul><li>`C8` is soldered by default connecting pin to VREF-<br><li>`C7` is not soldered by default and can be used for a custom capacitor to GND</ul><br><li>PA23 can be connected to boosterpack connector<br><ul><li>`R22` is not soldered by default<br><li>Solder `R22` to connect pin to J28_10</ul></ul> |
| PA20 | DEBUGSS | SWCLK | N/A | <ul><li>PA20 is used by SWD during debugging<br><ul><li>`J101 15:16 ON` Connect to XDS-110 SWCLK while debugging<br><li>`J101 15:16 OFF` Disconnect from XDS-110 SWCLK if using pin in application</ul></ul> |
| PA19 | DEBUGSS | SWDIO | N/A | <ul><li>PA19 is used by SWD during debugging<br><ul><li>`J101 13:14 ON` Connect to XDS-110 SWDIO while debugging<br><li>`J101 13:14 OFF` Disconnect from XDS-110 SWDIO if using pin in application</ul></ul> |

### Device Migration Recommendations
This project was developed for a superset device included in the LP_MSPM0G3507 LaunchPad. Please
visit the [CCS User's Guide](https://software-dl.ti.com/msp430/esd/MSPM0-SDK/latest/docs/english/tools/ccs_ide_guide/doc_guide/doc_guide-srcs/ccs_ide_guide.html#sysconfig-project-migration)
for information about migrating to other MSPM0 devices.

### Low-Power Recommendations
TI recommends to terminate unused pins by setting the corresponding functions to
GPIO and configure the pins to output low or input with internal
pullup/pulldown resistor.

SysConfig allows developers to easily configure unused pins by selecting **Board**→**Configure Unused Pins**.

For more information about jumper configuration to achieve low-power using the
MSPM0 LaunchPad, please visit the [LP-MSPM0G3507 User's Guide](https://www.ti.com/lit/slau873).

## Example Usage
Compile, load and run the example. Input a signal within 0 - 2.5V into the ADC input. 
Inspect the buffers at the software breakpoint, the result will alternate between buffers with each continue of the program.  
