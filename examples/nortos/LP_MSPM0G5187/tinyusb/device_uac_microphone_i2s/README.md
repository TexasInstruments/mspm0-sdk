## Example Summary

This code example configures the USB peripheral in device mode with the
USB Audio Class (UAC) and I2S in target mode. The example will have the
Launchpad behave as a microphone that will send the host I2S data being
sent to the Launchpad.


## Example Usage

To utilize this example please connect USB2 on the Launchpad to any host.
Then send an I2S signal to the Launchpad via a I2S controller or external
peripheral such as a microphone. Once the device is connected to a host and
I2S data is provided, audio data can be captured via external software
of choice such as Audacity or played back through a speaker.
