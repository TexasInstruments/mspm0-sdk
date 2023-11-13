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
#include <string.h>

#if __ENABLE_LOG
/** @brief Array used for logging foc variables */
int32_t ccsLog[FOC_LOG_IDX_MAX][FOC_LOG_BUF_SIZE];

/** @brief Pointer to the next index in the log array */
uint16_t ccsLogPtr[FOC_LOG_IDX_MAX]={0};
#endif

/*! @brief 31 bit mask for speed reference */
#define FOC_SPEEDREF_MASK                   (0x7FFFFFFF)

/** @brief Create a ipd instance */
IPD_Instance ipd;

/** @brief Create an estimator instance */
EST_Instance est = EST_DEFAULTS;

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

/** @brief Create a phaseVolt instance */
PHASEVOLT_Instance phaseVolt = PHASEVOLT_DEFAULTS;

/** @brief Create a Space Vector PWM modulator instance */
SVGEN_Instance svgen = SVGEN_DEFAULTS;

/** @brief Create a pwmgen instance */
PWMGEN_Instance pwmgen = PWMGEN_DEFAULTS;

/** @brief Counter for offset calibration */
uint32_t offsetCalibcnt = 0;

/** @brief Calibration sum of phase A */
uint32_t calibSumA = 0;

/** @brief Calibration sum of phase B */
uint32_t calibSumB = 0;

/** @brief Calibration sum of phase C */
uint32_t calibSumC = 0;

/** @brief Temp variable used for storing angle */
uint32_t pAngle;

/** @brief Counter for userVar update */
uint8_t userVarUpdateCounter = 0;

/**
 * @brief Stores the pwm frequency
 */
uint32_t pwmFreq[FOC_PWM_FREQ_MAX] = {10000, 15000, 20000, 25000, 30000, 35000,
                                      40000, 45000, 50000};

/**
 * @brief Stores the pwm frequency prescaler to be used with repeat counter
 */
uint8_t pwmFreqPreScale[FOC_PWM_FREQ_MAX] = {0, 0, 1, 1, 1, 2, 2, 2, 3};

/**
 * @brief Stores the sampling frequency
 */
uint32_t samplingFreq[FOC_PWM_FREQ_MAX] = {10000, 15000, 10000, 12500, 15000,
                                           11667, 10000, 15000, 12500};
/**
 * @brief Stores the Sample time in seconds for each pwm frequency
 */
_iq samplingTime[FOC_PWM_FREQ_MAX] = {_IQ(0.0001), _IQ(0.0000666666),
                                      _IQ(0.0001), _IQ(0.00008),
                                      _IQ(0.00006666666), _IQ(0.00008571428),
                                      _IQ(0.000075), _IQ(0.00006666666),
                                      _IQ(0.00008)};

/** @brief Pointer that gets rolled over at end of user parameters */
PARA_IDX paraUpdatePtr = PARA_IDX_USER_RS;

void FOC_init(FOC_Instance *handle)
{
  HAL_PWMDisableChannel(handle->hal.pwmAHal);
  HAL_PWMDisableChannel(handle->hal.pwmBHal);
  HAL_PWMDisableChannel(handle->hal.pwmCHal);

  /* use repeat counter to set event rate division */
  HAL_supressCompEvent(handle->hal.adcTrigHal);

  /* Set the hardware scale factors for voltage and current */
  handle->voltageSF = _IQ(USER_DEFAULT_VOLTAGE_SF);
  handle->currentSF = _IQ(USER_DEFAULT_CURRENT_SF);

  /* Set the ADC trigger compare value */
  handle->adcTrigVal = USER_DEFAULT_FOC_PWMADCSAMPLE;

  /* Set the adc trigger point */
  FOC_setADCTrig(handle);

  /* Clear fault when starting */
  FOC_clearFault(handle);

  /* Init flash vars and set the init value for the foc vars */
  FOC_initFLASH();
  FOC_initUserParamsFoc(&userParamsFoc, &userVar.userParams, sizeof(userVar.userParams) - FOC_WRITE_FLAG_SIZE_BYTES - FOC_CRC_SIZE_BYTES);

}

