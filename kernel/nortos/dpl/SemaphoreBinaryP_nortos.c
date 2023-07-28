/*
 * Copyright (c) 2021, Texas Instruments Incorporated
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
 *  ======== SemaphoreBinaryP_nortos.c ========
 */

#include <cmsis_compiler.h>
#include <ti/drivers/dpl/ClockP.h>
#include <ti/drivers/dpl/HwiP.h>
#include <ti/drivers/dpl/SemaphoreBinaryP.h>

static bool pendInProgress      = false;
static bool doEnablePowerPolicy = false;
static bool enablePowerPolicy   = true;

extern void ClockP_startup(void);
extern void ClockP_add(
    ClockP_Struct *handle, ClockP_Fxn fxn, uint32_t timeout, uintptr_t arg);

/*
 * ClockP objects need a fxn, but we don't need to do anything in the fxn
 * since we just use the Clock_isActive status to determine if it's still
 * running or not.
 */
static void clkFxn(uintptr_t arg)
{
}

/*
 * Idle function waits for interrupts to occur.
 */
static inline void IdleFunc(void)
{
    if (enablePowerPolicy) {
        __WFI();
    }
}

/*
 * Similar to Power_enablePolicy, but just used as local power policy for
 * No-RTOS semaphores.
 */
static inline void EnablePowerPolicy(void)
{
    enablePowerPolicy = true;
}

/*
 * Similar to Power_disablePolicy, but just used as local power policy for
 * No-RTOS semaphores.
 */
static inline bool DisablePowerPolicy(void)
{
    bool policy       = enablePowerPolicy;
    enablePowerPolicy = false;
    return (policy);
}

/*
 *  ======== SemaphoreBinaryP_destruct ========
 */
void SemaphoreBinaryP_destruct(SemaphoreBinaryP_Struct *handle)
{
}

/*
 *  ======== SemaphoreBinaryP_pend ========
 */
SemaphoreBinaryP_Status SemaphoreBinaryP_pend(
    SemaphoreBinaryP_Handle handle, uint32_t timeout)
{
    SemaphoreBinaryP_Struct *pSemaphore = (SemaphoreBinaryP_Struct *) handle;
    SemaphoreBinaryP_Status status;
    ClockP_Struct clockStruct;
    uintptr_t key;

    ClockP_startup();

    /*
     * Always add Clock (but don't start) so that ClockP_isActive() below
     * is valid.  It's OK to add a Clock even when timeout is 0 or forever
     * (but it is not OK to start it).
     */
    ClockP_add(&clockStruct, clkFxn, timeout, (uintptr_t) NULL);

    if ((timeout != 0) && (timeout != SemaphoreBinaryP_WAIT_FOREVER)) {
        ClockP_start((ClockP_Handle) &clockStruct);
    }

    key = HwiP_disable();

    pendInProgress = true;

    while ((*pSemaphore == false) &&
           ((timeout == SemaphoreBinaryP_WAIT_FOREVER) ||
               ClockP_isActive((ClockP_Handle) &clockStruct))) {
        IdleFunc();
        HwiP_restore(key);
        key = HwiP_disable();

        if (doEnablePowerPolicy) {
            EnablePowerPolicy();
            doEnablePowerPolicy = false;
        }
    }

    pendInProgress = false;

    if (*pSemaphore == true) {
        *pSemaphore = false;
        status      = SemaphoreBinaryP_OK;
    } else {
        status = SemaphoreBinaryP_TIMEOUT;
    }

    HwiP_restore(key);

    ClockP_destruct(&clockStruct);

    return (status);
}

/*
 *  ======== SemaphoreBinaryP_pendNoTimeout ========
 */
SemaphoreBinaryP_Status SemaphoreBinaryP_pendNoTimeout(
    SemaphoreBinaryP_Handle handle)
{
    SemaphoreBinaryP_Struct *pSemaphore = (SemaphoreBinaryP_Struct *) handle;
    SemaphoreBinaryP_Status status;
    uintptr_t key;

    key = HwiP_disable();

    pendInProgress = true;

    while (*pSemaphore == false) {
        IdleFunc();
        HwiP_restore(key);
        key = HwiP_disable();

        if (doEnablePowerPolicy) {
            EnablePowerPolicy();
            doEnablePowerPolicy = false;
        }
    }

    pendInProgress = false;
    *pSemaphore    = false;
    status         = SemaphoreBinaryP_OK;

    HwiP_restore(key);

    return (status);
}

/*
 *  ======== SemaphoreBinaryP_postFromISR ========
 */
void SemaphoreBinaryP_postFromISR(SemaphoreBinaryP_Handle handle)
{
    SemaphoreBinaryP_Struct *pSemaphore = (SemaphoreBinaryP_Struct *) handle;

    *pSemaphore = true;

    if (pendInProgress && !doEnablePowerPolicy) {
        /* short-circuit potential policy invocation */
        doEnablePowerPolicy = DisablePowerPolicy();
    }
}

/*
 *  ======== SemaphoreBinaryP_post ========
 */
void SemaphoreBinaryP_post(SemaphoreBinaryP_Handle handle)
{
    SemaphoreBinaryP_Struct *pSemaphore = (SemaphoreBinaryP_Struct *) handle;
    uintptr_t key;

    key = HwiP_disable();

    *pSemaphore = true;

    if (HwiP_inISR() && pendInProgress && !doEnablePowerPolicy) {
        /* short-circuit potential policy invocation */
        doEnablePowerPolicy = DisablePowerPolicy();
    }

    HwiP_restore(key);
}
