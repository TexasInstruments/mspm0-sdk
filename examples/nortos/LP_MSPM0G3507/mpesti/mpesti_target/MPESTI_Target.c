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

//
// Include files
//
#include "MPESTI_Target.h"
#include <stdint.h>
#include "MPESTI_Target_Config.h"

volatile MPESTI_Target target;
volatile MPESTI_Target *MPESTITarget;
volatile MPESTI_T_FlagBits MPESTI_flags;

volatile uint8_t broadcastCmdValue = 0;
static uint8_t crc8(
    uint8_t *data, size_t length, uint8_t polynomial, uint8_t seed);
__STATIC_INLINE void MPESTI_T_readFIFODiscard(MPESTI_Target *target);

/*
 *  ======== MPESTI_T_init ========
 */
uint8_t MPESTI_T_init(MPESTI_T_Params *params)
{
    /* Initialize the MPESTI target instance */
    MPESTITarget                              = &target;
    target.nwk.currentCmd                     = UNDEFINED_CMD;
    target.nwk.rxBuffPtr                      = params->rxBuffPtr;
    target.nwk.txBuffPtr                      = NULL;
    target.nwk.txSize                         = 0;
    target.nwk.txIndex                        = 0;
    target.nwk.txLen                          = 0;
    target.nwk.rxLen                          = 0;
    target.state                              = MPESTI_T_Simple_Presence;
    target.phy.MPESTI_T_Phy_uartBase          = params->targetUartBase;
    target.phy.MPESTI_T_gpioReadyToReceive    = params->readyToReceiveGpio;
    target.phy.MPESTI_T_readyToReceivePinMask = params->readyToReceivePinMask;
    target.targetDPR                          = params->targetDPR;
    target.Broadcast_Callback                 = params->Broadcast_Callback;
    target.VWE_Callback                       = params->VWE_Callback;
    MPESTI_flags.isDPRRequestReceived         = false;
    target.MPESTI_T_Error                     = MPESTI_T_ErrorCode_NoError;
    MPESTI_flags.isBreakSent                  = false;
    return MPESTI_T_RET_OK;
}

/*
 *  ======== MPESTI_T_setReadyToReceive ========
 */
/* Requirement: Jira req: MPESTI-2, MPESTI-4, MPESTI-5 */
void MPESTI_T_setReadyToReceive(MPESTI_Target *target)
{
    /* Turn on the FET so the PESTI line goes high. The initiator will detect a break release */
    DL_GPIO_writePinsVal(target->phy.MPESTI_T_gpioReadyToReceive,
        target->phy.MPESTI_T_readyToReceivePinMask,
        (~target->phy.MPESTI_T_readyToReceivePinMask));
    MPESTI_T_clearUARTInts(target);
    MPESTI_flags.isBreakSent = true;
}

/*
 *  ======== MPESTI_T_clearUARTInts ========
 */
void MPESTI_T_clearUARTInts(MPESTI_Target *target)
{
    /* Clear previous interrupts */
    DL_UART_clearInterruptStatus(target->phy.MPESTI_T_Phy_uartBase, 0x1FFFF);
}

/*
 *  ======== MPESTI_T_prepareDPRResponse ========
 */
/* This function will return the size of the data */
/* Requirement: Jira req: MPESTI-7, MPESTI-15, MPESTI-16,  */
uint8_t MPESTI_T_prepareDPRResponse(MPESTI_T_DPR_PKT *packet)
{
    /* The Payload Format Version must be 02h to differentiate it
     * from the previous 01h definition and match
     * the PCI-SIG format version */
    uint8_t payloadSize = 0;
    if (!MPESTI_flags.isDPRDataLoaded) {
        uint16_t deviceID = DL_FactoryRegion_getPartNumber();
        uint8_t *data;

        packet->DPR_payload[0] = PAYLOAD_FORMAT_VERSION;
        packet->DPR_payload[2] = ((uint16_t) VENDOR_ID) >> 8;
        packet->DPR_payload[3] = (uint8_t) VENDOR_ID;
        packet->DPR_payload[4] = DEVICE_CLASS;
        packet->DPR_payload[5] = ((uint16_t) deviceID) >> 8;
        packet->DPR_payload[6] = (uint8_t) deviceID;
        packet->DPR_payload[7] = (AP_PEC_SUPPORT | VENDOR_ID_FORMAT);
        /* Padding of N bytes with a value of 00h is required to make the payload size an integer
        multiple of eight. If the vendor specific region is not used, the number of padding bytes
        = 00h in the payload is seven.*/
        payloadSize = DPR_PAYLOAD_SIZE_WITHOUT_VENDOR_PAYLOAD +
                      packet->vendorDataSize + packet->noOfPaddingBytes;
        packet->DPR_payload[1] = payloadSize / 8;
        /* Copy the address of the data section in the payload */
        data = &packet->DPR_payload[8];
        for (uint8_t i = 0; i < packet->vendorDataSize; i++) {
            *data++ = *packet->vendorData++;
        }
        *data = crc8(
            packet->DPR_payload, payloadSize - 1, CRC_POLYNOMIAL, CRC_SEED);
        MPESTI_flags.isDPRDataLoaded = true;
    }
    return payloadSize;
}