void FOC_writeFLASH(USER_PARAMS *params, uint32_t size)
{
  uint32_t crcCheckSum;

  /* Erase Reserved Flash sector */
  HAL_unprotectFLASH(FLASH_PARAMS_BASE_ADDRESS);
  HAL_eraseFLASH(FLASH_PARAMS_BASE_ADDRESS);

  /* Set the write flag signature */
  params->writeFlag = FOC_WRITE_FLAG_SIGNATURE;

  /* Set the CRC of Motor Configuration */
  params->crc = HAL_getCRC(FOC_CRC_SEED, params, size - FOC_WRITE_FLAG_SIZE_BYTES - FOC_CRC_SIZE_BYTES);

  /* Write Motor Configuration in Reserved Flash sector */
  HAL_copyMemoryBlock((uint32_t *)FLASH_PARAMS_BASE_ADDRESS, params,
                      size,(uint32_t)FLASH_PARAMS_BASE_ADDRESS);

  /* Verify CRC */
  crcCheckSum = HAL_getCRC(FOC_CRC_SEED, (void *)&savedParams, sizeof(savedParams) - FOC_WRITE_FLAG_SIZE_BYTES - FOC_CRC_SIZE_BYTES);
  if(crcCheckSum != params->crc)
  {
      /* CRC not matching, Failed to write crc to flash, code for error handling*/
      __BKPT(0);
  }
}

void FOC_initUserParamsFoc(USER_PARAMS *pUserParamsFoc, USER_PARAMS *pUserParams, uint32_t size)
{
  size >>= BYTES_TO_WORD_RSHIFT;
  uint32_t *destPtr = (uint32_t *)pUserParamsFoc;
  uint32_t *srcPtr = (uint32_t *)pUserParams;

  for(uint32_t offset = 0; offset < size; offset++)
  {
    *(destPtr + offset) = *(srcPtr + offset) + 1;
  }
}

void FOC_loadSavedParams(USER_PARAMS *dstParams, const USER_PARAMS *srcParams, uint32_t size)
{
  memcpy(dstParams, srcParams, size);
}

void FOC_initFLASH(void)
{
  uint32_t crcCheckSum;
  if(savedParams.writeFlag != FOC_WRITE_FLAG_SIGNATURE)
  {
    /* Load the defaultParams to userParams */
    FOC_loadSavedParams(&userVar.userParams, &defaultParams, sizeof(defaultParams));

    /* Add CRC, write flags and write to flash */
    FOC_writeFLASH(&userVar.userParams, sizeof(userVar.userParams));
  }

  /* Load the saved parameters */
  FOC_loadSavedParams(&userVar.userParams, &savedParams, sizeof(savedParams));

  /* Verify CRC */
  crcCheckSum = HAL_getCRC(FOC_CRC_SEED, &userVar.userParams, sizeof(userVar.userParams) - FOC_WRITE_FLAG_SIZE_BYTES - FOC_CRC_SIZE_BYTES);
  if(crcCheckSum != userVar.userParams.crc)
  {
    /* CRC not matching , code for error handling*/
    __BKPT(0);
  }
}

