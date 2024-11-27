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

#include "configTables.h"

/* ISD Config Tables*/
/*! @brief Table for forward and reverse drive */
const uint16_t tbl_fwRevDrv_pMil[16]    = {50,100,150,200,250,300,350,400,450,500,550,600,700,800,900,1000};

/*! @brief Table for HiZ brake time */
const uint16_t tbl_hiZ_brk_Time_ms[16] = {10,50,100,200,300,400,500,750,1000,2000,3000,4000,5000,7500,10000,15000};

/*! @brief Table for Stall detection threshold */
const _iq tbl_StatDetectThr_pu[8] = {_IQ(0.00104),_IQ(0.003125),_IQ(0.0041),_IQ(0.0104),_IQ(0.0208),_IQ(0.03125),_IQ(0.0416),_IQ(0.0625)};

/*Closed Loop PWM Frequency Table */
const uint16_t tbl1_clPWMFreqKHz[16] = {5,10,16,20,25,32,40,48,50,64,80,80,80,80,80,80};

/* Open Loop Speed and Acceleration Slew Rates */
/*! @brief Table 1 for open loop acceleration */
const uint16_t tbl1_olAccA1A2_centiHzPerSec[8] = {1, 5, 100, 250, 500, 1000, 2500, 5000};
/*! @brief Table 2 for open loop acceleration */
const uint16_t tbl2_olAccA1A2_HzPerSec[8] = {75, 100, 250, 500, 750, 1000, 5000, 10000};

/* Close Loop Speed and Acceleration Slew Rates */
/*! @brief Table 1 for close loop acceleration and deceleration */
const uint16_t tbl1_clDecClAcc_deciHzPerSec[16] = {5,10,25,50,75,100,200,400,600,800,1000,2000,3000,4000,5000,6000};
/*! @brief Table 2 for close loop acceleration and deceleration */
const uint16_t tbl2_clDecClAcc_HzPerSec[14] = {700,800,900,1000,2000,4000,6000,8000,10000,20000,30000,40000,50000,60000};

/* Close Loop 2 */
/*! @brief Table for brake speed threshold and active spin threshold */
const uint16_t tbl_brkDutyActSPinThr_pMil[16] = {1000,900,800,700,600,500,450,400,350,300,250,200,150,100,50,25};

/* Motor StartUp1 */
/*! @brief Table for ipd clock frequency */
const uint16_t tbl_ipdClkFreq_Hz[8] = {50,100,250,500,1000,2000,5000,10000};

/*! @brief Table for align time */
const uint16_t tbl_alignTime_msec[16] ={10,50,100,200,300,400,500,750,1000,1500,2000,3000,\
                                    4000,5000,7500,10000};
                                    
/*! @brief Table for align and slow cycle start ramp rate */
const uint16_t tbl_alignSlowRampRate[16] = {1,10,50,100,150,250,500,1000,1500,2000,2500,5000,10000,20000,50000,0};

/* Motor Start Up2 */
/*! @brief Table for theta error ramp rate */
const uint16_t tbl_thetaErrRampRate_mili[8]  = {10,50,100,150,200,500,1000,2000};

/*! @brief Table for align angle */
const uint16_t tbl_alignAngle[32] = {0,10,20,30,45,60,70,80,90,110,120,135,150,160,170,180,190, \
                                     210,225,240,250,260,270,280,290,315,330,340,350,350,350,350};

/*! @brief Table for slow first cycle frequency */
const uint16_t tbl_slowFirstCycFreqPerMil[16]  = {10,20,30,50,75,100,125,150,175,200,250,300,350,400,450,500};

/* Fault Config 1 and Config2 */
/*! @brief Table for abnormal speed lock */
const uint16_t tbl_lckAbnormalSpeed_pMil[8] = {1300, 1400, 1500, 1600, 1700, 1800, 1900, 2000};

/*! @brief Table for abnormal backemf threshold */
const uint16_t tbl_AbnormalBEMFThr_pMil[8] = {10,20,30,50,80,100,120,150};

/* Internal algo 1 Params*/

/*! @brief Table for isd stop time and isd run time */
const uint16_t tbl_isdStopTime_msec[4]     = {1,5,50,100};

/*! @brief Table for isd timeout */
const uint16_t tbl_isdRunTime_msec[4]     = {100,250,500,1000};

/*! @brief Table for autohandoff minimum backemf */
const uint16_t tbl_autoHandOffMinBemf_mV[8]   = {0,50,100,250,500,1000,1250,1500};

/*! @brief Table for persistent brake current */
const uint16_t tbl_brakeCurrPersist_msec[4]   = {50,100,250,500};

/* Internal algo 2 Params*/
/*! @brief Table for close loop alow acceleration and deceleration */
const uint16_t tbl_clSlowAcc_deci[16]     = {1,10,20,30,50,100,200,300,400,500,1000,2000,5000,7500,10000,20000};

/*! @brief Table for current */
const _iq tbl_pu[32] = {_IQ(0.075),_IQ(0.080),_IQ(0.085),_IQ(0.090),_IQ(0.095),_IQ(0.100),_IQ(0.110),_IQ(0.120),   \
                        _IQ(0.130),_IQ(0.140),_IQ(0.150),_IQ(0.160),_IQ(0.170),_IQ(0.180),_IQ(0.200),_IQ(0.225),   \
                        _IQ(0.250),_IQ(0.275),_IQ(0.300),_IQ(0.350),_IQ(0.400),_IQ(0.450),_IQ(0.500),_IQ(0.550),   \
                        _IQ(0.600),_IQ(0.700),_IQ(0.750),_IQ(0.800),_IQ(0.850),_IQ(0.900),_IQ(0.950),_IQ(1.000)};

/*! @brief Table for minimum VM */
const _iq tbl_minVm_pMil[8] = {_IQ(0.0),_IQ(0.05),_IQ(0.10),_IQ(0.12),_IQ(0.15),_IQ(0.18),_IQ(0.20),_IQ(0.25)};

/*! @brief Table for maximum VM */
const _iq tbl_maxVm_pMil[8] = {_IQ(0.60),_IQ(0.65),_IQ(0.70),_IQ(0.75),_IQ(0.80),_IQ(0.85),_IQ(0.90),_IQ(1.0)};

/*! @brief Table for Modulation Limit Beyond which Flux weakening is enabled */
const _iq tbl_mSqrRef[4] = {_IQ(0.49), _IQ(0.64),_IQ(0.81), _IQ(0.902)};

/* Misc algo 1 Params*/
/*! @brief Table for IPD maximum overflow */
const uint8_t tbl_ipdMaxOverflow[4] = {5, 10, 20, 40};
