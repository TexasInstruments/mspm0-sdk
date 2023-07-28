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
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include <ti/driverlib/dl_rtc.h>

#ifdef __MSPM0_HAS_RTC__

void DL_RTC_initCalendar(
    RTC_Regs *rtc, DL_RTC_Calendar calendarTime, DL_RTC_FORMAT formatSelect)
{
    DL_RTC_setClockFormat(rtc, formatSelect);

    if (formatSelect == DL_RTC_FORMAT_BINARY) {
        DL_RTC_setCalendarSecondsBinary(rtc, calendarTime.seconds);
        DL_RTC_setCalendarMinutesBinary(rtc, calendarTime.minutes);
        DL_RTC_setCalendarHoursBinary(rtc, calendarTime.hours);
        /*
         * Back to back writes to counter/calendar registers such as
         * SEC, MIN, HOUR, DAY, MON, YEAR need to be avoided since writes
         * to calendar registers take 2 to 3 RTCCLK cycles to take effect.
         */
        DL_Common_updateReg(&rtc->DAY,
            (uint32_t) calendarTime.dayOfWeek |
                ((uint32_t) calendarTime.dayOfMonth << RTC_DAY_DOMBIN_OFS),
            RTC_DAY_DOW_MASK | RTC_DAY_DOMBIN_MASK);
        DL_RTC_setCalendarMonthBinary(rtc, calendarTime.month);
        DL_RTC_setCalendarYearBinary(rtc, calendarTime.year);
    } else {
        DL_RTC_setCalendarSecondsBCD(rtc, calendarTime.seconds);
        DL_RTC_setCalendarMinutesBCD(rtc, calendarTime.minutes);
        DL_RTC_setCalendarHoursBCD(rtc, calendarTime.hours);
        DL_Common_updateReg(&rtc->DAY,
            (uint32_t) calendarTime.dayOfWeek |
                ((uint32_t) calendarTime.dayOfMonth << RTC_DAY_DOMLOWBCD_OFS),
            RTC_DAY_DOW_MASK | RTC_DAY_DOMLOWBCD_MASK |
                RTC_DAY_DOMHIGHBCD_MASK);
        DL_RTC_setCalendarMonthBCD(rtc, calendarTime.month);
        DL_RTC_setCalendarYearBCD(rtc, calendarTime.year);
    }
}

DL_RTC_Calendar DL_RTC_getCalendarTime(RTC_Regs *rtc)
{
    DL_RTC_Calendar calendarTime;
    DL_RTC_FORMAT format;

    format = DL_RTC_getClockFormat(rtc);

    if (format == DL_RTC_FORMAT_BINARY) {
        calendarTime.seconds    = DL_RTC_getCalendarSecondsBinary(rtc);
        calendarTime.minutes    = DL_RTC_getCalendarMinutesBinary(rtc);
        calendarTime.hours      = DL_RTC_getCalendarHoursBinary(rtc);
        calendarTime.dayOfWeek  = DL_RTC_getCalendarDayOfWeekBinary(rtc);
        calendarTime.dayOfMonth = DL_RTC_getCalendarDayOfMonthBinary(rtc);
        calendarTime.month      = DL_RTC_getCalendarMonthBinary(rtc);
        calendarTime.year       = DL_RTC_getCalendarYearBinary(rtc);
    } else {
        calendarTime.seconds    = DL_RTC_getCalendarSecondsBCD(rtc);
        calendarTime.minutes    = DL_RTC_getCalendarMinutesBCD(rtc);
        calendarTime.hours      = DL_RTC_getCalendarHoursBCD(rtc);
        calendarTime.dayOfWeek  = DL_RTC_getCalendarDayOfWeekBCD(rtc);
        calendarTime.dayOfMonth = DL_RTC_getCalendarDayOfMonthBCD(rtc);
        calendarTime.month      = DL_RTC_getCalendarMonthBCD(rtc);
        calendarTime.year       = DL_RTC_getCalendarYearBCD(rtc);
    }

    return calendarTime;
}

