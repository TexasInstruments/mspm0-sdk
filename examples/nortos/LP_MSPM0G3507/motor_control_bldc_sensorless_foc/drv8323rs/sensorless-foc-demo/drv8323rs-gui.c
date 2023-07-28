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
#include "drv8323rs-gui.h"

/** @brief Structure to store the gui values */
gui_var guiVar;

/** @brief Structure to store the firmware values */
gui_var firmVar;

/** @brief Array to store the gui spi variables*/
gui_spi_var gui_spi;

/** @brief Array to store the fault flags */
gui_fault gui_faults;

/** @brief Stores the over voltage limit for vm in volts*/
float guiVmOverVoltageLimit = 50.0;

/** @brief Stores the under voltage limit for vm in volts */
float guiVmUnderVoltageLimit = 6.0;

/** @brief Stores the over current limit in ampere */
float guiOverCurrentLimit = 12.0;

/** @brief Stores the consecutive faults */
uint8_t gui_faultCounter = 0;

/** @brief Stores the measured vm in volts in iq format */
_iq gui_vm ;

/** @brief Stores the measured ia in ampere in iq format */
_iq gui_ia ;

/** @brief Stores the measured ib in ampere in iq format */
_iq gui_ib ;

/* This macro sets the gui variable to the default value specified
 * while also making sure the firmware value is different so during
 * the first loop everything will be configured properly.
 */
#define setGUIVarDefault(X,Y) \
    guiVar.X = Y; \
    firmVar.X = (Y+1)

void GUI_setupDefaultValues(FOC_Instance *foc_handle,
                                                 DRV8323RS_Instance *drv_handle)
{
    DRV8323RS_setVdsLvl(drv_handle, GUI_DEFAULT_DRV_VDS_LVL);
    DRV8323RS_setSenLvl(drv_handle, GUI_DEFAULT_DRV_SEN_LVL);

    setGUIVarDefault(rs,        GUI_DEFAULT_FOC_RS);
    setGUIVarDefault(ls,        GUI_DEFAULT_FOC_LS);
    setGUIVarDefault(poles,     GUI_DEFAULT_FOC_POLES);
    setGUIVarDefault(baseRpm,   GUI_DEFAULT_FOC_BASE_RPM);
    setGUIVarDefault(pwmFreq,   GUI_DEFAULT_FOC_PWMFREQ);
    setGUIVarDefault(deadband,  GUI_DEFAULT_FOC_DEADBAND);
    setGUIVarDefault(gain,      GUI_DEFAULT_DRV_CSA_GAIN);
    setGUIVarDefault(rshunt,    GUI_DEFAULT_DRV_RSHUNT);
    setGUIVarDefault(piSpdKp,    GUI_DEFAULT_FOC_PISPD_KP);
    setGUIVarDefault(piSpdKi,    GUI_DEFAULT_FOC_PISPD_KI);
    setGUIVarDefault(piIdKp,    GUI_DEFAULT_FOC_PIID_KP);
    setGUIVarDefault(piIdKi,    GUI_DEFAULT_FOC_PIID_KI);
    setGUIVarDefault(piIqKp,    GUI_DEFAULT_FOC_PIIQ_KP);
    setGUIVarDefault(piIqKi,    GUI_DEFAULT_FOC_PIIQ_KI);
    setGUIVarDefault(kslide,    GUI_DEFAULT_FOC_KSLIDE);

    FOC_setPara(foc_handle, PARA_IDX_PWMADCSAMPLE,
                                                  GUI_DEFAULT_FOC_PWMADCSAMPLE);
    FOC_setPara(foc_handle, PARA_IDX_CNTRLDIV, GUI_DEFAULT_FOC_CNTRLDIV);
    FOC_setPara(foc_handle, PARA_IDX_VOLT_RATIO, GUI_DEFAULT_FOC_VOLT_RATIO);
    FOC_setPara(foc_handle, PARA_IDX_PISPD_MAX, GUI_DEFAULT_FOC_PISPD_MAX);
    FOC_setPara(foc_handle, PARA_IDX_PISPD_MIN, GUI_DEFAULT_FOC_PISPD_MIN);
    FOC_setPara(foc_handle, PARA_IDX_PISPD_DIV, GUI_DEFAULT_FOC_PISPD_DIV);
    FOC_setPara(foc_handle, PARA_IDX_PIIQ_MAX, GUI_DEFAULT_FOC_PIIQ_MAX);
    FOC_setPara(foc_handle, PARA_IDX_PIIQ_MIN, GUI_DEFAULT_FOC_PIIQ_MIN);
    FOC_setPara(foc_handle, PARA_IDX_PIID_MAX, GUI_DEFAULT_FOC_PIID_MAX);
    FOC_setPara(foc_handle, PARA_IDX_PIID_MIN, GUI_DEFAULT_FOC_PIID_MIN);
}

