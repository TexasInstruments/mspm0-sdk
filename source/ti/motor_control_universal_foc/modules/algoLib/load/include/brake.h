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
 *  @file       brake.h
 *  @brief      brake Module
 *
 * 
 *  @anchor brake_h
 *  # Overview
 *
 *
 *  <hr>
 ******************************************************************************/


#ifndef ALGOLIB_LOAD_INCLUDE_BRAKE_H_
#define ALGOLIB_LOAD_INCLUDE_BRAKE_H_

#include "stdint.h"
#include "mcLib.h"
#include "measure.h"

/*! @brief Brake Flags Working Structure */
typedef union
{
    /*! @brief Brake Flags Structure */
    struct brakeFlags
    {
         _Bool
        /*! Flag to indicate if brake is at start or stop of the motor */
           volatile brakeOnStart: 1,
          /*! Enables Brake on start */
            enableOnStart: 1,
           /*! Enables Brake on stop */
            enableOnStop: 1,
           /*! Enables Brake on fault */
            enableOnFault: 1,
           /*! Check Brake Complete on phase currents */
            brakeCheckOnCurrent: 1,
           /*! Check Brake Complete on currents on start */
            brakeCheckCurrentStart: 1,
           /*! Check Brake Complete on currents on stop */
            brakeCheckCurrentStop: 1,
           /*! Check Brake Complete on currents on fault */
            brakeCheckCurrentFault: 1,
           /*! Status of Brake based on phase currents */
            statusCurrent: 1,
           /*! Status of Brake based on time out */
            statusTimeOut: 1;
    }
    /*! Bitwise access */
    b;
    /*! Block access */
    uint16_t w;

}BRAKE_FLAGS_T;

/*! @brief Brake variable Working Structure */

typedef struct
{
   /*! Brake Inputs */
    const int32_t
   /*! Three phase currents */
        *pIMagSqr;

    int32_t
   /*! Square of Current magnitude threshold */
        currentThresholdSqr;

    uint16_t
   /*! Counter to track motor is stopped or not */
        motorStopCount,
       /*! Maximum count to declare motor is stopped */
        motorStopCountMax,
       /*! Timeout counter */
        timeOutCount,
       /*! Maximum count to declare time out */
        timeOutCountMax,
       /*! Maximum count to declare time out: startup */
        timeOutCountMaxOnStart,
       /*! Maximum count to declare time out: stop */
        timeOutCountMaxOnStop;

    BRAKE_FLAGS_T
   /*! Flags */
        flags;

}BRAKE_T;

/**
 * @brief     Brake  Init
 * @param[in] *pBrake Input Pointer
 */
void brakeInit(BRAKE_T *pBrake);
/**
 * @brief     Brake Run
 * @param[in] *pBrake Input Pointer
 */
void brakeRun(BRAKE_T *pBrake);
/**
 * @brief     Brake Low Priority Run
 * @param[in] *pBrake Input Pointer
 */
void brakeLowPriorityRun(BRAKE_T *pBrake);

/**
 * @brief     Brake Current Status check
 * @param[in] *pBrake  Pointer
 * @return    Returns the status
 */
inline _Bool getBrakeStatusCurrent(BRAKE_T *pBrake)
{
    return (pBrake->flags.b.statusCurrent || pBrake->flags.b.statusTimeOut);
}

/**
 * @brief     Brake Timout Status check
 * @param[in] *pBrake  Pointer
 * @return    Returns the status
 */
inline _Bool getBrakeStatusTimeOut(BRAKE_T *pBrake)
{
    return pBrake->flags.b.statusTimeOut;
}

#endif /* ALGOLIB_LOAD_INCLUDE_BRAKE_H_ */