/*
 *  ======== crc8 ========
 */
static uint8_t crc8(
    uint8_t *data, size_t length, uint8_t polynomial, uint8_t seed)
{
    uint8_t bit = 0;
    uint8_t crc = seed;
    for (size_t i = 0; i < length; i++) {
        crc ^= data[i]; /* XOR the current byte to crc */

        /* Process each bit */
        for (bit = 0; bit < 8; bit++) {
            if (crc & 0x80) /* Check if MSB is set */
            {
                crc = (crc << 1) ^
                      polynomial; /* Shift and XOR with Polynomial */
            } else {
                crc <<= 1; /* Just shift the CRC */
            }
        }
    }
    return crc; /* Return the final CRC */
}

/*
 *  ======== MPESTI_T_uartInterruptHandler ========
 */
void MPESTI_T_uartInterruptHandler(void)
{
    MPESTI_Target *target = (MPESTI_Target *) MPESTITarget;
    MPESTI_T_Phy *phy     = &target->phy;

    /* Get all enabled interrupt status */
    uint32_t status =
        DL_UART_getEnabledInterruptStatus(phy->MPESTI_T_Phy_uartBase, 0x1FFFF);
    DL_UART_clearInterruptStatus(phy->MPESTI_T_Phy_uartBase, status);

    if (status & UART_RX_ERRS) {
        if (status & DL_UART_INTERRUPT_OVERRUN_ERROR) {
            /* Throw away the rest in order to clear the overrun */
            while (!DL_UART_isRXFIFOEmpty(phy->MPESTI_T_Phy_uartBase)) {
                volatile uint8_t data =
                    DL_UART_receiveData(phy->MPESTI_T_Phy_uartBase);
                /* Discard the data */
                (void) data;
            }
            target->state       = MPESTI_T_State_Error;
            MPESTI_flags.OVRErr = true;
        }
        /* Requirement: Jira req: MPESTI-11, MPESTI-12 */
        if (status & DL_UART_INTERRUPT_BREAK_ERROR) {
            if (MPESTITarget->state == MPESTI_T_VWE) {
                target->state = MPESTI_T_State_Error;
            } else {
                /* Detect break signal from the Initiator */
                target->state = MPESTI_T_Simple_Presence;
            }
        }
        if (status & DL_UART_INTERRUPT_RX_TIMEOUT_ERROR) {
            if (MPESTITarget->state == MPESTI_T_VWE) {
                target->state          = MPESTI_T_State_Error;
                target->MPESTI_T_Error = MPESTI_T_Active_Phase_Error;
            }
        }
        /* Discard the data if anything has been received */
        MPESTI_T_readFIFODiscard(target);
    }

    if (status & DL_UART_INTERRUPT_RX) {
        volatile uint8_t rxData = RESPONSE_NTR;
#if HW_UART_SINGLE_WIRE == 0
        if (((target->state == MPESTI_T_DPR) ||
                (target->state == MPESTI_T_VWE)) &&
            ((target->nwk.txLen != target->nwk.txIndex) ||
                (target->nwk.rxLen != 0))) {
            /* We receive the same data back due to UART 1 wire */
            MPESTI_T_readFIFODiscard(target);
        }
#endif
        /* receive characters as long as we can */
        while (DL_UART_isRXFIFOEmpty(phy->MPESTI_T_Phy_uartBase) == false) {
            /* data in the RX FIFO, pull it out of the hardware */
            rxData = DL_UART_receiveData(phy->MPESTI_T_Phy_uartBase);

            /* In the M-PESTI, the expected data would be a 1 byte command */
            /* Requirement: Jira req: MPESTI-13 */
            if ((rxData == BROADCAST_CMD) ||
                (target->nwk.currentCmd == BROADCAST_CMD)) {
                if (MPESTI_T_getCurrentState() != MPESTI_T_Simple_Presence) {
                    if (rxData == BROADCAST_CMD) {
                        target->nwk.currentCmd = rxData;
                    } else {
                        broadcastCmdValue = rxData;
                        target->Broadcast_Callback(broadcastCmdValue);
                        target->nwk.currentCmd = UNDEFINED_CMD;
                    }
                }
            }
            /* Requirement: Jira req: MPESTI-9, MPESTI-14,  */
            else if (rxData == DPR_CMD) {
                target->state                     = MPESTI_T_DPR;
                MPESTI_flags.isDPRRequestReceived = true;
            }
            /* Requirement: Jira req: MPESTI-9 */
            else if (rxData == VWE_CMD) {
                target->state = MPESTI_T_VWE;
                /* Requirement: Jira req: MPESTI-27, MPESTI-28 */
                if (MPESTI_flags.isDPRSent == true) {
                    MPESTI_flags.isVWEDataSent   = false;
                    MPESTI_flags.isVWEDataLoaded = false;
#if VWE_SUPPORT == 1
                    MPESTI_T_receiveData(target);
#endif
                }
            } else {
                target->MPESTI_T_Error = MPESTI_T_ErrorCode_Cmd;
            }
        }
    }
    if (status & DL_UART_INTERRUPT_TX) {
        if (MPESTI_T_sendData((MPESTI_Target *) target)) {
            DL_UART_enableInterrupt(
                phy->MPESTI_T_Phy_uartBase, DL_UART_INTERRUPT_EOT_DONE);
        }
    }
    if (status & DL_UART_INTERRUPT_EOT_DONE) {
        DL_UART_disableInterrupt(
            phy->MPESTI_T_Phy_uartBase, DL_UART_INTERRUPT_EOT_DONE);
        target->state = MPESTI_T_Simple_Presence;
    }
}

