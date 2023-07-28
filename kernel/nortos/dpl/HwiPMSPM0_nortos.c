/*
 * Copyright (c) 2020, Texas Instruments Incorporated
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
/*
 *  ======== HwiPMSPM0_nortos.c ========
 */

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <ti/driverlib/m0p/dl_interrupt.h>
#include <ti/devices/msp/msp.h>
#include <ti/drivers/dpl/HwiP.h>
#include <ti/drivers/dpl/DebugP.h>

/* Sourced from dl_interrupt.c */
#define MAX_INTERRUPTS                                                     (48)
#define NUM_RES_M0_EXC                                                     (16)

/* Masks off all bits but the VECTACTIVE bits in the ICSR register. */
#define portVECTACTIVE_MASK (0xFFUL)

#define INT_PRI_LEVEL0          0
#define INT_PRI_LEVEL1          1
#define INT_PRI_LEVEL2          2
#define INT_PRI_LEVEL3          3

#ifdef __IAR_SYSTEMS_ICC__
  #define portNVIC_INT_CTRL_REG (*(portNVIC_INT_CTRL))
#endif

int HwiP_swiPIntNum = PendSV_VECn;

typedef struct _HwiP_Obj
{
    uint32_t intNum;
    HwiP_Fxn fxn;
    uintptr_t arg;
} HwiP_Obj;

static HwiP_Obj *HwiP_dispatchTable[MAX_INTERRUPTS] = {0};

void HwiP_dispatch(void);

/*
 *  ======== HwiP_enable ========
 */
void HwiP_enable(void)
{
    __enable_irq();
}

/*
 *  ======== HwiP_disable ========
 */
uintptr_t HwiP_disable(void)
{
    uint32_t status;
    status = __get_PRIMASK();
    __disable_irq();
    return status;
}

/*
 *  ======== HwiP_restore ========
 */
void HwiP_restore(uintptr_t alreadyDisabled)
{
    if (!alreadyDisabled) {
        __enable_irq();
    }
}

/*
 *  ======== HwiP_post ========
 */
void HwiP_post(int interruptNum)
{
    NVIC_SetPendingIRQ((IRQn_Type) interruptNum);
}

/*
 *  ======== HwiP_clearInterrupt ========
 */
void HwiP_clearInterrupt(int interruptNum)
{
    NVIC_ClearPendingIRQ((IRQn_Type) interruptNum);
}

/*
 * ======== HwiP_setPri ========
 */
void HwiP_setPriority(int interruptNum, uint32_t priority)
{
    NVIC_SetPriority((IRQn_Type) interruptNum, priority);
}

/*
 *  ======== HwiP_disableInterrupt ========
 */
void HwiP_disableInterrupt(int interruptNum)
{
    NVIC_DisableIRQ((IRQn_Type) interruptNum);
}

/*
 *  ======== HwiP_enableInterrupt ========
 */
void HwiP_enableInterrupt(int interruptNum)
{
    NVIC_EnableIRQ((IRQn_Type) interruptNum);
}

/*
 *  ======== HwiP_interruptsEnabled ========
 */
bool HwiP_interruptsEnabled(void)
{
    uint32_t priMask;

    priMask = __get_PRIMASK();

    return (priMask == 0);
}

/*
 *  ======== HwiP_construct ========
 */
