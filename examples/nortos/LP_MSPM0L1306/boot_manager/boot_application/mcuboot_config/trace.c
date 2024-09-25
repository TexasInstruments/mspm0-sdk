/******************************************************************************

 @file  itm.c

 @brief Provides ITM module functions

 Group: WCS, LPC
 $Target Device: DEVICES $

 ******************************************************************************
 $License: TISD 2011 $
 ******************************************************************************
 $Release Name: PACKAGE NAME $
 $Release Date: PACKAGE RELEASE DATE $
 *****************************************************************************/

#ifndef EXCLUDE_TRACE

#include <ti/drivers/dpl/SemaphoreP.h>
#include <ti/drivers/dpl/SystemP.h>

#endif

#include "itm_private.h"
#include "trace.h"

#include <stdint.h>

#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

//#include <ti/devices/DeviceFamily.h>
//#include DeviceFamily_constructPath(driverlib/ioc.h)

#ifndef EXCLUDE_TRACE

#define ITM_STIM_PORT_8(x) (*(volatile uint8_t *) ITM_STIM_PORT((x)))
#define ITM_STIM_PORT_32(x) (*(volatile uint32_t *) ITM_STIM_PORT((x)))

#define ITM_PORT 0

#define VT100_COLOR_ERROR "\x1b[31m"
#define VT100_COLOR_WARN "\x1b[33m"
#define VT100_COLOR_INFO "\x1b[39m"
#define VT100_COLOR_DEBUG "\x1b[90m"

#define DEFAULT_TRACE_TMP_LINE_LEN 128

typedef enum {
    ITM_9600    = 9600,
    ITM_57600   = 57600,
    ITM_115200  = 115200,
    ITM_230400  = 230400,
    ITM_460800  = 460800,
    ITM_500000  = 500000,
    ITM_576000  = 576000,
    ITM_921600  = 921600,
    ITM_1000000 = 1000000,
    ITM_1152000 = 1152000,
    ITM_1500000 = 1500000,
    ITM_2000000 = 2000000,
    ITM_2500000 = 2500000,
    ITM_3000000 = 3000000,
    ITM_3500000 = 3500000,
    ITM_4000000 = 4000000,
    ITM_6000000 = 6000000
} ITM_baudrate;

typedef struct _ITM_config_ {
    uint_fast32_t systemClock;
    ITM_baudrate baudRate;
} ITM_config;

typedef enum {
    ITM_TS_DIV_NONE = 0,
    ITM_TS_DIV_4    = 1,
    ITM_TS_DIV_16   = 2,
    ITM_TS_DIV_64   = 3
} ITM_tsPrescale;

typedef enum {
    ITM_SYNC_NONE        = 0,
    ITM_SYNC_16M_CYCLES  = 1,
    ITM_SYNC_64M_CYCLES  = 2,
    ITM_SYNC_256M_CYCLES = 3
} ITM_syncPacketRate;

char tmpStr[DEFAULT_TRACE_TMP_LINE_LEN];

static SemaphoreP_Handle trace_mutex_handle;
static SemaphoreP_Struct trace_mutex_struct;

void trace_init(void)
{
    ITM_config itm_config = {48000000, ITM_3000000};

    /* Construct a SemaphoreP object to be use as a resource lock, initial count 1 */
    trace_mutex_handle = SemaphoreP_constructBinary(&trace_mutex_struct, 1);

    // Disable module
    SCS_DEMCR &= (~SCS_DEMCR_TRCEN);
    ITM_TCR = 0x00000000;

    // Enable trace
    SCS_DEMCR |= SCS_DEMCR_TRCEN;

    // Unlock and Setup TPIU for SWO UART mode
    TPIU_LAR   = CS_LAR_UNLOCK;
    TPIU_SPPR  = TPIU_SPPR_SWO_UART;
    TPIU_CSPSR = TPIU_CSPSR_PIN_1;

    // Unlock and enable all ITM stimulus ports with default settings
    ITM_LAR = CS_LAR_UNLOCK;
    ITM_TER = ITM_TER_ENABLE_ALL;
    ITM_TPR = ITM_TPR_ENABLE_USER_ALL;

    // Setup Baud rate
    if (itm_config.systemClock) {
        uint32_t prescalar = itm_config.systemClock / itm_config.baudRate;

        // Offset with current prescalar value
        uint32_t diff1 =
            itm_config.systemClock - (prescalar * itm_config.baudRate);
        // Offset with prescalar+1 value
        uint32_t diff2 =
            ((prescalar + 1) * itm_config.baudRate) - itm_config.systemClock;

        if (diff2 < diff1) prescalar++;
        // Program prescalar value as (prescalar factor - 1)
        TPIU_ACPR = (prescalar - 1);
    }

    // Disable formatter
    TPIU_FFCR = 0;

    // Unlock DWT
    DWT_LAR = CS_LAR_UNLOCK;

    // Configure SWO Traces
    IOCPortConfigureSet(IOID_13, IOC_PORT_MCU_SWV, IOC_STD_OUTPUT);

    trace_enable_module();
    //trace_enable_exception_trace();
}

