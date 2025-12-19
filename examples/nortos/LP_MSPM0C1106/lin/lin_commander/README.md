## Example Summary

This example configures the UART as a LIN Commander, and demonstrates basic
transmit and receive of LIN 2.0 packets using enhanced checksum.
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
| TIMA0 | | | | |
| BOARD | Debug Clock | PA20 | J14_4 | |
| BOARD | Debug Data In Out | PA19 | J14_4 | |
| GPIOA | Standard Input | PA18 | S1 | |
| GPIOA | Standard Output | PB20 | LED2 Blue | |
| GPIOB | Standard Output | PB24 | LED2 Red | |
| GPIOB | Standard Input with pull-up | PA14 | S2 | |
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

Connect a LIN Responder to a LIN BoosterPack with the following connections:
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

The LIN Commander is configured to run at 32MHz at 19200 baud. These settings
can be modified in the application.
The commander can transmit data either in polling mode or interrupt mode.

The list of acceptable PID commands are:
  - PID 0x39 / 0x3A / 0x3B
      - Usage: LIN Commander sends a packet containing 8 bytes of data for LIN Responder to receive and store.
      - Packet: [PID, DATA1, DATA2, DATA3, DATA4, DATA5, DATA6, DATA7, DATA8, ENHANCED_CHECKSUM]
      - Example Packet: [0x39, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0xA2]
  - PID 0x08 / 0x09 / 0x0A
      - Usage: LIN Commander tells the LIN Responder to respond with 5 bytes of stored data.
      - Packet: [PID]
      - Example Packet: [0x8]
The PIDs, message size, and functionality of the callback handlers can be modified
and customized in the "gCommanderMessageTable" array in lin_commander.c.

Compile, load and run the example.

When S2 button is pressed, the following LIN 2.0 packet will be transmitted:
  - PID: 0x39
  - Data: [0x1, 0x2, 0x3, 0x4, 0x5, 0x6, 0x7, 0x8]
  - Enhanced Checksum: 0xA2
Each time the S2 button is pressed, LED2 Blue will toggle and the value of the first and last data bytes will be incremented by one.

When S1 button is pressed, the following LIN 2.0 packet will be transmitted:
  - PID: 0x08
  - Data: None
  - Enhanced Checksum: None

It is expected for the LIN Commander to receive 5 bytes of data after in response to PID 0x08.

If the LIN Responder correctly sends a response, then LED2 Red will toggle and the variable "gDataReceived" will be set to true.

The received bytes will be stored in "gCommanderRXBuffer". Each time a new packet of data is received, it will overwrite what was
previously stored in "gCommanderRXBuffer".
