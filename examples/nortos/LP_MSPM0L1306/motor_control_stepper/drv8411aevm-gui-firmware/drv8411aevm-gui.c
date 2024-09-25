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
#include "drv8411aevm-gui.h"

/** @brief Structure to store the gui values */
gui_var guiVar;

/** @brief Structure to store the firmware values */
gui_var firmVar;

/** @brief Array to store the fault flags */
gui_fault gui_faults;

/** @brief Stores the consecutive faults */
uint8_t gui_faultCounter = 0;

/** @brief Stores the sampled IA current in IQ15 */
_iq15 gui_iaSampled = 0;

/** @brief Stores the sampled IB current in IQ15 */
_iq15 gui_ibSampled = 0;

/** @brief Stores A phase current to be plotted in gui */
float gui_iaPlotVar = 0;

/** @brief Stores A phase current to be plotted in gui */
float gui_ibPlotVar = 0;

/** @brief Flag to update the gui plot variables */
bool gui_updatePlotVars = false;

/** @brief Flag to start monitoring if motor stops */
bool gui_monitorMotor = false;

/** @brief  Value of AIpropi in uA/A unit, This is a readonly value
 set in initialization and cant be changed afterwards */
uint16_t gui_aIpropi;

/* This macro sets the gui variable to the default value specified
 * while also making sure the firmware value is different so during
 * the first loop everything will be configured properly.
 */
#define setGUIVarDefault(X,Y) \
    guiVar.X = Y; \
    firmVar.X = (Y+1)

void gui_setupDefaultValues(void)
{
    setGUIVarDefault(stepMode,      GUI_DEFAULT_STEP_MODE);
    setGUIVarDefault(decayMode,     GUI_DEFAULT_DECAY_MODE);
    setGUIVarDefault(dir,           GUI_DEFAULT_DIRECTION);
    setGUIVarDefault(controlMode,   GUI_DEFAULT_CONTROL_MODE);
    setGUIVarDefault(sleep,         GUI_DEFAULT_SLEEP);
    setGUIVarDefault(speed,         GUI_DEFAULT_SPEED);
    setGUIVarDefault(ripropiVal,    GUI_DEFAULT_RIPROPI);
    setGUIVarDefault(itrip,         GUI_DEFAULT_ITRIP);
    setGUIVarDefault(drvAdcVRef,    GUI_DEFAULT_DRV_ADC_REF_VOLTAGE);
    setGUIVarDefault(drvDacVRef,    GUI_DEFAULT_DRV_DAC_REF_VOLTAGE);
    gui_aIpropi =                   GUI_DEFAULT_DRV_A_IPROPI;
}

void gui_init(DRV8411A_Instance *drv_handle)
{
    gui_setupDefaultValues();

    /* Set the value of aipropi*/
    uint32_t _1ByAIpropoi = 1000000/gui_aIpropi;
    DRV8411A_setISf(drv_handle, _IQ15(_1ByAIpropoi));
}

