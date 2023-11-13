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

//*****************************************************************************
// the includes
//*****************************************************************************
#include "tida-010251.h"

//*****************************************************************************
//global variables
//*****************************************************************************
/** @brief Structure to store the GUI values */
GUI_Params        guiVar;

/** @brief Structure to store the firmware values */
GUI_Params        firmVar;

/** @brief Structure to store the fault flags*/
GUI_faults        fault;

/** @brief Structure to store voltage and current readings  */
GUI_readings      reading;

/** @brief Flag to start monitoring if motor stops */
MOTOR_STATE       motorState = GUI_MOTOR_RUN;

//*****************************************************************************
//local variables
//*****************************************************************************
/** @brief Stores the consecutive faults */
uint16_t faultCounter = 0;

/** @brief Stores the acceleration count */
uint16_t accCount = 0;

/** @brief Stores the acceleration rate  */
uint16_t accValue;

/** @brief Stores the measured module voltage   */
_iq20 moduleVoltage;

/** @brief Stores the measured module voltage   */
_iq20 voltagePhaseA;

/** @brief Stores the measured module voltage   */
_iq20 voltagePhaseB;

/** @brief Stores the measured module voltage   */
_iq20 voltagePhaseC;

/** @brief Stores the measured phase A current  */
_iq20 moduleCurrent;

/** @brief Stores the GUI over voltage limit    */
uint32_t guiOverVoltageLimit  = GUI_DEFAULT_OVER_VOLTAGE_LIMIT;

/** @brief Stores the GUI under voltage limit   */
uint32_t guiUnderVoltageLimit = GUI_DEFAULT_UNDER_VOLTAGE_LIMIT;

/** @brief Stores the GUI over current limit    */
uint32_t guiOverCurrentLimit  = GUI_DEFAULT_OVER_CURRENT_LIMIT;

/** @brief Stores the status of ADC data availability   */
uint8_t guiADCStatus = GUI_ADC_STATUS_RESET;

/** @brief Stores the motor speed in rpm    */
uint32_t guiMotorSpeed;

/** @brief Speed timeout    */
uint32_t guiSpeedTimeout;

/** @brief Stores the number of poles in motor  */
uint32_t guiMotorPoles = GUI_MOTOR_NUMBER_OF_POLES;

//*****************************************************************************
// the function prototypes
//*****************************************************************************
/**
 * @brief     checks for all faults
 * @param[in] drvHandle  The drv8328 instance
 */
void GUI_faultCheck(drv8328_Instance *drv8328);

/**
 * @brief     get adc data
 * @param[in] tida010251 The TIDA instance
 * @param[in] drvHandle  The drv8328 instance
 */
void GUI_getADCdata(TIDAInstance *tida010251, drv8328_Instance *drv8328);

//*****************************************************************************
//defines
//*****************************************************************************
/* This macro sets the GUI variable to the default value specified
 * while also making sure the firmware value is different so during
 * the first loop everything will be configured properly.
 */
#define setGUIInitial(X,data) \
    guiVar.X = data; \
    firmVar.X = (data+1)

/**
 * @brief     setup initial GUI values
 * @param[in] drvHandle  The drv8328 instance
 */
void GUI_setupGuiValues(drv8328_Instance *drvHandle)
{
    setGUIInitial(motorDirection,   GUI_INITIAL_MOTOR_DIRECTION);
    setGUIInitial(nsleepSignal,     GUI_INITIAL_NSLEEP);
    setGUIInitial(drvoffSignal,     GUI_INITIAL_DRVOFF);
    setGUIInitial(stopMotor,        GUI_INITIAL_STOPMOTOR);
    setGUIInitial(CSAGain,          GUI_INITIAL_CSAGAIN);
    setGUIInitial(pwmFreq,          GUI_INITIAL_PWM_FREQ);
    setGUIInitial(deadTime_ns,      GUI_INITIAL_DEAD_TIME_NS);
    setGUIInitial(accRate,          GUI_INITIAL_ACCRATE);
    setGUIInitial(pulseWidth,       GUI_INITIAL_PULSE_WIDTH);
    setGUIInitial(motorBraketype,   GUI_INITIAL_BRAKETYPE);
    setGUIInitial(adcVRef,          GUI_INITIAL_ADCVREF);
    setGUIInitial(adcInternalVRef,  GUI_INITIAL_ADC_INTVREF);
    setGUIInitial(adcExternalVRef,  GUI_INITIAL_ADC_EXTVREF);

    drv8328_updateCSAScaleFactor(drvHandle, firmVar.CSAGain);
}

/**
 * @brief     checks for any change in GUI parameters
 * @param[in] tida010251 The TIDA instance
 * @param[in] drvHandle  The drv8328 instance
 * @param[in] hallTrap    The hallTrap instance
 */
