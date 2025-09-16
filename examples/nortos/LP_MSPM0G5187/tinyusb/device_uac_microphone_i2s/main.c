/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2020 Reinhard Panhuber
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
 */

/* plot_audio_samples.py requires following modules:
 * $ sudo apt install libportaudio
 * $ pip3 install sounddevice matplotlib
 *
 * Then run
 * $ python3 plot_audio_samples.py
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "bsp/board_api.h"
#include "ti_msp_dl_config.h"
#include "tusb.h"

#define FILTERED_I2S_SAMPLE_SIZE (48)

#define RAW_I2S_SAMPLE_SIZE (FILTERED_I2S_SAMPLE_SIZE * 2)

/* Audio data buffer */
int32_t audioBuffer[FILTERED_I2S_SAMPLE_SIZE];
//--------------------------------------------------------------------+
// MACRO CONSTANT TYPEDEF PROTYPES
//--------------------------------------------------------------------+

/* Blink pattern
 * - 250 ms  : device not mounted
 * - 1000 ms : device mounted
 * - 2500 ms : device is suspended
 */
enum {
    BLINK_NOT_MOUNTED = 250,
    BLINK_MOUNTED     = 1000,
    BLINK_SUSPENDED   = 2500,
};

static uint32_t blink_interval_ms = BLINK_NOT_MOUNTED;

// Audio controls
// Current states
bool mute[CFG_TUD_AUDIO_FUNC_1_N_CHANNELS_TX + 1];  // +1 for master channel 0
uint16_t
    volume[CFG_TUD_AUDIO_FUNC_1_N_CHANNELS_TX + 1];  // +1 for master channel 0
uint32_t sampFreq;
uint8_t clkValid;

// Range states
audio_control_range_2_n_t(1)
    volumeRng[CFG_TUD_AUDIO_FUNC_1_N_CHANNELS_TX + 1];  // Volume range state
audio_control_range_4_n_t(1) sampleFreqRng;  // Sample frequency range state

// Audio test data
int32_t test_buffer_audio[(CFG_TUD_AUDIO_EP_SZ_IN - 2) / 2];
uint16_t startVal = 0;

void led_blinking_task(void);

__STATIC_INLINE void DL_GPIO_inena_Function(
    uint32_t pincmIndex, uint32_t inena_function)
{
    IOMUX->SECCFG.PINCM[pincmIndex] |=
        (inena_function | IOMUX_PINCM_PIPD_ENABLE);
}

/*------------- MAIN -------------*/
int main(void)
{
    board_init();

    // init device stack on configured roothub port
    tusb_rhport_init_t dev_init = {
        .role = TUSB_ROLE_DEVICE, .speed = TUSB_SPEED_AUTO};
    tusb_init(BOARD_TUD_RHPORT, &dev_init);

    if (board_init_after_tusb) {
        board_init_after_tusb();
    }

    // Init values
    sampFreq = CFG_TUD_AUDIO_FUNC_1_SAMPLE_RATE;
    clkValid = 1;

    sampleFreqRng.wNumSubRanges    = 1;
    sampleFreqRng.subrange[0].bMin = CFG_TUD_AUDIO_FUNC_1_SAMPLE_RATE;
    sampleFreqRng.subrange[0].bMax = CFG_TUD_AUDIO_FUNC_1_SAMPLE_RATE;
    sampleFreqRng.subrange[0].bRes = 0;

    DL_I2S_disable(I2S_0_INST);
    DL_GPIO_inena_Function(GPIO_I2S_0_IOMUX_BCLK, IOMUX_PINCM_INENA_ENABLE);
    DL_GPIO_inena_Function(GPIO_I2S_0_IOMUX_WCLK, IOMUX_PINCM_INENA_ENABLE);
    DL_I2S_enable(I2S_0_INST);

    /* Configuring src address for DMA0 */
    DL_DMA_setSrcAddr(DMA, DMA_CH0_CHAN_ID, (uint32_t)(&I2S_0_INST->RXDATA));

    /* Configuring dst address for DMA0 */
    DL_DMA_setDestAddr(DMA, DMA_CH0_CHAN_ID, (uint32_t) &audioBuffer[0]);

    /* Configuring transfer size of DMA0 */
    DL_DMA_setTransferSize(DMA, DMA_CH0_CHAN_ID, FILTERED_I2S_SAMPLE_SIZE);

    /* Enable the DMA Channel 0 */
    DL_DMA_enableChannel(DMA, DMA_CH0_CHAN_ID);

    while (1) {
        tud_task();  // tinyusb device task
        led_blinking_task();
    }
}

