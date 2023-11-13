/*
 * Copyright (c) 2020, Texas Instruments Incorporated
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * *  Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * *  Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * *  Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
/*!****************************************************************************
 *  @file       dl_rtc.h
 *  @brief      Real Time Clock (RTC) Peripheral Interface
 *  @defgroup   RTC Real Time Clock (RTC)
 *
 *  @anchor ti_devices_msp_dl_rtc_Overview
 *  # Overview
 *  The RTC Driver Library allows full configuration of the MSPM0 RTC module.
 *  The real-time clock (RTC) module provides clock counters with calendar mode,
 *  a flexible programmable alarm, offset calibration, and a provision for
 *  temperature compensation.
 *
 *  <hr>
 ******************************************************************************
 */
/** @addtogroup RTC
 * @{
 */
#ifndef ti_dl_dl_rtc__include
#define ti_dl_dl_rtc__include

#include <stdbool.h>
#include <stdint.h>

#include <ti/devices/msp/msp.h>
#include <ti/driverlib/dl_common.h>

#ifdef __MSPM0_HAS_RTC__

#ifdef __cplusplus
extern "C" {
#endif

/* clang-format off */

/** @addtogroup DL_RTC_INTERRUPT
 *  @{
 */

/**
 * @brief RTC alarm 1 interrupt
 */
#define DL_RTC_INTERRUPT_CALENDAR_ALARM1        (RTC_CPU_INT_IMASK_RTCA1_SET)

/**
 * @brief RTC alarm 2 interrupt
 */
#define DL_RTC_INTERRUPT_CALENDAR_ALARM2        (RTC_CPU_INT_IMASK_RTCA2_SET)

/**
 * @brief RTC prescaler 0 interrupt
 */
#define DL_RTC_INTERRUPT_PRESCALER0             (RTC_CPU_INT_IMASK_RT0PS_SET)

/**
 * @brief RTC prescaler 1 interrupt
 */
#define DL_RTC_INTERRUPT_PRESCALER1             (RTC_CPU_INT_IMASK_RT1PS_SET)

/**
 * @brief RTC interval timer interrupt
 */
#define DL_RTC_INTERRUPT_INTERVAL_ALARM        (RTC_CPU_INT_IMASK_RTCTEV_SET)

/**
 * @brief RTC ready interrupt
 */
#define DL_RTC_INTERRUPT_READY                 (RTC_CPU_INT_IMASK_RTCRDY_SET)

/** @}*/

/** @addtogroup DL_RTC_EVENT
 *  @{
 */

/**
 * @brief RTC alarm 1 event
 */
#define DL_RTC_EVENT_CALENDAR_ALARM1            (RTC_GEN_EVENT_IMASK_RTCA1_SET)

/**
 * @brief RTC alarm 2 event
 */
#define DL_RTC_EVENT_CALENDAR_ALARM2            (RTC_GEN_EVENT_IMASK_RTCA2_SET)

/**
 * @brief RTC prescaler 0 event
 */
#define DL_RTC_EVENT_PRESCALER0                 (RTC_GEN_EVENT_IMASK_RT0PS_SET)

/**
 * @brief RTC prescaler 1 event
 */
#define DL_RTC_EVENT_PRESCALER1                 (RTC_GEN_EVENT_IMASK_RT1PS_SET)

/**
 * @brief RTC interval timer event
 */
#define DL_RTC_EVENT_INTERVAL_ALARM            (RTC_GEN_EVENT_IMASK_RTCTEV_SET)

/**
 * @brief RTC ready event
 */
#define DL_RTC_EVENT_READY                     (RTC_GEN_EVENT_IMASK_RTCRDY_SET)


/** @}*/

/*! @enum DL_RTC_STATUS */
typedef enum {
    /*! RTC is safe for reading */
    DL_RTC_STATUS_READY = RTC_STA_RTCRDY_READY,
    /*! RTC is not safe for reading */
    DL_RTC_STATUS_NOT_READY = RTC_STA_RTCRDY_NOT_READY
} DL_RTC_STATUS;

/*! @enum DL_RTC_COMPENSATION_STATUS */
typedef enum {
    /*! RTC ready to get new compensation values */
    DL_RTC_COMPENSATION_STATUS_READY = RTC_STA_RTCTCRDY_READY,
    /*! RTC not ready to get new compensaiton values */
    DL_RTC_COMPENSATION_STATUS_NOT_READY = RTC_STA_RTCTCRDY_NOT_READY
} DL_RTC_COMPENSATION_STATUS;

/*! @enum DL_RTC_COMPENSATION_WRITE_RESULT */
typedef enum {
    /*! Compensation write successful */
    DL_RTC_COMPENSATION_WRITE_RESULT_OK = RTC_STA_RTCTCOK_OK,
    /*! Compensation write unsuccessful */
    DL_RTC_COMPENSATION_WRITE_RESULT_NOT_OK = RTC_STA_RTCTCOK_NOT_OK
} DL_RTC_COMPENSATION_WRITE_RESULT;

/*! @enum DL_RTC_OFFSET_CALIBRATION_SIGN */
typedef enum {
    /*! Calibration frequency adjusted down */
    DL_RTC_OFFSET_CALIBRATION_SIGN_DOWN = RTC_CAL_RTCOCALS_DOWN,
    /*! Calibration frequency adjusted up */
    DL_RTC_OFFSET_CALIBRATION_SIGN_UP = RTC_CAL_RTCOCALS_UP
} DL_RTC_OFFSET_CALIBRATION_SIGN;

/*! @enum DL_RTC_OFFSET_CALIBRATION_FREQUENCY */
typedef enum {
    /*! No frequency output to RTCCLK pin */
    DL_RTC_OFFSET_CALIBRATION_FREQUENCY_OFF = RTC_CAL_RTCCALFX_OFF,
    /*! 512 Hz frequency output to RTCLK pin */
    DL_RTC_OFFSET_CALIBRATION_FREQUENCY_512 = RTC_CAL_RTCCALFX_F512HZ,
    /*! 256 Hz frequency output to RTCLK pin */
    DL_RTC_OFFSET_CALIBRATION_FREQUENCY_256 = RTC_CAL_RTCCALFX_F256HZ,
    /*! 1 Hz frequency output to RTCLK pin */
    DL_RTC_OFFSET_CALIBRATION_FREQUENCY_1 = RTC_CAL_RTCCALFX_F1HZ
} DL_RTC_OFFSET_CALIBRATION_FREQUENCY;

/*! @enum DL_RTC_TEMP_CALIBRATION */
typedef enum {
    /*! Calibrate at steps of -1 */
    DL_RTC_TEMP_CALIBRATION_DOWN1PPM = RTC_TCMP_RTCTCMPS_DOWN,
    /*! Calibrate at steps of +1 */
    DL_RTC_TEMP_CALIBRATION_UP1PPM = RTC_TCMP_RTCTCMPS_UP
} DL_RTC_TEMP_CALIBRATION;

/*! @enum DL_RTC_FORMAT */
typedef enum {
    /*! Select binary format for the real-time clock */
    DL_RTC_FORMAT_BINARY = RTC_CTL_RTCBCD_BINARY,
    /*! Select BCD format for the real-time clock */
    DL_RTC_FORMAT_BCD = RTC_CTL_RTCBCD_BCD
} DL_RTC_FORMAT;

/*! @enum DL_RTC_INTERVAL_ALARM */
typedef enum {
    /*! Assert Calendar interrupt on every minute */
    DL_RTC_INTERVAL_ALARM_MINUTECHANGE = RTC_CTL_RTCTEVTX_MINUTE,
    /*! Assert Calendar interrupt on every hour */
    DL_RTC_INTERVAL_ALARM_HOURCHANGE = RTC_CTL_RTCTEVTX_HOUR,
    /*! Assert Calendar interrupt when hour is 12 */
    DL_RTC_INTERVAL_ALARM_NOON = RTC_CTL_RTCTEVTX_NOON,
    /*! Assert Calendar interrupt when hour is 0 */
    DL_RTC_INTERVAL_ALARM_MIDNIGHT = RTC_CTL_RTCTEVTX_MIDNIGHT
} DL_RTC_INTERVAL_ALARM;

/*! @enum DL_RTC_PRESCALER0_DIVIDE */
typedef enum {
    /*! Prescaler 0 Divide by 8 */
    DL_RTC_PRESCALER0_DIVIDE_8 = RTC_PSCTL_RT0IP_DIV8,
    /*! Prescaler 0 Divide by 16 */
    DL_RTC_PRESCALER0_DIVIDE_16 = RTC_PSCTL_RT0IP_DIV16,
    /*! Prescaler 0 Divide by 32 */
    DL_RTC_PRESCALER0_DIVIDE_32 = RTC_PSCTL_RT0IP_DIV32,
    /*! Prescaler 0 Divide by 64 */
    DL_RTC_PRESCALER0_DIVIDE_64 = RTC_PSCTL_RT0IP_DIV64,
    /*! Prescaler 0 Divide by 128 */
    DL_RTC_PRESCALER0_DIVIDE_128 = RTC_PSCTL_RT0IP_DIV128,
    /*! Prescaler 0 Divide by 256 */
    DL_RTC_PRESCALER0_DIVIDE_256 = RTC_PSCTL_RT0IP_DIV256
} DL_RTC_PRESCALER0_DIVIDE;

/*! @enum DL_RTC_PRESCALER1_DIVIDE */
typedef enum {
    /*! Prescaler 1 Divide by 2 */
    DL_RTC_PRESCALER1_DIVIDE_2 = RTC_PSCTL_RT1IP_DIV2,
    /*! Prescaler 1 Divide by 4 */
    DL_RTC_PRESCALER1_DIVIDE_4 = RTC_PSCTL_RT1IP_DIV4,
    /*! Prescaler 1 Divide by 8 */
    DL_RTC_PRESCALER1_DIVIDE_8 = RTC_PSCTL_RT1IP_DIV8,
    /*! Prescaler 1 Divide by 16 */
    DL_RTC_PRESCALER1_DIVIDE_16 = RTC_PSCTL_RT1IP_DIV16,
    /*! Prescaler 1 Divide by 32 */
    DL_RTC_PRESCALER1_DIVIDE_32 = RTC_PSCTL_RT1IP_DIV32,
    /*! Prescaler 1 Divide by 64 */
    DL_RTC_PRESCALER1_DIVIDE_64 = RTC_PSCTL_RT1IP_DIV64,
    /*! Prescaler 1 Divide by 128 */
    DL_RTC_PRESCALER1_DIVIDE_128 = RTC_PSCTL_RT1IP_DIV128,
    /*! Prescaler 1 Divide by 256 */
    DL_RTC_PRESCALER1_DIVIDE_256 = RTC_PSCTL_RT1IP_DIV256
} DL_RTC_PRESCALER1_DIVIDE;

/*! @enum DL_RTC_IIDX */
typedef enum {
    /*! RTC interrupt index for alarm 1 */
    DL_RTC_IIDX_ALARM1 = RTC_CPU_INT_IIDX_STAT_RTCA1,
    /*! RTC interrupt index for alarm 2 */
    DL_RTC_IIDX_ALARM2 = RTC_CPU_INT_IIDX_STAT_RTCA2,
    /*! RTC interrupt index for prescaler 0 */
    DL_RTC_IIDX_PRESCALER0 = RTC_CPU_INT_IIDX_STAT_RT0PS,
    /*! RTC interrupt index for prescaler 1 */
    DL_RTC_IIDX_PRESCALER1 = RTC_CPU_INT_IIDX_STAT_RT1PS,
    /*! RTC interrupt index for interval timer */
    DL_RTC_IIDX_INTERVAL_TIMER = RTC_CPU_INT_IIDX_STAT_RTCTEV,
    /*! RTC interrupt index for RTC ready */
    DL_RTC_IIDX_READY = RTC_CPU_INT_IIDX_STAT_RTCRDY
} DL_RTC_IIDX;

/* clang-format on */

/**
 * @brief   Alarm struct to use in DL_RTC_setCalendarAlarm
 *
 * @note    All struct values must be in the same format (Binary OR BCD).
 *          RTC format is set during initialization.
 */
typedef struct {
    /** The minute to set the Alarm time to. Valid values are 0-59 in Binary
     * format OR 0x0-0x59 in BCD format. */
    uint8_t minutes;
    /** The hour to set the Alarm time to. Valid values are 0-23 in Binary
     * format OR 0x0-0x23 in BCD format. */
    uint8_t hours;
    /** The day of the week to set the Alarm time to. Valid values are 0-6
     * in Binary format OR 0x0-0x6 in BCD format. */
    uint8_t dayOfWeek;
    /** The day of the month to set the Alarm time to. Valid values are 1-31
     * in Binary format OR 0x1-0x31 in BCD format. */
    uint8_t dayOfMonth;
} DL_RTC_CalendarAlarm;

/**
 * @brief   Calendar struct to use in DL_RTC_initCalendar()
 *
 * @note    All struct values must be in the same format (Binary OR BCD).
 *          RTC format is set during initialization.
 */
typedef struct {
    /** The second to set the Calendar time to. Valid values are 0-59 in Binary
     * format OR 0x0-0x59 in BCD format. */
    uint8_t seconds;
    /** The minute to set the Calendar time to. Valid values are 0-59 in Binary
     * format OR 0x0-0x59 in BCD format. */
    uint8_t minutes;
    /** The hour to set the Calendar time to. Valid values are 0-23 in Binary
     * format OR 0x0-0x23 in BCD format. */
    uint8_t hours;
    /** The day of the week to set the Calendar time to. Valid values are 0-6
     * in Binary format OR 0x0-0x6 in BCD format. */
    uint8_t dayOfWeek;
    /** The day of the month to set the Calendar time to. Valid values are 1-31
     * in Binary format OR 0x1-0x31 in BCD format. */
    uint8_t dayOfMonth;
    /** The month to set the Calendar time to. Valid values are 1-12 in Binary
     * format OR 0x1-0x12 in BCD format. */
    uint8_t month;
    /** The year to set the Calendar time to. Valid values are 0-4095 in Binary
     * format OR 0x0-0x4095 in BCD format. */
    uint16_t year;
} DL_RTC_Calendar;

/**
 *  @brief      Enable power to the RTC module.
 *
 *
 *  @param[in]  rtc    Pointer to the register overlay for the RTC peripheral
 */
__STATIC_INLINE void DL_RTC_enablePower(RTC_Regs *rtc)
{
    rtc->GPRCM.PWREN = (RTC_PWREN_KEY_UNLOCK_W | RTC_PWREN_ENABLE_ENABLE);
}

/**
 *  @brief      Disable power to the RTC module.
 *
 *
 *  @param[in]  rtc    Pointer to the register overlay for the RTC peripheral
 */
__STATIC_INLINE void DL_RTC_disablePower(RTC_Regs *rtc)
{
    rtc->GPRCM.PWREN = (RTC_PWREN_KEY_UNLOCK_W | RTC_PWREN_ENABLE_DISABLE);
}

/**
 *  @brief Returns if power is enabled for the RTC module
 *
 *  @param[in]  rtc    Pointer to the register overlay for the RTC peripheral
 *
 *  @return true if power is enabled
 *  @return false if power is disabled
 */
__STATIC_INLINE bool DL_RTC_isPowerEnabled(RTC_Regs *rtc)
{
    return (
        (rtc->GPRCM.PWREN & RTC_PWREN_ENABLE_MASK) == RTC_PWREN_ENABLE_ENABLE);
}

/**
 *  @brief      Resets all registers in the RTC module.
 *
 *
 *  @param[in]  rtc    Pointer to the register overlay for the RTC peripheral
 */
__STATIC_INLINE void DL_RTC_reset(RTC_Regs *rtc)
{
    rtc->GPRCM.RSTCTL =
        (RTC_RSTCTL_KEY_UNLOCK_W | RTC_RSTCTL_RESETSTKYCLR_CLR |
            RTC_RSTCTL_RESETASSERT_ASSERT);
}

/**
 *  @brief      Returns if the RTC module was reset
 *
 *
 *  @param[in]  rtc    Pointer to the register overlay for the RTC peripheral
 *
 *  @return true if module was reset
 *  @return false if module was not reset
 */
__STATIC_INLINE bool DL_RTC_isReset(RTC_Regs *rtc)
{
    return ((rtc->GPRCM.STAT & RTC_STAT_RESETSTKY_MASK) ==
            RTC_STAT_RESETSTKY_RESET);
}

/**
 *  @brief      Enables the supply of the 32kHz clock to the RTC. It will not
 *              power up the 32kHz crystal oscillator. This needs to be done
 *              in the Clock System Module.
 *
 *
 *  @param[in]  rtc    Pointer to the register overlay for the RTC peripheral
 */
__STATIC_INLINE void DL_RTC_enableClockControl(RTC_Regs *rtc)
{
    rtc->CLKCTL = RTC_CLKCTL_MODCLKEN_ENABLE;
}

/**
 *  @brief      Disables the supply of the 32kHz clock to the RTC.
 *
 *
 *  @param[in]  rtc    Pointer to the register overlay for the RTC peripheral
 */
__STATIC_INLINE void DL_RTC_disableClockControl(RTC_Regs *rtc)
{
    rtc->CLKCTL = RTC_CLKCTL_MODCLKEN_DISABLE;
}

/**
 *  @brief      Enables interrupts when the CPU is in debug mode. Interrupt
 *              requests are signaled to the interrupt controller.
 *
 *
 *  @param[in]  rtc    Pointer to the register overlay for the RTC peripheral
 */
__STATIC_INLINE void DL_RTC_enableDebugInterrupts(RTC_Regs *rtc)
{
    rtc->DBGCTL |= RTC_DBGCTL_DBGINT_ON;
}

/**
 *  @brief      Disables interrupts when the CPU is in debug mode. Interrupts
 *              of the RTC module will not be captured. There will be no
 *              updates to the RTCRIS, RTCMISC and RTCIIDX registers.
 *
 *
 *  @param[in]  rtc    Pointer to the register overlay for the RTC peripheral
 */
__STATIC_INLINE void DL_RTC_disableDebugInterrupts(RTC_Regs *rtc)
{
    rtc->DBGCTL &= ~(RTC_DBGCTL_DBGINT_MASK);
}

/**
 *  @brief      Enables the RTC modules debug mode.
 *
 *
 *  @param[in]  rtc    Pointer to the register overlay for the RTC peripheral
 */
__STATIC_INLINE void DL_RTC_enableDebugMode(RTC_Regs *rtc)
{
    rtc->DBGCTL |= RTC_DBGCTL_DBGRUN_RUN;
}

/**
 *  @brief      Disables the RTC modules debug mode.
 *
 *
 *  @param[in]  rtc    Pointer to the register overlay for the RTC peripheral
 */
__STATIC_INLINE void DL_RTC_disableDebugMode(RTC_Regs *rtc)
{
    rtc->DBGCTL &= ~(RTC_DBGCTL_DBGRUN_MASK);
}

/**
 *  @brief      Set the format of the real-time clock
 *
 * The format must be selected before the time is set.
 *
 *  @param[in]  rtc       Pointer to the register overlay for the RTC peripheral
 *  @param[in]  formatSelect    Selectable format the RTC peripheral will
 *                              provide time in. One of @ref DL_RTC_FORMAT
 */
__STATIC_INLINE void DL_RTC_setClockFormat(
    RTC_Regs *rtc, DL_RTC_FORMAT formatSelect)
{
    rtc->CTL |= (uint32_t) formatSelect;
}

/**
 *  @brief      Get the format currently being used for counting the
 *              real-time clock.
 *
 *  @param[in]  rtc    Pointer to the register overlay for the RTC peripheral
 *
 *  @return            Format for counting the real-time clock.
 *
 *  @retval            One of @ref DL_RTC_FORMAT
 */
__STATIC_INLINE DL_RTC_FORMAT DL_RTC_getClockFormat(RTC_Regs *rtc)
{
    uint32_t format = (rtc->CTL & RTC_CTL_RTCBCD_MASK);

    return (DL_RTC_FORMAT)(format);
}

/**
 *  @brief      Sets the interval alarm event condition
 *
 * The interval alarm can be configured to generate an interval alarm event
 * when one of the following events occurs:
 * - MIN changes
 * - HOUR changes
 * - Every day at midnight (00:00:00)
 * - Every day at noon (12:00:00)
 *
 * The interval alarm sources the @ref DL_RTC_INTERRUPT_INTERVAL_ALARM interrupt
 *
 *  @param[in]  rtc    Pointer to the register overlay for the RTC peripheral
 *  @param[in]  alarmEvent  The interval alarm interrupt condition. One of
 *                          @ref DL_RTC_INTERVAL_ALARM
 */
__STATIC_INLINE void DL_RTC_setIntervalAlarm(
    RTC_Regs *rtc, DL_RTC_INTERVAL_ALARM alarmEvent)
{
    DL_Common_updateReg(
        &rtc->CTL, (uint32_t) alarmEvent, RTC_CTL_RTCTEVTX_MASK);
}

/**
 *  @brief      Gets the interval alarm event condition
 *
 * The interval alarm can be configured to generate an interval alarm event
 * when one of the following events occurs:
 * - MIN changes
 * - HOUR changes
 * - Every day at midnight (00:00:00)
 * - Every day at noon (12:00:00)
 *
 *  @param[in]  rtc    Pointer to the register overlay for the RTC peripheral
 *
 *  @return DL_RTC_INTERVAL_ALARM
 */
__STATIC_INLINE DL_RTC_INTERVAL_ALARM DL_RTC_getIntervalAlarm(RTC_Regs *rtc)
{
    uint32_t alarm = (rtc->CTL & RTC_CTL_RTCTEVTX_MASK);

    return (DL_RTC_INTERVAL_ALARM)(alarm);
}

/**
 *  @brief      Checks if RTC is safe for reading
 *
 * This API checks if the counter/calender registers can be safely read.
 *
 * The RTC counter/calendar registers are updated once per second. To prevent
 * reading any counter/calendar register at the time of an update (which could
 * result in an invalid time being read), a keep-out window is provided. The
 * keep-out window is approximately 128/32768 seconds before the counters
 * update.
 *
 *  @param[in]  rtc    Pointer to the register overlay for the RTC peripheral
 *
 *  @return true if RTC is safe for reading
 *  @return false if RTC values are in transition
 *
 */
__STATIC_INLINE bool DL_RTC_isSafetoRead(RTC_Regs *rtc)
{
    return ((rtc->STA & RTC_STA_RTCRDY_MASK) == RTC_STA_RTCRDY_READY);
}

/**
 *  @brief      Checks if RTC is ready to get new calibration values
 *
 *  @param[in]  rtc    Pointer to the register overlay for the RTC peripheral
 *
 *  @return true if RTC can get new calibration values
 *  @return false if RTC is not ready to get new calibration values
 *
 */
__STATIC_INLINE bool DL_RTC_isReadyToCalibrate(RTC_Regs *rtc)
{
    return ((rtc->STA & RTC_STA_RTCTCRDY_MASK) == RTC_STA_RTCTCRDY_READY);
}

/**
 *  @brief      Checks if RTC new calibration values were written OK
 *
 *  @param[in]  rtc    Pointer to the register overlay for the RTC peripheral
 *
 *  @return true if calibration write was successful
 *  @return false if calibration write was unsuccessful
 *
 */
__STATIC_INLINE bool DL_RTC_isCalibrationWriteResultOK(RTC_Regs *rtc)
{
    return ((rtc->STA & RTC_STA_RTCTCOK_MASK) == RTC_STA_RTCTCOK_OK);
}

/**
 *  @brief      Sets output frequency for offset calibration
 *
 * Sets the frequency output to RTC_OUT pin for calibration measurement.
 * The corresponding port must be configured for the peripheral module
 * function.
 *
 *  @param[in]  rtc     Pointer to the register overlay for the RTC peripheral
 *  @param[in]  offsetFrequency   The frequency of offset calibration.
 *                                Valid values are one of
 *                                @ref DL_RTC_OFFSET_CALIBRATION_FREQUENCY
 */
__STATIC_INLINE void DL_RTC_setOffsetCalibrationFrequency(
    RTC_Regs *rtc, DL_RTC_OFFSET_CALIBRATION_FREQUENCY offsetFrequency)
{
    DL_Common_updateReg(
        &rtc->CAL, (uint32_t) offsetFrequency, RTC_CAL_RTCCALFX_MASK);
}

/**
 *  @brief      Sets the offset error calibration adjustment value.
 *
 * Each LSB represents approximately +1ppm (RTCOCALXS = 1) or
 * -1ppm (RTCOCALXS = 0) adjustment in frequency. Maximum effective calibration
 * value is +/-240ppm.
 * Excess values written above +/-240ppm will be ignored by hardware.
 *
 *  @param[in]  rtc     Pointer to the register overlay for the RTC peripheral
 *  @param[in]  offsetSign        The sign of offset calibration.
 *                                Valid values are one of
 *                                @ref DL_RTC_OFFSET_CALIBRATION_SIGN
 *  @param[in]  offsetValue       The value that the offset will be a factor of.
 *                                Must be any integer from 1-240.
 */
__STATIC_INLINE void DL_RTC_setOffsetCalibrationAdjValue(RTC_Regs *rtc,
    DL_RTC_OFFSET_CALIBRATION_SIGN offsetSign, uint8_t offsetValue)
{
    DL_Common_updateReg(&rtc->CAL,
        ((uint32_t) offsetSign | (uint32_t) offsetValue),
        (RTC_CAL_RTCOCALX_MASK | RTC_CAL_RTCOCALS_MASK));
}

/**
 *  @brief      Sets the offset error calibration.
 *
 * Each LSB represents approximately +1ppm (RTCOCALXS = 1) or
 * -1ppm (RTCOCALXS = 0) adjustment in frequency. Maximum effective calibration
 * value is +/-240ppm.
 * Excess values written above +/-240ppm will be ignored by hardware.
 *
 * Sets the frequency output to RTCCLK pin for calibration measurement.
 * The corresponding port must be configured for the peripheral module
 * function.
 *
 *  @param[in]  rtc     Pointer to the register overlay for the RTC peripheral
 *  @param[in]  offsetFrequency   The frequency of offset calibration.
 *                                Valid values are one of
 *                                @ref DL_RTC_OFFSET_CALIBRATION_FREQUENCY
 *  @param[in]  offsetSign        The sign of offset calibration.
 *                                Valid values are one of
 *                                @ref DL_RTC_OFFSET_CALIBRATION_SIGN
 *  @param[in]  offsetValue       The value that the offset will be a factor of.
 *                                Must be any integer from 1-240.
 */
__STATIC_INLINE void DL_RTC_setOffsetCalibration(RTC_Regs *rtc,
    DL_RTC_OFFSET_CALIBRATION_FREQUENCY offsetFrequency,
    DL_RTC_OFFSET_CALIBRATION_SIGN offsetSign, uint8_t offsetValue)
{
    rtc->CAL = (offsetValue & RTC_CAL_RTCOCALX_MASK) |
               (uint32_t) offsetFrequency | (uint32_t) offsetSign;
}

/**
 *  @brief      Gets the offset calibration frequency.
 *
 *  @param[in]  rtc    Pointer to the register overlay for the RTC peripheral
 *
 *  @return            Frequency used in offset calibration.
 *
 *  @retval            One of @ref DL_RTC_OFFSET_CALIBRATION_FREQUENCY
 */
__STATIC_INLINE DL_RTC_OFFSET_CALIBRATION_FREQUENCY
DL_RTC_getOffsetCalibrationFrequency(RTC_Regs *rtc)
{
    uint32_t freq = (rtc->CAL & RTC_CAL_RTCCALFX_MASK);

    return (DL_RTC_OFFSET_CALIBRATION_FREQUENCY)(freq);
}

/**
 *  @brief      Gets the offset calibration sign.
 *
 *  @param[in]  rtc    Pointer to the register overlay for the RTC peripheral
 *
 *  @return            Sign used in offset calibration.
 *
 *  @retval            One of @ref DL_RTC_OFFSET_CALIBRATION_SIGN
 */
__STATIC_INLINE DL_RTC_OFFSET_CALIBRATION_SIGN DL_RTC_getOffsetCalibrationSign(
    RTC_Regs *rtc)
{
    uint32_t sign = (rtc->CAL & RTC_CAL_RTCOCALS_MASK);

    return (DL_RTC_OFFSET_CALIBRATION_SIGN)(sign);
}

/**
 *  @brief      Gets the offset error calibration value.
 *
 *  @param[in]  rtc    Pointer to the register overlay for the RTC peripheral
 *
 *  @return            Offset value used in offset calibration.
 *
 *  @retval            0 - 240  The effective offset error calibration.
 */
__STATIC_INLINE uint32_t DL_RTC_getOffsetCalibrationValue(RTC_Regs *rtc)
{
    return (rtc->CAL & RTC_CAL_RTCOCALX_MASK);
}

/**
 *  @brief      Set the specified RTC temperature compensation.
 *
 *  This function sets the calibration offset to make the RTC as accurate as
 *  possible. The offsetDirection can be either +1-ppm or -1-ppm, and the
 *  offsetValue should be from 1-240 and is multiplied by the direction setting
 *  (i.e. +1-ppm * 8 (offsetValue) = +8-ppm).
 *
 *  @param[in]  rtc               Pointer to the register overlay for the RTC
 *                                peripheral
 *  @param[in]  offsetDirection   The direction that the calibration offset
 *                                will go. Valid values are one of
 *                                @ref DL_RTC_TEMP_CALIBRATION
 *  @param[in]  offsetValue       The value that the offset will be a factor of.
 *                                Must be any integer from 1-240.
 */
__STATIC_INLINE void DL_RTC_setTemperatureCompensation(RTC_Regs *rtc,
    DL_RTC_TEMP_CALIBRATION offsetDirection, uint32_t offsetValue)
{
    rtc->TCMP =
        (offsetValue & RTC_TCMP_RTCTCMPX_MASK) | (uint32_t) offsetDirection;
}

/**
 *  @brief      Get the RTC temperature compensation.
 *
 *  @param[in]  rtc    Pointer to the register overlay for the RTC peripheral
 *
 *  @return            The cumulative temperature compensation
 *
 *  @retval            Signed addition of RTCOCALX and RTCTCMPX values, and
 *                     the updated sign bit (RTCTCMPS) of the addition result.
 *
 */
__STATIC_INLINE uint32_t DL_RTC_getTemperatureCompensation(RTC_Regs *rtc)
{
    return (rtc->TCMP);
}

/**
 *  @brief      Set the seconds in BCD in the RTC calendar
 *
 *  @param[in]  rtc    Pointer to the register overlay for the RTC peripheral
 *  @param[in]  seconds    Value for the seconds of the RTC calendar
 *                         Valid values are 0x0-0x59.
 */
__STATIC_INLINE void DL_RTC_setCalendarSecondsBCD(
    RTC_Regs *rtc, uint8_t seconds)
{
    rtc->SEC = ((uint32_t) seconds << RTC_SEC_SECLOWBCD_OFS) &
               (RTC_SEC_SECLOWBCD_MASK | RTC_SEC_SECHIGHBCD_MASK);
}

/**
 *  @brief      Get the seconds in in BCD the RTC calendar
 *
 *  @param[in]  rtc    Pointer to the register overlay for the RTC peripheral
 *
 *  @pre To prevent reading any counter/calendar registers while those registers
 *  are being updated, the user can call @ref DL_RTC_isSafetoRead to ensure that
 *  the registers are safe to read.
 *
 *  @return            The seconds in the RTC calendar
 *
 *  @retval            Value in range: 0x0-0x59
 *
 */
__STATIC_INLINE uint8_t DL_RTC_getCalendarSecondsBCD(RTC_Regs *rtc)
{
    return (uint8_t)(
        (rtc->SEC & (RTC_SEC_SECLOWBCD_MASK | RTC_SEC_SECHIGHBCD_MASK)) >>
        RTC_SEC_SECLOWBCD_OFS);
}

/**
 *  @brief      Set the minutes in in BCD the RTC calendar
 *
 *  @param[in]  rtc    Pointer to the register overlay for the RTC peripheral
 *  @param[in]  minutes    Value for the minutes of the RTC calendar
 *                         Valid values are 0x0-0x59.
 */
__STATIC_INLINE void DL_RTC_setCalendarMinutesBCD(
    RTC_Regs *rtc, uint8_t minutes)
{
    rtc->MIN = ((uint32_t) minutes << RTC_MIN_MINLOWBCD_OFS) &
               (RTC_MIN_MINLOWBCD_MASK | RTC_MIN_MINHIGHBCD_MASK);
}

/**
 *  @brief      Get the minutes in in BCD the RTC calendar
 *
 *  @param[in]  rtc    Pointer to the register overlay for the RTC peripheral
 *
 *  @pre To prevent reading any counter/calendar registers while those registers
 *  are being updated, the user can call @ref DL_RTC_isSafetoRead to ensure that
 *  the registers are safe to read.
 *
 *  @return            The minutes in the RTC calendar
 *
 *  @retval            Value in range: 0x0-0x59
 *
 */
__STATIC_INLINE uint8_t DL_RTC_getCalendarMinutesBCD(RTC_Regs *rtc)
{
    return (uint8_t)(
        (rtc->MIN & (RTC_MIN_MINLOWBCD_MASK | RTC_MIN_MINHIGHBCD_MASK)) >>
        RTC_MIN_MINLOWBCD_OFS);
}

/**
 *  @brief      Set the hours in in BCD the RTC calendar
 *
 *  @param[in]  rtc    Pointer to the register overlay for the RTC peripheral
 *  @param[in]  hours    Value for the hours of the RTC calendar
 *                       Valid values are 0x0-0x23
 */
__STATIC_INLINE void DL_RTC_setCalendarHoursBCD(RTC_Regs *rtc, uint8_t hours)
{
    rtc->HOUR = ((uint32_t) hours << RTC_HOUR_HOURLOWBCD_OFS) &
                (RTC_HOUR_HOURLOWBCD_MASK | RTC_HOUR_HOURHIGHBCD_MASK);
}

/**
 *  @brief      Get the hours in in BCD the RTC calendar
 *
 *  @param[in]  rtc    Pointer to the register overlay for the RTC peripheral
 *
 *  @pre To prevent reading any counter/calendar registers while those registers
 *  are being updated, the user can call @ref DL_RTC_isSafetoRead to ensure that
 *  the registers are safe to read.
 *
 *  @return            The hours in the RTC calendar
 *
 *  @retval            Value in range: 0x0-0x23
 *
 */
__STATIC_INLINE uint8_t DL_RTC_getCalendarHoursBCD(RTC_Regs *rtc)
{
    return (uint8_t)(
        (rtc->HOUR & (RTC_HOUR_HOURLOWBCD_MASK | RTC_HOUR_HOURHIGHBCD_MASK)) >>
        RTC_HOUR_HOURLOWBCD_OFS);
}

/**
 *  @brief      Set the day of the week in BCD in the RTC calendar
 *
 *  @param[in]  rtc    Pointer to the register overlay for the RTC peripheral
 *  @param[in]  day    Value for the day of the week of the RTC calendar
 *                     Valid values are 0x0-0x6
 */
__STATIC_INLINE void DL_RTC_setCalendarDayOfWeekBCD(RTC_Regs *rtc, uint8_t day)
{
    DL_Common_updateReg(&rtc->DAY, day, RTC_DAY_DOW_MASK);
}

/**
 *  @brief      Get the day of the week in BCD in the RTC calendar
 *
 *  @param[in]  rtc    Pointer to the register overlay for the RTC peripheral
 *
 *  @pre To prevent reading any counter/calendar registers while those registers
 *  are being updated, the user can call @ref DL_RTC_isSafetoRead to ensure that
 *  the registers are safe to read.
 *
 *  @return            The day of the week in the RTC calendar
 *
 *  @retval            Value in range: 0x0-0x6
 *
 */
__STATIC_INLINE uint8_t DL_RTC_getCalendarDayOfWeekBCD(RTC_Regs *rtc)
{
    return (uint8_t)(rtc->DAY & RTC_DAY_DOW_MASK);
}

/**
 *  @brief      Set the day of the month in BCD in the RTC calendar
 *
 *  @param[in]  rtc    Pointer to the register overlay for the RTC peripheral
 *  @param[in]  day    Value for the day of the month of the RTC calendar
 *                     Valid values are 0x1-0x31
 */
__STATIC_INLINE void DL_RTC_setCalendarDayOfMonthBCD(
    RTC_Regs *rtc, uint8_t day)
{
    DL_Common_updateReg(&rtc->DAY, (uint32_t) day << RTC_DAY_DOMLOWBCD_OFS,
        (RTC_DAY_DOMLOWBCD_MASK | RTC_DAY_DOMHIGHBCD_MASK));
}

/**
 *  @brief      Get the day of the month in BCD in the RTC calendar
 *
 *  @param[in]  rtc    Pointer to the register overlay for the RTC peripheral
 *
 *  @pre To prevent reading any counter/calendar registers while those registers
 *  are being updated, the user can call @ref DL_RTC_isSafetoRead to ensure that
 *  the registers are safe to read.
 *
 *  @return            The day of the month in the RTC calendar
 *
 *  @retval            Value in range: 0x1-0x31
 *
 */
__STATIC_INLINE uint8_t DL_RTC_getCalendarDayOfMonthBCD(RTC_Regs *rtc)
{
    return (uint8_t)(
        (rtc->DAY & (RTC_DAY_DOMLOWBCD_MASK | RTC_DAY_DOMHIGHBCD_MASK)) >>
        RTC_DAY_DOMLOWBCD_OFS);
}

/**
 *  @brief      Set the month in in BCD the RTC calendar
 *
 *  @param[in]  rtc    Pointer to the register overlay for the RTC peripheral
 *  @param[in]  month    Value for the month of the RTC calendar
 *                       Valid values are 0x1-0x12
 */
__STATIC_INLINE void DL_RTC_setCalendarMonthBCD(RTC_Regs *rtc, uint8_t month)
{
    rtc->MON = ((uint32_t) month << RTC_MON_MONLOWBCD_OFS) &
               (RTC_MON_MONLOWBCD_MASK | RTC_MON_MONHIGHBCD_MASK);
}

/**
 *  @brief      Get the month of the year in BCD in the RTC calendar
 *
 *  @param[in]  rtc    Pointer to the register overlay for the RTC peripheral
 *
 *  @pre To prevent reading any counter/calendar registers while those registers
 *  are being updated, the user can call @ref DL_RTC_isSafetoRead to ensure that
 *  the registers are safe to read.
 *
 *  @return            The month of the year in the RTC calendar
 *
 *  @retval            Value in range: 0x1-0x12
 *
 */
__STATIC_INLINE uint8_t DL_RTC_getCalendarMonthBCD(RTC_Regs *rtc)
{
    return (uint8_t)(
        (rtc->MON & (RTC_MON_MONLOWBCD_MASK | RTC_MON_MONHIGHBCD_MASK)) >>
        RTC_MON_MONLOWBCD_OFS);
}

/**
 *  @brief      Set the year in in BCD the RTC calendar
 *
 *  @param[in]  rtc    Pointer to the register overlay for the RTC peripheral
 *  @param[in]  year    Value for the year of the RTC calendar
 *                      Valid values are 0x0-0x4095
 */
__STATIC_INLINE void DL_RTC_setCalendarYearBCD(RTC_Regs *rtc, uint16_t year)
{
    rtc->YEAR = ((uint32_t) year << RTC_YEAR_YEARLOWESTBCD_OFS) &
                (RTC_YEAR_YEARLOWESTBCD_MASK | RTC_YEAR_DECADEBCD_MASK |
                    RTC_YEAR_CENTLOWBCD_MASK | RTC_YEAR_CENTHIGHBCD_MASK);
}

/**
 *  @brief      Get the year in in BCD the RTC calendar
 *
 *  @param[in]  rtc    Pointer to the register overlay for the RTC peripheral
 *
 *  @pre To prevent reading any counter/calendar registers while those registers
 *  are being updated, the user can call @ref DL_RTC_isSafetoRead to ensure that
 *  the registers are safe to read.
 *
 *  @return            The year in the RTC calendar
 *
 *  @retval            Value in range: 0x0-0x4095
 *
 */
__STATIC_INLINE uint16_t DL_RTC_getCalendarYearBCD(RTC_Regs *rtc)
{
    return (uint16_t)(
        (rtc->YEAR &
            (RTC_YEAR_YEARLOWESTBCD_MASK | RTC_YEAR_DECADEBCD_MASK |
                RTC_YEAR_CENTLOWBCD_MASK | RTC_YEAR_CENTHIGHBCD_MASK)) >>
        RTC_YEAR_YEARLOWESTBCD_OFS);
}

/**
 *  @brief      Enable minutes in BCD for RTC Calendar Alarm 1
 *
 *  @param[in]  rtc    Pointer to the register overlay for the RTC peripheral
 *
 */
__STATIC_INLINE void DL_RTC_enableAlarm1MinutesBCD(RTC_Regs *rtc)
{
    rtc->A1MIN |= RTC_A1MIN_AMINAEBCD_ENABLE;
}

/**
 *  @brief      Disable minutes in BCD for RTC Calendar Alarm 1
 *
 *  @param[in]  rtc    Pointer to the register overlay for the RTC peripheral
 *
 */
__STATIC_INLINE void DL_RTC_disableAlarm1MinutesBCD(RTC_Regs *rtc)
{
    rtc->A1MIN &= ~(RTC_A1MIN_AMINAEBCD_MASK);
}

/**
 *  @brief      Set the minutes in BCD for RTC Calendar Alarm 1
 *
 *  @param[in]  rtc    Pointer to the register overlay for the RTC peripheral
 *  @param[in]  minutes    Value for the minutes of alarm 1
 *                         Valid values are 0x0-0x59
 *
 */
__STATIC_INLINE void DL_RTC_setAlarm1MinutesBCD(RTC_Regs *rtc, uint8_t minutes)
{
    rtc->A1MIN = ((uint32_t) minutes << RTC_A1MIN_AMINLOWBCD_OFS) &
                 (RTC_A1MIN_AMINLOWBCD_MASK | RTC_A1MIN_AMINHIGHBCD_MASK);
}

/**
 *  @brief      Get the minutes in BCD for RTC Calendar Alarm 1
 *
 *  @param[in]  rtc    Pointer to the register overlay for the RTC peripheral
 *
 *  @return            The minutes of alarm 1
 *
 *  @retval            Value in range: 0x0-0x59
 *
 */
__STATIC_INLINE uint8_t DL_RTC_getAlarm1MinutesBCD(RTC_Regs *rtc)
{
    return (uint8_t)((rtc->A1MIN & (RTC_A1MIN_AMINLOWBCD_MASK |
                                       RTC_A1MIN_AMINHIGHBCD_MASK)) >>
                     RTC_A1MIN_AMINLOWBCD_OFS);
}

/**
 *  @brief      Enable hours in BCD for RTC Calendar Alarm 1
 *
 *  @param[in]  rtc    Pointer to the register overlay for the RTC peripheral
 *
 */
__STATIC_INLINE void DL_RTC_enableAlarm1HoursBCD(RTC_Regs *rtc)
{
    rtc->A1HOUR |= RTC_A1HOUR_AHOURAEBIN_ENABLE;
}

/**
 *  @brief      Disable hours in BCD for RTC Calendar Alarm 1
 *
 *  @param[in]  rtc    Pointer to the register overlay for the RTC peripheral
 *
 */
__STATIC_INLINE void DL_RTC_disableAlarm1HoursBCD(RTC_Regs *rtc)
{
    rtc->A1HOUR &= ~(RTC_A1HOUR_AHOURAEBIN_MASK);
}

/**
 *  @brief      Set the hours in BCD for RTC Calendar Alarm 1
 *
 *  @param[in]  rtc    Pointer to the register overlay for the RTC peripheral
 *  @param[in]  hours    Value for the hours of alarm 1
 *                       Valid values are 0x0-0x23
 */
__STATIC_INLINE void DL_RTC_setAlarm1HoursBCD(RTC_Regs *rtc, uint8_t hours)
{
    rtc->A1HOUR = ((uint32_t) hours << RTC_A1HOUR_AHOURLOWBCD_OFS) &
                  (RTC_A1HOUR_AHOURLOWBCD_MASK | RTC_A1HOUR_AHOURHIGHBCD_MASK);
}

/**
 *  @brief      Get the hours in BCD for RTC Calendar Alarm 1
 *
 *  @param[in]  rtc    Pointer to the register overlay for the RTC peripheral
 *
 *  @return            The hours of alarm 1
 *
 *  @retval            Value in range: 0x0-0x23
 *
 */
__STATIC_INLINE uint8_t DL_RTC_getAlarm1HoursBCD(RTC_Regs *rtc)
{
    return (uint8_t)((rtc->A1HOUR & (RTC_A1HOUR_AHOURLOWBCD_MASK |
                                        RTC_A1HOUR_AHOURHIGHBCD_MASK)) >>
                     RTC_A1HOUR_AHOURLOWBCD_OFS);
}

/**
 *  @brief      Enable day of the week in BCD for RTC Calendar Alarm 1
 *
 *  @param[in]  rtc    Pointer to the register overlay for the RTC peripheral
 *
 */
__STATIC_INLINE void DL_RTC_enableAlarm1DayOfWeekBCD(RTC_Regs *rtc)
{
    rtc->A1DAY |= RTC_A1DAY_ADOWAE_ENABLE;
}

/**
 *  @brief      Disable day of the week in BCD for RTC Calendar Alarm 1
 *
 *  @param[in]  rtc    Pointer to the register overlay for the RTC peripheral
 *
 */
__STATIC_INLINE void DL_RTC_disableAlarm1DayOfWeekBCD(RTC_Regs *rtc)
{
    rtc->A1DAY &= ~(RTC_A1DAY_ADOWAE_MASK);
}

/**
 *  @brief      Set the day of the week in BCD for RTC Calendar Alarm 1
 *
 *  @param[in]  rtc    Pointer to the register overlay for the RTC peripheral
 *  @param[in]  day    Value for the day of the week of the RTC calendar
 *                     Valid values are 0x0-0x6
 */
__STATIC_INLINE void DL_RTC_setAlarm1DayOfWeekBCD(RTC_Regs *rtc, uint8_t day)
{
    DL_Common_updateReg(&rtc->A1DAY, day, RTC_A1DAY_ADOW_MASK);
}

/**
 *  @brief      Get the day of the week in BCD for RTC Calendar Alarm 1
 *
 *  @param[in]  rtc    Pointer to the register overlay for the RTC peripheral
 *
 *  @return            The day of the week in the RTC calendar
 *
 *  @retval            Value in range: 0x0-0x6
 *
 */
__STATIC_INLINE uint8_t DL_RTC_getAlarm1DayOfWeekBCD(RTC_Regs *rtc)
{
    return (uint8_t)(rtc->DAY & RTC_A1DAY_ADOW_MASK);
}

/**
 *  @brief      Enable day of the month in BCD for RTC Calendar Alarm 1
 *
 *  @param[in]  rtc    Pointer to the register overlay for the RTC peripheral
 *
 */
__STATIC_INLINE void DL_RTC_enableAlarm1DayOfMonthBCD(RTC_Regs *rtc)
{
    rtc->A1DAY |= RTC_A1DAY_ADOMAEBCD_ENABLE;
}

/**
 *  @brief      Disable day of the month in BCD for RTC Calendar Alarm 1
 *
 *  @param[in]  rtc    Pointer to the register overlay for the RTC peripheral
 *
 */
__STATIC_INLINE void DL_RTC_disableAlarm1DayOfMonthBCD(RTC_Regs *rtc)
{
    rtc->A1DAY &= ~(RTC_A1DAY_ADOMAEBCD_MASK);
}

/**
 *  @brief      Set the day of the month in BCD for RTC Calendar Alarm 1
 *
 *  @param[in]  rtc    Pointer to the register overlay for the RTC peripheral
 *  @param[in]  day    Value for the day of the month of the RTC calendar
 *                     Valid values are 0x1-0x31
 */
__STATIC_INLINE void DL_RTC_setAlarm1DayOfMonthBCD(RTC_Regs *rtc, uint8_t day)
{
    DL_Common_updateReg(&rtc->A1DAY,
        (uint32_t) day << RTC_A1DAY_ADOMLOWBCD_OFS,
        (RTC_A1DAY_ADOMLOWBCD_MASK | RTC_A1DAY_ADOMHIGHBCD_MASK));
}

/**
 *  @brief      Get the day of the month in BCD for RTC Calendar Alarm 1
 *
 *  @param[in]  rtc    Pointer to the register overlay for the RTC peripheral
 *
 *  @return            The day of the month in the RTC calendar
 *
 *  @retval            Value in range: 0x1-0x31
 *
 */
__STATIC_INLINE uint8_t DL_RTC_getAlarm1DayOfMonthBCD(RTC_Regs *rtc)
{
    return (uint8_t)((rtc->A1DAY & (RTC_A1DAY_ADOMLOWBCD_MASK |
                                       RTC_A1DAY_ADOMHIGHBCD_MASK)) >>
                     RTC_A1DAY_ADOMLOWBCD_OFS);
}

/**
 *  @brief      Enable minutes in BCD for RTC Calendar Alarm 2
 *
 *  @param[in]  rtc    Pointer to the register overlay for the RTC peripheral
 *
 */
__STATIC_INLINE void DL_RTC_enableAlarm2MinutesBCD(RTC_Regs *rtc)
{
    rtc->A2MIN |= RTC_A2MIN_AMINAEBCD_ENABLE;
}

/**
 *  @brief      Disable minutes in BCD for RTC Calendar Alarm 2
 *
 *  @param[in]  rtc    Pointer to the register overlay for the RTC peripheral
 *
 */
__STATIC_INLINE void DL_RTC_disableAlarm2MinutesBCD(RTC_Regs *rtc)
{
    rtc->A2MIN &= ~(RTC_A2MIN_AMINAEBCD_MASK);
}

/**
 *  @brief      Set the minutes in BCD for RTC Calendar Alarm 2
 *
 *  @param[in]  rtc    Pointer to the register overlay for the RTC peripheral
 *  @param[in]  minutes    Value for the minutes of alarm 2
 *                         Valid values are 0x0-0x59
 *
 */
__STATIC_INLINE void DL_RTC_setAlarm2MinutesBCD(RTC_Regs *rtc, uint8_t minutes)
{
    rtc->A2MIN = ((uint32_t) minutes << RTC_A2MIN_AMINLOWBCD_OFS) &
                 (RTC_A2MIN_AMINLOWBCD_MASK | RTC_A2MIN_AMINHIGHBCD_MASK);
}

/**
 *  @brief      Get the minutes in BCD for RTC Calendar Alarm 2
 *
 *  @param[in]  rtc    Pointer to the register overlay for the RTC peripheral
 *
 *  @return            The minutes of alarm 2
 *
 *  @retval            Value in range: 0x0-0x59
 *
 */
__STATIC_INLINE uint8_t DL_RTC_getAlarm2MinutesBCD(RTC_Regs *rtc)
{
    return (uint8_t)((rtc->A2MIN & (RTC_A2MIN_AMINLOWBCD_MASK |
                                       RTC_A2MIN_AMINHIGHBCD_MASK)) >>
                     RTC_A2MIN_AMINLOWBCD_OFS);
}

/**
 *  @brief      Enable hours in BCD for RTC Calendar Alarm 2
 *
 *  @param[in]  rtc    Pointer to the register overlay for the RTC peripheral
 *
 */
__STATIC_INLINE void DL_RTC_enableAlarm2HoursBCD(RTC_Regs *rtc)
{
    rtc->A2HOUR |= RTC_A2HOUR_AHOURAEBIN_ENABLE;
}

/**
 *  @brief      Disable hours in BCD for RTC Calendar Alarm 2
 *
 *  @param[in]  rtc    Pointer to the register overlay for the RTC peripheral
 *
 */
__STATIC_INLINE void DL_RTC_disableAlarm2HoursBCD(RTC_Regs *rtc)
{
    rtc->A2HOUR &= ~(RTC_A2HOUR_AHOURAEBIN_MASK);
}

/**
 *  @brief      Set the hours in BCD for RTC Calendar Alarm 2
 *
 *  @param[in]  rtc    Pointer to the register overlay for the RTC peripheral
 *  @param[in]  hours    Value for the hours of alarm 2
 *                       Valid values are 0x0-0x23
 */
__STATIC_INLINE void DL_RTC_setAlarm2HoursBCD(RTC_Regs *rtc, uint8_t hours)
{
    rtc->A2HOUR = ((uint32_t) hours << RTC_A2HOUR_AHOURLOWBCD_OFS) &
                  (RTC_A2HOUR_AHOURLOWBCD_MASK | RTC_A2HOUR_AHOURHIGHBCD_MASK);
}

/**
 *  @brief      Get the hours in BCD for RTC Calendar Alarm 2
 *
 *  @param[in]  rtc    Pointer to the register overlay for the RTC peripheral
 *
 *  @return            The hours of alarm 2
 *
 *  @retval            Value in range: 0x0-0x23
 *
 */
__STATIC_INLINE uint8_t DL_RTC_getAlarm2HoursBCD(RTC_Regs *rtc)
{
    return (uint8_t)((rtc->A2HOUR & (RTC_A2HOUR_AHOURLOWBCD_MASK |
                                        RTC_A2HOUR_AHOURHIGHBCD_MASK)) >>
                     RTC_A2HOUR_AHOURLOWBCD_OFS);
}

/**
 *  @brief      Enable day of the week in BCD for RTC Calendar Alarm 2
 *
 *  @param[in]  rtc    Pointer to the register overlay for the RTC peripheral
 *
 */
__STATIC_INLINE void DL_RTC_enableAlarm2DayOfWeekBCD(RTC_Regs *rtc)
{
    rtc->A2DAY |= RTC_A2DAY_ADOWAE_ENABLE;
}

/**
 *  @brief      Disable day of the week in BCD for RTC Calendar Alarm 2
 *
 *  @param[in]  rtc    Pointer to the register overlay for the RTC peripheral
 *
 */
__STATIC_INLINE void DL_RTC_disableAlarm2DayOfWeekBCD(RTC_Regs *rtc)
{
    rtc->A2DAY &= ~(RTC_A2DAY_ADOWAE_MASK);
}

/**
 *  @brief      Set the day of the week in BCD for RTC Calendar Alarm 2
 *
 *  @param[in]  rtc    Pointer to the register overlay for the RTC peripheral
 *  @param[in]  day    Value for the day of the week of the RTC calendar
 *                     Valid values are 0x0-0x6
 */
__STATIC_INLINE void DL_RTC_setAlarm2DayOfWeekBCD(RTC_Regs *rtc, uint8_t day)
{
    DL_Common_updateReg(&rtc->A2DAY, day, RTC_A2DAY_ADOW_MASK);
}

/**
 *  @brief      Get the day of the week in BCD for RTC Calendar Alarm 2
 *
 *  @param[in]  rtc    Pointer to the register overlay for the RTC peripheral
 *
 *  @return            The day of the week in the RTC calendar
 *
 *  @retval            Value in range: 0x0-0x6
 *
 */
__STATIC_INLINE uint8_t DL_RTC_getAlarm2DayOfWeekBCD(RTC_Regs *rtc)
{
    return (uint8_t)(rtc->DAY & RTC_A2DAY_ADOW_MASK);
}

/**
 *  @brief      Enable day of the month in BCD for RTC Calendar Alarm 2
 *
 *  @param[in]  rtc    Pointer to the register overlay for the RTC peripheral
 *
 */
__STATIC_INLINE void DL_RTC_enableAlarm2DayOfMonthBCD(RTC_Regs *rtc)
{
    rtc->A2DAY |= RTC_A2DAY_ADOMAEBCD_ENABLE;
}

/**
 *  @brief      Disable day of the month in BCD for RTC Calendar Alarm 2
 *
 *  @param[in]  rtc    Pointer to the register overlay for the RTC peripheral
 *
 */
__STATIC_INLINE void DL_RTC_disableAlarm2DayOfMonthBCD(RTC_Regs *rtc)
{
    rtc->A2DAY &= ~(RTC_A2DAY_ADOMAEBCD_MASK);
}

/**
 *  @brief      Set the day of the month in BCD for RTC Calendar Alarm 2
 *
 *  @param[in]  rtc    Pointer to the register overlay for the RTC peripheral
 *  @param[in]  day    Value for the day of the month of the RTC calendar
 *                     Valid values are 0x1-0x31
 */
__STATIC_INLINE void DL_RTC_setAlarm2DayOfMonthBCD(RTC_Regs *rtc, uint8_t day)
{
    DL_Common_updateReg(&rtc->A2DAY,
        (uint32_t) day << RTC_A2DAY_ADOMLOWBCD_OFS,
        (RTC_A2DAY_ADOMLOWBCD_MASK | RTC_A2DAY_ADOMHIGHBCD_MASK));
}

/**
 *  @brief      Get the day of the month in BCD for RTC Calendar Alarm 2
 *
 *  @param[in]  rtc    Pointer to the register overlay for the RTC peripheral
 *
 *  @return            The day of the month in the RTC calendar
 *
 *  @retval            Value in range: 0x1-0x31
 *
 */
__STATIC_INLINE uint8_t DL_RTC_getAlarm2DayOfMonthBCD(RTC_Regs *rtc)
{
    return (uint8_t)((rtc->A2DAY & (RTC_A2DAY_ADOMLOWBCD_MASK |
                                       RTC_A2DAY_ADOMHIGHBCD_MASK)) >>
                     RTC_A2DAY_ADOMLOWBCD_OFS);
}

/**
 *  @brief      Set the seconds in binary the RTC calendar
 *
 *  @param[in]  rtc    Pointer to the register overlay for the RTC peripheral
 *  @param[in]  seconds    Value for the seconds of the RTC calendar
 *                         Valid values are 0-59
 */
__STATIC_INLINE void DL_RTC_setCalendarSecondsBinary(
    RTC_Regs *rtc, uint8_t seconds)
{
    rtc->SEC = (seconds & RTC_SEC_SECBIN_MASK);
}

/**
 *  @brief      Get the seconds in binary in the RTC calendar
 *
 *  @param[in]  rtc    Pointer to the register overlay for the RTC peripheral
 *
 *  @pre To prevent reading any counter/calendar registers while those registers
 *  are being updated, the user can call @ref DL_RTC_isSafetoRead to ensure that
 *  the registers are safe to read.
 *
 *  @return            The seconds in the RTC calendar
 *
 *  @retval            Value in range: 0-59
 *
 */
__STATIC_INLINE uint8_t DL_RTC_getCalendarSecondsBinary(RTC_Regs *rtc)
{
    return (uint8_t)(rtc->SEC & RTC_SEC_SECBIN_MASK);
}

/**
 *  @brief      Set the minutes in binary in the RTC calendar
 *
 *  @param[in]  rtc    Pointer to the register overlay for the RTC peripheral
 *  @param[in]  minutes    Value for the minutes of the RTC calendar
 *                         Valid values are 0-59
 */
__STATIC_INLINE void DL_RTC_setCalendarMinutesBinary(
    RTC_Regs *rtc, uint8_t minutes)
{
    rtc->MIN = (minutes & RTC_MIN_MINBIN_MASK);
}

/**
 *  @brief      Get the minutes in binary in the RTC calendar
 *
 *  @param[in]  rtc    Pointer to the register overlay for the RTC peripheral
 *
 *  @pre To prevent reading any counter/calendar registers while those registers
 *  are being updated, the user can call @ref DL_RTC_isSafetoRead to ensure that
 *  the registers are safe to read.
 *
 *  @return            The minutes in the RTC calendar
 *
 *  @retval            Value in range: 0-59
 *
 */
__STATIC_INLINE uint8_t DL_RTC_getCalendarMinutesBinary(RTC_Regs *rtc)
{
    return (uint8_t)(rtc->MIN & RTC_MIN_MINBIN_MASK);
}

/**
 *  @brief      Set the hours in binary in the RTC calendar
 *
 *  @param[in]  rtc    Pointer to the register overlay for the RTC peripheral
 *  @param[in]  hours    Value for the hours of the RTC calendar
 *                       Valid values are 0-23
 */
__STATIC_INLINE void DL_RTC_setCalendarHoursBinary(
    RTC_Regs *rtc, uint8_t hours)
{
    rtc->HOUR = (hours & RTC_HOUR_HOURBIN_MASK);
}

/**
 *  @brief      Get the hours in binary in the RTC calendar
 *
 *  @param[in]  rtc    Pointer to the register overlay for the RTC peripheral
 *
 *  @pre To prevent reading any counter/calendar registers while those registers
 *  are being updated, the user can call @ref DL_RTC_isSafetoRead to ensure that
 *  the registers are safe to read.
 *
 *  @return            The hours in the RTC calendar
 *
 *  @retval            Value in range: 0-23
 *
 */
__STATIC_INLINE uint8_t DL_RTC_getCalendarHoursBinary(RTC_Regs *rtc)
{
    return (uint8_t)(rtc->HOUR & RTC_HOUR_HOURBIN_MASK);
}

/**
 *  @brief      Set the day of the week in binary in the RTC calendar
 *
 *  @param[in]  rtc    Pointer to the register overlay for the RTC peripheral
 *  @param[in]  day    Value for the day of the week of the RTC calendar
 *                     Valid values are 0-6
 */
__STATIC_INLINE void DL_RTC_setCalendarDayOfWeekBinary(
    RTC_Regs *rtc, uint8_t day)
{
    DL_Common_updateReg(&rtc->DAY, day, RTC_DAY_DOW_MASK);
}

/**
 *  @brief      Get the day of the week in binary in the RTC calendar
 *
 *  @param[in]  rtc    Pointer to the register overlay for the RTC peripheral
 *
 *  @pre To prevent reading any counter/calendar registers while those registers
 *  are being updated, the user can call @ref DL_RTC_isSafetoRead to ensure that
 *  the registers are safe to read.
 *
 *  @return            The day of the week in the RTC calendar
 *
 *  @retval            Value in range: 0-6
 *
 */
__STATIC_INLINE uint8_t DL_RTC_getCalendarDayOfWeekBinary(RTC_Regs *rtc)
{
    return (uint8_t)(rtc->DAY & RTC_DAY_DOW_MASK);
}

/**
 *  @brief      Set the day of the month in binary in the RTC calendar
 *
 *  @param[in]  rtc    Pointer to the register overlay for the RTC peripheral
 *  @param[in]  day    Value for the day of the month of the RTC calendar
 *                     Valid values are 1-31
 */
__STATIC_INLINE void DL_RTC_setCalendarDayOfMonthBinary(
    RTC_Regs *rtc, uint8_t day)
{
    DL_Common_updateReg(
        &rtc->DAY, (uint32_t) day << RTC_DAY_DOMBIN_OFS, RTC_DAY_DOMBIN_MASK);
}

/**
 *  @brief      Get the day of the month in binary in the RTC calendar
 *
 *  @param[in]  rtc    Pointer to the register overlay for the RTC peripheral
 *
 *  @pre To prevent reading any counter/calendar registers while those registers
 *  are being updated, the user can call @ref DL_RTC_isSafetoRead to ensure that
 *  the registers are safe to read.
 *
 *  @return            The day of the month in the RTC calendar
 *
 *  @retval            Value in range: 1-31
 *
 */
__STATIC_INLINE uint8_t DL_RTC_getCalendarDayOfMonthBinary(RTC_Regs *rtc)
{
    return (uint8_t)((rtc->DAY & RTC_DAY_DOMBIN_MASK) >> RTC_DAY_DOMBIN_OFS);
}

/**
 *  @brief      Set the month in binary in the RTC calendar
 *
 *  @param[in]  rtc    Pointer to the register overlay for the RTC peripheral
 *  @param[in]  month    Value for the month of the RTC calendar
 *                       Valid values are 1-12
 */
__STATIC_INLINE void DL_RTC_setCalendarMonthBinary(
    RTC_Regs *rtc, uint8_t month)
{
    rtc->MON = (month & RTC_MON_MONBIN_MASK);
}

/**
 *  @brief      Get the month of the year in binary in the RTC calendar
 *
 *  @param[in]  rtc    Pointer to the register overlay for the RTC peripheral
 *
 *  @pre To prevent reading any counter/calendar registers while those registers
 *  are being updated, the user can call @ref DL_RTC_isSafetoRead to ensure that
 *  the registers are safe to read.
 *
 *  @return            The month of the year in the RTC calendar
 *
 *  @retval            Value in range: 1-12
 *
 */
__STATIC_INLINE uint8_t DL_RTC_getCalendarMonthBinary(RTC_Regs *rtc)
{
    return (uint8_t)(rtc->MON & RTC_MON_MONBIN_MASK);
}

/**
 *  @brief      Set the year in binary in the RTC calendar
 *
 *  @param[in]  rtc    Pointer to the register overlay for the RTC peripheral
 *  @param[in]  year    Value for the year of the RTC calendar
 *                      Valid values are 0-4095
 */
__STATIC_INLINE void DL_RTC_setCalendarYearBinary(RTC_Regs *rtc, uint16_t year)
{
    rtc->YEAR = ((uint32_t) year &
                 (RTC_YEAR_YEARLOWBIN_MASK | RTC_YEAR_YEARHIGHBIN_MASK));
}

/**
 *  @brief      Get the year in binary in the RTC calendar
 *
 *  @param[in]  rtc    Pointer to the register overlay for the RTC peripheral
 *
 *  @pre To prevent reading any counter/calendar registers while those registers
 *  are being updated, the user can call @ref DL_RTC_isSafetoRead to ensure that
 *  the registers are safe to read.
 *
 *  @return            The year in the RTC calendar
 *
 *  @retval            Value in range: 0-4095
 *
 */
__STATIC_INLINE uint16_t DL_RTC_getCalendarYearBinary(RTC_Regs *rtc)
{
    return (uint16_t)(
        rtc->YEAR & (RTC_YEAR_YEARLOWBIN_MASK | RTC_YEAR_YEARHIGHBIN_MASK));
}

/**
 *  @brief      Enable minutes in binary in binary for RTC Calendar Alarm 1
 *
 *  @param[in]  rtc    Pointer to the register overlay for the RTC peripheral
 *
 */
__STATIC_INLINE void DL_RTC_enableAlarm1MinutesBinary(RTC_Regs *rtc)
{
    rtc->A1MIN |= RTC_A1MIN_AMINAEBIN_ENABLE;
}

/**
 *  @brief      Disable minutes in binary for RTC Calendar Alarm 1
 *
 *  @param[in]  rtc    Pointer to the register overlay for the RTC peripheral
 *
 */
__STATIC_INLINE void DL_RTC_disableAlarm1MinutesBinary(RTC_Regs *rtc)
{
    rtc->A1MIN &= ~(RTC_A1MIN_AMINAEBIN_MASK);
}

/**
 *  @brief      Set the minutes in binary for RTC Calendar Alarm 1
 *
 *  @param[in]  rtc    Pointer to the register overlay for the RTC peripheral
 *  @param[in]  minutes    Value for the minutes of alarm 1
 *                         Valid values are 0-59
 *
 */
__STATIC_INLINE void DL_RTC_setAlarm1MinutesBinary(
    RTC_Regs *rtc, uint8_t minutes)
{
    rtc->A1MIN = (minutes & RTC_A1MIN_AMINBIN_MASK);
}

/**
 *  @brief      Get the minutes in binary for RTC Calendar Alarm 1
 *
 *  @param[in]  rtc    Pointer to the register overlay for the RTC peripheral
 *
 *  @return            The minutes of alarm 1
 *
 *  @retval            Value in range: 0-59
 *
 */
__STATIC_INLINE uint8_t DL_RTC_getAlarm1MinutesBinary(RTC_Regs *rtc)
{
    return (uint8_t)(rtc->A1MIN & RTC_A1MIN_AMINBIN_MASK);
}

/**
 *  @brief      Enable hours in binary for RTC Calendar Alarm 1
 *
 *  @param[in]  rtc    Pointer to the register overlay for the RTC peripheral
 *
 */
__STATIC_INLINE void DL_RTC_enableAlarm1HoursBinary(RTC_Regs *rtc)
{
    rtc->A1HOUR |= RTC_A1HOUR_AHOURAEBIN_ENABLE;
}

/**
 *  @brief      Disable hours in binary for RTC Calendar Alarm 1
 *
 *  @param[in]  rtc    Pointer to the register overlay for the RTC peripheral
 *
 */
__STATIC_INLINE void DL_RTC_disableAlarm1HoursBinary(RTC_Regs *rtc)
{
    rtc->A1HOUR &= ~(RTC_A1HOUR_AHOURAEBIN_MASK);
}

/**
 *  @brief      Set the hours in binary for RTC Calendar Alarm 1
 *
 *  @param[in]  rtc    Pointer to the register overlay for the RTC peripheral
 *  @param[in]  hours    Value for the hours of alarm 1
 *                       Valid values are 0-23
 */
__STATIC_INLINE void DL_RTC_setAlarm1HoursBinary(RTC_Regs *rtc, uint8_t hours)
{
    rtc->A1HOUR = (hours & RTC_A1HOUR_AHOURBIN_MASK);
}

/**
 *  @brief      Get the hours in binary for RTC Calendar Alarm 1
 *
 *  @param[in]  rtc    Pointer to the register overlay for the RTC peripheral
 *
 *  @return            The hours of alarm 1
 *
 *  @retval            Value in range: 0-23
 *
 */
__STATIC_INLINE uint8_t DL_RTC_getAlarm1HoursBinary(RTC_Regs *rtc)
{
    return (uint8_t)(rtc->A1HOUR & RTC_A1HOUR_AHOURBIN_MASK);
}

/**
 *  @brief      Enable day of the week in binary for RTC Calendar Alarm 1
 *
 *  @param[in]  rtc    Pointer to the register overlay for the RTC peripheral
 *
 */
__STATIC_INLINE void DL_RTC_enableAlarm1DayOfWeekBinary(RTC_Regs *rtc)
{
    rtc->A1DAY |= RTC_A1DAY_ADOWAE_ENABLE;
}

/**
 *  @brief      Disable day of the week in binary for RTC Calendar Alarm 1
 *
 *  @param[in]  rtc    Pointer to the register overlay for the RTC peripheral
 *
 */
__STATIC_INLINE void DL_RTC_disableAlarm1DayOfWeekBinary(RTC_Regs *rtc)
{
    rtc->A1DAY &= ~(RTC_A1DAY_ADOWAE_MASK);
}

/**
 *  @brief      Set the day of the week in binary for RTC Calendar Alarm 1
 *
 *  @param[in]  rtc    Pointer to the register overlay for the RTC peripheral
 *  @param[in]  day    Value for the day of the week of the RTC calendar
 *                     Valid values are 0-6
 */
__STATIC_INLINE void DL_RTC_setAlarm1DayOfWeekBinary(
    RTC_Regs *rtc, uint8_t day)
{
    DL_Common_updateReg(&rtc->A1DAY, day, RTC_A1DAY_ADOW_MASK);
}

/**
 *  @brief      Get the day of the week in binary for RTC Calendar Alarm 1
 *
 *  @param[in]  rtc    Pointer to the register overlay for the RTC peripheral
 *
 *  @return            The day of the week in the RTC calendar
 *
 *  @retval            Value in range: 0-6
 *
 */
__STATIC_INLINE uint8_t DL_RTC_getAlarm1DayOfWeekBinary(RTC_Regs *rtc)
{
    return (uint8_t)(rtc->A1DAY & RTC_A1DAY_ADOW_MASK);
}

/**
 *  @brief      Enable day of the month in binary for RTC Calendar Alarm 1
 *
 *  @param[in]  rtc    Pointer to the register overlay for the RTC peripheral
 *
 */
__STATIC_INLINE void DL_RTC_enableAlarm1DayOfMonthBinary(RTC_Regs *rtc)
{
    rtc->A1DAY |= RTC_A1DAY_ADOMAEBIN_ENABLE;
}

/**
 *  @brief      Disable day of the month in binary for RTC Calendar Alarm 1
 *
 *  @param[in]  rtc    Pointer to the register overlay for the RTC peripheral
 *
 */
__STATIC_INLINE void DL_RTC_disableAlarm1DayOfMonthBinary(RTC_Regs *rtc)
{
    rtc->A1DAY &= ~(RTC_A1DAY_ADOMAEBIN_MASK);
}

/**
 *  @brief      Set the day of the month in binary for RTC Calendar Alarm 1
 *
 *  @param[in]  rtc    Pointer to the register overlay for the RTC peripheral
 *  @param[in]  day    Value for the day of the month of the RTC calendar
 *                     Valid values are 1-31
 */
__STATIC_INLINE void DL_RTC_setAlarm1DayOfMonthBinary(
    RTC_Regs *rtc, uint8_t day)
{
    DL_Common_updateReg(&rtc->A1DAY, (uint32_t) day << RTC_A1DAY_ADOMBIN_OFS,
        RTC_A1DAY_ADOMBIN_MASK);
}

/**
 *  @brief      Get the day of the month in binary for RTC Calendar Alarm 1
 *
 *  @param[in]  rtc    Pointer to the register overlay for the RTC peripheral
 *
 *  @return            The day of the month in the RTC calendar
 *
 *  @retval            Value in range: 1-31
 *
 */
__STATIC_INLINE uint8_t DL_RTC_getAlarm1DayOfMonthBinary(RTC_Regs *rtc)
{
    return (uint8_t)(
        (rtc->A1DAY & RTC_A1DAY_ADOMBIN_MASK) >> RTC_A1DAY_ADOMBIN_OFS);
}

/**
 *  @brief      Enable minutes in binary for RTC Calendar Alarm 2
 *
 *  @param[in]  rtc    Pointer to the register overlay for the RTC peripheral
 *
 */
__STATIC_INLINE void DL_RTC_enableAlarm2MinutesBinary(RTC_Regs *rtc)
{
    rtc->A2MIN |= RTC_A2MIN_AMINAEBIN_ENABLE;
}

/**
 *  @brief      Disable minutes in binary for RTC Calendar Alarm 2
 *
 *  @param[in]  rtc    Pointer to the register overlay for the RTC peripheral
 *
 */
__STATIC_INLINE void DL_RTC_disableAlarm2MinutesBinary(RTC_Regs *rtc)
{
    rtc->A2MIN &= ~(RTC_A2MIN_AMINAEBIN_MASK);
}

/**
 *  @brief      Set the minutes in binary for RTC Calendar Alarm 2
 *
 *  @param[in]  rtc    Pointer to the register overlay for the RTC peripheral
 *  @param[in]  minutes    Value for the minutes of alarm 2
 *                         Valid values are 0-59
 *
 */
__STATIC_INLINE void DL_RTC_setAlarm2MinutesBinary(
    RTC_Regs *rtc, uint8_t minutes)
{
    rtc->A2MIN = (minutes & RTC_A2MIN_AMINBIN_MASK);
}

/**
 *  @brief      Get the minutes in binary for RTC Calendar Alarm 2
 *
 *  @param[in]  rtc    Pointer to the register overlay for the RTC peripheral
 *
 *  @return            The minutes of alarm 2
 *
 *  @retval            Value in range: 0-59
 *
 */
__STATIC_INLINE uint8_t DL_RTC_getAlarm2MinutesBinary(RTC_Regs *rtc)
{
    return (uint8_t)(rtc->A2MIN & RTC_A2MIN_AMINBIN_MASK);
}

/**
 *  @brief      Enable hours in binary for RTC Calendar Alarm 2
 *
 *  @param[in]  rtc    Pointer to the register overlay for the RTC peripheral
 *
 */
__STATIC_INLINE void DL_RTC_enableAlarm2HoursBinary(RTC_Regs *rtc)
{
    rtc->A2HOUR |= RTC_A2HOUR_AHOURAEBIN_ENABLE;
}

/**
 *  @brief      Disable hours in binary for RTC Calendar Alarm 2
 *
 *  @param[in]  rtc    Pointer to the register overlay for the RTC peripheral
 *
 */
__STATIC_INLINE void DL_RTC_disableAlarm2HoursBinary(RTC_Regs *rtc)
{
    rtc->A2HOUR &= ~(RTC_A2HOUR_AHOURAEBIN_MASK);
}

/**
 *  @brief      Set the hours in binary for RTC Calendar Alarm 2
 *
 *  @param[in]  rtc    Pointer to the register overlay for the RTC peripheral
 *  @param[in]  hours    Value for the hours of alarm 2
 *                       Valid values are 0-23
 */
__STATIC_INLINE void DL_RTC_setAlarm2HoursBinary(RTC_Regs *rtc, uint8_t hours)
{
    rtc->A2HOUR = (hours & RTC_A2HOUR_AHOURBIN_MASK);
}

/**
 *  @brief      Get the hours in binary for RTC Calendar Alarm 2
 *
 *  @param[in]  rtc    Pointer to the register overlay for the RTC peripheral
 *
 *  @return            The hours of alarm 2
 *
 *  @retval            Value in range: 0-23
 *
 */
__STATIC_INLINE uint8_t DL_RTC_getAlarm2HoursBinary(RTC_Regs *rtc)
{
    return (uint8_t)(rtc->A2HOUR & RTC_A2HOUR_AHOURBIN_MASK);
}

/**
 *  @brief      Enable day of the week in binary for RTC Calendar Alarm 2
 *
 *  @param[in]  rtc    Pointer to the register overlay for the RTC peripheral
 *
 */
__STATIC_INLINE void DL_RTC_enableAlarm2DayOfWeekBinary(RTC_Regs *rtc)
{
    rtc->A2DAY |= RTC_A2DAY_ADOWAE_ENABLE;
}

/**
 *  @brief      Disable day of the week in binary for RTC Calendar Alarm 2
 *
 *  @param[in]  rtc    Pointer to the register overlay for the RTC peripheral
 *
 */
__STATIC_INLINE void DL_RTC_disableAlarm2DayOfWeekBinary(RTC_Regs *rtc)
{
    rtc->A2DAY &= ~(RTC_A2DAY_ADOWAE_MASK);
}

/**
 *  @brief      Set the day of the week in binary for RTC Calendar Alarm 2
 *
 *  @param[in]  rtc    Pointer to the register overlay for the RTC peripheral
 *  @param[in]  day    Value for the day of the week of the RTC calendar
 *                     Valid values are 0-6
 */
__STATIC_INLINE void DL_RTC_setAlarm2DayOfWeekBinary(
    RTC_Regs *rtc, uint8_t day)
{
    DL_Common_updateReg(&rtc->A2DAY, day, RTC_A2DAY_ADOW_MASK);
}

/**
 *  @brief      Get the day of the week in binary for RTC Calendar Alarm 2
 *
 *  @param[in]  rtc    Pointer to the register overlay for the RTC peripheral
 *
 *  @return            The day of the week in the RTC calendar
 *
 *  @retval            Value in range: 0-6
 *
 */
__STATIC_INLINE uint8_t DL_RTC_getAlarm2DayOfWeekBinary(RTC_Regs *rtc)
{
    return (uint8_t)(rtc->A2DAY & RTC_A2DAY_ADOW_MASK);
}

/**
 *  @brief      Enable day of the month in binary for RTC Calendar Alarm 2
 *
 *  @param[in]  rtc    Pointer to the register overlay for the RTC peripheral
 *
 */
__STATIC_INLINE void DL_RTC_enableAlarm2DayOfMonthBinary(RTC_Regs *rtc)
{
    rtc->A2DAY |= RTC_A2DAY_ADOMAEBIN_ENABLE;
}

/**
 *  @brief      Disable day of the month in binary for RTC Calendar Alarm 2
 *
 *  @param[in]  rtc    Pointer to the register overlay for the RTC peripheral
 *
 */
__STATIC_INLINE void DL_RTC_disableAlarm2DayOfMonthBinary(RTC_Regs *rtc)
{
    rtc->A2DAY &= ~(RTC_A2DAY_ADOMAEBIN_MASK);
}

/**
 *  @brief      Set the day of the month in binary for RTC Calendar Alarm 2
 *
 *  @param[in]  rtc    Pointer to the register overlay for the RTC peripheral
 *  @param[in]  day    Value for the day of the month of the RTC calendar
 *                     Valid values are 1-31
 */
__STATIC_INLINE void DL_RTC_setAlarm2DayOfMonthBinary(
    RTC_Regs *rtc, uint8_t day)
{
    DL_Common_updateReg(&rtc->A2DAY, (uint32_t) day << RTC_A2DAY_ADOMBIN_OFS,
        RTC_A2DAY_ADOMBIN_MASK);
}

/**
 *  @brief      Get the day of the month in binary for RTC Calendar Alarm 2
 *
 *  @param[in]  rtc    Pointer to the register overlay for the RTC peripheral
 *
 *  @return            The day of the month in the RTC calendar
 *
 *  @retval            Value in range: 1-31
 *
 */
__STATIC_INLINE uint8_t DL_RTC_getAlarm2DayOfMonthBinary(RTC_Regs *rtc)
{
    return (uint8_t)(
        (rtc->A2DAY & RTC_A2DAY_ADOMBIN_MASK) >> RTC_A2DAY_ADOMBIN_OFS);
}

/**
 *  @brief      Set prescale interrupt interval for timer 0 and 1
 *
 *  Set prescale timer interrupt interval to DL_RTC_PRESCALE_NONE to keep the
 *  timer disabled.
 *
 *  @param[in]  rtc    Pointer to the register overlay for the RTC peripheral
 *
 *  @param[in]  prescaler0Divider    Value to set for prescaler 0
 *                                       interrupt interval
 *                                       One of @ref DL_RTC_PRESCALER0_DIVIDE
 *
 *  @param[in]  prescaler1Divider    Value to set for prescaler 1
 *                                       interrupt interval
 *                                       One of @ref DL_RTC_PRESCALER1_DIVIDE
 */
__STATIC_INLINE void DL_RTC_setPrescalerEvents(RTC_Regs *rtc,
    DL_RTC_PRESCALER0_DIVIDE prescaler0Divider,
    DL_RTC_PRESCALER1_DIVIDE prescaler1Divider)
{
    rtc->PSCTL = (((uint32_t) prescaler0Divider & RTC_PSCTL_RT0IP_MASK) |
                  ((uint32_t) prescaler1Divider & RTC_PSCTL_RT1IP_MASK));
}

/**
 *  @brief      Set periodic alarm 0
 *
 *  @param[in]  rtc    Pointer to the register overlay for the RTC peripheral
 *
 *  @param[in]  prescaler0Divider  Value to set for prescaler 0 interrupt
 *                                 interval. One of @ref DL_RTC_PRESCALER0_DIVIDE
 *
 */
__STATIC_INLINE void DL_RTC_setPeriodicAlarm0(
    RTC_Regs *rtc, DL_RTC_PRESCALER0_DIVIDE prescaler0Divider)
{
    DL_Common_updateReg(
        &rtc->PSCTL, (uint32_t) prescaler0Divider, RTC_PSCTL_RT0IP_MASK);
}

/**
 *  @brief      Set periodic alarm 1

 *
 *  @param[in]  rtc    Pointer to the register overlay for the RTC peripheral
 *
 *  @param[in]  prescaler1Divider  Value to set for prescaler 1 interrupt
 *                                 interval. One of @ref DL_RTC_PRESCALER1_DIVIDE
 */
__STATIC_INLINE void DL_RTC_setPeriodicAlarm1(
    RTC_Regs *rtc, DL_RTC_PRESCALER1_DIVIDE prescaler1Divider)
{
    DL_Common_updateReg(
        &rtc->PSCTL, (uint32_t) prescaler1Divider, RTC_PSCTL_RT1IP_MASK);
}

/**
 *  @brief      Get prescaler 0 interrupt interval
 *
 *  @param[in]  rtc    Pointer to the register overlay for the RTC peripheral
 *
 *  @return            Value for prescaler 0 interrupt interval
 *
 *  @retval            One of @ref DL_RTC_PRESCALER0_DIVIDE
 *
 */
__STATIC_INLINE DL_RTC_PRESCALER0_DIVIDE DL_RTC_getPrescaler0(RTC_Regs *rtc)
{
    uint32_t prescaler = (rtc->PSCTL & RTC_PSCTL_RT0IP_MASK);

    return (DL_RTC_PRESCALER0_DIVIDE)(prescaler);
}

/**
 *  @brief      Get prescaler 1 interrupt interval
 *
 *  @param[in]  rtc    Pointer to the register overlay for the RTC peripheral
 *
 *  @return            Value for prescaler 1 interrupt interval
 *
 *  @retval            One of @ref DL_RTC_PRESCALER1_DIVIDE
 */
__STATIC_INLINE DL_RTC_PRESCALER1_DIVIDE DL_RTC_getPrescaler1(RTC_Regs *rtc)
{
    uint32_t prescaler = (rtc->PSCTL & RTC_PSCTL_RT1IP_MASK);

    return (DL_RTC_PRESCALER1_DIVIDE)(prescaler);
}

/**
 *  @brief      Enable RTC interrupts
 *
 *  @param[in]  rtc    Pointer to the register overlay for the RTC peripheral
 *
 *  @param[in]  interruptMask  Bit mask of interrupts to enable. Logical OR of
 *                             @ref DL_RTC_INTERRUPT.
 */
__STATIC_INLINE void DL_RTC_enableInterrupt(
    RTC_Regs *rtc, uint32_t interruptMask)
{
    rtc->CPU_INT.IMASK |= interruptMask;
}

/**
 *  @brief      Disable RTC interrupts
 *
 *  @param[in]  rtc    Pointer to the register overlay for the RTC peripheral
 *
 *  @param[in]  interruptMask  Bit mask of interrupts to enable. Logical OR of
 *                             @ref DL_RTC_INTERRUPT
 */
__STATIC_INLINE void DL_RTC_disableInterrupt(
    RTC_Regs *rtc, uint32_t interruptMask)
{
    rtc->CPU_INT.IMASK &= ~(interruptMask);
}

/**
 *  @brief      Check which RTC interrupts are enabled
 *
 *  @param[in]  rtc    Pointer to the register overlay for the RTC peripheral
 *
 *  @param[in]  interruptMask  Bit mask of interrupts to check. Logical OR of
 *                             @ref DL_RTC_INTERRUPT
 *
 *  @return     Which of the requested RTC interrupts are enabled
 *
 *  @retval     Logical OR of @ref DL_RTC_INTERRUPT values
 */
__STATIC_INLINE uint32_t DL_RTC_getEnabledInterrupts(
    RTC_Regs *rtc, uint32_t interruptMask)
{
    return (rtc->CPU_INT.IMASK & interruptMask);
}

/**
 *  @brief      Check interrupt flag of enabled RTC interrupts
 *
 *  Checks if any of the RTC interrupts that were previously enabled are
 *  pending.
 *
 *  @param[in]  rtc    Pointer to the register overlay for the RTC peripheral
 *
 *  @param[in]  interruptMask  Bit mask of interrupts to check. Logical OR of
 *                             @ref DL_RTC_INTERRUPT
 *
 *  @return     Which of the requested RTC interrupts are pending
 *
 *  @retval     Logical OR of @ref DL_RTC_INTERRUPT values
 *
 *  @sa         DL_RTC_enableInterrupt
 */
__STATIC_INLINE uint32_t DL_RTC_getEnabledInterruptStatus(
    RTC_Regs *rtc, uint32_t interruptMask)
{
    return (rtc->CPU_INT.MIS & interruptMask);
}

/**
 *  @brief      Check interrupt flag of any RTC interrupt
 *
 *  Checks if any of the RTC interrupts are pending. Interrupts do not have to
 *  be previously enabled.
 *
 *  @param[in]  rtc    Pointer to the register overlay for the RTC peripheral
 *
 *  @param[in]  interruptMask  Bit mask of interrupts to check. Logical OR of
 *                             @ref DL_RTC_INTERRUPT
 *
 *  @return     Which of the requested RTC interrupts are pending
 *
 *  @retval     Logical OR of @ref DL_RTC_INTERRUPT values
 */
__STATIC_INLINE uint32_t DL_RTC_getRawInterruptStatus(
    RTC_Regs *rtc, uint32_t interruptMask)
{
    return (rtc->CPU_INT.RIS & interruptMask);
}

/**
 *  @brief      Get highest priority pending RTC interrupt
 *
 *  Checks if any of the RTC interrupts are pending. Interrupts do not have to
 *  be previously enabled.
 *
 *  @param[in]  rtc    Pointer to the register overlay for the RTC peripheral
 *
 *  @return     The highest priority pending RTC interrupt
 *
 *  @retval     One of @ref DL_RTC_IIDX
 */
__STATIC_INLINE DL_RTC_IIDX DL_RTC_getPendingInterrupt(RTC_Regs *rtc)
{
    return (DL_RTC_IIDX)(rtc->CPU_INT.IIDX);
}

/**
 *  @brief      Clear pending RTC interrupts
 *
 *  @param[in]  rtc    Pointer to the register overlay for the RTC peripheral
 *
 *  @param[in]  interruptMask  Bit mask of interrupts to check. Logical OR of
 *                             @ref DL_RTC_INTERRUPT
 */
__STATIC_INLINE void DL_RTC_clearInterruptStatus(
    RTC_Regs *rtc, uint32_t interruptMask)
{
    rtc->CPU_INT.ICLR = interruptMask;
}

/**
 *  @brief      Enable RTC event
 *
 *  @param[in]  rtc         Pointer to the register overlay for the peripheral
 *  @param[in]  eventMask   Bit mask of interrupts to enable. Bitwise OR of
 *                          @ref DL_RTC_EVENT.
 */
__STATIC_INLINE void DL_RTC_enableEvent(RTC_Regs *rtc, uint32_t eventMask)
{
    rtc->GEN_EVENT.IMASK |= (eventMask);
}

/**
 *  @brief      Disable RTC event
 *
 *  @param[in]  rtc         Pointer to the register overlay for the peripheral
 *  @param[in]  eventMask   Bit mask of interrupts to enable. Bitwise OR of
 *                          @ref DL_RTC_EVENT.
 */
__STATIC_INLINE void DL_RTC_disableEvent(RTC_Regs *rtc, uint32_t eventMask)
{
    rtc->GEN_EVENT.IMASK &= ~(eventMask);
}

/**
 *  @brief      Check which RTC events are enabled
 *
 *  @param[in]  rtc         Pointer to the register overlay for the peripheral
 *  @param[in]  eventMask   Bit mask of interrupts to check. Bitwise OR of
 *                          @ref DL_RTC_EVENT.
 *
 *  @return     Which of the requested RTC interrupts are enabled
 *
 *  @retval     Bitwise OR of @ref DL_RTC_EVENT values
 */
__STATIC_INLINE uint32_t DL_RTC_getEnabledEvents(
    RTC_Regs *rtc, uint32_t eventMask)
{
    return ((rtc->GEN_EVENT.IMASK) & (eventMask));
}

/**
 *  @brief      Check event flag of enabled RTC event
 *
 *  Checks if any of the RTC events that were previously enabled are
 *  pending.
 *
 *  @param[in]  rtc         Pointer to the register overlay for the peripheral
 *  @param[in]  eventMask   Bit mask of interrupts to check. Bitwise OR of
 *                          @ref DL_RTC_EVENT.
 *
 *  @return     Which of the requested RTC interrupts are pending
 *
 *  @retval     Bitwise OR of @ref DL_RTC_EVENT values
 *
 *  @sa         DL_RTC_enableInterrupt
 */
__STATIC_INLINE uint32_t DL_RTC_getEnabledEventStatus(
    RTC_Regs *rtc, uint32_t eventMask)
{
    return ((rtc->GEN_EVENT.MIS) & eventMask);
}

/**
 *  @brief      Check interrupt flag of any RTC event
 *
 *  Checks if any events are pending. Events do not have to
 *  be previously enabled.
 *
 *  @param[in]  rtc         Pointer to the register overlay for the peripheral
 *  @param[in]  eventMask   Bit mask of interrupts to check. Bitwise OR of
 *                          @ref DL_RTC_EVENT.
 *
 *  @return     Which of the requested RTC interrupts are pending
 *
 *  @retval     Bitwise OR of @ref DL_RTC_EVENT values
 */
__STATIC_INLINE uint32_t DL_RTC_getRawEventsStatus(
    RTC_Regs *rtc, uint32_t eventMask)
{
    return ((rtc->GEN_EVENT.RIS) & eventMask);
}

/**
 *  @brief      Clear pending RTC events
 *
 *  @param[in]  rtc         Pointer to the register overlay for the peripheral
 *  @param[in]  eventMask   Bit mask of interrupts to clear. Bitwise OR of
 *                          @ref DL_RTC_EVENT.
 */
__STATIC_INLINE void DL_RTC_clearEventsStatus(
    RTC_Regs *rtc, uint32_t eventMask)
{
    rtc->GEN_EVENT.ICLR |= (eventMask);
}

/**
 *  @brief Sets the event publisher channel ID
 *
 *  @param[in]  rtc     Pointer to the register overlay for the
 *                      peripheral
 *  @param[in]  chanID  Channel ID number. Valid range 0-15. If ChanID == 0
 *                      publisher is disconnected.
 */
__STATIC_INLINE void DL_RTC_setPublisherChanID(RTC_Regs *rtc, uint8_t chanID)
{
    rtc->FPUB_0 = (chanID & RTC_FPUB_0_CHANID_MAXIMUM);
}

/**
 *  @brief Gets the event publisher channel ID
 *
 *  @param[in]  rtc Pointer to the register overlay for the
 *                  peripheral
 *
 *  @return     Event publisher channel ID
 *
 */
__STATIC_INLINE uint8_t DL_RTC_getPublisherChanID(RTC_Regs *rtc)
{
    return (uint8_t)(rtc->FPUB_0 & RTC_FPUB_0_CHANID_MAXIMUM);
}

/**
 *  @brief      Initializes the settings to operate the RTC in Calendar mode.
 *
 *  @param[in]  rtc                 Pointer to the register overlay for the
 *                                  RTC peripheral
 *  @param[in]  calendarTime        Structure containing the values for the
 *                                  Calendar to be initialized to
 *  @param[in]  formatSelect        Selectable format the RTC peripheral will
 *                                  provide time in.
 *                                  @ref DL_RTC_FORMAT
 *
 */
extern void DL_RTC_initCalendar(
    RTC_Regs *rtc, DL_RTC_Calendar calendarTime, DL_RTC_FORMAT formatSelect);

/**
 *  @brief      Returns the Calendar Time
 *
 *  @param[in]  rtc                 Pointer to the register overlay for the
 *                                  RTC peripheral
 *
 * This function returns the current Calendar Time in the form of a Calendar
 * structure.
 * The time returned will be in the previously selected format.
 *
 *  @return            A Calendar structure containing the current time.
 *
 */
extern DL_RTC_Calendar DL_RTC_getCalendarTime(RTC_Regs *rtc);

/**
 *  @brief      Set calendar alarm 1
 *
 *  @param[in]  rtc                 Pointer to the register overlay for the
 *                                  RTC peripheral
 *  @param[in]  alarmTime           Structure containing the values for the
 *                                  alarm to be set to
 *
 */
extern void DL_RTC_setCalendarAlarm1(
    RTC_Regs *rtc, DL_RTC_CalendarAlarm alarmTime);

/**
 *  @brief      Get calendar alarm 1
 *
 *  @param[in]  rtc                 Pointer to the register overlay for the
 *                                  RTC peripheral
 *
 *  @return     A Alarm structure containing the set time for alarm 1
 *
 */
extern DL_RTC_CalendarAlarm DL_RTC_getCalendarAlarm1(RTC_Regs *rtc);

/**
 *  @brief      Enable calendar alarm 1
 *
 *  @param[in]  rtc                 Pointer to the register overlay for the
 *                                  RTC peripheral
 *
 */
extern void DL_RTC_enableCalendarAlarm1(RTC_Regs *rtc);

/**
 *  @brief      Disable calendar alarm 1
 *
 *  @param[in]  rtc                 Pointer to the register overlay for the
 *                                  RTC peripheral
 *
 */
extern void DL_RTC_disableCalendarAlarm1(RTC_Regs *rtc);

/**
 *  @brief      Set calendar alarm 2
 *
 *  @param[in]  rtc                 Pointer to the register overlay for the
 *                                  RTC peripheral
 *  @param[in]  alarmTime           Structure containing the values for the
 *                                  alarm to be set to
 *
 */
extern void DL_RTC_setCalendarAlarm2(
    RTC_Regs *rtc, DL_RTC_CalendarAlarm alarmTime);

/**
 *  @brief      Get calendar alarm 2
 *
 *  @param[in]  rtc                 Pointer to the register overlay for the
 *                                  RTC peripheral
 *
 *  @return     A Alarm structure containing the set time for alarm 2
 *
 */
extern DL_RTC_CalendarAlarm DL_RTC_getCalendarAlarm2(RTC_Regs *rtc);

/**
 *  @brief      Enable calendar alarm 2
 *
 *  @param[in]  rtc                 Pointer to the register overlay for the
 *                                  RTC peripheral
 *
 */
extern void DL_RTC_enableCalendarAlarm2(RTC_Regs *rtc);

/**
 *  @brief      Disable calendar alarm 2
 *
 *  @param[in]  rtc                 Pointer to the register overlay for the
 *                                  RTC peripheral
 *
 */
extern void DL_RTC_disableCalendarAlarm2(RTC_Regs *rtc);

#ifdef __cplusplus
}
#endif
#endif /* __MSPM0_HAS_RTC__ */

#endif /* ti_dl_dl_rtc__include */
/** @}*/