void gui_loop(DRV8411A_Instance *drv_handle)
{
    /* Check for change in sleep mode */
    if(firmVar.sleep != guiVar.sleep)
    {
        if(guiVar.sleep == GUI_DRV_STATE_ASLEEP)
        {
            DRV8411A_disableDrive(drv_handle);
        }
        else if(guiVar.sleep == GUI_DRV_STATE_AWAKE)
        {
            DRV8411A_enableDrive(drv_handle);
        }
        else
        {
            /* This is expected to be empty */
        }
        firmVar.sleep = guiVar.sleep;
    }

    /* Check for change in direction */
    if(firmVar.dir != guiVar.dir)
    {
        DRV8411A_setDirection(drv_handle, guiVar.dir);
        firmVar.dir = guiVar.dir;
    }

    /* Check for change in step mode */
    if(firmVar.stepMode != guiVar.stepMode)
    {
        DRV8411A_setStepMode(drv_handle, guiVar.stepMode);
        firmVar.stepMode = guiVar.stepMode;
    }

    /* Check for change in decay mode */
    if(firmVar.decayMode != guiVar.decayMode)
    {
        DRV8411A_setDecayMode(drv_handle, guiVar.decayMode);
        firmVar.decayMode = guiVar.decayMode;
    }

    /* Check for change in speed */
    if(firmVar.speed != guiVar.speed)
    {
        DRV8411A_setSpeed(drv_handle, guiVar.speed);
        firmVar.speed = guiVar.speed;
    }

    /* Check for change in step */
    if(firmVar.steps != guiVar.steps)
    {
        DRV8411A_setStep(drv_handle, guiVar.steps);
        firmVar.steps = guiVar.steps;
    }

    /* Check for change in motor state */
    if(firmVar.motorState != guiVar.motorState)
    {
        if(guiVar.motorState == GUI_MOTOR_STATE_STOP)
        {
            DRV8411A_stopMotor(drv_handle);
            firmVar.motorState = guiVar.motorState;
        }
        else if(guiVar.motorState == GUI_MOTOR_STATE_RUN)
        {
            gui_monitorMotor = true;
            DRV8411A_startMotor(drv_handle);
            firmVar.motorState = guiVar.motorState;
        }
        else
        {
            /* This is expected to be empty */
        }
    }

    /* Check for change in control mode */
    if(firmVar.controlMode != guiVar.controlMode)
    {
        DRV8411A_setControlMode(drv_handle, guiVar.controlMode);
        firmVar.controlMode = guiVar.controlMode;
    }

    /* Check for change in ripropi value */
    if(firmVar.ripropiVal != guiVar.ripropiVal)
    {
        DRV8411A_setRIPROPI(drv_handle, guiVar.ripropiVal);
        DRV8411A_setItrip(drv_handle, _IQ15(guiVar.itrip));
        firmVar.ripropiVal = guiVar.ripropiVal;
    }

    /* Check for change in itrip value */
    if(firmVar.itrip != guiVar.itrip)
    {
        DRV8411A_setItrip(drv_handle, _IQ15(guiVar.itrip));
        firmVar.itrip = guiVar.itrip;
    }

    /* Check for gui_monitorMotor flag */
    if(guiVar.controlMode == DRV8411A_CONTROL_MODE_STEP
                                                    && gui_monitorMotor == true)
    {
        if(DRV8411A_isMotorRunning(drv_handle) == false)
        {
            gui_monitorMotor = false;
            guiVar.motorState = GUI_MOTOR_STATE_STOP;
            firmVar.motorState = GUI_MOTOR_STATE_STOP;
        }
    }

    /* Check for change in DRV ADC reference */
    if(firmVar.drvAdcVRef != guiVar.drvAdcVRef)
    {
        DRV8411A_setPhaseCurrentRefVoltage(
                            drv_handle, _IQ15(guiVar.drvAdcVRef));
        firmVar.drvAdcVRef = guiVar.drvAdcVRef;
    }

    /* Check for change in DRV DAC reference */
    if(firmVar.drvDacVRef != guiVar.drvDacVRef)
    {
        DRV8411A_setVrefRefVoltage(
                            drv_handle, _IQ15(guiVar.drvDacVRef));
        /* Setting the itrip, so that the new Vref is used */
        DRV8411A_setItrip(drv_handle, _IQ15(guiVar.itrip));
        firmVar.drvDacVRef = guiVar.drvDacVRef;
    }

    /* Check for clear fault flag */
    if(gui_faults.clearFault == true)
    {
        gui_faults.drvFault = false;
        if(guiVar.sleep == GUI_DRV_STATE_AWAKE)
        {
            DRV8411A_enableDrive(drv_handle);
        }
        gui_faults.clearFault = false;
    }
}

void gui_checkFaults(DRV8411A_Instance *drv_handle)
{
    bool fault = false;
    if(DRV8411A_isFaultOccurred(drv_handle) == true)
    {
        /* Increments the count for consecutive faults */
        gui_faultCounter++;
        if(gui_faultCounter >= GUI_FAULT_COUNT_THRES)
        {
            fault = true;
            gui_faults.drvFault = true;
        }
    }
    else
    {
        /* Clears the count for consecutive faults */
        gui_faultCounter = 0;
    }
    if(fault == true)
    {
        DRV8411A_stopMotor(drv_handle);
        DRV8411A_disableDrive(drv_handle);
        guiVar.motorState = GUI_MOTOR_STATE_STOP;
        firmVar.motorState = GUI_MOTOR_STATE_STOP;
    }
}

void gui_convertPhaseCurrent(DRV8411A_Instance *drv_handle)
{
    gui_iaSampled = DRV8411A_getIA(drv_handle);
    gui_ibSampled = DRV8411A_getIB(drv_handle);

    if(gui_updatePlotVars)
    {
        gui_iaPlotVar = _IQ15toF(gui_iaSampled);
        gui_ibPlotVar = _IQ15toF(gui_ibSampled);
    }
}

void gui_setUpdatePlotVarFlag(void)
{
    gui_updatePlotVars = true;
}
