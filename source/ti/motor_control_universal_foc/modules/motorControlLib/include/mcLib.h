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
 *  @file       mcLib.h
 *  @brief      Motor Control Library Module
 *
 * 
 *  @anchor mcLib_h
 *  # Overview
 *
 *  defines motor control algorithm APIs
 *
 *  <hr>
 ******************************************************************************/

#ifndef MCLIB_H
#define MCLIB_H

#include "iqTrig.h"

/* Include the IQmath Library */
#include <ti/iqmath/include/IQmathLib.h>

/* Include the mathacl optimized IQmath */
#include "IQmath_mathacl.h"

#include "stdint.h"
#include "main.h"

#ifdef __cplusplus
extern "C" {
#endif

/*! @brief 1/3 in IQ */
#define ONE_BY_THREE        _IQ(1.0/3.0)

/*! @brief 2/3 in IQ */
#define TWO_BY_THREE        _IQ(2.0/3.0)

/*! @brief 1/sqrt(3) in IQ */
#define ONE_BY_SQRT_THREE   _IQ(0.57735026918963)

/*! @brief 2/sqrt(3) in IQ */
#define TWO_BY_SQRT_THREE   _IQmpy2(ONE_BY_SQRT_THREE)

/*! @brief sqrt(3)/2 in IQ */
#define SQRT_THREE_BY_TWO   _IQ(0.866025403784439)

/*! @brief 1/2 in IQ */
#define ONE_BY_TWO          _IQ(0.5)

/*! @brief sqrt(3) in IQ */
#define SQRT_THREE          _IQ(1.732050808)

/*! @brief ABC 3 phase structure */
typedef struct
{
    int32_t
        /*! a phase */
        a,
        /*! b phase */
        b,
        /*! c phase */
        c;
}MC_ABC_T;

/*! @brief Alpha Beta structure */
typedef struct
{
    int32_t
        /*! Alpha phase */
        alpha,
        /*! Beta phase */
        beta;
}MC_ALPHABETA_T;

/*! @brief DQ structure */
typedef struct
{
    int32_t
        /*! D axis */
        d,
        /*! Q axis */
        q;
}MC_DQ_T;

/*! @brief Sine cosine structure */
typedef struct
{
    int32_t
        /*! Sine value */
        sin,
        /*! Cosine value */
        cos;
}MC_SINCOS_T;

/*! @brief Phase types */
typedef enum
{
    /*! Invalid phase */
    PHASE_INVALID = 0,
    /*! A phase */
    PHASE_A = 1,
    /*! B phase */
    PHASE_B = 2,
    /*! C phase */
    PHASE_C = 3,
}MC_PHASE_TYPES;

/*! @brief Sector types */
typedef enum
{
    /*! Invalid sector */
    SECTOR_INVALID = 0,
    /*! Sector 1 */
    SECTOR_1 = 1,
    /*! Sector 2 */
    SECTOR_2 = 2,
    /*! Sector 3 */
    SECTOR_3 = 3,
    /*! Sector 4 */
    SECTOR_4 = 4,
    /*! Sector 5 */
    SECTOR_5 = 5,
    /*! Sector 6 */
    SECTOR_6 = 6,
}MC_SECTOR_TYPES;

/*! @brief SVM generation types */
typedef enum
{
    /*! Continuous SVM */
    SVM_CONTINUOUS = 0,
    /*! Discontinuous SVM */
    SVM_DISCONTINUOUS = 1,
}SVM_GENERATOR_TYPES;

/*! @brief SVM structure */
typedef struct
{

    MC_SECTOR_TYPES
        /*! Voltage Sector information */
        voltageSector,  
        /*! Angle Sector information */
        angleSector,
        /*! Previous Previous Voltage Sector information */
        prevPrevVoltageSector,
        /*! Previous Voltage Sector information */
        prevVoltageSector;

    MC_PHASE_TYPES
        /*! Phase that has maximum duty */
        maxDutyPhase,   
        /*! Phase that has minimum duty */
        minDutyPhase;   

    int32_t
        /*! Minimum duty */
        minDuty,
        /*! Minimum duty discontinuous */
        minDutyDisCont,
        /*! Blanking time */
        blankingTime,
        /*! Minimum PWM count delta */
        minPWMdelta;

    SVM_GENERATOR_TYPES
        /*! Space Vector Modulation Generator type */
        svmGen;         

    MC_ABC_T
        /*! Duty Cycle */
        Dabc;
    uint8_t
        /*! Sector Count */
        sectorCount;

}MC_SVM_T;

/**
 * @brief     Clark transform no zero sequence
 * @param[in]  pAlphaBeta Pointer to alpha beta structure
 * @param[in]  pABC Pointer to 3 phase structure
 */
void MC_ClarkeTransformNoZeroSeq(MC_ALPHABETA_T *pAlphaBeta,
                                     const MC_ABC_T *pABC);

/**
 * @brief     Clark transform
 * @param[in]  pAlphaBeta Pointer to alpha beta structure
 * @param[in]  pABC Pointer to 3 phase structure
 */
void MC_ClarkeTransform(MC_ALPHABETA_T *pAlphaBeta,
                                     const MC_ABC_T *pABC);

/**
 * @brief     SVM Clark transform no zero sequence
 * @param[in]  pAlphaBeta Pointer to alpha beta structure
 * @param[in]  pABC Pointer to 3 phase structure
 */
void MC_SVM_ClarkeTransform(MC_ALPHABETA_T *pAlphaBeta,
                                     const MC_ABC_T *pABC);

/**
 * @brief     Inverse Clark transform no zero sequence
 * @param[in]  pABC Pointer to 3 phase structure
 * @param[in]  pAlphaBeta Pointer to alpha beta structure
 */
void MC_InverseClarkeTransform(MC_ABC_T *pABC,
                                   const MC_ALPHABETA_T *pAlphaBeta);

/**
 * @brief     Park transform no zero sequence
 * @param[in]  pDQ Pointer to DQ structure
 * @param[in]  pAlphaBeta Pointer to alpha beta structure
 * @param[in]  pSinCos Pointer to sine cosine structure
 */
void MC_ParkTransform(MC_DQ_T *pDQ,
                           const MC_ALPHABETA_T *pAlphaBeta,
                           const MC_SINCOS_T *pSinCos);

/**
 * @brief     Inverse park transform no zero sequence
 * @param[in]  pAlphaBeta Pointer to alpha beta structure
 * @param[in]  pDQ Pointer to DQ structure
 * @param[in]  pSinCos Pointer to sine cosine structure
 */
void MC_InverseParkTransform(MC_ALPHABETA_T *pAlphaBeta,
                                  const MC_DQ_T *pDQ,
                                  const MC_SINCOS_T *pSinCos);

/**
 * @brief     Sine cosine of angle
 * @param[in]  pSinCos Pointer to sine cosine structure
 * @param[in]  angle Angle input
 */
void MC_SinCos(MC_SINCOS_T *pSinCos, const int32_t angle);

/**
 * @brief     Space vector generation
 * @param[in]  pSVM Pointer to SVM structure
 * @param[in]  pMabc Pointer to 3 phase modulation index structure
 */
void MC_SpaceVectorGen(MC_SVM_T *pSVM, MC_ABC_T *pMabc);

/**
 * @brief     Sector from sine cosine and velocity
 * @param[in]  pSVM Pointer to SVM structure
 * @param[in]  pSinCos Pointer to sine cosine structure
 * @param[in]  velocity Velocity
 */
void MC_AngleSectorGen(MC_SVM_T *pSVM,
                              const MC_SINCOS_T *pSinCos,
                              const int32_t velocity);

#endif /* MCLIB_H */
