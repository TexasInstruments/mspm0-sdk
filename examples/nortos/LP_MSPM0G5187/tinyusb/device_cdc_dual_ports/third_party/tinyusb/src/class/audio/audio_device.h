/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2020 Ha Thach (tinyusb.org)
 * Copyright (c) 2020 Reinhard Panhuber
 * Copyright (c) 2023 HiFiPhile
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 *
 * This file is part of the TinyUSB stack.
 */
/*!****************************************************************************
 *  @file       audio_device.h
 *  @brief      Audio Device Class Driver implementation
 * 
 * @details This file implements the Audio device driver for TinyUSB stack.
 * It handles the USB Audio protocol including:
 * - Device enumeration and configuration
 * - Audio streaming between USB host and device
 * - Control requests handling (volume, mute, sampling frequency)
 * - Buffer management for audio data
 *
 * The Audio implementation supports:
 * - Multiple audio interfaces
 * - Configurable audio formats and sampling rates
 * - Synchronous and asynchronous audio streaming modes
 * - Volume and mute controls
 * - Multiple audio channels
 * - Feedback for adaptive isochronous transfers
 *
 *   <hr>
 ******************************************************************************/
/** @addtogroup AUDIO_DEVICE
 * @{
 */

#ifndef _TUSB_AUDIO_DEVICE_H_
#define _TUSB_AUDIO_DEVICE_H_

#include "audio.h"

//--------------------------------------------------------------------+
// Class Driver Configuration
//--------------------------------------------------------------------+

// All sizes are in bytes!

#ifndef CFG_TUD_AUDIO_FUNC_1_DESC_LEN
#error You must tell the driver the length of the audio function descriptor including IAD descriptor
#endif
#if CFG_TUD_AUDIO > 1
#ifndef CFG_TUD_AUDIO_FUNC_2_DESC_LEN
#error You must tell the driver the length of the audio function descriptor including IAD descriptor
#endif
#endif
#if CFG_TUD_AUDIO > 2
#ifndef CFG_TUD_AUDIO_FUNC_3_DESC_LEN
#error You must tell the driver the length of the audio function descriptor including IAD descriptor
#endif
#endif

// Number of Standard AS Interface Descriptors (4.9.1) defined per audio function - this is required to be able to remember the current alternate settings of these interfaces
#ifndef CFG_TUD_AUDIO_FUNC_1_N_AS_INT
#error You must tell the driver the number of Standard AS Interface Descriptors you have defined in the audio function descriptor!
#endif
#if CFG_TUD_AUDIO > 1
#ifndef CFG_TUD_AUDIO_FUNC_2_N_AS_INT
#error You must tell the driver the number of Standard AS Interface Descriptors you have defined in the audio function descriptor!
#endif
#endif
#if CFG_TUD_AUDIO > 2
#ifndef CFG_TUD_AUDIO_FUNC_3_N_AS_INT
#error You must tell the driver the number of Standard AS Interface Descriptors you have defined in the audio function descriptor!
#endif
#endif

// Size of control buffer used to receive and send control messages via EP0 - has to be big enough to hold your biggest request structure e.g. range requests with multiple intervals defined or cluster descriptors
#ifndef CFG_TUD_AUDIO_FUNC_1_CTRL_BUF_SZ
#error You must define an audio class control request buffer size!
#endif

#if CFG_TUD_AUDIO > 1
#ifndef CFG_TUD_AUDIO_FUNC_2_CTRL_BUF_SZ
#error You must define an audio class control request buffer size!
#endif
#endif

#if CFG_TUD_AUDIO > 2
#ifndef CFG_TUD_AUDIO_FUNC_3_CTRL_BUF_SZ
#error You must define an audio class control request buffer size!
#endif
#endif

/**
 * @brief Enable Audio IN endpoint (device-to-host audio streaming)
 * 
 * When set to 1, this configuration enables the Audio IN endpoint functionality,
 * allowing the device to transmit audio data to the host. This is used for
 * microphone or audio source functionality. The endpoint sizes are specified in bytes,
 * with maximum limits of 1023 bytes for Full Speed and 1024 bytes for High Speed.
 * 
 * If set to 0, the Audio IN endpoint is disabled, and related code for audio
 * transmission to host will not be compiled.
 */
#ifndef CFG_TUD_AUDIO_ENABLE_EP_IN
#define CFG_TUD_AUDIO_ENABLE_EP_IN 0   // TX
#endif

/**
 * @brief Enable Audio OUT endpoint (host-to-device audio streaming)
 * 
 * When set to 1, this configuration enables the Audio OUT endpoint functionality,
 * allowing the device to receive audio data from the host. This is used for
 * speaker or audio sink functionality. The endpoint sizes are specified in bytes,
 * with maximum limits of 1023 bytes for Full Speed and 1024 bytes for High Speed.
 * 
 * If set to 0, the Audio OUT endpoint is disabled, and related code for audio
 * reception from host will not be compiled.
 */
#ifndef CFG_TUD_AUDIO_ENABLE_EP_OUT
#define CFG_TUD_AUDIO_ENABLE_EP_OUT 0  // RX
#endif

// Maximum EP sizes for all alternate AS interface settings - used for checks and buffer allocation
#if CFG_TUD_AUDIO_ENABLE_EP_IN
#ifndef CFG_TUD_AUDIO_FUNC_1_EP_IN_SZ_MAX
#error You must tell the driver the biggest EP IN size!
#endif
#if CFG_TUD_AUDIO > 1
#ifndef CFG_TUD_AUDIO_FUNC_2_EP_IN_SZ_MAX
#error You must tell the driver the biggest EP IN size!
#endif
#endif
#if CFG_TUD_AUDIO > 2
#ifndef CFG_TUD_AUDIO_FUNC_3_EP_IN_SZ_MAX
#error You must tell the driver the biggest EP IN size!
#endif
#endif
#endif // CFG_TUD_AUDIO_ENABLE_EP_IN

#if CFG_TUD_AUDIO_ENABLE_EP_OUT
#ifndef CFG_TUD_AUDIO_FUNC_1_EP_OUT_SZ_MAX
#error You must tell the driver the biggest EP OUT size!
#endif
#if CFG_TUD_AUDIO > 1
#ifndef CFG_TUD_AUDIO_FUNC_2_EP_OUT_SZ_MAX
#error You must tell the driver the biggest EP OUT size!
#endif
#endif
#if CFG_TUD_AUDIO > 2
#ifndef CFG_TUD_AUDIO_FUNC_3_EP_OUT_SZ_MAX
#error You must tell the driver the biggest EP OUT size!
#endif
#endif
#endif // CFG_TUD_AUDIO_ENABLE_EP_OUT

/**
 * @brief Software buffer size for Audio function 1's IN endpoint (host ← device)
 * 
 * @details Defines the size of the software FIFO buffer used for the audio IN endpoint 
 * of the first audio function. This buffer stores audio data before transmission to the host.
 * 
 * The buffer size MUST be greater than or equal to the maximum endpoint size
 * (CFG_TUD_AUDIO_FUNC_1_EP_IN_SZ_MAX). This is required to ensure proper data handling
 * without buffer overflows.
 */
#ifndef CFG_TUD_AUDIO_FUNC_1_EP_IN_SW_BUF_SZ
#define CFG_TUD_AUDIO_FUNC_1_EP_IN_SW_BUF_SZ                0
#endif

/**
 * @brief Software buffer size for Audio function 2's IN endpoint (host ← device)
 * 
 * @details Defines the size of the software FIFO buffer used for the audio IN endpoint 
 * of the first audio function. This buffer stores audio data before transmission to the host.
 * 
 * The buffer size MUST be greater than or equal to the maximum endpoint size
 * (CFG_TUD_AUDIO_FUNC_2_EP_IN_SZ_MAX). This is required to ensure proper data handling
 * without buffer overflows.
 */
#ifndef CFG_TUD_AUDIO_FUNC_2_EP_IN_SW_BUF_SZ
#define CFG_TUD_AUDIO_FUNC_2_EP_IN_SW_BUF_SZ                0
#endif

