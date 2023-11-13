## Example Summary

This demo shows how to program a MSPM0 MCU loaded with CAN BSL Plugin example (bsl_can_flash_interface)
using a LP-MSPM0G3507 as host by this BSL CAN Host example.

## Peripherals & Pin Assignments
                LP-MSPM0G3507
                   Host
             -----------------
      NRST -|                 |
            |                 |
            |             PB21|<--S3
            |                 |
            |              PB0|-->NRST
            |             PB16|-->Invoke GPIO
            |                 |
            |              PA0|-->LED
            |             PB27|-->LED
            |             PA12|-->CAN_TX
            |             PA13|<--CAN_RX

## BoosterPacks, Board Resources & Jumper Settings
Visit [LP_MSPM0G3507](https://www.ti.com/tool/LP-MSPM0G3507) for LaunchPad information, including user guide and hardware files.

- Board resources:
	+ PB0 (GPIO output function to generate NRST signal)
	+ PB16 (GPIO output function to generate BSL invoke signal)
	+ S3 - PB21 (Push button to start the program)
	+ LEDs - PA0/PB27 show state of the code
     + CANTX/RX - PA12, PA13 to transfer the CAN frames


| Pin | Peripheral | Function | LaunchPad Pin | LaunchPad Settings |
| --- | --- | --- | --- | --- |
| PA5 | SYSCTL | FCC_IN | J28_4 | Connected to HFXT  Y2<br>Remove Y2 for other functions<br>Solder R15 to connect to J28_4 |
| PA5 | SYSCTL | HFXIN | J28_4 | Connected to HFXT  Y2<br>Solder R15 to connect to J28_4 |
| PA6 | SYSCTL | HFCLKIN | J28_5 | Connected to HFXT  Y2<br>Remove Y2 for other functions<br>Solder R17 to connect to J28_5 |
| PA6 | SYSCTL | HFXOUT | J28_5 | Connected to HFXT  Y2<br>Solder R17 to connect to J28_5 |
| PA13 | CANFD0 | CANRX | J4_32/J26_3 | R65 connects J26_3 for CAN (default) |
| PA12 | CANFD0 | CANTX | J4_32/J26_1 | R64 connects J26_1 for CAN (default) |
| PA20 | DEBUGSS | SWCLK | N/A | J101 15:16 ON: Connect to XDS-110 SWCLK (debug) |
| PA19 | DEBUGSS | SWDIO | N/A | J101 13:14 ON: Connect to XDS-110 SWDIO (debug) |

For more information about jumper configuration to achieve low-power using the
MSPM0 LaunchPad, please visit the [LP-MSPM0G3507 web page](https://www.ti.com/tool/LP-MSPM0G3507).


### Low-Power Recommendations
TI recommends to terminate unused pins by setting the corresponding functions to
GPIO and configure the pins to output low or input with internal
pullup/pulldown resistor.

## Example Usage
The device CAN_RX and CAN_TX are to be connected to a CAN transceiver which will route the lines to the CAN Bus.

Make the following connections, which can Invoke BSL through GPIO togggling, 
- Host NRST -> Target NRST
- Host BSL Invoke -> Target BSL Invoke.

Compile, load and run the example.
Push the S3 button to start program MSPM0G3507.

# Note
* The Transceiver used to test the example is TCAN1462-Q1.
* By default the example is initialized with CAN FD configuration
      * nominal bitrate : 1Mbps
      * data bitrate : 5Mbps
* To make connection with the BSL, the host communicates with
 the flexible data mode (fd mode), bitrate switching (brs) as disabled 
 and sends CAN frames at 1 Mbps   
* HFXT input frequency is 40 MHz
* Achieved CANCLK frequency is 40 MHz 
* Achieved MCLK frequency is 80 MHz