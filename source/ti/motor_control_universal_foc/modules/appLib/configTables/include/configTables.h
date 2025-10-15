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
 *  @file       configTables.h
 *  @brief      Application services HAL Module
 *
 * 
 *  @anchor configTables
 *  # Overview
 *
 *  defines the config tables
 *
 *  <hr>
 ******************************************************************************/


#ifndef CONFIGTABLES_H
#define CONFIGTABLES_H

#include "stdint.h"

/* Include the IQmath Library */
#include <ti/iqmath/include/IQmathLib.h>

#ifdef __cplusplus
extern "C" {
#endif

/*! @brief Full scale current factor */
#define FULL_SCALE_CURRENT_FACTOR   1200.0/32768.0

/* ISD Config Tables */

/*! @brief Table for forward and reverse drive */
extern const uint16_t tbl_fwRevDrv_pMil[16];

/*! @brief Table for HiZ brake time */
extern const uint16_t tbl_hiZ_brk_Time_ms[16];

/*! @brief Table for Stall detection threshold */
extern const _iq tbl_StatDetectThr_pu[8];

/*! @brief Table for reverse drive handoff threshold % of Max Speed */
extern const uint16_t tbl_revDrvHandOffThr_pMil[16];

/* Open Loop Speed and Acceleration Slew Rates */
/*! @brief Table 1 for open loop acceleration */
extern const uint16_t tbl1_olAccA1A2_centiHzPerSec[8];
/*! @brief Table 2 for open loop acceleration */
extern const uint16_t tbl2_olAccA1A2_HzPerSec[8];

/*! @brief Table for PWM Frequency Selection */
extern const uint16_t tbl1_clPWMFreqKHz[16];
/* Close Loop Speed and Acceleration Slew Rates */
/*! @brief Table 1 for close loop acceleration and deceleration */
extern const uint16_t tbl1_clDecClAcc_deciHzPerSec[16];
/*! @brief Table 2 for close loop acceleration and deceleration */
extern const uint16_t tbl2_clDecClAcc_HzPerSec[14];

/* Close Loop 2 */
/*! @brief Table for brake speed threshold and active spin threshold */
extern const uint16_t tbl_brkDutyActSPinThr_pMil[16];

/* Motor StartUp1 */
/*! @brief Table for ipd clock frequency */
extern const uint16_t tbl_ipdClkFreq_Hz[8];

/*! @brief Table for align time */
extern const uint16_t tbl_alignTime_msec[16];

/*! @brief Table for align and slow cycle start ramp rate */
extern const uint16_t tbl_alignSlowRampRate[16];

/* Motor Start Up2 */
/*! @brief Table for theta error ramp rate */
extern const uint16_t tbl_thetaErrRampRate_mili[8];

/*! @brief Table for align angle */
extern const uint16_t tbl_alignAngle[32];

/*! @brief Table for slow first cycle frequency */
extern const uint16_t tbl_slowFirstCycFreqPerMil[16];

/* Fault Config 1 and Config2 */
/*! @brief Table for abnormal speed lock */
extern const uint16_t tbl_lckAbnormalSpeed_pMil[8];

/*! @brief Table for abnormal backemf threshold */
extern const uint16_t tbl_AbnormalBEMFThr_pMil[8];

/*! @brief Table for minimum VM */
extern const _iq tbl_minVm_pMil[8];

/*! @brief Table for maximum VM */
extern const _iq tbl_maxVm_pMil[8];

/* Internal algo 1 Params*/
/*! @brief Table for isd stop time and isd run time */
extern const uint16_t tbl_isdStopTime_msec[4];

/*! @brief Table for isd timeout */
extern const uint16_t tbl_isdRunTime_msec[4];

/*! @brief Table for autohandoff minimum backemf */
extern const uint16_t tbl_autoHandOffMinBemf_mV[8];

/*! @brief Table for persistent brake current */
extern const uint16_t tbl_brakeCurrPersist_msec[4];

/*! @brief Table for avs reverse drive open loop deceleration */
extern const uint16_t tbl_avsRevDrvOLDec_pMil[8];

/* Internal algo 2 Params*/
/*! @brief Table for close loop alow acceleration and deceleration */
extern const uint16_t tbl_clSlowAcc_deci[16];

/*! @brief Table for current */
extern const _iq tbl_pu[32];

/*! @brief Table for Modulation Limit Beyond which Flux weakening is enabled */
extern const _iq tbl_mSqrRef[4];

/* Misc algo 1 Params*/
/*! @brief Table for IPD maximum overflow */
extern const uint8_t tbl_ipdMaxOverflow[4];

#ifdef __cplusplus
}
#endif
#endif /* CONFIGTABLES_H */
