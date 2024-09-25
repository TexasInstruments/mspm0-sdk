## Example Summary

The SPI-CAN bridge connects the CAN and SPI interfaces. SPI-CAN Bridge allows the external User SPI device (that does not have CAN node) to communicate with external CAN node. The Bridge device supports SPI to work in Peripheral mode. Initially, User should configure the CAN module registers in Bridge device by sending the External SPI frames to bridge SPI with different instruction commands mentioned as below.
- BIT_MODIFY_INSTRUCTION
- RESET_INSTRUCTION
- POWER_ENABLE_AND_CAN_CONFIG_INSTRUCTION
- LOAD_TX_BUFFER_INSTRUCTION
- LOAD_TX_FIFO_INSTRUCTION
- REQUEST_TO_SEND_TXBUFFER_INSTRUCTION
- REQUEST_TO_SEND_TXFIFO_INSTRUCTION
- WRITE_REGISTER_INSTRUCTION
- READ_REGISTER_DATA_INSTRUCTION
- FETCH_REGISTER_DATA_INSTRUCTION
- READ_RX_BUF_DATA_INSTRUCTION
- FETCH_RX_BUF_DATA_INSTRUCTION
- READ_RX_FIFO_DATA_INSTRUCTION
- FETCH_RX_FIFO_DATA_INSTRUCTION
- READ_RX_STATUS
- FETCH_RX_STATUS

The instruction format for above instructions are mentioned in "Instruction Format" section.

## Peripherals & Pin Assignments

| Peripheral | Pin | Function |
| --- | --- | --- |
| CANFD0 | PA12 | TX Pin |
| CANFD0 | PA13 | RX Pin |
| GPIOA | PA0 | Open-Drain Output |
| GPIOB | PB8 | Standard Output |
| GPIOB | PB7 | Standard Output |
| SYSCTL | PA5 | HFXIN |
| SYSCTL | PA6 | HFXOUT |
| SYSCTL | PA3 | LFXIN |
| SYSCTL | PA4 | LFXOUT |
| TIMA0 |  |  |
| SPI1 | PA17 | SPI SCLK (Clock) |
| SPI1 | PA18 | SPI PICO (Peripheral In, Controller Out) |
| SPI1 | PA16 | SPI POCI (Peripheral Out, Controller In) |
| SPI1 | PB1 | SPI CS3/CD (Chip Select 3/Command Data) |
| EVENT |  |  |
| DMA |  |  |
| DEBUGSS | PA20 | Debug Clock |
| DEBUGSS | PA19 | Debug Data In Out |

## BoosterPacks, Board Resources & Jumper Settings

