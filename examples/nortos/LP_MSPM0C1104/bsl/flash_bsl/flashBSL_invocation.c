/*
 * Copyright (c) 2024, Texas Instruments Incorporated
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

#include "flashBSL_invocation.h"

/* Static functions */
static uint8_t GPIO_readPinState(
    GPIO_Regs *gpioReg, uint8_t pinNum, uint8_t padNum, uint8_t expPinState);
extern volatile BSL_Config *pBSLConfig;
extern volatile BCR_Config *pBCRConfig;

void invocation_check(void)
{
    blInvocationState_t invoke_status = blInvocation_checkBLInvokeCondition();

    if (invoke_status == BL_INVOCATION_TRUE) {
        FlashBSL_Reset_Handler();
    } else {
        uint32_t *appResetHandler =
            (uint32_t *) (MAIN_APP_START_ADDR + VTOR_RESET_HANDLER_OFFSET);
        appPointer FlashBSL_applicationStart =
            (appPointer) * (appResetHandler);

        /*
         * Before branch check if the address of reset handler
         * is a valid Flash address
         */
        if ((*((uint32_t *) MAIN_APP_RESET_VECTOR_ADDR) >=
                MAIN_APP_START_ADDR) &&
            (*((uint32_t *) MAIN_APP_RESET_VECTOR_ADDR) <
                (MAIN_APP_START_ADDR + DEVICE_FLASH_SIZE))) {
            FlashBSL_applicationStart();
        }
    }
    while (1)
        ;
}

/**
 * @brief Checks if the Bootloader invocation is requested through any of
 * the following 3 sources.
 * \n1. Blank device
 * \n2. GPIO pin request
 * \n3. Application request
 *
 * Updates the result if the Bootloader invocation request is
 * detected or not. And error status if there is any error detected during the check.
 */
blInvocationState_t blInvocation_checkBLInvokeCondition(void)
{
    volatile blInvocationState_t blInvocationState = BL_INVOCATION_FALSE;
    if ((&BSLConfig)->bootloaderMode != BSL_CFG_BOOTLOADER_MODE_EN) {
        return BL_INVOCATION_FALSE;
    }

    do {
#if BSL_BLANK_FLASH_INVOKE
        /* 1. Checks if the device is blank */
        blInvocationState = blInvocation_checkBlankDevice();
        if (blInvocationState == BL_INVOCATION_BLANK_DEVICE) {
            return BL_INVOCATION_TRUE;
        }
#endif

#if BSL_PIN_INVOKE
        /* 2. Checks if the GPIO pin is asserted */
        blInvocationState = blInvocation_checkPinCondition();
        if (blInvocationState == BL_INVOCATION_BSL_PIN) {
            return BL_INVOCATION_TRUE;
        }
#endif
        /* 3. Checks if BSL invocation is set by
         * User Application before Shutdown*/
        if (DL_SYSCTL_getShutdownStorageByte(BSL_APP_INVOCATION_MEM) ==
            BSL_APP_INVOCATION_HIGH) {
            DL_SYSCTL_setShutdownStorageByte(
                BSL_APP_INVOCATION_MEM, BSL_APP_INVOCATION_LOW);
            return BL_INVOCATION_TRUE;
        }

    } while (false);
    return BL_INVOCATION_FALSE;
}

/**
 * @brief Checks if the device is blank. Read all 0xFF or 0x00 at
 * IVT offset of Application(0x1800)UL and (0x1804)UL is considered as blank device.
 *
 * @return Returns the result if the Bootloader invocation condition is
 * detected or not
 */
blInvocationState_t blInvocation_checkBlankDevice(void)
{
    // Check if Application Reset vector exists
    if ((*((uint32_t *) (MAIN_APP_RESET_VECTOR_ADDR))) == BLANK_VALUE) {
        return BL_INVOCATION_BLANK_DEVICE;
    } else {
        return BL_INVOCATION_FALSE;
    }
}

/**
 * @brief Checks if the GPIO pin state matches the invocation condition
 * defined in Bootloader configuration memory. If the Bootloader configuration
 * is not programmed, or if the Bootloader configuration structure CRC is incorrect
 * then the pin details and invocation condition is taken from TRIM memory
 *
 * @return Returns the result if the Bootloader invocation condition is
 * detected or not. And error status if there is any error detected during the check.
 */