void GUI_init(FOC_Instance *foc_handle, DRV8323RS_Instance *drv_handle)
{
    GUI_setupDefaultValues(foc_handle, drv_handle);
}

void GUI_loop(FOC_Instance *foc_handle, DRV8323RS_Instance *drv_handle)
{    
    /* Check for change in rs */
    if(firmVar.rs != guiVar.rs)
    {
        FOC_setPara(foc_handle, PARA_IDX_RS, guiVar.rs);
        firmVar.rs = guiVar.rs;
    }
    
    /* Check for change in ls */
    if(firmVar.ls != guiVar.ls)
    {
        FOC_setPara(foc_handle, PARA_IDX_LS, guiVar.ls);
        firmVar.ls = guiVar.ls;
    }
    
    /* Check for change in poles */
    if(firmVar.poles != guiVar.poles)
    {
        FOC_setPara(foc_handle, PARA_IDX_POLES, guiVar.poles);
        firmVar.poles = guiVar.poles;
    }

    /* Check for change in baseRpm */
    if(firmVar.baseRpm != guiVar.baseRpm)
    {
        FOC_setPara(foc_handle, PARA_IDX_BASE_RPM, guiVar.baseRpm);
        firmVar.baseRpm = guiVar.baseRpm;
    }

    /* Check for change in pwmFreq */
    if(firmVar.pwmFreq != guiVar.pwmFreq)
    {
        FOC_setPara(foc_handle, PARA_IDX_PWMFREQ, guiVar.pwmFreq);
        firmVar.pwmFreq = guiVar.pwmFreq;
    }

    /* Check for change in deadband */
    if(firmVar.deadband != guiVar.deadband)
    {
        FOC_setPara(foc_handle, PARA_IDX_DEADBAND, guiVar.deadband);
        firmVar.deadband = guiVar.deadband;
    }
    
    /* Check for change in gain */
    if(firmVar.gain != guiVar.gain)
    {
        DRV8323RS_setCsaGain(drv_handle, guiVar.gain);
        DRV8323RS_setISf(drv_handle, guiVar.gain,
                                                 _IQ15(guiVar.rshunt));
        FOC_setPara(foc_handle, PARA_IDX_AMP_GAIN, drvGain[guiVar.gain]);
        firmVar.gain = guiVar.gain;
    }
    
    /* Check for change in rshunt */
    if(firmVar.rshunt != guiVar.rshunt)
    {
        DRV8323RS_setISf(drv_handle, guiVar.gain, _IQ15(guiVar.rshunt));
        FOC_setPara(foc_handle, PARA_IDX_RSHUNT, guiVar.rshunt);
        firmVar.rshunt = guiVar.rshunt;
    }

    /* Check for change in piSpdKp */
    if(firmVar.piSpdKp != guiVar.piSpdKp)
    {
        FOC_setPara(foc_handle, PARA_IDX_PISPD_KP, guiVar.piSpdKp);
        firmVar.piSpdKp = guiVar.piSpdKp;
    }

    /* Check for change in piSpdKi */
    if(firmVar.piSpdKi != guiVar.piSpdKi)
    {
        FOC_setPara(foc_handle, PARA_IDX_PISPD_KI, guiVar.piSpdKi);
        firmVar.piSpdKi = guiVar.piSpdKi;
    }

    /* Check for change in piIqKp */
    if(firmVar.piIdKp != guiVar.piIdKp)
    {
        FOC_setPara(foc_handle, PARA_IDX_PIID_KP, guiVar.piIdKp);
        firmVar.piIdKp = guiVar.piIdKp;
    }

    /* Check for change in piIdKi */
    if(firmVar.piIdKi != guiVar.piIdKi)
    {
        FOC_setPara(foc_handle, PARA_IDX_PIID_KI, guiVar.piIdKi);
        firmVar.piIdKi = guiVar.piIdKi;
    }

    /* Check for change in piIqKp */
    if(firmVar.piIqKp != guiVar.piIqKp)
    {
        FOC_setPara(foc_handle, PARA_IDX_PIIQ_KP, guiVar.piIqKp);
        firmVar.piIqKp = guiVar.piIqKp;
    }

    /* Check for change in piIqKi */
    if(firmVar.piIqKi != guiVar.piIqKi)
    {
        FOC_setPara(foc_handle, PARA_IDX_PIIQ_KI, guiVar.piIqKi);
        firmVar.piIqKi = guiVar.piIqKi;
    }

    /* Check for change in kslide */
    if(firmVar.kslide != guiVar.kslide)
    {
        FOC_setPara(foc_handle, PARA_IDX_KSLIDE, guiVar.kslide);
        firmVar.kslide = guiVar.kslide;
    }

    /* Check for clear fault flag */
    if(gui_faults.clearFault == true)
    {
        GUI_clearFaults(foc_handle, drv_handle);
    }
}