HwiP_Handle HwiP_construct(HwiP_Struct *handle, int interruptNum, HwiP_Fxn hwiFxn, HwiP_Params *params)
{
    HwiP_Params defaultParams;
    HwiP_Obj *obj = (HwiP_Obj *)handle;

    if (handle != NULL)
    {
        if (params == NULL)
        {
            params = &defaultParams;
            HwiP_Params_init(&defaultParams);
        }

        if ((params->priority & 0xFF) == 0xFF)
        {
            /* SwiP_nortos.c uses INT_PRI_LEVEL7 as its scheduler */
            params->priority = INT_PRI_LEVEL2;
        }

        if (interruptNum != HwiP_swiPIntNum && params->priority == INT_PRI_LEVEL3)
        {
            handle = NULL;
        }
        else
        {
            HwiP_dispatchTable[interruptNum] = obj;
            obj->fxn                         = hwiFxn;
            obj->arg                         = params->arg;
            obj->intNum                      = (uint32_t)interruptNum;

            DL_Interrupt_registerInterrupt((uint32_t)interruptNum, HwiP_dispatch);
            NVIC_SetPriority((uint32_t)interruptNum, (uint8_t)params->priority);

            if (params->enableInt)
            {
                NVIC_EnableIRQ((uint32_t)interruptNum);
            }
        }
    }

    return ((HwiP_Handle)handle);
}

/*
 *  ======== HwiP_create ========
 */
HwiP_Handle HwiP_create(int interruptNum, HwiP_Fxn hwiFxn, HwiP_Params *params)
{
    HwiP_Handle handle;
    HwiP_Handle retHandle;

    handle = (HwiP_Handle)malloc(sizeof(HwiP_Obj));

    /*
     * Even though HwiP_construct will check handle for NULL and not do
     * anything, we should check it here so that we can know afterwards
     * that construct failed with non-NULL pointer and that we need to
     * free the handle.
     */
    if (handle != NULL)
    {
        retHandle = HwiP_construct((HwiP_Struct *)handle, interruptNum, hwiFxn, params);
        if (retHandle == NULL)
        {
            free(handle);
            handle = NULL;
        }
    }

    return (handle);
}

/*
 *  ======== HwiP_delete ========
 */
void HwiP_delete(HwiP_Handle handle)
{
    HwiP_destruct((HwiP_Struct *)handle);

    free(handle);
}

/*
 *  ======== HwiP_destruct ========
 */
void HwiP_destruct(HwiP_Struct *handle)
{
    HwiP_Obj *obj = (HwiP_Obj *)handle;

    HwiP_disableInterrupt(obj->intNum);
    DL_Interrupt_unregisterInterrupt(obj->intNum);
}
/*
 *  ======== HwiP_Params_init ========
 */
void HwiP_Params_init(HwiP_Params *params)
{
    if (params != NULL)
    {
        params->arg       = 0;
        params->priority  = (~0);
        params->enableInt = true;
    }
}

/*
 *  ======== HwiP_plug ========
 */
void HwiP_plug(int interruptNum, void *fxn)
{
    DL_Interrupt_registerInterrupt((uint32_t)interruptNum, (void (*)(void))fxn);
}

/*
 *  ======== HwiP_setFunc ========
 */
void HwiP_setFunc(HwiP_Handle hwiP, HwiP_Fxn fxn, uintptr_t arg)
{
    HwiP_Obj *obj = (HwiP_Obj *)hwiP;

    uintptr_t key = HwiP_disable();

    obj->fxn = fxn;
    obj->arg = arg;

    HwiP_restore(key);
}

/*
 *  ======== HwiP_inISR ========
 */
bool HwiP_inISR(void)
{
    bool stat;

    if (__get_IPSR() == 0) {
        /* IPSR value of 0 indicates we are in thread mode, not handling an
           interrupt */
        stat = false;
    } else {
        stat = true;
    }

    return stat;
}

/*
 *  ======== HwiP_inSwi ========
 */
bool HwiP_inSwi(void)
{
    bool stat;

    if (__get_IPSR() == HwiP_swiPIntNum) {
        stat = true;
    } else {
        stat = false;
    }

    return stat;
}
/*
 *  ======== HwiP_dispatch ========
 */
void HwiP_dispatch(void)
{
    uint32_t intNum = __get_IPSR() - NUM_RES_M0_EXC;
    HwiP_Obj *obj   = HwiP_dispatchTable[intNum];
    if (obj)
    {
        (obj->fxn)(obj->arg);
    }
}