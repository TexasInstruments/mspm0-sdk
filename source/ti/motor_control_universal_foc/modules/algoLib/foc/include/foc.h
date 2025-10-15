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
 *  @file       foc.h
 *  @brief      foc Module
 *
 * 
 *  @anchor foc_h
 *  # Overview
 *
 *
 *  <hr>
 ******************************************************************************/


#ifndef FOC_INCLUDE_FOC_H_
#define FOC_INCLUDE_FOC_H_

/* Include the IQmath Library */
#include <ti/iqmath/include/IQmathLib.h>

/* Include the mathacl optimized IQmath */
#include "IQmath_mathacl.h"

#include "pi.h"
#include "stdint.h"
#include "mcLib.h"
#include "rotorAlign.h"
#include "openLoop.h"
#include "closeLoop.h"
#include "estimator.h"
#include "stallDetect.h"
#include "measure.h"
#include "esmo.h"
/*! @brief Modulation Index Limit for Sine Modulation */
#define MOD_INDEX_LIMIT_SINE                        _IQ(1.0)
/*! @brief Modulation Index Limit for Over Modulation */
#define MOD_INDEX_LIMIT_OVERMODULATION              _IQ(5.0)
/*! @brief AVS Min Source current Limit */
#define AVS_MIN_SOURCE_CURRENT_PU                   _IQ(-0.25)

/*! @brief Foc Flags Structure */
typedef union
{
    /*! @brief Close Loop Flags Structure*/

    struct focFlags
    {
        _Bool
            /*! Enables Speed Reversal
                                                     * in Open Loop */
            openLoopTransitionEnable: 1,
            /*! Enables voltage mode */
            voltageModeEnable: 1,
            /*! Voltage mode set by user */
            voltageModeEnableSet: 1;
    }
    /*! Bitwise access */
    b;
    /*! Block access */
    uint16_t w;

}FOC_FLAGS_T;

/*! @brief Foc Flags Structure */
typedef enum
{
    /*! Initialize FOC Parameters */
    FOC_INIT = 0,
    /*! Align Rotor */
    FOC_ROTOR_ALIGN = 1,
    /*! Start Motor in Open Loop */
    FOC_OPEN_LOOP = 2,
    /*! Motor Running in Closed Loop */
    FOC_CLOSE_LOOP = 3,
    /*! Motor has stalled */
    FOC_STALL = 4,
    /*! FOC is stopped */
    FOC_STOP = 5,
}FOC_STATE_TYPES;

/*! @brief FOC Working Structure */
typedef struct
{
    /*! FOC Inputs */
    const MC_ABC_T
    /*! Input 3 phase current */
        *pIabc;

    const int32_t
    /*! Commutation Angle */
        *pFOCAngle,
        /*! Speed reference */
        *pVelocityReference,
        /*! DC bus voltage */
        *pVdc,
        /*! Inverse of DC bus Voltage */
        *pInvVdc;

    /*! FOC parameters for processing */
    FOC_STATE_TYPES
    /*! FOC State */
        state;

    ESTIMATOR_T
        /*! Estimator */
        estim;

    ESMO_T
        /*! ESMO Estimtator */
        esmo;
    MC_SINCOS_T
        /*! Sin and Cosine of commutation angle */
        sinCos;

    MC_ABC_T
        /*! Modulation Index */
        mabc;

    MC_SVM_T
        /*! Space Vector Modulation Structure */
        svm;

    MC_ALPHABETA_T
        /*! Stationary Frame Current */
        iAlphaBeta,
        /*! Stationary Frame Voltage */
        vAlphaBeta,
        /*! Stationary Frame Modulation Index */
        mAlphaBeta,
        /*! Stationary Frame Applied Duty Cycle */
        dAlphaBeta;

    MC_DQ_T
        /*! Rotating Frame Current */
        idq,
        /*! Rotating Frame Voltage */
        vdq,
        /*! Rotating Frame Voltage Forced */
        vdqForce,
        /*! Rotating Frame Voltage set by user */
        mdqForceSet,
        /*! Rotating Frame Modulation Index */
        mdq,
        /*! Rotating Frame Applied Duty Cycle */
        ddq;

    PI_T
        /*! D-axis current controller */
        piId,
        /*! Q-axis current controller */
        piIq;

    ROTOR_ALIGN_T
        /*! Rotor Align Parameters */
        rotorAlign;

    OPEN_LOOP_T
        /*! Open Loop Parameters */
        openLoop;

    CLOSE_LOOP_T
        /*! Close Loop Parameters */
        closeLoop;

    STALL_DETECT_T
        /*! Stall Detection Parameters */
        stallDetect;

    int32_t
        /*! Modulation Index Limit */
        modIndexLimit,
        /*! Count for 1 complete electrical cycle */
        electricalCycleCountMax;

    FOC_FLAGS_T
        /*! Flags */
        flags;

}FOC_T;

/**
 * @brief     FOC Init
 * @param[in] *pFOC Input Pointer
 */
void focInit(FOC_T *pFOC);
/**
 * @brief     FOC Run
 * @param[in] *pFOC Input Pointer
 */
void focRun(FOC_T *pFOC);
/**
 * @brief     FOC Low Priority Run
 * @param[in] *pFOC Input Pointer
 */
void focLowPriorityRun(FOC_T *pFOC);
/**
 * @brief     FOC Stall Status  check
 * @param[in] *pFOC  Pointer
 * @return    Returns the status
 */
inline _Bool getFOCStallStatusForAction(FOC_T *pFOC)
{
    return (pFOC->state == FOC_STALL);
}

/**
 * @brief     FOC Stall Report Status check
 * @param[in] *pFOC  Pointer
 * @return    Returns the status
 */
inline _Bool getFOCStallStatusForReport(FOC_T *pFOC)
{
    return (pFOC->stallDetect.status != NO_STALL);
}

/**
 * @brief     FOC Kick Back check
 * @param[in] *pFOC  Pointer
 * @return    Returns the status
 */
inline _Bool getKickBackStatus(FOC_T *pFOC)
{
    _Bool kickBackExpected;

    int32_t velocity = *(pFOC->closeLoop.pVelocity);

    kickBackExpected = (velocity > 0 && (pFOC->idq.q < 0)) ||
            (velocity < 0 && (pFOC->idq.q > 0));

    return kickBackExpected;
}

#endif /* FOC_INCLUDE_FOC_H_ */