void FOC_paramsUpdateProcess(FOC_Instance *handle, USER_VAR *userVar)
{
  USER_PARAMS *pUserParamsFoc = &userParamsFoc;
  if(handle->runUserVarUpdate)
  {
    /* Update all the control variables */
    if(userVar->controlRegs.cntrl1.enableMotor)
    {
      handle->cmd.motorState = MOTOR_STATE_START;
    }
    else
    {
      handle->cmd.motorState = MOTOR_STATE_STOP;
    }
    handle->cmd.speedRef = userVar->controlRegs.speedRef & FOC_SPEEDREF_MASK;
    handle->cmd.IdRef = userVar->controlRegs.idRef;
    handle->cmd.IqRef = userVar->controlRegs.iqRef;
    handle->cmd.disCL = (FOC_DISCL) userVar->controlRegs.cntrl1.disCL;

    /* Flash operations done only if the motor is in standby or fault state */
    if(handle->state == MOTOR_STANDBY || handle->state == MOTOR_FAULT)
    {
      if((userVar->controlRegs.cntrl1.flashRead)
      &&  (userVar->controlRegs.cntrl1.flashAccessKey == FOC_FLASH_ACCESSKEY))
      {
        userVar->controlRegs.cntrl1.flashRead = 0;
        FOC_loadSavedParams(&userVar->userParams, &savedParams,
                                                            sizeof(savedParams));
        FOC_initUserParamsFoc(&userParamsFoc, &userVar->userParams, 
                          sizeof(userVar->userParams) - FOC_WRITE_FLAG_SIZE_BYTES
                          - FOC_CRC_SIZE_BYTES);
        /* Clearing the flash access key */
        userVar->controlRegs.cntrl1.flashAccessKey = 0;
      }

      if((userVar->controlRegs.cntrl1.flashWrite)
      &&  (userVar->controlRegs.cntrl1.flashAccessKey == FOC_FLASH_ACCESSKEY))
      {
        userVar->controlRegs.cntrl1.flashWrite = 0;
        FOC_writeFLASH(&userVar->userParams, sizeof(userVar->userParams));
        /* Clearing the flash access key */
        userVar->controlRegs.cntrl1.flashAccessKey = 0;
      }

      if((userVar->controlRegs.cntrl1.defaultRead)
      &&  (userVar->controlRegs.cntrl1.flashAccessKey == FOC_FLASH_ACCESSKEY))
      {
        userVar->controlRegs.cntrl1.defaultRead = 0;
        FOC_loadSavedParams(&userVar->userParams, &defaultParams,
                                                          sizeof(defaultParams));
        FOC_initUserParamsFoc(&userParamsFoc, &userVar->userParams, 
                          sizeof(userVar->userParams) - FOC_WRITE_FLAG_SIZE_BYTES
                          - FOC_CRC_SIZE_BYTES);
        /* Clearing the flash access key */
        userVar->controlRegs.cntrl1.flashAccessKey = 0;
      }
    }

    if(userVar->controlRegs.cntrl1.clearFault)
    {
        if(handle->faultStatus.all)
      {
        FOC_clearFault(handle);
      }
      userVar->controlRegs.cntrl1.clearFault = 0;
    }

    /* Update the status registers */
    userVar->statusRegs.faultStatus.all = handle->faultStatus.all;
    userVar->statusRegs.vdc = handle->vdc;
    userVar->statusRegs.motorState = handle->state;
    userVar->statusRegs.speedFbk = est.freq;
    userVar->statusRegs.speedRef = piSpd.ref;
    userVar->statusRegs.iqCurr = piIq.fbk;
    userVar->statusRegs.iqCurrRef = piIq.ref;
    userVar->statusRegs.idCurr = piId.fbk;
    userVar->statusRegs.idCurrRef = piId.ref;

    /* Update the parameter in the pointer */
    if( (*((uint32_t*)&userVar->userParams + paraUpdatePtr)) !=
                                  (*((uint32_t*)pUserParamsFoc + paraUpdatePtr)))
    {
      FOC_setPara(handle, paraUpdatePtr, 
                              *((uint32_t*)&userVar->userParams + paraUpdatePtr));
      *((uint32_t*)pUserParamsFoc + paraUpdatePtr) = 
                              *((uint32_t*)&userVar->userParams + paraUpdatePtr);
    }
    
    /* Parameter pointer incremented and rolls over if end of user params */
    paraUpdatePtr++;
    if(paraUpdatePtr >= PARA_IDX_USER_MAX)
    {
      paraUpdatePtr = PARA_IDX_USER_RS;
      handle->runUserVarUpdate = 0;
      handle->userVarsLoadCmplt = 1;
    }
  }
}

