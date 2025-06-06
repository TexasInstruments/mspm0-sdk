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
 *
 */


#include "dali_103_command.h"


static inline void DALI_setResponse(uint8_t respByte)
{
    dali_gpio.Tx.frame = BACKWARD_FRAME;
    dali_gpio.Tx.dataArr[0] = respByte;
    dali_gpio.Tx.toSend = true;
    dali_gpio.Tx.settlingTime = BACKWARD_FRAME_SETTLING_MIN;    
}

static uint32_t DALI_getCounterValue(uint32_t time)
{
    return time/((uint32_t) TIMER_FR_PERIOD);
}

void DALI_103_identifyDevice(void)
{
    /* TEMPLATE FOR IDENTIFY DEVICE */
}

void DALI_103_resetPowerCycleSeen(void)
{
    /* Check for Send Twice Commands */
    if(dali_gpio.Rx.isSendTwiceCmd == false)
    {
        return;
    }

    gControlDeviceVar1.powerCycleSeen = false;
}


void DALI_103_reset(void)
{
    /* Check for Send Twice Commands */
    if(dali_gpio.Rx.isSendTwiceCmd == false)
    {
        return;
    }

    gControlDeviceVar1.resetState = true;

    gTickCounter.resetCounter = DALI_getCounterValue(RESET_CMD_COUNTER);
    DL_Timer_startCounter(TIMER_FR_INST);

    /* Reset Control Device Vars */
    gControlDeviceVar1.deviceGroups = DEVICE_GROUP_RESET;
    gControlDeviceVar1.searchAddress = SEARCH_ADDR_RESET;
    gControlDeviceVar1.randomAddress = RANDOM_ADDRESS_RESET;
    gControlDeviceVar1.quiescentMode = QUIESCENT_MODE_RESET;
    gControlDeviceVar1.writeEnableState = WRITE_ENABLE_RESET;
    gControlDeviceVar1.powerCycleSeen = POWER_CYC_SEEN_RESET;
    gControlDeviceVar1.initialisationState = INIT_STATE_RESET;
    gControlDeviceVar1.applicationControllerError = APP_CONTROLLER_ERROR_RESET;
    gControlDeviceVar1.inputDeviceError = INPUT_DEVICE_ERROR_RESET;
    gControlDeviceVar1.resetState = RESET_STATE_RESET;

    for(int i = 0; i < DEVICE_INSTANCE_COUNT ; i++)
    {
        gControlDeviceVar1.instance[i].instanceGroups0 = INST_GROUP0_RESET;
        gControlDeviceVar1.instance[i].instanceGroups1 = INST_GROUP1_RESET;
        gControlDeviceVar1.instance[i].instanceGroups2 = INST_GROUP2_RESET;
        gControlDeviceVar1.instance[i].eventFilter = EVENT_FILTER_RESET;
        gControlDeviceVar1.instance[i].eventScheme = EVENT_SCHEME_RESET;
        gControlDeviceVar1.instance[i].instanceError = INST_ERROR_RESET;

    }

    /* Stop counter */
    gTickCounter.initialisationCounter = 0;
    gTickCounter.quiescentMode = 0;

}

void DALI_103_resetMemBank(void)
{
    /* TEMPLATE for Reset Memory Bank */
}

void DALI_103_setShortAddr(void)
{
    /* Check for Send Twice Commands */
    if(dali_gpio.Rx.isSendTwiceCmd == false)
    {
        return;
    }

    /* If value sent is MASK, short address should be MASK */
    if(gControlDeviceVar1.DTR0 == MASK)
    {
        gControlDeviceVar1.shortAddress = MASK;
    }
    /*
     * otherwise if DTR0 has correctly formatted address value,
     * set to new value
     */
    else if(gControlDeviceVar1.DTR0 < 0x40){
        gControlDeviceVar1.shortAddress = gControlDeviceVar1.DTR0;
    }
    /*
     * Otherwise maintain old value
     */
    else{
        /* Do nothing */
    }
}

void DALI_103_enableWriteMemory(void)
{
    /* Check for Send Twice Commands */
    if(dali_gpio.Rx.isSendTwiceCmd == false)
    {
        return;
    }

    gControlDeviceVar1.writeEnableState = ENABLED;
}

