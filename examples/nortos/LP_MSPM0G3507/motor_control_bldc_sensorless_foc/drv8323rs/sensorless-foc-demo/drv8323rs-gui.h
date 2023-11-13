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
#ifndef GUI_H
#define GUI_H

#include <stdbool.h>
#include "drv8323rs.h"

#ifdef __cplusplus
extern "C" {
#endif

/*! @brief Define gui spi variables */
typedef struct
{  /*!  Write Bitfield flag */
   volatile bool writeBitfieldRegFlag;
   /*!  Stores register address to write Bitfield. @ref DRV8323RS_REG_ADDR */
   volatile DRV8323RS_REG_ADDR writeBitfieldRegAddr;
   /*!  Stores position of Bitfield to write */
   volatile uint16_t bitfieldRegPosData;
   /*!  Stores mask of Bitfield to write */
   volatile uint16_t bitfieldRegMaskData;
   /*!  Stores data of Bitfield to write */
   volatile uint16_t writeBitfieldRegData;
   /*!  Write register flag */
   volatile bool writeRegFlag;
   /*!  Stores address of register to write. @ref DRV8323RS_REG_ADDR */
   volatile DRV8323RS_REG_ADDR writeRegAddr;
   /*!  Stores data to write to register */
   volatile uint16_t writeRegData;
   /*!  Read register flag */
   volatile bool readRegFlag;
   /*!  Stores address of register to read. @ref DRV8323RS_REG_ADDR */
   volatile DRV8323RS_REG_ADDR readRegAddr;
   /*!  Stores data read from register */
   volatile uint16_t readRegData;
}gui_spi_var;

/**
 * @brief     Handles the SPI register request from GUI
 * @param[in] drv_handle  A pointer to drv instance
 */
void GUI_spiCommands(DRV8323RS_Instance *drv_handle);

#ifdef __cplusplus
}
#endif
#endif /* GUI_H */
