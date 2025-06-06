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

#include "dali_cd_comm.h"

/* Depending on the number of Instances, Change EEPROM RECORD SIZE accordingly */
const uint32_t defaultInfo[EEPROM_EMULATION_DATA_SIZE / sizeof(uint32_t)] = {
    (uint32_t) SHORT_ADDRESS_DEFAULT, (uint32_t) DEVICE_GROUP_DEFAULT,
    (uint32_t) RANDOM_ADDRESS_DEFAULT, (uint32_t) OPERATING_MODE,
    (uint32_t) APP_CONTROLLER_PRESENT,
    (uint32_t) POWER_CYC_NOTIFICATION_DEFAULT, (uint32_t) INST_GROUP0_DEFAULT,
    (uint32_t) INST_GROUP1_DEFAULT, (uint32_t) INST_GROUP2_DEFAULT,
    (uint32_t) INST_ACTIVE_DEFAULT, (uint32_t) EVENT_FILTER_DEFAULT,
    (uint32_t) EVENT_SCHEME_DEFAULT, (uint32_t) EVENT_PRIORITY_DEFAULT, 0};

uint32_t flash_data[EEPROM_EMULATION_DATA_SIZE / sizeof(uint32_t)];

/**
 * @brief Sets the event address byte based on the instance index and event scheme.
 *
 * @param[in] instanceIndex The index of the instance for which to set the event address byte.
 */
static void DALI_ControlDevice_setEventAddressByte(uint8_t instanceIndex)
{
    uint8_t eventScheme =
        gControlDeviceVar1.instance[instanceIndex].eventScheme;

    /* Instance Addressing */
    if (eventScheme == 0) {
        dali_gpio.Tx.dataArr[0] =
            0x80 |
            (gControlDeviceVar1.instance[instanceIndex].instanceType << 1);
        dali_gpio.Tx.dataArr[1] =
            0x80 |
            (gControlDeviceVar1.instance[instanceIndex].instanceNumber << 2);
    }

    /* Device Addressing */
    else if (eventScheme == 1) {
        dali_gpio.Tx.dataArr[0] = (gControlDeviceVar1.shortAddress << 1);
        dali_gpio.Tx.dataArr[1] =
            (gControlDeviceVar1.instance[instanceIndex].instanceType << 2);
    }

    /* Device/Instance Addressing */
    else if (eventScheme == 2) {
        dali_gpio.Tx.dataArr[0] = (gControlDeviceVar1.shortAddress << 1);
        dali_gpio.Tx.dataArr[1] =
            0x80 |
            (gControlDeviceVar1.instance[instanceIndex].instanceNumber << 2);
    }

    /* Device Group Addressing */
    else if (eventScheme == 3) {
        dali_gpio.Tx.dataArr[0] =
            0x80 | (gControlDeviceVar1.deviceGroups << 1);
        dali_gpio.Tx.dataArr[1] =
            (gControlDeviceVar1.instance[instanceIndex].instanceType << 2);
    }

    /* Instance Group Addressing */
    else if (eventScheme == 4) {
        dali_gpio.Tx.dataArr[0] =
            0xC0 |
            (gControlDeviceVar1.instance[instanceIndex].instanceGroups0 << 1);
        dali_gpio.Tx.dataArr[1] =
            (gControlDeviceVar1.instance[instanceIndex].instanceType << 2);
    }
}

void DALI_ControlDevice_init()
{
    /* Init DALI Tx */
    DALI_TX_enableTimer_Output();
    /* Code for initialising the Default values or from Flash */
    DALI_ControlDevice_loadDefaults();
}

void DALI_ControlDevice_receive(void)
{
    while (gControlDeviceVar1.resetState) {
        /* Do not execute commands during reset */
    }

    if (!gReceive) {
        return;
    }

    gReceive = false;

    if (dali_gpio.Rx.dataSize == 3) {
        /* Command Frame */
        if (dali_gpio.Rx.dataArr[0] & 0x1) {
            if (DALI_ControlDevice_AddrCheck(dali_gpio.Rx.dataArr[0])) {
                /* Device Commands */
                if (dali_gpio.Rx.dataArr[1] == 0xFE) {
                    DALI_ControlDevice_execDeviceCommand(
                        dali_gpio.Rx.dataArr[2]);
                }

                /* Instance Commands */
                else {
                    uint32_t instanceMask =
                        DALI_ControlDevice_InstCheck(dali_gpio.Rx.dataArr[1]);

                    if (instanceMask != 0) {
                        DALI_ControlDevice_execInstanceCommand(
                            dali_gpio.Rx.dataArr[2], instanceMask);
                    }
                }

            }

            else {
                DALI_ControlDevice_execSpecialCommand();
            }
        }

        /* Event Frame */
        else {
            /* User can implement custom handling for received event messages */
        }
    }
}

