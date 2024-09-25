## Example Summary

This example demonstrates a basic implementation of the SENT protocol using the MCU to transmit a SENT frame.

## Peripherals & Pin Assignments

| Peripheral | Pin | Function |
| --- | --- | --- |
| GPIOA | PA1 | Open-Drain Output |
| SYSCTL |  |  |
| EVENT |  |  |
| DEBUGSS | PA20 | Debug Clock |
| DEBUGSS | PA19 | Debug Data In Out |

## BoosterPacks, Board Resources & Jumper Settings

Visit [LP_MSPM0L1306](https://www.ti.com/tool/LP-MSPM0L1306) for LaunchPad information, including user guide and hardware files.

| Pin | Peripheral | Function | LaunchPad Pin | LaunchPad Settings |
| --- | --- | --- | --- | --- |
| PA1 | GPIOA | PA1 | J1_9 | <ul><li>PA1 is 5V tolerant open-drain so it requires pull-up<ul><li>`J19 2:3` Use 3.3V pull-up <li>`J9 1:2` Use 5V pull-up</ul></ul> |
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

The SENT transmit example is configured to run at 32MHz. These settings
can be modified in the application.

The example configures the device to operate in SENT transmitter mode using the following configuration:

~~~
  tickDuration            = 3;
  pauseConfig.mode        = SENT_PAUSE_MODE_FIXED_FRAME;
  pauseConfig.numOfTicks  = 500;
  numDataNibbles          = SENT_NUM_DATA_NIBBLES_6;
~~~

Two SENT frames are preconfigured and toggled every 1.5 ms (500 ticks)

The example can be configured in the following ways:
- SENT_PACK struct
    - status: 1 status nibble
    - d0 - d5: 6 data nibbles
    - crc: 1 crc nibble
    - pause: pause pulse value

- SENT_config_obj struct
    - tickDuration: number of us per tick (between 1-90 us)
    - pauseConfig: pause configuration struct
    - numDataNibbles: number of data nibbles (configured to 6 by default)

- SENT_PAUSE struct
    - mode: mode for pause pulse configuration
    - number of ticks for pause pulse or frame (depending on configuration)

- SENT_Handler struct
    - *pPeriodTable: look-up table with tick count values per nibble
    - tickDuration: number of us per tick (between 1-90 us)
    - pauseConfig: pause config struct
    - syncCount: timer count for SYNC pulse
    - lowTimeCount: timer count for low ticks of each pulse
    - *pCurrentSentPack: pointer to current SENT packet
    - *pNextSentPack: pointer to next SENT packet
    - numOfNibbles: number of nibbles in data packet (configured to 6 by default)
    - sentState: current state of SENT packet
    - updatePack: flag for when to update pointer to packet


Configuration is done in sent_transmit.c.

Compile, load and run the example.

LED1 will turn on when the SENT packet transfer starts and two alternating fixed frame SENT packets will output on PA1.

The output on PA1 can be connected to the RX of a SENT bus along with a connection between the LaunchPad GND and the SENT bus GND.
