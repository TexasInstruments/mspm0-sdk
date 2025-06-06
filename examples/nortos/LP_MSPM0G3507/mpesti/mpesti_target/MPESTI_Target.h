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
 *  @file       MPESTI_Target.h
 *  @brief      MPESTI Target header file
 *  @defgroup   MPESTI MPESTI_Target
 *
 *  @anchor MPESTI_Target_H__Overview
 *  # Overview
 *
 *  PESTI communication is based on a command and response method
 *  where the peripheral/target is permitted to only transmit data upon
 *  request by the initiator. This header file contains all the defines and
 *  configuration as well as list of APIs.
 *
 *  <hr>
 ******************************************************************************/
/** @addtogroup MPESTI_Target
 * @{
 */

#ifndef __MPESTI_TARGET_H__
#define __MPESTI_TARGET_H__

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

#include <ti/devices/msp/msp.h>
#include <ti/driverlib/driverlib.h>
#include <ti/driverlib/m0p/dl_core.h>
//*****************************************************************************
//
//! \addtogroup MPESTI_Target_API Application API layer
//! @{
//
//*****************************************************************************

//*****************************************************************************
//
// If building with a C++ compiler, make all of the definitions in this header
// have a C binding.
//
//*****************************************************************************
#ifdef __cplusplus
extern "C" {
#endif

/*****************************************************************************
* defines
******************************************************************************
*/
/*!
 * @brief Between Target RX and Target TX of a response. MAX reduces time to sample
 * initiator BREAK/Abort signal just before START. Target MCU should not have
 * trouble meeting minimum time required to allow the initiator to prepare for
 * RX following TX.
 */
#define TIME_tAPTAR 100  //100ns
/*!
 * @brief Initiator Abort BREAK assertion time.
 */
#define TIME_tDBREAK 1599  //50us
/*!
 * @brief Time between initiator End of STOP to START. STOP is typically sampled at
 * the midpoint of the bit, so there is approximately 2 us of time to the next START.
 */
#define TIME_tTIDLE 500  //500us
/*!
 * @brief Broadcast command from initiator
 */
#define BROADCAST_CMD 0xFF
/*!
 * @brief Discovery Pay load Request Command from initiator
 */
#define DPR_CMD 0x00
/*!
 * @brief Active Phase command (Virtual Wire Exchange) from initiator
 */
#define VWE_CMD 0x01
/*!
 * @brief CRC polynomial being used to calculate CRC8
 */
#define CRC_POLYNOMIAL 0x07
/*!
 * @brief CRC seed being used to calculate the CRC8
 */
#define CRC_SEED 0x00
/*!
 * @brief This is a provision that if we use the HW controlled single wire UART then there
 * is not need to discard the Rx data when Device executes data for Tx
 */
#define HW_UART_SINGLE_WIRE 0

/*!
 * @brief Undefined Command Value
 */
#define UNDEFINED_CMD 0xFE
/*!
 * @brief Fixed payload size without vendor payload
 */
#define DPR_PAYLOAD_SIZE_WITHOUT_VENDOR_PAYLOAD 9

/*!
 * @brief The UART interrupt supported
 */
#define UART_RX_ERRS                                                   \
    (DL_UART_INTERRUPT_OVERRUN_ERROR | DL_UART_INTERRUPT_BREAK_ERROR | \
        DL_UART_MAIN_INTERRUPT_RX_TIMEOUT_ERROR)

//*****************************************************************************
// Constant definitions
//*****************************************************************************
/*!
 * @brief Default response when there's nothing to send
 */
#define RESPONSE_NTR 0x00

/*!
 * @brief Return value when successful
 */
#define MPESTI_T_RET_OK (0)
/*!
 * @brief Return value when not successful
 */
#define MPESTI_T_RET_NOT_OK (1)

//*****************************************************************************
// typedefs
//*****************************************************************************

/*!
 * @brief Physical and Data Link Layer object
 *
 * This structure contains the base address pointers to the peripherals
 * which are being used
 */
typedef struct {
    /*! UART register map struct */
    UART_Regs *MPESTI_T_Phy_uartBase;
    /*! Ready to receive gpio */
    GPIO_Regs *MPESTI_T_gpioReadyToReceive;
    /*! Ready to receive mask */
    uint32_t MPESTI_T_readyToReceivePinMask;
    /*! Timer for the MPESTI (This is a place holder) */
    GPTIMER_Regs *MPESTI_T_Phy_timerBase;
} MPESTI_T_Phy;

/*!
 * @brief Definition of MPESTI Target Network structure
 *
 * This structure contains the communication related parameters which
 * included the buffer size, pointer, index etc.
 */
typedef struct {
    /*! Current Command */
    uint8_t currentCmd;
    /*! Rx length */
    uint8_t rxLen;
    /*! Reception Buffer pointer */
    uint8_t *rxBuffPtr;
    /*! Byte counter */
    uint8_t txIndex;
    /*! Bytes to send */
    uint8_t txLen;
    /*! Transmission pointer */
    uint8_t *txBuffPtr;
    /*! Max size of buffer */
    uint8_t txSize;
} MPESTI_T_Nwk;

/*!
 * @brief List of error codes used by the application to indicate an
 * error in the library
 */
typedef enum {
    /*! No error detected */
    MPESTI_T_ErrorCode_NoError = 0,
    /*! Incorrect packet was received */
    MPESTI_T_ErrorCode_Packet,
    /*! Command is not supported */
    MPESTI_T_ErrorCode_Cmd,
    /*! Active Phase Error */
    MPESTI_T_Active_Phase_Error,
    /*! PEC Error */
    MPESTI_T_PEC_Error,
    /*! Parity Error */
    MPESTI_T_Parity_Error,

} MPESTI_T_ErrorCode;

/*!
 * @brief MPESTI Target state sent to application layer
 *
 * MPESTI Target states that are being used to track the
 * pesti operation
 */
typedef enum {
    /*! Idle / Simple Presence / Ready to receive */
    MPESTI_T_Simple_Presence = 0,
    /*! Ready to receive and process commands */
    MPESTI_T_Ready_To_Receive,
    /*! Discovery Pay load Request */
    MPESTI_T_DPR,
    /*! Active Phase */
    MPESTI_T_VWE,
    /*! Error State */
    MPESTI_T_State_Error,
    /*! Initiator Abort */
    MPESTI_T_Initiator_Abort,
    /*! PEC Error detected */
    MPESTI_T_State_PECError,
    /*! Timeout Error */
    MPESTI_T_State_TimeOutError,
    /*! MPESTI Target State Unknown */
    MPESTI_T_State_Unknown
} MPESTI_T_State;

/*!
 * @brief MPESTI Target flags that are used to track the operation
 * during the different command operations
 */
typedef struct {
    /*! is DPR data sent out */
    uint8_t isDPRSent : 1;
    /*! is DPR payload loaded */
    uint8_t isDPRDataLoaded : 1;
    /*! is break sent out */
    uint8_t isBreakSent : 1;
    /*! is break received */
    uint8_t isBreakReceived : 1;
    /*! is VWE data sent */
    uint8_t isVWEDataSent : 1;
    /*! is VWE data loaded */
    uint8_t isVWEDataLoaded : 1;
    /*! Discovery Completed */
    uint8_t isDPRRequestReceived : 1;
    /*! OVR Error bit */
    uint8_t OVRErr : 1;
} MPESTI_T_FlagBits;

/*!
 * @brief DPR packet
 *
 * This structure takes an external buffer to load the
 * DPR packet values. It also takes user defined values for the
 * DPR packet
 */
typedef struct {
    /*! DPR payload */
    uint8_t *DPR_payload;
    /*! Vendor data to be sent out with DPR packet */
    uint8_t *vendorData;
    /*! Vendor data size to calculate the total payload size */
    uint8_t vendorDataSize;
    /*! Number of padding bytes to use */
    uint8_t noOfPaddingBytes;

} MPESTI_T_DPR_PKT;

/*!
 * @brief Main MPESTI Target object
 *
 * This structure is the main MPESTI Target related parameters
 * An instance of this structure should be created to configure the
 * MPESTI object
 */
typedef struct {
    /*! PHY and DataLink object */
    MPESTI_T_Phy phy;
    /*! Network object */
    MPESTI_T_Nwk nwk;
    /*! MPESTI Target reported state */
    MPESTI_T_State state;
    /*! Own Target address */
    uint8_t ownTargetAddr;
    /*! Discovery Payload */
    MPESTI_T_DPR_PKT *targetDPR;
    /*! Callback function for the broadcast command */
    void (*Broadcast_Callback)(uint8_t cmd);
    /*! Callback function for VWE command */
    void (*VWE_Callback)(void *);
    /*! Type of an Error */
    MPESTI_T_ErrorCode MPESTI_T_Error;
} MPESTI_Target;

/*!
 * @brief The parameters being used to configure the
 */
typedef struct {
    /*! Base address of the Ready to receive GPIO */
    GPIO_Regs *readyToReceiveGpio;
    /*! Ready to receive GPIO pin mask */
    uint32_t readyToReceivePinMask;
    /*! Base address of the UART */
    UART_Regs *targetUartBase;
    /*! Receive buffer */
    uint8_t *rxBuffPtr;
    /*! Discovery packet container */
    MPESTI_T_DPR_PKT *targetDPR;
    /*! Base address for the timer being used (This is a place holder) */
    GPTIMER_Regs *MPESTI_T_Phy_timerBase;
    /*! Boradcast command callback  */
    void (*Broadcast_Callback)(uint8_t cmd);
    /*! VWE command Callback function */
    void (*VWE_Callback)(void *);
} MPESTI_T_Params;

/*!
 *  @brief  Function to initialize the MPESTI Target object with the parameters provided
 *
 *  @param[in]  params     A pointer to a parameter structure.
 *
 *  @return Returns a status indicating success or failure of the initialization.
 *
 *  @retval MPESTI_T_RET_OK         The init was successful.
 *  @retval MPESTI_T_RET_NOT_OK     The init was not successful.
 *
 */
uint8_t MPESTI_T_init(MPESTI_T_Params *params);

/*!
 *  @brief  Set the target ready to receive
 *
 *  @param[in]  target     A pointer to a MPESTI Target structure.
 *
 */
void MPESTI_T_setReadyToReceive(MPESTI_Target *target);

/*!
 *  @brief  Clear UART interrupts
 *
 *  @param[in]  target     A pointer to a MPESTI Target structure.
 *
 */
void MPESTI_T_clearUARTInts(MPESTI_Target *target);

/*!
 *  @brief  This API will prepare the DPR packet to send out
 *
 *  @param[in]  packet     A pointer to a DPR packet structure.
 *
 *  @return Returns the total size of data being copied.
 *
 */
uint8_t MPESTI_T_prepareDPRResponse(MPESTI_T_DPR_PKT *packet);

/*!
 *  @brief  Uart interrupt handler
 */
void MPESTI_T_uartInterruptHandler(void);

/*!
 *  @brief  This API will send data to the initiator
 *
 *  @param[in]  target     A pointer to a MPESTI Target structure.
 *
 *  @return Returns if all the bytes sent out properly
 *
 *  @return true: all bytes are sent out
 *  @return false: all bytes are not sent out
 */
bool MPESTI_T_sendData(MPESTI_Target *target);

/*!
 *  @brief  Receive data
 *
 *  @param[in]  target     A pointer to a MPESTI Target structure.
 *
 *  @return Returns number of data received.
 *
 */
uint8_t MPESTI_T_receiveData(MPESTI_Target *target);

/*!
 *  @brief  Get the current state of the MPESTI Target state machine
 *
 *  @return Returns state of MPESTI Target state machine.
 *
 */
MPESTI_T_State MPESTI_T_getCurrentState(void);

/*!
 *  @brief  Get the error code
 *
 *  @return Returns a error code if any error have occurred.
 *
 */
MPESTI_T_ErrorCode MPESTI_T_getErrorCode(void);

/*!
 *  @brief  MPESTI Target state machine
 */
void MPESTI_T_stateMachine(void);

//*****************************************************************************
//
// Mark the end of the C bindings section for C++ compilers.
//
//*****************************************************************************
#ifdef __cplusplus
}
#endif

//*****************************************************************************
//
// Close the Doxygen group.
//! @}
//
//*****************************************************************************
#endif /* __MPESTI_TARGET_H__ */
/** @}*/
