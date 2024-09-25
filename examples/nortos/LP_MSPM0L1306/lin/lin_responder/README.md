## Example Summary

This example configures the UART as a LIN Responder, and demonstrates basic
transmit and receive of LIN 2.0 packets using enhanced checksum.
LIN is a feature only usable with a UART Extend instance.

## Peripherals & Pin Assignments

| Peripheral | Pin | Function |
| --- | --- | --- |
| GPIOA | PA22 | Standard Output |
| GPIOA | PA26 | Standard Output |
| GPIOA | PA27 | Standard Output |
| SYSCTL |  |  |
| UART0 | PA9 | RX Pin |
| UART0 | PA8 | TX Pin |
| EVENT |  |  |
| DEBUGSS | PA20 | Debug Clock |
| DEBUGSS | PA19 | Debug Data In Out |

## BoosterPacks, Board Resources & Jumper Settings

Visit [LP_MSPM0L1306](https://www.ti.com/tool/LP-MSPM0L1306) for LaunchPad information, including user guide and hardware files.

| Pin | Peripheral | Function | LaunchPad Pin | LaunchPad Settings |
| --- | --- | --- | --- | --- |
| PA22 | GPIOA | PA22 | J1_8 | <ul><li>PA22 can be connected to photodiode D1<br><ul><li>`J6 OFF` Disconnect from photodiode D1</ul></ul> |
| PA26 | GPIOA | PA26 | J4_38 | <ul><li>PA26 can be connected to LED2 Red<br><ul><li>`J12 ON` Connect to LED2 Red<br><li>`J12 OFF` Disconnect from LED2 Red</ul></ul> |
| PA27 | GPIOA | PA27 | J4_37 | <ul><li>PA27 can be connected to LED2 Blue<br><ul><li>`J13 ON` Connect to LED2 Blue<br><li>`J13 OFF` Disconnect from LED2 Blue</ul></ul> |
| PA9 | UART0 | RX | J1_3 | <ul><li>PA9 can be used as UART RX connected to XDS-110 backchannel or to boosterpack connector:<br><ul><li>To use backchannel UART on J101:<br>  `J17 1:2`: Connects XDS-110 backchannel UART RX to `J101`<br>  `J101 7:8` Connects XDS-110 backchannel to UART RX<br><li>To use UART on boosterpack connector:<br>  `J17 2:3`: Connects UART RX to `J1_3`</ul></ul> |
| PA8 | UART0 | TX | J1_4 | <ul><li>PA8 can be used as UART TX connected to XDS-110 backchannel or to boosterpack connector:<br><ul><li>To use backchannel UART on J101:<br>  `J16 1:2`: Connects XDS-110 backchannel UART TX to `J101`<br>  `J101 9:10` Connects XDS-110 backchannel to UART TX<br><li>To use UART on boosterpack connector:<br>  `J16 2:3`: Connects UART TX to` J1_4`</ul></ul> |
| PA20 | DEBUGSS | SWCLK | J2_13 | <ul><li>PA20 is used by SWD during debugging<br><ul><li>`J101 15:16 ON` Connect to XDS-110 SWCLK while debugging<br><li>`J101 15:16 OFF` Disconnect from XDS-110 SWCLK if using pin in application</ul></ul> |
| PA19 | DEBUGSS | SWDIO | J2_17 | <ul><li>PA19 is used by SWD during debugging<br><ul><li>`J101 13:14 ON` Connect to XDS-110 SWDIO while debugging<br><li>`J101 13:14 OFF` Disconnect from XDS-110 SWDIO if using pin in application</ul></ul> |

### Device Migration Recommendations
This project was developed for a superset device included in the LP_MSPM0L1306 LaunchPad. Please
visit the [CCS User's Guide](https://software-dl.ti.com/msp430/esd/MSPM0-SDK/latest/docs/english/tools/ccs_ide_guide/doc_guide/doc_guide-srcs/ccs_ide_guide.html#sysconfig-project-migration)
for information about migrating to other MSPM0 devices.

### Low-Power Recommendations
TI recommends to terminate unused pins by setting the corresponding functions to
GPIO and configure the pins to output low or input with internal
pullup/pulldown resistor.

SysConfig allows developers to easily configure unused pins by selecting **Board**→**Configure Unused Pins**.

For more information about jumper configuration to achieve low-power using the
MSPM0 LaunchPad, please visit the [LP-MSPM0L1306 User's Guide](https://www.ti.com/lit/slau869).

## Example Usage
Connect the LIN Commander to a LIN BoosterPack with the following connections:
- Commander GND         -> BoosterPack GND
- Commander LIN_ENABLE  -> BoosterPack LIN_EN
- Commander TX          -> BoosterPack UATX (LIN TX)
- Commander RX          -> BoosterPack UARX (LIN RX)

Connect the LIN Responder to a LIN BoosterPack with the following connections:
- Responder GND         -> BoosterPack GND
- Responder LIN_ENABLE  -> BoosterPack LIN_EN
- Responder TX          -> BoosterPack UATX (LIN TX)
- Responder RX          -> BoosterPack UARX (LIN RX)

Note: the BOOSTXL-CANFD-LIN BoosterPack is compatible with the LP_MSPM0L1306 LaunchPad and it can be stacked on top.

Connect the LIN Commander BoosterPack and the LIN Responder BoosterPack using the LIN bus lines in J5.

NOTE: Alternatively, a Network Analyzer compatible with LIN 2.0 can be substituted for a LIN
Responder. To use the Network Analyzer, make the following connections between
the Network Analyzer and LIN BoosterPack:
- Network Analyzer GND    -> BoosterPack GND
- Network Analyzer LINbus -> BoosterPack LIN_TERM

The LIN responder is configured to run at 32MHz at 19200 baud. These settings
can be updated in SysConfig.

Compile, load and run the example.

Send the LIN 2.0 packets with enhanced checksum using the LIN Commander/Network Analyzer.
The list of acceptable PID commands are:
  - PID 0x08 / 0x09 / 0x0A
      - Usage: LIN Commander tells the LIN Responder to respond with 5 bytes of data stored in gResponderTXMessageData
      - Packet: [PID, DATA1, DATA2, DATA3, DATA4, DATA5, ENHANCED_CHECKSUM]
      - Example Packet: [0x8, 0x01, 0x02, 0x03, 0x04, 0x05, 0xE8]
  - PID 0x39 / 0x3A / 0x3B
      - Usage: LIN Commander sends a packet containing 8 bytes for LIN Responder to receive. The LIN Responder will then save the 8 bytes of the received data and store it in gResponderTXMessageData, overwriting what was previously saved.
      - Packet: [PID, DATA1, DATA2, DATA3, DATA4, DATA5, DATA6, DATA7, DATA8, ENHANCED_CHECKSUM]
      - Example Packet: [0x39, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0xA2]

The PIDs, message size, and functionality of the callback handlers can be modified
and customized in the "responderMessageTable" array in lin_responder.c.

When the LIN Responder receives a packet, LED1 will toggle.

When the LIN Responder transmits a packet, LED2 will toggle.