void DALI_ControlDevice_execDeviceCommand(uint8_t opCode)
{
    switch (opCode) {
        case 0x00:
            DALI_103_identifyDevice();
            break;

        case 0x01:
            DALI_103_resetPowerCycleSeen();
            break;

        case 0x10:
            DALI_103_reset();
            break;

        case 0x11:
            DALI_103_resetMemBank();
            break;

        case 0x14:
            DALI_103_setShortAddr();
            break;

        case 0x15:
            DALI_103_enableWriteMemory();
            break;

        case 0x16:
            DALI_103_enableAppController();
            break;

        case 0x17:
            DALI_103_disableAppController();
            break;

        case 0x18:
            DALI_103_setOperatingMode();
            break;

        case 0x19:
            DALI_103_addDeviceGroups_0_15();
            break;

        case 0x1A:
            DALI_103_addDeviceGroups_16_31();
            break;

        case 0x1B:
            DALI_103_removeDeviceGroups_0_15();
            break;

        case 0x1C:
            DALI_103_removeDeviceGroups_16_31();
            break;

        case 0x1D:
            DALI_103_startQuiescentMode();
            break;

        case 0x1E:
            DALI_103_stopQuiescentMode();
            break;

        case 0x1F:
            DALI_103_enablePowerCycNotification();
            break;

        case 0x20:
            DALI_103_disablePowerCycNotification();
            break;

        case 0x21:
            DALI_103_savePersistentVariables();
            break;

        case 0x30:
            DALI_103_query_deviceStatus();
            break;

        case 0x31:
            DALI_103_query_appControllerError();
            break;

        case 0x32:
            DALI_103_query_inputDeviceError();
            break;

        case 0x33:
            DALI_103_query_missingShortAddr();
            break;

        case 0x34:
            DALI_103_query_versionNumber();
            break;

        case 0x35:
            DALI_103_query_numberOfInstances();
            break;

        case 0x36:
            DALI_103_query_DTR0();
            break;

        case 0x37:
            DALI_103_query_DTR1();
            break;

        case 0x38:
            DALI_103_query_DTR2();
            break;

        case 0x39:
            DALI_103_query_randomAddrH();
            break;

        case 0x3A:
            DALI_103_query_randomAddrM();
            break;

        case 0x3B:
            DALI_103_query_randomAddrL();
            break;

        case 0x3C:
            DALI_103_query_readMemLocation();
            break;

        case 0x3D:
            DALI_103_query_appControlEnabled();
            break;

        case 0x3E:
            DALI_103_query_operatingMode();
            break;

        case 0x3F:
            DALI_103_query_manufacturerSpecificMode();
            break;

        case 0x40:
            DALI_103_query_quiescentMode();
            break;

        case 0x41:
            DALI_103_query_deviceGroups_0_7();
            break;

        case 0x42:
            DALI_103_query_deviceGroups_8_15();
            break;

        case 0x43:
            DALI_103_query_deviceGroups_16_23();
            break;

        case 0x44:
            DALI_103_query_deviceGroups_24_31();
            break;

        case 0x45:
            DALI_103_query_powerCycNotification();
            break;

        case 0x46:
            DALI_103_query_deviceCapabilites();
            break;

        case 0x47:
            DALI_103_query_extendedVersionNumber();
            break;

        case 0x48:
            DALI_103_query_resetState();
            break;
    }
}

