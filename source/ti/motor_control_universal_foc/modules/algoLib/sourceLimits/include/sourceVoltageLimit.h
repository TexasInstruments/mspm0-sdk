/*
 * Copyright (c) 2024, Texas Instruments Incorporated
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
/*!****************************************************************************
 *  @file       sourceVoltageLimit.h
 *  @brief      Voltage limit Module
 *
 * 
 *  @anchor sourceVoltageLimit_h
 *  # Overview
 *
 *  APIs for voltage limits
 *
 *  <hr>
 ******************************************************************************/

#ifndef APPLIMITS_INCLUDE_SOURCEVOLTAGELIMIT_H_
#define APPLIMITS_INCLUDE_SOURCEVOLTAGELIMIT_H_

#include "stdint.h"

/*! @brief Source Voltage flags Structure */
typedef union
{
    /*! @brief Source Voltage  Structure */
    struct sourceVoltageLimitFlags
    {
        _Bool
        /*! Under Voltage Fault En */
            underVoltageFaultEnable: 1,
            /*! Over Volt Fault En*/
            overVoltageFaultEnable: 1,
            /*! Under Voltage Fault En */
            underVoltageFaultAutoClear: 1,
            /*! Over Voltage Fault En */
            overVoltageFaultAutoClear: 1,
            /*! Under Voltage Fault En*/
            underVoltageFaultStatus: 1,
            /*! Over Voltage Fault En */
            overVoltageFaultStatus: 1;

    }
    /*! Bit Wise Acces */
    b;
    /*! Block Access */
    uint16_t w;

}SOURCE_VOLTAGE_LIMIT_FLAGS_T;

/*! @brief Source Voltage Fault types Structure */
typedef enum
{
    /*! No Voltage Fault */
    NO_VOLTAGE_FAULT = 0,
    /*! Under Voltage */
    UNDER_VOLTAGE = 1,
    /*! Over Voltage */
    OVER_VOLTAGE = 2,

}SOURCE_VOLTAGE_FAULT_TYPES;

/*! @brief Source Voltage Structure */
typedef struct
{

    /*! Voltage Limit Inputs */
    const int32_t
    /*! DC Bus reading */
        *pVdc;

    /*! Voltage Limit parameters for processing */
    int32_t
    /*! Minimum DC Bus Voltage */
        minDCBusVoltage,
        /*! Maximum DC Bus Voltage */
        maxDCBusVoltage,
        /*! Fault Clear Threshold */
        underVoltageFaultClearThreshold,
        /*! Fault Clear Threshold */
        overVoltageFaultClearThreshold;

    int16_t
        /*! Tracks time for voltage fault */
        count,
        /*! Maximum count for voltage fault */
        countMax;

    SOURCE_VOLTAGE_LIMIT_FLAGS_T
        /*! Flags */
        flags;

    SOURCE_VOLTAGE_FAULT_TYPES
      /*! Fault Status */
        faultStatus;

}SOURCE_VOLTAGE_LIMIT_T;

/**
 * @brief     Source Voltage Limit Status Check
 * @param[in] *pSourceVoltageLimit  Pointer
 * @return    Returns the status
 */
inline SOURCE_VOLTAGE_FAULT_TYPES
            getSourceVoltageFaultStatus(SOURCE_VOLTAGE_LIMIT_T
                                                     *pSourceVoltageLimit)
{
    return pSourceVoltageLimit->faultStatus;
}
/**
 * @brief     source Voltage Limits Init
 * @param[in] *pSourceVoltageLimit Input Pointer
 */
void sourceVoltageLimitInit(SOURCE_VOLTAGE_LIMIT_T *pSourceVoltageLimit);
/**
 * @brief     source Voltage Limits Run
 * @param[in] *pSourceVoltageLimit Input Pointer
 */
void sourceVoltageLimitRun(SOURCE_VOLTAGE_LIMIT_T *pSourceVoltageLimit);

#endif /* APPLIMITS_INCLUDE_SOURCEVOLTAGELIMIT_H_ */