/**
 * @brief Software buffer size for Audio function 3's IN endpoint (host ← device)
 * 
 * @details Defines the size of the software FIFO buffer used for the audio IN endpoint 
 * of the first audio function. This buffer stores audio data before transmission to the host.
 * 
 * The buffer size MUST be greater than or equal to the maximum endpoint size
 * (CFG_TUD_AUDIO_FUNC_3_EP_IN_SZ_MAX). This is required to ensure proper data handling
 * without buffer overflows.
 */
#ifndef CFG_TUD_AUDIO_FUNC_3_EP_IN_SW_BUF_SZ
#define CFG_TUD_AUDIO_FUNC_3_EP_IN_SW_BUF_SZ                0
#endif

/**
 * @brief Software buffer size for Audio function 1's OUT endpoint (host → device)
 * 
 * @details Defines the size of the software FIFO buffer used for the audio OUT endpoint 
 * of the first audio function. This buffer stores audio data received from the host.
 * 
 * The buffer size MUST be greater than or equal to the maximum endpoint size
 * (CFG_TUD_AUDIO_FUNC_1_EP_OUT_SZ_MAX). This is required to ensure proper data handling
 * without buffer overflows.
 */
#ifndef CFG_TUD_AUDIO_FUNC_1_EP_OUT_SW_BUF_SZ
#define CFG_TUD_AUDIO_FUNC_1_EP_OUT_SW_BUF_SZ               0
#endif

/**
 * @brief Software buffer size for Audio function 2's OUT endpoint (host → device)
 * 
 * @details Defines the size of the software FIFO buffer used for the audio OUT endpoint 
 * of the first audio function. This buffer stores audio data received from the host.
 * 
 * The buffer size MUST be greater than or equal to the maximum endpoint size
 * (CFG_TUD_AUDIO_FUNC_2_EP_OUT_SZ_MAX). This is required to ensure proper data handling
 * without buffer overflows.
 */
#ifndef CFG_TUD_AUDIO_FUNC_2_EP_OUT_SW_BUF_SZ
#define CFG_TUD_AUDIO_FUNC_2_EP_OUT_SW_BUF_SZ               0
#endif

/**
 * @brief Software buffer size for Audio function 3's OUT endpoint (host → device)
 * 
 * @details Defines the size of the software FIFO buffer used for the audio OUT endpoint 
 * of the first audio function. This buffer stores audio data received from the host.
 * 
 * The buffer size MUST be greater than or equal to the maximum endpoint size
 * (CFG_TUD_AUDIO_FUNC_3_EP_OUT_SZ_MAX). This is required to ensure proper data handling
 * without buffer overflows.
 */
#ifndef CFG_TUD_AUDIO_FUNC_3_EP_OUT_SW_BUF_SZ
#define CFG_TUD_AUDIO_FUNC_3_EP_OUT_SW_BUF_SZ               0
#endif

#if CFG_TUD_AUDIO_ENABLE_EP_IN
#if CFG_TUD_AUDIO_FUNC_1_EP_IN_SW_BUF_SZ < CFG_TUD_AUDIO_FUNC_1_EP_IN_SZ_MAX
#error EP software buffer size MUST BE at least as big as maximum EP size
#endif

#if CFG_TUD_AUDIO > 1
#if CFG_TUD_AUDIO_FUNC_2_EP_IN_SW_BUF_SZ < CFG_TUD_AUDIO_FUNC_2_EP_IN_SZ_MAX
#error EP software buffer size MUST BE at least as big as maximum EP size
#endif
#endif

#if CFG_TUD_AUDIO > 2
#if CFG_TUD_AUDIO_FUNC_3_EP_IN_SW_BUF_SZ < CFG_TUD_AUDIO_FUNC_3_EP_IN_SZ_MAX
#error EP software buffer size MUST BE at least as big as maximum EP size
#endif
#endif
#endif

#if CFG_TUD_AUDIO_ENABLE_EP_OUT
#if CFG_TUD_AUDIO_FUNC_1_EP_OUT_SW_BUF_SZ < CFG_TUD_AUDIO_FUNC_1_EP_OUT_SZ_MAX
#error EP software buffer size MUST BE at least as big as maximum EP size
#endif

#if CFG_TUD_AUDIO > 1
#if CFG_TUD_AUDIO_FUNC_2_EP_OUT_SW_BUF_SZ < CFG_TUD_AUDIO_FUNC_2_EP_OUT_SZ_MAX
#error EP software buffer size MUST BE at least as big as maximum EP size
#endif
#endif

#if CFG_TUD_AUDIO > 2
#if CFG_TUD_AUDIO_FUNC_3_EP_OUT_SW_BUF_SZ < CFG_TUD_AUDIO_FUNC_3_EP_OUT_SZ_MAX
#error EP software buffer size MUST BE at least as big as maximum EP size
#endif
#endif
#endif

/**
 * @brief Controls flow control behavior for Audio IN endpoint (host ← device)
 * 
 * @details This configuration option determines how flow control is handled for 
 * the Audio IN endpoint (data flowing from device to host).
 * 
 * When enabled (1), the stack will automatically manage flow control by:
 * - Detecting buffer overflow/underflow conditions
 * - Adjusting the data rate to maintain synchronization with the host
 * - Handling feedback data from the host to ensure proper isochronous transfers
 * 
 * When disabled (0), the application is responsible for handling flow control
 * and maintaining proper data flow to avoid buffer issues.
 */
#ifndef CFG_TUD_AUDIO_EP_IN_FLOW_CONTROL
#define CFG_TUD_AUDIO_EP_IN_FLOW_CONTROL  1
#endif

/**
 * @brief Enables the Audio feedback endpoint
 * 
 * @details This configuration option controls whether the Audio class driver
 * enables a dedicated feedback endpoint for isochronous adaptive or asynchronous
 * audio streaming.
 * 
 * When enabled (1):
 * - The stack will create and manage an additional feedback endpoint
 * - This endpoint allows the device to communicate timing information to the host
 * - Helps maintain synchronization between host and device clocks
 * - Required for adaptive and asynchronous isochronous transfers
 * 
 * When disabled (0):
 * - No feedback endpoint is created
 * - Only synchronous isochronous transfers are supported
 * - The device and host must share the same clock or use other synchronization methods
 */
#ifndef CFG_TUD_AUDIO_ENABLE_FEEDBACK_EP
#define CFG_TUD_AUDIO_ENABLE_FEEDBACK_EP                    0                             // Feedback - 0 or 1
#endif

/**
 * @brief Enables automatic feedback format correction
 * 
 * @details This configuration option controls whether the Audio class driver
 * automatically corrects the format of feedback data sent to the host.
 * 
 * When enabled (1):
 * - The stack will automatically convert feedback values to the proper format
 *   required by the USB Audio Class specification
 * - Ensures that feedback data is correctly formatted as 10.14 or 16.16 
 *   fixed-point values depending on the speed mode
 * - Handles endianness conversion and bit-precision adjustments
 * - Simplifies application development by abstracting format details
 * 
 * When disabled (0):
 * - The application must provide correctly formatted feedback values
 * - The application is responsible for ensuring compliance with USB Audio
 *   format requirements
 */
#ifndef CFG_TUD_AUDIO_ENABLE_FEEDBACK_FORMAT_CORRECTION
#define CFG_TUD_AUDIO_ENABLE_FEEDBACK_FORMAT_CORRECTION     0                             // 0 or 1
#endif

/**
 * @brief Enables the Audio interrupt endpoint
 * 
 * @details This configuration option controls whether the Audio class driver
 * creates and manages an interrupt endpoint for control and status information.
 * 
 * When enabled (1):
 * - The stack will create and manage an interrupt endpoint
 * - This endpoint allows the device to send status information and notifications to the host
 * - Enables reporting of volume changes, mute status updates, and other control events
 * - Supports features like hardware button presses on audio devices
 * 
 * When disabled (0):
 * - No interrupt endpoint is created
 * - Status changes can only be discovered by the host through polling control requests
 * - Reduces endpoint usage but limits notification capabilities
 */
#ifndef CFG_TUD_AUDIO_ENABLE_INTERRUPT_EP
#define CFG_TUD_AUDIO_ENABLE_INTERRUPT_EP                   0                             // Feedback - 0 or 1
#endif