void DALI_103_enableAppController(void)
{
    /* Check for Send Twice Commands */
    if(dali_gpio.Rx.isSendTwiceCmd == false)
    {
        return;
    }

    if(gControlDeviceVar1.applicationControllerPresent)
    {
        gControlDeviceVar1.applicationActive = true;
    }
}

void DALI_103_disableAppController(void)
{
    /* Check for Send Twice Commands */
    if(dali_gpio.Rx.isSendTwiceCmd == false)
    {
        return;
    }

    if(gControlDeviceVar1.applicationControllerPresent)
    {
        gControlDeviceVar1.applicationActive = false;
    }
}

void DALI_103_setOperatingMode(void)
{
    /* Check for Send Twice Commands */
    if(dali_gpio.Rx.isSendTwiceCmd == false)
    {
        return;
    }

    /* TEMPLATE FOR SET OPERATING MODE */
}

void DALI_103_addDeviceGroups_0_15(void)
{
    /* Check for Send Twice Commands */
    if(dali_gpio.Rx.isSendTwiceCmd == false)
    {
        return;
    }

    gControlDeviceVar1.deviceGroups |= ((gControlDeviceVar1.DTR2 << 8) | gControlDeviceVar1.DTR1);
}

void DALI_103_addDeviceGroups_16_31(void)
{
    /* Check for Send Twice Commands */
    if(dali_gpio.Rx.isSendTwiceCmd == false)
    {
        return;
    }

    gControlDeviceVar1.deviceGroups |= (((gControlDeviceVar1.DTR2 << 8) | gControlDeviceVar1.DTR1) << 16);
}

void DALI_103_removeDeviceGroups_0_15(void)
{
    /* Check for Send Twice Commands */
    if(dali_gpio.Rx.isSendTwiceCmd == false)
    {
        return;
    }

    gControlDeviceVar1.deviceGroups &= ~(( gControlDeviceVar1.DTR2 << 8) | (gControlDeviceVar1.DTR1));
}

void DALI_103_removeDeviceGroups_16_31(void)
{
    /* Check for Send Twice Commands */
    if(dali_gpio.Rx.isSendTwiceCmd == false)
    {
        return;
    }

    gControlDeviceVar1.deviceGroups &= ~((( gControlDeviceVar1.DTR2 << 8) | (gControlDeviceVar1.DTR1)) << 16);
}

void DALI_103_startQuiescentMode(void)
{
    /* Check for Send Twice Commands */
    if(dali_gpio.Rx.isSendTwiceCmd == false)
    {
        return;
    }

    gControlDeviceVar1.quiescentMode = ENABLED;
    gTickCounter.quiescentMode = DALI_getCounterValue(QUIESCENT_MODE_COUNTER);
    DL_Timer_startCounter(TIMER_FR_INST);

}

void DALI_103_stopQuiescentMode(void)
{
    /* Check for Send Twice Commands */
    if(dali_gpio.Rx.isSendTwiceCmd == false)
    {
        return;
    }

    gControlDeviceVar1.quiescentMode = DISABLED;

    /* Stop Counter for Quiescent Mode */
    gTickCounter.quiescentMode = 0;
}

void DALI_103_enablePowerCycNotification(void)
{
    /* Check for Send Twice Commands */
    if(dali_gpio.Rx.isSendTwiceCmd == false)
    {
        return;
    }

    gControlDeviceVar1.powerCycleNotification = ENABLED;
}

void DALI_103_disablePowerCycNotification(void)
{
    /* Check for Send Twice Commands */
    if(dali_gpio.Rx.isSendTwiceCmd == false)
    {
        return;
    }

    gControlDeviceVar1.powerCycleNotification = DISABLED;
}

