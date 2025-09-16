## Example Summary

This code example configures the USB peripheral in device mode with the
Human Interface Device Class (HID). The keyboard interface is enabled
in this example, allowing the Launchpad to behave as a keyboard.

## Example Usage

This example will enable only the keyboard interface. To utilize this
example please connect USB2 on the Launchpad to any host. Once the device is
connected open a text editor and press S2 on the Launchpad. The switch input
will behave as a keyboard press but instead of printing a singular
character, MSPM0 in ascii art will be printed to the text editor.
Pressing cap lock on a keyboard attached to the host will keep the Launchpad
LED on.