/**
 * @brief Size of the Audio interrupt endpoint
 * 
 * @details Defines the maximum packet size for the interrupt endpoint used by
 * the Audio class driver for sending status and control information to the host.
 * 
 * This value determines how much status data can be sent in a single interrupt
 * transfer. For most audio control applications, a small size (2-8 bytes) is
 * sufficient as interrupt transfers typically contain minimal control information
 * such as volume changes, mute status, or button presses.
 * 
 * The USB specification limits interrupt endpoint sizes to:
 * - Full-speed: 64 bytes maximum
 * - High-speed: 1024 bytes maximum
 */
#define CFG_TUD_AUDIO_INTERRUPT_EP_SZ                       6

// Use software encoding/decoding

// The software coding feature of the driver is not mandatory. It is useful if, for instance, you have two I2S streams which need to be interleaved
// into a single PCM stream as SAMPLE_1 | SAMPLE_2 | SAMPLE_3 | SAMPLE_4.
//
// Currently, only PCM type I encoding/decoding is supported!
//
// If the coding feature is to be used, support FIFOs need to be configured. Their sizes and numbers are defined below.

// Encoding/decoding is done in software and thus time consuming. If you can encode/decode your stream more efficiently do not use the
// support FIFOs but write/read directly into/from the EP_X_SW_BUFFER_FIFOs using
// - tud_audio_n_write() or
// - tud_audio_n_read().
// To write/read to/from the support FIFOs use
// - tud_audio_n_write_support_ff() or
// - tud_audio_n_read_support_ff().
//
// The encoding/decoding format type done is defined below.
//
// The encoding/decoding starts when the private callback functions
// - audio_tx_done_cb()
// - audio_rx_done_cb()
// are invoked. If support FIFOs are used, the corresponding encoding/decoding functions are called from there.
// Once encoding/decoding is done the result is put directly into the EP_X_SW_BUFFER_FIFOs. You can use the public callback functions
// - tud_audio_tx_done_pre_load_cb() or tud_audio_tx_done_post_load_cb()
// - tud_audio_rx_done_pre_read_cb() or tud_audio_rx_done_post_read_cb()
// if you want to get informed what happened.
//
// If you don't use the support FIFOs you may use the public callback functions
// - tud_audio_tx_done_pre_load_cb() or tud_audio_tx_done_post_load_cb()
// - tud_audio_rx_done_pre_read_cb() or tud_audio_rx_done_post_read_cb()
// to write/read from/into the EP_X_SW_BUFFER_FIFOs at the right time.
//
// If you need a different encoding which is not support so far implement it in the
// - audio_tx_done_cb()
// - audio_rx_done_cb()
// functions.

// Enable encoding/decodings - for these to work, support FIFOs need to be setup in appropriate numbers and size
// The actual coding parameters of active AS alternate interface is parsed from the descriptors

// The item size of the FIFO is always fixed to one i.e. bytes! Furthermore, the actively used FIFO depth is reconfigured such that the depth is a multiple
// of the current sample size in order to avoid samples to get split up in case of a wrap in the FIFO ring buffer (depth = (max_depth / sample_sz) * sample_sz)!
// This is important to remind in case you use DMAs! If the sample sizes changes, the DMA MUST BE RECONFIGURED just like the FIFOs for a different depth!!!

// For PCM encoding/decoding

/**
 * @brief Enables audio encoding/decoding support
 * 
 * @details This configuration option controls whether the Audio class driver
 * includes support for encoding and decoding audio data formats.
 * 
 * When enabled (1):
 * - The stack includes code for processing different audio encodings
 * - Supports conversion between encoded formats and PCM
 * - Allows the device to handle compressed audio formats
 * - Enables features like on-device encoding/decoding of audio streams
 * 
 * When disabled (0):
 * - Only raw audio data transfer is supported
 * - Reduces code size by excluding encoding/decoding functionality
 * - The application must handle any format conversions externally
 */
#ifndef CFG_TUD_AUDIO_ENABLE_ENCODING
#define CFG_TUD_AUDIO_ENABLE_ENCODING                       0
#endif

/**
 * @brief Enables audio decoding support
 * 
 * @details This configuration option controls whether the Audio class driver
 * includes support for decoding audio data from various formats to PCM.
 * 
 * When enabled (1):
 * - The stack includes code for decoding compressed or encoded audio formats
 * - Supports conversion from encoded formats (like MP3, AAC, etc.) to PCM
 * - Allows the device to receive compressed audio and process it natively
 * - Enables features like on-device playback of compressed audio streams
 * 
 * When disabled (0):
 * - Only raw audio data handling is supported
 * - Reduces code size by excluding decoding functionality
 * - The application must handle any format decoding externally
 */
#ifndef CFG_TUD_AUDIO_ENABLE_DECODING
#define CFG_TUD_AUDIO_ENABLE_DECODING                       0
#endif

/**
 * @brief Enables Type I audio encoding support
 * 
 * @details This configuration option controls whether the Audio class driver
 * includes support for Type I audio encoding as defined in the USB Audio Class
 * specification.
 */
#ifndef CFG_TUD_AUDIO_ENABLE_TYPE_I_ENCODING
#define CFG_TUD_AUDIO_ENABLE_TYPE_I_ENCODING                0
#endif

/**
 * @brief Enables Type I audio decoding support
 * 
 * @details This configuration option controls whether the Audio class driver
 * includes support for Type I audio decoding as defined in the USB Audio Class
 * specification.
 */
#ifndef CFG_TUD_AUDIO_ENABLE_TYPE_I_DECODING
#define CFG_TUD_AUDIO_ENABLE_TYPE_I_DECODING                0
#endif

// Type I Coding parameters not given within UAC2 descriptors
// It would be possible to allow for a more flexible setting and not fix this parameter as done below. However, this is most often not needed and kept for later if really necessary. The more flexible setting could be implemented within set_interface(), however, how the values are saved per alternate setting is to be determined!
#if CFG_TUD_AUDIO_ENABLE_EP_IN && CFG_TUD_AUDIO_ENABLE_ENCODING && CFG_TUD_AUDIO_ENABLE_TYPE_I_ENCODING
#ifndef CFG_TUD_AUDIO_FUNC_1_CHANNEL_PER_FIFO_TX
#error You must tell the driver the number of channels per FIFO for the interleaved encoding! E.g. for an I2S interface having two channels, CHANNEL_PER_FIFO = 2 as the I2S stream having two channels is usually saved within one FIFO
#endif
#if CFG_TUD_AUDIO > 1
#ifndef CFG_TUD_AUDIO_FUNC_2_CHANNEL_PER_FIFO_TX
#error You must tell the driver the number of channels per FIFO for the interleaved encoding! E.g. for an I2S interface having two channels, CHANNEL_PER_FIFO = 2 as the I2S stream having two channels is usually saved within one FIFO
#endif
#endif
#if CFG_TUD_AUDIO > 2
#ifndef CFG_TUD_AUDIO_FUNC_3_CHANNEL_PER_FIFO_TX
#error You must tell the driver the number of channels per FIFO for the interleaved encoding! E.g. for an I2S interface having two channels, CHANNEL_PER_FIFO = 2 as the I2S stream having two channels is usually saved within one FIFO
#endif
#endif
#endif

#if CFG_TUD_AUDIO_ENABLE_EP_OUT && CFG_TUD_AUDIO_ENABLE_DECODING && CFG_TUD_AUDIO_ENABLE_TYPE_I_DECODING
#ifndef CFG_TUD_AUDIO_FUNC_1_CHANNEL_PER_FIFO_RX
#error You must tell the driver the number of channels per FIFO for the interleaved encoding! E.g. for an I2S interface having two channels, CHANNEL_PER_FIFO = 2 as the I2S stream having two channels is usually saved within one FIFO
#endif
#if CFG_TUD_AUDIO > 1
#ifndef CFG_TUD_AUDIO_FUNC_2_CHANNEL_PER_FIFO_RX
#error You must tell the driver the number of channels per FIFO for the interleaved encoding! E.g. for an I2S interface having two channels, CHANNEL_PER_FIFO = 2 as the I2S stream having two channels is usually saved within one FIFO
#endif
#endif
#if CFG_TUD_AUDIO > 2
#ifndef CFG_TUD_AUDIO_FUNC_3_CHANNEL_PER_FIFO_RX
#error You must tell the driver the number of channels per FIFO for the interleaved encoding! E.g. for an I2S interface having two channels, CHANNEL_PER_FIFO = 2 as the I2S stream having two channels is usually saved within one FIFO
#endif
#endif
#endif