/*
 *  ======== MPESTI_T_receiveData ========
 */
uint8_t MPESTI_T_receiveData(MPESTI_Target *target)
{
    uint8_t i;
    MPESTI_T_Phy *phy = &target->phy;

    /* Read the data from RX FIFO for 8-bit transfer */
    for (i = 0U; i < VWE_PAYLOAD_SIZE; i++) {
        if (!DL_UART_isRXFIFOEmpty(phy->MPESTI_T_Phy_uartBase)) {
            target->nwk.rxBuffPtr[i] =
                DL_UART_receiveData(phy->MPESTI_T_Phy_uartBase);
        } else {
            break;
        }
    }
    return i;
}

/*
 *  ======== MPESTI_T_sendData ========
 */
bool MPESTI_T_sendData(MPESTI_Target *target)
{
    uint8_t i;

    if (target->nwk.txLen != target->nwk.txIndex) {
        /* Write the data in TX FIFO for 8-bit transfer */
        for (i = target->nwk.txIndex; i < target->nwk.txLen; i++) {
            if (!DL_UART_isTXFIFOFull(target->phy.MPESTI_T_Phy_uartBase)) {
                DL_UART_transmitData(target->phy.MPESTI_T_Phy_uartBase,
                    target->nwk.txBuffPtr[i]);
                target->nwk.txIndex++;
            } else {
                break;
            }
        }
        /* When all the data is transferred, discard the last received data*/
        if (target->nwk.txLen == target->nwk.txIndex) {
            MPESTI_T_readFIFODiscard(target);

            if (target->state == MPESTI_T_DPR) {
                if (MPESTI_flags.isDPRSent == false) {
                    MPESTI_flags.isDPRSent       = true;
                    MPESTI_flags.isVWEDataSent   = false;
                    MPESTI_flags.isVWEDataLoaded = false;
                }
            } else if (target->state == MPESTI_T_VWE) {
                if (MPESTI_flags.isVWEDataSent == false) {
                    MPESTI_flags.isVWEDataSent = true;
                }
            }
            return true; /* All bytes transmitted */
        } else {
            return false; /* All bytes are not transmitted */
        }
    }
    return false; /* All bytes are transmitted already */
}

/*
 *  ======== MPESTI_T_readFIFODiscard ========
 */
