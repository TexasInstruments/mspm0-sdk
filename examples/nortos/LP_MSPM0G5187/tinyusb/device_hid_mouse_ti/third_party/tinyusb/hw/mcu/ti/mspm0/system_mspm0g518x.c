/******************************************************************************
*
* Copyright (C) 2017 Texas Instruments Incorporated - http://www.ti.com/
*
* Redistribution and use in source and binary forms, with or without
* modification, are permitted provided that the following conditions
* are met:
*
*  Redistributions of source code must retain the above copyright
*  notice, this list of conditions and the following disclaimer.
*
*  Redistributions in binary form must reproduce the above copyright
*  notice, this list of conditions and the following disclaimer in the
*  documentation and/or other materials provided with the
*  distribution.
*
*  Neither the name of Texas Instruments Incorporated nor the names of
*  its contributors may be used to endorse or promote products derived
*  from this software without specific prior written permission.
*
* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
* "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
* LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
* A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
* OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
* SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
* LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
* DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
* THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
* (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
* OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 ******************************************************************************/

#include <stdint.h>
#include <ti/devices/msp/msp.h>
#include <ti/driverlib/dl_common.h>

#if (DeviceFamily_ID == DeviceFamily_ID_MSPM0G518X)

#define QDIV 2
/*! PLL feedback loop input clock frequency [32MHz, 48MHz] */
#define    SYSCTL_SYSPLL_INPUT_FREQ_32_48_MHZ  0x41C40034
/*! 2 flash wait states */
#define    SYSCTL_FLASH_WAIT_STATE_2    0x00000200U

/*----------------------------------------------------------------------------
  Clock functions
 *----------------------------------------------------------------------------*/