void DALI_ControlDevice_execInstanceCommand(uint8_t opCode, uint32_t instMask)
{
    switch (opCode) {
#ifdef DALI_303_SUPPORT
        /* DALI 303 Commands */
        case 0x20:
            DALI_303_catchMovement();
            break;

        case 0x21:
            DALI_303_setHoldTimer();
            break;

        case 0x22:
            DALI_303_setReportTimer();
            break;

        case 0x23:
            DALI_303_setDeadtimeTimer();
            break;

        case 0x24:
            DALI_303_cancelHoldTimer();
            break;

        case 0x2C:
            DALI_303_queryDeadtimeTimer();
            break;

        case 0x2D:
            DALI_303_queryHoldTimer();
            break;

        case 0x2E:
            DALI_303_queryReportTimer();
            break;

        case 0x2F:
            DALI_303_queryCatching();
            break;
#endif

        /* DALI 103 Instance Commands */
        case 0x61:
            DALI_103_setEventPriority(instMask);
            break;

        case 0x62:
            DALI_103_enableInstance(instMask);
            break;

        case 0x63:
            DALI_103_disableInstance(instMask);
            break;

        case 0x64:
            DALI_103_setPrimaryInstGroup(instMask);
            break;

        case 0x65:
            DALI_103_setInstGroup1(instMask);
            break;

        case 0x66:
            DALI_103_setInstGroup2(instMask);
            break;

        case 0x67:
            DALI_103_setEventScheme(instMask);
            break;

        case 0x68:
            DALI_103_setEventFilter(instMask);
            break;
    }
}

void DALI_ControlDevice_execSpecialCommand(void)
{
    if (dali_gpio.Rx.dataArr[0] == 0xC1) {
        if ((dali_gpio.Rx.dataArr[1] == 0x00) &&
            (dali_gpio.Rx.dataArr[2] == 0x00)) {
            DALI_103_terminate();
        }

        else if (dali_gpio.Rx.dataArr[1] == 0x01) {
            DALI_103_initialise();
        }

        else if ((dali_gpio.Rx.dataArr[1] == 0x02) &&
                 (dali_gpio.Rx.dataArr[2] == 0x00)) {
            DALI_103_randomise();
        }

        else if ((dali_gpio.Rx.dataArr[1] == 0x03) &&
                 (dali_gpio.Rx.dataArr[2] == 0x00)) {
            DALI_103_compare();
        }

        else if ((dali_gpio.Rx.dataArr[1] == 0x04) &&
                 (dali_gpio.Rx.dataArr[2] == 0x00)) {
            DALI_103_withdraw();
        }

        else if (dali_gpio.Rx.dataArr[1] == 0x05) {
            DALI_103_searchAddrH();
        }

        else if (dali_gpio.Rx.dataArr[1] == 0x06) {
            DALI_103_searchAddrM();
        }

        else if (dali_gpio.Rx.dataArr[1] == 0x07) {
            DALI_103_searchAddrL();
        }

        else if (dali_gpio.Rx.dataArr[1] == 0x08) {
            DALI_103_programShortAddr();
        }

        else if (dali_gpio.Rx.dataArr[1] == 0x09) {
            DALI_103_verifyShortAddr();
        }

        else if ((dali_gpio.Rx.dataArr[1] == 0x0A) &&
                 (dali_gpio.Rx.dataArr[2] == 0x00)) {
            DALI_103_queryShortAddr();
        }

        else if (dali_gpio.Rx.dataArr[1] == 0x20) {
            DALI_103_writeMemLocation();
        }

        else if (dali_gpio.Rx.dataArr[1] == 0x21) {
            DALI_103_writeMemLocation_NoReply();
        }

        else if (dali_gpio.Rx.dataArr[1] == 0x30) {
            DALI_103_DTR0();
        }

        else if (dali_gpio.Rx.dataArr[1] == 0x31) {
            DALI_103_DTR1();
        }

        else if (dali_gpio.Rx.dataArr[1] == 0x32) {
            DALI_103_DTR2();
        }

        else if (dali_gpio.Rx.dataArr[1] == 0x33) {
            DALI_103_sendTestFrame();
        }
    }

    else if (dali_gpio.Rx.dataArr[0] == 0xC5) {
        DALI_103_directWriteMem();
    }

    else if (dali_gpio.Rx.dataArr[0] == 0xC7) {
        DALI_103_DTR1_DTR0();
    }

    else if (dali_gpio.Rx.dataArr[0] == 0xC9) {
        DALI_103_DTR2_DTR1();
    }
}

/* For testing purpose */
void DALI_ControlDevice_testTransmit(void)
{
    gReceive            = false;
    dali_gpio.Tx.toSend = true;

    dali_gpio.Tx.dataArr[0] = gUserVar.addrByte;
    dali_gpio.Tx.dataArr[1] = gUserVar.instanceByte;
    dali_gpio.Tx.dataArr[2] = gUserVar.opcodeByte;

    dali_gpio.Tx.frame = FORWARD_FRAME_24;

    dali_gpio.Tx.settlingTime = FORWARD_FRAME_P1_SETTLING_MIN;

    DALI_ControlDevice_transmit();

    if (gUserVar.isSendTwiceCmd) {
        dali_gpio.Tx.toSend = true;
        DALI_ControlDevice_transmit();
    }
}

