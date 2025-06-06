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
#include "drv8889-q1evm-gui.h"

/** @brief Structure to store the gui values */
gui_var guiVar;

/** @brief Structure to store the firmware values */
gui_var firmVar;

/** @brief Array to store the fault flags*/
gui_fault gui_faults;

/** @brief Array to store the gui spi variables*/
gui_spi_var gui_spi;

/** @brief Flag to start monitoring if motor stops */
bool gui_monitorMotor = false;

/** @brief Stores the consecutive faults */
uint8_t gui_faultCounter = 0;

/* This macro sets the gui variable to the default value specified
 * while also making sure the firmware value is different so during
 * the first loop everything will be configured properly.
 */
#define setGUIVarDefault(X,Y) \
    guiVar.X = Y; \
    firmVar.X = (Y+1)

void gui_setupDefaultValues(void)
{
    setGUIVarDefault(stepMode,          GUI_DEFAULT_STEP_MODE);
    setGUIVarDefault(decayMode,         GUI_DEFAULT_DECAY_MODE);
    setGUIVarDefault(slewRate,          GUI_DEFAULT_SLEW_RATE);
    setGUIVarDefault(dir,               GUI_DEFAULT_DIRECTION);
    setGUIVarDefault(controlMode,       GUI_DEFAULT_CONTROL_MODE);
    setGUIVarDefault(trqDac,            GUI_DEFAULT_TRQ_DAC);
    setGUIVarDefault(toff,              GUI_DEFAULT_TOFF);
    setGUIVarDefault(drvOutput,         GUI_DEFAULT_DRV_OUTPUT);
    setGUIVarDefault(speed,             GUI_DEFAULT_SPEED);
    setGUIVarDefault(fsCurr,            GUI_DEFAULT_FS_CURR);
    setGUIVarDefault(stallThres,        GUI_DEFAULT_STALL_THRES);
    setGUIVarDefault(stallDetState,     GUI_DEFAULT_STALL_DET_STATE);
    setGUIVarDefault(olState,           GUI_DEFAULT_OL_STATE);
    setGUIVarDefault(reverseOnStall,    GUI_DEFAULT_REVERSE_ON_STALL);
    setGUIVarDefault(drvDacVRef,        GUI_DEFAULT_DRV_VREF_REF_VOLTAGE);
}

void gui_init(DRV8889Q1_Instance *drv_handle)
{
    gui_setupDefaultValues();

    /*Enabling the motor to write the SPI default values
    * When the DRV is in sleep mode the SPI will not be active */
    setGUIVarDefault(sleep,             GUI_DRV_STATE_AWAKE);

    gui_loop(drv_handle);

    setGUIVarDefault(sleep,             GUI_DEFAULT_SLEEP);
}