Visit [LP_MSPM0G3507](https://www.ti.com/tool/LP-MSPM0G3507) for LaunchPad information, including user guide and hardware files.

| Pin | Peripheral | Function | LaunchPad Pin | LaunchPad Settings |
| --- | --- | --- | --- | --- |
| PA13 | CANFD0 | CANRX | J4_32/J26_3 | <ul><li>PA13 can be connected to CAN/LIN connector in addition to boosterpack connector:<br><ul><li>To use on J26 CAN/LIN connector:<br>  `R65` is populated by default and connects pin to `J26_3`</ul></ul> |
| PA12 | CANFD0 | CANTX | J4_32/J26_1 | <ul><li>PA12 can be connected to CAN/LIN connector in addition to boosterpack connector:<br><ul><li>To use on J26 CAN/LIN connector:<br>  `R64` is populated by default and connects pin to `J26_1`</ul></ul> |
| PA0 | GPIOA | PA0 | J27_9 | <ul><li>PA0 is 5V tolerant open-drain so it requires pull-up<br><ul><li>`J19 1:2` Use 3.3V pull-up<br><li>`J19 2:3` Use 5V pull-up</ul><br><li>PA0 can be connected to LED1<br><ul><li>`J4 ON` Connect to LED1<br><li>`J4 OFF` Disconnect from LED1</ul></ul> |
| PB8 | GPIOB | PB8 | J2_15 | N/A |
| PB7 | GPIOB | PB7 | J2_14 | N/A |
| PA5 | SYSCTL | HFXIN | J28_4 | <ul><li>PA5 is used for HFXT functionality by default in Launchpad<br><ul><li>`Y2` crystal is soldered and connected by default</ul><br><li>PA5 can be connected to J28_4<br><ul><li>`R15` is not soldered soldered by default<br><li>Solder `R15` to connect pin to J28_4</ul> |
| PA6 | SYSCTL | HFXOUT | J28_5 | <ul><li>PA6 is used for HFXT functionality by default in Launchpad<br><ul><li>`Y2` crystal is soldered and connected by default</ul><br><li>PA6 can be connected to J28_5<br><ul><li>`R17` is not soldered soldered by default<br><li>Solder `R17` to connect pin to J28_5</ul> |
| PA3 | SYSCTL | LFXIN | J28_2 | <ul><li>PA3 is used for LFXT functionality by default in Launchpad<br><ul><li>`Y1` crystal is soldered and connected by default</ul><br><li>PA3 can be connected to J28_2<br><ul><li>`R10` is not soldered soldered by default<br><li>Solder `R10` to connect pin to J28_2</ul> |
| PA4 | SYSCTL | LFXOUT | J28_3 | <ul><li>PA4 is used for LFXT functionality by default in Launchpad<br><ul><li>`Y1` crystal is soldered and connected by default</ul><br><li>PA4 can be connected to J28_3<br><ul><li>`R11` is not soldered soldered by default<br><li>Solder `R11` to connect pin to J28_3</ul> |
| PA17 | SPI1 | SCLK | J3_28 | N/A |
| PA18 | SPI1 | MOSI | J3_26/J3_29 | <ul><li>PA18 can be connected to S1 button to VCC with external pull-down<br><ul><li>`J8 ON` Connect S1 button and external pull-down<br><li>`J8 OFF` Disconnect S1 button and external pull-down</ul><br><li>PA18 can be connected to XDS-110 BSL_Invoke<br><ul><li>`J101 17:18 OFF` Disconnect pin to XDS-110 BSL Invoke</ul><br><li>PA18 can be connected to boosterpack pin J3_29<br><ul><li>`J15 2:3` Connect pin to J3_29<br><li>`J15 1:2/OFF` Disconnect pin from J3_29</ul><br><li>PA18 can be connected to boosterpack pin J3_26<br><ul><li>`R58` is soldered and connects pin to J3_26 by default</ul></ul> |
| PA16 | SPI1 | MISO | J3_29 | <ul><li>PA16 can be connected to boosterpack connector<br><ul><li>`J15 1:2` Connect pin to `J3_29`<br><li>`J15 2:3/OFF` Disconnect pin from `J3_29`</ul></ul> |
| PB1 | SPI1 | CS3_CD_MISO3 | J4_39 | N/A |
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
In the example code, the Bridge SPI is configured to work as Peripheral.
Make the following connections between the External User's SPI (Controller) and the Bridge device SPI (Peripheral):
- External Controller SCLK -> Bridge Peripheral SCLK
- External Controller PICO -> Bridge Peripheral PICO
- External Controller POCI <- Bridge Peripheral POCI
- External Controller CS   -> Bridge Peripheral CS
	
The SPI is initialized with the following configuration:
- SPI Peripheral
- Motorola 4 Wire with Polarity 0, Phase 0
- No parity
- 16 bits per transfer
- MSB first

Make the connections between the External User CAN node and Bridge CAN node.

Compile, load and run the example.

Initially, all the MCAN module register configurations in the bridge device should be done before CAN transmissions by receiving corresponding data from User External SPI to Bridge SPI. Data receiving from User External SPI should be in specific Instruction formats in order to configure Bridge MCAN module properly.

For SPI, the bridge device will support upto 8Mbps bit rate with 16 Bit datasize.

For CAN, Nominal phase and Data phase Bit Rates can be configured with maximum:
- Nominal Bit Rate (1 Mbps)
- Data Bit Rate- (8 Mbps)

The following are the CAN Message RAM configurations implemented in SPI-CAN Bridge:

Each FIFO/Buffer element has 40 bytes size (which includes 32 bytes size data payload)
- 4 TX Buffer elements
- 4 TX FIFO elements
- 4 RX Buffer elements
- 4 RX FIFO-0 elements
- 8 standard filter elements
- 8 Extended filter elements


On receiving the "POWER_ENABLE_AND_CAN_CONFIG_INSTRUCTION", the MCAN module in bridge device will get configured with above Message RAM congurations. 

On receiving any instruction from External SPI, the BUSY_STATUS Pin in bridge device will get set and it gets clear once the bridge process the data corressponding to the instruction. So, It is expected that the External SPI device is in handshacking with the bridge device (on BUSY_STATUS Pin) to maintain data integrity.

On receiving the CAN Frame from External CAN node to the bridge, the bridge will toggle the CAN_NEWMSG_INDICATION Pin to give indication to the External SPI device that Bridge device received the New CAN frame from the External CAN node. 

## Instruction Formats:
The following are the instructions and instruction formats to be followed while sending data from external SPI to Bridge SPI.

BIT_MODIFY_INSTRUCTION:
- Instruction Bytes: 0b0000000000010000
- Instruction format: < Instruction_bytes > <2 offset address bytes> <4 bytes of mask data> <4 bytes of new data>

RESET_INSTRUCTION:
- Instruction Bytes: 0b0000000000100000
- Instruction format: < Instruction_bytes >

POWER_ENABLE_AND_CAN_CONFIG_INSTRUCTION:
- Instruction Bytes: 0b0000000000110000
- Instruction format: < Instruction_bytes >

LOAD_TX_BUFFER_INSTRUCTION:
- Instruction Bytes: 0b0000000001000000
- Instruction format: < Instruction_bytes > < data bytes size to be stored in TXBUFFER> < Data to be stored in TX Buffer>
- Last 2 bits in the instruction indicates the CANFD buffer number to store the data

REQUEST_TO_SEND_TXBUFFER_INSTRUCTION:
- Instruction Bytes: 0b0000000001100000
- Instruction format: < Instruction_bytes >
-  Last 2 bits in the instruction indicates the CANFD buffer number to store the data

LOAD_TX_FIFO_INSTRUCTION:
- Instruction Bytes: 0b0000000001010000
- Instruction format: < Instruction_bytes > < data bytes size to be stored in TXBUFFER> < Data to be stored in TX FIFO>

REQUEST_TO_SEND_TXFIFO_INSTRUCTION:
- Instruction Bytes: 0b0000000001110000
- Instruction format: < Instruction_bytes >

WRITE_REGISTER_INSTRUCTION:
- Instruction Bytes: 0b0000000010000000
- Instruction format: < Instruction_bytes > <2 offset address bytes> <4 bytes of register data to be written>

READ_REGISTER_DATA_INSTRUCTION:
- Instruction Bytes: 0b0000000010010000
- Instruction format: < Instruction_bytes > <2 offset address bytes>

FETCH_REGISTER_DATA_INSTRUCTION:
- Instruction Bytes: 0b0000000010100000
- Instruction format: < Instruction_bytes > < Number of data bytes to fetch (2 or 4)> < Dummy data bytes from controller to receive the register data from bridge>

READ_RX_BUF_DATA_INSTRUCTION:
- Instruction Bytes: 0b0000000010110000
- Instruction format: < Instruction_bytes > < data bytes size to read data from RX Buffer>
- Last 2 bits in the instruction indicates the CANFD buffer number to read the data

FETCH_RX_BUF_DATA_INSTRUCTION:
- Instruction Bytes: 0b0000000011000000
- Instruction format: < Instruction_type > < Number of data bytes to fetch> < Dummy data bytes from controller to receive the RX Buffer data from bridge>

READ_RX_FIFO_DATA_INSTRUCTION:
- Instruction Bytes: 0b0000000011010000
- Instruction format: < Instruction_bytes > < data bytes size to read data from RX Buffer>

FETCH_RX_FIFO_DATA_INSTRUCTION:
- Instruction Bytes: 0b0000000011100000
- Instruction format: < Instruction_type > < Number of data bytes to fetch> < Dummy data bytes from controller to receive the RX Buffer data from bridge>

READ_RX_STATUS:
- Instruction Bytes: 0b0000000011110000
- Instruction format: < Instruction_bytes> < data bytes size to read status (1 or 2)>

FETCH_RX_STATUS:
- Instruction Bytes: 0b0000000000001000
- Instruction format: < Instruction_type> < Number of data bytes to fetch(1 or 2)> < Dummy data bytes from controller to receive the status data from bridge>