void trace_printf(uint8_t dlevel, const char *grp, const char *fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);
    trace_vprintf(dlevel, grp, fmt, ap);
    va_end(ap);
}

void trace_vprintf(
    uint8_t dlevel, const char *grp, const char *fmt, va_list ap)
{
    SemaphoreP_pend(trace_mutex_handle, SemaphoreP_WAIT_FOREVER);
    uint32_t strLen;
    uint32_t strIdx;

    switch (dlevel) {
        case (TRACE_LEVEL_ERROR):
            strLen = SystemP_snprintf(tmpStr, DEFAULT_TRACE_TMP_LINE_LEN,
                "%s[ERR ][%-4s]: ", VT100_COLOR_ERROR, grp);
            break;
        case (TRACE_LEVEL_WARN):
            strLen = SystemP_snprintf(tmpStr, DEFAULT_TRACE_TMP_LINE_LEN,
                "%s[WARN][%-4s]: ", VT100_COLOR_WARN, grp);
            break;
        case (TRACE_LEVEL_INFO):
            strLen = SystemP_snprintf(tmpStr, DEFAULT_TRACE_TMP_LINE_LEN,
                "%s[INFO][%-4s]: ", VT100_COLOR_INFO, grp);
            break;
        case (TRACE_LEVEL_DEBUG):
            strLen = SystemP_snprintf(tmpStr, DEFAULT_TRACE_TMP_LINE_LEN,
                "%s[DBG ][%-4s]: ", VT100_COLOR_DEBUG, grp);
            break;
        default:
            strLen = SystemP_snprintf(
                tmpStr, DEFAULT_TRACE_TMP_LINE_LEN, "                ");
            break;
    }

    for (strIdx = 0; strLen > 0; strLen--, strIdx++) {
        trace_put_char_blocking(tmpStr[strIdx]);
    }

    strLen = SystemP_vsnprintf(tmpStr, DEFAULT_TRACE_TMP_LINE_LEN, fmt, ap);
    for (strIdx = 0; strLen > 0; strLen--, strIdx++) {
        trace_put_char_blocking(tmpStr[strIdx]);
    }

    //add zero color VT100 and new line
    strLen = SystemP_snprintf(tmpStr, DEFAULT_TRACE_TMP_LINE_LEN, "\x1b[0m\n");
    for (strIdx = 0; strLen > 0; strLen--, strIdx++) {
        trace_put_char_blocking(tmpStr[strIdx]);
    }

    SemaphoreP_post(trace_mutex_handle);
}

void trace_enable_module(void)
{
    // Enable ITM module
    ITM_TCR |= ITM_TCR_ENABLE_ITM;
}

void trace_disable_module(void)
{
    // Disable ITM module
    ITM_TCR &= ~ITM_TCR_ENABLE_ITM;
}

void trace_put_char_blocking(const char ch)
{
    while (0 == ITM_STIM_PORT_8(ITM_PORT))
        ;
    ITM_STIM_PORT_8(ITM_PORT) = ch;
}

void trace_enable_exception_trace(void)
{
    DWT_CTRL |= DWT_CTRL_ENABLE_EXC_TRC;
    ITM_TCR |= ITM_TCR_ENABLE_DWT_TX;
}