void DALI_ControlDevice_transmit(void)
{
    /* Check if any data needs to be send */
    if (dali_gpio.Tx.toSend == false) {
        return;
    }

    dali_gpio.Tx.toSend = false;

    if (dali_gpio.Tx.frame == BACKWARD_FRAME) {
        dali_gpio.Tx.dataSize = 1;
    }

    else if (dali_gpio.Tx.frame == FORWARD_FRAME_16) {
        dali_gpio.Tx.dataSize = 2;
    }

    else if (dali_gpio.Tx.frame == FORWARD_FRAME_24) {
        dali_gpio.Tx.dataSize = 3;
    }

    while (dali_gpio.Tx.status != TX_TRANSMIT_SUCCESS) {
        /* Wait until bus is idle */
        while (!((dali_gpio.Tx.status == TX_IDLE) &&
                 (dali_gpio.Rx.isActive == false))) {
        };

        /* Check for Settling Time */
        while ((dali_gpio.Rx.isActive == true) ||
               (DL_TimerG_getTimerCount(SETTLING_TIMER_INST) *
                       SETTLING_TIMER_TICK <
                   dali_gpio.Tx.settlingTime)) {
        };

        DALI_TX_transmitFrame();
    }

    while (dali_gpio.Tx.status != TX_IDLE) {
    };
}

bool DALI_ControlDevice_AddrCheck(uint8_t AddrByte)
{
    /* Short Addressing */
    if ((AddrByte & 0x80) == 0) {
        uint8_t shortAddr = AddrByte >> 1;
        if (gControlDeviceVar1.shortAddress == shortAddr) {
            return true;
        }
    }

    /* Device Group Addressing */
    else if ((AddrByte & 0xC0) == 0x80) {
        uint8_t groupAddr = (AddrByte & 0x3E) >> 1;
        if (gControlDeviceVar1.deviceGroups & groupAddr) {
            return true;
        }
    }

    /* Broadcast Message */
    else if (((AddrByte & 0xFE) == 0xFE) ||
             (((AddrByte & 0xFE) == 0xFC) &&
                 (gControlDeviceVar1.shortAddress == MASK))) {
        return true;
    }

    return false;
}

uint32_t DALI_ControlDevice_InstCheck(uint8_t InstByte)
{
    uint32_t instMask = 0;
    /* Instance Number Addressing */
    if ((InstByte & 0xE0) == 0) {
        if (InstByte < gControlDeviceVar1.numberOfInstances)
            return (instMask << (InstByte));
    }

    /* Instance Group Addressing */
    else if ((InstByte & 0xE0) == 0x80) {
        uint8_t InstGroup = InstByte & 0x1F;

        for (int i = 0; i < gControlDeviceVar1.numberOfInstances; i++) {
            if (gControlDeviceVar1.instance[i].instanceGroups0 == InstGroup ||
                gControlDeviceVar1.instance[i].instanceGroups1 == InstGroup ||
                gControlDeviceVar1.instance[i].instanceGroups2 == InstGroup) {
                instMask |= (1 << i);
            }
        }
    }

    /* Instance Type Addressing */
    else if ((InstByte & 0xE0) == 0xC0) {
        uint8_t InstType = InstByte & 0x1F;

        for (int i = 0; i < gControlDeviceVar1.numberOfInstances; i++) {
            if (gControlDeviceVar1.instance[i].instanceType == InstType) {
                instMask |= (1 << i);
            }
        }
    }

    /* Instance BroadCast */
    else if (InstByte == 0xFF) {
        return 0xFFFFFFFF;
    }

    return 0;
}