void GUI_changeCheck(TIDAInstance *tida010251, drv8328_Instance *drvHandle, Halltrap_Instance *hallTrap)
{
    /* Check for change in adc reference    */
    if(firmVar.adcVRef != guiVar.adcVRef ||
       firmVar.adcInternalVRef != guiVar.adcInternalVRef ||
       firmVar.adcExternalVRef != guiVar.adcExternalVRef)
    {
        firmVar.adcVRef = guiVar.adcVRef;
        firmVar.adcInternalVRef = guiVar.adcInternalVRef;
        firmVar.adcExternalVRef = guiVar.adcExternalVRef;

        HAL_ADCVRefSel(firmVar.adcVRef, tida010251->Vsen,
                           firmVar.adcInternalVRef, firmVar.adcExternalVRef);
        HAL_ADCVRefSel(firmVar.adcVRef, drvHandle->Isen,
                           firmVar.adcInternalVRef, firmVar.adcExternalVRef);
        HAL_ADCVRefSel(firmVar.adcVRef, tida010251->speed,
                           firmVar.adcInternalVRef, firmVar.adcExternalVRef);
    }

    /* Check for change in motor state input    */
    if(firmVar.stopMotor != guiVar.stopMotor)
    {
        firmVar.stopMotor = guiVar.stopMotor;
    }

    /* Check for change in motor brake type    */
    if(firmVar.motorBraketype != guiVar.motorBraketype)
    {
        firmVar.motorBraketype = guiVar.motorBraketype;
    }

    /* Check for change in acceleration rate    */
    if(firmVar.accRate != guiVar.accRate)
    {
        firmVar.accRate = guiVar.accRate;
        accValue = firmVar.accRate * (firmVar.pwmFreq/1000);
    }
    /* Check for change in nsleep signal    */
    if(firmVar.nsleepSignal != guiVar.nsleepSignal)
    {
        firmVar.nsleepSignal = guiVar.nsleepSignal;
        drv8328_setnSleep(drvHandle, firmVar.nsleepSignal);
    }

    /* Check for change in drvoff signal    */
    if(firmVar.drvoffSignal != guiVar.drvoffSignal)
    {
        firmVar.drvoffSignal = guiVar.drvoffSignal;
        drv8328_setDrvoff(drvHandle, firmVar.drvoffSignal);
    }

    /* Check for change in CSA gain    */
    if(firmVar.CSAGain != guiVar.CSAGain)
    {
        firmVar.CSAGain = guiVar.CSAGain;
        drv8328_updateCSAScaleFactor(
                drvHandle, firmVar.CSAGain);
    }

    /* Check for change in pwm frequency and update the pwm frequency    */
    if(firmVar.pwmFreq != guiVar.pwmFreq)
    {
        firmVar.pwmFreq = guiVar.pwmFreq;
        Halltrap_setPWMFreq(hallTrap, firmVar.pwmFreq);
    }

    /* Check for change in deadband time and update deadband    */
    if(firmVar.deadTime_ns != guiVar.deadTime_ns)
    {
        firmVar.deadTime_ns = guiVar.deadTime_ns;
        Halltrap_setDeadband(hallTrap, firmVar.deadTime_ns);
    }

    /* Read ADC Data and check for faults    */
    if(guiADCStatus == GUI_ADC_READY)
    {
        GUI_getADCdata(tida010251, drvHandle);
        GUI_faultCheck(drvHandle);
        guiADCStatus = GUI_ADC_STATUS_RESET;
    }

    if(HAL_getTimerFaultStatus(drvHandle->faultIn))
    {
        fault.drvFault = TRUE;
    }
    else
    {
        fault.drvFault = FALSE;
    }

    /* check for fault reset input   */
    if(fault.Reset)
    {
        fault.ledIndiaction = GUI_FAULT_STATUS_LOW;
        fault.overVoltage = GUI_FAULT_STATUS_LOW;
        fault.underVoltage = GUI_FAULT_STATUS_LOW;
        fault.overCurrent = GUI_FAULT_STATUS_LOW;
        fault.drvFault = GUI_FAULT_STATUS_LOW;
        faultCounter = 0;
        fault.Reset = FALSE;

        HAL_clearTimerFault(drvHandle->faultIn);
    }

    if(motorState == GUI_MOTOR_RUN)
    {
        if((!firmVar.nsleepSignal) ||
           (firmVar.drvoffSignal) ||
           (firmVar.stopMotor) ||
           (fault.ledIndiaction))
        {
            motorState = GUI_MOTOR_STOP;
            Halltrap_stopMotor(hallTrap, firmVar.motorBraketype);
            firmVar.pulseWidth = 0;
            guiVar.pulseWidth = 0;
            guiADCStatus = GUI_ADC_STATUS_RESET;
        }
    }
    else if(motorState == GUI_MOTOR_STOP)
    {
        guiMotorSpeed = 0;
        if ((firmVar.nsleepSignal) &&
            (!firmVar.drvoffSignal) &&
            (!firmVar.stopMotor) &&
            (!fault.ledIndiaction))
        {
            motorState = GUI_MOTOR_RUN;
            HAL_clearTimerFault(drvHandle->faultIn);
            Halltrap_PWMUpdate(hallTrap, firmVar.motorDirection,
                               firmVar.pulseWidth);
        }
    }
    else
    {
        /* This is expected to be empty */
    }

    /* check for change in motor direction only when motor state is in run */
    if (motorState == GUI_MOTOR_RUN)
    {
        guiVar.motorDirection = HAL_readGPIOPin(tida010251->dir);
        if(firmVar.motorDirection != guiVar.motorDirection)
        {
            firmVar.motorDirection = guiVar.motorDirection;
            firmVar.pulseWidth = 0;
            motorState = GUI_MOTOR_STOP;
            Halltrap_changeMotorDirection(hallTrap, firmVar.motorDirection);
            motorState = GUI_MOTOR_RUN;
        }
    }
}