void DALI_103_savePersistentVariables(void)
{
    /* Check for Send Twice Commands */
    if(dali_gpio.Rx.isSendTwiceCmd == false)
    {
        return;
    }

    uint32_t flashVars[EEPROM_EMULATION_DATA_SIZE / sizeof(uint32_t)];

    flashVars[0] = gControlDeviceVar1.shortAddress;
    flashVars[1] = gControlDeviceVar1.deviceGroups;
    flashVars[2] = gControlDeviceVar1.randomAddress;
    flashVars[3] = gControlDeviceVar1.operatingMode;
    flashVars[4] = gControlDeviceVar1.applicationControllerPresent;
    flashVars[5] = gControlDeviceVar1.powerCycleNotification;
    
    uint8_t flashVarsIndex = 6;
    for(int i = 0; i < gControlDeviceVar1.numberOfInstances ; i++)
    {   
        flashVars[flashVarsIndex++] = gControlDeviceVar1.instance[i].instanceGroups0;
        flashVars[flashVarsIndex++] = gControlDeviceVar1.instance[i].instanceGroups1;
        flashVars[flashVarsIndex++] = gControlDeviceVar1.instance[i].instanceGroups2;
        flashVars[flashVarsIndex++] = gControlDeviceVar1.instance[i].instanceActive;
        flashVars[flashVarsIndex++] = gControlDeviceVar1.instance[i].eventFilter;
        flashVars[flashVarsIndex++] = gControlDeviceVar1.instance[i].eventScheme;
        flashVars[flashVarsIndex++] = gControlDeviceVar1.instance[i].eventPriority;
    }

    EEPROM_TypeA_writeData(&flashVars[0]);
    if(gEEPROMTypeAEraseFlag){
        EEPROM_TypeA_eraseLastSector();
        gEEPROMTypeAEraseFlag = 0;
    }
}

/* Special Commands */
void DALI_103_terminate(void)
{
    gControlDeviceVar1.initialisationState = DISABLED;
    gTickCounter.initialisationCounter = 0;
}

void DALI_103_initialise(void)
{
    /* Check for Send Twice Commands */
    if(dali_gpio.Rx.isSendTwiceCmd == false)
    {
        return;
    }

    /* Check if the command is addressed to this device */
    if((dali_gpio.Rx.dataArr[2] != MASK) && 
       (dali_gpio.Rx.dataArr[2] != gControlDeviceVar1.shortAddress) && 
       ((dali_gpio.Rx.dataArr[2] == 0x7F) && (gControlDeviceVar1.shortAddress != MASK)))
    {
        return;
    }

    gControlDeviceVar1.initialisationState = ENABLED;
    gTickCounter.initialisationCounter = DALI_getCounterValue(INITIALISE_STATE_COUNTER);
    DL_Timer_startCounter(TIMER_FR_INST);

    return;
}

void DALI_103_randomise(void)
{
    /* Check for Send Twice Commands */
    if(dali_gpio.Rx.isSendTwiceCmd == false)
    {
        return;
    }

    /* Ignore if device is not in Initialisation state */
    if(gControlDeviceVar1.initialisationState == DISABLED)
    {
        return ;
    }

    while (!DL_TRNG_isCaptureReady(TRNG))
        ;
    DL_TRNG_clearInterruptStatus(
        TRNG, DL_TRNG_INTERRUPT_CAPTURE_RDY_EVENT);

    gControlDeviceVar1.randomAddress = (DL_TRNG_getCapture(TRNG) & 0xFFFFFF);

    //Handle case where randomAddress == 0xFFFFFF

    return ;
}

void DALI_103_compare(void)
{
    /* Ignore if device is not in Initialisation state */
    if(gControlDeviceVar1.initialisationState != ENABLED)
    {
        return ;
    }

    if(gControlDeviceVar1.randomAddress <= gControlDeviceVar1.searchAddress)
    {
        DALI_setResponse(YES);
    }

    return;
}

void DALI_103_withdraw(void)
{
    /* Ignore if device is not in Initialisation state AND randomAddr is not equal to searchAddr */
    if(gControlDeviceVar1.initialisationState == DISABLED &&
       gControlDeviceVar1.randomAddress == gControlDeviceVar1.searchAddress)
    {
        return ;
    }

    gControlDeviceVar1.initialisationState = WITHDRAWN;
    
    /* Stopping Counter for Initialisation state */
    gTickCounter.initialisationCounter = 0;
}

void DALI_103_searchAddrH(void)
{
    /* Ignore if device is not in Initialisation state */
    if(gControlDeviceVar1.initialisationState == DISABLED)
    {
        return ;
    }

    gControlDeviceVar1.searchAddress &= 0x00FFFF;
    gControlDeviceVar1.searchAddress |= (dali_gpio.Rx.dataArr[2] << 16);

    return ;
}