void DL_RTC_setCalendarAlarm1(RTC_Regs *rtc, DL_RTC_CalendarAlarm alarmTime)
{
    DL_RTC_FORMAT format;

    format = DL_RTC_getClockFormat(rtc);

    if (format == DL_RTC_FORMAT_BINARY) {
        DL_RTC_setAlarm1MinutesBinary(rtc, alarmTime.minutes);
        DL_RTC_setAlarm1HoursBinary(rtc, alarmTime.hours);
        DL_RTC_setAlarm1DayOfWeekBinary(rtc, alarmTime.dayOfWeek);
        DL_RTC_setAlarm1DayOfMonthBinary(rtc, alarmTime.dayOfMonth);
    } else {
        DL_RTC_setAlarm1MinutesBCD(rtc, alarmTime.minutes);
        DL_RTC_setAlarm1HoursBCD(rtc, alarmTime.hours);
        DL_RTC_setAlarm1DayOfWeekBCD(rtc, alarmTime.dayOfWeek);
        DL_RTC_setAlarm1DayOfMonthBCD(rtc, alarmTime.dayOfMonth);
    }
}

DL_RTC_CalendarAlarm DL_RTC_getCalendarAlarm1(RTC_Regs *rtc)
{
    DL_RTC_CalendarAlarm alarmTime;
    DL_RTC_FORMAT format;

    format = DL_RTC_getClockFormat(rtc);

    if (format == DL_RTC_FORMAT_BINARY) {
        alarmTime.minutes    = DL_RTC_getAlarm1MinutesBinary(rtc);
        alarmTime.hours      = DL_RTC_getAlarm1HoursBinary(rtc);
        alarmTime.dayOfWeek  = DL_RTC_getAlarm1DayOfWeekBinary(rtc);
        alarmTime.dayOfMonth = DL_RTC_getAlarm1DayOfMonthBinary(rtc);
    } else {
        alarmTime.minutes    = DL_RTC_getAlarm1MinutesBCD(rtc);
        alarmTime.hours      = DL_RTC_getAlarm1HoursBCD(rtc);
        alarmTime.dayOfWeek  = DL_RTC_getAlarm1DayOfWeekBCD(rtc);
        alarmTime.dayOfMonth = DL_RTC_getAlarm1DayOfMonthBCD(rtc);
    }

    return alarmTime;
}

void DL_RTC_enableCalendarAlarm1(RTC_Regs *rtc)
{
    DL_RTC_FORMAT format;

    format = DL_RTC_getClockFormat(rtc);

    if (format == DL_RTC_FORMAT_BINARY) {
        DL_RTC_enableAlarm1MinutesBinary(rtc);
        DL_RTC_enableAlarm1HoursBinary(rtc);
        DL_RTC_enableAlarm1DayOfWeekBinary(rtc);
        DL_RTC_enableAlarm1DayOfMonthBinary(rtc);
    } else {
        DL_RTC_enableAlarm1MinutesBCD(rtc);
        DL_RTC_enableAlarm1HoursBCD(rtc);
        DL_RTC_enableAlarm1DayOfWeekBCD(rtc);
        DL_RTC_enableAlarm1DayOfMonthBCD(rtc);
    }
}

void DL_RTC_disableCalendarAlarm1(RTC_Regs *rtc)
{
    DL_RTC_FORMAT format;

    format = DL_RTC_getClockFormat(rtc);

    if (format == DL_RTC_FORMAT_BINARY) {
        DL_RTC_disableAlarm1MinutesBinary(rtc);
        DL_RTC_disableAlarm1HoursBinary(rtc);
        DL_RTC_disableAlarm1DayOfWeekBinary(rtc);
        DL_RTC_disableAlarm1DayOfMonthBinary(rtc);
    } else {
        DL_RTC_disableAlarm1MinutesBCD(rtc);
        DL_RTC_disableAlarm1HoursBCD(rtc);
        DL_RTC_disableAlarm1DayOfWeekBCD(rtc);
        DL_RTC_disableAlarm1DayOfMonthBCD(rtc);
    }
}

