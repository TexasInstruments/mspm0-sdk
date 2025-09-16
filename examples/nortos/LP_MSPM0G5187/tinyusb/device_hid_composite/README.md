## Example Summary

This code example configures the USB peripheral in device mode with the
Human Interface Device Class (HID). The example provides a composite
implementation meaning the device can function as multiple HID interfaces
such as keyboard, mouse, or consumer control at once.

## Example Usage

The composite example will enable the keyboard, mouse, consumer control,
and gamepad interface. To utilize this example please connect the USB2
on the Launchpad to any host. Once the device is connected open a text
editor and press S2 on the Launchpad. Upon pressing the switch it
will type the letter "a" into the text editor first, move the cursor down
and right in a diagonal manner, then lower host volume. Gamepad is included
but by default no action is performed using this interface. Pressing cap
lock on a keyboard attached to the host will keep the Launchpad LED on.