void DALI_103_searchAddrM(void)
{
    /* Ignore if device is not in Initialisation state */
    if(gControlDeviceVar1.initialisationState == DISABLED)
    {
        return ;
    }

    gControlDeviceVar1.searchAddress &= 0xFF00FF;
    gControlDeviceVar1.searchAddress |= (dali_gpio.Rx.dataArr[2] << 8);

    return ;
}

void DALI_103_searchAddrL(void)
{
    /* Ignore if device is not in Initialisation state */
    if(gControlDeviceVar1.initialisationState == DISABLED)
    {
        return ;
    }

    gControlDeviceVar1.searchAddress &= 0xFFFF00;
    gControlDeviceVar1.searchAddress |= (dali_gpio.Rx.dataArr[2]);

    return ;
}

void DALI_103_programShortAddr(void)
{
    /* Ignore if device is not in Initialisation state or Withdrawn State */
    if(gControlDeviceVar1.initialisationState == DISABLED)
    {
        return ;
    }

    if(gControlDeviceVar1.randomAddress != gControlDeviceVar1.searchAddress)
    {
        return;
    }

    if((dali_gpio.Rx.dataArr[2] < 0x40) || (dali_gpio.Rx.dataArr[2] == MASK))
    {
        gControlDeviceVar1.shortAddress = dali_gpio.Rx.dataArr[2];
    }

    return;
}

void DALI_103_verifyShortAddr(void)
{
    /* Ignore if device is not in Initialisation state or Withdrawn State */
    if(gControlDeviceVar1.initialisationState == DISABLED)
    {
        return ;
    }

    if(gControlDeviceVar1.shortAddress == dali_gpio.Rx.dataArr[2])
    {
        DALI_setResponse(YES);
    }    

    return;
}

void DALI_103_queryShortAddr(void)
{
    /* Ignore if Initialisation state is Disabled */
    if(gControlDeviceVar1.initialisationState == DISABLED)
    {
        return ;
    }

    /* Ignore if randomAddr is not equal to searchAddr */
    if(gControlDeviceVar1.randomAddress != gControlDeviceVar1.searchAddress)
    {
        return ;
    }

    DALI_setResponse(gControlDeviceVar1.shortAddress);

    return;    
}

void DALI_103_writeMemLocation(void)
{
    /* TEMPLATE for Write Memory Location */
}

void DALI_103_writeMemLocation_NoReply(void)
{
    /* TEMPLATE for Write Memory Location - No Reply */
}

void DALI_103_DTR0(void)
{
    gControlDeviceVar1.DTR0 = dali_gpio.Rx.dataArr[2];
}

void DALI_103_DTR1(void)
{
    gControlDeviceVar1.DTR1 = dali_gpio.Rx.dataArr[2];
}

void DALI_103_DTR2(void)
{
    gControlDeviceVar1.DTR2 = dali_gpio.Rx.dataArr[2];
}

void DALI_103_directWriteMem(void)
{
    /* TEMPLATE FOR Direct Write Memory */
}

void DALI_103_DTR1_DTR0(void)
{
    gControlDeviceVar1.DTR1 = dali_gpio.Rx.dataArr[1];
    gControlDeviceVar1.DTR0 = dali_gpio.Rx.dataArr[2];
}

void DALI_103_DTR2_DTR1(void)
{
    gControlDeviceVar1.DTR2 = dali_gpio.Rx.dataArr[1];
    gControlDeviceVar1.DTR1 = dali_gpio.Rx.dataArr[2];  
}


