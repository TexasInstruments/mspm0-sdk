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
#include "foc.h"

/** @brief Stores Vd input used in Build level 1 and 2 */
_iq VdTesting = _IQ(0.0);

/** @brief Stores Vd input used in Build level 1 and 2 */
_iq VqTesting = _IQ(0.05);

/** @brief Stores Id reference used in align mode or lsw = 0 */
_iq idRef = _IQ(0.10);

/** @brief Stores Id reference used in rampup mode or lsw = 1 */
_iq iqRef = _IQ(0.10);

/** @brief Stores speed reference reference used in lsw = 2 */
_iq  speedRef = _IQ(0.10);

/** @brief Stores the state of the switch to control the FOC control modes
 * lsw =  0 is align state
 * lsw =  1 is rampup state
 * lsw =  2 is closed loop state
 * */
uint16_t lsw = 0;

#if __ENABLE_LOG
/** @brief Array used for logging foc variables */
int32_t ccsLog[FOC_LOG_IDX_MAX][FOC_LOG_BUF_SIZE];

/** @brief Pointer to the next index in the log array */
uint16_t ccsLogPtr[FOC_LOG_IDX_MAX]={0};
#endif

/** @brief Enables the foc motor control */
bool enableFOC = false;

/** @brief Used to enable the PWM when FOC is enabled */
bool enablePWM = false;

/** @brief Counter variable used to have a smooth transition to closed loop */
_iq startCnt;

/** @brief Create an esmo instance */
ESMO_Instance esmo = ESMO_DEFAULTS;

/** @brief Create a speed estimator instance */
SPDEST_Instance speedEst = SPDEST_DEFAULTS;

/** @brief Create a Ramp control instance for speed ramping */
RMPCNTL_Instance rc = RMPCNTL_DEFAULTS;

/** @brief Create a Ramp control instance for generating the open loop angle */
RAMPGEN_Instance rg = RAMPGEN_DEFAULTS;

/** @brief Create a clarke instance */
CLARKE_Instance clarke = CLARKE_DEFAULTS;

/** @brief Create a park instance */
PARK_Instance park = PARK_DEFAULTS;

/** @brief Create a inverse park instance */
IPARK_Instance ipark = IPARK_DEFAULTS;

/** @brief Create a PI speed instance */
PI_Instance piSpd = PI_DEFAULTS;

/** @brief Create a PI Id instance */
PI_Instance piId  = PI_DEFAULTS;

/** @brief Create a PI Iq instance */
PI_Instance piIq  = PI_DEFAULTS;

/** @brief Create a Circle limiting instance */
CIRLMT_Instance cirlimit = CIRLMT_DEFAULTS;

/** @brief Create a phaseVolt instance */
PHASEVOLT_Instance phaseVolt = PHASEVOLT_DEFAULTS;

/** @brief Create a Space Vector PWM modulator instance */
SVGEN_Instance svgen = SVGEN_DEFAULTS;

/** @brief Create a pwmgen instance */
PWMGEN_Instance pwmgen = PWMGEN_DEFAULTS;

void FOC_init(FOC_Instance *handle)
{
  HAL_setPWMCcValue(handle->pwmAHal, pwmgen.HalfPerMax<<1);
  HAL_setPWMCcValue(handle->pwmBHal, pwmgen.HalfPerMax<<1);
  HAL_setPWMCcValue(handle->pwmCHal, pwmgen.HalfPerMax<<1);
}

void FOC_setPara(FOC_Instance *handle, PARA_IDX paraIdx, float value)
{
  /* Update the parameter */
  handle->parameter[paraIdx] = value;

  switch (paraIdx)
  {
    case PARA_IDX_PWMFREQ:
      HAL_setPWMFreq(handle->pwmAHal, 
                               (uint32_t)handle->parameter[PARA_IDX_PWMFREQ]);
      handle->parameter[PARA_IDX_PWMPERIOD] 
                                         = HAL_getPWMLoadValue(handle->pwmAHal);
      break;
    
    case PARA_IDX_PISPD_DIV:
      handle->PISpdExecDivider = 
                                (uint16_t)handle->parameter[PARA_IDX_PISPD_DIV]; 
      break;

    case PARA_IDX_DEADBAND:
      handle->parameter[PARA_IDX_DEADBAND_CYC] = 
      HAL_setDeadband(handle->pwmAHal, 
                                (uint16_t)handle->parameter[PARA_IDX_DEADBAND]);
      break;

    default:
      /* This is expected to be empty */
      break;
  }

  /* Call module updates */
  PARA_checkESMO(&handle->parameter[0], &esmo, paraIdx);
  PARA_checkPISPD(&handle->parameter[0], &piSpd ,paraIdx);
  PARA_checkPIIQ(&handle->parameter[0], &piIq ,paraIdx);
  PARA_checkPIID(&handle->parameter[0], &piId ,paraIdx);
  PARA_checkRG(&handle->parameter[0], &rg ,paraIdx);
  PARA_checkPWMGEN(&handle->parameter[0], &pwmgen ,paraIdx);
  PARA_checkSPDEST(&handle->parameter[0], &speedEst ,paraIdx);
}

void FOC_setFault(FOC_Instance *handle)
{
  enableFOC = false;
  lsw = 0;
  HAL_PWMDisableChannel(handle->pwmAHal);
  HAL_PWMDisableChannel(handle->pwmBHal);
  HAL_PWMDisableChannel(handle->pwmCHal);
}

void FOC_clearFault(FOC_Instance *handle)
{
  lsw = 0;
  HAL_setPWMCcValue(handle->pwmAHal, pwmgen.HalfPerMax<<1);
  HAL_setPWMCcValue(handle->pwmBHal, pwmgen.HalfPerMax<<1);
  HAL_setPWMCcValue(handle->pwmCHal, pwmgen.HalfPerMax<<1);
  HAL_PWMEnableChannel(handle->pwmAHal);
  HAL_PWMEnableChannel(handle->pwmBHal);
  HAL_PWMEnableChannel(handle->pwmCHal);
}
