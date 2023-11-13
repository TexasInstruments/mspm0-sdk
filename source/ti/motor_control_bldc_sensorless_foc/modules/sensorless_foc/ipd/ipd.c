/*
 * Copyright (c) 2023, Texas Instruments Incorporated
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
#include "ipd.h"

/*! @brief Radians of 30 degree in IQ 32 */
#define IPD_ANGLE_30        (357913941)

/*! @brief Radians of 9- degree in IQ 32 */
#define IPD_ANGLE_90        (1073741824)

/*! @brief Radians of 150 degree in IQ 32 */
#define IPD_ANGLE_150       (1789569706)

/*! @brief Radians of 210 degree in IQ 32 */
#define IPD_ANGLE_210       (2505397589)

/*! @brief Radians of 270 degree in IQ 32 */
#define IPD_ANGLE_270       (3221225472)

/*! @brief Radians of 330 degree in IQ 32 */
#define IPD_ANGLE_330       (3937053354)

/*! @brief IPD Angle array */
uint32_t ipdAngle[IPD_VECTOR_MAX] = { IPD_ANGLE_30, IPD_ANGLE_210, IPD_ANGLE_90,
                                   IPD_ANGLE_270, IPD_ANGLE_150, IPD_ANGLE_330};

/*! @brief Array of IPD frequencies */
uint32_t ipdFreq[FOC_IPD_FREQ_MAX] = { 50, 100, 250, 500, 1000, 2000, 4000,
                                                                   8000, 10000};

/*! @brief IPD threshold mask of 7 bits */
#define IPD_THRESHOLD_MASK              (0x7F)

void IPD_UpdateParams(IPD_Instance *handle, IPD_PARA *para)
{
    handle->ipdThr = para->ipdThr & IPD_THRESHOLD_MASK;
    handle->pulsePeriodLimit = para->sampleFreq/ipdFreq[para->ipdFreq];
}

void IPD_reset(IPD_Instance *handle)
{
    handle->ipdVector = 0;
    handle->waitCnt = 0;
    handle->minTime = 0;
    handle->minTimeVector = 0;
    handle->ipdState = IPD_INIT;
    handle->pulsePeriodCnt = 0;
}

/**
 * @brief     Check if compare value is within maximum
 * @param[in] CompVal  Compare value
 */
__STATIC_INLINE void IPD_checkCompVal(uint8_t *CompVal)
{
    if(*CompVal > IPD_MAX_COMP_VAL)
    {
        *CompVal = IPD_MAX_COMP_VAL;
    }
}

/**
 * @brief     Set IPD configurations
 * @param[in] handle A pointer to ipd instance
 * @param[in] hal    A pointer to foc hal instance
 * @param[in] offsetA 8 bit value of the A phase offset
 * @param[in] offsetB 8 bit value of the B phase offset
 * @param[in] offsetC 8 bit value of the C phase offset
 */
__STATIC_INLINE void IPD_setConfig(IPD_Instance *handle, FOC_HAL* hal,
                            uint8_t offsetA , uint8_t offsetB , uint8_t offsetC)
{
    uint8_t CompVal;
    switch(handle->ipdVector)
    {
        case IPD_VECTOR_AC:
            HAL_setCaptureSubChannel(hal->ipdTimer,
                                    HAL_getCOMPPubChannelID(hal->isenCComp));
            CompVal = offsetC - handle->ipdThr;
            IPD_checkCompVal(&CompVal);
            HAL_setCOMPDAC(hal->isenCComp, CompVal);
            HAL_setCOMPDAC(hal->isenAComp, 0);
            HAL_setCOMPDAC(hal->isenBComp, 0);
            HAL_setCaptureCount(hal->ipdTimer,
                                        HAL_getCaptureLoadValue(hal->ipdTimer));
            HAL_PWMForceHigh(hal->pwmAHal);
            HAL_PWMDisableChannel(hal->pwmBHal);
            HAL_PWMForceLow(hal->pwmCHal);
            break;
        case IPD_VECTOR_CA:
            HAL_setCaptureSubChannel(hal->ipdTimer,
                                    HAL_getCOMPPubChannelID(hal->isenAComp));
            CompVal = offsetA - handle->ipdThr;
            IPD_checkCompVal(&CompVal);
            HAL_setCOMPDAC(hal->isenAComp, CompVal);
            HAL_setCOMPDAC(hal->isenBComp, 0);
            HAL_setCOMPDAC(hal->isenCComp, 0);
            HAL_setCaptureCount(hal->ipdTimer,
                                        HAL_getCaptureLoadValue(hal->ipdTimer));
            HAL_PWMForceLow(hal->pwmAHal);
            HAL_PWMDisableChannel(hal->pwmBHal);
            HAL_PWMForceHigh(hal->pwmCHal);
            break;
        case IPD_VECTOR_BC:
            HAL_setCaptureSubChannel(hal->ipdTimer,
                                    HAL_getCOMPPubChannelID(hal->isenCComp));
            CompVal = offsetC - handle->ipdThr;
            IPD_checkCompVal(&CompVal);
            HAL_setCOMPDAC(hal->isenCComp, CompVal);
            HAL_setCOMPDAC(hal->isenBComp, 0);
            HAL_setCOMPDAC(hal->isenAComp, 0);
            HAL_setCaptureCount(hal->ipdTimer,
                                        HAL_getCaptureLoadValue(hal->ipdTimer));
            HAL_PWMDisableChannel(hal->pwmAHal);
            HAL_PWMForceHigh(hal->pwmBHal);
            HAL_PWMForceLow(hal->pwmCHal);
            break;
        case IPD_VECTOR_CB:
            HAL_setCaptureSubChannel(hal->ipdTimer,
                                    HAL_getCOMPPubChannelID(hal->isenBComp));
            CompVal = offsetB - handle->ipdThr;
            IPD_checkCompVal(&CompVal);
            HAL_setCOMPDAC(hal->isenBComp, CompVal);
            HAL_setCOMPDAC(hal->isenAComp, 0);
            HAL_setCOMPDAC(hal->isenCComp, 0);
            HAL_setCaptureCount(hal->ipdTimer,
                                        HAL_getCaptureLoadValue(hal->ipdTimer));
            HAL_PWMDisableChannel(hal->pwmAHal);
            HAL_PWMForceLow(hal->pwmBHal);
            HAL_PWMForceHigh(hal->pwmCHal);
            break;
        case IPD_VECTOR_BA:
            HAL_setCaptureSubChannel(hal->ipdTimer,
                                    HAL_getCOMPPubChannelID(hal->isenAComp));
            CompVal = offsetA - handle->ipdThr;
            IPD_checkCompVal(&CompVal);
            HAL_setCOMPDAC(hal->isenAComp, offsetA - handle->ipdThr);
            HAL_setCOMPDAC(hal->isenBComp, 0);
            HAL_setCOMPDAC(hal->isenCComp, 0);
            HAL_setCaptureCount(hal->ipdTimer,
                                        HAL_getCaptureLoadValue(hal->ipdTimer));
            HAL_PWMForceLow(hal->pwmAHal);
            HAL_PWMForceHigh(hal->pwmBHal);
            HAL_PWMDisableChannel(hal->pwmCHal);
            break;
        case IPD_VECTOR_AB:
            HAL_setCaptureSubChannel(hal->ipdTimer,
                                    HAL_getCOMPPubChannelID(hal->isenBComp));
            CompVal = offsetB - handle->ipdThr;
            IPD_checkCompVal(&CompVal);
            HAL_setCOMPDAC(hal->isenBComp, CompVal);
            HAL_setCOMPDAC(hal->isenAComp, 0);
            HAL_setCOMPDAC(hal->isenCComp, 0);
            HAL_setCaptureCount(hal->ipdTimer,
                                        HAL_getCaptureLoadValue(hal->ipdTimer));
            HAL_PWMForceHigh(hal->pwmAHal);
            HAL_PWMForceLow(hal->pwmBHal);
            HAL_PWMDisableChannel(hal->pwmCHal);
            break;
        default:
            /* This is expected to be empty */
            break;
    }
}

