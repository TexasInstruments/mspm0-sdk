## Example Summary

This example configures the UART as a LIN Responder, and demonstrates basic
transmit and receive of LIN 2.0 packets using enhanced checksum. Additionally,
the example has the ability to automatically reconfigure baud rate if the commander
sends data at a rate within a 15% difference of what the responder expects.
LIN is a feature only usable with a UART Extend instance.
This example is provided for reference purposes only.

## Peripherals, Pin Functions, MCU Pins, Launchpad Pins
| Peripheral | Function | MCU Pin | Launchpad Pin | Launchpad Settings |
| --- | --- | --- | --- | --- |
| UART0 | RX Pin | PA11 | J4_35 | |
| UART0 | TX Pin | PA10 | J2_15 | |
| EVENT | | | | |
| DMA | | | | |
| CRCP | | | | |
| TIMG0 | | | | |
| BOARD | Debug Clock | PA20 | J14_4 | |
| BOARD | Debug Data In Out | PA19 | J14_4 | |
| GPIOA | Standard Output | PB20 | LED2 Blue | |
| GPIOB | Standard Output | PB24 | LED2 Red | |
| GPIOB | Standard Output  | PB15 | J3_24 | |

### Device Migration Recommendations
This project was developed for a superset device included in the LP_MSPM0C1106 LaunchPad. Please
visit the [CCS User's Guide](https://software-dl.ti.com/msp430/esd/MSPM0-SDK/latest/docs/english/tools/ccs_ide_guide/doc_guide/doc_guide-srcs/ccs_ide_guide.html#sysconfig-project-migration)
for information about migrating to other MSPM0 devices.

### Low-Power Recommendations
TI recommends to terminate unused pins by setting the corresponding functions to
GPIO and configure the pins to output low or input with internal
pullup/pulldown resistor.

SysConfig allows developers to easily configure unused pins by selecting **Board**→**Configure Unused Pins**.

For more information about jumper configuration to achieve low-power using the
MSPM0 LaunchPad, please visit the [LP_MSPM0C1106 User's Guide](https://www.ti.com/tool/LP-MSPM0C1106).

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

Note: the BOOSTXL-CANFD-LIN BoosterPack is not directly compatible with the LP_MSPM0C1106 LaunchPad since the pins on the UART connector don't support LIN. For this reason, the BoosterPack shouldn't be stacked on top of the LaunchPad.

Connect the LIN Commander BoosterPack and the LIN Responder BoosterPack using the LIN bus lines in J5.

NOTE: Alternatively, a Network Analyzer compatible with LIN 2.0 can be substituted for a LIN
Responder. To use the Network Analyzer, make the following connections between
the Network Analyzer and LIN BoosterPack:
- Network Analyzer GND    -> BoosterPack GND
- Network Analyzer LINbus -> BoosterPack LIN_TERM

The LIN responder is configured to run at 32MHz at 19200 baud. These settings
can be updated in SysConfig. If desired, an automatic baud rate synchronizer can be disabled,
since it is enabled by default. This feature uses the sync field to determine a new baud rate
given the responder receives data at a rate different than what it is expecting. The feature has
a 15% tolerance. To turn off the automatic baud rate synchronization, in "lin_config.h", set "AUTO_BAUD_ENABLED" to false.
The responder can transmit data either in polling mode or interrupt mode.

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

When the LIN Responder receives a packet, LED2 Blue will toggle.

When the LIN Responder transmits a packet, LED2 Red will toggle.