void GUI_getVars(DRV8323RS_Instance *drv_handle)
{
    /* mathacl is shared between ISRs.
     so we need to protect the use of mathacl */
    __disable_irq();
     gui_vm = DRV8323RS_getVM(drv_handle);
     gui_ia = DRV8323RS_getIA(drv_handle);
     gui_ib = DRV8323RS_getIB(drv_handle);
     __enable_irq();
}

void GUI_checkFaults(FOC_Instance *foc_handle, DRV8323RS_Instance *drv_handle)
{
    bool fault = false;

    if(DRV8323RS_isFaultOccurred(drv_handle) == true)
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

    if(gui_ia > _IQ(guiOverCurrentLimit) ||
     gui_ib > _IQ(guiOverCurrentLimit) )
    {
        fault = true;
        gui_faults.ocp = true;
    }

    if(gui_vm > _IQ(guiVmOverVoltageLimit))
    {
        fault = true;
        gui_faults.ovlo = true;
    }
    if(gui_vm < _IQ(guiVmUnderVoltageLimit))
    {
        fault = true;
        gui_faults.uvlo = true;
    }

    if(fault == true)
    {
        FOC_setFault(foc_handle);
    }
}

void GUI_clearFaults(FOC_Instance *foc_handle, DRV8323RS_Instance *drv_handle)
{
    gui_faults.uvlo = false;
    gui_faults.ovlo = false;
    gui_faults.ocp = false;
    gui_faults.drvFault = false;
    gui_faults.clearFault = false;
    DRV8323RS_clearFaults(drv_handle);
    FOC_clearFault(foc_handle);
}

void GUI_spiCommands(DRV8323RS_Instance *drv_handle)
{
    if(gui_spi.writeRegFlag)
    {
        DRV8323RS_spiWrite(
                       drv_handle, gui_spi.writeRegAddr, gui_spi.writeRegData);
        gui_spi.writeRegFlag = false;

        gui_spi.readRegAddr = gui_spi.writeRegAddr;
        gui_spi.readRegData = 
                             DRV8323RS_spiRead(drv_handle, gui_spi.readRegAddr);
    }

    if(gui_spi.readRegFlag)
    {
        gui_spi.readRegData = 
                             DRV8323RS_spiRead(drv_handle, gui_spi.readRegAddr);
        gui_spi.readRegFlag = false;
    }

    if(gui_spi.writeBitfieldRegFlag)
    {
        uint8_t mask = (gui_spi.bitfieldRegMaskData
                                                 << gui_spi.bitfieldRegPosData);
        uint8_t data = (gui_spi.writeBitfieldRegData
                                                 << gui_spi.bitfieldRegPosData);
        DRV8323RS_spiUpdateRegister(drv_handle,
                                      gui_spi.writeBitfieldRegAddr, mask, data);
        gui_spi.writeBitfieldRegFlag = false;
    }
}
