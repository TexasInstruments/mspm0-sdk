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
 *  @file       rotorAlign.h
 *  @brief      rotorAlign Module
 *
 * 
 *  @anchor rotorAlign_h
 *  # Overview
 *
 *
 *  <hr>
 ******************************************************************************/


#ifndef FOC_INCLUDE_ROTORALIGN_H_
#define FOC_INCLUDE_ROTORALIGN_H_

#include "stdint.h"

/*! @brief Second align offset */
#define SECOND_ALIGN_ANGLE_OFFSET  _IQ(90.0/360.0)

/*! @brief Rotor align flags union */
typedef union
{
    /*! @brief Rotor align flags */
    struct rotorAlignFlags
    {
        _Bool
            /*! Disables exit for align */
            forceEnable: 1,
            /*! Force align completion status */
            forceAlignStatus: 1,
            /*! Enable Align on Stop */
            enableOnStop: 1,
            /*! Enable Align on Brake */
            enableOnBrake: 1,
            /*! Enable Align on Brake set by user */
            enableOnBrakeSet: 1,
            /*! Enable Align on Start */
            enableOnStart: 1,
            /*! Select Commutation Angle for Align */
            alignAngleSel: 1,
            /*! Completion status */
            status: 1;
    }
    /*! Bitwise access */
    b;
    /*! Block access */
    uint16_t w;

}ROTOR_ALIGN_FLAGS_T;

/*! @brief Rotor align enums */
typedef enum
{
    /*! Initialize Align */
    ALIGN_INIT = 0,

    /*! First align in progress */
    FIRST_ALIGN = 1,

}ROTOR_ALIGN_STATE_TYPES;

/*! @brief Rotor align structure */
typedef struct
{
    /* Rotor Align Inputs */
    const int32_t
        /*! Target Speed */
        *pTargetVelocity;

    /* Rotor Align parameters for processing */

    int32_t
        /*! D-axis Current Reference */
        idRef,
        /*! Q-axis Current Reference */
        iqRef,
        /*! Maximum Q-axis Current Reference */
        iqRefMax,
        /*! Slew rate for Q-axis current reference */
        iqRefSlewRate,
        /*! First Align D-axis current set by user */
        iqRefSet,
        /*! Align Angle set by user */
        angleSet,
        /*! Align Angle */
        angle;

    uint16_t
        /*! Counter to track align time */
        count,
        /*! Maximum count value for align time */
        countMax,
        /*! Maximum count value for align on start */
        countMaxStart,
        /*! Maximum count value for align on stop */
        countMaxStop;

    ROTOR_ALIGN_STATE_TYPES
        /*! rotor align state */
        state;

    ROTOR_ALIGN_FLAGS_T
        /*! Flags */
        flags;


}ROTOR_ALIGN_T;

/**
 * @brief     Rotor align init
 * @param[in] *pAlign Pointer
 */
void rotorAlignInit(ROTOR_ALIGN_T *pAlign);

/**
 * @brief     Rotor align run
 * @param[in] *pAlign Pointer
 */
void rotorAlignRun(ROTOR_ALIGN_T *pAlign);

/**
 * @brief     Rotor align Status
 * @param[in] *pAlign  Pointer
 * @return    Returns the status
 */
inline _Bool getRotorAlignStatus(ROTOR_ALIGN_T *pAlign)
{
    return pAlign->flags.b.status;
}

/**
 * @brief     Rotor force align Status
 * @param[in] *pAlign  Pointer
 * @return    Returns the status
 */
inline _Bool getRotorForceAlignStatus(ROTOR_ALIGN_T *pAlign)
{
    return pAlign->flags.b.forceAlignStatus;
}

#endif /* FOC_INCLUDE_ROTORALIGN_H_ */
