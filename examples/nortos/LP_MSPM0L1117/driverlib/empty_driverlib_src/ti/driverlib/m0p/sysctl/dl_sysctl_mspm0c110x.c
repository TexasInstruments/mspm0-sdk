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

#include <stdint.h>
#include <ti/devices/DeviceFamily.h>

#if (DeviceFamily_PARENT == DeviceFamily_PARENT_MSPM0C110X)

#include <ti/driverlib/m0p/sysctl/dl_sysctl_mspm0c110x.h>

void DL_SYSCTL_switchMCLKfromSYSOSCtoLFCLK(bool disableSYSOSC)
{
    // Set SYSOSC back to base frequency if left enabled
    if (disableSYSOSC == false) {
        DL_SYSCTL_setSYSOSCFreq(DL_SYSCTL_SYSOSC_FREQ_BASE);
        // Do not set both bits
        SYSCTL->SOCLOCK.SYSOSCCFG &= ~SYSCTL_SYSOSCCFG_DISABLE_ENABLE;
        SYSCTL->SOCLOCK.MCLKCFG |= SYSCTL_MCLKCFG_USELFCLK_ENABLE;
    } else {
        // Do not set both bits
        SYSCTL->SOCLOCK.MCLKCFG &= ~SYSCTL_MCLKCFG_USELFCLK_ENABLE;
        SYSCTL->SOCLOCK.SYSOSCCFG |= SYSCTL_SYSOSCCFG_DISABLE_ENABLE;
    }

    // Verify LFCLK -> MCLK
    while ((DL_SYSCTL_getClockStatus() & SYSCTL_CLKSTATUS_CURMCLKSEL_MASK) !=
           DL_SYSCTL_CLK_STATUS_MCLK_SOURCE_LFCLK) {
        ;
    }
}

void DL_SYSCTL_switchMCLKfromLFCLKtoSYSOSC(void)
{
    // Only one should have been set, but clear both because unknown incoming state
    // Clear SYSOSCCFG.DISABLE to get SYSOSC running again
    // Clear MCLKCFG.USELFCLK to switch MCLK source from LFCLK to SYSOSC
    SYSCTL->SOCLOCK.SYSOSCCFG &= ~SYSCTL_SYSOSCCFG_DISABLE_ENABLE;
    SYSCTL->SOCLOCK.MCLKCFG &= ~SYSCTL_MCLKCFG_USELFCLK_ENABLE;

    // Verify SYSOSC -> MCLK
    while ((DL_SYSCTL_getClockStatus() & SYSCTL_CLKSTATUS_CURMCLKSEL_MASK) ==
           DL_SYSCTL_CLK_STATUS_MCLK_SOURCE_LFCLK) {
        ;
    }
}

void DL_SYSCTL_switchMCLKfromSYSOSCtoHSCLK(void)
{
    // Switch MCLK to HSCLK
    SYSCTL->SOCLOCK.MCLKCFG |= SYSCTL_MCLKCFG_USEHSCLK_ENABLE;

    // Verify HSCLK -> MCLK
    while ((DL_SYSCTL_getClockStatus() & SYSCTL_CLKSTATUS_HSCLKMUX_MASK) !=
           DL_SYSCTL_CLK_STATUS_MCLK_SOURCE_HSCLK) {
        ;
    }
}

void DL_SYSCTL_switchMCLKfromHSCLKtoSYSOSC(void)
{
    // Switch MCLK to SYSOSC
    SYSCTL->SOCLOCK.MCLKCFG &= ~SYSCTL_MCLKCFG_USEHSCLK_ENABLE;

    // Verify SYSOSC -> MCLK
    while ((DL_SYSCTL_getClockStatus() & SYSCTL_CLKSTATUS_HSCLKMUX_MASK) ==
           DL_SYSCTL_CLK_STATUS_MCLK_SOURCE_HSCLK) {
        ;
    }
}

