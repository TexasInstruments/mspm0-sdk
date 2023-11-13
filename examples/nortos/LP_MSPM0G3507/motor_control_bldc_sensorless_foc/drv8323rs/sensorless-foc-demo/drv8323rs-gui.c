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

/** @brief Array to store the gui spi variables*/
gui_spi_var gui_spi;

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
