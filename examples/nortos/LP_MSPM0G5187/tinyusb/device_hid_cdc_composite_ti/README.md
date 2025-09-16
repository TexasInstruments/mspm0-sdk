## Example Summary

This code example configures the USB peripheral as a Composite device
with Human Interface Device Class (HID) and Communication Device Class (CDC)
enabled. The example provides a serial port and consumer control class
interface allowing users to send commands through the serial port to
perform actions from the consumer control interface. The consumer control
interface provides users the ability to control multimedia functions on
the host.

## Example Usage

The composite example will enable a serial port and the consumer control
interface. Through the serial port, commands are entered via a terminal
that is then processed by the example code to then perform specific
actions from the consumer control interface. To utilize this example please
connect USB2 on the Launchpad to any host that contains a serial terminal, then
configure with the settings from the Serial Settings table below.
The serial port will allow the user to input text into the terminal and echo
back their input. Upon typing specific commands into the terminal and pressing
enter, the command will be processed then perform specific consumer control
commands. All possible commands utilized by the example are documented in
"List of Commands" below.

## Serial Settings

| Serial Setting | Value |
| --- | --- |
| Baud Rate | 115200 |
| Data Bits | 8 |
| Stop Bits | 1 |
| Parity | None |
| Flow Control | None |

## List of Commands

| Commands | Input | Description |
| --- | --- | --- |
| Mute | mute | Mutes host volume |
| Volume Up | vol up | Increments host volume by 2 |
| Volume Down | vol down | Decrements host volume by 2 |
| Brightness Up | bright up | Increases host brightness |
| Brightness Down | bright down | Decreases host brightness |
| Help | help | Prints to terminal list of possible commands |
| MSPM0 | mspm0 | Prints to terminal "MSPM0 is awesome" |