void DALI_103_sendTestFrame(void)
{
    /* Data should be interpreted as (CTARRPPPb)*/
    uint8_t data = dali_gpio.Rx.dataArr[2];

    uint8_t Cbyte = (data & 0x80);
    uint8_t Tbyte = (data & 0x40);
    uint8_t Abyte = (data & 0x20);
    uint8_t Rbyte = (data & 0x18);
    uint8_t Pbyte = (data & 0x07);

    /* Ignore the instruction if */
    if( (Cbyte != 0) || (Pbyte > 5) || (Pbyte < 1) || (Abyte && (gControlDeviceVar1.applicationControllerPresent == false)))
    {
        return ;
    } 

    if(Abyte == 0)
    {
        dali_gpio.Tx.frame = FORWARD_FRAME_24;
        dali_gpio.Tx.dataSize = 3;
        dali_gpio.Tx.dataArr[0] = gControlDeviceVar1.DTR0;
        dali_gpio.Tx.dataArr[1] = gControlDeviceVar1.DTR1;
        dali_gpio.Tx.dataArr[2] = gControlDeviceVar1.DTR2;
    }

    else
    {
        dali_gpio.Tx.frame = FORWARD_FRAME_16;
        dali_gpio.Tx.dataSize = 2;
        dali_gpio.Tx.dataArr[0] = gControlDeviceVar1.DTR0;
        dali_gpio.Tx.dataArr[1] = gControlDeviceVar1.DTR1;
    }

    if(Pbyte == 1)
        dali_gpio.Tx.settlingTime = FORWARD_FRAME_P1_SETTLING_MIN;
    
    else if(Pbyte == 2)
        dali_gpio.Tx.settlingTime = FORWARD_FRAME_P2_SETTLING_MIN;

    else if(Pbyte == 3)
        dali_gpio.Tx.settlingTime = FORWARD_FRAME_P3_SETTLING_MIN;
    
    else if(Pbyte == 4)
        dali_gpio.Tx.settlingTime = FORWARD_FRAME_P4_SETTLING_MIN;
    
    else if(Pbyte == 5)
        dali_gpio.Tx.settlingTime = FORWARD_FRAME_P5_SETTLING_MIN;
    
    
    /* Transmitting First Forward Frame */
    while(dali_gpio.Tx.status != TX_TRANSMIT_SUCCESS)
    {
        /* Wait until bus is idle */
        while(!((dali_gpio.Tx.status == TX_IDLE) && (dali_gpio.Rx.isActive == false )))
        {};

        /* Check for Settling Time */
        while((dali_gpio.Rx.isActive == true)
                ||
              (DL_TimerG_getTimerCount(SETTLING_TIMER_INST)*SETTLING_TIMER_TICK < dali_gpio.Tx.settlingTime))
        {};

        DALI_TX_transmitFrame();
    }

    if(Tbyte)
    {
        dali_gpio.Tx.settlingTime = FORWARD_FRAME_P1_SETTLING_MIN;
    }

    while(Rbyte--)
    {
        while(dali_gpio.Tx.status != TX_TRANSMIT_SUCCESS)
        {
            /* Wait until bus is idle */
            while(!((dali_gpio.Tx.status == TX_IDLE) && (dali_gpio.Rx.isActive == false )))
            {};

            /* Check for Settling Time */
            while((dali_gpio.Rx.isActive == true)
                    ||
                (DL_TimerG_getTimerCount(SETTLING_TIMER_INST)*SETTLING_TIMER_TICK < dali_gpio.Tx.settlingTime))
            {};

            DALI_TX_transmitFrame();
        }

        while(dali_gpio.Tx.status != TX_IDLE)
        {};
    }

    dali_gpio.Tx.toSend = false;

    return;
}


/* Device Query Instructions */
void DALI_103_query_deviceStatus(void)
{
    uint8_t respByte = 0;

    respByte |= (gControlDeviceVar1.inputDeviceError == true ? 1 : 0);
    respByte |= (gControlDeviceVar1.quiescentMode == ENABLED ? 1 : 0) << 1;
    respByte |= (gControlDeviceVar1.shortAddress == MASK ? 1 : 0) << 2;
    respByte |= (gControlDeviceVar1.applicationActive == true ? 1 : 0) << 3;
    respByte |= (gControlDeviceVar1.applicationControllerError == true ? 1 : 0) << 4;
    respByte |= (gControlDeviceVar1.powerCycleSeen == true ? 1 : 0) << 5;
    respByte |= (gControlDeviceVar1.resetState == true ? 1 : 0) << 6;

    DALI_setResponse(respByte);

    return ;
}

void DALI_103_query_appControllerError(void)
{
    if(gControlDeviceVar1.applicationControllerError)
    {
        DALI_setResponse(YES);
    }

    return;
}

void DALI_103_query_inputDeviceError(void)
{
    if(gControlDeviceVar1.inputDeviceError)
    {
        DALI_setResponse(YES);
    }
    
    return;
}