/**
 * @brief     checks for all faults
 * @param[in] drvHandle  The drv8328 instance
 */
void GUI_faultCheck(drv8328_Instance *drvHandle)
{
    _iq20 overVoltageLimit  = _IQ20(guiOverVoltageLimit);
    _iq20 underVoltageLimit = _IQ20(guiUnderVoltageLimit);
    _iq20 overCurrentLimit  = _IQ20(guiOverCurrentLimit);

    if(((moduleVoltage > overVoltageLimit)||
       (moduleVoltage < underVoltageLimit)||
       (moduleCurrent > overCurrentLimit)))
    {
        faultCounter++;
        if (faultCounter > GUI_FAULT_LIMIT)
        {
            fault.ledIndiaction = GUI_FAULT_STATUS_HIGH;
            if (moduleVoltage > overVoltageLimit)
            {
                fault.overVoltage = GUI_FAULT_STATUS_HIGH;
            }
            if (moduleVoltage < underVoltageLimit)
            {
                fault.underVoltage = GUI_FAULT_STATUS_HIGH;
            }
            if ((moduleCurrent > overCurrentLimit) &&
                (motorState == GUI_MOTOR_RUN))
            {
                fault.overCurrent = GUI_FAULT_STATUS_HIGH;
            }
            faultCounter = 0;
        }
    }
}

/**
 * @brief     accelerate motor
 * @param[in] hallTrap    The hallTrap instance
 */
void GUI_accMotor(Halltrap_Instance *hallTrap)
{
  if(accCount < accValue)
  {
    accCount++;
  }
  else
  {
    if((firmVar.pulseWidth != guiVar.pulseWidth) &&
       (motorState == GUI_MOTOR_RUN))
    {
      if(guiVar.pulseWidth <= GUI_MAX_PULSEWIDTH)
      {
        if(guiVar.pulseWidth > firmVar.pulseWidth)
        {
          firmVar.pulseWidth++;
        }
        else if(guiVar.pulseWidth < firmVar.pulseWidth)
        {
          firmVar.pulseWidth--;
        }
        else
        {
            /* This is expected to be empty */
        }
        Halltrap_PWMUpdate(hallTrap, firmVar.motorDirection,
                           firmVar.pulseWidth);
      }
    }
    accCount = 0;
  }
}

/**
 * @brief     get adc data
 * @param[in] tida010251 The TIDA instance
 * @param[in] drvHandle  The drv8328 instance
 */
void GUI_getADCdata(TIDAInstance *tida010251, drv8328_Instance *drvHandle)
{
    static _iq20 current[20];
    static _iq20 currentSum;
    static int i = 0;

    moduleVoltage = _IQ20mpy(HAL_getIQ20VoltageFromADC(tida010251->Vsen), TIDA_VOLTAGE_SCALEFCATOR);

    guiVar.pulseWidth = (uint16_t) (HAL_getmvFromADC(tida010251->speed)/33);  /* mV * 100/(3.3* 1000) */

    currentSum -= current[i];
    current[i] = drv8328_getCurrent(drvHandle->Isen,drvHandle);

    currentSum += current[i];

    moduleCurrent = _IQ20div(currentSum, _IQ20(20));
    i++;
    if(i >= 20)
    {
        i = 0;
    }

    GUI_convertReadingsFromIQtoFloat();
}

/**
 * @ brief converts IQ data to float
 */
void GUI_convertReadingsFromIQtoFloat()
{
    reading.busVoltage    = _IQ20toF(moduleVoltage);
    reading.busCurrent    = _IQ20toF(moduleCurrent);
}