blInvocationState_t blInvocation_checkPinCondition(void)
{
    blInvocationState_t invocationState = BL_INVOCATION_FALSE;
    if ((((&SYSCTL->SOCLOCK)->SYSSTATUS & SYSCTL_SYSSTATUS_SHDNIOLOCK_MASK) !=
            SYSCTL_SYSSTATUS_SHDNIOLOCK_TRUE)) {
        GPIO_Regs *gpio;
        uint8_t pinNum;
        uint8_t padNum;
        uint8_t expPinState;
        uint8_t portIndex;

        uint8_t actPinState;
        volatile BSLInvokePin *pin;

        uint32_t len, i = ((uint32_t) 0);
        volatile uint32_t *ptr;
        expPinState =
            ((&BSLConfig)->pin.pinData0 & BSL_CFG_INVOKE_PIN_LEVEL_MASK) >>
            BSL_CFG_INVOKE_PIN_LEVEL_POS;
        padNum = ((&BSLConfig)->pin.pinData0 &
                     BSL_CFG_INVOKE_PIN_PINCM_INDEX_MASK) >>
                 BSL_CFG_INVOKE_PIN_PINCM_INDEX_POS;
        pinNum = ((&BSLConfig)->pin.pinData1 &
                     BSL_CFG_INVOKE_PIN_GPIO_PIN_NUM_MASK) >>
                 BSL_CFG_INVOKE_PIN_GPIO_PIN_NUM_POS;
        portIndex = ((&BSLConfig)->pin.pinData1 &
                        BSL_CFG_INVOKE_PIN_GPIO_PORT_NUM_MASK) >>
                    BSL_CFG_INVOKE_PIN_GPIO_PORT_NUM_POS;
        gpio = Boot_GPIO_RegArr[portIndex];

        actPinState = GPIO_readPinState(gpio, pinNum, padNum, expPinState);

        /* Check if the expected pin state matches with actual */
        if (actPinState == expPinState) {
            invocationState = BL_INVOCATION_BSL_PIN;
        }
    }
    return invocationState;
}

/**
 * @brief GPIO_readPinState configures the specific pin
 * with pull register and reads the pin state.
 *
 * @return Returns the pin state
 */
static inline uint8_t GPIO_readPinState(
    GPIO_Regs *gpioReg, uint8_t pinNum, uint8_t padNum, uint8_t expPinState)
{
    uint32_t pinMask = ((uint32_t) 1UL << pinNum);
    uint32_t pinState;
    uint8_t pincmIndex = padNum - 1U;

    /* Pull-Down register is configured if expected Pin State is High */
    /* Pull-Up register is configured if expected Pin State is Low */
    DL_GPIO_RESISTOR pull_resistor = DL_GPIO_RESISTOR_PULL_DOWN;
    if (expPinState == LOW_PIN_STATE) {
        pull_resistor = DL_GPIO_RESISTOR_PULL_UP;
    }

    /* Configure the IOMUX for the pin */
    DL_GPIO_initDigitalInputFeatures((uint32_t) pincmIndex,
        DL_GPIO_INVERSION_DISABLE, pull_resistor, DL_GPIO_HYSTERESIS_DISABLE,
        DL_GPIO_WAKEUP_DISABLE);

    /* Enable power to the module */
    DL_GPIO_enablePower(gpioReg);

    while (DL_GPIO_isPowerEnabled(gpioReg) != true) {
        /* Empty wait loop */
    }

    /* Disables output if enabled already */
    DL_GPIO_disableOutput(gpioReg, pinMask);

    /*
     * 2us delay has to be given for the pull up/down to settle.
     * ~1us delay will be given by the code execution.
     * ~1us is given by this 'for' loop
     * @note: Assuming the code is running at 32MHz clock
     * */
    uint8_t i = 0;
    for (i = 0; i < (uint32_t) 5; i++) {
        __NOP();
    }

    /* Read Pin State */
    pinState = DL_GPIO_readPins(gpioReg, pinMask);
    pinState = (pinState == pinMask) ? ((uint32_t) 1U) : ((uint32_t) 0U);

    /* Disable GPIO Input */
    DL_GPIO_reset(gpioReg);

    return (uint8_t) pinState;
}
