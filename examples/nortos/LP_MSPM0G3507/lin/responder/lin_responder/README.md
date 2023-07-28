## Example Summary

This example configures the UART as a LIN Responder, and demonstrates basic
transmit and receive of LIN 2.0 packets using enhanced checksum.
LIN is a feature only usable with a UART Extend instance.

## Peripherals & Pin Assignments

| Peripheral | Pin | Function |
| --- | --- | --- |
| GPIOB | PB15 | Standard Output |
| GPIOB | PB22 | Standard Output |
| GPIOB | PB26 | Standard Output |
| SYSCTL |  |  |
| UART0 | PA11 | RX Pin |
| UART0 | PA10 | TX Pin |
| EVENT |  |  |
| DEBUGSS | PA20 | Debug Clock |
| DEBUGSS | PA19 | Debug Data In Out |

## BoosterPacks, Board Resources & Jumper Settings

Visit [LP_MSPM0G3507](https://www.ti.com/tool/LP-MSPM0G3507) for LaunchPad information, including user guide and hardware files.

| Pin | Peripheral | Function | LaunchPad Pin | LaunchPad Settings |
| --- | --- | --- | --- | --- |
| PB15 | GPIOB | PB15 | J2_17 | N/A |
| PB22 | GPIOB | PB22 | J27_5 | J5 ON/OFF Connect/Disconnect LED2 Blue |
| PB26 | GPIOB | PB26 | J27_8 | J6 ON/OFF Connect/Disconnect LED2 Red |
| PA11 | UART0 | RX | J4_33/J26_6 | J22 1:2 Connect XDS-110 BackChannel UART on J101<br>J22 2:3 Connect to J4_33<br>R63 connects J26_6 for LIN (default)<br>J101 7:8 Connect XDS-110 BackChannel UART RX |
| PA10 | UART0 | TX | J4_34/J26_5 | J21 1:2 Connect XDS-110 BackChannel UART on J101<br>J21 2:3 Connect to J4_34<br>R62 connects J26_5 for LIN (default)<br>J101 9:10 Connect XDS-110 BackChannel UART TX |
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

Note: the BOOSTXL-CANFD-LIN BoosterPack is not directly compatible with the LP_MSPM0L1306 LaunchPad since the pins on the UART connector don't support LIN. For this reason, the BoosterPack shouldn't be stacked on top of the LaunchPad.

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