// Remaining types not support so far

// Number of support FIFOs to set up - multiple channels can be handled by one FIFO - very common is two channels per FIFO stemming from one I2S interface

/**
 * @brief Number of supported software FIFOs for TX (device → host) for Audio function 1
 * 
 * @details Defines how many software FIFO buffers are supported for transmitting
 * audio data from the device to the host for Audio function 1.
 * 
 * Each FIFO represents a separate audio data channel or stream that can be
 * transmitted independently. Multiple FIFOs are useful for:
 * - Stereo or multi-channel audio (e.g., one FIFO per audio channel)
 * - Supporting multiple sample rates or bit depths simultaneously
 * - Handling different audio formats in parallel
 */
#ifndef CFG_TUD_AUDIO_FUNC_1_N_TX_SUPP_SW_FIFO
#define CFG_TUD_AUDIO_FUNC_1_N_TX_SUPP_SW_FIFO              0
#endif

/**
 * @brief Number of supported software FIFOs for TX (device → host) for Audio function 2
 * 
 * @details Defines how many software FIFO buffers are supported for transmitting
 * audio data from the device to the host for Audio function 2.
 * 
 * Each FIFO represents a separate audio data channel or stream that can be
 * transmitted independently. Multiple FIFOs are useful for:
 * - Stereo or multi-channel audio (e.g., one FIFO per audio channel)
 * - Supporting multiple sample rates or bit depths simultaneously
 * - Handling different audio formats in parallel
 */
#ifndef CFG_TUD_AUDIO_FUNC_2_N_TX_SUPP_SW_FIFO
#define CFG_TUD_AUDIO_FUNC_2_N_TX_SUPP_SW_FIFO              0
#endif

/**
 * @brief Number of supported software FIFOs for TX (device → host) for Audio function 3
 * 
 * @details Defines how many software FIFO buffers are supported for transmitting
 * audio data from the device to the host for Audio function 3.
 * 
 * Each FIFO represents a separate audio data channel or stream that can be
 * transmitted independently. Multiple FIFOs are useful for:
 * - Stereo or multi-channel audio (e.g., one FIFO per audio channel)
 * - Supporting multiple sample rates or bit depths simultaneously
 * - Handling different audio formats in parallel
 */
#ifndef CFG_TUD_AUDIO_FUNC_3_N_TX_SUPP_SW_FIFO
#define CFG_TUD_AUDIO_FUNC_3_N_TX_SUPP_SW_FIFO              0
#endif

/**
 * @brief Number of supported software FIFOs for RX (host → device) for Audio function 1
 * 
 * @details Defines how many software FIFO buffers are supported for receiving
 * audio data from the host to the device for Audio function 1.
 * 
 * Each FIFO represents a separate audio data channel or stream that can be
 * received independently. Multiple FIFOs are useful for:
 * - Stereo or multi-channel audio (e.g., one FIFO per audio channel)
 * - Supporting multiple sample rates or bit depths simultaneously
 * - Handling different audio formats in parallel
 */
#ifndef CFG_TUD_AUDIO_FUNC_1_N_RX_SUPP_SW_FIFO
#define CFG_TUD_AUDIO_FUNC_1_N_RX_SUPP_SW_FIFO              0
#endif

/**
 * @brief Number of supported software FIFOs for RX (host → device) for Audio function 2
 * 
 * @details Defines how many software FIFO buffers are supported for receiving
 * audio data from the host to the device for Audio function 2.
 * 
 * Each FIFO represents a separate audio data channel or stream that can be
 * received independently. Multiple FIFOs are useful for:
 * - Stereo or multi-channel audio (e.g., one FIFO per audio channel)
 * - Supporting multiple sample rates or bit depths simultaneously
 * - Handling different audio formats in parallel
 */
#ifndef CFG_TUD_AUDIO_FUNC_2_N_RX_SUPP_SW_FIFO
#define CFG_TUD_AUDIO_FUNC_2_N_RX_SUPP_SW_FIFO              0
#endif

/**
 * @brief Number of supported software FIFOs for RX (host → device) for Audio function 3
 * 
 * @details Defines how many software FIFO buffers are supported for receiving
 * audio data from the host to the device for Audio function 3.
 * 
 * Each FIFO represents a separate audio data channel or stream that can be
 * received independently. Multiple FIFOs are useful for:
 * - Stereo or multi-channel audio (e.g., one FIFO per audio channel)
 * - Supporting multiple sample rates or bit depths simultaneously
 * - Handling different audio formats in parallel
 */
#ifndef CFG_TUD_AUDIO_FUNC_3_N_RX_SUPP_SW_FIFO
#define CFG_TUD_AUDIO_FUNC_3_N_RX_SUPP_SW_FIFO              0
#endif

// Size of support FIFOs IN BYTES - if size > 0 there are as many FIFOs set up as CFG_TUD_AUDIO_FUNC_X_N_TX_SUPP_SW_FIFO and CFG_TUD_AUDIO_FUNC_X_N_RX_SUPP_SW_FIFO

/**
 * @brief Number of supported software FIFOs for RX (host → device) for Audio function 1
 * 
 * @details Defines how many software FIFO buffers are supported for receiving
 * audio data from the host to the device for Audio function 1.
 * 
 * Each FIFO represents a separate audio data channel or stream that can be
 * received independently. Multiple FIFOs are useful for:
 * - Stereo or multi-channel audio (e.g., one FIFO per audio channel)
 * - Supporting multiple sample rates or bit depths simultaneously
 * - Handling different audio formats in parallel
 */
#ifndef CFG_TUD_AUDIO_FUNC_1_TX_SUPP_SW_FIFO_SZ
#define CFG_TUD_AUDIO_FUNC_1_TX_SUPP_SW_FIFO_SZ             0         // FIFO size - minimum size: ceil(f_s/1000) * max(# of TX channels) / (# of TX support FIFOs) * max(# of bytes per sample)
#endif

/**
 * @brief Number of supported software FIFOs for RX (host → device) for Audio function 2
 * 
 * @details Defines how many software FIFO buffers are supported for receiving
 * audio data from the host to the device for Audio function 2.
 * 
 * Each FIFO represents a separate audio data channel or stream that can be
 * received independently. Multiple FIFOs are useful for:
 * - Stereo or multi-channel audio (e.g., one FIFO per audio channel)
 * - Supporting multiple sample rates or bit depths simultaneously
 * - Handling different audio formats in parallel
 */
#ifndef CFG_TUD_AUDIO_FUNC_2_TX_SUPP_SW_FIFO_SZ
#define CFG_TUD_AUDIO_FUNC_2_TX_SUPP_SW_FIFO_SZ             0
#endif

/**
 * @brief Number of supported software FIFOs for RX (host → device) for Audio function 3
 * 
 * @details Defines how many software FIFO buffers are supported for receiving
 * audio data from the host to the device for Audio function 3.
 * 
 * Each FIFO represents a separate audio data channel or stream that can be
 * received independently. Multiple FIFOs are useful for:
 * - Stereo or multi-channel audio (e.g., one FIFO per audio channel)
 * - Supporting multiple sample rates or bit depths simultaneously
 * - Handling different audio formats in parallel
 */
#ifndef CFG_TUD_AUDIO_FUNC_3_TX_SUPP_SW_FIFO_SZ
#define CFG_TUD_AUDIO_FUNC_3_TX_SUPP_SW_FIFO_SZ             0
#endif

/**
 * @brief Size of each software FIFO for RX (host → device) for Audio function 1
 * 
 * @details Defines the size in bytes of each software FIFO buffer used for receiving
 * audio data from the host to the device for Audio function 1.
 */
#ifndef CFG_TUD_AUDIO_FUNC_1_RX_SUPP_SW_FIFO_SZ
#define CFG_TUD_AUDIO_FUNC_1_RX_SUPP_SW_FIFO_SZ             0         // FIFO size - minimum size: ceil(f_s/1000) * max(# of RX channels) / (# of RX support FIFOs) * max(# of bytes per sample)
#endif