DL_SYSCTL_POWER_POLICY_RUN_SLEEP DL_SYSCTL_getPowerPolicyRUNSLEEP(void)
{
    DL_SYSCTL_POWER_POLICY_RUN_SLEEP policy =
        DL_SYSCTL_POWER_POLICY_RUN_SLEEP_NOT_ENABLED;

    // Check if SLEEP is enabled
    if ((SCB->SCR & SCB_SCR_SLEEPDEEP_Msk) != SCB_SCR_SLEEPDEEP_Msk) {
        // Check which policy is enabled
        if ((SYSCTL->SOCLOCK.MCLKCFG & SYSCTL_MCLKCFG_USELFCLK_MASK) ==
            SYSCTL_MCLKCFG_USELFCLK_ENABLE) {
            policy = DL_SYSCTL_POWER_POLICY_RUN_SLEEP1;
        } else if ((SYSCTL->SOCLOCK.SYSOSCCFG &
                       SYSCTL_SYSOSCCFG_DISABLESTOP_MASK) ==
                   SYSCTL_SYSOSCCFG_DISABLESTOP_ENABLE) {
            policy = DL_SYSCTL_POWER_POLICY_RUN_SLEEP2;
        } else {
            policy = DL_SYSCTL_POWER_POLICY_RUN_SLEEP0;
        }
    }
    return policy;
}

DL_SYSCTL_POWER_POLICY_STOP DL_SYSCTL_getPowerPolicySTOP(void)
{
    DL_SYSCTL_POWER_POLICY_STOP policy =
        DL_SYSCTL_POWER_POLICY_STOP_NOT_ENABLED;

    // Check if STOP is enabled
    if ((SYSCTL->SOCLOCK.PMODECFG == SYSCTL_PMODECFG_DSLEEP_STOP) &&
        (SCB->SCR & SCB_SCR_SLEEPDEEP_Msk) == SCB_SCR_SLEEPDEEP_Msk) {
        // Check which policy is enabled
        if ((SYSCTL->SOCLOCK.SYSOSCCFG & SYSCTL_SYSOSCCFG_DISABLESTOP_MASK) ==
            SYSCTL_SYSOSCCFG_DISABLESTOP_ENABLE) {
            policy = DL_SYSCTL_POWER_POLICY_STOP2;
        } else {
            policy = DL_SYSCTL_POWER_POLICY_STOP0;
        }
    }
    return policy;
}

DL_SYSCTL_POWER_POLICY_STANDBY DL_SYSCTL_getPowerPolicySTANDBY(void)
{
    DL_SYSCTL_POWER_POLICY_STANDBY policy =
        DL_SYSCTL_POWER_POLICY_STANDBY_NOT_ENABLED;

    // Check if STOP is enabled
    if ((SYSCTL->SOCLOCK.PMODECFG == SYSCTL_PMODECFG_DSLEEP_STANDBY) &&
        (SCB->SCR & SCB_SCR_SLEEPDEEP_Msk) == SCB_SCR_SLEEPDEEP_Msk) {
        // Check which policy is enabled
        if ((SYSCTL->SOCLOCK.MCLKCFG & SYSCTL_MCLKCFG_STOPCLKSTBY_MASK) ==
            SYSCTL_MCLKCFG_STOPCLKSTBY_ENABLE) {
            policy = DL_SYSCTL_POWER_POLICY_STANDBY1;
        } else {
            policy = DL_SYSCTL_POWER_POLICY_STANDBY0;
        }
    }
    return policy;
}

void DL_SYSCTL_configFCC(DL_SYSCTL_FCC_TRIG_TYPE trigLvl,
    DL_SYSCTL_FCC_TRIG_SOURCE trigSrc, DL_SYSCTL_FCC_CLOCK_SOURCE clkSrc)
{
    DL_Common_updateReg(&SYSCTL->SOCLOCK.GENCLKCFG,
        (uint32_t) trigLvl | (uint32_t) trigSrc | (uint32_t) clkSrc,
        SYSCTL_GENCLKCFG_FCCLVLTRIG_MASK | SYSCTL_GENCLKCFG_FCCTRIGSRC_MASK |
            SYSCTL_GENCLKCFG_FCCSELCLK_MASK);
}

#endif /* DeviceFamily_PARENT_MSPM0C110X */