void trace_enable_ps_sampling(void)
{
    // Clear the PC Sampling and Cycle Event bits
    DWT_CTRL &= ~(DWT_CTRL_ENABLE_PC_SAMP | DWT_CTRL_ENABLE_CYC_EVT);
    // Setup Sampling interval
    DWT_CTRL |= DWT_CTRL_CYC_CNT_1024;
    ITM_TCR |= ITM_TCR_ENABLE_DWT_TX;
    DWT_CTRL |= DWT_CTRL_ENABLE_CYC_CNT;
    // Enable PC sampling event
    DWT_CTRL |= DWT_CTRL_ENABLE_PC_SAMP;
}

bool trace_enable_data_trace(const uint32_t *variable)
{
    uint_least8_t numDwtComp =
        (DWT_CTRL & DWT_CTRL_MASK_NUM_COMP) >> DWT_CTRL_SHIFT_NUM_COMP;
    uint_least8_t dwtIndex = 0;
    bool dwtAvailable      = false;
    for (dwtIndex = 0; dwtIndex < numDwtComp; dwtIndex++) {
        if (0 == DWT_FUNC(dwtIndex)) {
            DWT_COMP(dwtIndex) = (uint32_t) variable;
            DWT_MASK(dwtIndex) = 0x0;
            DWT_FUNC(dwtIndex) =
                (DWT_FUNC_DATA_SIZE_32 | DWT_FUNC_ENABLE_ADDR_OFFSET |
                    DWT_FUNC_ENABLE_COMP_RW);
            dwtAvailable = true;
        }
    }
    return dwtAvailable;
}

void trace_enable_cycle_counter(void)
{
    // Clear the PC Sampling and Cycle Event bits
    DWT_CTRL &= ~(DWT_CTRL_ENABLE_PC_SAMP | DWT_CTRL_ENABLE_CYC_EVT);
    // Setup Sampling interval
    DWT_CTRL |= DWT_CTRL_CYC_CNT_1024;
    ITM_TCR |= ITM_TCR_ENABLE_DWT_TX;
    DWT_CTRL |= DWT_CTRL_ENABLE_CYC_CNT;
    // Enable Cycle Count event
    DWT_CTRL |= DWT_CTRL_ENABLE_CYC_EVT;
}

void trace_enable_timing(ITM_tsPrescale tsPrescale)
{
    // Set timestamp prescalar enable timestamp generation
    ITM_TCR |=
        ((tsPrescale << ITM_TCR_TS_PRESCALE_SHIFT) & ITM_TCR_TS_PRESCALE_MASK);
    ITM_TCR |= (ITM_TCR_ENABLE_TS);
}

void trace_enable_sync_packets(ITM_syncPacketRate syncPacketRate)
{
    // Clear sync packet rate
    DWT_CTRL &= ~(DWT_CTRL_MASK_SYNCTAP);
    // Set sync packet rate
    DWT_CTRL |=
        ((syncPacketRate << DWT_CTRL_SHIFT_SYNCTAP) & DWT_CTRL_MASK_SYNCTAP);
    // Enable sync packet generation
    DWT_CTRL |= DWT_CTRL_ENABLE_CYC_CNT;
    ITM_TCR |= (ITM_TCR_ENABLE_SYNC);
}

void trace_flush_module(void)
{
    // Wait till the ITM Busy status has been cleared
    while (ITM_TCR & ITM_TCR_BUSY)
        ;
}

char *trace_array(const uint8_t *buf, uint16_t len)
{
    int i;
    if (len == 0 || tmpStr == NULL) {
        return "";
    }
    if (buf == NULL) {
        return "<null>";
    }
    tmpStr[0]          = 0;
    const uint8_t *ptr = buf;
    char overflow      = 0;
    for (i = 0; i < len; i++) {
        int retval =
            snprintf(tmpStr, DEFAULT_TRACE_TMP_LINE_LEN, "%02x:", *ptr++);
        if (retval <= 0 || retval > DEFAULT_TRACE_TMP_LINE_LEN) {
            overflow = 1;
            break;
        }
    }

    if (overflow) {
        // replace last character as 'star',
        // which indicate buffer len is not enough
        tmpStr[DEFAULT_TRACE_TMP_LINE_LEN - 1] = '*';
    } else {
        //null to replace last ':' character
        tmpStr[DEFAULT_TRACE_TMP_LINE_LEN - 1] = 0;
    }

    return tmpStr;
}

#endif  //EXCLUDE_TRACE
