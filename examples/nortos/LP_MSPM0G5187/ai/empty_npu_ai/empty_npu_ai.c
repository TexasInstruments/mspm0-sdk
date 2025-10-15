/*
 * Copyright (c) 2025, Texas Instruments Incorporated
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
/*!****************************************************************************
 *  @file       empty_npu_ai.c
 *  @brief      Empty example for NPU-based edge AI inference
 *
 *****************************************************************************/

/*
 * @brief Define to 1 when a compiled model is added to the project.
 */
#define MODEL_AVAILABLE (0)
#if MODEL_AVAILABLE == (0)
#warning A compiled model must be added to this project - see the README file.
#endif

#include <ti/npu/npu.h>
#include "ti_msp_dl_config.h"

#if MODEL_AVAILABLE == (1)
#include "model/tvmgen_default.h"
#endif

int main(void)
{
    /* Initialize the device for 80MHz operation from SYSPLL. */
    SYSCFG_DL_init();

    /* Power up the neural processing unit (NPU) module.
     * Clear and enable the NPU interrupt.
     * NPU interrupts will be handled inside the NPU
     * library created during model compilation (outside of this project).
     * NOTE: In future releases, the NPU initialization calls made below
     * may be transitioned to SysConfig generated calls placed in 
     * the auto-generatead ti_msp_dl_config.h/.c files, at which point
     * they will be handled inside of the SYSCFG_DL_init() call.
     */
    DL_NPU_reset(NPU);
    DL_NPU_enablePower(NPU);
    while (!(DL_SYSCTL_getStatus() & DL_SYSCTL_STATUS_NPU_READY));
    DL_NPU_clearInterruptStatus(NPU, DL_NPU_INTERRUPT_DONE);
    DL_NPU_enableInterrupt(NPU, DL_NPU_INTERRUPT_DONE);
    NVIC_EnableIRQ(NPU_INT_IRQn);

    /*
     * Enter execution loop - acquire data, pre-process, and run inference
     */
    while (1)
    {
        /*
         * TEMPLATE: Replace the NOP below by inserting data acquisition
         * (and any associated pre-processing steps) here before calling
         * the inserted model for inference computation.
         */
        __NOP();

        /*
         * TEMPLATE: After acquiring & pre-processing the data inputs to the
         * model, call the compiled inference library.  Replace the
         * tags <YOUR_INPUT_MAP_HERE>, <YOUR_OUTPUT_MAP_HERE> with the
         * arrays containing the input features and location to place
         * the output features for the model, respectively.
         */
#if MODEL_AVAILABLE == (1)
        tvmgen_default_run(<YOUR_INPUT_MAP_HERE>, <YOUR_OUTPUT_MAP_HERE>);
#endif
    }
}