void gui_loop(DRV8889Q1_Instance *drv_handle)
{
    /* Check for change in sleep mode */
    if(firmVar.sleep != guiVar.sleep)
    {
        if(guiVar.sleep == GUI_DRV_STATE_ASLEEP)
        {
            DRV8889Q1_clearNSleep(drv_handle);
        }
        else if(guiVar.sleep == GUI_DRV_STATE_AWAKE)
        {
            DRV8889Q1_setNSleep(drv_handle);
        }
        else
        {
            /* This is expected to be empty */
        }
        firmVar.sleep = guiVar.sleep;
    }

    /* Check for change in drv output */
    if(firmVar.drvOutput != guiVar.drvOutput)
    {
        if(guiVar.drvOutput == GUI_DRV_OUTPUT_DISABLED)
        {
            DRV8889Q1_disableOutput(drv_handle);
        }
        else if(guiVar.drvOutput == GUI_DRV_OUTPUT_ENABLED)
        {
            DRV8889Q1_enableOutput(drv_handle);
        }
        else
        {
            /* This is expected to be empty */
        }
        firmVar.drvOutput = guiVar.drvOutput;
    }

    /* Check for change in direction */
    if(firmVar.dir != guiVar.dir)
    {
        DRV8889Q1_setDirection(drv_handle, guiVar.dir);
        firmVar.dir = guiVar.dir;
    }

    /* Check for change in step mode */
    if(firmVar.stepMode != guiVar.stepMode)
    {
        DRV8889Q1_setStepMode(drv_handle, guiVar.stepMode);
        firmVar.stepMode = guiVar.stepMode;
    }

    /* Check for change in slew rate */
    if(firmVar.slewRate != guiVar.slewRate)
    {
        DRV8889Q1_setSlewRate(drv_handle, guiVar.slewRate);
        firmVar.slewRate = guiVar.slewRate;
    }

    /* Check for change in decay mode */
    if(firmVar.decayMode != guiVar.decayMode)
    {
        DRV8889Q1_setDecayMode(drv_handle, guiVar.decayMode);
        firmVar.decayMode = guiVar.decayMode;
    }

    /* Check for change in speed */
    if(firmVar.speed != guiVar.speed)
    {
        DRV8889Q1_setSpeed(drv_handle, guiVar.speed);
        firmVar.speed = guiVar.speed;
    }

    /* Check for change in step */
    if(firmVar.steps != guiVar.steps)
    {
        DRV8889Q1_setStep(drv_handle, guiVar.steps);
        firmVar.steps = guiVar.steps;
    }

    /* Check for change in motor state */
    if(firmVar.motorState != guiVar.motorState)
    {
        if(guiVar.motorState == GUI_MOTOR_STATE_STOP)
        {
            DRV8889Q1_stopMotor(drv_handle);
            firmVar.motorState = guiVar.motorState;
        }
        else if(guiVar.motorState == GUI_MOTOR_STATE_RUN)
        {
            if(guiVar.controlMode == DRV8889Q1_CONTROL_MODE_STEP 
                && guiVar.steps ==0)
            {
                /* if the motor is in step mode and the set step is 0
                 we dont start the motor*/
                guiVar.motorState = GUI_MOTOR_STATE_STOP;
            }
            else
            {
                gui_monitorMotor = true;
                DRV8889Q1_startMotor(drv_handle);
                firmVar.motorState = guiVar.motorState;
            }
            
        }
        else
        {
            /* This is expected to be empty */
        }
    }

    /* Check for change in control mode */
    if(firmVar.controlMode != guiVar.controlMode)
    {
        DRV8889Q1_setControlMode(drv_handle, guiVar.controlMode);
        firmVar.controlMode = guiVar.controlMode;
    }

    /* Check for change in torque percentage */
    if(firmVar.trqDac != guiVar.trqDac)
    {
        DRV8889Q1_setTrqDac(drv_handle, guiVar.trqDac);
        firmVar.trqDac = guiVar.trqDac;
    }

    /* Check for change in toff */
    if(firmVar.toff != guiVar.toff)
    {
        DRV8889Q1_setToff(drv_handle, guiVar.toff);
        firmVar.toff = guiVar.toff;
    }

    /* Check for change in stall threshold */
    if(firmVar.stallThres != guiVar.stallThres)
    {
        DRV8889Q1_setStallThres(drv_handle, guiVar.stallThres);
        firmVar.stallThres = guiVar.stallThres;
    }

    /* Check for change in reverse on stall */
    if(firmVar.reverseOnStall != guiVar.reverseOnStall)
    {
        firmVar.reverseOnStall = guiVar.reverseOnStall;
    }
    
    /* Check for change in stall mode */
    if(firmVar.stallDetState != guiVar.stallDetState)
    {
        if(guiVar.stallDetState == true)
        {
            DRV8889Q1_enableStallDet(drv_handle);
            /* Stall detection only works in Smart Tune ripple control 
               decay mode so we set the mode*/
            DRV8889Q1_setDecayMode(
                            drv_handle, DRV8889Q1_DECAY_SMART_TUNE_RIPPLE_CTRL);
            guiVar.decayMode = DRV8889Q1_DECAY_SMART_TUNE_RIPPLE_CTRL;
            guiVar.decayMode = DRV8889Q1_DECAY_SMART_TUNE_RIPPLE_CTRL;
        }
        else
        {
            DRV8889Q1_disableStallDet(drv_handle);
        }
        firmVar.stallDetState = guiVar.stallDetState;
    }

    /* Check for change in stall mode */
    if(firmVar.olState != guiVar.olState)
    {
        if(guiVar.olState == GUI_OL_ENABLED)
        {
            DRV8889Q1_enableOlDet(drv_handle);
        }
        else if(guiVar.olState == GUI_OL_DISABLED)
        {
            DRV8889Q1_disableOlDet(drv_handle);
        }
        else
        {
            /* This is expected to be empty */
        }
        firmVar.olState = guiVar.olState;
    }
    
    /* Check for change in control mode */
    if(firmVar.drvDacVRef != guiVar.drvDacVRef)
    {
        DRV8889Q1_setVrefRefVoltage(drv_handle, _IQ15(guiVar.drvDacVRef));
        /* Setting the itrip, so that the new Vref is used */
        DRV8889Q1_setFSCurr(drv_handle, _IQ15(guiVar.fsCurr));
        firmVar.drvDacVRef = guiVar.drvDacVRef;
    }

    /* Check for change in full scale current */
    if(firmVar.fsCurr != guiVar.fsCurr)
    {
        DRV8889Q1_setFSCurr(drv_handle, _IQ15(guiVar.fsCurr));
        firmVar.fsCurr = guiVar.fsCurr;
    }

    /* Check for gui_monitorMotor flag */
    if(guiVar.controlMode == DRV8889Q1_CONTROL_MODE_STEP 
                                                    && gui_monitorMotor == true)
    {
        if(DRV8889Q1_isMotorRunning(drv_handle) == false)
        {
            gui_monitorMotor = false;
            guiVar.motorState = GUI_MOTOR_STATE_STOP;
            firmVar.motorState = GUI_MOTOR_STATE_STOP;
        }
    }

    /* Check for clear fault flag */
    if(gui_faults.clearFault == true)
    {
        gui_clearFaults(drv_handle);
    }
}