void IPD_run(IPD_Instance *handle, FOC_HAL *hal, 
                            uint8_t offsetA , uint8_t offsetB , uint8_t offsetC)
{
    uint32_t oppVector;
    switch(handle->ipdState)
    {
        case IPD_INIT:
            /* IPD threshold should be minimum 1 */
            if(handle->ipdThr == 0)
            {
                handle->ipdThr = 1;
            }
            handle->ipdState = IPD_CONFIG;
            break;
        case IPD_CONFIG:
            handle->waitCnt = 0;
            HAL_clearCaptureInterruptStatus(hal->ipdTimer);
            HAL_enableCaptureInterrupt(hal->ipdTimer);
            /* set the ipd configuration */
            IPD_setConfig(handle, hal, offsetA , offsetB , offsetC );
            handle->ipdState = IPD_WAIT;
            break;
        case IPD_WAIT:
            handle->waitCnt++;
            break;
        case IPD_PERIOD_WAIT:
            handle->pulsePeriodCnt++;
            if(handle->pulsePeriodCnt >= handle->pulsePeriodLimit)
            {
                handle->pulsePeriodCnt = 0;
                handle->ipdVector++;
                if(handle->ipdVector >= IPD_VECTOR_MAX)
                {
                    handle->ipdState = IPD_COMPUTE;
                }
                else{
                    handle->ipdState = IPD_CONFIG;
                }
            }
            break;
        case IPD_COMPUTE:
            handle->minTime = handle->ipdT[0];
            handle->minTimeVector = 0;
            for(int8_t vector = 0;vector < IPD_VECTOR_MAX;vector++)
            {
                if( handle->ipdT[vector] < handle->minTime)
                {
                    handle->minTime = handle->ipdT[vector];
                    handle->minTimeVector = vector;
                }
            }
            /* finding the opposite vector */
            if(handle->minTimeVector & 1 )
            {
                oppVector = handle->minTimeVector - 1;
            }
            else
            {
                oppVector = handle->minTimeVector + 1;
            }
            handle->deltaT = handle->ipdT[oppVector] 
                                          - handle->ipdT[handle->minTimeVector];
            handle->outAngle = ipdAngle[handle->minTimeVector];
            handle->ipdState = IPD_CMPLT;
            break;
        case IPD_CMPLT:
            /* This is expected to be empty */
            break;
        default:
            /* This is expected to be empty */
            break;
    }
}

void IPD_ISR(IPD_Instance *handle, FOC_HAL *hal)
{
    if(handle->ipdState == IPD_WAIT)
    {
            HAL_PWMDisableChannel(hal->pwmAHal);
            HAL_PWMDisableChannel(hal->pwmBHal);
            HAL_PWMDisableChannel(hal->pwmCHal);
            HAL_disableCaptureInterrupt(hal->ipdTimer);
            handle->ipdT[handle->ipdVector] =  
                                        (HAL_getCaptureLoadValue(hal->ipdTimer)
                                        -HAL_getCaptureValue(hal->ipdTimer));
            handle->ipdState = IPD_PERIOD_WAIT;
    }
}