//--------------------------------------------------------------------+
// Device callbacks
//--------------------------------------------------------------------+

// Invoked when device is mounted
void tud_mount_cb(void)
{
    blink_interval_ms = BLINK_MOUNTED;
}

// Invoked when device is unmounted
void tud_umount_cb(void)
{
    blink_interval_ms = BLINK_NOT_MOUNTED;
}

// Invoked when usb bus is suspended
// remote_wakeup_en : if host allow us  to perform remote wakeup
// Within 7ms, device must draw an average of current less than 2.5 mA from bus
void tud_suspend_cb(bool remote_wakeup_en)
{
    (void) remote_wakeup_en;
    blink_interval_ms = BLINK_SUSPENDED;
}

// Invoked when usb bus is resumed
void tud_resume_cb(void)
{
    blink_interval_ms = tud_mounted() ? BLINK_MOUNTED : BLINK_NOT_MOUNTED;
}

//--------------------------------------------------------------------+
// Application Callback API Implementations
//--------------------------------------------------------------------+

// Invoked when audio class specific set request received for an EP
bool tud_audio_set_req_ep_cb(
    uint8_t rhport, tusb_control_request_t const *p_request, uint8_t *pBuff)
{
    (void) rhport;
    (void) pBuff;

    // We do not support any set range requests here, only current value requests
    TU_VERIFY(p_request->bRequest == AUDIO_CS_REQ_CUR);

    // Page 91 in UAC2 specification
    uint8_t channelNum = TU_U16_LOW(p_request->wValue);
    uint8_t ctrlSel    = TU_U16_HIGH(p_request->wValue);
    uint8_t ep         = TU_U16_LOW(p_request->wIndex);

    (void) channelNum;
    (void) ctrlSel;
    (void) ep;

    return false;  // Yet not implemented
}

// Invoked when audio class specific set request received for an interface
bool tud_audio_set_req_itf_cb(
    uint8_t rhport, tusb_control_request_t const *p_request, uint8_t *pBuff)
{
    (void) rhport;
    (void) pBuff;

    // We do not support any set range requests here, only current value requests
    TU_VERIFY(p_request->bRequest == AUDIO_CS_REQ_CUR);

    // Page 91 in UAC2 specification
    uint8_t channelNum = TU_U16_LOW(p_request->wValue);
    uint8_t ctrlSel    = TU_U16_HIGH(p_request->wValue);
    uint8_t itf        = TU_U16_LOW(p_request->wIndex);

    (void) channelNum;
    (void) ctrlSel;
    (void) itf;

    return false;  // Yet not implemented
}

// Invoked when audio class specific set request received for an entity
bool tud_audio_set_req_entity_cb(
    uint8_t rhport, tusb_control_request_t const *p_request, uint8_t *pBuff)
{
    (void) rhport;

    // Page 91 in UAC2 specification
    uint8_t channelNum = TU_U16_LOW(p_request->wValue);
    uint8_t ctrlSel    = TU_U16_HIGH(p_request->wValue);
    uint8_t itf        = TU_U16_LOW(p_request->wIndex);
    uint8_t entityID   = TU_U16_HIGH(p_request->wIndex);

    (void) itf;

    // We do not support any set range requests here, only current value requests
    TU_VERIFY(p_request->bRequest == AUDIO_CS_REQ_CUR);

    // If request is for our feature unit
    if (entityID == 2) {
        switch (ctrlSel) {
            case AUDIO_FU_CTRL_MUTE:
                // Request uses format layout 1
                TU_VERIFY(p_request->wLength == sizeof(audio_control_cur_1_t));

                mute[channelNum] = ((audio_control_cur_1_t *) pBuff)->bCur;

                TU_LOG2("    Set Mute: %d of channel: %u\r\n",
                    mute[channelNum], channelNum);
                return true;

            case AUDIO_FU_CTRL_VOLUME:
                // Request uses format layout 2
                TU_VERIFY(p_request->wLength == sizeof(audio_control_cur_2_t));

                volume[channelNum] =
                    (uint16_t)((audio_control_cur_2_t *) pBuff)->bCur;

                TU_LOG2("    Set Volume: %d dB of channel: %u\r\n",
                    volume[channelNum], channelNum);
                return true;

                // Unknown/Unsupported control
            default:
                TU_BREAKPOINT();
                return false;
        }
    }
    return false;  // Yet not implemented
}

