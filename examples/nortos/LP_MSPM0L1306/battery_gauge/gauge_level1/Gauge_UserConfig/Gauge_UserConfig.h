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

#ifndef GAUGECONFIG_GAUGECONFIG_H_
#define GAUGECONFIG_GAUGECONFIG_H_

#include "Gauge_Algorithm/Gauge_Type.h"

/* clang-format off */
//******************************Algorithm detection mode selection*********************************************//
//#define DETECTION_MODE  (FLASH_DATA_INPUT)
//#define DETECTION_MODE  (COMMUNICATION_DATA_INPUT)
#define DETECTION_MODE  (DETECTION_DATA_INPUT)

//******************************Algorithm data output mode selection*********************************************//
//#define OUTPUT_MODE     (NO_OUTPUT)
#define OUTPUT_MODE     (UART_OUTPUT)

//**********************************Customized configuration settings********************************************//
#define VCELL_FACTOR                                                            (3.2)    //Voltage detection resistor divider (22k+10k)/10k
#define NTC_PAIR_RESISTOR_VALUE                                                 (10000)  //10k
#define ADC_AVERAGE_TIMES                                                       (4)
#define ADC_SAMPLE_TIME                                                         (10)     //Max is 1023, only affect ADC sampling time, ADC conversion time is fixed
#define CIRCUIT_TABLE_LENGTH                                                    (33)      //OCV-SOC-Rcell table

//*****************************************System settings*******************************************************//
#define INTERNAL_REF                                                            (1400)   //1400mV
#define SYSTEM_TIK_SHIFT                                                        (2)      //As the least sys clock is 500ms, we multiply it with 2
/* clang-format on */

extern tGaugeApplication gaugeApp;
extern tBattGlobalParams battGlobalParams;
extern tBattInfoInput battInfoInput;
extern tBattParamsConfig battParamsCfg;
extern tBattCircuitParams circuitParamsTable[CIRCUIT_TABLE_LENGTH];
extern tVGaugeFilterParams vGaugeParams;
extern tOutputParams outputParams;
extern tInputParams inputRarams;
extern tWarningFlags warningFlags;

#endif /* GAUGECONFIG_GAUGECONFIG_H_ */