/* Check this function during testing */
static inline void MPESTI_T_readFIFODiscard(MPESTI_Target *target)
{
    uint8_t i;
    volatile uint8_t rxData;

    /* Read the data from RX FIFO and discard it */
    for (i = 0; i < target->nwk.rxLen; i++) {
        if (!DL_UART_isRXFIFOEmpty(target->phy.MPESTI_T_Phy_uartBase)) {
            rxData = DL_UART_receiveData(target->phy.MPESTI_T_Phy_uartBase);
            target->nwk.rxLen--;
        } else {
            break;
        }
        (void) rxData;
    }
}

/*
 *  ======== MPESTI_T_getCurrentState ========
 */
MPESTI_T_State MPESTI_T_getCurrentState(void)
{
    return MPESTITarget->state;
}

/*
 *  ======== MPESTI_T_getErrorCode ========
 */
MPESTI_T_ErrorCode MPESTI_T_getErrorCode(void)
{
    return MPESTITarget->MPESTI_T_Error;
}

/*
 *  ======== MPESTI_T_stateMachine ========
 */
/* state machine */
void MPESTI_T_stateMachine(void)
{
    static uint8_t payloadSize = 0;

    switch (MPESTITarget->state) {
        case MPESTI_T_Simple_Presence:
            /* Requirement: Jira req: MPESTI-29 */
            if (MPESTI_flags.isBreakSent == false) {
                /* Satisfy the requirements: 3.1, 3.2 */
                MPESTI_T_setReadyToReceive((MPESTI_Target *) MPESTITarget);
                MPESTI_flags.isDPRSent = false;
                /* If there is a need to send DPR packet again then reset the needed params */
                MPESTI_flags.isDPRDataLoaded = false;
                MPESTITarget->nwk.txLen      = 0;
            }
            break;

        case MPESTI_T_DPR:
            /* Requirement: Jira req: MPESTI-18 */
            if (MPESTI_flags.isDPRRequestReceived == true) {
                if (MPESTI_flags.isDPRSent == false) {
                    if (!MPESTI_flags.isDPRDataLoaded) {
                        payloadSize = MPESTI_T_prepareDPRResponse(
                            MPESTITarget->targetDPR);
                    } else if (MPESTITarget->nwk.txLen == 0) {
                        MPESTITarget->nwk.txIndex = 0;
                        MPESTITarget->nwk.txLen   = payloadSize;
#if HW_UART_SINGLE_WIRE == 0
                        MPESTITarget->nwk.rxLen = MPESTITarget->nwk.txLen;
#endif
                        MPESTITarget->nwk.txBuffPtr =
                            MPESTITarget->targetDPR->DPR_payload;
                        MPESTI_T_sendData((MPESTI_Target *) MPESTITarget);
                    } else if (MPESTITarget->nwk.txIndex !=
                               MPESTITarget->nwk.txLen) {
                        /* Send data until the length becomes 0 */
                        MPESTI_T_sendData((MPESTI_Target *) MPESTITarget);
                    }
                }
            }
            break;

        case MPESTI_T_VWE:
            if (MPESTI_flags.isVWEDataSent == false) {
                if (MPESTI_flags.isVWEDataLoaded == false) {
#if VWE_SUPPORT == 1
                    /* Requirement: Jira req: MPESTI-24 */
                    MPESTITarget->VWE_Callback((MPESTI_Target *) MPESTITarget);
#else
                    /* Requirement: Jira req: MPESTI-8, MPESTI-21, */
                    uint8_t vwe_data            = RESPONSE_NTR;
                    MPESTITarget->nwk.txIndex   = 0;
                    MPESTITarget->nwk.txLen     = 1;
                    MPESTITarget->nwk.rxLen     = MPESTITarget->nwk.txLen;
                    MPESTITarget->nwk.txBuffPtr = &vwe_data;
#endif
                    /* Requirement: Jira req: MPESTI-17, MPESTI-23 */
                    MPESTI_T_sendData((MPESTI_Target *) MPESTITarget);
                    MPESTI_flags.isVWEDataLoaded = true;
                } else if (MPESTITarget->nwk.txIndex !=
                           MPESTITarget->nwk.txLen) {
                    /* Send data until the length becomes 0 */
                    MPESTI_T_sendData((MPESTI_Target *) MPESTITarget);
                }
            }
            break;

        case MPESTI_T_State_Error:
            /* Requirement: 3.8, restart the sequence */
            if (MPESTITarget->state != MPESTI_T_VWE) {
                MPESTITarget->state = MPESTI_T_Simple_Presence;
            }
            break;
        default:
            break;
    };
}