void DALI_103_query_missingShortAddr(void)
{
    if(gControlDeviceVar1.shortAddress == MASK)
    {
        DALI_setResponse(YES);
    }

    return;
}

void DALI_103_query_versionNumber(void)
{
    /* TEMPLATE for Query Version Number */
}

void DALI_103_query_numberOfInstances(void)
{
    DALI_setResponse(gControlDeviceVar1.numberOfInstances);

    return;
}

void DALI_103_query_DTR0(void)
{
    DALI_setResponse(gControlDeviceVar1.DTR0);

    return ;
}

void DALI_103_query_DTR1(void)
{
    DALI_setResponse(gControlDeviceVar1.DTR1);

    return ;
}

void DALI_103_query_DTR2(void)
{
    DALI_setResponse(gControlDeviceVar1.DTR2);

    return ;
}

void DALI_103_query_randomAddrH(void)
{
    uint8_t respByte = ((gControlDeviceVar1.randomAddress & (0xFF0000)) >> 16);

    DALI_setResponse(respByte);
    return;
}

void DALI_103_query_randomAddrM(void)
{
    uint8_t respByte = ((gControlDeviceVar1.randomAddress & (0x00FF00)) >> 8);

    DALI_setResponse(respByte);
    return;
}

void DALI_103_query_randomAddrL(void)
{
    uint8_t respByte = ((gControlDeviceVar1.randomAddress & (0x0000FF)));

    DALI_setResponse(respByte);
    return;
}

void DALI_103_query_readMemLocation(void)
{
    /* TEMPLATE for Query Read Memory Location */
}

void DALI_103_query_appControlEnabled(void)
{
    if(gControlDeviceVar1.applicationActive)
    {
        DALI_setResponse(YES);
    }

    return;
}

void DALI_103_query_operatingMode(void)
{
    DALI_setResponse(gControlDeviceVar1.operatingMode);

    return;
}

void DALI_103_query_manufacturerSpecificMode(void)
{
    if((gControlDeviceVar1.operatingMode >= 0x80) && (gControlDeviceVar1.operatingMode <= 0xFF))
    {
        DALI_setResponse(YES);
    }

    return;
}

void DALI_103_query_quiescentMode(void)
{
    if(gControlDeviceVar1.quiescentMode == ENABLED)
    {
        DALI_setResponse(YES);
    }

    return;
}

void DALI_103_query_deviceGroups_0_7(void)
{
    uint8_t respByte = gControlDeviceVar1.deviceGroups & 0xFF;
    DALI_setResponse(respByte);

    return;
}

void DALI_103_query_deviceGroups_8_15(void)
{
    uint8_t respByte = (gControlDeviceVar1.deviceGroups & 0xFF00) >> 8;
    DALI_setResponse(respByte);

    return;
}

void DALI_103_query_deviceGroups_16_23(void)
{
    uint8_t respByte = (gControlDeviceVar1.deviceGroups & 0xFF0000) >> 16;
    DALI_setResponse(respByte);

    return;
}

void DALI_103_query_deviceGroups_24_31(void)
{
    uint8_t respByte = (gControlDeviceVar1.deviceGroups & 0xFF000000) >> 24;
    DALI_setResponse(respByte);

    return;
}

void DALI_103_query_powerCycNotification(void)
{
    if(gControlDeviceVar1.powerCycleNotification == ENABLED)
    {
        DALI_setResponse(YES);
    }

    return;
}

void DALI_103_query_deviceCapabilites(void)
{
    uint8_t respByte = (gControlDeviceVar1.applicationControllerPresent ? 0x1 : 0) | (gControlDeviceVar1.numberOfInstances > 0 ? 0x2 : 0);

    DALI_setResponse(respByte);

    return;
}

void DALI_103_query_extendedVersionNumber(void)
{
    /* TEMPLATE for Query Extended Version Number */
}

void DALI_103_query_resetState(void)
{
    if(gControlDeviceVar1.resetState)
    {
        DALI_setResponse(YES);
    }

    return;
}