// Invoked when audio class specific get request received for an EP
bool tud_audio_get_req_ep_cb(
    uint8_t rhport, tusb_control_request_t const *p_request)
{
    (void) rhport;

    // Page 91 in UAC2 specification
    uint8_t channelNum = TU_U16_LOW(p_request->wValue);
    uint8_t ctrlSel    = TU_U16_HIGH(p_request->wValue);
    uint8_t ep         = TU_U16_LOW(p_request->wIndex);

    (void) channelNum;
    (void) ctrlSel;
    (void) ep;

    //    return tud_control_xfer(rhport, p_request, &tmp, 1);

    return false;  // Yet not implemented
}

// Invoked when audio class specific get request received for an interface
bool tud_audio_get_req_itf_cb(
    uint8_t rhport, tusb_control_request_t const *p_request)
{
    (void) rhport;

    // Page 91 in UAC2 specification
    uint8_t channelNum = TU_U16_LOW(p_request->wValue);
    uint8_t ctrlSel    = TU_U16_HIGH(p_request->wValue);
    uint8_t itf        = TU_U16_LOW(p_request->wIndex);

    (void) channelNum;
    (void) ctrlSel;
    (void) itf;

    return false;  // Yet not implemented
}

// Invoked when audio class specific get request received for an entity
bool tud_audio_get_req_entity_cb(
    uint8_t rhport, tusb_control_request_t const *p_request)
{
    (void) rhport;

    // Page 91 in UAC2 specification
    uint8_t channelNum = TU_U16_LOW(p_request->wValue);
    uint8_t ctrlSel    = TU_U16_HIGH(p_request->wValue);
    // uint8_t itf = TU_U16_LOW(p_request->wIndex);           // Since we have only one audio function implemented, we do not need the itf value
    uint8_t entityID = TU_U16_HIGH(p_request->wIndex);

    // Input terminal (Microphone input)
    if (entityID == 1) {
        switch (ctrlSel) {
            case AUDIO_TE_CTRL_CONNECTOR: {
                // The terminal connector control only has a get request with only the CUR attribute.
                audio_desc_channel_cluster_t ret;

                // Those are dummy values for now
                ret.bNrChannels     = 1;
                ret.bmChannelConfig = (audio_channel_config_t) 0;
                ret.iChannelNames   = 0;

                TU_LOG2("    Get terminal connector\r\n");

                return tud_audio_buffer_and_schedule_control_xfer(
                    rhport, p_request, (void *) &ret, sizeof(ret));
            } break;

                // Unknown/Unsupported control selector
            default:
                TU_BREAKPOINT();
                return false;
        }
    }

    // Feature unit
    if (entityID == 2) {
        switch (ctrlSel) {
            case AUDIO_FU_CTRL_MUTE:
                // Audio control mute cur parameter block consists of only one byte - we thus can send it right away
                // There does not exist a range parameter block for mute
                TU_LOG2("    Get Mute of channel: %u\r\n", channelNum);
                return tud_control_xfer(
                    rhport, p_request, &mute[channelNum], 1);

            case AUDIO_FU_CTRL_VOLUME:
                switch (p_request->bRequest) {
                    case AUDIO_CS_REQ_CUR:
                        TU_LOG2(
                            "    Get Volume of channel: %u\r\n", channelNum);
                        return tud_control_xfer(rhport, p_request,
                            &volume[channelNum], sizeof(volume[channelNum]));

                    case AUDIO_CS_REQ_RANGE:
                        TU_LOG2("    Get Volume range of channel: %u\r\n",
                            channelNum);

                        // Copy values - only for testing - better is version below
                        audio_control_range_2_n_t(1) ret;

                        ret.wNumSubRanges    = 1;
                        ret.subrange[0].bMin = -90;  // -90 dB
                        ret.subrange[0].bMax = 90;   // +90 dB
                        ret.subrange[0].bRes = 1;    // 1 dB steps

                        return tud_audio_buffer_and_schedule_control_xfer(
                            rhport, p_request, (void *) &ret, sizeof(ret));

                        // Unknown/Unsupported control
                    default:
                        TU_BREAKPOINT();
                        return false;
                }
                break;

                // Unknown/Unsupported control
            default:
                TU_BREAKPOINT();
                return false;
        }
    }

    // Clock Source unit
    if (entityID == 4) {
        switch (ctrlSel) {
            case AUDIO_CS_CTRL_SAM_FREQ:
                // channelNum is always zero in this case
                switch (p_request->bRequest) {
                    case AUDIO_CS_REQ_CUR:
                        TU_LOG2("    Get Sample Freq.\r\n");
                        return tud_control_xfer(
                            rhport, p_request, &sampFreq, sizeof(sampFreq));

                    case AUDIO_CS_REQ_RANGE:
                        TU_LOG2("    Get Sample Freq. range\r\n");
                        return tud_control_xfer(rhport, p_request,
                            &sampleFreqRng, sizeof(sampleFreqRng));

                        // Unknown/Unsupported control
                    default:
                        TU_BREAKPOINT();
                        return false;
                }
                break;

            case AUDIO_CS_CTRL_CLK_VALID:
                // Only cur attribute exists for this request
                TU_LOG2("    Get Sample Freq. valid\r\n");
                return tud_control_xfer(
                    rhport, p_request, &clkValid, sizeof(clkValid));

            // Unknown/Unsupported control
            default:
                TU_BREAKPOINT();
                return false;
        }
    }

    TU_LOG2("  Unsupported entity: %d\r\n", entityID);
    return false;  // Yet not implemented
}

