## Example Summary

This demo shows how to program a MSPM0 MCU using a LP-MSPM0G3507 as host by BSL CAN interface.

## Peripherals & Pin Assignments

| Peripheral | Pin | Function |
| --- | --- | --- |
| SYSCTL | PA5 | HFXIN |
| SYSCTL | PA6 | HFXOUT |
| CANFD0 | PA13 | RX Pin |
| CANFD0 | PA12 | TX Pin |
| DEBUGSS | PA20 | Debug Clock |
| DEBUGSS | PA19 | Debug Data In Out |
| BSL | PA18 | BSL Invoke Pin |
| BSL | NRST | NRST |

## BoosterPacks, Board Resources & Jumper Settings
Visit [LP_MSPM0G3507](https://www.ti.com/tool/LP-MSPM0G3507) for LaunchPad information, including user guide and hardware files.

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
- Controller NRST -> Peripheral NRST
- Controller BSL Invoke -> Peripheral BSL Invoke.

Compile, load and run the example.
Set up Initial CAN configurations as
    * CAN Mode : Classical CAN (BRS disabled)
    * Nominal Speed : 1 Mbps
    * Sampling point : 87.5
Push the S2 button to start program MSPM0G3507.

# Note
* * The Example is configured to work in CAN mode initially at 1 Mbps to get response from CAN plugin.
* To change the bitrate of communication based on the configuration obtained from host through Change Baudrate Command
    * The Data section in Change Baudrate Command is expected to match the format shown below

|     Padding      (5)    |     DRP      (5)    |     DSJW     (4)    |     DTSEG2      (4)    |     DTSEG1      (5)    |     NRP      (9)    |     NSJW      (7)    |     NSEG2      (7)    |     NTSEG1      (8)    |     BRS      (1)    |     FD      (1)    |
|------------------|-------------------------|---------------------|---------------------|------------------------|------------------------|---------------------|----------------------|-----------------------|------------------------|---------------------|

* An arbitrary CAN frame is injected into CAN bus, on changing the CAN Mode to CAN FD to calibrate the transmission delay compensation attribute values. The Identity value can be modified as required.
* Message Identifier accepted by BSL CAN Host is 0x004
* Message Identifier sent from BSL CAN HOST to BSL CAN PLUGIN is 0x003