void DL_RTC_setCalendarAlarm2(RTC_Regs *rtc, DL_RTC_CalendarAlarm alarmTime)
{
    DL_RTC_FORMAT format;

    format = DL_RTC_getClockFormat(rtc);

    if (format == DL_RTC_FORMAT_BINARY) {
        DL_RTC_setAlarm2MinutesBinary(rtc, alarmTime.minutes);
        DL_RTC_setAlarm2HoursBinary(rtc, alarmTime.hours);
        DL_RTC_setAlarm2DayOfWeekBinary(rtc, alarmTime.dayOfWeek);
        DL_RTC_setAlarm2DayOfMonthBinary(rtc, alarmTime.dayOfMonth);
    } else {
        DL_RTC_setAlarm2MinutesBCD(rtc, alarmTime.minutes);
        DL_RTC_setAlarm2HoursBCD(rtc, alarmTime.hours);
        DL_RTC_setAlarm2DayOfWeekBCD(rtc, alarmTime.dayOfWeek);
        DL_RTC_setAlarm2DayOfMonthBCD(rtc, alarmTime.dayOfMonth);
    }
}

DL_RTC_CalendarAlarm DL_RTC_getCalendarAlarm2(RTC_Regs *rtc)
{
    DL_RTC_CalendarAlarm alarmTime;
    DL_RTC_FORMAT format;

    format = DL_RTC_getClockFormat(rtc);

    if (format == DL_RTC_FORMAT_BINARY) {
        alarmTime.minutes    = DL_RTC_getAlarm2MinutesBinary(rtc);
        alarmTime.hours      = DL_RTC_getAlarm2HoursBinary(rtc);
        alarmTime.dayOfWeek  = DL_RTC_getAlarm2DayOfWeekBinary(rtc);
        alarmTime.dayOfMonth = DL_RTC_getAlarm2DayOfMonthBinary(rtc);
    } else {
        alarmTime.minutes    = DL_RTC_getAlarm2MinutesBCD(rtc);
        alarmTime.hours      = DL_RTC_getAlarm2HoursBCD(rtc);
        alarmTime.dayOfWeek  = DL_RTC_getAlarm2DayOfWeekBCD(rtc);
        alarmTime.dayOfMonth = DL_RTC_getAlarm2DayOfMonthBCD(rtc);
    }

    return alarmTime;
}

void DL_RTC_enableCalendarAlarm2(RTC_Regs *rtc)
{
    DL_RTC_FORMAT format;

    format = DL_RTC_getClockFormat(rtc);

    if (format == DL_RTC_FORMAT_BINARY) {
        DL_RTC_enableAlarm2MinutesBinary(rtc);
        DL_RTC_enableAlarm2HoursBinary(rtc);
        DL_RTC_enableAlarm2DayOfWeekBinary(rtc);
        DL_RTC_enableAlarm2DayOfMonthBinary(rtc);
    } else {
        DL_RTC_enableAlarm2MinutesBCD(rtc);
        DL_RTC_enableAlarm2HoursBCD(rtc);
        DL_RTC_enableAlarm2DayOfWeekBCD(rtc);
        DL_RTC_enableAlarm2DayOfMonthBCD(rtc);
    }
}

void DL_RTC_disableCalendarAlarm2(RTC_Regs *rtc)
{
    DL_RTC_FORMAT format;

    format = DL_RTC_getClockFormat(rtc);

    if (format == DL_RTC_FORMAT_BINARY) {
        DL_RTC_disableAlarm2MinutesBinary(rtc);
        DL_RTC_disableAlarm2HoursBinary(rtc);
        DL_RTC_disableAlarm2DayOfWeekBinary(rtc);
        DL_RTC_disableAlarm2DayOfMonthBinary(rtc);
    } else {
        DL_RTC_disableAlarm2MinutesBCD(rtc);
        DL_RTC_disableAlarm2HoursBCD(rtc);
        DL_RTC_disableAlarm2DayOfWeekBCD(rtc);
        DL_RTC_disableAlarm2DayOfMonthBCD(rtc);
    }
}

#endif /* __MSPM0_HAS_RTC__ */