bool tud_audio_tx_done_pre_load_cb(
    uint8_t rhport, uint8_t itf, uint8_t ep_in, uint8_t cur_alt_setting)
{
    (void) rhport;
    (void) itf;
    (void) ep_in;
    (void) cur_alt_setting;

    tud_audio_write((int32_t *) test_buffer_audio, CFG_TUD_AUDIO_EP_SZ_IN - 2);

    return true;
}

bool tud_audio_tx_done_post_load_cb(uint8_t rhport, uint16_t n_bytes_copied,
    uint8_t itf, uint8_t ep_in, uint8_t cur_alt_setting)
{
    (void) rhport;
    (void) n_bytes_copied;
    (void) itf;
    (void) ep_in;
    (void) cur_alt_setting;

#define filter (1)
#if filter
    /* Higher gain for speech frequencies */
    const float a0 = 0.045f;
    /* Zero creates slight resonance in speech range */
    const float a1 = 0.0f;
    /* boost mid frequencies */
    const float a2 = -0.025f;
    /* Tuned for speech frequency range */
    const float b1 = -1.62f;
    /* Balances between clarity and noise reduction */
    const float b2 = 0.74f;

    /* Input history */
    static float x1 = 0, x2 = 0;
    /* Output history*/
    static float y1 = 0, y2 = 0;
    const int gainReduction = 9;

    /* Increased noise threshold and added hysteresis */
    const float noiseThreshold =
        0.006f; /* Slightly lower to catch more speech */
    const float noiseFloor =
        0.004f; /* Lower to prevent cutting off quiet parts */
    static bool is_silent = true; /* Track noise gate state */

    for (size_t cnt = 0; cnt < 48; cnt++) {
        /* Apply gain reduction */
        float x0 = (float) ((audioBuffer[cnt]) >> gainReduction);

        /* Apply bandpass filter */
        float y0 = a0 * x0 + a1 * x1 + a2 * x2 - b1 * y1 - b2 * y2;

        /* Update history */
        x2 = x1;
        x1 = x0;
        y2 = y1;
        y1 = y0;

        /* Apply improved noise gate with hysteresis to prevent "flutter" */
        float absY0 = fabsf(y0);
        if (is_silent) {
            /* Currently silent, check if we should start passing audio */
            if (absY0 > noiseThreshold) {
                is_silent = false;
            } else {
                y0 = 0.0f;
            }
        } else {
            /* Currently passing audio, check if we should go silent */
            if (absY0 < noiseFloor) {
                is_silent = true;
                y0        = 0.0f;
            }
        }

        /* Store filtered output */
        test_buffer_audio[cnt] = (int32_t) y0;
    }
#else
    /* Unfiltered audio */
    for (size_t cnt = 0; cnt < 48; cnt++) {
        test_buffer_audio[cnt] = (audioBuffer[cnt]);
    }
#endif

    return true;
}

bool tud_audio_set_itf_close_EP_cb(
    uint8_t rhport, tusb_control_request_t const *p_request)
{
    (void) rhport;
    (void) p_request;
    startVal = 0;

    return true;
}

//--------------------------------------------------------------------+
// BLINKING TASK
//--------------------------------------------------------------------+
void led_blinking_task(void)
{
    static uint32_t start_ms = 0;
    static bool led_state    = false;

    // Blink every interval ms
    if (board_millis() - start_ms < blink_interval_ms)
        return;  // not enough time
    start_ms += blink_interval_ms;

    board_led_write(led_state);
    led_state = 1 - led_state;  // toggle
}