/* Instance Configuration Instructions */
void DALI_103_setEventPriority(uint32_t instMask)
{
    /* Check for Send Twice Commands */
    if(dali_gpio.Rx.isSendTwiceCmd == false)
    {
        return;
    }

    /* Check if DTR0 value is proper */
    if(gControlDeviceVar1.DTR0 < 2 || gControlDeviceVar1.DTR0 > 5)
    {
        return;
    }
    
    for(int i = 0; i < 32;i++)
    {
        if(instMask & (1 << i))
        {
            gControlDeviceVar1.instance[i].eventPriority = gControlDeviceVar1.DTR0;
        }
    }
}

void DALI_103_enableInstance(uint32_t instMask)
{
    /* Check for Send Twice Commands */
    if(dali_gpio.Rx.isSendTwiceCmd == false)
    {
        return;
    }

    for(int i = 0; i < 32;i++)
    {
        if(instMask & (1 << i))
        {
            gControlDeviceVar1.instance[i].instanceActive = true;
        }
    }
}

void DALI_103_disableInstance(uint32_t instMask)
{
    /* Check for Send Twice Commands */
    if(dali_gpio.Rx.isSendTwiceCmd == false)
    {
        return;
    }

    for(int i = 0; i < 32;i++)
    {
        if(instMask & (1 << i))
        {
            gControlDeviceVar1.instance[i].instanceActive = false;
        }
    }
}

void DALI_103_setPrimaryInstGroup(uint32_t instMask)
{
    /* Check for Send Twice Commands */
    if(dali_gpio.Rx.isSendTwiceCmd == false)
    {
        return;
    }

    if(gControlDeviceVar1.DTR0 > 31 && gControlDeviceVar1.DTR0 != MASK)
    {
        return;
    }

    for(int i = 0; i < 32;i++)
    {
        if(instMask & (1 << i))
        {
            gControlDeviceVar1.instance[i].instanceGroups0 = gControlDeviceVar1.DTR0;
        }
    }
}

void DALI_103_setInstGroup1(uint32_t instMask)
{
    /* Check for Send Twice Commands */
    if(dali_gpio.Rx.isSendTwiceCmd == false)
    {
        return;
    }

    if(gControlDeviceVar1.DTR0 > 31 && gControlDeviceVar1.DTR0 != MASK)
    {
        return;
    }

    for(int i = 0; i < 32;i++)
    {
        if(instMask & (1 << i))
        {
            gControlDeviceVar1.instance[i].instanceGroups1 = gControlDeviceVar1.DTR0;
        }
    }        
}

void DALI_103_setInstGroup2(uint32_t instMask)
{
    /* Check for Send Twice Commands */
    if(dali_gpio.Rx.isSendTwiceCmd == false)
    {
        return;
    }

    if(gControlDeviceVar1.DTR0 > 31 && gControlDeviceVar1.DTR0 != MASK)
    {
        return;
    }

    for(int i = 0; i < 32;i++)
    {
        if(instMask & (1 << i))
        {
            gControlDeviceVar1.instance[i].instanceGroups2 = gControlDeviceVar1.DTR0;
        }
    }
      
}

void DALI_103_setEventScheme(uint32_t instMask)
{
    /* Check for Send Twice Commands */
    if(dali_gpio.Rx.isSendTwiceCmd == false)
    {
        return;
    }

    if(gControlDeviceVar1.DTR0 > 4)
    {
        return;
    }

    for(int i = 0; i < 32;i++)
    {
        if(instMask & (1 << i))
        {
            gControlDeviceVar1.instance[i].eventScheme = gControlDeviceVar1.DTR0;
        }
    }    
}

void DALI_103_setEventFilter(uint32_t instMask)
{
    /* Check for Send Twice Commands */
    if(dali_gpio.Rx.isSendTwiceCmd == false)
    {
        return;
    }

    for(int i = 0; i < 32;i++)
    {
        if(instMask & (1 << i))
        {
            #ifdef DALI_303_SUPPORT
            if(i == PIR_INSTANCE_INDEX)
            {
                if(gControlDeviceVar1.DTR0 < 0x20)
                {
                    gControlDeviceVar1.instance[i].eventFilter = gControlDeviceVar1.DTR0;
                }

                continue;
            }
            #endif
            
            gControlDeviceVar1.instance[i].eventFilter = (gControlDeviceVar1.DTR2 << 16 | gControlDeviceVar1.DTR1 << 8 | gControlDeviceVar1.DTR0);
        }
    }

}