void FOC_setPara(FOC_Instance *handle, PARA_IDX paraIdx, int32_t value)
{
  /* Update the parameter */
  handle->parameter[paraIdx] = value;

  switch (paraIdx)
  {
    case PARA_IDX_USER_PWMFREQ:
      if(handle->parameter[PARA_IDX_USER_PWMFREQ] < FOC_PWM_FREQ_MAX)
      {
        handle->parameter[PARA_IDX_SYS_SAMPLE_FREQ] = 
                         samplingFreq[handle->parameter[PARA_IDX_USER_PWMFREQ]];
        
        HAL_setPWMFreq(handle->hal.pwmAHal,
                   (uint32_t)pwmFreq[handle->parameter[PARA_IDX_USER_PWMFREQ]]);
        handle->parameter[PARA_IDX_SYS_PWMPERIOD]
                                         = HAL_getPWMLoadValue(handle->hal.pwmAHal);
        handle->parameter[PARA_IDX_SYS_TS] = 
                         samplingTime[handle->parameter[PARA_IDX_USER_PWMFREQ]];

        HAL_setRepeatCounter(handle->hal.pwmAHal, 
                     pwmFreqPreScale[handle->parameter[PARA_IDX_USER_PWMFREQ]]);
      }
      break;
    
    case PARA_IDX_USER_PISPD_DIV:
      handle->PISpdExecDivider = handle->parameter[PARA_IDX_USER_PISPD_DIV];
      break;

    case PARA_IDX_USER_DEADBAND:
      handle->parameter[PARA_IDX_SYS_DEADBAND_CYC] =
      HAL_setDeadband(handle->hal.pwmAHal, 
                           (uint16_t)handle->parameter[PARA_IDX_USER_DEADBAND]);
      break;
    case PARA_IDX_USER_STARTUP_METHOD:
      handle->control.startupMethod = 
                                handle->parameter[PARA_IDX_USER_STARTUP_METHOD];
      break;
    case PARA_IDX_USER_SLOW_FIRST_CYC_FREQ:
      handle->control.slowFirstCycFreq = 
                           handle->parameter[PARA_IDX_USER_SLOW_FIRST_CYC_FREQ];
      break;
    case PARA_IDX_USER_RAMPUP_CURRENT:
      handle->control.rampup.rampupCurr =
                                handle->parameter[PARA_IDX_USER_RAMPUP_CURRENT];
      break;

    case PARA_IDX_USER_RAMPUP_RATE:
      handle->control.rampup.rate = 
      _IQmpy_rts(handle->parameter[PARA_IDX_USER_RAMPUP_RATE]
                            ,_IQdiv_rts(_IQ(1), _IQ(FOC_CONTROL_LOOP_FREQ_HZ)));
      break;

    case PARA_IDX_USER_RAMPUP_TARGET:
      handle->control.rampup.target = 
                                 handle->parameter[PARA_IDX_USER_RAMPUP_TARGET];
      break;

    case PARA_IDX_USER_ALIGN_TIME:
      handle->control.align.alignTime = 
       _IQmpy_rts(handle->parameter[PARA_IDX_USER_ALIGN_TIME],
                                                      FOC_CONTROL_LOOP_FREQ_HZ);
      break;

    case PARA_IDX_USER_ALIGN_CURRENT:
      handle->control.align.alignCurr = 
                                 handle->parameter[PARA_IDX_USER_ALIGN_CURRENT];
      break;
    
    case PARA_IDX_USER_AMP_GAIN:
      if(handle->parameter[PARA_IDX_USER_AMP_GAIN]< FOC_CSA_GAIN_MAX)
      {
#ifdef USER_DEFAULT_ENABLE_GAIN_CHANGE
      DRV_setCsaGain((DRV_HANDLE *)handle->drv_handle, 
                       (DRV_CSA_GAIN)handle->parameter[PARA_IDX_USER_AMP_GAIN]);
#endif
          handle->csaDiv = handle->parameter[PARA_IDX_USER_AMP_GAIN];
      }
      break;
    case PARA_IDX_USER_OUTER_LOOP:
      handle->cmd.outerLoop = handle->parameter[PARA_IDX_USER_OUTER_LOOP];
      break;
    case PARA_IDX_USER_DIRECTION_REVERSE:
      handle->cmd.directionReversal = 
                             handle->parameter[PARA_IDX_USER_DIRECTION_REVERSE];
      break;
    case PARA_IDX_USER_OVER_VOLTAGE_LIMIT:
      handle->overVoltageLimit = 
                            handle->parameter[PARA_IDX_USER_OVER_VOLTAGE_LIMIT];
      break;
    case PARA_IDX_USER_UNDER_VOLTAGE_LIMIT:
      handle->underVoltageLimit = 
                           handle->parameter[PARA_IDX_USER_UNDER_VOLTAGE_LIMIT];
      break;
    case PARA_IDX_USER_OVER_CURRENT_LIMIT:
      handle->overCurrentLimit = 
                            handle->parameter[PARA_IDX_USER_OVER_CURRENT_LIMIT];
      break;
    default:
      /* This is expected to be empty */
      break;
  }

  /* Call module updates */
  PARA_checkEST(&handle->parameter[0], &est, paraIdx);
  PARA_checkPISPD(&handle->parameter[0], &piSpd ,paraIdx);
  PARA_checkPIIQ(&handle->parameter[0], &piIq ,paraIdx);
  PARA_checkPIID(&handle->parameter[0], &piId ,paraIdx);
  PARA_checkRC(&handle->parameter[0], &rc ,paraIdx);
  PARA_checkRG(&handle->parameter[0], &rg ,paraIdx);
  PARA_checkPWMGEN(&handle->parameter[0], &pwmgen ,paraIdx);
  PARA_checkIPD(&handle->parameter[0], &ipd ,paraIdx);
}

void FOC_setFault(FOC_Instance *handle)
{
  handle->enablePWM = 0;
  HAL_PWMDisableChannel(handle->hal.pwmAHal);
  HAL_PWMDisableChannel(handle->hal.pwmBHal);
  HAL_PWMDisableChannel(handle->hal.pwmCHal);
  FOC_setPwmEnableStatus(handle, 0);
  handle->state = MOTOR_FAULT;
  
  /* set motor to stop and also update the userVar*/
  handle->cmd.motorState = MOTOR_STATE_STOP;
  userVar.controlRegs.cntrl1.enableMotor = 0;
}

void FOC_clearFault(FOC_Instance *handle)
{
#ifdef USER_DEFAULT_ENABLE_FAULT_INPUT_CHECK
  /* check the external fault input */
  FAULT_INPUT_CLEAR((DRV_HANDLE *)handle->drv_handle);
#endif

  handle->faultStatus.all = 0;
}