/**
 * @brief Size of each software FIFO for RX (host → device) for Audio function 2
 * 
 * @details Defines the size in bytes of each software FIFO buffer used for receiving
 * audio data from the host to the device for Audio function 2.
 */
#ifndef CFG_TUD_AUDIO_FUNC_2_RX_SUPP_SW_FIFO_SZ
#define CFG_TUD_AUDIO_FUNC_2_RX_SUPP_SW_FIFO_SZ             0
#endif

/**
 * @brief Size of each software FIFO for RX (host → device) for Audio function 3
 * 
 * @details Defines the size in bytes of each software FIFO buffer used for receiving
 * audio data from the host to the device for Audio function 3.
 */
#ifndef CFG_TUD_AUDIO_FUNC_3_RX_SUPP_SW_FIFO_SZ
#define CFG_TUD_AUDIO_FUNC_3_RX_SUPP_SW_FIFO_SZ             0
#endif

//static_assert(sizeof(tud_audio_desc_lengths) != CFG_TUD_AUDIO, "Supply audio function descriptor pack length!");

// Supported types of this driver:
// AUDIO_DATA_FORMAT_TYPE_I_PCM     -   Required definitions: CFG_TUD_AUDIO_N_CHANNELS and CFG_TUD_AUDIO_BYTES_PER_CHANNEL