void DALI_ControlDevice_loadDefaults(void)
{
    uint32_t state = EEPROM_TypeA_init(&flash_data[0]);
    if (state == EEPROM_EMULATION_INIT_OK && gEEPROMTypeASearchFlag == false) {
        for (uint8_t i = 0; i < EEPROM_EMULATION_DATA_SIZE / sizeof(uint32_t);
             i++) {
            flash_data[i] = defaultInfo[i];
        }
        EEPROM_TypeA_writeData(&flash_data[0]);
    }

    /* Set Control Device Default Values */
    gControlDeviceVar1.shortAddress                 = flash_data[0];
    gControlDeviceVar1.deviceGroups                 = flash_data[1];
    gControlDeviceVar1.randomAddress                = flash_data[2];
    gControlDeviceVar1.numberOfInstances            = DEVICE_INSTANCE_COUNT;
    gControlDeviceVar1.operatingMode                = flash_data[3];
    gControlDeviceVar1.applicationActive            = flash_data[4];
    gControlDeviceVar1.applicationControllerPresent = APP_CONTROLLER_PRESENT;
    gControlDeviceVar1.powerCycleNotification       = flash_data[5];
    gControlDeviceVar1.resetState                   = RESET_STATE_DEFAULT;

    gControlDeviceVar1.powerCycleSeen = true;

    uint8_t flash_InstanceIndex = 6;
    for (int i = 0; i < DEVICE_INSTANCE_COUNT; i++) {
        gControlDeviceVar1.instance[i].instanceGroups0 =
            flash_data[flash_InstanceIndex++];
        gControlDeviceVar1.instance[i].instanceGroups1 =
            flash_data[flash_InstanceIndex++];
        gControlDeviceVar1.instance[i].instanceGroups2 =
            flash_data[flash_InstanceIndex++];
        gControlDeviceVar1.instance[i].instanceActive =
            flash_data[flash_InstanceIndex++];
        gControlDeviceVar1.instance[i].eventFilter =
            flash_data[flash_InstanceIndex++];
        gControlDeviceVar1.instance[i].eventScheme =
            flash_data[flash_InstanceIndex++];
        gControlDeviceVar1.instance[i].eventPriority =
            flash_data[flash_InstanceIndex++];
    }
}

void DALI_ControlDevice_EventMessages(uint8_t instanceIndex)
{
    uint8_t eventInfo = 0;

#ifdef DALI_303_SUPPORT
    if (gOccupancySensorVar1.isDeadtimeOn) {
        return;
    }

    if (instanceIndex == PIR_INSTANCE_INDEX) {
        uint8_t inputVal =
            gControlDeviceVar1.instance[instanceIndex].inputValue;

        eventInfo |= (inputVal & VACANT_WITH_MOVEMENT ? 0x1 : 0);
        eventInfo |= (inputVal & OCCCUPIED_NO_MOVEMENT ? 0x2 : 0);

        /* Movement Sensor */
        eventInfo |= 0x8;
    }
#endif

    DALI_ControlDevice_setEventAddressByte(instanceIndex);

    dali_gpio.Tx.dataArr[2] = eventInfo;

    dali_gpio.Tx.frame    = FORWARD_FRAME_24;
    dali_gpio.Tx.dataSize = 3;

    dali_gpio.Tx.toSend = true;
}

#ifdef DALI_303_SUPPORT
void DALI_303_init(PIR_configRegister *configRegister)
{
    /* Init PIR Sensor */
    PIR_init();

    /* Configure PIR Sensor */
    PIR_sendConfig(configRegister);
}

void DALI_303_PIR_readInput(void)
{
    /* User can use this if need more data from the PIR to decide Occupancy */
    // PIR_readDirectLink();

    /* To Clear the Interrupt from PIR */
    DL_GPIO_enableOutput(GPIO_PIR_DIRECT_LINK_PORT, GPIO_PIR_DIRECT_LINK_PIN);

    DL_GPIO_clearPins(GPIO_PIR_DIRECT_LINK_PORT, GPIO_PIR_DIRECT_LINK_PIN);

    DL_GPIO_disableOutput(GPIO_PIR_DIRECT_LINK_PORT, GPIO_PIR_DIRECT_LINK_PIN);

    gControlDeviceVar1.instance[PIR_INSTANCE_INDEX].inputValue =
        OCCUPIED_WITH_MOVEMENT;

    g303_TimerCounter.blindTimeCounter = DALI_303_getCounterValue(
        (uint32_t) DALI_303_BLINDTIME_INCREMENT *
        ((uint32_t) gPIR_configRegister.blindTime + 1));

    DL_Timer_startCounter(TIMER_303_INST);

    /* Check if event filter or catching is enabled, for sending the event message */
    if (gOccupancySensorVar1.catching ||
        (gControlDeviceVar1.instance[PIR_INSTANCE_INDEX].eventFilter & 0x80)) {
        DALI_ControlDevice_EventMessages(PIR_INSTANCE_INDEX);
    }
}

#endif /* DALI_303_SUPPORT */