void SystemCoreClockUpdate (void)            /* Get Core Clock Frequency      */
{
    /*
    * The accuracy of the USBFLL is sufficient to meet the accuracy requirements for USB in
    * device mode if it is in host mode then the PLL must be utilized alongside an external
    * crystal.
    */

    /* Low Power Mode is configured to be SLEEP0 */
    SYSCTL->SOCLOCK.BORTHRESHOLD = (uint32_t) SYSCTL_BORTHRESHOLD_LEVEL_BORMIN;

    /* Configure flash wait states to be 2 */
    DL_Common_updateReg(&SYSCTL->SOCLOCK.MCLKCFG, ((uint32_t) 0x00000200U), SYSCTL_MCLKCFG_FLASHWAIT_MASK);

    /* Configuring SYSOSC to operate at 32 MHz */
    DL_Common_updateReg(&SYSCTL->SOCLOCK.SYSOSCCFG, (uint32_t)SYSCTL_SYSOSCCFG_FREQ_SYSOSCBASE, SYSCTL_SYSOSCCFG_FREQ_MASK);

    /* Disable HFXT and SYSPLL */
    SYSCTL->SOCLOCK.HSCLKEN &= ~(SYSCTL_HSCLKEN_HFXTEN_MASK | SYSCTL_HSCLKEN_SYSPLLEN_MASK);

    /*
     * Set HFXT frquency range
     * The default frequency for the HFXT crystal on the TI evm will always be 40 MHz
     */
    DL_Common_updateReg(&SYSCTL->SOCLOCK.HFCLKCLKCFG, SYSCTL_HFCLKCLKCFG_HFXTRSEL_RANGE32TO48,
                        SYSCTL_HFCLKCLKCFG_HFXTRSEL_MASK);

    /*
     * Set the HFXT startup time to be 10, this value is set by default.
     * 1 -> 64us in startup time, since it is configured as 10 it will 640us
     */
    DL_Common_updateReg(&SYSCTL->SOCLOCK.HFCLKCLKCFG, (uint32_t)10, SYSCTL_HFCLKCLKCFG_HFXTTIME_MASK);

    /* Enable HFXT */
    SYSCTL->SOCLOCK.HSCLKEN |= SYSCTL_HSCLKEN_HFXTEN_ENABLE;

    /* Enable HFXT startup monitor */
    SYSCTL->SOCLOCK.HFCLKCLKCFG |= SYSCTL_HFCLKCLKCFG_HFCLKFLTCHK_ENABLE;

    /* Wait until HFXT is stable */
    while ((SYSCTL->SOCLOCK.CLKSTATUS & SYSCTL_CLKSTATUS_HFCLKGOOD_MASK) !=
            SYSCTL_CLKSTATUS_HFCLKGOOD_TRUE)
            ;

    /* Check that SYSPLL is disabled before configuration */
    while ((SYSCTL->SOCLOCK.CLKSTATUS & (SYSCTL_CLKSTATUS_SYSPLLOFF_TRUE))
            != SYSCTL_CLKSTATUS_SYSPLLOFF_TRUE)
            ;

    /* Set HFXT as SYSPLL reference clock */
    DL_Common_updateReg(&SYSCTL->SOCLOCK.SYSPLLCFG0, ((uint32_t)SYSCTL_SYSPLLCFG0_SYSPLLREF_HFCLK),
                        SYSCTL_SYSPLLCFG0_SYSPLLREF_MASK);

    /*
     * Set predivider PDIV (divides reference clock)
     * 40 MHz / 1 = 40 MHz
     */
    DL_Common_updateReg(&SYSCTL->SOCLOCK.SYSPLLCFG1, ((uint32_t)SYSCTL_SYSPLLCFG1_PDIV_REFDIV1),
                        SYSCTL_SYSPLLCFG1_PDIV_MASK);

    /* Before access TRIM we will save CPUSS CTL state and disable the cache */
    uint32_t cpussTemp = (CPUSS->CTL & (CPUSS_CTL_ICACHE_MASK | CPUSS_CTL_PREFETCH_MASK | CPUSS_CTL_LITEN_MASK));
    CPUSS->CTL = (CPUSS_CTL_PREFETCH_ENABLE | CPUSS_CTL_ICACHE_DISABLE | CPUSS_CTL_LITEN_ENABLE);

    /* Populate SYSPLLPARAM0/1 tuning registers from flash, based on input freq */
    SYSCTL->SOCLOCK.SYSPLLPARAM0 =
        *(volatile uint32_t *) ((uint32_t) SYSCTL_SYSPLL_INPUT_FREQ_32_48_MHZ);
    SYSCTL->SOCLOCK.SYSPLLPARAM1 =
        *(volatile uint32_t *) ((uint32_t) SYSCTL_SYSPLL_INPUT_FREQ_32_48_MHZ + (uint32_t) 0x4);

    /* Restore CPUSS state */
    CPUSS->CTL = cpussTemp;

    /*
     * Set feedback divider QDIV (multiplies to give output frequency) intended value
     * of QDIV is (QDIV - 1), if you want a QDIV of 3 then the value written will be 2
     * 40 MHz * 3 = 120 MHz
     */
    DL_Common_updateReg(&SYSCTL->SOCLOCK.SYSPLLCFG1, ((QDIV << SYSCTL_SYSPLLCFG1_QDIV_OFS) & SYSCTL_SYSPLLCFG1_QDIV_MASK),
                        SYSCTL_SYSPLLCFG1_QDIV_MASK);

#ifdef CFG_TUH_ENABLED
    /*
     * Write clock output dividers, enable outputs, and MCLK source to SYSPLLCFG0
     * Enable PLLCLK2 as on output with divider of 3
     * Enable PLLCLK1 as an output with divider of 2
     * PLLCLK2: (120 MHz * 2) / 3 = 80 MHz
     * PLLCLK1: 120 MHz / 2 = 60 MHz
     * PLLCLK0: Disabled
    */
    DL_Common_updateReg(&SYSCTL->SOCLOCK.SYSPLLCFG0,
            (((SYSCTL_SYSPLLCFG0_RDIVCLK2X_CLK2XDIV3) &
                SYSCTL_SYSPLLCFG0_RDIVCLK2X_MASK) |
            ((SYSCTL_SYSPLLCFG0_RDIVCLK1_CLK1DIV2) &
                SYSCTL_SYSPLLCFG0_RDIVCLK1_MASK) |
            ((SYSCTL_SYSPLLCFG0_ENABLECLK0_DISABLE << SYSCTL_SYSPLLCFG0_RDIVCLK0_OFS) &
                SYSCTL_SYSPLLCFG0_RDIVCLK0_MASK) |
                SYSCTL_SYSPLLCFG0_ENABLECLK2X_ENABLE | SYSCTL_SYSPLLCFG0_ENABLECLK1_ENABLE | SYSCTL_SYSPLLCFG0_ENABLECLK0_DISABLE |
            (uint32_t) SYSCTL_SYSPLLCFG0_MCLK2XVCO_ENABLE),
        (SYSCTL_SYSPLLCFG0_RDIVCLK2X_MASK | SYSCTL_SYSPLLCFG0_RDIVCLK1_MASK |
            SYSCTL_SYSPLLCFG0_RDIVCLK0_MASK |
            SYSCTL_SYSPLLCFG0_ENABLECLK2X_MASK |
            SYSCTL_SYSPLLCFG0_ENABLECLK1_MASK |
            SYSCTL_SYSPLLCFG0_ENABLECLK0_MASK |
            SYSCTL_SYSPLLCFG0_MCLK2XVCO_MASK));
#else
    /*
     * Write clock output dividers, enable outputs, and MCLK source to SYSPLLCFG0
     * Enable PLLCLK2 as an output with divider of 3
     * PLLCLK2: (120 MHz * 2) / 3 = 80 MHz
     * PLLCLK1: Disabled
     * PLLCLK0: Disabled
     */
    DL_Common_updateReg(&SYSCTL->SOCLOCK.SYSPLLCFG0,
        (((SYSCTL_SYSPLLCFG0_RDIVCLK2X_CLK2XDIV3) &
                SYSCTL_SYSPLLCFG0_RDIVCLK2X_MASK) |
            ((SYSCTL_SYSPLLCFG0_ENABLECLK1_DISABLE << SYSCTL_SYSPLLCFG0_RDIVCLK1_OFS) &
                SYSCTL_SYSPLLCFG0_RDIVCLK1_MASK) |
            ((SYSCTL_SYSPLLCFG0_ENABLECLK0_DISABLE << SYSCTL_SYSPLLCFG0_RDIVCLK0_OFS) &
                SYSCTL_SYSPLLCFG0_RDIVCLK0_MASK) |
                SYSCTL_SYSPLLCFG0_ENABLECLK2X_ENABLE | SYSCTL_SYSPLLCFG0_ENABLECLK1_DISABLE | SYSCTL_SYSPLLCFG0_ENABLECLK0_DISABLE |
            (uint32_t) SYSCTL_SYSPLLCFG0_MCLK2XVCO_ENABLE),
        (SYSCTL_SYSPLLCFG0_RDIVCLK2X_MASK | SYSCTL_SYSPLLCFG0_RDIVCLK1_MASK |
            SYSCTL_SYSPLLCFG0_RDIVCLK0_MASK |
            SYSCTL_SYSPLLCFG0_ENABLECLK2X_MASK |
            SYSCTL_SYSPLLCFG0_ENABLECLK1_MASK |
            SYSCTL_SYSPLLCFG0_ENABLECLK0_MASK |
            SYSCTL_SYSPLLCFG0_MCLK2XVCO_MASK));
#endif
    /* Enable SYSPLL */
    SYSCTL->SOCLOCK.HSCLKEN |= SYSCTL_HSCLKEN_SYSPLLEN_ENABLE;

    /* Wait until SYSPLL startup is stabilized */
    while ((SYSCTL->SOCLOCK.CLKSTATUS & SYSCTL_CLKSTATUS_SYSPLLGOOD_MASK) !=
            SYSCTL_CLKSTATUS_SYSPLLGOOD_TRUE)
            ;

    /* Polling to ensure all clocks are stable */
    while ((SYSCTL->SOCLOCK.CLKSTATUS & (SYSCTL_CLKSTATUS_HFCLKGOOD_MASK |
            SYSCTL_CLKSTATUS_SYSPLLGOOD_MASK | SYSCTL_CLKSTATUS_LFOSCGOOD_MASK)) ==
            (SYSCTL_CLKSTATUS_HFCLKGOOD_FALSE | SYSCTL_CLKSTATUS_SYSPLLGOOD_FALSE |
            SYSCTL_RIS_HFCLKGOOD_FALSE))
            ;

    /*
     * Divide ULPCLK by 2 so it stays within its limit of 40 MHz
     * 80 MHz / 2 = 40 MHz
     */
    DL_Common_updateReg(&SYSCTL->SOCLOCK.MCLKCFG, SYSCTL_MCLKCFG_UDIV_DIVIDE2,
                        SYSCTL_MCLKCFG_UDIV_MASK);

    /* Configure HSCLK so it is sourced from SYSPLL */
    SYSCTL->SOCLOCK.HSCLKCFG = SYSCTL_HSCLKCFG_HSCLKSEL_SYSPLL;

    /* Verify HSCLK source is valid */
    while ((SYSCTL->SOCLOCK.CLKSTATUS & SYSCTL_CLKSTATUS_HSCLKGOOD_MASK) !=
            SYSCTL_CLKSTATUS_HSCLKGOOD_TRUE)
            ;

    /* Switch from MCLK to HSCLK */
    SYSCTL->SOCLOCK.MCLKCFG |= SYSCTL_MCLKCFG_USEHSCLK_ENABLE;

    /* Verify switch was succesful */
    while ((SYSCTL->SOCLOCK.CLKSTATUS & SYSCTL_CLKSTATUS_HSCLKMUX_MASK) !=
            SYSCTL_CLKSTATUS_HSCLKMUX_HSCLK)
            ;

#ifdef CFG_TUH_ENABLED
    /* Once SYSPLL CLK1 has been configured at 60 MHz switch USBCLKSRC to PLLCLK1 */
    SYSCTL->SOCLOCK.GENCLKCFG |= SYSCTL_GENCLKCFG_USBCLKSRC_SYSPLLOUT1;
#else
    /* Configuring USBFLL to be sourced from SOF */
    SYSCTL->SOCLOCK.USBFLLCTL &= ~(SYSCTL_USBFLLCTL_REFSEL_LFCLK);

    /* Enabling USB oscillator clock */
    SYSCTL->SOCLOCK.USBFLLCTL |= (SYSCTL_USBFLLCTL_CLKEN_TRUE);

    /* Polling for completion of USB oscillator initializiation */
    while((SYSCTL->SOCLOCK.USBFLLSTAT & SYSCTL_USBFLLSTAT_CLKRDY_MASK) !=
            SYSCTL_USBFLLSTAT_CLKRDY_TRUE)
                ;

    /* Enabling USBFLL */
    SYSCTL->SOCLOCK.USBFLLCTL |= SYSCTL_USBFLLCTL_FLLEN_TRUE;

    /* Configuring USBCLKSRC to use USBFLL */
    SYSCTL->SOCLOCK.GENCLKCFG &= ~(SYSCTL_GENCLKCFG_USBCLKSRC_SYSPLLOUT1);
#endif
}
#endif /* DeviceFamily_ID_MSPM0G518X */