#ifdef __cplusplus
extern "C" {
#endif

//--------------------------------------------------------------------+
// Application API (Multiple Interfaces)
// CFG_TUD_AUDIO > 1
//--------------------------------------------------------------------+
/**
 * @brief Check if the specified audio function is mounted by the host
 * 
 * This function checks if the specified audio function has been configured and
 * mounted by the USB host. An audio function is considered mounted when the host
 * has completed device configuration and the interface is ready for use.
 * 
 * @param[in] func_id    Audio function ID (zero-based index, less than CFG_TUD_AUDIO)
 * 
 * @return true if the audio function is mounted and ready for use
 * @return false if the audio function is not mounted or the func_id is invalid
 */
bool     tud_audio_n_mounted    (uint8_t func_id);

#if CFG_TUD_AUDIO_ENABLE_EP_OUT && !CFG_TUD_AUDIO_ENABLE_DECODING
uint16_t tud_audio_n_available                    (uint8_t func_id);
uint16_t tud_audio_n_read                         (uint8_t func_id, void* buffer, uint16_t bufsize);
bool     tud_audio_n_clear_ep_out_ff              (uint8_t func_id);                          // Delete all content in the EP OUT FIFO
tu_fifo_t*   tud_audio_n_get_ep_out_ff            (uint8_t func_id);
#endif

#if CFG_TUD_AUDIO_ENABLE_EP_OUT && CFG_TUD_AUDIO_ENABLE_DECODING
bool     tud_audio_n_clear_rx_support_ff          (uint8_t func_id, uint8_t ff_idx);       // Delete all content in the support RX FIFOs
uint16_t tud_audio_n_available_support_ff         (uint8_t func_id, uint8_t ff_idx);
uint16_t tud_audio_n_read_support_ff              (uint8_t func_id, uint8_t ff_idx, void* buffer, uint16_t bufsize);
tu_fifo_t* tud_audio_n_get_rx_support_ff          (uint8_t func_id, uint8_t ff_idx);
#endif

#if CFG_TUD_AUDIO_ENABLE_EP_IN && !CFG_TUD_AUDIO_ENABLE_ENCODING
uint16_t tud_audio_n_write                        (uint8_t func_id, const void * data, uint16_t len);
bool     tud_audio_n_clear_ep_in_ff               (uint8_t func_id);                          // Delete all content in the EP IN FIFO
tu_fifo_t*   tud_audio_n_get_ep_in_ff             (uint8_t func_id);
#endif

#if CFG_TUD_AUDIO_ENABLE_EP_IN && CFG_TUD_AUDIO_ENABLE_ENCODING
uint16_t tud_audio_n_flush_tx_support_ff          (uint8_t func_id);      // Force all content in the support TX FIFOs to be written into EP SW FIFO
bool     tud_audio_n_clear_tx_support_ff          (uint8_t func_id, uint8_t ff_idx);
uint16_t tud_audio_n_write_support_ff             (uint8_t func_id, uint8_t ff_idx, const void * data, uint16_t len);
tu_fifo_t* tud_audio_n_get_tx_support_ff          (uint8_t func_id, uint8_t ff_idx);
#endif

#if CFG_TUD_AUDIO_ENABLE_INTERRUPT_EP
bool    tud_audio_int_n_write                     (uint8_t func_id, const audio_interrupt_data_t * data);
#endif


//--------------------------------------------------------------------+
// Application API (Interface0)
//--------------------------------------------------------------------+

/**
 * @brief Check if the default audio function is mounted by the host
 * 
 * This function checks if the first audio function (function ID 0) has been 
 * configured and mounted by the USB host. An audio function is considered 
 * mounted when the host has completed device configuration and the interface 
 * is ready for use.
 * 
 * This is a convenience wrapper around tud_audio_n_mounted(0).
 * 
 * @return true if the default audio function is mounted and ready for use
 * @return false if the default audio function is not mounted
 */
static inline bool         tud_audio_mounted                (void);

// RX API

#if CFG_TUD_AUDIO_ENABLE_EP_OUT && !CFG_TUD_AUDIO_ENABLE_DECODING
static inline uint16_t     tud_audio_available              (void);
static inline bool         tud_audio_clear_ep_out_ff        (void);                       // Delete all content in the EP OUT FIFO
static inline uint16_t     tud_audio_read                   (void* buffer, uint16_t bufsize);
static inline tu_fifo_t*   tud_audio_get_ep_out_ff          (void);
#endif

#if CFG_TUD_AUDIO_ENABLE_EP_OUT && CFG_TUD_AUDIO_ENABLE_DECODING
static inline bool     tud_audio_clear_rx_support_ff        (uint8_t ff_idx);
static inline uint16_t tud_audio_available_support_ff       (uint8_t ff_idx);
static inline uint16_t tud_audio_read_support_ff            (uint8_t ff_idx, void* buffer, uint16_t bufsize);
static inline tu_fifo_t* tud_audio_get_rx_support_ff        (uint8_t ff_idx);
#endif

// TX API

#if CFG_TUD_AUDIO_ENABLE_EP_IN && !CFG_TUD_AUDIO_ENABLE_ENCODING
static inline uint16_t tud_audio_write                      (const void * data, uint16_t len);
static inline bool 	   tud_audio_clear_ep_in_ff             (void);
static inline tu_fifo_t* tud_audio_get_ep_in_ff             (void);
#endif

#if CFG_TUD_AUDIO_ENABLE_EP_IN && CFG_TUD_AUDIO_ENABLE_ENCODING
static inline uint16_t tud_audio_flush_tx_support_ff        (void);
static inline uint16_t tud_audio_clear_tx_support_ff        (uint8_t ff_idx);
static inline uint16_t tud_audio_write_support_ff           (uint8_t ff_idx, const void * data, uint16_t len);
static inline tu_fifo_t* tud_audio_get_tx_support_ff        (uint8_t ff_idx);
#endif

// INT CTR API

#if CFG_TUD_AUDIO_ENABLE_INTERRUPT_EP
static inline bool tud_audio_int_write                      (const audio_interrupt_data_t * data);
#endif

// Buffer control EP data and schedule a transmit
// This function is intended to be used if you do not have a persistent buffer or memory location available (e.g. non-local variables) and need to answer onto a
// get request. This function buffers your answer request frame into the control buffer of the corresponding audio driver and schedules a transmit for sending it.
// Since transmission is triggered via interrupts, a persistent memory location is required onto which the buffer pointer in pointing. If you already have such
// available you may directly use 'tud_control_xfer(...)'. In this case data does not need to be copied into an additional buffer and you save some time.
// If the request's wLength is zero, a status packet is sent instead.

/**
 * @brief Buffer control endpoint data and schedule a transmit
 * 
 * @details This function is intended to be used when responding to control requests
 * without a persistent buffer. It copies the provided data into the control buffer
 * of the corresponding audio driver and schedules a transmit to send the response.
 * 
 * Since transmission is triggered via interrupts, a persistent memory location is
 * required for the buffer pointer. If you already have such a persistent buffer
 * available, you may directly use 'tud_control_xfer(...)' instead. This avoids
 * the additional data copy into the internal buffer and saves processing time.
 * 
 * This function handles only control requests with IN direction (device to host).
 * If the request's wLength is zero, a status packet is sent instead of data.
 * 
 * @param[in] rhport        USB port number
 * @param[in] p_request     Pointer to the setup request
 * @param[in] data          Pointer to the response data to be sent to host
 * @param[in] len           Length of the response data in bytes
 * 
 * @return true             If the operation was successful
 * @return false            If the operation failed (invalid parameters or OUT direction)
 */
bool tud_audio_buffer_and_schedule_control_xfer(uint8_t rhport, tusb_control_request_t const * p_request, void* data, uint16_t len);

//--------------------------------------------------------------------+
// Application Callback API
//--------------------------------------------------------------------+

#if CFG_TUD_AUDIO_ENABLE_EP_IN
bool tud_audio_tx_done_pre_load_cb(uint8_t rhport, uint8_t func_id, uint8_t ep_in, uint8_t cur_alt_setting);
bool tud_audio_tx_done_post_load_cb(uint8_t rhport, uint16_t n_bytes_copied, uint8_t func_id, uint8_t ep_in, uint8_t cur_alt_setting);
#endif

#if CFG_TUD_AUDIO_ENABLE_EP_OUT
bool tud_audio_rx_done_pre_read_cb(uint8_t rhport, uint16_t n_bytes_received, uint8_t func_id, uint8_t ep_out, uint8_t cur_alt_setting);
bool tud_audio_rx_done_post_read_cb(uint8_t rhport, uint16_t n_bytes_received, uint8_t func_id, uint8_t ep_out, uint8_t cur_alt_setting);
#endif

#if CFG_TUD_AUDIO_ENABLE_EP_OUT && CFG_TUD_AUDIO_ENABLE_FEEDBACK_EP
void tud_audio_fb_done_cb(uint8_t func_id);


// Note about feedback calculation
//
// Option 1 - AUDIO_FEEDBACK_METHOD_FIFO_COUNT
// Feedback value is calculated within the audio driver by regulating the FIFO level to half fill.
// Advantage: No ISR interrupt is enabled, hence the CPU need not to handle an ISR every 1ms or 125us and thus less CPU load, well tested
// (Windows, Linux, OSX) with a reliable result so far.
// Disadvantage: A FIFO of minimal 4 frames is needed to compensate for jitter, an average delay of 2 frames is introduced.
//
// Option 2 - AUDIO_FEEDBACK_METHOD_FREQUENCY_FIXED / AUDIO_FEEDBACK_METHOD_FREQUENCY_FLOAT
// Feedback value is calculated within the audio driver by use of SOF interrupt. The driver needs information about the master clock f_m from
// which the audio sample frequency f_s is derived, f_s itself, and the cycle count of f_m at time of the SOF interrupt (e.g. by use of a hardware counter).
// See tud_audio_set_fb_params() and tud_audio_feedback_update()
// Advantage: Reduced jitter in the feedback value computation, hence, the receive FIFO can be smaller and thus a smaller delay is possible.
// Disadvantage: higher CPU load due to SOF ISR handling every frame i.e. 1ms or 125us. (The most critical point is the reading of the cycle counter value of f_m.
// It is read from within the SOF ISR - see: audiod_sof() -, hence, the ISR must has a high priority such that no software dependent "random" delay i.e. jitter is introduced).
// Long-term drift could occur since error is accumulated.
//
// Option 3 - manual
// Determined by the user itself and set by use of tud_audio_n_fb_set(). The feedback value may be determined e.g. from some fill status of some FIFO buffer.
// Advantage: No ISR interrupt is enabled, hence the CPU need not to handle an ISR every 1ms or 125us and thus less CPU load.
// Disadvantage: typically a larger FIFO is needed to compensate for jitter (e.g. 6 frames), i.e. a larger delay is introduced.


// This function is used to provide data rate feedback from an asynchronous sink. Feedback value will be sent at FB endpoint interval till it's changed.
//
// The feedback format is specified to be 16.16 for HS and 10.14 for FS devices (see Universal Serial Bus Specification Revision 2.0 5.12.4.2). By default,
// the choice of format is left to the caller and feedback argument is sent as-is. If CFG_TUD_AUDIO_ENABLE_FEEDBACK_FORMAT_CORRECTION is set or tud_audio_feedback_format_correction_cb()
// return true, then tinyusb expects 16.16 format and handles the conversion to 10.14 on FS.
//
// Note that due to a bug in its USB Audio 2.0 driver, Windows currently requires 16.16 format for _all_ USB 2.0 devices. On Linux and it seems the
// driver can work with either format.
//
// Feedback value can be determined from within the SOF ISR of the audio driver. This should reduce jitter. If the feature is used, the user can not set the feedback value.
//
// Determine feedback value - The feedback method is described in 5.12.4.2 of the USB 2.0 spec
// Boiled down, the feedback value Ff = n_samples / (micro)frame.
// Since an accuracy of less than 1 Sample / second is desired, at least n_frames = ceil(2^K * f_s / f_m) frames need to be measured, where K = 10 for full speed and K = 13
// for high speed, f_s is the sampling frequency e.g. 48 kHz and f_m is the cpu clock frequency e.g. 100 MHz (or any other master clock whose clock count is available and locked to f_s)
// The update interval in the (4.10.2.1) Feedback Endpoint Descriptor must be less or equal to 2^(K - P), where P = min( ceil(log2(f_m / f_s)), K)
// feedback = n_cycles / n_frames * f_s / f_m in 16.16 format, where n_cycles are the number of main clock cycles within fb_n_frames
bool tud_audio_n_fb_set(uint8_t func_id, uint32_t feedback);

// Update feedback value with passed MCLK cycles since last time this update function is called.
// Typically called within tud_audio_sof_isr(). Required tud_audio_feedback_params_cb() is implemented
// This function will also call tud_audio_feedback_set()
// return feedback value in 16.16 for reference (0 for error)
// Example :
//   binterval=3 (4ms); FS = 48kHz; MCLK = 12.288MHz
//   In 4 SOF MCLK counted 49152 cycles
uint32_t tud_audio_feedback_update(uint8_t func_id, uint32_t cycles);

enum {
  AUDIO_FEEDBACK_METHOD_DISABLED,
  AUDIO_FEEDBACK_METHOD_FREQUENCY_FIXED,
  AUDIO_FEEDBACK_METHOD_FREQUENCY_FLOAT,
  AUDIO_FEEDBACK_METHOD_FREQUENCY_POWER_OF_2, // For driver internal use only
  AUDIO_FEEDBACK_METHOD_FIFO_COUNT
};

typedef struct {
  uint8_t method;
  uint32_t sample_freq;   //  sample frequency in Hz

  union {
    struct {
      uint32_t mclk_freq; // Main clock frequency in Hz i.e. master clock to which sample clock is based on
    }frequency;

  };
}audio_feedback_params_t;

// Invoked when needed to set feedback parameters
void tud_audio_feedback_params_cb(uint8_t func_id, uint8_t alt_itf, audio_feedback_params_t* feedback_param);

// Callback in ISR context, invoked periodically according to feedback endpoint bInterval.
// Could be used to compute and update feedback value, should be placed in RAM if possible
// frame_number  : current SOF count
// interval_shift: number of bit shift i.e log2(interval) from Feedback endpoint descriptor
TU_ATTR_FAST_FUNC void tud_audio_feedback_interval_isr(uint8_t func_id, uint32_t frame_number, uint8_t interval_shift);

// (Full-Speed only) Callback to set feedback format correction is applied or not,
// default to CFG_TUD_AUDIO_ENABLE_FEEDBACK_FORMAT_CORRECTION if not implemented.
bool tud_audio_feedback_format_correction_cb(uint8_t func_id);
#endif // CFG_TUD_AUDIO_ENABLE_EP_OUT && CFG_TUD_AUDIO_ENABLE_FEEDBACK_EP

#if CFG_TUD_AUDIO_ENABLE_INTERRUPT_EP
void tud_audio_int_done_cb(uint8_t rhport);
#endif

/**
 * @brief Callback when audio set interface request is received
 * 
 * @details This callback is invoked when the host sends a Set Interface request
 * to an audio interface. It is commonly used to handle audio streaming format changes
 * or to start/stop audio streaming. The alternate setting value indicates the 
 * specific format to be used, or 0 for no streaming.
 * 
 * @param[in] rhport      USB port number
 * @param[in] p_request   Pointer to the setup request containing interface and alternate setting
 * 
 * @return true           If the request is accepted
 * @return false          If the request is rejected, will cause a STALL response
 */
bool tud_audio_set_itf_cb(uint8_t rhport, tusb_control_request_t const * p_request);

/**
 * @brief Callback when audio set interface request is received that closes an endpoint
 * 
 * @details This callback is invoked when the host sends a Set Interface request
 * that would close an audio endpoint, typically when switching to alternate setting 0
 * (no streaming). This gives the application a chance to perform any necessary cleanup
 * or state changes when audio streaming is stopped.
 * 
 * @param[in] rhport      USB port number
 * @param[in] p_request   Pointer to the setup request containing interface and alternate setting
 * 
 * @return true           If the request is accepted
 * @return false          If the request is rejected, will cause a STALL response
 */
bool tud_audio_set_itf_close_EP_cb(uint8_t rhport, tusb_control_request_t const * p_request);

/**
 * @brief Callback when audio class specific set request is received for an endpoint
 * 
 * @details This callback is invoked when the host sends an audio class-specific
 * control request targeted at an endpoint. These requests are used to configure
 * various endpoint-specific audio parameters such as:
 * - Sampling frequency
 * - Pitch control
 * - Data overrun/underrun control
 * 
 * @param[in] rhport      USB port number
 * @param[in] p_request   Pointer to the setup request
 * @param[in,out] pBuff   Buffer containing data for SET requests or to be filled for GET requests
 * 
 * @return true           If the request is handled successfully
 * @return false          If the request is rejected, will cause a STALL response
 */
bool tud_audio_set_req_ep_cb(uint8_t rhport, tusb_control_request_t const * p_request, uint8_t *pBuff);

/**
 * @brief Callback when audio class specific set request is received for an interface
 * 
 * @details This callback is invoked when the host sends an audio class-specific
 * control request targeted at an interface. These requests are used to configure
 * various interface-specific audio parameters such as:
 * - Volume control
 * - Mute control
 * - Tone control (bass, mid, treble)
 * - Graphic equalizer
 * - Automatic gain control
 * - Delay control
 * - Bass boost
 * 
 * @param[in] rhport      USB port number
 * @param[in] p_request   Pointer to the setup request
 * @param[in,out] pBuff   Buffer containing data for SET requests or to be filled for GET requests
 * 
 * @return true           If the request is handled successfully
 * @return false          If the request is rejected, will cause a STALL response
 */
bool tud_audio_set_req_itf_cb(uint8_t rhport, tusb_control_request_t const * p_request, uint8_t *pBuff);

/**
 * @brief Callback when audio class specific set request is received for an entity
 * 
 * @details This callback is invoked when the host sends an audio class-specific
 * control request targeted at an audio entity. Audio entities include feature units,
 * mixers, selectors, clock sources, and other audio processing blocks defined in the
 * USB Audio Class specification.
 * 
 * @param[in] rhport      USB port number
 * @param[in] p_request   Pointer to the setup request
 * @param[in,out] pBuff   Buffer containing data for SET requests
 * 
 * @return true           If the request is handled successfully
 * @return false          If the request is rejected, will cause a STALL response
 */
bool tud_audio_set_req_entity_cb(uint8_t rhport, tusb_control_request_t const * p_request, uint8_t *pBuff);

/**
 * @brief Callback when audio class specific get request is received for an endpoint
 * 
 * @details This callback is invoked when the host sends an audio class-specific
 * get request targeted at an endpoint. These requests are used to query
 * various endpoint-specific audio parameters such as:
 * - Sampling frequency
 * - Pitch control
 * - Data overrun/underrun control
 * 
 * @param[in] rhport      USB port number
 * @param[in] p_request   Pointer to the setup request
 * 
 * @return true           If the request is handled successfully
 * @return false          If the request is rejected, will cause a STALL response
 */
bool tud_audio_get_req_ep_cb(uint8_t rhport, tusb_control_request_t const * p_request);

/**
 * @brief Callback when audio class specific get request is received for an interface
 * 
 * @details This callback is invoked when the host sends an audio class-specific
 * get request targeted at an interface. These requests are used to query
 * various interface-specific audio parameters such as:
 * - Volume control
 * - Mute control
 * - Tone control (bass, mid, treble)
 * - Graphic equalizer
 * - Automatic gain control
 * - Delay control
 * - Bass boost
 * 
 * @param[in] rhport      USB port number
 * @param[in] p_request   Pointer to the setup request
 * 
 * @return true           If the request is handled successfully
 * @return false          If the request is rejected, will cause a STALL response
 */
bool tud_audio_get_req_itf_cb(uint8_t rhport, tusb_control_request_t const * p_request);

/**
 * @brief Callback when audio class specific get request is received for an entity
 * 
 * @details This callback is invoked when the host sends an audio class-specific
 * get request targeted at an audio entity. Audio entities include feature units,
 * mixers, selectors, clock sources, and other audio processing blocks defined in the
 * USB Audio Class specification.
 * 
 * @param[in] rhport      USB port number
 * @param[in] p_request   Pointer to the setup request
 * 
 * @return true           If the request is handled successfully
 * @return false          If the request is rejected, will cause a STALL response
 */
bool tud_audio_get_req_entity_cb(uint8_t rhport, tusb_control_request_t const * p_request);

//--------------------------------------------------------------------+
// Inline Functions
//--------------------------------------------------------------------+

/**
 * @brief Check if the default audio function is mounted by the host
 * 
 * @details This function checks if the first audio function (function ID 0) has been 
 * configured and mounted by the USB host. An audio function is considered 
 * mounted when the host has completed device configuration and the interface 
 * is ready for use.
 * 
 * This is a convenience wrapper around tud_audio_n_mounted(0).
 * 
 * The mounted state indicates that the device is properly enumerated and 
 * the audio interface is ready to transfer data. Applications should check
 * this before attempting to use audio functions.
 * 
 * @return true    If the default audio function is mounted and ready for use
 * @return false   If the default audio function is not mounted
 */
static inline bool tud_audio_mounted(void)
{
  return tud_audio_n_mounted(0);
}

// RX API

#if CFG_TUD_AUDIO_ENABLE_EP_OUT && !CFG_TUD_AUDIO_ENABLE_DECODING

static inline uint16_t tud_audio_available(void)
{
  return tud_audio_n_available(0);
}

static inline uint16_t tud_audio_read(void* buffer, uint16_t bufsize)
{
  return tud_audio_n_read(0, buffer, bufsize);
}

static inline bool tud_audio_clear_ep_out_ff(void)
{
  return tud_audio_n_clear_ep_out_ff(0);
}

static inline tu_fifo_t* tud_audio_get_ep_out_ff(void)
{
  return tud_audio_n_get_ep_out_ff(0);
}

#endif

#if CFG_TUD_AUDIO_ENABLE_EP_OUT && CFG_TUD_AUDIO_ENABLE_DECODING

static inline bool tud_audio_clear_rx_support_ff(uint8_t ff_idx)
{
  return tud_audio_n_clear_rx_support_ff(0, ff_idx);
}

static inline uint16_t tud_audio_available_support_ff(uint8_t ff_idx)
{
  return tud_audio_n_available_support_ff(0, ff_idx);
}

static inline uint16_t tud_audio_read_support_ff(uint8_t ff_idx, void* buffer, uint16_t bufsize)
{
  return tud_audio_n_read_support_ff(0, ff_idx, buffer, bufsize);
}

static inline tu_fifo_t* tud_audio_get_rx_support_ff(uint8_t ff_idx)
{
  return tud_audio_n_get_rx_support_ff(0, ff_idx);
}

#endif

// TX API

#if CFG_TUD_AUDIO_ENABLE_EP_IN && !CFG_TUD_AUDIO_ENABLE_ENCODING

static inline uint16_t tud_audio_write(const void * data, uint16_t len)
{
  return tud_audio_n_write(0, data, len);
}

static inline bool tud_audio_clear_ep_in_ff(void)
{
  return tud_audio_n_clear_ep_in_ff(0);
}

static inline tu_fifo_t* tud_audio_get_ep_in_ff(void)
{
  return tud_audio_n_get_ep_in_ff(0);
}

#endif

#if CFG_TUD_AUDIO_ENABLE_EP_IN && CFG_TUD_AUDIO_ENABLE_ENCODING

static inline uint16_t tud_audio_flush_tx_support_ff(void)
{
  return tud_audio_n_flush_tx_support_ff(0);
}

static inline uint16_t tud_audio_clear_tx_support_ff(uint8_t ff_idx)
{
  return tud_audio_n_clear_tx_support_ff(0, ff_idx);
}

static inline uint16_t tud_audio_write_support_ff(uint8_t ff_idx, const void * data, uint16_t len)
{
  return tud_audio_n_write_support_ff(0, ff_idx, data, len);
}

static inline tu_fifo_t* tud_audio_get_tx_support_ff(uint8_t ff_idx)
{
  return tud_audio_n_get_tx_support_ff(0, ff_idx);
}

#endif

#if CFG_TUD_AUDIO_ENABLE_INTERRUPT_EP
static inline bool tud_audio_int_write(const audio_interrupt_data_t * data)
{
  return tud_audio_int_n_write(0, data);
}
#endif

#if CFG_TUD_AUDIO_ENABLE_EP_OUT && CFG_TUD_AUDIO_ENABLE_FEEDBACK_EP

static inline bool tud_audio_fb_set(uint32_t feedback)
{
  return tud_audio_n_fb_set(0, feedback);
}

#endif

//--------------------------------------------------------------------+
// Internal Class Driver API
//--------------------------------------------------------------------+

/**
 * @brief Initialize the Audio Device driver
 * 
 * @details This function initializes the internal state of the Audio Device driver.
 * It is called during the USB device stack initialization and performs the following tasks:
 * 
 * - Clears the internal audio function structures
 * - Sets up control buffers for each audio function
 * - Initializes alternate interface settings
 * - Configures FIFOs for audio data transfer (based on compile-time configuration)
 * - Sets up any required linear buffers
 */
void     audiod_init           (void);

/**
 * @brief De-initialize the Audio Device driver
 * 
 * @details This function is intended to clean up resources used by the Audio Device driver
 * when the USB device is disconnected or when the audio functions need to be reset.
 * 
 * When properly implemented, this function would be expected to:
 * - Free any dynamically allocated resources
 * - Close any open audio endpoints
 * - Reset internal state variables
 * - Potentially notify the application layer that audio is no longer available
 * 
 * @return bool  Currently always returns false indicating the operation is not implemented
 */
bool     audiod_deinit         (void);

/**
 * @brief Reset the Audio Device driver state
 * 
 * @details This function resets the internal state of the Audio Device driver
 * when a USB bus reset occurs or when the device needs to return to a known state.
 * It performs the following operations:
 * 
 * - Clears the audio function structures (up to ITF_MEM_RESET_SIZE bytes)
 * - Resets all FIFOs associated with audio endpoints
 * - Clears transmit and receive support FIFOs if enabled
 * 
 * @param[in] rhport  USB port number (currently unused in the function)
 */
void     audiod_reset          (uint8_t rhport);

/**
 * @brief Initialize an audio function when its interface descriptor is processed
 * 
 * @details This function is called by the USB stack when it encounters an Audio Class
 * Control Interface descriptor during device enumeration. It initializes the audio
 * function with the appropriate settings from the descriptor.
 * 
 * The function performs the following operations:
 * - Verifies the interface is an Audio Class Control Interface
 * - Verifies protocol version and alternate setting
 * - Finds an available audio driver interface slot
 * - Stores descriptor information and configures function parameters
 * - Allocates and configures endpoints if necessary
 * - Sets up audio streaming interfaces and their alternate settings
 * - Initializes FIFOs for audio data transfer
 * 
 * @param[in] rhport      USB port number
 * @param[in] itf_desc    Pointer to the interface descriptor being processed
 * @param[in] max_len     Maximum length of the descriptor in bytes
 * 
 * @return uint16_t       The number of bytes processed from the descriptor,
 *                        or 0 if an error occurred during processing
 */
uint16_t audiod_open           (uint8_t rhport, tusb_desc_interface_t const * itf_desc, uint16_t max_len);

/**
 * @brief Callback for handling control transfers to the Audio Class interface
 * 
 * @details This function is called by the USB device stack during control transfers
 * targeted at the Audio Class interface. It handles both the SETUP and DATA stages
 * of control transfers.
 * 
 * The function operates as follows:
 * - For CONTROL_STAGE_SETUP: Calls audiod_control_request() to process the setup packet
 * - For CONTROL_STAGE_DATA: Calls audiod_control_complete() to finalize the request
 * - For other stages (ACK): Returns true to indicate successful completion
 * 
 * This callback is registered with the USB device stack and is invoked automatically
 * when control transfers are directed to the Audio Class interface.
 * 
 * @param[in] rhport    USB port number
 * @param[in] stage     Current stage of the control transfer (SETUP, DATA, or ACK)
 * @param[in] request   Pointer to the control request structure
 * 
 * @return true         If the stage was processed successfully
 * @return false        If an error occurred during processing
 */
bool     audiod_control_xfer_cb(uint8_t rhport, uint8_t stage, tusb_control_request_t const * request);

/**
 * @brief Callback for handling audio data transfers
 * 
 * @details This function is called by the USB device stack when a data transfer
 * completes on an audio endpoint. It handles transfers for both IN (device to host)
 * and OUT (host to device) endpoints, as well as interrupt endpoints.
 * 
 * The function operates as follows:
 * - Searches for the audio function associated with the endpoint address
 * - For interrupt endpoints: Calls tud_audio_int_done_cb() to notify the application
 * - For IN endpoints (device to host):
 *   - Handles isochronous transfers according to USB 2.0 specification
 *   - Prepares the next packet for transmission
 *   - Calls tud_audio_tx_done_pre_load_cb() to allow application processing
 *   - Handles audio encoding if enabled
 * - For OUT endpoints (host to device):
 *   - Processes received audio data
 *   - Calls tud_audio_rx_done_pre_read_cb() to allow application processing
 *   - Handles audio decoding if enabled
 *   - Prepares for the next reception
 * 
 * @param[in] rhport          USB port number
 * @param[in] edpt_addr       Endpoint address
 * @param[in] result          Result of the transfer (successful or failed)
 * @param[in] xferred_bytes   Number of bytes transferred
 * 
 * @return true               If the callback was handled successfully
 * @return false              If an error occurred during processing
 */
bool     audiod_xfer_cb        (uint8_t rhport, uint8_t edpt_addr, xfer_result_t result, uint32_t xferred_bytes);

/**
 * @brief Process Start of Frame (SOF) interrupt for audio feedback
 * 
 * @details This function is called by the USB device stack when a Start of Frame (SOF)
 * interrupt is received. It is primarily used to calculate and update feedback values
 * for isochronous audio endpoints that implement adaptive synchronization.
 * 
 * When both OUT endpoints and feedback endpoints are enabled, this function:
 * - Iterates through all configured audio functions
 * - For each function with a feedback endpoint configured (audio->ep_fb != 0):
 *   - Calculates the appropriate interval for feedback updates based on speed
 *   - Calls tud_audio_feedback_interval_isr() at the calculated intervals
 * 
 * The feedback mechanism is described in USB 2.0 specification section 5.12.4.2,
 * allowing the device to communicate its actual sampling rate to the host for
 * proper synchronization.
 * 
 * @param[in] rhport        USB port number
 * @param[in] frame_count   Current USB frame number
 */
void     audiod_sof_isr        (uint8_t rhport, uint32_t frame_count);

#ifdef __cplusplus
}
#endif

#endif /* _TUSB_AUDIO_DEVICE_H_ */

/** @}*/ /* End of AUDIO_DEVICE */