void gui_checkFaults(DRV8889Q1_Instance *drv_handle)
{
    bool fault = false;
    if(DRV8889Q1_isFaultOccurred(drv_handle) == true)
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
        DRV8889Q1_stopMotor(drv_handle);
        DRV8889Q1_disableOutput(drv_handle);
        guiVar.motorState = GUI_MOTOR_STATE_STOP;
        firmVar.motorState = GUI_MOTOR_STATE_STOP;
    }
    
    /* Check and update the fault status*/
    gui_monitorFaultStatus(drv_handle);
}

void gui_clearFaults(DRV8889Q1_Instance *drv_handle)
{
    if(guiVar.sleep == GUI_DRV_STATE_AWAKE)
    {
        DRV8889Q1_setNSleep(drv_handle);
    }
    if(guiVar.drvOutput == GUI_DRV_OUTPUT_ENABLED)
    {
        DRV8889Q1_enableOutput(drv_handle);
    }
    DRV8889Q1_clearFaults(drv_handle);
    gui_faults.uvlo = false;
    gui_faults.cpuv = false;
    gui_faults.ocp = false;
    gui_faults.stl = false;
    gui_faults.tf = false;
    gui_faults.ol = false;
    gui_faults.drvFault = false;
    gui_faults.clearFault = false;
}

void gui_monitorFaultStatus(DRV8889Q1_Instance *drv_handle)
{   
    /* Reading the drv fault status register*/ 
    uint8_t regRead = DRV8889Q1_spiRead(
                                drv_handle, DRV8889Q1_REG_ADDR_FAULT_STATUS);

    if(regRead & DRV8889Q1_REG_FAULT_STATUS_UVLO_MASK)
    {
        gui_faults.uvlo = true;
    }
    else
    {
        gui_faults.uvlo = false;
    }
    if(regRead & DRV8889Q1_REG_FAULT_STATUS_CPUV_MASK)
    {
        gui_faults.cpuv = true;
    }
    else
    {
        gui_faults.cpuv = false;
    }
    if(regRead & DRV8889Q1_REG_FAULT_STATUS_OCP_MASK)
    {
        gui_faults.ocp = true;
    }
    else
    {
        gui_faults.ocp = false;
    }
    if(regRead & DRV8889Q1_REG_FAULT_STATUS_STL_MASK)
    {
        gui_faults.stl = true;
    }
    else
    {
        gui_faults.stl = false;
    }
    if(regRead & DRV8889Q1_REG_FAULT_STATUS_TF_MASK)
    {
        gui_faults.tf = true;
    }
    else
    {
        gui_faults.tf = false;
    }
    if(regRead & DRV8889Q1_REG_FAULT_STATUS_OL_MASK)
    {
        gui_faults.ol = true;
    }
    else
    {
        gui_faults.ol = false;
    }
}


void gui_CheckStallReverse(DRV8889Q1_Instance *drv_handle)
{
    if(gui_faults.drvFault == true 
       && guiVar.stallDetState == GUI_STALL_ENABLED
       && guiVar.reverseOnStall == GUI_REVERSE_ON_STALL_ENABLED
       && gui_faults.stl == true)
    {
        /* Clear the fault */
        gui_clearFaults(drv_handle);

        /* Small delay before reversing the direction*/
        HAL_delayMilliSeconds(GUI_WAIT_BEFORE_REVERSE_ON_STALL);

        /* Switch the direction*/
        if(guiVar.dir == DRV8889Q1_DIR_FORWARD)
        {
            guiVar.dir = DRV8889Q1_DIR_REVERSE;
            firmVar.dir = DRV8889Q1_DIR_REVERSE;
        }
        else
        {
            guiVar.dir = DRV8889Q1_DIR_FORWARD;
            firmVar.dir = DRV8889Q1_DIR_FORWARD;
        }
        DRV8889Q1_setDirection(drv_handle, guiVar.dir);

        /* Start the motor */
        guiVar.motorState = GUI_MOTOR_STATE_RUN;
        firmVar.motorState = GUI_MOTOR_STATE_RUN;
        DRV8889Q1_startMotor(drv_handle);
    }
}

void gui_spiCommands(DRV8889Q1_Instance *drv_handle)
{
    if(gui_spi.writeRegFlag)
    {
        DRV8889Q1_spiWrite(
                        drv_handle, gui_spi.writeRegAddr, gui_spi.writeRegData);
        gui_spi.writeRegFlag = false;

        gui_spi.readRegAddr = gui_spi.writeRegAddr;
        gui_spi.readRegData = DRV8889Q1_spiRead(drv_handle, gui_spi.readRegAddr);
    }

    if(gui_spi.readRegFlag)
    {
        gui_spi.readRegData = DRV8889Q1_spiRead(drv_handle, gui_spi.readRegAddr);
        gui_spi.readRegFlag = false;
    }

    if(gui_spi.writeBitfieldRegFlag)
    {
        uint8_t mask = (gui_spi.bitfieldRegMaskData 
                                                << gui_spi.bitfieldRegPosData);
        uint8_t data = (gui_spi.writeBitfieldRegData 
                                                << gui_spi.bitfieldRegPosData);
        DRV8889Q1_spiUpdateRegister(drv_handle, 
                                    gui_spi.writeBitfieldRegAddr, mask, data);
        gui_spi.writeBitfieldRegFlag = false;
    }
}
