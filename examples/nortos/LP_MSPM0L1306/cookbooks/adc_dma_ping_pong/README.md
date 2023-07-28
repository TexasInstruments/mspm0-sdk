## Example Summary
This example demonstrates how to create a DMA ping pong to transfer ADC data between 2 buffers. The ADC is using 12-bit resolution mode at 10us sampling time and a reference of VDDA. The example will take 64 samples and transfer from the ADC conversion memory register to a the first buffer via the DMA. The DMA transfer is triggered every time a new result is loaded into the ADC conversion memory and the transfer will continue until all 64 samples are loaded into the current buffer. Once all the data is transferred the ADC module will generate the DMA done interrupt. The DMA will then be configured to the other buffer as the destination address. Once the DMA destination gets changed the current data pointer is updated to the previous buffer. The pointer will be used by the CPU if there are any actions needed to be performed on the collected data, in this example there is no actions that are performed on the collected data.

Refer to the documentation of this design at www.ti.com/lit/slaae84

## Peripherals & Pin Assignments

| Peripheral | Pin | Function |
| --- | --- | --- |
| SYSCTL |  |  |
| ADC0 | PA27 | ADC12 Channel 0 Pin |
| VREF |  |  |
| EVENT |  |  |
| DMA |  |  |
| DEBUGSS | PA20 | Debug Clock |
| DEBUGSS | PA19 | Debug Data In Out |

## BoosterPacks, Board Resources & Jumper Settings

Visit [LP_MSPM0L1306](https://www.ti.com/tool/LP-MSPM0L1306) for LaunchPad information, including user guide and hardware files.

| Pin | Peripheral | Function | LaunchPad Pin | LaunchPad Settings |
| --- | --- | --- | --- | --- |
| PA27 | ADC0 | A0 | J4_37 | J13 OFF: Disconnect LED2 Blue |
| PA20 | DEBUGSS | SWCLK | J2_13 | J101 15:16 ON: Connect to XDS-110 SWCLK (debug) |
| PA19 | DEBUGSS | SWDIO | J2_17 | J101 13:14 ON: Connect to XDS-110 SWDIO (debug) |

### Low-Power Recommendations
TI recommends to terminate unused pins by setting the corresponding functions to
GPIO and configure the pins to output low or input with internal
pullup/pulldown resistor.

SysConfig allows developers to easily configure unused pins by selecting **Board**→**Configure Unused Pins**.

For more information about jumper configuration to achieve low-power using the
MSPM0 LaunchPad, please visit the [LP-MSPM0L1306 User's Guide](https://www.ti.com/lit/slau869).

## Example Usage
Compile, load and run the example. Software breakpoints will allow the user to inspect memory during the lifecycle of  the progam.
