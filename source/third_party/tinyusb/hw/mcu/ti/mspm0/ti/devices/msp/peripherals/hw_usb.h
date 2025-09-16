/*****************************************************************************

  Copyright (C) 2024 Texas Instruments Incorporated - http://www.ti.com/ 

  Redistribution and use in source and binary forms, with or without 
  modification, are permitted provided that the following conditions 
  are met:

   Redistributions of source code must retain the above copyright 
   notice, this list of conditions and the following disclaimer.

   Redistributions in binary form must reproduce the above copyright
   notice, this list of conditions and the following disclaimer in the 
   documentation and/or other materials provided with the   
   distribution.

   Neither the name of Texas Instruments Incorporated nor the names of
   its contributors may be used to endorse or promote products derived
   from this software without specific prior written permission.

  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS 
  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT 
  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
  A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT 
  OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, 
  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT 
  LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
  DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
  THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT 
  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE 
  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

*****************************************************************************/

#ifndef ti_devices_msp_peripherals_hw_usb__include
#define ti_devices_msp_peripherals_hw_usb__include

/* Filename: hw_usb.h */
/* Revised: 2024-10-17 19:02:43 */
/* Revision: 57ee25d5f5e8d3be5c1a7d5449fc2e7ef36548c9 */

#ifndef __CORTEX_M
  #ifdef __cplusplus
    #define __I  volatile        /*!< Defines 'read only' permissions */
  #else
    #define __I  volatile const  /*!< Defines 'read only' permissions */
  #endif
  #define __O  volatile          /*!< Defines 'write only' permissions */
  #define __IO  volatile         /*!< Defines 'read / write' permissions */

  /* following defines should be used for structure members */
  #define __IM  volatile const   /*! Defines 'read only' structure member permissions */
  #define __OM  volatile         /*! Defines 'write only' structure member permissions */
  #define __IOM  volatile        /*! Defines 'read / write' structure member permissions */
#endif

/* Use standard integer types with explicit width */
#include <stdint.h>

#ifdef __cplusplus
 extern "C" {
#endif

#if defined ( __CC_ARM )
#pragma anon_unions
#endif

/******************************************************************************
* USB Registers
******************************************************************************/
#define USB_CSR_OFS                              ((uint32_t)0x00002110U)
#define USB_TADDR_OFS                            ((uint32_t)0x00002080U)
#define USB_REGISTERS_OFS                        ((uint32_t)0x00002000U)
#define USB_CPU_INT_OFS                          ((uint32_t)0x00001020U)
#define USB_GPRCM_OFS                            ((uint32_t)0x00000800U)


/** @addtogroup USB_CSR
  @{
*/

typedef struct {
  __IO uint16_t TXMAXP;                            /* !< (@ 0x00002110) USB Maximum Transmit Data Endpoint [n] */
  __IO uint8_t TXCSRL;                            /* !< (@ 0x00002112) USB Transmit Control and Status Endpoint [n] Low */
  __IO uint8_t TXCSRH;                            /* !< (@ 0x00002113) USB Transmit Control and Status Endpoint [n] High */
  __IO uint16_t RXMAXP;                            /* !< (@ 0x00002114) USB Maximum Receive Data Endpoint [n] */
  __IO uint8_t RXCSRL;                            /* !< (@ 0x00002116) USB Receive Control and Status Endpoint [n] Low */
  __IO uint8_t RXCSRH;                            /* !< (@ 0x00002117) USB Receive Control and Status Endpoint [n] High */
  __IO uint16_t RXCOUNT;                           /* !< (@ 0x00002118) USB Receive Byte Count Endpoint [n] */
  __IO uint8_t TXTYPE;                            /* !< (@ 0x0000211A) USB Host Transmit Configure Type Endpoint [n] */
  __IO uint8_t TXINTERVAL;                        /* !< (@ 0x0000211B) USB Host Transmit Interval Endpoint [n] */
  __IO uint8_t RXTYPE;                            /* !< (@ 0x0000211C) USB Host Configure Receive Type Endpoint [n] */
  __IO uint8_t RXINTERVAL;                        /* !< (@ 0x0000211D) USB Host Receive Polling Interval Endpoint [n] */
       uint8_t  RESERVED0;
  __I  uint8_t FIFOSZ;                            /* !< (@ 0x0000211F) USB TX and RX FIFO Size Endpoint [n] */
} USB_CSR_Regs;

/*@}*/ /* end of group USB_CSR */

/** @addtogroup USB_TADDR
  @{
*/

typedef struct {
  __IO uint8_t TXFUNCADDR;                        /* !< (@ 0x00002080) USB Transmit Functional Address Endpoint [n] */
       uint8_t  RESERVED0;
  __IO uint8_t TXHUBADDR;                         /* !< (@ 0x00002082) USB Transmit Hub Address Endpoint [n] */
  __IO uint8_t TXHUBPORT;                         /* !< (@ 0x00002083) USB Transmit Hub Port Endpoint [n] */
  __IO uint8_t RXFUNCADDR;                        /* !< (@ 0x00002084) USB Receive Functional Address Endpoint [n] */
       uint8_t  RESERVED1;
  __IO uint8_t RXHUBADDR;                         /* !< (@ 0x00002086) USB Receive Hub Address Endpoint [n] */
  __IO uint8_t RXHUBPORT;                         /* !< (@ 0x00002087) USB Receive Hub Port Endpoint [n] */
} USB_TADDR_Regs;

/*@}*/ /* end of group USB_TADDR */

/** @addtogroup USB_REGISTERS
  @{
*/

typedef struct {
  __IO uint8_t FADDR;                             /* !< (@ 0x00002000) USB Device Functional Address */
  __IO uint8_t POWER;                             /* !< (@ 0x00002001) USB Power Control */
  __I  uint16_t TXIS;                              /* !< (@ 0x00002002) USB Transmit Interrupt Status */
  __I  uint16_t RXIS;                              /* !< (@ 0x00002004) USB Receive Interrupt Status */
  __IO uint16_t TXIE;                              /* !< (@ 0x00002006) USB Transmit Interrupt Enable */
  __IO uint16_t RXIE;                              /* !< (@ 0x00002008) USB Receive Interrupt Enable */
  __IO uint8_t USBIS;                             /* !< (@ 0x0000200A) USB General Interrupt Status */
  __IO uint8_t USBIE;                             /* !< (@ 0x0000200B) USB General Interrupt Enable */
  __IO uint16_t FRAME;                             /* !< (@ 0x0000200C) USB Last Received Frame Number */
  __IO uint8_t EPINDEX;                           /* !< (@ 0x0000200E) USB Endpoint Index */
  __IO uint8_t TESTMODE;                          /* !< (@ 0x0000200F) USB Test Mode */
  __IO uint16_t IDXTXMAXP;                         /* !< (@ 0x00002010) Indexed TXMAP */
  __IO uint8_t IDXTXCSRL;                         /* !< (@ 0x00002012) Indexed TXCSRL */
  __IO uint8_t IDXTXCSRH;                         /* !< (@ 0x00002013) Indexed TXCSRH */
  __IO uint16_t IDXRXMAXP;                         /* !< (@ 0x00002014) Indexed RXMAXP */
  __IO uint8_t IDXRXCSRL;                         /* !< (@ 0x00002016) Indexed RXCSRL */
  __IO uint8_t IDXRXCSRH;                         /* !< (@ 0x00002017) Indexed RXCSRH */
  __IO uint16_t IDXRXCOUNT;                        /* !< (@ 0x00002018) Indexed RXCOUNT */
  __IO uint8_t IDXTXTYPE;                         /* !< (@ 0x0000201A) Indexed TXTYPE */
  __IO uint8_t IDXTXINTERVAL;                     /* !< (@ 0x0000201B) Indexed TXINTERVAL */
  __IO uint8_t IDXRXTYPE;                         /* !< (@ 0x0000201C) Indexed RXTYPE */
  __IO uint8_t IDXRXINTERVAL;                     /* !< (@ 0x0000201D) Indexed RXINTERVAL */
       uint8_t  RESERVED0;
  __IO uint8_t IDXFIFOSZ;                         /* !< (@ 0x0000201F) Indexed FIFOSZ */
  union {
      __IO uint32_t FIFO[16];                          /* !< USB FIFO Endpoint [n] */
      __IO uint8_t FIFO_BYTE[16];                     /* !< Byte access to FIFO for Endpoint [y]. */
      __IO uint16_t FIFO_HALF[16];                     /* !< Half word access to FIFO for Endpoint [y]. */
      __IO uint32_t FIFO_WORD[16];                     /* !< Word access to FIFO for Endpoint [y]. */
  };
  __IO uint8_t DEVCTL;                            /* !< (@ 0x00002060) USB Device Control */
  __IO uint8_t MISC;                              /* !< (@ 0x00002061) USB Miscellaneous Register */
  __IO uint8_t IDXTXFIFOSZ;                       /* !< (@ 0x00002062) Indexed USB Transmit Dynamic FIFO Sizing */
  __IO uint8_t IDXRXFIFOSZ;                       /* !< (@ 0x00002063) Indexed USB Receive Dynamic FIFO Sizing */
  __IO uint16_t IDXTXFIFOADD;                      /* !< (@ 0x00002064) Indexed USB Transmit FIFO Start Address */
  __IO uint16_t IDXRXFIFOADD;                      /* !< (@ 0x00002066) Indexed USB Receive FIFO Start Address */
       uint32_t RESERVED1[4];
  __I  uint8_t EPINFO;                            /* !< (@ 0x00002078) Endpoint Information */
  __I  uint8_t RAMINFO;                           /* !< (@ 0x00002079) USB FIFO Buffer RAM Information */
  __IO uint8_t CONTIM;                            /* !< (@ 0x0000207A) USB Connect Timing */
       uint8_t RESERVED2[2];
  __IO uint8_t FSEOF;                             /* !< (@ 0x0000207D) USB Full-Speed Last Transaction to End of Frame
                                                      Timing */
  __IO uint8_t LSEOF;                             /* !< (@ 0x0000207E) USB Low-Speed Last Transaction to End of Frame
                                                      Timing */
       uint8_t  RESERVED3;
  USB_TADDR_Regs  TADDR[16];                         /* !< (@ 0x00002080) */
       uint16_t RESERVED4;
  __IO uint8_t CSR0L;                             /* !< (@ 0x00002102) USB Control and Status Endpoint 0 Low */
  __IO uint8_t CSR0H;                             /* !< (@ 0x00002103) USB Control and Status Endpoint 0 High */
       uint32_t RESERVED5;
  __IO uint8_t COUNT0;                            /* !< (@ 0x00002108) USB Receive Byte Count Endpoint 0 */
       uint8_t  RESERVED6;
  __IO uint8_t TYPE0;                             /* !< (@ 0x0000210A) USB Type Endpoint 0 */
  __IO uint8_t NAKLMT0;                           /* !< (@ 0x0000210B) USB NAK Limit Endpoint 0 */
       uint8_t  RESERVED7[3];
  __I  uint8_t CONFIG;                            /* !< (@ 0x0000210F) USB Config Data */
  USB_CSR_Regs  CSR[15];                           /* !< (@ 0x00002110) */
       uint32_t RESERVED8[65];
  __IO uint32_t RQPKTCOUNT[15];                    /* !< (@ 0x00002304) USB Request Packet Count in Block Transfer
                                                      Endpoint [n] */
  __IO uint16_t RXDPKTBUFDIS;                      /* !< (@ 0x00002340) USB Receive Double Packet Buffer Disable */
  __IO uint16_t TXDPKTBUFDIS;                      /* !< (@ 0x00002342) USB Transmit Double Packet Buffer Disable */
} USB_REGISTERS_Regs;

/*@}*/ /* end of group USB_REGISTERS */

/** @addtogroup USB_CPU_INT
  @{
*/

typedef struct {
  __I  uint32_t IIDX;                              /* !< (@ 0x00001020) Interrupt index */
       uint32_t RESERVED0;
  __IO uint32_t IMASK;                             /* !< (@ 0x00001028) Interrupt mask */
       uint32_t RESERVED1;
  __I  uint32_t RIS;                               /* !< (@ 0x00001030) Raw interrupt status */
       uint32_t RESERVED2;
  __I  uint32_t MIS;                               /* !< (@ 0x00001038) Masked interrupt status */
       uint32_t RESERVED3;
  __O  uint32_t ISET;                              /* !< (@ 0x00001040) Interrupt set */
       uint32_t RESERVED4;
  __O  uint32_t ICLR;                              /* !< (@ 0x00001048) Interrupt clear */
} USB_CPU_INT_Regs;

/*@}*/ /* end of group USB_CPU_INT */

/** @addtogroup USB_GPRCM
  @{
*/

typedef struct {
  __IO uint32_t PWREN;                             /* !< (@ 0x00000800) Power enable */
  __O  uint32_t RSTCTL;                            /* !< (@ 0x00000804) Reset Control */
  __IO uint32_t CLKCTL;                            /* !< (@ 0x00000808) Clock control */
       uint32_t RESERVED0[2];
  __I  uint32_t STAT;                              /* !< (@ 0x00000814) Status Register */
} USB_GPRCM_Regs;

/*@}*/ /* end of group USB_GPRCM */

/** @addtogroup USB
  @{
*/

typedef struct {
       uint32_t RESERVED0[512];
  USB_GPRCM_Regs  GPRCM;                             /* !< (@ 0x00000800) */
       uint32_t RESERVED1[514];
  USB_CPU_INT_Regs  CPU_INT;                           /* !< (@ 0x00001020) */
       uint32_t RESERVED2[37];
  __I  uint32_t EVT_MODE;                          /* !< (@ 0x000010E0) Event Mode */
       uint32_t RESERVED3[6];
  __I  uint32_t DESC;                              /* !< (@ 0x000010FC) Module Description */
  __IO uint32_t USBMODE;                           /* !< (@ 0x00001100) USB mode control */
  __IO uint32_t USBMONITOR;                        /* !< (@ 0x00001104) USB Clock Control Register */
  __IO uint32_t USBDMASEL;                         /* !< (@ 0x00001108) USB DMA trigger select */
       uint32_t RESERVED4[957];
  USB_REGISTERS_Regs  REGISTERS;                         /* !< (@ 0x00002000) */
} USB_Regs;

/*@}*/ /* end of group USB */



#if defined ( __CC_ARM )
#pragma no_anon_unions
#endif

/******************************************************************************
* USB Register Control Bits
******************************************************************************/

/* USB_TXMAXP Bits */
/* USB_TXMAXP[MAXLOAD] Bits */
#define USB_TXMAXP_MAXLOAD_OFS                   (0)                             /* !< MAXLOAD Offset */
#define USB_TXMAXP_MAXLOAD_MASK                  ((uint16_t)0x000007FFU)         /* !< Maximum Payload specifies the
                                                                                    maximum payload in bytes per
                                                                                    transaction. */

/* USB_TXCSRL Bits */
/* USB_TXCSRL[TXRDY] Bits */
#define USB_TXCSRL_TXRDY_OFS                     (0)                             /* !< TXRDY Offset */
#define USB_TXCSRL_TXRDY_MASK                    ((uint8_t)0x00000001U)          /* !< Transmit Packet Ready. The CPU sets
                                                                                    this bit after loading a data packet
                                                                                    into the FIFO.  This bit is cleared
                                                                                    automatically when a data packet has
                                                                                    been transmitted. The EPn bit in the
                                                                                    USBTXIS register is also set at this
                                                                                    point. TXRDY is also automatically
                                                                                    cleared prior to loading a second
                                                                                    packet into a double-buffered FIFO.
                                                                                    Note: Host and device mode */
#define USB_TXCSRL_TXRDY_WAIT                    ((uint8_t)0x00000000U)          /* !< No transmit packet is ready. */
#define USB_TXCSRL_TXRDY_READY                   ((uint8_t)0x00000001U)          /* !< Software sets this bit after
                                                                                    loading a data packet into the TX
                                                                                    FIFO. */
/* USB_TXCSRL[FIFONE] Bits */
#define USB_TXCSRL_FIFONE_OFS                    (1)                             /* !< FIFONE Offset */
#define USB_TXCSRL_FIFONE_MASK                   ((uint8_t)0x00000002U)          /* !< FIFO Not Empty. The USB controller
                                                                                    sets this bit when there is at least
                                                                                    1 packet in the TX FIFO. Note: Host
                                                                                    and device mode. */
#define USB_TXCSRL_FIFONE_EMPTY                  ((uint8_t)0x00000000U)          /* !< The FIFO is empty */
#define USB_TXCSRL_FIFONE_NOT_EMPTY              ((uint8_t)0x00000002U)          /* !< At least one packet is in the
                                                                                    transmit FIFO. */
/* USB_TXCSRL[UNDRNERROR] Bits */
#define USB_TXCSRL_UNDRNERROR_OFS                (2)                             /* !< UNDRNERROR Offset */
#define USB_TXCSRL_UNDRNERROR_MASK               ((uint8_t)0x00000004U)          /* !< Underrun / Error condition. Device
                                                                                    mode: The USB controller sets this
                                                                                    bit if an IN token is received when
                                                                                    TXRDY is not set. The CPU should
                                                                                    clear this bit. Host mode: The USB
                                                                                    controller sets this bit when 3
                                                                                    attempts have been made to send a
                                                                                    packet and no handshake packet has
                                                                                    been received. When the bit is set,
                                                                                    an interrupt is generated, TXRDY is
                                                                                    cleared and the FIFO is completely
                                                                                    flushed. The CPU should clear this
                                                                                    bit. Valid only when the endpoint is
                                                                                    operating in Bulk or Interrupt mode. */
#define USB_TXCSRL_UNDRNERROR_CLEAR              ((uint8_t)0x00000000U)          /* !< No underrun or error condition */
#define USB_TXCSRL_UNDRNERROR_SET                ((uint8_t)0x00000004U)          /* !< Underrun or error condition exist. */
/* USB_TXCSRL[FLUSH] Bits */
#define USB_TXCSRL_FLUSH_OFS                     (3)                             /* !< FLUSH Offset */
#define USB_TXCSRL_FLUSH_MASK                    ((uint8_t)0x00000008U)          /* !< Flush FIFO. The CPU writes a 1 to
                                                                                    this bit to flush the latest packet
                                                                                    from the endpoint TX FIFO. The FIFO
                                                                                    pointer is reset, the TXRDY bit is
                                                                                    cleared and an interrupt is
                                                                                    generated. May be set simultaneously
                                                                                    with TXRDY to abort the packet that
                                                                                    is currently being loaded into the
                                                                                    FIFO.  Note: FLUSH should only be
                                                                                    used when TXRDY is set. At other
                                                                                    times, it may cause data to be
                                                                                    corrupted. Also note that, if the
                                                                                    FIFO is double-buffered, FLUSH may
                                                                                    need to be set twice to completely
                                                                                    clear the FIFO. Note: Host and device
                                                                                    mode. */
#define USB_TXCSRL_FLUSH_NO_EFFECT               ((uint8_t)0x00000000U)          /* !< No effect */
#define USB_TXCSRL_FLUSH_EFFECT                  ((uint8_t)0x00000008U)          /* !< Flushes the latest packet from the
                                                                                    endpoint transmit FIFO. */
/* USB_TXCSRL[STALLSETUP] Bits */
#define USB_TXCSRL_STALLSETUP_OFS                (4)                             /* !< STALLSETUP Offset */
#define USB_TXCSRL_STALLSETUP_MASK               ((uint8_t)0x00000010U)          /* !< Send stall / Setup packet request.
                                                                                    Device mode: The CPU writes a 1 to
                                                                                    this bit to issue a STALL handshake
                                                                                    to an IN token. The CPU clears this
                                                                                    bit to terminate the stall condition.
                                                                                    Note: This bit has no effect where
                                                                                    the endpoint is being used for
                                                                                    Isochronous transfers. Host mode: The
                                                                                    CPU sets this bit, at the same time
                                                                                    as the TXRDY bit is set, to send a
                                                                                    SETUP token instead of an OUT token
                                                                                    for the transaction. Note: Setting
                                                                                    this bit also clears the USBTXCSRH.DT
                                                                                    register. */
#define USB_TXCSRL_STALLSETUP_CLEAR              ((uint8_t)0x00000000U)          /* !< No effect */
#define USB_TXCSRL_STALLSETUP_SET                ((uint8_t)0x00000010U)          /* !< Send stall or setup packet request. */
/* USB_TXCSRL[STALLED] Bits */
#define USB_TXCSRL_STALLED_OFS                   (5)                             /* !< STALLED Offset */
#define USB_TXCSRL_STALLED_MASK                  ((uint8_t)0x00000020U)          /* !< Sent stall / RX stalled. Device
                                                                                    mode: This bit is set when a STALL
                                                                                    handshake is transmitted. The FIFO is
                                                                                    flushed and the TXRDY bit is cleared.
                                                                                    The CPU should clear this bit. Host
                                                                                    mode: This bit is set when a STALL
                                                                                    handshake is received. When this bit
                                                                                    is set, any DMA request that is in
                                                                                    progress is stopped, the FIFO is
                                                                                    completely flushed and the TXRDY bit
                                                                                    is cleared. The CPU should clear this
                                                                                    bit. */
#define USB_TXCSRL_STALLED_CLEAR                 ((uint8_t)0x00000000U)          /* !< Clear stall indication. */
#define USB_TXCSRL_STALLED_SET                   ((uint8_t)0x00000020U)          /* !< Indicates that a STALL handshake
                                                                                    has been sent or received. */
/* USB_TXCSRL[CLRDT] Bits */
#define USB_TXCSRL_CLRDT_OFS                     (6)                             /* !< CLRDT Offset */
#define USB_TXCSRL_CLRDT_MASK                    ((uint8_t)0x00000040U)          /* !< Clear DataToggle. The CPU writes a
                                                                                    1 to this bit to reset the endpoint
                                                                                    data toggle to 0. Note: Host and
                                                                                    device mode. */
#define USB_TXCSRL_CLRDT_NO_EFFECT               ((uint8_t)0x00000000U)          /* !< No effect */
#define USB_TXCSRL_CLRDT_EFFECT                  ((uint8_t)0x00000040U)          /* !< Writing a 1 to this bit clears the
                                                                                    DT bit in the USBTXCSRH[n] register. */
/* USB_TXCSRL[INCOMPNAKTO] Bits */
#define USB_TXCSRL_INCOMPNAKTO_OFS               (7)                             /* !< INCOMPNAKTO Offset */
#define USB_TXCSRL_INCOMPNAKTO_MASK              ((uint8_t)0x00000080U)          /* !< Incomplete TX / NAK Timeout. Device
                                                                                    mode: When the endpoint is being used
                                                                                    for high-bandwidth Isochronous, this
                                                                                    bit is set to indicate where a large
                                                                                    packet has been split into 2 or 3
                                                                                    packets for transmission but
                                                                                    insufficient IN tokens have been
                                                                                    received to send all the parts. Note:
                                                                                    In anything other than isochronous
                                                                                    transfers, this bit will always
                                                                                    return 0. Host mode - bulk endpoints
                                                                                    only: This bit will be set when the
                                                                                    TX endpoint is halted following the
                                                                                    receipt of NAK responses for longer
                                                                                    than the time set as theTXPOLLNAKLMT
                                                                                    by the TXINTERVAL register. The CPU
                                                                                    should clear this bit to allow the
                                                                                    endpoint to continue. Host mode -
                                                                                    high-bandwidth Interrupt endpoints
                                                                                    only: This bit will be set if no
                                                                                    response is received from the device
                                                                                    to which the packet is being sent. */
#define USB_TXCSRL_INCOMPNAKTO_CLEAR             ((uint8_t)0x00000000U)          /* !< No condition present / clear
                                                                                    condition. */
#define USB_TXCSRL_INCOMPNAKTO_SET               ((uint8_t)0x00000080U)          /* !< Condition present. */

/* USB_TXCSRH Bits */
/* USB_TXCSRH[DT] Bits */
#define USB_TXCSRH_DT_OFS                        (0)                             /* !< DT Offset */
#define USB_TXCSRH_DT_MASK                       ((uint8_t)0x00000001U)          /* !< Data Toggle. When read, this bit
                                                                                    indicates the current state of the
                                                                                    transmit endpoint data toggle. If
                                                                                    DTWE is High, this bit can be written
                                                                                    with the required setting of the data
                                                                                    toggle. If DTWE is Low, any value
                                                                                    written to this bit is ignored. Care
                                                                                    should be taken when writing to this
                                                                                    bit as it should only be changed to
                                                                                    RESET the transmit endpoint. Note:
                                                                                    Host mode only */
#define USB_TXCSRH_DT_LOW                        ((uint8_t)0x00000000U)          /* !< Data toggle is low */
#define USB_TXCSRH_DT_HIGH                       ((uint8_t)0x00000001U)          /* !< Data toggle is high */
/* USB_TXCSRH[DTWE] Bits */
#define USB_TXCSRH_DTWE_OFS                      (1)                             /* !< DTWE Offset */
#define USB_TXCSRH_DTWE_MASK                     ((uint8_t)0x00000002U)          /* !< Data Toggle Write Enable.  The CPU
                                                                                    writes a 1 to this bit to enable the
                                                                                    current state of the TX Endpoint data
                                                                                    toggle to be written. This bit is
                                                                                    automatically cleared once the new
                                                                                    value is written. Note: Host mode
                                                                                    only. */
#define USB_TXCSRH_DTWE_DISABLE                  ((uint8_t)0x00000000U)          /* !< The DT bit cannot be written. */
#define USB_TXCSRH_DTWE_ENABLE                   ((uint8_t)0x00000002U)          /* !< Enables the current state of the
                                                                                    transmit endpoint data to be written
                                                                                    (see DT bit). */
/* USB_TXCSRH[DMAMOD] Bits */
#define USB_TXCSRH_DMAMOD_OFS                    (2)                             /* !< DMAMOD Offset */
#define USB_TXCSRH_DMAMOD_MASK                   ((uint8_t)0x00000004U)          /* !< DMA Request Mode. The CPU sets this
                                                                                    bit to select DMA Request Mode 1 and
                                                                                    clears it to select DMA Request Mode
                                                                                    0. Note: This bit must not be cleared
                                                                                    either before or in the same cycle as
                                                                                    the DMAEN bit is cleared. Note:
                                                                                    Device and host mode. */
#define USB_TXCSRH_DMAMOD_MODE0                  ((uint8_t)0x00000000U)          /* !< An interrupt is generated after
                                                                                    every DMA packet transfer. */
#define USB_TXCSRH_DMAMOD_MODE1                  ((uint8_t)0x00000004U)          /* !< An interrupt is generated only
                                                                                    after the entire DMA transfer is
                                                                                    complete. */
/* USB_TXCSRH[FDT] Bits */
#define USB_TXCSRH_FDT_OFS                       (3)                             /* !< FDT Offset */
#define USB_TXCSRH_FDT_MASK                      ((uint8_t)0x00000008U)          /* !< Force Data Toggle. The CPU sets
                                                                                    this bit to force the endpoint DT bit
                                                                                    to switch and the data packet to be
                                                                                    cleared from the FIFO, regardless of
                                                                                    whether an ACK was received. This can
                                                                                    be used by Interrupt TX endpoints
                                                                                    that are used to communicate rate
                                                                                    feedback for Isochronous endpoints.
                                                                                    Note: This bit should only be set
                                                                                    when the TXRDY bit is set. At other
                                                                                    times, it may cause data to be
                                                                                    corrupted. Note: Device and host
                                                                                    mode. */
#define USB_TXCSRH_FDT_NO_EFFECT                 ((uint8_t)0x00000000U)          /* !< No effect */
#define USB_TXCSRH_FDT_EFFECT                    ((uint8_t)0x00000008U)          /* !< Forces data toggle */
/* USB_TXCSRH[DMAEN] Bits */
#define USB_TXCSRH_DMAEN_OFS                     (4)                             /* !< DMAEN Offset */
#define USB_TXCSRH_DMAEN_MASK                    ((uint8_t)0x00000010U)          /* !< DMA Request Enable The CPU sets
                                                                                    this bit to enable the DMA request
                                                                                    for the TX endpoint. Note: Four TX
                                                                                    and four RX endpoints can be
                                                                                    connected to the DMA module. If this
                                                                                    bit is set for a particular endpoint,
                                                                                    the TRIGATX, TRIGBTX, TRIGCTX, or
                                                                                    TRIGDTX field in the USB DMA Select
                                                                                    (USBDMASEL) register must be
                                                                                    programmed correspondingly. Note:
                                                                                    Device and host mode. */
#define USB_TXCSRH_DMAEN_DISABLE                 ((uint8_t)0x00000000U)          /* !< Disables the DMA request for the
                                                                                    transmit endpoint. */
#define USB_TXCSRH_DMAEN_ENABLE                  ((uint8_t)0x00000010U)          /* !< Enables the DMA request for the
                                                                                    transmit endpoint. */
/* USB_TXCSRH[MODE] Bits */
#define USB_TXCSRH_MODE_OFS                      (5)                             /* !< MODE Offset */
#define USB_TXCSRH_MODE_MASK                     ((uint8_t)0x00000020U)          /* !< Mode The CPU sets this bit to
                                                                                    enable the endpoint direction as TX,
                                                                                    and clears the bit to enable it as
                                                                                    Rx.  Note: This bit only has an
                                                                                    effect when the same endpoint FIFO is
                                                                                    used for both transmit and receive
                                                                                    transactions. Note: Host and device
                                                                                    mode. */
#define USB_TXCSRH_MODE_RX                       ((uint8_t)0x00000000U)          /* !< Enables the endpoint direction as
                                                                                    RX. */
#define USB_TXCSRH_MODE_TX                       ((uint8_t)0x00000020U)          /* !< Enables the endpoint direction as
                                                                                    TX. */
/* USB_TXCSRH[ISO] Bits */
#define USB_TXCSRH_ISO_OFS                       (6)                             /* !< ISO Offset */
#define USB_TXCSRH_ISO_MASK                      ((uint8_t)0x00000040U)          /* !< Isochronous Transfers. The CPU sets
                                                                                    this bit to enable the TX endpoint
                                                                                    for Isochronous transfers, and clears
                                                                                    it to enable the TX endpoint for Bulk
                                                                                    or Interrupt transfers.  Note: This
                                                                                    bit only has any effect in device
                                                                                    mode. In host mode, it always returns
                                                                                    zero. */
#define USB_TXCSRH_ISO_BULK_INT_TRANSFER         ((uint8_t)0x00000000U)          /* !< Enables the transmit endpoint for
                                                                                    bulk or interrupt transfers. */
#define USB_TXCSRH_ISO_ISO_TRANSFER              ((uint8_t)0x00000040U)          /* !< Enables the transmit endpoint for
                                                                                    isochronous transfers. */
/* USB_TXCSRH[AUTOSET] Bits */
#define USB_TXCSRH_AUTOSET_OFS                   (7)                             /* !< AUTOSET Offset */
#define USB_TXCSRH_AUTOSET_MASK                  ((uint8_t)0x00000080U)          /* !< Auto Set. If the CPU sets this bit,
                                                                                    the TXRDY bit will be be
                                                                                    automatically set when data of the
                                                                                    maximum packet size (value in
                                                                                    USBTXMAXP[n]) is loaded into the
                                                                                    transmit FIFO. If a packet of less
                                                                                    than the maximum packet size is
                                                                                    loaded, then the TXRDY bit must be
                                                                                    set manually. Note: Should not be set
                                                                                    for either high-bandwidth Isochronous
                                                                                    endpoints or high-bandwidth Interrupt
                                                                                    endpoints. Note: Device and host
                                                                                    mode. */
#define USB_TXCSRH_AUTOSET_MANUAL                ((uint8_t)0x00000000U)          /* !< The TXRDY bit must be set manually. */
#define USB_TXCSRH_AUTOSET_AUTO                  ((uint8_t)0x00000080U)          /* !< The TXRDY bit is automatically set
                                                                                    when maximum packet size is loaded
                                                                                    into the TX FIFO. */

/* USB_RXMAXP Bits */
/* USB_RXMAXP[MAXLOAD] Bits */
#define USB_RXMAXP_MAXLOAD_OFS                   (0)                             /* !< MAXLOAD Offset */
#define USB_RXMAXP_MAXLOAD_MASK                  ((uint16_t)0x000007FFU)         /* !< Maximum Payload specifies the
                                                                                    maximum payload in bytes per
                                                                                    transaction. */

/* USB_RXCSRL Bits */
/* USB_RXCSRL[RXRDY] Bits */
#define USB_RXCSRL_RXRDY_OFS                     (0)                             /* !< RXRDY Offset */
#define USB_RXCSRL_RXRDY_MASK                    ((uint8_t)0x00000001U)          /* !< Receive Packet Ready. This bit is
                                                                                    set when a data packet has been
                                                                                    received. The CPU should clear this
                                                                                    bit when the packet has been unloaded
                                                                                    from the Rx FIFO. An interrupt is
                                                                                    generated when the bit is set. If the
                                                                                    AUTOCLR bit in the USBRXCSRH[n]
                                                                                    register is set, then the this bit is
                                                                                    automatically cleared when a packet
                                                                                    of USBRXMAXP[n] bytes has been
                                                                                    unloaded from the receive FIFO. If
                                                                                    the AUTOCLR bit is clear, or if
                                                                                    packets of less than the maximum
                                                                                    packet size are unloaded, then
                                                                                    software must clear this bit manually
                                                                                    when the packet has been unloaded
                                                                                    from the receive FIFO. Note: Device
                                                                                    and host mode. */
#define USB_RXCSRL_RXRDY_WAIT                    ((uint8_t)0x00000000U)          /* !< No data packet has been received. */
#define USB_RXCSRL_RXRDY_READY                   ((uint8_t)0x00000001U)          /* !< Indicates that a data packet has
                                                                                    been received. The EP[j] bit in the
                                                                                    USBTXIS register is also set in this
                                                                                    situation */
/* USB_RXCSRL[FULL] Bits */
#define USB_RXCSRL_FULL_OFS                      (1)                             /* !< FULL Offset */
#define USB_RXCSRL_FULL_MASK                     ((uint8_t)0x00000002U)          /* !< FIFO Full. This bit is set when no
                                                                                    more packets can be loaded into the
                                                                                    RX FIFO. Note: Device and host mode. */
#define USB_RXCSRL_FULL_NO                       ((uint8_t)0x00000000U)          /* !< The receive FIFO is not full. */
#define USB_RXCSRL_FULL_YES                      ((uint8_t)0x00000002U)          /* !< The receive FIFO is full. No more
                                                                                    packets can be loaded into the
                                                                                    receive FIFO. */
/* USB_RXCSRL[OVERERROR] Bits */
#define USB_RXCSRL_OVERERROR_OFS                 (2)                             /* !< OVERERROR Offset */
#define USB_RXCSRL_OVERERROR_MASK                ((uint8_t)0x00000004U)          /* !< Over run / Error. Device mode: Over
                                                                                    run - This bit is set if an OUT
                                                                                    packet cannot be loaded into the
                                                                                    receive FIFO. The CPU should clear
                                                                                    this bit. - Note: This bit is only
                                                                                    valid when the endpoint is operating
                                                                                    in ISO mode. In Bulk mode, it always
                                                                                    returns zero. Host mode: The USB sets
                                                                                    this bit when 3 attempts have been
                                                                                    made to receive a packet and no data
                                                                                    packet has been received. The CPU
                                                                                    should clear this bit. An interrupt
                                                                                    is generated when the bit is set. -
                                                                                    Note: This bit is only valid when the
                                                                                    RX endpoint is operating in Bulk or
                                                                                    Interrupt mode. In ISO mode, it
                                                                                    always returns zero. */
#define USB_RXCSRL_OVERERROR_CLEAR               ((uint8_t)0x00000000U)          /* !< No over run or error condition. */
#define USB_RXCSRL_OVERERROR_SET                 ((uint8_t)0x00000004U)          /* !< Under run or error condition exist. */
/* USB_RXCSRL[DATAERRNAKTO] Bits */
#define USB_RXCSRL_DATAERRNAKTO_OFS              (3)                             /* !< DATAERRNAKTO Offset */
#define USB_RXCSRL_DATAERRNAKTO_MASK             ((uint8_t)0x00000008U)          /* !< Data Error / NAK Timeout Device
                                                                                    mode: Data Error - This bit is set
                                                                                    when RXRDY is set, if the data packet
                                                                                    has a CRC or bit-stuff error. It is
                                                                                    cleared when RXRDY is cleared. -
                                                                                    Note: This bit is only valid when the
                                                                                    endpoint is operating in ISO mode. In
                                                                                    Bulk mode, it always returns zero.
                                                                                    Host mode: NAK Timeout - When
                                                                                    operating in ISO mode, this bit is
                                                                                    set when RXRDY is set, if the data
                                                                                    packet has a CRC or bit-stuff error
                                                                                    and cleared when RXRDY is cleared. In
                                                                                    Bulk mode, this bit will be set when
                                                                                    the Rx endpoint is halted following
                                                                                    the receipt of NAK responses for
                                                                                    longer than the time set as the NAK
                                                                                    Limit by the USBRXINTERVAL register.
                                                                                    The CPU should clear this bit to
                                                                                    allow the endpoint to continue. */
#define USB_RXCSRL_DATAERRNAKTO_CLEAR            ((uint8_t)0x00000000U)          /* !< Normal operation */
#define USB_RXCSRL_DATAERRNAKTO_SET              ((uint8_t)0x00000008U)          /* !< Data error or NAK Timeout condition
                                                                                    exist. */
/* USB_RXCSRL[FLUSH] Bits */
#define USB_RXCSRL_FLUSH_OFS                     (4)                             /* !< FLUSH Offset */
#define USB_RXCSRL_FLUSH_MASK                    ((uint8_t)0x00000010U)          /* !< Flush FIFO. The CPU writes a 1 to
                                                                                    this bit to flush the next packet to
                                                                                    be read from the endpoint RX FIFO.
                                                                                    The FIFO pointer is reset and the
                                                                                    RXRDY bit is cleared.  Note: Thsi bit
                                                                                    should only be set when RXRDY is set.
                                                                                    At other times, it may cause data to
                                                                                    be corrupted.  Note: If the FIFO is
                                                                                    double-buffered, FLUSH may need to be
                                                                                    set twice to completely clear the
                                                                                    FIFO. Note: Device and host mode. */
#define USB_RXCSRL_FLUSH_NO_EFFECT               ((uint8_t)0x00000000U)          /* !< No effect */
#define USB_RXCSRL_FLUSH_EFFECT                  ((uint8_t)0x00000010U)          /* !< Flushes the next packet to be read
                                                                                    from the endpoint receive FIFO. The
                                                                                    FIFO pointer is reset and the RXRDY
                                                                                    bit is cleared */
/* USB_RXCSRL[STALLREQPKT] Bits */
#define USB_RXCSRL_STALLREQPKT_OFS               (5)                             /* !< STALLREQPKT Offset */
#define USB_RXCSRL_STALLREQPKT_MASK              ((uint8_t)0x00000020U)          /* !< Send Stall / Request Packet. Device
                                                                                    mode: The CPU writes a 1 to this bit
                                                                                    to issue a STALL handshake. The CPU
                                                                                    clears this bit to terminate the
                                                                                    stall condition. - Note: This bit has
                                                                                    no effect where the endpoint is being
                                                                                    used for Isochronous transfers. Host
                                                                                    mode: The CPU writes a 1 to this bit
                                                                                    to request an IN transaction. It is
                                                                                    cleared when RXRDY is set. */
#define USB_RXCSRL_STALLREQPKT_CLEAR             ((uint8_t)0x00000000U)          /* !< No effect */
#define USB_RXCSRL_STALLREQPKT_SET               ((uint8_t)0x00000020U)          /* !< Send stall or packet request. */
/* USB_RXCSRL[STALLED] Bits */
#define USB_RXCSRL_STALLED_OFS                   (6)                             /* !< STALLED Offset */
#define USB_RXCSRL_STALLED_MASK                  ((uint8_t)0x00000040U)          /* !< Sent Stall / RX Stall Device mode:
                                                                                    This bit is set when a STALL
                                                                                    handshake is transmitted. The CPU
                                                                                    should clear this bit. Host mode:
                                                                                    When a STALL handshake is received,
                                                                                    this bit is set and an interrupt is
                                                                                    generated. The CPU should clear this
                                                                                    bit. */
#define USB_RXCSRL_STALLED_STALLED_WAIT          ((uint8_t)0x00000000U)          /* !< No handshake has been received. */
#define USB_RXCSRL_STALLED_STALLED_RECEIVE       ((uint8_t)0x00000040U)          /* !< A STALL handshake has been
                                                                                    received. The EPn bit in the USBRXIS
                                                                                    register is also set. */
/* USB_RXCSRL[CLRDT] Bits */
#define USB_RXCSRL_CLRDT_OFS                     (7)                             /* !< CLRDT Offset */
#define USB_RXCSRL_CLRDT_MASK                    ((uint8_t)0x00000080U)          /* !< Clear Data Toggle. The CPU writes a
                                                                                    1 to this bit to reset the endpoint
                                                                                    data toggle to 0. Note: Device and
                                                                                    host mode. */
#define USB_RXCSRL_CLRDT_NO_EFFECT               ((uint8_t)0x00000000U)          /* !< No effect */
#define USB_RXCSRL_CLRDT_EFFECT                  ((uint8_t)0x00000080U)          /* !< Writing a 1 to this bit clears the
                                                                                    DT bit in the USBRXCSRH[n] register. */

/* USB_RXCSRH Bits */
/* USB_RXCSRH[INCOMPRX] Bits */
#define USB_RXCSRH_INCOMPRX_OFS                  (0)                             /* !< INCOMPRX Offset */
#define USB_RXCSRH_INCOMPRX_MASK                 ((uint8_t)0x00000001U)          /* !< This bit will be set in a
                                                                                    high-bandwidth Isochronous or
                                                                                    Interrupt transfer if the packet
                                                                                    received is incomplete, because parts
                                                                                    of the data were not received. It is
                                                                                    cleared when RXRDY is cleared. Note:
                                                                                    In anything other than Isochronous
                                                                                    transfer, this bit will always return
                                                                                    0. Note: Device and host mode. */
/* USB_RXCSRH[DT] Bits */
#define USB_RXCSRH_DT_OFS                        (1)                             /* !< DT Offset */
#define USB_RXCSRH_DT_MASK                       ((uint8_t)0x00000002U)          /* !< Data Toggle. When read, this bit
                                                                                    indicates the current state of the
                                                                                    receive data toggle. If DTWE is High,
                                                                                    this bit may be written with the
                                                                                    required setting of the data toggle.
                                                                                    If DTWE is Low, any value written to
                                                                                    this bit is ignored. Care should be
                                                                                    taken when writing to this bit as it
                                                                                    should only be changed to RESET the
                                                                                    receive endpoint. Note: Host mode
                                                                                    only. */
#define USB_RXCSRH_DT_LOW                        ((uint8_t)0x00000000U)          /* !< Data toggle is low */
#define USB_RXCSRH_DT_HIGH                       ((uint8_t)0x00000002U)          /* !< Data toggle is high */
/* USB_RXCSRH[DTWE] Bits */
#define USB_RXCSRH_DTWE_OFS                      (2)                             /* !< DTWE Offset */
#define USB_RXCSRH_DTWE_MASK                     ((uint8_t)0x00000004U)          /* !< Data Toggle Write Enable. The CPU
                                                                                    writes a 1 to this bit to enable the
                                                                                    current state of the Endpoint 0 data
                                                                                    toggle to be written. This bit is
                                                                                    automatically cleared once the new
                                                                                    value is written. Note: Host mode
                                                                                    only. */
#define USB_RXCSRH_DTWE_DISABLE                  ((uint8_t)0x00000000U)          /* !< The DT bit cannot be written. */
#define USB_RXCSRH_DTWE_ENABLE                   ((uint8_t)0x00000004U)          /* !< Enables the current state of the
                                                                                    receive endpoint data to be written
                                                                                    (see DT bit). */
/* USB_RXCSRH[DMAMOD] Bits */
#define USB_RXCSRH_DMAMOD_OFS                    (3)                             /* !< DMAMOD Offset */
#define USB_RXCSRH_DMAMOD_MASK                   ((uint8_t)0x00000008U)          /* !< DMA request mode The CPU sets this
                                                                                    bit to select DMA Request Mode 1 and
                                                                                    clears it to select DMA Request Mode
                                                                                    0. Note: This bit must not be cleared
                                                                                    either before or in the same cycle as
                                                                                    the above DMAEN bit is cleared. Note:
                                                                                    Device and host mode. */
#define USB_RXCSRH_DMAMOD_MODE0                  ((uint8_t)0x00000000U)          /* !< An interrupt is generated after
                                                                                    every DMA packet transfer. */
#define USB_RXCSRH_DMAMOD_MODE1                  ((uint8_t)0x00000008U)          /* !< An interrupt is generated only
                                                                                    after the entire DMA transfer is
                                                                                    complete. */
/* USB_RXCSRH[PIDERR] Bits */
#define USB_RXCSRH_PIDERR_OFS                    (4)                             /* !< PIDERR Offset */
#define USB_RXCSRH_PIDERR_MASK                   ((uint8_t)0x00000010U)          /* !< PID error ISO Transactions: The USB
                                                                                    controller sets this bit to indicate
                                                                                    a PID error in the received packet.
                                                                                    Bulk/Interrupt Transactions: The
                                                                                    setting of this bit is ignored. Note:
                                                                                    Device and host mode. */
#define USB_RXCSRH_PIDERR_NO_ERROR               ((uint8_t)0x00000000U)          /* !< No error */
#define USB_RXCSRH_PIDERR_ERROR                  ((uint8_t)0x00000010U)          /* !< Indicates a PID error in the
                                                                                    received packet of an isochronous
                                                                                    transaction. */
/* USB_RXCSRH[DMAEN] Bits */
#define USB_RXCSRH_DMAEN_OFS                     (5)                             /* !< DMAEN Offset */
#define USB_RXCSRH_DMAEN_MASK                    ((uint8_t)0x00000020U)          /* !< DMA Request Enable The CPU sets
                                                                                    this bit to enable the DMA request
                                                                                    for the Rx endpoint. Note: Four TX
                                                                                    and four RX endpoints can be
                                                                                    connected to the DMA module. If this
                                                                                    bit is set for a particular endpoint,
                                                                                    the TRIGARX, TRIGBRX, TRIGCRX, or
                                                                                    TRIGDRX field in the USB DMA Select
                                                                                    (USBDMASEL) register must be
                                                                                    programmed correspondingly */
#define USB_RXCSRH_DMAEN_DISABLE                 ((uint8_t)0x00000000U)          /* !< Disables the DMA request for the
                                                                                    receive endpoint. */
#define USB_RXCSRH_DMAEN_ENABLE                  ((uint8_t)0x00000020U)          /* !< Enables the DMA request for the
                                                                                    receive endpoint. */
/* USB_RXCSRH[ISOAUTORQ] Bits */
#define USB_RXCSRH_ISOAUTORQ_OFS                 (6)                             /* !< ISOAUTORQ Offset */
#define USB_RXCSRH_ISOAUTORQ_MASK                ((uint8_t)0x00000040U)          /* !< Isochronous transfer mode / Auto
                                                                                    request. Device mode: The CPU sets
                                                                                    this bit to enable the Rx endpoint
                                                                                    for Isochronous transfers, and clears
                                                                                    it to enable the Rx endpoint for
                                                                                    Bulk/Interrupt transfers. Host mode:
                                                                                    If the CPU sets this bit, the
                                                                                    STALLREQPKT bit will be automatically
                                                                                    set when the RXRDY bit is cleared. -
                                                                                    Note: This bit is automatically
                                                                                    cleared when a short packet is
                                                                                    received. */
#define USB_RXCSRH_ISOAUTORQ_DISABLE             ((uint8_t)0x00000000U)          /* !< No effect */
#define USB_RXCSRH_ISOAUTORQ_ENABLE              ((uint8_t)0x00000040U)          /* !< Sets ISO transfer mode or enables
                                                                                    the REQPKT bit to be automatically
                                                                                    set when the RXRDY bit is cleared. */
/* USB_RXCSRH[AUTOCLR] Bits */
#define USB_RXCSRH_AUTOCLR_OFS                   (7)                             /* !< AUTOCLR Offset */
#define USB_RXCSRH_AUTOCLR_MASK                  ((uint8_t)0x00000080U)          /* !< Auto Clear. If the CPU sets this
                                                                                    bit then the RXRDY bit will be
                                                                                    automatically cleared when a packet
                                                                                    of USBRXMAXP bytes has been unloaded
                                                                                    from the RX FIFO. When packets of
                                                                                    less than the maximum packet size are
                                                                                    unloaded, RXRDY will have to be
                                                                                    cleared manually. When using a DMA to
                                                                                    unload the RX-FIFO, data is read from
                                                                                    the RX-FIFO in 4 byte chunks
                                                                                    regardless of the USBRXMAXP setting.
                                                                                    Note: Device and host mode. */
#define USB_RXCSRH_AUTOCLR_DSIABLE               ((uint8_t)0x00000000U)          /* !< No effect */
#define USB_RXCSRH_AUTOCLR_ENABLE                ((uint8_t)0x00000080U)          /* !< Enables the RXRDY bit to be
                                                                                    automatically cleared when a packet
                                                                                    of USBRXMAXP[n] bytes has been
                                                                                    unloaded from the receive FIFO. */

/* USB_RXCOUNT Bits */
/* USB_RXCOUNT[COUNT] Bits */
#define USB_RXCOUNT_COUNT_OFS                    (0)                             /* !< COUNT Offset */
#define USB_RXCOUNT_COUNT_MASK                   ((uint16_t)0x00001FFFU)         /* !< Receive Packet Count indicates the
                                                                                    number of bytes in the receive
                                                                                    packet. */

/* USB_TXTYPE Bits */
/* USB_TXTYPE[TEP] Bits */
#define USB_TXTYPE_TEP_OFS                       (0)                             /* !< TEP Offset */
#define USB_TXTYPE_TEP_MASK                      ((uint8_t)0x0000000FU)          /* !< Target Endpoint Number. Software
                                                                                    must configure this value to the
                                                                                    endpoint number contained in the
                                                                                    transmit endpoint descriptor returned
                                                                                    to the USB controller during Device
                                                                                    enumeration. */
/* USB_TXTYPE[PROTO] Bits */
#define USB_TXTYPE_PROTO_OFS                     (4)                             /* !< PROTO Offset */
#define USB_TXTYPE_PROTO_MASK                    ((uint8_t)0x00000030U)          /* !< Protocol. Software must configure
                                                                                    this bit field to select the required
                                                                                    protocol for the transmit endpoint: */
#define USB_TXTYPE_PROTO_CTRL                    ((uint8_t)0x00000000U)          /* !< Control */
#define USB_TXTYPE_PROTO_ISO                     ((uint8_t)0x00000010U)          /* !< isochronous */
#define USB_TXTYPE_PROTO_BULK                    ((uint8_t)0x00000020U)          /* !< Bulk */
#define USB_TXTYPE_PROTO_INT                     ((uint8_t)0x00000030U)          /* !< Interrupt */
/* USB_TXTYPE[SPEED] Bits */
#define USB_TXTYPE_SPEED_OFS                     (6)                             /* !< SPEED Offset */
#define USB_TXTYPE_SPEED_MASK                    ((uint8_t)0x000000C0U)          /* !< Operating Speed. This bit field
                                                                                    specifies the operating speed of the
                                                                                    target Device: */
#define USB_TXTYPE_SPEED_DEFAULT                 ((uint8_t)0x00000000U)          /* !< Default. The target is assumed to
                                                                                    be using the same connection speed as
                                                                                    the USB controller. */
#define USB_TXTYPE_SPEED_FULL                    ((uint8_t)0x00000080U)          /* !< Full speed */
#define USB_TXTYPE_SPEED_LOW                     ((uint8_t)0x000000C0U)          /* !< Low speed */

/* USB_TXINTERVAL Bits */
/* USB_TXINTERVAL[TXPOLLNAKLMT] Bits */
#define USB_TXINTERVAL_TXPOLLNAKLMT_OFS          (0)                             /* !< TXPOLLNAKLMT Offset */
#define USB_TXINTERVAL_TXPOLLNAKLMT_MASK         ((uint8_t)0x000000FFU)          /* !< TX Polling / NAK Limit The polling
                                                                                    interval for interrupt/isochronous
                                                                                    transfers; the NAK limit for bulk
                                                                                    transfers. */

/* USB_RXTYPE Bits */
/* USB_RXTYPE[TEP] Bits */
#define USB_RXTYPE_TEP_OFS                       (0)                             /* !< TEP Offset */
#define USB_RXTYPE_TEP_MASK                      ((uint8_t)0x0000000FU)          /* !< Target Endpoint Number. Software
                                                                                    must configure this value to the
                                                                                    endpoint number contained in the
                                                                                    transmit endpoint descriptor returned
                                                                                    to the USB controller during Device
                                                                                    enumeration. */
/* USB_RXTYPE[PROTO] Bits */
#define USB_RXTYPE_PROTO_OFS                     (4)                             /* !< PROTO Offset */
#define USB_RXTYPE_PROTO_MASK                    ((uint8_t)0x00000030U)          /* !< Protocol. Software must configure
                                                                                    this bit field to select the required
                                                                                    protocol for the transmit endpoint: */
#define USB_RXTYPE_PROTO_CTRL                    ((uint8_t)0x00000000U)          /* !< Control */
#define USB_RXTYPE_PROTO_ISO                     ((uint8_t)0x00000010U)          /* !< isochronous */
#define USB_RXTYPE_PROTO_BULK                    ((uint8_t)0x00000020U)          /* !< Bulk */
#define USB_RXTYPE_PROTO_INT                     ((uint8_t)0x00000030U)          /* !< Interrupt */
/* USB_RXTYPE[SPEED] Bits */
#define USB_RXTYPE_SPEED_OFS                     (6)                             /* !< SPEED Offset */
#define USB_RXTYPE_SPEED_MASK                    ((uint8_t)0x000000C0U)          /* !< Operating Speed.  #br#This bit
                                                                                    field specifies the operating speed
                                                                                    of the target Device: */
#define USB_RXTYPE_SPEED_DEFAULT                 ((uint8_t)0x00000000U)          /* !< Default. The target is assumed to
                                                                                    be using the same connection speed as
                                                                                    the USB controller. */
#define USB_RXTYPE_SPEED_FULL                    ((uint8_t)0x00000080U)          /* !< Full speed */
#define USB_RXTYPE_SPEED_LOW                     ((uint8_t)0x000000C0U)          /* !< Low speed */

/* USB_RXINTERVAL Bits */
/* USB_RXINTERVAL[RXPOLLNAKLMT] Bits */
#define USB_RXINTERVAL_RXPOLLNAKLMT_OFS          (0)                             /* !< RXPOLLNAKLMT Offset */
#define USB_RXINTERVAL_RXPOLLNAKLMT_MASK         ((uint8_t)0x000000FFU)          /* !< RX Polling / NAK Limit The polling
                                                                                    interval for interrupt/isochronous
                                                                                    transfers; the NAK limit for bulk
                                                                                    transfers. */

/* USB_FIFOSZ Bits */
/* USB_FIFOSZ[TXSIZE] Bits */
#define USB_FIFOSZ_TXSIZE_OFS                    (0)                             /* !< TXSIZE Offset */
#define USB_FIFOSZ_TXSIZE_MASK                   ((uint8_t)0x0000000FU)          /* !< Returns the configured size of the
                                                                                    TX FIFO */
#define USB_FIFOSZ_TXSIZE_BYTES_8                ((uint8_t)0x00000000U)          /* !< FIFO size 8-byte */
#define USB_FIFOSZ_TXSIZE_BYTES_16               ((uint8_t)0x00000001U)          /* !< FIFO size 16-byte */
#define USB_FIFOSZ_TXSIZE_BYTES_32               ((uint8_t)0x00000002U)          /* !< FIFO size 32-byte */
#define USB_FIFOSZ_TXSIZE_BYTES_64               ((uint8_t)0x00000003U)          /* !< FIFO size 64-byte */
#define USB_FIFOSZ_TXSIZE_BYTES_128              ((uint8_t)0x00000004U)          /* !< FIFO size 128-byte */
#define USB_FIFOSZ_TXSIZE_BYTES_256              ((uint8_t)0x00000005U)          /* !< FIFO size 256-byte */
#define USB_FIFOSZ_TXSIZE_BYTES_512              ((uint8_t)0x00000006U)          /* !< FIFO size 512-byte */
#define USB_FIFOSZ_TXSIZE_BYTES_1024             ((uint8_t)0x00000007U)          /* !< FIFO size 1024-byte */
#define USB_FIFOSZ_TXSIZE_BYTES_2048             ((uint8_t)0x00000008U)          /* !< FIFO size 2048-byte */
/* USB_FIFOSZ[RXSIZE] Bits */
#define USB_FIFOSZ_RXSIZE_OFS                    (4)                             /* !< RXSIZE Offset */
#define USB_FIFOSZ_RXSIZE_MASK                   ((uint8_t)0x000000F0U)          /* !< Returns the configured size of the
                                                                                    RX FIFO */
#define USB_FIFOSZ_RXSIZE_BYTES_8                ((uint8_t)0x00000000U)          /* !< FIFO size 8-byte */
#define USB_FIFOSZ_RXSIZE_BYTES_16               ((uint8_t)0x00000010U)          /* !< FIFO size 16-byte */
#define USB_FIFOSZ_RXSIZE_BYTES_32               ((uint8_t)0x00000020U)          /* !< FIFO size 32-byte */
#define USB_FIFOSZ_RXSIZE_BYTES_64               ((uint8_t)0x00000030U)          /* !< FIFO size 64-byte */
#define USB_FIFOSZ_RXSIZE_BYTES_128              ((uint8_t)0x00000040U)          /* !< FIFO size 128-byte */
#define USB_FIFOSZ_RXSIZE_BYTES_256              ((uint8_t)0x00000050U)          /* !< FIFO size 256-byte */
#define USB_FIFOSZ_RXSIZE_BYTES_512              ((uint8_t)0x00000060U)          /* !< FIFO size 512-byte */
#define USB_FIFOSZ_RXSIZE_BYTES_1024             ((uint8_t)0x00000070U)          /* !< FIFO size 1024-byte */
#define USB_FIFOSZ_RXSIZE_BYTES_2048             ((uint8_t)0x00000080U)          /* !< FIFO size 2048-byte */

/* USB_TXFUNCADDR Bits */
/* USB_TXFUNCADDR[ADDR] Bits */
#define USB_TXFUNCADDR_ADDR_OFS                  (0)                             /* !< ADDR Offset */
#define USB_TXFUNCADDR_ADDR_MASK                 ((uint8_t)0x0000007FU)          /* !< Device Address specifies the USB
                                                                                    bus address for the target Device. */

/* USB_TXHUBADDR Bits */
/* USB_TXHUBADDR[ADDR] Bits */
#define USB_TXHUBADDR_ADDR_OFS                   (0)                             /* !< ADDR Offset */
#define USB_TXHUBADDR_ADDR_MASK                  ((uint8_t)0x0000007FU)          /* !< Device Address specifies the USB
                                                                                    bus address for the target Device. */
/* USB_TXHUBADDR[MULTTRAN] Bits */
#define USB_TXHUBADDR_MULTTRAN_OFS               (7)                             /* !< MULTTRAN Offset */
#define USB_TXHUBADDR_MULTTRAN_MASK              ((uint8_t)0x00000080U)          /* !< Record whether the hub has multiple
                                                                                    transaction translators. */
#define USB_TXHUBADDR_MULTTRAN_CLEAR             ((uint8_t)0x00000000U)          /* !< Clear to indicate that the hub has
                                                                                    a single transaction translator. */
#define USB_TXHUBADDR_MULTTRAN_SET               ((uint8_t)0x00000080U)          /* !< Set to indicate that the hub has
                                                                                    multiple transaction translators. */

/* USB_TXHUBPORT Bits */
/* USB_TXHUBPORT[ADDR] Bits */
#define USB_TXHUBPORT_ADDR_OFS                   (0)                             /* !< ADDR Offset */
#define USB_TXHUBPORT_ADDR_MASK                  ((uint8_t)0x0000007FU)          /* !< Hub Port specifies the USB hub port
                                                                                    number. */

/* USB_RXFUNCADDR Bits */
/* USB_RXFUNCADDR[ADDR] Bits */
#define USB_RXFUNCADDR_ADDR_OFS                  (0)                             /* !< ADDR Offset */
#define USB_RXFUNCADDR_ADDR_MASK                 ((uint8_t)0x0000007FU)          /* !< Device Address specifies the USB
                                                                                    bus address for the target Device. */

/* USB_RXHUBADDR Bits */
/* USB_RXHUBADDR[ADDR] Bits */
#define USB_RXHUBADDR_ADDR_OFS                   (0)                             /* !< ADDR Offset */
#define USB_RXHUBADDR_ADDR_MASK                  ((uint8_t)0x0000007FU)          /* !< Hub Address */
/* USB_RXHUBADDR[MULTTRAN] Bits */
#define USB_RXHUBADDR_MULTTRAN_OFS               (7)                             /* !< MULTTRAN Offset */
#define USB_RXHUBADDR_MULTTRAN_MASK              ((uint8_t)0x00000080U)          /* !< Record whether the hub has multiple
                                                                                    transaction translators. */
#define USB_RXHUBADDR_MULTTRAN_CLEAR             ((uint8_t)0x00000000U)          /* !< Clear to indicate that the hub has
                                                                                    a single transaction translator. */
#define USB_RXHUBADDR_MULTTRAN_SET               ((uint8_t)0x00000080U)          /* !< Set to indicate that the hub has
                                                                                    multiple transaction translators. */

/* USB_RXHUBPORT Bits */
/* USB_RXHUBPORT[ADDR] Bits */
#define USB_RXHUBPORT_ADDR_OFS                   (0)                             /* !< ADDR Offset */
#define USB_RXHUBPORT_ADDR_MASK                  ((uint8_t)0x0000007FU)          /* !< Hub Address */

/* USB_FADDR Bits */
/* USB_FADDR[FUNCADDR] Bits */
#define USB_FADDR_FUNCADDR_OFS                   (0)                             /* !< FUNCADDR Offset */
#define USB_FADDR_FUNCADDR_MASK                  ((uint8_t)0x0000007FU)          /* !< FAddr is an 8-bit register that
                                                                                    should be written with the 7-bit
                                                                                    address of the peripheral part of the
                                                                                    transaction. When the USB module is
                                                                                    being used in Peripheral mode
                                                                                    (DevCtl.D2=0), this register should
                                                                                    be written with the address received
                                                                                    through a SET_ADDRESS command, which
                                                                                    will then be used for decoding the
                                                                                    function address in subsequent token
                                                                                    packets. Notes: Peripheral Mode
                                                                                    Only!! */

/* USB_POWER Bits */
/* USB_POWER[ENSUSPENDM] Bits */
#define USB_POWER_ENSUSPENDM_OFS                 (0)                             /* !< ENSUSPENDM Offset */
#define USB_POWER_ENSUSPENDM_MASK                ((uint8_t)0x00000001U)          /* !< Set by the CPU to enable the
                                                                                    Suspend mode for the USB PHY. */
#define USB_POWER_ENSUSPENDM_DISABLE             ((uint8_t)0x00000000U)          /* !< USB PHY is powered */
#define USB_POWER_ENSUSPENDM_ENABLE              ((uint8_t)0x00000001U)          /* !< Powers down the internal USB PHY. */
/* USB_POWER[SUSPEND] Bits */
#define USB_POWER_SUSPEND_OFS                    (1)                             /* !< SUSPEND Offset */
#define USB_POWER_SUSPEND_MASK                   ((uint8_t)0x00000002U)          /* !< In Host mode, this bit is set by
                                                                                    the CPU to enter Suspend mode. In
                                                                                    Peripheral mode, this bit is set on
                                                                                    entry into Suspend mode. It is
                                                                                    cleared when the CPU reads the
                                                                                    interrupt register, or sets the
                                                                                    Resume bit. */
#define USB_POWER_SUSPEND_DISABLE                ((uint8_t)0x00000000U)          /* !< Ends SUSPEND signaling */
#define USB_POWER_SUSPEND_ENABLE                 ((uint8_t)0x00000002U)          /* !< Enables SUSPEND signaling */
/* USB_POWER[RESUME] Bits */
#define USB_POWER_RESUME_OFS                     (2)                             /* !< RESUME Offset */
#define USB_POWER_RESUME_MASK                    ((uint8_t)0x00000004U)          /* !< Set by the CPU to generate Resume
                                                                                    signaling when the device is in
                                                                                    Suspend mode.  In Peripheral mode,
                                                                                    the CPU should clear this bit after
                                                                                    10 ms (a maximum of 15 ms), to end
                                                                                    Resume signaling.  In Host mode, the
                                                                                    CPU should clear this bit after 20
                                                                                    ms. */
#define USB_POWER_RESUME_DISABLE                 ((uint8_t)0x00000000U)          /* !< Ends RESUME signaling on the bus. */
#define USB_POWER_RESUME_ENABLE                  ((uint8_t)0x00000004U)          /* !< Enables RESUME signaling when the
                                                                                    Device is in SUSPEND mode. */
/* USB_POWER[RESET] Bits */
#define USB_POWER_RESET_OFS                      (3)                             /* !< RESET Offset */
#define USB_POWER_RESET_MASK                     ((uint8_t)0x00000008U)          /* !< This bit is set when Reset
                                                                                    signaling is present on the bus.
                                                                                    Note: This bit is Read/Write from the
                                                                                    CPU in Host Mode but Read-Only in
                                                                                    Peripheral Mode. */
#define USB_POWER_RESET_DISABLE                  ((uint8_t)0x00000000U)          /* !< Ends RESET signaling on the bus. */
#define USB_POWER_RESET_ENABLE                   ((uint8_t)0x00000008U)          /* !< Enables RESET signaling on the bus. */
/* USB_POWER[SOFT_CONN] Bits */
#define USB_POWER_SOFT_CONN_OFS                  (6)                             /* !< SOFT_CONN Offset */
#define USB_POWER_SOFT_CONN_MASK                 ((uint8_t)0x00000040U)          /* !< Soft Connect/Disconnect feature.
                                                                                    The USB D+/D- lines are enabled when
                                                                                    this bit is set by the CPU and
                                                                                    tri-stated when this bit is cleared
                                                                                    by the CPU.  Note: Only valid in
                                                                                    Peripheral Mode. */
#define USB_POWER_SOFT_CONN_TRISTATE             ((uint8_t)0x00000000U)          /* !< The USB D+/D- lines are tri-stated. */
#define USB_POWER_SOFT_CONN_ENABLE               ((uint8_t)0x00000040U)          /* !< The USB D+/D- lines are enabled. */
/* USB_POWER[ISOUP] Bits */
#define USB_POWER_ISOUP_OFS                      (7)                             /* !< ISOUP Offset */
#define USB_POWER_ISOUP_MASK                     ((uint8_t)0x00000080U)          /* !< Isochronous Update. When set by the
                                                                                    CPU, the USB module will wait for an
                                                                                    SOF token from the time TXRDY bit is
                                                                                    set in the USBTXCSRLn before sending
                                                                                    the packet. If an IN token is
                                                                                    received before an SOF token, then a
                                                                                    zero length data packet will be sent.
                                                                                    Note: Only valid in Peripheral Mode.
                                                                                    Also, this bit only affects endpoints
                                                                                    performing Isochronous transfers. */
#define USB_POWER_ISOUP_NO_EFFECT                ((uint8_t)0x00000000U)          /* !< No effect */
#define USB_POWER_ISOUP_WAIT_SOF                 ((uint8_t)0x00000080U)          /* !< The USB controller waits for an SOF
                                                                                    token. */

/* USB_TXIS Bits */
/* USB_TXIS[EP0] Bits */
#define USB_TXIS_EP0_OFS                         (0)                             /* !< EP0 Offset */
#define USB_TXIS_EP0_MASK                        ((uint16_t)0x00000001U)         /* !< Transmit Endpoint 0 Interrupt */
#define USB_TXIS_EP0_NO_INT                      ((uint16_t)0x00000000U)         /* !< No interrupt */
#define USB_TXIS_EP0_INT                         ((uint16_t)0x00000001U)         /* !< The Endpoint 0 transmit and receive
                                                                                    interrupt is asserted. */
/* USB_TXIS[EP1] Bits */
#define USB_TXIS_EP1_OFS                         (1)                             /* !< EP1 Offset */
#define USB_TXIS_EP1_MASK                        ((uint16_t)0x00000002U)         /* !< Transmit Endpoint 1 Interrupt */
#define USB_TXIS_EP1_NO_INT                      ((uint16_t)0x00000000U)         /* !< No interrupt */
#define USB_TXIS_EP1_INT                         ((uint16_t)0x00000002U)         /* !< The Endpoint 1 transmit interrupt
                                                                                    is asserted. */
/* USB_TXIS[EP2] Bits */
#define USB_TXIS_EP2_OFS                         (2)                             /* !< EP2 Offset */
#define USB_TXIS_EP2_MASK                        ((uint16_t)0x00000004U)         /* !< Transmit Endpoint 2 Interrupt */
#define USB_TXIS_EP2_NO_INT                      ((uint16_t)0x00000000U)         /* !< No interrupt */
#define USB_TXIS_EP2_INT                         ((uint16_t)0x00000004U)         /* !< The Endpoint 2 transmit interrupt
                                                                                    is asserted. */
/* USB_TXIS[EP3] Bits */
#define USB_TXIS_EP3_OFS                         (3)                             /* !< EP3 Offset */
#define USB_TXIS_EP3_MASK                        ((uint16_t)0x00000008U)         /* !< Transmit Endpoint 3 Interrupt */
#define USB_TXIS_EP3_NO_INT                      ((uint16_t)0x00000000U)         /* !< No interrupt */
#define USB_TXIS_EP3_INT                         ((uint16_t)0x00000008U)         /* !< The Endpoint 3 transmit interrupt
                                                                                    is asserted. */
/* USB_TXIS[EP4] Bits */
#define USB_TXIS_EP4_OFS                         (4)                             /* !< EP4 Offset */
#define USB_TXIS_EP4_MASK                        ((uint16_t)0x00000010U)         /* !< Transmit Endpoint 4 Interrupt */
#define USB_TXIS_EP4_NO_INT                      ((uint16_t)0x00000000U)         /* !< No interrupt */
#define USB_TXIS_EP4_INT                         ((uint16_t)0x00000010U)         /* !< The Endpoint 4 transmit interrupt
                                                                                    is asserted. */
/* USB_TXIS[EP5] Bits */
#define USB_TXIS_EP5_OFS                         (5)                             /* !< EP5 Offset */
#define USB_TXIS_EP5_MASK                        ((uint16_t)0x00000020U)         /* !< Transmit Endpoint 5 Interrupt */
#define USB_TXIS_EP5_NO_INT                      ((uint16_t)0x00000000U)         /* !< No interrupt */
#define USB_TXIS_EP5_INT                         ((uint16_t)0x00000020U)         /* !< The Endpoint 5 transmit interrupt
                                                                                    is asserted. */
/* USB_TXIS[EP6] Bits */
#define USB_TXIS_EP6_OFS                         (6)                             /* !< EP6 Offset */
#define USB_TXIS_EP6_MASK                        ((uint16_t)0x00000040U)         /* !< Transmit Endpoint 6 Interrupt */
#define USB_TXIS_EP6_NO_INT                      ((uint16_t)0x00000000U)         /* !< No interrupt */
#define USB_TXIS_EP6_INT                         ((uint16_t)0x00000040U)         /* !< The Endpoint 6 transmit interrupt
                                                                                    is asserted. */
/* USB_TXIS[EP7] Bits */
#define USB_TXIS_EP7_OFS                         (7)                             /* !< EP7 Offset */
#define USB_TXIS_EP7_MASK                        ((uint16_t)0x00000080U)         /* !< Transmit Endpoint 7 Interrupt */
#define USB_TXIS_EP7_NO_INT                      ((uint16_t)0x00000000U)         /* !< No interrupt */
#define USB_TXIS_EP7_INT                         ((uint16_t)0x00000080U)         /* !< The Endpoint 7 transmit interrupt
                                                                                    is asserted. */

/* USB_RXIS Bits */
/* USB_RXIS[EP1] Bits */
#define USB_RXIS_EP1_OFS                         (1)                             /* !< EP1 Offset */
#define USB_RXIS_EP1_MASK                        ((uint16_t)0x00000002U)         /* !< Recieve Endpoint 1 Interrupt */
#define USB_RXIS_EP1_NO_INT                      ((uint16_t)0x00000000U)         /* !< No interrupt */
#define USB_RXIS_EP1_INT                         ((uint16_t)0x00000002U)         /* !< The Endpoint 1 transmit interrupt
                                                                                    is asserted. */
/* USB_RXIS[EP2] Bits */
#define USB_RXIS_EP2_OFS                         (2)                             /* !< EP2 Offset */
#define USB_RXIS_EP2_MASK                        ((uint16_t)0x00000004U)         /* !< Recieve Endpoint 2 Interrupt */
#define USB_RXIS_EP2_NO_INT                      ((uint16_t)0x00000000U)         /* !< No interrupt */
#define USB_RXIS_EP2_INT                         ((uint16_t)0x00000004U)         /* !< The Endpoint 2 transmit interrupt
                                                                                    is asserted. */
/* USB_RXIS[EP3] Bits */
#define USB_RXIS_EP3_OFS                         (3)                             /* !< EP3 Offset */
#define USB_RXIS_EP3_MASK                        ((uint16_t)0x00000008U)         /* !< Recieve Endpoint 3 Interrupt */
#define USB_RXIS_EP3_NO_INT                      ((uint16_t)0x00000000U)         /* !< No interrupt */
#define USB_RXIS_EP3_INT                         ((uint16_t)0x00000008U)         /* !< The Endpoint 3 transmit interrupt
                                                                                    is asserted. */
/* USB_RXIS[EP4] Bits */
#define USB_RXIS_EP4_OFS                         (4)                             /* !< EP4 Offset */
#define USB_RXIS_EP4_MASK                        ((uint16_t)0x00000010U)         /* !< Recieve Endpoint 4 Interrupt */
#define USB_RXIS_EP4_NO_INT                      ((uint16_t)0x00000000U)         /* !< No interrupt */
#define USB_RXIS_EP4_INT                         ((uint16_t)0x00000010U)         /* !< The Endpoint 4 transmit interrupt
                                                                                    is asserted. */
/* USB_RXIS[EP5] Bits */
#define USB_RXIS_EP5_OFS                         (5)                             /* !< EP5 Offset */
#define USB_RXIS_EP5_MASK                        ((uint16_t)0x00000020U)         /* !< Recieve Endpoint 5 Interrupt */
#define USB_RXIS_EP5_NO_INT                      ((uint16_t)0x00000000U)         /* !< No interrupt */
#define USB_RXIS_EP5_INT                         ((uint16_t)0x00000020U)         /* !< The Endpoint 5 transmit interrupt
                                                                                    is asserted. */
/* USB_RXIS[EP6] Bits */
#define USB_RXIS_EP6_OFS                         (6)                             /* !< EP6 Offset */
#define USB_RXIS_EP6_MASK                        ((uint16_t)0x00000040U)         /* !< Recieve Endpoint 6 Interrupt */
#define USB_RXIS_EP6_NO_INT                      ((uint16_t)0x00000000U)         /* !< No interrupt */
#define USB_RXIS_EP6_INT                         ((uint16_t)0x00000040U)         /* !< The Endpoint 6 transmit interrupt
                                                                                    is asserted. */
/* USB_RXIS[EP7] Bits */
#define USB_RXIS_EP7_OFS                         (7)                             /* !< EP7 Offset */
#define USB_RXIS_EP7_MASK                        ((uint16_t)0x00000080U)         /* !< Recieve Endpoint 7 Interrupt */
#define USB_RXIS_EP7_NO_INT                      ((uint16_t)0x00000000U)         /* !< No interrupt */
#define USB_RXIS_EP7_INT                         ((uint16_t)0x00000080U)         /* !< The Endpoint 7 transmit interrupt
                                                                                    is asserted. */

/* USB_TXIE Bits */
/* USB_TXIE[EP0] Bits */
#define USB_TXIE_EP0_OFS                         (0)                             /* !< EP0 Offset */
#define USB_TXIE_EP0_MASK                        ((uint16_t)0x00000001U)         /* !< Transmit Endpoint 0 Interrupt
                                                                                    Enable */
#define USB_TXIE_EP0_DISABLE                     ((uint16_t)0x00000000U)         /* !< The EP0 transmit and receive
                                                                                    interrupt is suppressed and not sent
                                                                                    to the interupt controller. */
#define USB_TXIE_EP0_ENABLE                      ((uint16_t)0x00000001U)         /* !< An interrupt is sent to the
                                                                                    interrupt controller when the EP0 bit
                                                                                    in the USBTXIS register is set. */
/* USB_TXIE[EP1] Bits */
#define USB_TXIE_EP1_OFS                         (1)                             /* !< EP1 Offset */
#define USB_TXIE_EP1_MASK                        ((uint16_t)0x00000002U)         /* !< Transmit Endpoint 1 Interrupt
                                                                                    Enable */
#define USB_TXIE_EP1_DISABLE                     ((uint16_t)0x00000000U)         /* !< The EP1 transmit interrupt is
                                                                                    suppressed and not sent to the
                                                                                    interrupt controller. */
#define USB_TXIE_EP1_ENABLE                      ((uint16_t)0x00000002U)         /* !< An interrupt is sent to the
                                                                                    interrupt controller when the EP1 bit
                                                                                    in the USBTXIS register is set. */
/* USB_TXIE[EP2] Bits */
#define USB_TXIE_EP2_OFS                         (2)                             /* !< EP2 Offset */
#define USB_TXIE_EP2_MASK                        ((uint16_t)0x00000004U)         /* !< Transmit Endpoint 2 Interrupt
                                                                                    Enable */
#define USB_TXIE_EP2_DISABLE                     ((uint16_t)0x00000000U)         /* !< The EP2 transmit interrupt is
                                                                                    suppressed and not sent to the
                                                                                    interrupt controller. */
#define USB_TXIE_EP2_ENABLE                      ((uint16_t)0x00000004U)         /* !< An interrupt is sent to the
                                                                                    interrupt controller when the EP2 bit
                                                                                    in the USBTXIS register is set. */
/* USB_TXIE[EP3] Bits */
#define USB_TXIE_EP3_OFS                         (3)                             /* !< EP3 Offset */
#define USB_TXIE_EP3_MASK                        ((uint16_t)0x00000008U)         /* !< Transmit Endpoint 3 Interrupt
                                                                                    Enable */
#define USB_TXIE_EP3_DISABLE                     ((uint16_t)0x00000000U)         /* !< The EP3 transmit interrupt is
                                                                                    suppressed and not sent to the
                                                                                    interrupt controller. */
#define USB_TXIE_EP3_ENABLE                      ((uint16_t)0x00000008U)         /* !< An interrupt is sent to the
                                                                                    interrupt controller when the EP3 bit
                                                                                    in the USBTXIS register is set. */
/* USB_TXIE[EP4] Bits */
#define USB_TXIE_EP4_OFS                         (4)                             /* !< EP4 Offset */
#define USB_TXIE_EP4_MASK                        ((uint16_t)0x00000010U)         /* !< Transmit Endpoint 4 Interrupt
                                                                                    Enable */
#define USB_TXIE_EP4_DISABLE                     ((uint16_t)0x00000000U)         /* !< The EP4 transmit interrupt is
                                                                                    suppressed and not sent to the
                                                                                    interrupt controller. */
#define USB_TXIE_EP4_ENABLE                      ((uint16_t)0x00000010U)         /* !< An interrupt is sent to the
                                                                                    interrupt controller when the EP4 bit
                                                                                    in the USBTXIS register is set. */
/* USB_TXIE[EP5] Bits */
#define USB_TXIE_EP5_OFS                         (5)                             /* !< EP5 Offset */
#define USB_TXIE_EP5_MASK                        ((uint16_t)0x00000020U)         /* !< Transmit Endpoint 5 Interrupt
                                                                                    Enable */
#define USB_TXIE_EP5_DISABLE                     ((uint16_t)0x00000000U)         /* !< The EP5 transmit interrupt is
                                                                                    suppressed and not sent to the
                                                                                    interrupt controller. */
#define USB_TXIE_EP5_ENABLE                      ((uint16_t)0x00000020U)         /* !< An interrupt is sent to the
                                                                                    interrupt controller when the EP5 bit
                                                                                    in the USBTXIS register is set. */
/* USB_TXIE[EP6] Bits */
#define USB_TXIE_EP6_OFS                         (6)                             /* !< EP6 Offset */
#define USB_TXIE_EP6_MASK                        ((uint16_t)0x00000040U)         /* !< Transmit Endpoint 6 Interrupt
                                                                                    Enable */
#define USB_TXIE_EP6_DISABLE                     ((uint16_t)0x00000000U)         /* !< The EP6 transmit interrupt is
                                                                                    suppressed and not sent to the
                                                                                    interrupt controller. */
#define USB_TXIE_EP6_ENABLE                      ((uint16_t)0x00000040U)         /* !< An interrupt is sent to the
                                                                                    interrupt controller when the EP6 bit
                                                                                    in the USBTXIS register is set. */
/* USB_TXIE[EP7] Bits */
#define USB_TXIE_EP7_OFS                         (7)                             /* !< EP7 Offset */
#define USB_TXIE_EP7_MASK                        ((uint16_t)0x00000080U)         /* !< Transmit Endpoint 7 Interrupt
                                                                                    Enable */
#define USB_TXIE_EP7_DISABLE                     ((uint16_t)0x00000000U)         /* !< The EP7 transmit interrupt is
                                                                                    suppressed and not sent to the
                                                                                    interrupt controller. */
#define USB_TXIE_EP7_ENABLE                      ((uint16_t)0x00000080U)         /* !< An interrupt is sent to the
                                                                                    interrupt controller when the EP7 bit
                                                                                    in the USBTXIS register is set. */

/* USB_RXIE Bits */
/* USB_RXIE[EP1] Bits */
#define USB_RXIE_EP1_OFS                         (1)                             /* !< EP1 Offset */
#define USB_RXIE_EP1_MASK                        ((uint16_t)0x00000002U)         /* !< Recieve Endpoint 1 Interrupt Enable */
#define USB_RXIE_EP1_NO_INT                      ((uint16_t)0x00000000U)         /* !< No interrupt */
#define USB_RXIE_EP1_INT                         ((uint16_t)0x00000002U)         /* !< The Endpoint 1 transmit interrupt
                                                                                    is asserted. */
/* USB_RXIE[EP2] Bits */
#define USB_RXIE_EP2_OFS                         (2)                             /* !< EP2 Offset */
#define USB_RXIE_EP2_MASK                        ((uint16_t)0x00000004U)         /* !< Recieve Endpoint 2 Interrupt Enable */
#define USB_RXIE_EP2_NO_INT                      ((uint16_t)0x00000000U)         /* !< No interrupt */
#define USB_RXIE_EP2_INT                         ((uint16_t)0x00000004U)         /* !< The Endpoint 2 transmit interrupt
                                                                                    is asserted. */
/* USB_RXIE[EP3] Bits */
#define USB_RXIE_EP3_OFS                         (3)                             /* !< EP3 Offset */
#define USB_RXIE_EP3_MASK                        ((uint16_t)0x00000008U)         /* !< Recieve Endpoint 3 Interrupt Enable */
#define USB_RXIE_EP3_NO_INT                      ((uint16_t)0x00000000U)         /* !< No interrupt */
#define USB_RXIE_EP3_INT                         ((uint16_t)0x00000008U)         /* !< The Endpoint 3 transmit interrupt
                                                                                    is asserted. */
/* USB_RXIE[EP4] Bits */
#define USB_RXIE_EP4_OFS                         (4)                             /* !< EP4 Offset */
#define USB_RXIE_EP4_MASK                        ((uint16_t)0x00000010U)         /* !< Recieve Endpoint 4 Interrupt Enable */
#define USB_RXIE_EP4_NO_INT                      ((uint16_t)0x00000000U)         /* !< No interrupt */
#define USB_RXIE_EP4_INT                         ((uint16_t)0x00000010U)         /* !< The Endpoint 4 transmit interrupt
                                                                                    is asserted. */
/* USB_RXIE[EP5] Bits */
#define USB_RXIE_EP5_OFS                         (5)                             /* !< EP5 Offset */
#define USB_RXIE_EP5_MASK                        ((uint16_t)0x00000020U)         /* !< Recieve Endpoint 5 Interrupt Enable */
#define USB_RXIE_EP5_NO_INT                      ((uint16_t)0x00000000U)         /* !< No interrupt */
#define USB_RXIE_EP5_INT                         ((uint16_t)0x00000020U)         /* !< The Endpoint 5 transmit interrupt
                                                                                    is asserted. */
/* USB_RXIE[EP6] Bits */
#define USB_RXIE_EP6_OFS                         (6)                             /* !< EP6 Offset */
#define USB_RXIE_EP6_MASK                        ((uint16_t)0x00000040U)         /* !< Recieve Endpoint 6 Interrupt Enable */
#define USB_RXIE_EP6_NO_INT                      ((uint16_t)0x00000000U)         /* !< No interrupt */
#define USB_RXIE_EP6_INT                         ((uint16_t)0x00000040U)         /* !< The Endpoint 6 transmit interrupt
                                                                                    is asserted. */
/* USB_RXIE[EP7] Bits */
#define USB_RXIE_EP7_OFS                         (7)                             /* !< EP7 Offset */
#define USB_RXIE_EP7_MASK                        ((uint16_t)0x00000080U)         /* !< Recieve Endpoint 7 Interrupt Enable */
#define USB_RXIE_EP7_NO_INT                      ((uint16_t)0x00000000U)         /* !< No interrupt */
#define USB_RXIE_EP7_INT                         ((uint16_t)0x00000080U)         /* !< The Endpoint 7 transmit interrupt
                                                                                    is asserted. */

/* USB_USBIS Bits */
/* USB_USBIS[SUSPEND] Bits */
#define USB_USBIS_SUSPEND_OFS                    (0)                             /* !< SUSPEND Offset */
#define USB_USBIS_SUSPEND_MASK                   ((uint8_t)0x00000001U)          /* !< Set when Suspend signaling is
                                                                                    detected on the bus. Note: Only valid
                                                                                    in Peripheral mode. */
#define USB_USBIS_SUSPEND_NO_INT                 ((uint8_t)0x00000000U)          /* !< No interrupt */
#define USB_USBIS_SUSPEND_INT                    ((uint8_t)0x00000001U)          /* !< SUSPEND signaling has been detected
                                                                                    on the bus. */
/* USB_USBIS[RESUME] Bits */
#define USB_USBIS_RESUME_OFS                     (1)                             /* !< RESUME Offset */
#define USB_USBIS_RESUME_MASK                    ((uint8_t)0x00000002U)          /* !< Set when Resume signaling is
                                                                                    detected on the bus while the USB
                                                                                    controller is in Suspend mode. */
#define USB_USBIS_RESUME_NO_INT                  ((uint8_t)0x00000000U)          /* !< No interrupt */
#define USB_USBIS_RESUME_INT                     ((uint8_t)0x00000002U)          /* !< RESUME signaling has been detected
                                                                                    on the bus. */
/* USB_USBIS[RESETBABBLE] Bits */
#define USB_USBIS_RESETBABBLE_OFS                (2)                             /* !< RESETBABBLE Offset */
#define USB_USBIS_RESETBABBLE_MASK               ((uint8_t)0x00000004U)          /* !< Device mode - RESET: Set in
                                                                                    Peripheral mode when Reset signaling
                                                                                    is detected on the bus. Host mode -
                                                                                    BABBLE: Set in Host mode when babble
                                                                                    is detected. Note: Only active after
                                                                                    first SOF has been sent. */
#define USB_USBIS_RESETBABBLE_NO_INT             ((uint8_t)0x00000000U)          /* !< No interrupt */
#define USB_USBIS_RESETBABBLE_INT                ((uint8_t)0x00000004U)          /* !< RESET / BABBLE signaling has been
                                                                                    detected on the bus. */
/* USB_USBIS[SOF] Bits */
#define USB_USBIS_SOF_OFS                        (3)                             /* !< SOF Offset */
#define USB_USBIS_SOF_MASK                       ((uint8_t)0x00000008U)          /* !< Start of frame */
#define USB_USBIS_SOF_NO_INT                     ((uint8_t)0x00000000U)          /* !< No interrupt */
#define USB_USBIS_SOF_INT                        ((uint8_t)0x00000008U)          /* !< A new frame has started. */
/* USB_USBIS[CONN] Bits */
#define USB_USBIS_CONN_OFS                       (4)                             /* !< CONN Offset */
#define USB_USBIS_CONN_MASK                      ((uint8_t)0x00000010U)          /* !< Set when a device connection is
                                                                                    detected.  Note: Only valid in Host
                                                                                    mode. Valid at all transaction
                                                                                    speeds. */
#define USB_USBIS_CONN_NO_INT                    ((uint8_t)0x00000000U)          /* !< No interrupt */
#define USB_USBIS_CONN_INT                       ((uint8_t)0x00000010U)          /* !< The device has detected a
                                                                                    connection to the host. */
/* USB_USBIS[DISCON] Bits */
#define USB_USBIS_DISCON_OFS                     (5)                             /* !< DISCON Offset */
#define USB_USBIS_DISCON_MASK                    ((uint8_t)0x00000020U)          /* !< Set in Host mode when a device
                                                                                    disconnect is detected. Set in
                                                                                    Peripheral mode when a session ends.
                                                                                    Valid at all transaction speeds. */
#define USB_USBIS_DISCON_NO_INT                  ((uint8_t)0x00000000U)          /* !< No interrupt */
#define USB_USBIS_DISCON_INT                     ((uint8_t)0x00000020U)          /* !< The device has been disconnected
                                                                                    from the host. */

/* USB_USBIE Bits */
/* USB_USBIE[SUSPEND] Bits */
#define USB_USBIE_SUSPEND_OFS                    (0)                             /* !< SUSPEND Offset */
#define USB_USBIE_SUSPEND_MASK                   ((uint8_t)0x00000001U)          /* !< Interrupt enable for SUSPEND
                                                                                    signaling detected. Note: Only valid
                                                                                    in Peripheral mode. */
#define USB_USBIE_SUSPEND_DISABLE                ((uint8_t)0x00000000U)          /* !< The SUSPEND interrupt is suppressed
                                                                                    and not sent to the interrupt
                                                                                    controller. */
#define USB_USBIE_SUSPEND_ENABLE                 ((uint8_t)0x00000001U)          /* !< An interrupt is sent to the
                                                                                    interrupt controller when the DISCON
                                                                                    bit in the USBIS register is set. */
/* USB_USBIE[RESUME] Bits */
#define USB_USBIE_RESUME_OFS                     (1)                             /* !< RESUME Offset */
#define USB_USBIE_RESUME_MASK                    ((uint8_t)0x00000002U)          /* !< Interrupt enable for RESUME
                                                                                    signaling detected. */
#define USB_USBIE_RESUME_DISABLE                 ((uint8_t)0x00000000U)          /* !< The RESUME interrupt is suppressed
                                                                                    and not sent to the interrupt
                                                                                    controller. */
#define USB_USBIE_RESUME_ENABLE                  ((uint8_t)0x00000002U)          /* !< An interrupt is sent to the
                                                                                    interrupt controller when the RESUME
                                                                                    bit in the USBIS register is set. */
/* USB_USBIE[RESETBABBLE] Bits */
#define USB_USBIE_RESETBABBLE_OFS                (2)                             /* !< RESETBABBLE Offset */
#define USB_USBIE_RESETBABBLE_MASK               ((uint8_t)0x00000004U)          /* !< Interrupt enable for RESET/BABBLE
                                                                                    signaling detected. */
#define USB_USBIE_RESETBABBLE_DSIABLE            ((uint8_t)0x00000000U)          /* !< The RESET interrupt is suppressed
                                                                                    and not sent to the interrupt
                                                                                    controller. */
#define USB_USBIE_RESETBABBLE_ENABLE             ((uint8_t)0x00000004U)          /* !< An interrupt is sent to the
                                                                                    interrupt controller when the RESET
                                                                                    bit in the USBIS register is set. */
/* USB_USBIE[SOF] Bits */
#define USB_USBIE_SOF_OFS                        (3)                             /* !< SOF Offset */
#define USB_USBIE_SOF_MASK                       ((uint8_t)0x00000008U)          /* !< Interrupt enable for Start of
                                                                                    frame. */
#define USB_USBIE_SOF_DISABLE                    ((uint8_t)0x00000000U)          /* !< The SOF interrupt is suppressed and
                                                                                    not sent to the interrupt controller. */
#define USB_USBIE_SOF_ENABLE                     ((uint8_t)0x00000008U)          /* !< An interrupt is sent to the
                                                                                    interrupt controller when the SOF bit
                                                                                    in the USBIS register is set. */
/* USB_USBIE[CONN] Bits */
#define USB_USBIE_CONN_OFS                       (4)                             /* !< CONN Offset */
#define USB_USBIE_CONN_MASK                      ((uint8_t)0x00000010U)          /* !< Interrupt enable for connect
                                                                                    detected. Note: Only valid in Host
                                                                                    mode. Valid at all transaction
                                                                                    speeds. */
#define USB_USBIE_CONN_DISABLE                   ((uint8_t)0x00000000U)          /* !< The CONN interrupt is suppressed
                                                                                    and not sent to the interrupt
                                                                                    controller. */
#define USB_USBIE_CONN_ENABLE                    ((uint8_t)0x00000010U)          /* !< An interrupt is sent to the
                                                                                    interrupt controller when the CONN
                                                                                    bit in the USBIS register is set. */
/* USB_USBIE[DISCON] Bits */
#define USB_USBIE_DISCON_OFS                     (5)                             /* !< DISCON Offset */
#define USB_USBIE_DISCON_MASK                    ((uint8_t)0x00000020U)          /* !< Interrupt enable for session
                                                                                    disconnect. */
#define USB_USBIE_DISCON_DISABLE                 ((uint8_t)0x00000000U)          /* !< The DISCON interrupt is suppressed
                                                                                    and not sent to the interrupt
                                                                                    controller. */
#define USB_USBIE_DISCON_ENABLE                  ((uint8_t)0x00000020U)          /* !< An interrupt is sent to the
                                                                                    interrupt controller when the DISCON
                                                                                    bit in the USBIS register is set. */

/* USB_FRAME Bits */
/* USB_FRAME[FRAME] Bits */
#define USB_FRAME_FRAME_OFS                      (0)                             /* !< FRAME Offset */
#define USB_FRAME_FRAME_MASK                     ((uint16_t)0x000007FFU)         /* !< Frame Number */

/* USB_EPINDEX Bits */
/* USB_EPINDEX[EPIDX] Bits */
#define USB_EPINDEX_EPIDX_OFS                    (0)                             /* !< EPIDX Offset */
#define USB_EPINDEX_EPIDX_MASK                   ((uint8_t)0x0000000FU)          /* !< Endpoint Index.  #br#This bit field
                                                                                    configures which endpoint is accessed
                                                                                    when reading or writing to one of the
                                                                                    USB controller's indexed registers. A
                                                                                    value of 0x0 corresponds to Endpoint
                                                                                    0 and a value of 0xF corresponds to
                                                                                    Endpoint 15. */
#define USB_EPINDEX_EPIDX_EP0                    ((uint8_t)0x00000000U)          /* !< Select EP0 as indexed register */
#define USB_EPINDEX_EPIDX_EP1                    ((uint8_t)0x00000001U)          /* !< Select EP1 as indexed register */
#define USB_EPINDEX_EPIDX_EP2                    ((uint8_t)0x00000002U)          /* !< Select EP2 as indexed register */
#define USB_EPINDEX_EPIDX_EP3                    ((uint8_t)0x00000003U)          /* !< Select EP3 as indexed register */
#define USB_EPINDEX_EPIDX_EP4                    ((uint8_t)0x00000004U)          /* !< Select EP4 as indexed register */
#define USB_EPINDEX_EPIDX_EP5                    ((uint8_t)0x00000005U)          /* !< Select EP5 as indexed register */
#define USB_EPINDEX_EPIDX_EP6                    ((uint8_t)0x00000006U)          /* !< Select EP6 as indexed register */
#define USB_EPINDEX_EPIDX_EP7                    ((uint8_t)0x00000007U)          /* !< Select EP7 as indexed register */

/* USB_TESTMODE Bits */
/* USB_TESTMODE[FORCEFS] Bits */
#define USB_TESTMODE_FORCEFS_OFS                 (5)                             /* !< FORCEFS Offset */
#define USB_TESTMODE_FORCEFS_MASK                ((uint8_t)0x00000020U)          /* !< Force Full Speed Upon Reset. The
                                                                                    CPU sets this bit either in
                                                                                    conjunction with bit 7 (FORCEH) or to
                                                                                    force the USB controller into
                                                                                    Full-speed mode when it receives a
                                                                                    USB reset. */
#define USB_TESTMODE_FORCEFS_LOW                 ((uint8_t)0x00000000U)          /* !< The USB controller operates at Low
                                                                                    Speed. */
#define USB_TESTMODE_FORCEFS_FULL                ((uint8_t)0x00000020U)          /* !< Forces the USB controller into
                                                                                    Full-Speed mode upon receiving a USB
                                                                                    RESET. */
/* USB_TESTMODE[FIFOACC] Bits */
#define USB_TESTMODE_FIFOACC_OFS                 (6)                             /* !< FIFOACC Offset */
#define USB_TESTMODE_FIFOACC_MASK                ((uint8_t)0x00000040U)          /* !< FIFO Access. The CPU sets this bit
                                                                                    to transfer the packet in the
                                                                                    Endpoint 0 TX FIFO to the Endpoint 0
                                                                                    Rx FIFO.  Note: It is cleared
                                                                                    automatically. */
#define USB_TESTMODE_FIFOACC_NO_EFFECT           ((uint8_t)0x00000000U)          /* !< No effect */
#define USB_TESTMODE_FIFOACC_EFFECT              ((uint8_t)0x00000040U)          /* !< Transfers the packet in the
                                                                                    endpoint 0 transmit FIFO to the
                                                                                    endpoint 0 receive FIFO. */
/* USB_TESTMODE[FORCEH] Bits */
#define USB_TESTMODE_FORCEH_OFS                  (7)                             /* !< FORCEH Offset */
#define USB_TESTMODE_FORCEH_MASK                 ((uint8_t)0x00000080U)          /* !< Force Host Mode. The CPU sets this
                                                                                    bit to instruct the core to enter
                                                                                    Host mode when the Session bit is
                                                                                    set, regardless of whether it is
                                                                                    connected to any peripheral. The
                                                                                    state of the CID input,
                                                                                    HostDisconnect and LineState signals
                                                                                    are ignored. The core will then
                                                                                    remain in Host mode until the Session
                                                                                    bit is cleared, even if a device is
                                                                                    disconnected, and if the Force_Host
                                                                                    bit remains set, will re-enter Host
                                                                                    mode the next time the Session bit is
                                                                                    set. While in this mode, status of
                                                                                    the bus connection may be read using
                                                                                    the DEV bit of the USBDEVCTL
                                                                                    register. The operating speed is
                                                                                    determined from the FORCEFS bit. */
#define USB_TESTMODE_FORCEH_NO_EFFECT            ((uint8_t)0x00000000U)          /* !< No effect */
#define USB_TESTMODE_FORCEH_EFFECT               ((uint8_t)0x00000080U)          /* !< Forces the USB controller to enter
                                                                                    Host mode when the SESSION bit is
                                                                                    set, regardless ofwhether the USB
                                                                                    controller is connected to any
                                                                                    peripheral. The state of the USB0DP
                                                                                    and USB0DM signals is ignored. The
                                                                                    USB controller then remains in Host
                                                                                    mode until the SESSION #br#bit is
                                                                                    cleared, even if a Device is
                                                                                    disconnected. If the FORCEH bit
                                                                                    remains set, the USB controller
                                                                                    re-enters Host mode the next time the
                                                                                    SESSION bit is set. */

/* USB_IDXTXMAXP Bits */
/* USB_IDXTXMAXP[DATA] Bits */
#define USB_IDXTXMAXP_DATA_OFS                   (0)                             /* !< DATA Offset */
#define USB_IDXTXMAXP_DATA_MASK                  ((uint16_t)0x0000FFFFU)         /* !< Control / Status register data. */

/* USB_IDXTXCSRL Bits */
/* USB_IDXTXCSRL[DATA] Bits */
#define USB_IDXTXCSRL_DATA_OFS                   (0)                             /* !< DATA Offset */
#define USB_IDXTXCSRL_DATA_MASK                  ((uint8_t)0x000000FFU)          /* !< Control / Status register data. */

/* USB_IDXTXCSRH Bits */
/* USB_IDXTXCSRH[DATA] Bits */
#define USB_IDXTXCSRH_DATA_OFS                   (0)                             /* !< DATA Offset */
#define USB_IDXTXCSRH_DATA_MASK                  ((uint8_t)0x000000FFU)          /* !< Control / Status register data. */

/* USB_IDXRXMAXP Bits */
/* USB_IDXRXMAXP[DATA] Bits */
#define USB_IDXRXMAXP_DATA_OFS                   (0)                             /* !< DATA Offset */
#define USB_IDXRXMAXP_DATA_MASK                  ((uint16_t)0x0000FFFFU)         /* !< Control / Status register data. */

/* USB_IDXRXCSRL Bits */
/* USB_IDXRXCSRL[DATA] Bits */
#define USB_IDXRXCSRL_DATA_OFS                   (0)                             /* !< DATA Offset */
#define USB_IDXRXCSRL_DATA_MASK                  ((uint8_t)0x000000FFU)          /* !< Control / Status register data. */

/* USB_IDXRXCSRH Bits */
/* USB_IDXRXCSRH[DATA] Bits */
#define USB_IDXRXCSRH_DATA_OFS                   (0)                             /* !< DATA Offset */
#define USB_IDXRXCSRH_DATA_MASK                  ((uint8_t)0x000000FFU)          /* !< Control / Status register data. */

/* USB_IDXRXCOUNT Bits */
/* USB_IDXRXCOUNT[DATA] Bits */
#define USB_IDXRXCOUNT_DATA_OFS                  (0)                             /* !< DATA Offset */
#define USB_IDXRXCOUNT_DATA_MASK                 ((uint16_t)0x0000FFFFU)         /* !< Control / Status register data. */

/* USB_IDXTXTYPE Bits */
/* USB_IDXTXTYPE[DATA] Bits */
#define USB_IDXTXTYPE_DATA_OFS                   (0)                             /* !< DATA Offset */
#define USB_IDXTXTYPE_DATA_MASK                  ((uint8_t)0x000000FFU)          /* !< Control / Status register data. */

/* USB_IDXTXINTERVAL Bits */
/* USB_IDXTXINTERVAL[DATA] Bits */
#define USB_IDXTXINTERVAL_DATA_OFS               (0)                             /* !< DATA Offset */
#define USB_IDXTXINTERVAL_DATA_MASK              ((uint8_t)0x000000FFU)          /* !< Control / Status register data. */

/* USB_IDXRXTYPE Bits */
/* USB_IDXRXTYPE[DATA] Bits */
#define USB_IDXRXTYPE_DATA_OFS                   (0)                             /* !< DATA Offset */
#define USB_IDXRXTYPE_DATA_MASK                  ((uint8_t)0x000000FFU)          /* !< Control / Status register data. */

/* USB_IDXRXINTERVAL Bits */
/* USB_IDXRXINTERVAL[DATA] Bits */
#define USB_IDXRXINTERVAL_DATA_OFS               (0)                             /* !< DATA Offset */
#define USB_IDXRXINTERVAL_DATA_MASK              ((uint8_t)0x000000FFU)          /* !< Control / Status register data. */

/* USB_IDXFIFOSZ Bits */
/* USB_IDXFIFOSZ[DATA] Bits */
#define USB_IDXFIFOSZ_DATA_OFS                   (0)                             /* !< DATA Offset */
#define USB_IDXFIFOSZ_DATA_MASK                  ((uint8_t)0x000000FFU)          /* !< Control / Status register data. */

/* USB_FIFO Bits */
/* USB_FIFO[EPDATA] Bits */
#define USB_FIFO_EPDATA_OFS                      (0)                             /* !< EPDATA Offset */
#define USB_FIFO_EPDATA_MASK                     ((uint32_t)0xFFFFFFFFU)         /* !< Endpoint Data. Writing to this
                                                                                    register loads the data into the
                                                                                    Transmit FIFO and reading unloads
                                                                                    data from the Receive FIFO. */

/* USB_BYTE Bits */
/* USB_BYTE[EPDATA] Bits */
#define USB_BYTE_EPDATA_OFS                      (0)                             /* !< EPDATA Offset */
#define USB_BYTE_EPDATA_MASK                     ((uint32_t)0x000000FFU)         /* !< Endpoint Data 8-bit. */

/* USB_HALF Bits */
/* USB_HALF[EPDATA] Bits */
#define USB_HALF_EPDATA_OFS                      (0)                             /* !< EPDATA Offset */
#define USB_HALF_EPDATA_MASK                     ((uint32_t)0x0000FFFFU)         /* !< Endpoint Data 16-bit. */

/* USB_WORD Bits */
/* USB_WORD[EPDATA] Bits */
#define USB_WORD_EPDATA_OFS                      (0)                             /* !< EPDATA Offset */
#define USB_WORD_EPDATA_MASK                     ((uint32_t)0xFFFFFFFFU)         /* !< Endpoint Data 32-bit. */

/* USB_DEVCTL Bits */
/* USB_DEVCTL[SESSION] Bits */
#define USB_DEVCTL_SESSION_OFS                   (0)                             /* !< SESSION Offset */
#define USB_DEVCTL_SESSION_MASK                  ((uint8_t)0x00000001U)          /* !< Session Start/End. Host mode: This
                                                                                    bit is set or cleared by the CPU to
                                                                                    start or end a session. Device mode:
                                                                                    This bit is set/cleared by the USB
                                                                                    controller when a session
                                                                                    starts/ends. It is also set by the
                                                                                    CPU to initiate the Session Request
                                                                                    Protocol. When the USB controller is
                                                                                    in Suspend mode, the bit may be
                                                                                    cleared by the CPU to perform a
                                                                                    software disconnect.  Note: Clearing
                                                                                    this bit when the core is not
                                                                                    suspended will result in undefined
                                                                                    behavior. */
#define USB_DEVCTL_SESSION_CLEAR                 ((uint8_t)0x00000000U)          /* !< When operating as a Host: #br#When
                                                                                    cleared by software, this bit ends a
                                                                                    session. #br# #br#When operating as a
                                                                                    Device: #br#The USB controller has
                                                                                    ended a session. When the USB
                                                                                    controller is in SUSPEND mode, this
                                                                                    bit may be cleared by software to
                                                                                    perform a software disconnect. */
#define USB_DEVCTL_SESSION_SET                   ((uint8_t)0x00000001U)          /* !< When operating as a Host: #br#When
                                                                                    set by software, this bit starts a
                                                                                    session. #br# #br#When operating as a
                                                                                    Device: #br#The USB controller has
                                                                                    started a session. When set by
                                                                                    software, the Session Request
                                                                                    Protocol is initiated. #br#
                                                                                    #br#Clearing this bit when the USB
                                                                                    controller is not suspended results
                                                                                    in undefined behavior. */
/* USB_DEVCTL[HOSTREQ] Bits */
#define USB_DEVCTL_HOSTREQ_OFS                   (1)                             /* !< HOSTREQ Offset */
#define USB_DEVCTL_HOSTREQ_MASK                  ((uint8_t)0x00000002U)          /* !< When set, the USB controller will
                                                                                    initiate the Host Negotiation when
                                                                                    Suspend mode is entered.  It is
                                                                                    cleared when Host Negotiation is
                                                                                    completed. */
#define USB_DEVCTL_HOSTREQ_NO_EFFECT             ((uint8_t)0x00000000U)          /* !< No effect */
#define USB_DEVCTL_HOSTREQ_EFFECT                ((uint8_t)0x00000002U)          /* !< Initiates the Host Negotiation when
                                                                                    SUSPENDmode is entered. */
/* USB_DEVCTL[HOST] Bits */
#define USB_DEVCTL_HOST_OFS                      (2)                             /* !< HOST Offset */
#define USB_DEVCTL_HOST_MASK                     ((uint8_t)0x00000004U)          /* !< Host Mode. This Read-only bit is
                                                                                    set when the USB controller is acting
                                                                                    as a Host. Note: Only valid while a
                                                                                    session is in progress. */
#define USB_DEVCTL_HOST_DEVICE                   ((uint8_t)0x00000000U)          /* !< The USB controller is acting as a
                                                                                    Device. */
#define USB_DEVCTL_HOST_HOST                     ((uint8_t)0x00000004U)          /* !< The USB controller is acting as a
                                                                                    Host. */
/* USB_DEVCTL[VUSB] Bits */
#define USB_DEVCTL_VUSB_OFS                      (3)                             /* !< VUSB Offset */
#define USB_DEVCTL_VUSB_MASK                     ((uint8_t)0x00000018U)          /* !< VUSB / USB-PHY is powered. */
#define USB_DEVCTL_VUSB_NOT_POWERED              ((uint8_t)0x00000000U)          /* !< VUSB / USB-PHY supply is detected
                                                                                    as under 1.35V. */
#define USB_DEVCTL_VUSB_POWERED                  ((uint8_t)0x00000018U)          /* !< VUSB / USB-PHY supply is detected
                                                                                    as above 1.35V. */
/* USB_DEVCTL[LSDEV] Bits */
#define USB_DEVCTL_LSDEV_OFS                     (5)                             /* !< LSDEV Offset */
#define USB_DEVCTL_LSDEV_MASK                    ((uint8_t)0x00000020U)          /* !< Low Speed Device Detected. This
                                                                                    Read-only bit is set when a low-speed
                                                                                    device has been detected being
                                                                                    connected to the port. Note: Only
                                                                                    valid in Host mode. */
#define USB_DEVCTL_LSDEV_NO_DETCT                ((uint8_t)0x00000000U)          /* !< A low-speed Device has not been
                                                                                    detected on the port. */
#define USB_DEVCTL_LSDEV_DETCT                   ((uint8_t)0x00000020U)          /* !< A low-speed Device has been
                                                                                    detected on the port. */
/* USB_DEVCTL[FSDEV] Bits */
#define USB_DEVCTL_FSDEV_OFS                     (6)                             /* !< FSDEV Offset */
#define USB_DEVCTL_FSDEV_MASK                    ((uint8_t)0x00000040U)          /* !< Full Speed Device Detected. This
                                                                                    Read-only bit is set when a
                                                                                    full-speed device has been detected
                                                                                    being connected to the port.  Note:
                                                                                    Only valid in Host mode. */
#define USB_DEVCTL_FSDEV_NO_DETCT                ((uint8_t)0x00000000U)          /* !< A full-speed Device has not been
                                                                                    detected on the port. */
#define USB_DEVCTL_FSDEV_DETCT                   ((uint8_t)0x00000040U)          /* !< A full-speed Device has been
                                                                                    detected on the port. */
/* USB_DEVCTL[DEV] Bits */
#define USB_DEVCTL_DEV_OFS                       (7)                             /* !< DEV Offset */
#define USB_DEVCTL_DEV_MASK                      ((uint8_t)0x00000080U)          /* !< Device Mode. This Read-only bit
                                                                                    indicates whether the USB controller
                                                                                    is operating as the host mode or
                                                                                    device mode.  Note: Only valid while
                                                                                    a session is in progress. */
#define USB_DEVCTL_DEV_HOST                      ((uint8_t)0x00000000U)          /* !< The USB controller is operating in
                                                                                    host mode. */
#define USB_DEVCTL_DEV_DEVICE                    ((uint8_t)0x00000080U)          /* !< The USB controller is operating in
                                                                                    device mode. */

/* USB_MISC Bits */
/* USB_MISC[RX_EDMA] Bits */
#define USB_MISC_RX_EDMA_OFS                     (0)                             /* !< RX_EDMA Offset */
#define USB_MISC_RX_EDMA_MASK                    ((uint8_t)0x00000001U)          /* !< Receive early DMA mode. */
#define USB_MISC_RX_EDMA_LATE                    ((uint8_t)0x00000000U)          /* !< Late mode: DMA_REQ signal for all
                                                                                    OUT Endpoints will be de-asserted
                                                                                    when MAXP bytes have been read to an
                                                                                    endpoint. */
#define USB_MISC_RX_EDMA_EARLY                   ((uint8_t)0x00000001U)          /* !< Early Mode: DMA_REQ signal for all
                                                                                    OUT Endpoints will be de-asserted
                                                                                    when MAXP-8 bytes have been read to
                                                                                    an endpoint. */
/* USB_MISC[TX_EDMA] Bits */
#define USB_MISC_TX_EDMA_OFS                     (1)                             /* !< TX_EDMA Offset */
#define USB_MISC_TX_EDMA_MASK                    ((uint8_t)0x00000002U)          /* !< Transmit early DMA mode. */
#define USB_MISC_TX_EDMA_LATE                    ((uint8_t)0x00000000U)          /* !< Late mode: DMA_REQ signal for all
                                                                                    IN Endpoints will be de-asserted when
                                                                                    MAXP bytes have been written to an
                                                                                    endpoint. This is late mode. */
#define USB_MISC_TX_EDMA_EARLY                   ((uint8_t)0x00000002U)          /* !< Early mode: DMA_REQ signal for all
                                                                                    IN Endpoints will be de-asserted when
                                                                                    MAXP-8 bytes have been written to an
                                                                                    endpoint. This is early mode. */

/* USB_IDXTXFIFOSZ Bits */
/* USB_IDXTXFIFOSZ[SIZE] Bits */
#define USB_IDXTXFIFOSZ_SIZE_OFS                 (0)                             /* !< SIZE Offset */
#define USB_IDXTXFIFOSZ_SIZE_MASK                ((uint8_t)0x0000000FU)          /* !< Max Packet Size */
#define USB_IDXTXFIFOSZ_SIZE_BYTES_8             ((uint8_t)0x00000000U)          /* !< FIFO size 8-byte */
#define USB_IDXTXFIFOSZ_SIZE_BYTES_16            ((uint8_t)0x00000001U)          /* !< FIFO size 16-byte */
#define USB_IDXTXFIFOSZ_SIZE_BYTES_32            ((uint8_t)0x00000002U)          /* !< FIFO size 32-byte */
#define USB_IDXTXFIFOSZ_SIZE_BYTES_64            ((uint8_t)0x00000003U)          /* !< FIFO size 64-byte */
#define USB_IDXTXFIFOSZ_SIZE_BYTES_128           ((uint8_t)0x00000004U)          /* !< FIFO size 128-byte */
#define USB_IDXTXFIFOSZ_SIZE_BYTES_256           ((uint8_t)0x00000005U)          /* !< FIFO size 256-byte */
#define USB_IDXTXFIFOSZ_SIZE_BYTES_512           ((uint8_t)0x00000006U)          /* !< FIFO size 512-byte */
#define USB_IDXTXFIFOSZ_SIZE_BYTES_1024          ((uint8_t)0x00000007U)          /* !< FIFO size 1024-byte */
#define USB_IDXTXFIFOSZ_SIZE_BYTES_2048          ((uint8_t)0x00000008U)          /* !< FIFO size 2048-byte */
/* USB_IDXTXFIFOSZ[DPB] Bits */
#define USB_IDXTXFIFOSZ_DPB_OFS                  (4)                             /* !< DPB Offset */
#define USB_IDXTXFIFOSZ_DPB_MASK                 ((uint8_t)0x00000010U)          /* !< Double Packet Buffer Support */
#define USB_IDXTXFIFOSZ_DPB_SIGLE                ((uint8_t)0x00000000U)          /* !< Single packet buffering is
                                                                                    supported. */
#define USB_IDXTXFIFOSZ_DPB_DOUBLE               ((uint8_t)0x00000010U)          /* !< Double packet buffering is enabled. */

/* USB_IDXRXFIFOSZ Bits */
/* USB_IDXRXFIFOSZ[SIZE] Bits */
#define USB_IDXRXFIFOSZ_SIZE_OFS                 (0)                             /* !< SIZE Offset */
#define USB_IDXRXFIFOSZ_SIZE_MASK                ((uint8_t)0x0000000FU)          /* !< Maximum packet size to be allowed.
                                                                                    If DPB = 0, the FIFO also is this
                                                                                    size; if DPB = 1, the FIFO is twice
                                                                                    this size. Packet size in bytes: */
#define USB_IDXRXFIFOSZ_SIZE_BYTES_8             ((uint8_t)0x00000000U)          /* !< FIFO size 8-byte */
#define USB_IDXRXFIFOSZ_SIZE_BYTES_16            ((uint8_t)0x00000001U)          /* !< FIFO size 16-byte */
#define USB_IDXRXFIFOSZ_SIZE_BYTES_32            ((uint8_t)0x00000002U)          /* !< FIFO size 32-byte */
#define USB_IDXRXFIFOSZ_SIZE_BYTES_64            ((uint8_t)0x00000003U)          /* !< FIFO size 64-byte */
#define USB_IDXRXFIFOSZ_SIZE_BYTES_128           ((uint8_t)0x00000004U)          /* !< FIFO size 128-byte */
#define USB_IDXRXFIFOSZ_SIZE_BYTES_256           ((uint8_t)0x00000005U)          /* !< FIFO size 256-byte */
#define USB_IDXRXFIFOSZ_SIZE_BYTES_512           ((uint8_t)0x00000006U)          /* !< FIFO size 512-byte */
#define USB_IDXRXFIFOSZ_SIZE_BYTES_1024          ((uint8_t)0x00000007U)          /* !< FIFO size 1024-byte */
#define USB_IDXRXFIFOSZ_SIZE_BYTES_2048          ((uint8_t)0x00000008U)          /* !< FIFO size 2048-byte */
/* USB_IDXRXFIFOSZ[DPB] Bits */
#define USB_IDXRXFIFOSZ_DPB_OFS                  (4)                             /* !< DPB Offset */
#define USB_IDXRXFIFOSZ_DPB_MASK                 ((uint8_t)0x00000010U)          /* !< Double Packet Buffer Support */
#define USB_IDXRXFIFOSZ_DPB_SIGLE                ((uint8_t)0x00000000U)          /* !< Single packet buffering is
                                                                                    supported. */
#define USB_IDXRXFIFOSZ_DPB_DOUBLE               ((uint8_t)0x00000010U)          /* !< Double packet buffering is enabled. */

/* USB_IDXTXFIFOADD Bits */
/* USB_IDXTXFIFOADD[ADDR] Bits */
#define USB_IDXTXFIFOADD_ADDR_OFS                (0)                             /* !< ADDR Offset */
#define USB_IDXTXFIFOADD_ADDR_MASK               ((uint16_t)0x00003FFFU)         /* !< Endpoint Data */
#define USB_IDXTXFIFOADD_ADDR_ADDR_0             ((uint16_t)0x00000000U)         /* !< 0.0 */
#define USB_IDXTXFIFOADD_ADDR_ADDR_8             ((uint16_t)0x00000001U)         /* !< 8.0 */
#define USB_IDXTXFIFOADD_ADDR_ADDR_16            ((uint16_t)0x00000002U)         /* !< 16.0 */
#define USB_IDXTXFIFOADD_ADDR_ADDR_24            ((uint16_t)0x00000003U)         /* !< 24.0 */
#define USB_IDXTXFIFOADD_ADDR_ADDR_32            ((uint16_t)0x00000004U)         /* !< 32.0 */
#define USB_IDXTXFIFOADD_ADDR_ADDR_40            ((uint16_t)0x00000005U)         /* !< 40.0 */
#define USB_IDXTXFIFOADD_ADDR_ADDR_48            ((uint16_t)0x00000006U)         /* !< 48.0 */
#define USB_IDXTXFIFOADD_ADDR_ADDR_56            ((uint16_t)0x00000007U)         /* !< 56.0 */
#define USB_IDXTXFIFOADD_ADDR_ADDR_64            ((uint16_t)0x00000008U)         /* !< 64.0 */
#define USB_IDXTXFIFOADD_ADDR_ADDR_72            ((uint16_t)0x00000009U)         /* !< 72.0 */
#define USB_IDXTXFIFOADD_ADDR_ADDR_80            ((uint16_t)0x0000000AU)         /* !< 80.0 */
#define USB_IDXTXFIFOADD_ADDR_ADDR_88            ((uint16_t)0x0000000BU)         /* !< 88.0 */
#define USB_IDXTXFIFOADD_ADDR_ADDR_96            ((uint16_t)0x0000000CU)         /* !< 96.0 */
#define USB_IDXTXFIFOADD_ADDR_ADDR_104           ((uint16_t)0x0000000DU)         /* !< 104.0 */
#define USB_IDXTXFIFOADD_ADDR_ADDR_112           ((uint16_t)0x0000000EU)         /* !< 112.0 */
#define USB_IDXTXFIFOADD_ADDR_ADDR_120           ((uint16_t)0x0000000FU)         /* !< 120.0 */
#define USB_IDXTXFIFOADD_ADDR_ADDR_128           ((uint16_t)0x00000010U)         /* !< 128.0 */
#define USB_IDXTXFIFOADD_ADDR_ADDR_136           ((uint16_t)0x00000011U)         /* !< 136.0 */
#define USB_IDXTXFIFOADD_ADDR_ADDR_144           ((uint16_t)0x00000012U)         /* !< 144.0 */
#define USB_IDXTXFIFOADD_ADDR_ADDR_152           ((uint16_t)0x00000013U)         /* !< 152.0 */
#define USB_IDXTXFIFOADD_ADDR_ADDR_160           ((uint16_t)0x00000014U)         /* !< 160.0 */
#define USB_IDXTXFIFOADD_ADDR_ADDR_168           ((uint16_t)0x00000015U)         /* !< 168.0 */
#define USB_IDXTXFIFOADD_ADDR_ADDR_176           ((uint16_t)0x00000016U)         /* !< 176.0 */
#define USB_IDXTXFIFOADD_ADDR_ADDR_184           ((uint16_t)0x00000017U)         /* !< 184.0 */
#define USB_IDXTXFIFOADD_ADDR_ADDR_192           ((uint16_t)0x00000018U)         /* !< 192.0 */
#define USB_IDXTXFIFOADD_ADDR_ADDR_200           ((uint16_t)0x00000019U)         /* !< 200.0 */
#define USB_IDXTXFIFOADD_ADDR_ADDR_208           ((uint16_t)0x0000001AU)         /* !< 208.0 */
#define USB_IDXTXFIFOADD_ADDR_ADDR_216           ((uint16_t)0x0000001BU)         /* !< 216.0 */
#define USB_IDXTXFIFOADD_ADDR_ADDR_224           ((uint16_t)0x0000001CU)         /* !< 224.0 */
#define USB_IDXTXFIFOADD_ADDR_ADDR_232           ((uint16_t)0x0000001DU)         /* !< 232.0 */
#define USB_IDXTXFIFOADD_ADDR_ADDR_240           ((uint16_t)0x0000001EU)         /* !< 240.0 */
#define USB_IDXTXFIFOADD_ADDR_ADDR_248           ((uint16_t)0x0000001FU)         /* !< 248.0 */
#define USB_IDXTXFIFOADD_ADDR_ADDR_256           ((uint16_t)0x00000020U)         /* !< 256.0 */
#define USB_IDXTXFIFOADD_ADDR_ADDR_264           ((uint16_t)0x00000021U)         /* !< 264.0 */
#define USB_IDXTXFIFOADD_ADDR_ADDR_272           ((uint16_t)0x00000022U)         /* !< 272.0 */
#define USB_IDXTXFIFOADD_ADDR_ADDR_280           ((uint16_t)0x00000023U)         /* !< 280.0 */
#define USB_IDXTXFIFOADD_ADDR_ADDR_288           ((uint16_t)0x00000024U)         /* !< 288.0 */
#define USB_IDXTXFIFOADD_ADDR_ADDR_296           ((uint16_t)0x00000025U)         /* !< 296.0 */
#define USB_IDXTXFIFOADD_ADDR_ADDR_304           ((uint16_t)0x00000026U)         /* !< 304.0 */
#define USB_IDXTXFIFOADD_ADDR_ADDR_312           ((uint16_t)0x00000027U)         /* !< 312.0 */
#define USB_IDXTXFIFOADD_ADDR_ADDR_320           ((uint16_t)0x00000028U)         /* !< 320.0 */
#define USB_IDXTXFIFOADD_ADDR_ADDR_328           ((uint16_t)0x00000029U)         /* !< 328.0 */
#define USB_IDXTXFIFOADD_ADDR_ADDR_336           ((uint16_t)0x0000002AU)         /* !< 336.0 */
#define USB_IDXTXFIFOADD_ADDR_ADDR_344           ((uint16_t)0x0000002BU)         /* !< 344.0 */
#define USB_IDXTXFIFOADD_ADDR_ADDR_352           ((uint16_t)0x0000002CU)         /* !< 352.0 */
#define USB_IDXTXFIFOADD_ADDR_ADDR_360           ((uint16_t)0x0000002DU)         /* !< 360.0 */
#define USB_IDXTXFIFOADD_ADDR_ADDR_368           ((uint16_t)0x0000002EU)         /* !< 368.0 */
#define USB_IDXTXFIFOADD_ADDR_ADDR_376           ((uint16_t)0x0000002FU)         /* !< 376.0 */
#define USB_IDXTXFIFOADD_ADDR_ADDR_384           ((uint16_t)0x00000030U)         /* !< 384.0 */
#define USB_IDXTXFIFOADD_ADDR_ADDR_392           ((uint16_t)0x00000031U)         /* !< 392.0 */
#define USB_IDXTXFIFOADD_ADDR_ADDR_400           ((uint16_t)0x00000032U)         /* !< 400.0 */
#define USB_IDXTXFIFOADD_ADDR_ADDR_408           ((uint16_t)0x00000033U)         /* !< 408.0 */
#define USB_IDXTXFIFOADD_ADDR_ADDR_416           ((uint16_t)0x00000034U)         /* !< 416.0 */
#define USB_IDXTXFIFOADD_ADDR_ADDR_424           ((uint16_t)0x00000035U)         /* !< 424.0 */
#define USB_IDXTXFIFOADD_ADDR_ADDR_432           ((uint16_t)0x00000036U)         /* !< 432.0 */
#define USB_IDXTXFIFOADD_ADDR_ADDR_440           ((uint16_t)0x00000037U)         /* !< 440.0 */
#define USB_IDXTXFIFOADD_ADDR_ADDR_448           ((uint16_t)0x00000038U)         /* !< 448.0 */
#define USB_IDXTXFIFOADD_ADDR_ADDR_456           ((uint16_t)0x00000039U)         /* !< 456.0 */
#define USB_IDXTXFIFOADD_ADDR_ADDR_464           ((uint16_t)0x0000003AU)         /* !< 464.0 */
#define USB_IDXTXFIFOADD_ADDR_ADDR_472           ((uint16_t)0x0000003BU)         /* !< 472.0 */
#define USB_IDXTXFIFOADD_ADDR_ADDR_480           ((uint16_t)0x0000003CU)         /* !< 480.0 */
#define USB_IDXTXFIFOADD_ADDR_ADDR_488           ((uint16_t)0x0000003DU)         /* !< 488.0 */
#define USB_IDXTXFIFOADD_ADDR_ADDR_496           ((uint16_t)0x0000003EU)         /* !< 496.0 */
#define USB_IDXTXFIFOADD_ADDR_ADDR_504           ((uint16_t)0x0000003FU)         /* !< 504.0 */
#define USB_IDXTXFIFOADD_ADDR_ADDR_512           ((uint16_t)0x00000040U)         /* !< 512.0 */
#define USB_IDXTXFIFOADD_ADDR_ADDR_520           ((uint16_t)0x00000041U)         /* !< 520.0 */
#define USB_IDXTXFIFOADD_ADDR_ADDR_528           ((uint16_t)0x00000042U)         /* !< 528.0 */
#define USB_IDXTXFIFOADD_ADDR_ADDR_536           ((uint16_t)0x00000043U)         /* !< 536.0 */
#define USB_IDXTXFIFOADD_ADDR_ADDR_544           ((uint16_t)0x00000044U)         /* !< 544.0 */
#define USB_IDXTXFIFOADD_ADDR_ADDR_552           ((uint16_t)0x00000045U)         /* !< 552.0 */
#define USB_IDXTXFIFOADD_ADDR_ADDR_560           ((uint16_t)0x00000046U)         /* !< 560.0 */
#define USB_IDXTXFIFOADD_ADDR_ADDR_568           ((uint16_t)0x00000047U)         /* !< 568.0 */
#define USB_IDXTXFIFOADD_ADDR_ADDR_576           ((uint16_t)0x00000048U)         /* !< 576.0 */
#define USB_IDXTXFIFOADD_ADDR_ADDR_584           ((uint16_t)0x00000049U)         /* !< 584.0 */
#define USB_IDXTXFIFOADD_ADDR_ADDR_592           ((uint16_t)0x0000004AU)         /* !< 592.0 */
#define USB_IDXTXFIFOADD_ADDR_ADDR_600           ((uint16_t)0x0000004BU)         /* !< 600.0 */
#define USB_IDXTXFIFOADD_ADDR_ADDR_608           ((uint16_t)0x0000004CU)         /* !< 608.0 */
#define USB_IDXTXFIFOADD_ADDR_ADDR_616           ((uint16_t)0x0000004DU)         /* !< 616.0 */
#define USB_IDXTXFIFOADD_ADDR_ADDR_624           ((uint16_t)0x0000004EU)         /* !< 624.0 */
#define USB_IDXTXFIFOADD_ADDR_ADDR_632           ((uint16_t)0x0000004FU)         /* !< 632.0 */
#define USB_IDXTXFIFOADD_ADDR_ADDR_640           ((uint16_t)0x00000050U)         /* !< 640.0 */
#define USB_IDXTXFIFOADD_ADDR_ADDR_648           ((uint16_t)0x00000051U)         /* !< 648.0 */
#define USB_IDXTXFIFOADD_ADDR_ADDR_656           ((uint16_t)0x00000052U)         /* !< 656.0 */
#define USB_IDXTXFIFOADD_ADDR_ADDR_664           ((uint16_t)0x00000053U)         /* !< 664.0 */
#define USB_IDXTXFIFOADD_ADDR_ADDR_672           ((uint16_t)0x00000054U)         /* !< 672.0 */
#define USB_IDXTXFIFOADD_ADDR_ADDR_680           ((uint16_t)0x00000055U)         /* !< 680.0 */
#define USB_IDXTXFIFOADD_ADDR_ADDR_688           ((uint16_t)0x00000056U)         /* !< 688.0 */
#define USB_IDXTXFIFOADD_ADDR_ADDR_696           ((uint16_t)0x00000057U)         /* !< 696.0 */
#define USB_IDXTXFIFOADD_ADDR_ADDR_704           ((uint16_t)0x00000058U)         /* !< 704.0 */
#define USB_IDXTXFIFOADD_ADDR_ADDR_712           ((uint16_t)0x00000059U)         /* !< 712.0 */
#define USB_IDXTXFIFOADD_ADDR_ADDR_720           ((uint16_t)0x0000005AU)         /* !< 720.0 */
#define USB_IDXTXFIFOADD_ADDR_ADDR_728           ((uint16_t)0x0000005BU)         /* !< 728.0 */
#define USB_IDXTXFIFOADD_ADDR_ADDR_736           ((uint16_t)0x0000005CU)         /* !< 736.0 */
#define USB_IDXTXFIFOADD_ADDR_ADDR_744           ((uint16_t)0x0000005DU)         /* !< 744.0 */
#define USB_IDXTXFIFOADD_ADDR_ADDR_752           ((uint16_t)0x0000005EU)         /* !< 752.0 */
#define USB_IDXTXFIFOADD_ADDR_ADDR_760           ((uint16_t)0x0000005FU)         /* !< 760.0 */
#define USB_IDXTXFIFOADD_ADDR_ADDR_768           ((uint16_t)0x00000060U)         /* !< 768.0 */
#define USB_IDXTXFIFOADD_ADDR_ADDR_776           ((uint16_t)0x00000061U)         /* !< 776.0 */
#define USB_IDXTXFIFOADD_ADDR_ADDR_784           ((uint16_t)0x00000062U)         /* !< 784.0 */
#define USB_IDXTXFIFOADD_ADDR_ADDR_792           ((uint16_t)0x00000063U)         /* !< 792.0 */
#define USB_IDXTXFIFOADD_ADDR_ADDR_800           ((uint16_t)0x00000064U)         /* !< 800.0 */
#define USB_IDXTXFIFOADD_ADDR_ADDR_808           ((uint16_t)0x00000065U)         /* !< 808.0 */
#define USB_IDXTXFIFOADD_ADDR_ADDR_816           ((uint16_t)0x00000066U)         /* !< 816.0 */
#define USB_IDXTXFIFOADD_ADDR_ADDR_824           ((uint16_t)0x00000067U)         /* !< 824.0 */
#define USB_IDXTXFIFOADD_ADDR_ADDR_832           ((uint16_t)0x00000068U)         /* !< 832.0 */
#define USB_IDXTXFIFOADD_ADDR_ADDR_840           ((uint16_t)0x00000069U)         /* !< 840.0 */
#define USB_IDXTXFIFOADD_ADDR_ADDR_848           ((uint16_t)0x0000006AU)         /* !< 848.0 */
#define USB_IDXTXFIFOADD_ADDR_ADDR_856           ((uint16_t)0x0000006BU)         /* !< 856.0 */
#define USB_IDXTXFIFOADD_ADDR_ADDR_864           ((uint16_t)0x0000006CU)         /* !< 864.0 */
#define USB_IDXTXFIFOADD_ADDR_ADDR_872           ((uint16_t)0x0000006DU)         /* !< 872.0 */
#define USB_IDXTXFIFOADD_ADDR_ADDR_880           ((uint16_t)0x0000006EU)         /* !< 880.0 */
#define USB_IDXTXFIFOADD_ADDR_ADDR_888           ((uint16_t)0x0000006FU)         /* !< 888.0 */
#define USB_IDXTXFIFOADD_ADDR_ADDR_896           ((uint16_t)0x00000070U)         /* !< 896.0 */
#define USB_IDXTXFIFOADD_ADDR_ADDR_904           ((uint16_t)0x00000071U)         /* !< 904.0 */
#define USB_IDXTXFIFOADD_ADDR_ADDR_912           ((uint16_t)0x00000072U)         /* !< 912.0 */
#define USB_IDXTXFIFOADD_ADDR_ADDR_920           ((uint16_t)0x00000073U)         /* !< 920.0 */
#define USB_IDXTXFIFOADD_ADDR_ADDR_928           ((uint16_t)0x00000074U)         /* !< 928.0 */
#define USB_IDXTXFIFOADD_ADDR_ADDR_936           ((uint16_t)0x00000075U)         /* !< 936.0 */
#define USB_IDXTXFIFOADD_ADDR_ADDR_944           ((uint16_t)0x00000076U)         /* !< 944.0 */
#define USB_IDXTXFIFOADD_ADDR_ADDR_952           ((uint16_t)0x00000077U)         /* !< 952.0 */
#define USB_IDXTXFIFOADD_ADDR_ADDR_960           ((uint16_t)0x00000078U)         /* !< 960.0 */
#define USB_IDXTXFIFOADD_ADDR_ADDR_968           ((uint16_t)0x00000079U)         /* !< 968.0 */
#define USB_IDXTXFIFOADD_ADDR_ADDR_976           ((uint16_t)0x0000007AU)         /* !< 976.0 */
#define USB_IDXTXFIFOADD_ADDR_ADDR_984           ((uint16_t)0x0000007BU)         /* !< 984.0 */
#define USB_IDXTXFIFOADD_ADDR_ADDR_992           ((uint16_t)0x0000007CU)         /* !< 992.0 */
#define USB_IDXTXFIFOADD_ADDR_ADDR_1000          ((uint16_t)0x0000007DU)         /* !< 1000.0 */
#define USB_IDXTXFIFOADD_ADDR_ADDR_1008          ((uint16_t)0x0000007EU)         /* !< 1008.0 */
#define USB_IDXTXFIFOADD_ADDR_ADDR_1016          ((uint16_t)0x0000007FU)         /* !< 1016.0 */
#define USB_IDXTXFIFOADD_ADDR_ADDR_1024          ((uint16_t)0x00000080U)         /* !< 1024.0 */
#define USB_IDXTXFIFOADD_ADDR_ADDR_1032          ((uint16_t)0x00000081U)         /* !< 1032.0 */
#define USB_IDXTXFIFOADD_ADDR_ADDR_1040          ((uint16_t)0x00000082U)         /* !< 1040.0 */
#define USB_IDXTXFIFOADD_ADDR_ADDR_1048          ((uint16_t)0x00000083U)         /* !< 1048.0 */
#define USB_IDXTXFIFOADD_ADDR_ADDR_1056          ((uint16_t)0x00000084U)         /* !< 1056.0 */
#define USB_IDXTXFIFOADD_ADDR_ADDR_1064          ((uint16_t)0x00000085U)         /* !< 1064.0 */
#define USB_IDXTXFIFOADD_ADDR_ADDR_1072          ((uint16_t)0x00000086U)         /* !< 1072.0 */
#define USB_IDXTXFIFOADD_ADDR_ADDR_1080          ((uint16_t)0x00000087U)         /* !< 1080.0 */
#define USB_IDXTXFIFOADD_ADDR_ADDR_1088          ((uint16_t)0x00000088U)         /* !< 1088.0 */
#define USB_IDXTXFIFOADD_ADDR_ADDR_1096          ((uint16_t)0x00000089U)         /* !< 1096.0 */
#define USB_IDXTXFIFOADD_ADDR_ADDR_1104          ((uint16_t)0x0000008AU)         /* !< 1104.0 */
#define USB_IDXTXFIFOADD_ADDR_ADDR_1112          ((uint16_t)0x0000008BU)         /* !< 1112.0 */
#define USB_IDXTXFIFOADD_ADDR_ADDR_1120          ((uint16_t)0x0000008CU)         /* !< 1120.0 */
#define USB_IDXTXFIFOADD_ADDR_ADDR_1128          ((uint16_t)0x0000008DU)         /* !< 1128.0 */
#define USB_IDXTXFIFOADD_ADDR_ADDR_1136          ((uint16_t)0x0000008EU)         /* !< 1136.0 */
#define USB_IDXTXFIFOADD_ADDR_ADDR_1144          ((uint16_t)0x0000008FU)         /* !< 1144.0 */
#define USB_IDXTXFIFOADD_ADDR_ADDR_1152          ((uint16_t)0x00000090U)         /* !< 1152.0 */
#define USB_IDXTXFIFOADD_ADDR_ADDR_1160          ((uint16_t)0x00000091U)         /* !< 1160.0 */
#define USB_IDXTXFIFOADD_ADDR_ADDR_1168          ((uint16_t)0x00000092U)         /* !< 1168.0 */
#define USB_IDXTXFIFOADD_ADDR_ADDR_1176          ((uint16_t)0x00000093U)         /* !< 1176.0 */
#define USB_IDXTXFIFOADD_ADDR_ADDR_1184          ((uint16_t)0x00000094U)         /* !< 1184.0 */
#define USB_IDXTXFIFOADD_ADDR_ADDR_1192          ((uint16_t)0x00000095U)         /* !< 1192.0 */
#define USB_IDXTXFIFOADD_ADDR_ADDR_1200          ((uint16_t)0x00000096U)         /* !< 1200.0 */
#define USB_IDXTXFIFOADD_ADDR_ADDR_1208          ((uint16_t)0x00000097U)         /* !< 1208.0 */
#define USB_IDXTXFIFOADD_ADDR_ADDR_1216          ((uint16_t)0x00000098U)         /* !< 1216.0 */
#define USB_IDXTXFIFOADD_ADDR_ADDR_1224          ((uint16_t)0x00000099U)         /* !< 1224.0 */
#define USB_IDXTXFIFOADD_ADDR_ADDR_1232          ((uint16_t)0x0000009AU)         /* !< 1232.0 */
#define USB_IDXTXFIFOADD_ADDR_ADDR_1240          ((uint16_t)0x0000009BU)         /* !< 1240.0 */
#define USB_IDXTXFIFOADD_ADDR_ADDR_1248          ((uint16_t)0x0000009CU)         /* !< 1248.0 */
#define USB_IDXTXFIFOADD_ADDR_ADDR_1256          ((uint16_t)0x0000009DU)         /* !< 1256.0 */
#define USB_IDXTXFIFOADD_ADDR_ADDR_1264          ((uint16_t)0x0000009EU)         /* !< 1264.0 */
#define USB_IDXTXFIFOADD_ADDR_ADDR_1272          ((uint16_t)0x0000009FU)         /* !< 1272.0 */
#define USB_IDXTXFIFOADD_ADDR_ADDR_1280          ((uint16_t)0x000000A0U)         /* !< 1280.0 */
#define USB_IDXTXFIFOADD_ADDR_ADDR_1288          ((uint16_t)0x000000A1U)         /* !< 1288.0 */
#define USB_IDXTXFIFOADD_ADDR_ADDR_1296          ((uint16_t)0x000000A2U)         /* !< 1296.0 */
#define USB_IDXTXFIFOADD_ADDR_ADDR_1304          ((uint16_t)0x000000A3U)         /* !< 1304.0 */
#define USB_IDXTXFIFOADD_ADDR_ADDR_1312          ((uint16_t)0x000000A4U)         /* !< 1312.0 */
#define USB_IDXTXFIFOADD_ADDR_ADDR_1320          ((uint16_t)0x000000A5U)         /* !< 1320.0 */
#define USB_IDXTXFIFOADD_ADDR_ADDR_1328          ((uint16_t)0x000000A6U)         /* !< 1328.0 */
#define USB_IDXTXFIFOADD_ADDR_ADDR_1336          ((uint16_t)0x000000A7U)         /* !< 1336.0 */
#define USB_IDXTXFIFOADD_ADDR_ADDR_1344          ((uint16_t)0x000000A8U)         /* !< 1344.0 */
#define USB_IDXTXFIFOADD_ADDR_ADDR_1352          ((uint16_t)0x000000A9U)         /* !< 1352.0 */
#define USB_IDXTXFIFOADD_ADDR_ADDR_1360          ((uint16_t)0x000000AAU)         /* !< 1360.0 */
#define USB_IDXTXFIFOADD_ADDR_ADDR_1368          ((uint16_t)0x000000ABU)         /* !< 1368.0 */
#define USB_IDXTXFIFOADD_ADDR_ADDR_1376          ((uint16_t)0x000000ACU)         /* !< 1376.0 */
#define USB_IDXTXFIFOADD_ADDR_ADDR_1384          ((uint16_t)0x000000ADU)         /* !< 1384.0 */
#define USB_IDXTXFIFOADD_ADDR_ADDR_1392          ((uint16_t)0x000000AEU)         /* !< 1392.0 */
#define USB_IDXTXFIFOADD_ADDR_ADDR_1400          ((uint16_t)0x000000AFU)         /* !< 1400.0 */
#define USB_IDXTXFIFOADD_ADDR_ADDR_1408          ((uint16_t)0x000000B0U)         /* !< 1408.0 */
#define USB_IDXTXFIFOADD_ADDR_ADDR_1416          ((uint16_t)0x000000B1U)         /* !< 1416.0 */
#define USB_IDXTXFIFOADD_ADDR_ADDR_1424          ((uint16_t)0x000000B2U)         /* !< 1424.0 */
#define USB_IDXTXFIFOADD_ADDR_ADDR_1432          ((uint16_t)0x000000B3U)         /* !< 1432.0 */
#define USB_IDXTXFIFOADD_ADDR_ADDR_1440          ((uint16_t)0x000000B4U)         /* !< 1440.0 */
#define USB_IDXTXFIFOADD_ADDR_ADDR_1448          ((uint16_t)0x000000B5U)         /* !< 1448.0 */
#define USB_IDXTXFIFOADD_ADDR_ADDR_1456          ((uint16_t)0x000000B6U)         /* !< 1456.0 */
#define USB_IDXTXFIFOADD_ADDR_ADDR_1464          ((uint16_t)0x000000B7U)         /* !< 1464.0 */
#define USB_IDXTXFIFOADD_ADDR_ADDR_1472          ((uint16_t)0x000000B8U)         /* !< 1472.0 */
#define USB_IDXTXFIFOADD_ADDR_ADDR_1480          ((uint16_t)0x000000B9U)         /* !< 1480.0 */
#define USB_IDXTXFIFOADD_ADDR_ADDR_1488          ((uint16_t)0x000000BAU)         /* !< 1488.0 */
#define USB_IDXTXFIFOADD_ADDR_ADDR_1496          ((uint16_t)0x000000BBU)         /* !< 1496.0 */
#define USB_IDXTXFIFOADD_ADDR_ADDR_1504          ((uint16_t)0x000000BCU)         /* !< 1504.0 */
#define USB_IDXTXFIFOADD_ADDR_ADDR_1512          ((uint16_t)0x000000BDU)         /* !< 1512.0 */
#define USB_IDXTXFIFOADD_ADDR_ADDR_1520          ((uint16_t)0x000000BEU)         /* !< 1520.0 */
#define USB_IDXTXFIFOADD_ADDR_ADDR_1528          ((uint16_t)0x000000BFU)         /* !< 1528.0 */
#define USB_IDXTXFIFOADD_ADDR_ADDR_1536          ((uint16_t)0x000000C0U)         /* !< 1536.0 */
#define USB_IDXTXFIFOADD_ADDR_ADDR_1544          ((uint16_t)0x000000C1U)         /* !< 1544.0 */
#define USB_IDXTXFIFOADD_ADDR_ADDR_1552          ((uint16_t)0x000000C2U)         /* !< 1552.0 */
#define USB_IDXTXFIFOADD_ADDR_ADDR_1560          ((uint16_t)0x000000C3U)         /* !< 1560.0 */
#define USB_IDXTXFIFOADD_ADDR_ADDR_1568          ((uint16_t)0x000000C4U)         /* !< 1568.0 */
#define USB_IDXTXFIFOADD_ADDR_ADDR_1576          ((uint16_t)0x000000C5U)         /* !< 1576.0 */
#define USB_IDXTXFIFOADD_ADDR_ADDR_1584          ((uint16_t)0x000000C6U)         /* !< 1584.0 */
#define USB_IDXTXFIFOADD_ADDR_ADDR_1592          ((uint16_t)0x000000C7U)         /* !< 1592.0 */
#define USB_IDXTXFIFOADD_ADDR_ADDR_1600          ((uint16_t)0x000000C8U)         /* !< 1600.0 */
#define USB_IDXTXFIFOADD_ADDR_ADDR_1608          ((uint16_t)0x000000C9U)         /* !< 1608.0 */
#define USB_IDXTXFIFOADD_ADDR_ADDR_1616          ((uint16_t)0x000000CAU)         /* !< 1616.0 */
#define USB_IDXTXFIFOADD_ADDR_ADDR_1624          ((uint16_t)0x000000CBU)         /* !< 1624.0 */
#define USB_IDXTXFIFOADD_ADDR_ADDR_1632          ((uint16_t)0x000000CCU)         /* !< 1632.0 */
#define USB_IDXTXFIFOADD_ADDR_ADDR_1640          ((uint16_t)0x000000CDU)         /* !< 1640.0 */
#define USB_IDXTXFIFOADD_ADDR_ADDR_1648          ((uint16_t)0x000000CEU)         /* !< 1648.0 */
#define USB_IDXTXFIFOADD_ADDR_ADDR_1656          ((uint16_t)0x000000CFU)         /* !< 1656.0 */
#define USB_IDXTXFIFOADD_ADDR_ADDR_1664          ((uint16_t)0x000000D0U)         /* !< 1664.0 */
#define USB_IDXTXFIFOADD_ADDR_ADDR_1672          ((uint16_t)0x000000D1U)         /* !< 1672.0 */
#define USB_IDXTXFIFOADD_ADDR_ADDR_1680          ((uint16_t)0x000000D2U)         /* !< 1680.0 */
#define USB_IDXTXFIFOADD_ADDR_ADDR_1688          ((uint16_t)0x000000D3U)         /* !< 1688.0 */
#define USB_IDXTXFIFOADD_ADDR_ADDR_1696          ((uint16_t)0x000000D4U)         /* !< 1696.0 */
#define USB_IDXTXFIFOADD_ADDR_ADDR_1704          ((uint16_t)0x000000D5U)         /* !< 1704.0 */
#define USB_IDXTXFIFOADD_ADDR_ADDR_1712          ((uint16_t)0x000000D6U)         /* !< 1712.0 */
#define USB_IDXTXFIFOADD_ADDR_ADDR_1720          ((uint16_t)0x000000D7U)         /* !< 1720.0 */
#define USB_IDXTXFIFOADD_ADDR_ADDR_1728          ((uint16_t)0x000000D8U)         /* !< 1728.0 */
#define USB_IDXTXFIFOADD_ADDR_ADDR_1736          ((uint16_t)0x000000D9U)         /* !< 1736.0 */
#define USB_IDXTXFIFOADD_ADDR_ADDR_1744          ((uint16_t)0x000000DAU)         /* !< 1744.0 */
#define USB_IDXTXFIFOADD_ADDR_ADDR_1752          ((uint16_t)0x000000DBU)         /* !< 1752.0 */
#define USB_IDXTXFIFOADD_ADDR_ADDR_1760          ((uint16_t)0x000000DCU)         /* !< 1760.0 */
#define USB_IDXTXFIFOADD_ADDR_ADDR_1768          ((uint16_t)0x000000DDU)         /* !< 1768.0 */
#define USB_IDXTXFIFOADD_ADDR_ADDR_1776          ((uint16_t)0x000000DEU)         /* !< 1776.0 */
#define USB_IDXTXFIFOADD_ADDR_ADDR_1784          ((uint16_t)0x000000DFU)         /* !< 1784.0 */
#define USB_IDXTXFIFOADD_ADDR_ADDR_1792          ((uint16_t)0x000000E0U)         /* !< 1792.0 */
#define USB_IDXTXFIFOADD_ADDR_ADDR_1800          ((uint16_t)0x000000E1U)         /* !< 1800.0 */
#define USB_IDXTXFIFOADD_ADDR_ADDR_1808          ((uint16_t)0x000000E2U)         /* !< 1808.0 */
#define USB_IDXTXFIFOADD_ADDR_ADDR_1816          ((uint16_t)0x000000E3U)         /* !< 1816.0 */
#define USB_IDXTXFIFOADD_ADDR_ADDR_1824          ((uint16_t)0x000000E4U)         /* !< 1824.0 */
#define USB_IDXTXFIFOADD_ADDR_ADDR_1832          ((uint16_t)0x000000E5U)         /* !< 1832.0 */
#define USB_IDXTXFIFOADD_ADDR_ADDR_1840          ((uint16_t)0x000000E6U)         /* !< 1840.0 */
#define USB_IDXTXFIFOADD_ADDR_ADDR_1848          ((uint16_t)0x000000E7U)         /* !< 1848.0 */
#define USB_IDXTXFIFOADD_ADDR_ADDR_1856          ((uint16_t)0x000000E8U)         /* !< 1856.0 */
#define USB_IDXTXFIFOADD_ADDR_ADDR_1864          ((uint16_t)0x000000E9U)         /* !< 1864.0 */
#define USB_IDXTXFIFOADD_ADDR_ADDR_1872          ((uint16_t)0x000000EAU)         /* !< 1872.0 */
#define USB_IDXTXFIFOADD_ADDR_ADDR_1880          ((uint16_t)0x000000EBU)         /* !< 1880.0 */
#define USB_IDXTXFIFOADD_ADDR_ADDR_1888          ((uint16_t)0x000000ECU)         /* !< 1888.0 */
#define USB_IDXTXFIFOADD_ADDR_ADDR_1896          ((uint16_t)0x000000EDU)         /* !< 1896.0 */
#define USB_IDXTXFIFOADD_ADDR_ADDR_1904          ((uint16_t)0x000000EEU)         /* !< 1904.0 */
#define USB_IDXTXFIFOADD_ADDR_ADDR_1912          ((uint16_t)0x000000EFU)         /* !< 1912.0 */
#define USB_IDXTXFIFOADD_ADDR_ADDR_1920          ((uint16_t)0x000000F0U)         /* !< 1920.0 */
#define USB_IDXTXFIFOADD_ADDR_ADDR_1928          ((uint16_t)0x000000F1U)         /* !< 1928.0 */
#define USB_IDXTXFIFOADD_ADDR_ADDR_1936          ((uint16_t)0x000000F2U)         /* !< 1936.0 */
#define USB_IDXTXFIFOADD_ADDR_ADDR_1944          ((uint16_t)0x000000F3U)         /* !< 1944.0 */
#define USB_IDXTXFIFOADD_ADDR_ADDR_1952          ((uint16_t)0x000000F4U)         /* !< 1952.0 */
#define USB_IDXTXFIFOADD_ADDR_ADDR_1960          ((uint16_t)0x000000F5U)         /* !< 1960.0 */
#define USB_IDXTXFIFOADD_ADDR_ADDR_1968          ((uint16_t)0x000000F6U)         /* !< 1968.0 */
#define USB_IDXTXFIFOADD_ADDR_ADDR_1976          ((uint16_t)0x000000F7U)         /* !< 1976.0 */
#define USB_IDXTXFIFOADD_ADDR_ADDR_1984          ((uint16_t)0x000000F8U)         /* !< 1984.0 */
#define USB_IDXTXFIFOADD_ADDR_ADDR_1992          ((uint16_t)0x000000F9U)         /* !< 1992.0 */
#define USB_IDXTXFIFOADD_ADDR_ADDR_2000          ((uint16_t)0x000000FAU)         /* !< 2000.0 */
#define USB_IDXTXFIFOADD_ADDR_ADDR_2008          ((uint16_t)0x000000FBU)         /* !< 2008.0 */
#define USB_IDXTXFIFOADD_ADDR_ADDR_2016          ((uint16_t)0x000000FCU)         /* !< 2016.0 */
#define USB_IDXTXFIFOADD_ADDR_ADDR_2024          ((uint16_t)0x000000FDU)         /* !< 2024.0 */
#define USB_IDXTXFIFOADD_ADDR_ADDR_2032          ((uint16_t)0x000000FEU)         /* !< 2032.0 */
#define USB_IDXTXFIFOADD_ADDR_ADDR_2040          ((uint16_t)0x000000FFU)         /* !< 2040.0 */

/* USB_IDXRXFIFOADD Bits */
/* USB_IDXRXFIFOADD[ADDR] Bits */
#define USB_IDXRXFIFOADD_ADDR_OFS                (0)                             /* !< ADDR Offset */
#define USB_IDXRXFIFOADD_ADDR_MASK               ((uint16_t)0x00003FFFU)         /* !< Endpoint Data */
#define USB_IDXRXFIFOADD_ADDR_ADDR_0             ((uint16_t)0x00000000U)         /* !< 0.0 */
#define USB_IDXRXFIFOADD_ADDR_ADDR_8             ((uint16_t)0x00000001U)         /* !< 8.0 */
#define USB_IDXRXFIFOADD_ADDR_ADDR_16            ((uint16_t)0x00000002U)         /* !< 16.0 */
#define USB_IDXRXFIFOADD_ADDR_ADDR_24            ((uint16_t)0x00000003U)         /* !< 24.0 */
#define USB_IDXRXFIFOADD_ADDR_ADDR_32            ((uint16_t)0x00000004U)         /* !< 32.0 */
#define USB_IDXRXFIFOADD_ADDR_ADDR_40            ((uint16_t)0x00000005U)         /* !< 40.0 */
#define USB_IDXRXFIFOADD_ADDR_ADDR_48            ((uint16_t)0x00000006U)         /* !< 48.0 */
#define USB_IDXRXFIFOADD_ADDR_ADDR_56            ((uint16_t)0x00000007U)         /* !< 56.0 */
#define USB_IDXRXFIFOADD_ADDR_ADDR_64            ((uint16_t)0x00000008U)         /* !< 64.0 */
#define USB_IDXRXFIFOADD_ADDR_ADDR_72            ((uint16_t)0x00000009U)         /* !< 72.0 */
#define USB_IDXRXFIFOADD_ADDR_ADDR_80            ((uint16_t)0x0000000AU)         /* !< 80.0 */
#define USB_IDXRXFIFOADD_ADDR_ADDR_88            ((uint16_t)0x0000000BU)         /* !< 88.0 */
#define USB_IDXRXFIFOADD_ADDR_ADDR_96            ((uint16_t)0x0000000CU)         /* !< 96.0 */
#define USB_IDXRXFIFOADD_ADDR_ADDR_104           ((uint16_t)0x0000000DU)         /* !< 104.0 */
#define USB_IDXRXFIFOADD_ADDR_ADDR_112           ((uint16_t)0x0000000EU)         /* !< 112.0 */
#define USB_IDXRXFIFOADD_ADDR_ADDR_120           ((uint16_t)0x0000000FU)         /* !< 120.0 */
#define USB_IDXRXFIFOADD_ADDR_ADDR_128           ((uint16_t)0x00000010U)         /* !< 128.0 */
#define USB_IDXRXFIFOADD_ADDR_ADDR_136           ((uint16_t)0x00000011U)         /* !< 136.0 */
#define USB_IDXRXFIFOADD_ADDR_ADDR_144           ((uint16_t)0x00000012U)         /* !< 144.0 */
#define USB_IDXRXFIFOADD_ADDR_ADDR_152           ((uint16_t)0x00000013U)         /* !< 152.0 */
#define USB_IDXRXFIFOADD_ADDR_ADDR_160           ((uint16_t)0x00000014U)         /* !< 160.0 */
#define USB_IDXRXFIFOADD_ADDR_ADDR_168           ((uint16_t)0x00000015U)         /* !< 168.0 */
#define USB_IDXRXFIFOADD_ADDR_ADDR_176           ((uint16_t)0x00000016U)         /* !< 176.0 */
#define USB_IDXRXFIFOADD_ADDR_ADDR_184           ((uint16_t)0x00000017U)         /* !< 184.0 */
#define USB_IDXRXFIFOADD_ADDR_ADDR_192           ((uint16_t)0x00000018U)         /* !< 192.0 */
#define USB_IDXRXFIFOADD_ADDR_ADDR_200           ((uint16_t)0x00000019U)         /* !< 200.0 */
#define USB_IDXRXFIFOADD_ADDR_ADDR_208           ((uint16_t)0x0000001AU)         /* !< 208.0 */
#define USB_IDXRXFIFOADD_ADDR_ADDR_216           ((uint16_t)0x0000001BU)         /* !< 216.0 */
#define USB_IDXRXFIFOADD_ADDR_ADDR_224           ((uint16_t)0x0000001CU)         /* !< 224.0 */
#define USB_IDXRXFIFOADD_ADDR_ADDR_232           ((uint16_t)0x0000001DU)         /* !< 232.0 */
#define USB_IDXRXFIFOADD_ADDR_ADDR_240           ((uint16_t)0x0000001EU)         /* !< 240.0 */
#define USB_IDXRXFIFOADD_ADDR_ADDR_248           ((uint16_t)0x0000001FU)         /* !< 248.0 */
#define USB_IDXRXFIFOADD_ADDR_ADDR_256           ((uint16_t)0x00000020U)         /* !< 256.0 */
#define USB_IDXRXFIFOADD_ADDR_ADDR_264           ((uint16_t)0x00000021U)         /* !< 264.0 */
#define USB_IDXRXFIFOADD_ADDR_ADDR_272           ((uint16_t)0x00000022U)         /* !< 272.0 */
#define USB_IDXRXFIFOADD_ADDR_ADDR_280           ((uint16_t)0x00000023U)         /* !< 280.0 */
#define USB_IDXRXFIFOADD_ADDR_ADDR_288           ((uint16_t)0x00000024U)         /* !< 288.0 */
#define USB_IDXRXFIFOADD_ADDR_ADDR_296           ((uint16_t)0x00000025U)         /* !< 296.0 */
#define USB_IDXRXFIFOADD_ADDR_ADDR_304           ((uint16_t)0x00000026U)         /* !< 304.0 */
#define USB_IDXRXFIFOADD_ADDR_ADDR_312           ((uint16_t)0x00000027U)         /* !< 312.0 */
#define USB_IDXRXFIFOADD_ADDR_ADDR_320           ((uint16_t)0x00000028U)         /* !< 320.0 */
#define USB_IDXRXFIFOADD_ADDR_ADDR_328           ((uint16_t)0x00000029U)         /* !< 328.0 */
#define USB_IDXRXFIFOADD_ADDR_ADDR_336           ((uint16_t)0x0000002AU)         /* !< 336.0 */
#define USB_IDXRXFIFOADD_ADDR_ADDR_344           ((uint16_t)0x0000002BU)         /* !< 344.0 */
#define USB_IDXRXFIFOADD_ADDR_ADDR_352           ((uint16_t)0x0000002CU)         /* !< 352.0 */
#define USB_IDXRXFIFOADD_ADDR_ADDR_360           ((uint16_t)0x0000002DU)         /* !< 360.0 */
#define USB_IDXRXFIFOADD_ADDR_ADDR_368           ((uint16_t)0x0000002EU)         /* !< 368.0 */
#define USB_IDXRXFIFOADD_ADDR_ADDR_376           ((uint16_t)0x0000002FU)         /* !< 376.0 */
#define USB_IDXRXFIFOADD_ADDR_ADDR_384           ((uint16_t)0x00000030U)         /* !< 384.0 */
#define USB_IDXRXFIFOADD_ADDR_ADDR_392           ((uint16_t)0x00000031U)         /* !< 392.0 */
#define USB_IDXRXFIFOADD_ADDR_ADDR_400           ((uint16_t)0x00000032U)         /* !< 400.0 */
#define USB_IDXRXFIFOADD_ADDR_ADDR_408           ((uint16_t)0x00000033U)         /* !< 408.0 */
#define USB_IDXRXFIFOADD_ADDR_ADDR_416           ((uint16_t)0x00000034U)         /* !< 416.0 */
#define USB_IDXRXFIFOADD_ADDR_ADDR_424           ((uint16_t)0x00000035U)         /* !< 424.0 */
#define USB_IDXRXFIFOADD_ADDR_ADDR_432           ((uint16_t)0x00000036U)         /* !< 432.0 */
#define USB_IDXRXFIFOADD_ADDR_ADDR_440           ((uint16_t)0x00000037U)         /* !< 440.0 */
#define USB_IDXRXFIFOADD_ADDR_ADDR_448           ((uint16_t)0x00000038U)         /* !< 448.0 */
#define USB_IDXRXFIFOADD_ADDR_ADDR_456           ((uint16_t)0x00000039U)         /* !< 456.0 */
#define USB_IDXRXFIFOADD_ADDR_ADDR_464           ((uint16_t)0x0000003AU)         /* !< 464.0 */
#define USB_IDXRXFIFOADD_ADDR_ADDR_472           ((uint16_t)0x0000003BU)         /* !< 472.0 */
#define USB_IDXRXFIFOADD_ADDR_ADDR_480           ((uint16_t)0x0000003CU)         /* !< 480.0 */
#define USB_IDXRXFIFOADD_ADDR_ADDR_488           ((uint16_t)0x0000003DU)         /* !< 488.0 */
#define USB_IDXRXFIFOADD_ADDR_ADDR_496           ((uint16_t)0x0000003EU)         /* !< 496.0 */
#define USB_IDXRXFIFOADD_ADDR_ADDR_504           ((uint16_t)0x0000003FU)         /* !< 504.0 */
#define USB_IDXRXFIFOADD_ADDR_ADDR_512           ((uint16_t)0x00000040U)         /* !< 512.0 */
#define USB_IDXRXFIFOADD_ADDR_ADDR_520           ((uint16_t)0x00000041U)         /* !< 520.0 */
#define USB_IDXRXFIFOADD_ADDR_ADDR_528           ((uint16_t)0x00000042U)         /* !< 528.0 */
#define USB_IDXRXFIFOADD_ADDR_ADDR_536           ((uint16_t)0x00000043U)         /* !< 536.0 */
#define USB_IDXRXFIFOADD_ADDR_ADDR_544           ((uint16_t)0x00000044U)         /* !< 544.0 */
#define USB_IDXRXFIFOADD_ADDR_ADDR_552           ((uint16_t)0x00000045U)         /* !< 552.0 */
#define USB_IDXRXFIFOADD_ADDR_ADDR_560           ((uint16_t)0x00000046U)         /* !< 560.0 */
#define USB_IDXRXFIFOADD_ADDR_ADDR_568           ((uint16_t)0x00000047U)         /* !< 568.0 */
#define USB_IDXRXFIFOADD_ADDR_ADDR_576           ((uint16_t)0x00000048U)         /* !< 576.0 */
#define USB_IDXRXFIFOADD_ADDR_ADDR_584           ((uint16_t)0x00000049U)         /* !< 584.0 */
#define USB_IDXRXFIFOADD_ADDR_ADDR_592           ((uint16_t)0x0000004AU)         /* !< 592.0 */
#define USB_IDXRXFIFOADD_ADDR_ADDR_600           ((uint16_t)0x0000004BU)         /* !< 600.0 */
#define USB_IDXRXFIFOADD_ADDR_ADDR_608           ((uint16_t)0x0000004CU)         /* !< 608.0 */
#define USB_IDXRXFIFOADD_ADDR_ADDR_616           ((uint16_t)0x0000004DU)         /* !< 616.0 */
#define USB_IDXRXFIFOADD_ADDR_ADDR_624           ((uint16_t)0x0000004EU)         /* !< 624.0 */
#define USB_IDXRXFIFOADD_ADDR_ADDR_632           ((uint16_t)0x0000004FU)         /* !< 632.0 */
#define USB_IDXRXFIFOADD_ADDR_ADDR_640           ((uint16_t)0x00000050U)         /* !< 640.0 */
#define USB_IDXRXFIFOADD_ADDR_ADDR_648           ((uint16_t)0x00000051U)         /* !< 648.0 */
#define USB_IDXRXFIFOADD_ADDR_ADDR_656           ((uint16_t)0x00000052U)         /* !< 656.0 */
#define USB_IDXRXFIFOADD_ADDR_ADDR_664           ((uint16_t)0x00000053U)         /* !< 664.0 */
#define USB_IDXRXFIFOADD_ADDR_ADDR_672           ((uint16_t)0x00000054U)         /* !< 672.0 */
#define USB_IDXRXFIFOADD_ADDR_ADDR_680           ((uint16_t)0x00000055U)         /* !< 680.0 */
#define USB_IDXRXFIFOADD_ADDR_ADDR_688           ((uint16_t)0x00000056U)         /* !< 688.0 */
#define USB_IDXRXFIFOADD_ADDR_ADDR_696           ((uint16_t)0x00000057U)         /* !< 696.0 */
#define USB_IDXRXFIFOADD_ADDR_ADDR_704           ((uint16_t)0x00000058U)         /* !< 704.0 */
#define USB_IDXRXFIFOADD_ADDR_ADDR_712           ((uint16_t)0x00000059U)         /* !< 712.0 */
#define USB_IDXRXFIFOADD_ADDR_ADDR_720           ((uint16_t)0x0000005AU)         /* !< 720.0 */
#define USB_IDXRXFIFOADD_ADDR_ADDR_728           ((uint16_t)0x0000005BU)         /* !< 728.0 */
#define USB_IDXRXFIFOADD_ADDR_ADDR_736           ((uint16_t)0x0000005CU)         /* !< 736.0 */
#define USB_IDXRXFIFOADD_ADDR_ADDR_744           ((uint16_t)0x0000005DU)         /* !< 744.0 */
#define USB_IDXRXFIFOADD_ADDR_ADDR_752           ((uint16_t)0x0000005EU)         /* !< 752.0 */
#define USB_IDXRXFIFOADD_ADDR_ADDR_760           ((uint16_t)0x0000005FU)         /* !< 760.0 */
#define USB_IDXRXFIFOADD_ADDR_ADDR_768           ((uint16_t)0x00000060U)         /* !< 768.0 */
#define USB_IDXRXFIFOADD_ADDR_ADDR_776           ((uint16_t)0x00000061U)         /* !< 776.0 */
#define USB_IDXRXFIFOADD_ADDR_ADDR_784           ((uint16_t)0x00000062U)         /* !< 784.0 */
#define USB_IDXRXFIFOADD_ADDR_ADDR_792           ((uint16_t)0x00000063U)         /* !< 792.0 */
#define USB_IDXRXFIFOADD_ADDR_ADDR_800           ((uint16_t)0x00000064U)         /* !< 800.0 */
#define USB_IDXRXFIFOADD_ADDR_ADDR_808           ((uint16_t)0x00000065U)         /* !< 808.0 */
#define USB_IDXRXFIFOADD_ADDR_ADDR_816           ((uint16_t)0x00000066U)         /* !< 816.0 */
#define USB_IDXRXFIFOADD_ADDR_ADDR_824           ((uint16_t)0x00000067U)         /* !< 824.0 */
#define USB_IDXRXFIFOADD_ADDR_ADDR_832           ((uint16_t)0x00000068U)         /* !< 832.0 */
#define USB_IDXRXFIFOADD_ADDR_ADDR_840           ((uint16_t)0x00000069U)         /* !< 840.0 */
#define USB_IDXRXFIFOADD_ADDR_ADDR_848           ((uint16_t)0x0000006AU)         /* !< 848.0 */
#define USB_IDXRXFIFOADD_ADDR_ADDR_856           ((uint16_t)0x0000006BU)         /* !< 856.0 */
#define USB_IDXRXFIFOADD_ADDR_ADDR_864           ((uint16_t)0x0000006CU)         /* !< 864.0 */
#define USB_IDXRXFIFOADD_ADDR_ADDR_872           ((uint16_t)0x0000006DU)         /* !< 872.0 */
#define USB_IDXRXFIFOADD_ADDR_ADDR_880           ((uint16_t)0x0000006EU)         /* !< 880.0 */
#define USB_IDXRXFIFOADD_ADDR_ADDR_888           ((uint16_t)0x0000006FU)         /* !< 888.0 */
#define USB_IDXRXFIFOADD_ADDR_ADDR_896           ((uint16_t)0x00000070U)         /* !< 896.0 */
#define USB_IDXRXFIFOADD_ADDR_ADDR_904           ((uint16_t)0x00000071U)         /* !< 904.0 */
#define USB_IDXRXFIFOADD_ADDR_ADDR_912           ((uint16_t)0x00000072U)         /* !< 912.0 */
#define USB_IDXRXFIFOADD_ADDR_ADDR_920           ((uint16_t)0x00000073U)         /* !< 920.0 */
#define USB_IDXRXFIFOADD_ADDR_ADDR_928           ((uint16_t)0x00000074U)         /* !< 928.0 */
#define USB_IDXRXFIFOADD_ADDR_ADDR_936           ((uint16_t)0x00000075U)         /* !< 936.0 */
#define USB_IDXRXFIFOADD_ADDR_ADDR_944           ((uint16_t)0x00000076U)         /* !< 944.0 */
#define USB_IDXRXFIFOADD_ADDR_ADDR_952           ((uint16_t)0x00000077U)         /* !< 952.0 */
#define USB_IDXRXFIFOADD_ADDR_ADDR_960           ((uint16_t)0x00000078U)         /* !< 960.0 */
#define USB_IDXRXFIFOADD_ADDR_ADDR_968           ((uint16_t)0x00000079U)         /* !< 968.0 */
#define USB_IDXRXFIFOADD_ADDR_ADDR_976           ((uint16_t)0x0000007AU)         /* !< 976.0 */
#define USB_IDXRXFIFOADD_ADDR_ADDR_984           ((uint16_t)0x0000007BU)         /* !< 984.0 */
#define USB_IDXRXFIFOADD_ADDR_ADDR_992           ((uint16_t)0x0000007CU)         /* !< 992.0 */
#define USB_IDXRXFIFOADD_ADDR_ADDR_1000          ((uint16_t)0x0000007DU)         /* !< 1000.0 */
#define USB_IDXRXFIFOADD_ADDR_ADDR_1008          ((uint16_t)0x0000007EU)         /* !< 1008.0 */
#define USB_IDXRXFIFOADD_ADDR_ADDR_1016          ((uint16_t)0x0000007FU)         /* !< 1016.0 */
#define USB_IDXRXFIFOADD_ADDR_ADDR_1024          ((uint16_t)0x00000080U)         /* !< 1024.0 */
#define USB_IDXRXFIFOADD_ADDR_ADDR_1032          ((uint16_t)0x00000081U)         /* !< 1032.0 */
#define USB_IDXRXFIFOADD_ADDR_ADDR_1040          ((uint16_t)0x00000082U)         /* !< 1040.0 */
#define USB_IDXRXFIFOADD_ADDR_ADDR_1048          ((uint16_t)0x00000083U)         /* !< 1048.0 */
#define USB_IDXRXFIFOADD_ADDR_ADDR_1056          ((uint16_t)0x00000084U)         /* !< 1056.0 */
#define USB_IDXRXFIFOADD_ADDR_ADDR_1064          ((uint16_t)0x00000085U)         /* !< 1064.0 */
#define USB_IDXRXFIFOADD_ADDR_ADDR_1072          ((uint16_t)0x00000086U)         /* !< 1072.0 */
#define USB_IDXRXFIFOADD_ADDR_ADDR_1080          ((uint16_t)0x00000087U)         /* !< 1080.0 */
#define USB_IDXRXFIFOADD_ADDR_ADDR_1088          ((uint16_t)0x00000088U)         /* !< 1088.0 */
#define USB_IDXRXFIFOADD_ADDR_ADDR_1096          ((uint16_t)0x00000089U)         /* !< 1096.0 */
#define USB_IDXRXFIFOADD_ADDR_ADDR_1104          ((uint16_t)0x0000008AU)         /* !< 1104.0 */
#define USB_IDXRXFIFOADD_ADDR_ADDR_1112          ((uint16_t)0x0000008BU)         /* !< 1112.0 */
#define USB_IDXRXFIFOADD_ADDR_ADDR_1120          ((uint16_t)0x0000008CU)         /* !< 1120.0 */
#define USB_IDXRXFIFOADD_ADDR_ADDR_1128          ((uint16_t)0x0000008DU)         /* !< 1128.0 */
#define USB_IDXRXFIFOADD_ADDR_ADDR_1136          ((uint16_t)0x0000008EU)         /* !< 1136.0 */
#define USB_IDXRXFIFOADD_ADDR_ADDR_1144          ((uint16_t)0x0000008FU)         /* !< 1144.0 */
#define USB_IDXRXFIFOADD_ADDR_ADDR_1152          ((uint16_t)0x00000090U)         /* !< 1152.0 */
#define USB_IDXRXFIFOADD_ADDR_ADDR_1160          ((uint16_t)0x00000091U)         /* !< 1160.0 */
#define USB_IDXRXFIFOADD_ADDR_ADDR_1168          ((uint16_t)0x00000092U)         /* !< 1168.0 */
#define USB_IDXRXFIFOADD_ADDR_ADDR_1176          ((uint16_t)0x00000093U)         /* !< 1176.0 */
#define USB_IDXRXFIFOADD_ADDR_ADDR_1184          ((uint16_t)0x00000094U)         /* !< 1184.0 */
#define USB_IDXRXFIFOADD_ADDR_ADDR_1192          ((uint16_t)0x00000095U)         /* !< 1192.0 */
#define USB_IDXRXFIFOADD_ADDR_ADDR_1200          ((uint16_t)0x00000096U)         /* !< 1200.0 */
#define USB_IDXRXFIFOADD_ADDR_ADDR_1208          ((uint16_t)0x00000097U)         /* !< 1208.0 */
#define USB_IDXRXFIFOADD_ADDR_ADDR_1216          ((uint16_t)0x00000098U)         /* !< 1216.0 */
#define USB_IDXRXFIFOADD_ADDR_ADDR_1224          ((uint16_t)0x00000099U)         /* !< 1224.0 */
#define USB_IDXRXFIFOADD_ADDR_ADDR_1232          ((uint16_t)0x0000009AU)         /* !< 1232.0 */
#define USB_IDXRXFIFOADD_ADDR_ADDR_1240          ((uint16_t)0x0000009BU)         /* !< 1240.0 */
#define USB_IDXRXFIFOADD_ADDR_ADDR_1248          ((uint16_t)0x0000009CU)         /* !< 1248.0 */
#define USB_IDXRXFIFOADD_ADDR_ADDR_1256          ((uint16_t)0x0000009DU)         /* !< 1256.0 */
#define USB_IDXRXFIFOADD_ADDR_ADDR_1264          ((uint16_t)0x0000009EU)         /* !< 1264.0 */
#define USB_IDXRXFIFOADD_ADDR_ADDR_1272          ((uint16_t)0x0000009FU)         /* !< 1272.0 */
#define USB_IDXRXFIFOADD_ADDR_ADDR_1280          ((uint16_t)0x000000A0U)         /* !< 1280.0 */
#define USB_IDXRXFIFOADD_ADDR_ADDR_1288          ((uint16_t)0x000000A1U)         /* !< 1288.0 */
#define USB_IDXRXFIFOADD_ADDR_ADDR_1296          ((uint16_t)0x000000A2U)         /* !< 1296.0 */
#define USB_IDXRXFIFOADD_ADDR_ADDR_1304          ((uint16_t)0x000000A3U)         /* !< 1304.0 */
#define USB_IDXRXFIFOADD_ADDR_ADDR_1312          ((uint16_t)0x000000A4U)         /* !< 1312.0 */
#define USB_IDXRXFIFOADD_ADDR_ADDR_1320          ((uint16_t)0x000000A5U)         /* !< 1320.0 */
#define USB_IDXRXFIFOADD_ADDR_ADDR_1328          ((uint16_t)0x000000A6U)         /* !< 1328.0 */
#define USB_IDXRXFIFOADD_ADDR_ADDR_1336          ((uint16_t)0x000000A7U)         /* !< 1336.0 */
#define USB_IDXRXFIFOADD_ADDR_ADDR_1344          ((uint16_t)0x000000A8U)         /* !< 1344.0 */
#define USB_IDXRXFIFOADD_ADDR_ADDR_1352          ((uint16_t)0x000000A9U)         /* !< 1352.0 */
#define USB_IDXRXFIFOADD_ADDR_ADDR_1360          ((uint16_t)0x000000AAU)         /* !< 1360.0 */
#define USB_IDXRXFIFOADD_ADDR_ADDR_1368          ((uint16_t)0x000000ABU)         /* !< 1368.0 */
#define USB_IDXRXFIFOADD_ADDR_ADDR_1376          ((uint16_t)0x000000ACU)         /* !< 1376.0 */
#define USB_IDXRXFIFOADD_ADDR_ADDR_1384          ((uint16_t)0x000000ADU)         /* !< 1384.0 */
#define USB_IDXRXFIFOADD_ADDR_ADDR_1392          ((uint16_t)0x000000AEU)         /* !< 1392.0 */
#define USB_IDXRXFIFOADD_ADDR_ADDR_1400          ((uint16_t)0x000000AFU)         /* !< 1400.0 */
#define USB_IDXRXFIFOADD_ADDR_ADDR_1408          ((uint16_t)0x000000B0U)         /* !< 1408.0 */
#define USB_IDXRXFIFOADD_ADDR_ADDR_1416          ((uint16_t)0x000000B1U)         /* !< 1416.0 */
#define USB_IDXRXFIFOADD_ADDR_ADDR_1424          ((uint16_t)0x000000B2U)         /* !< 1424.0 */
#define USB_IDXRXFIFOADD_ADDR_ADDR_1432          ((uint16_t)0x000000B3U)         /* !< 1432.0 */
#define USB_IDXRXFIFOADD_ADDR_ADDR_1440          ((uint16_t)0x000000B4U)         /* !< 1440.0 */
#define USB_IDXRXFIFOADD_ADDR_ADDR_1448          ((uint16_t)0x000000B5U)         /* !< 1448.0 */
#define USB_IDXRXFIFOADD_ADDR_ADDR_1456          ((uint16_t)0x000000B6U)         /* !< 1456.0 */
#define USB_IDXRXFIFOADD_ADDR_ADDR_1464          ((uint16_t)0x000000B7U)         /* !< 1464.0 */
#define USB_IDXRXFIFOADD_ADDR_ADDR_1472          ((uint16_t)0x000000B8U)         /* !< 1472.0 */
#define USB_IDXRXFIFOADD_ADDR_ADDR_1480          ((uint16_t)0x000000B9U)         /* !< 1480.0 */
#define USB_IDXRXFIFOADD_ADDR_ADDR_1488          ((uint16_t)0x000000BAU)         /* !< 1488.0 */
#define USB_IDXRXFIFOADD_ADDR_ADDR_1496          ((uint16_t)0x000000BBU)         /* !< 1496.0 */
#define USB_IDXRXFIFOADD_ADDR_ADDR_1504          ((uint16_t)0x000000BCU)         /* !< 1504.0 */
#define USB_IDXRXFIFOADD_ADDR_ADDR_1512          ((uint16_t)0x000000BDU)         /* !< 1512.0 */
#define USB_IDXRXFIFOADD_ADDR_ADDR_1520          ((uint16_t)0x000000BEU)         /* !< 1520.0 */
#define USB_IDXRXFIFOADD_ADDR_ADDR_1528          ((uint16_t)0x000000BFU)         /* !< 1528.0 */
#define USB_IDXRXFIFOADD_ADDR_ADDR_1536          ((uint16_t)0x000000C0U)         /* !< 1536.0 */
#define USB_IDXRXFIFOADD_ADDR_ADDR_1544          ((uint16_t)0x000000C1U)         /* !< 1544.0 */
#define USB_IDXRXFIFOADD_ADDR_ADDR_1552          ((uint16_t)0x000000C2U)         /* !< 1552.0 */
#define USB_IDXRXFIFOADD_ADDR_ADDR_1560          ((uint16_t)0x000000C3U)         /* !< 1560.0 */
#define USB_IDXRXFIFOADD_ADDR_ADDR_1568          ((uint16_t)0x000000C4U)         /* !< 1568.0 */
#define USB_IDXRXFIFOADD_ADDR_ADDR_1576          ((uint16_t)0x000000C5U)         /* !< 1576.0 */
#define USB_IDXRXFIFOADD_ADDR_ADDR_1584          ((uint16_t)0x000000C6U)         /* !< 1584.0 */
#define USB_IDXRXFIFOADD_ADDR_ADDR_1592          ((uint16_t)0x000000C7U)         /* !< 1592.0 */
#define USB_IDXRXFIFOADD_ADDR_ADDR_1600          ((uint16_t)0x000000C8U)         /* !< 1600.0 */
#define USB_IDXRXFIFOADD_ADDR_ADDR_1608          ((uint16_t)0x000000C9U)         /* !< 1608.0 */
#define USB_IDXRXFIFOADD_ADDR_ADDR_1616          ((uint16_t)0x000000CAU)         /* !< 1616.0 */
#define USB_IDXRXFIFOADD_ADDR_ADDR_1624          ((uint16_t)0x000000CBU)         /* !< 1624.0 */
#define USB_IDXRXFIFOADD_ADDR_ADDR_1632          ((uint16_t)0x000000CCU)         /* !< 1632.0 */
#define USB_IDXRXFIFOADD_ADDR_ADDR_1640          ((uint16_t)0x000000CDU)         /* !< 1640.0 */
#define USB_IDXRXFIFOADD_ADDR_ADDR_1648          ((uint16_t)0x000000CEU)         /* !< 1648.0 */
#define USB_IDXRXFIFOADD_ADDR_ADDR_1656          ((uint16_t)0x000000CFU)         /* !< 1656.0 */
#define USB_IDXRXFIFOADD_ADDR_ADDR_1664          ((uint16_t)0x000000D0U)         /* !< 1664.0 */
#define USB_IDXRXFIFOADD_ADDR_ADDR_1672          ((uint16_t)0x000000D1U)         /* !< 1672.0 */
#define USB_IDXRXFIFOADD_ADDR_ADDR_1680          ((uint16_t)0x000000D2U)         /* !< 1680.0 */
#define USB_IDXRXFIFOADD_ADDR_ADDR_1688          ((uint16_t)0x000000D3U)         /* !< 1688.0 */
#define USB_IDXRXFIFOADD_ADDR_ADDR_1696          ((uint16_t)0x000000D4U)         /* !< 1696.0 */
#define USB_IDXRXFIFOADD_ADDR_ADDR_1704          ((uint16_t)0x000000D5U)         /* !< 1704.0 */
#define USB_IDXRXFIFOADD_ADDR_ADDR_1712          ((uint16_t)0x000000D6U)         /* !< 1712.0 */
#define USB_IDXRXFIFOADD_ADDR_ADDR_1720          ((uint16_t)0x000000D7U)         /* !< 1720.0 */
#define USB_IDXRXFIFOADD_ADDR_ADDR_1728          ((uint16_t)0x000000D8U)         /* !< 1728.0 */
#define USB_IDXRXFIFOADD_ADDR_ADDR_1736          ((uint16_t)0x000000D9U)         /* !< 1736.0 */
#define USB_IDXRXFIFOADD_ADDR_ADDR_1744          ((uint16_t)0x000000DAU)         /* !< 1744.0 */
#define USB_IDXRXFIFOADD_ADDR_ADDR_1752          ((uint16_t)0x000000DBU)         /* !< 1752.0 */
#define USB_IDXRXFIFOADD_ADDR_ADDR_1760          ((uint16_t)0x000000DCU)         /* !< 1760.0 */
#define USB_IDXRXFIFOADD_ADDR_ADDR_1768          ((uint16_t)0x000000DDU)         /* !< 1768.0 */
#define USB_IDXRXFIFOADD_ADDR_ADDR_1776          ((uint16_t)0x000000DEU)         /* !< 1776.0 */
#define USB_IDXRXFIFOADD_ADDR_ADDR_1784          ((uint16_t)0x000000DFU)         /* !< 1784.0 */
#define USB_IDXRXFIFOADD_ADDR_ADDR_1792          ((uint16_t)0x000000E0U)         /* !< 1792.0 */
#define USB_IDXRXFIFOADD_ADDR_ADDR_1800          ((uint16_t)0x000000E1U)         /* !< 1800.0 */
#define USB_IDXRXFIFOADD_ADDR_ADDR_1808          ((uint16_t)0x000000E2U)         /* !< 1808.0 */
#define USB_IDXRXFIFOADD_ADDR_ADDR_1816          ((uint16_t)0x000000E3U)         /* !< 1816.0 */
#define USB_IDXRXFIFOADD_ADDR_ADDR_1824          ((uint16_t)0x000000E4U)         /* !< 1824.0 */
#define USB_IDXRXFIFOADD_ADDR_ADDR_1832          ((uint16_t)0x000000E5U)         /* !< 1832.0 */
#define USB_IDXRXFIFOADD_ADDR_ADDR_1840          ((uint16_t)0x000000E6U)         /* !< 1840.0 */
#define USB_IDXRXFIFOADD_ADDR_ADDR_1848          ((uint16_t)0x000000E7U)         /* !< 1848.0 */
#define USB_IDXRXFIFOADD_ADDR_ADDR_1856          ((uint16_t)0x000000E8U)         /* !< 1856.0 */
#define USB_IDXRXFIFOADD_ADDR_ADDR_1864          ((uint16_t)0x000000E9U)         /* !< 1864.0 */
#define USB_IDXRXFIFOADD_ADDR_ADDR_1872          ((uint16_t)0x000000EAU)         /* !< 1872.0 */
#define USB_IDXRXFIFOADD_ADDR_ADDR_1880          ((uint16_t)0x000000EBU)         /* !< 1880.0 */
#define USB_IDXRXFIFOADD_ADDR_ADDR_1888          ((uint16_t)0x000000ECU)         /* !< 1888.0 */
#define USB_IDXRXFIFOADD_ADDR_ADDR_1896          ((uint16_t)0x000000EDU)         /* !< 1896.0 */
#define USB_IDXRXFIFOADD_ADDR_ADDR_1904          ((uint16_t)0x000000EEU)         /* !< 1904.0 */
#define USB_IDXRXFIFOADD_ADDR_ADDR_1912          ((uint16_t)0x000000EFU)         /* !< 1912.0 */
#define USB_IDXRXFIFOADD_ADDR_ADDR_1920          ((uint16_t)0x000000F0U)         /* !< 1920.0 */
#define USB_IDXRXFIFOADD_ADDR_ADDR_1928          ((uint16_t)0x000000F1U)         /* !< 1928.0 */
#define USB_IDXRXFIFOADD_ADDR_ADDR_1936          ((uint16_t)0x000000F2U)         /* !< 1936.0 */
#define USB_IDXRXFIFOADD_ADDR_ADDR_1944          ((uint16_t)0x000000F3U)         /* !< 1944.0 */
#define USB_IDXRXFIFOADD_ADDR_ADDR_1952          ((uint16_t)0x000000F4U)         /* !< 1952.0 */
#define USB_IDXRXFIFOADD_ADDR_ADDR_1960          ((uint16_t)0x000000F5U)         /* !< 1960.0 */
#define USB_IDXRXFIFOADD_ADDR_ADDR_1968          ((uint16_t)0x000000F6U)         /* !< 1968.0 */
#define USB_IDXRXFIFOADD_ADDR_ADDR_1976          ((uint16_t)0x000000F7U)         /* !< 1976.0 */
#define USB_IDXRXFIFOADD_ADDR_ADDR_1984          ((uint16_t)0x000000F8U)         /* !< 1984.0 */
#define USB_IDXRXFIFOADD_ADDR_ADDR_1992          ((uint16_t)0x000000F9U)         /* !< 1992.0 */
#define USB_IDXRXFIFOADD_ADDR_ADDR_2000          ((uint16_t)0x000000FAU)         /* !< 2000.0 */
#define USB_IDXRXFIFOADD_ADDR_ADDR_2008          ((uint16_t)0x000000FBU)         /* !< 2008.0 */
#define USB_IDXRXFIFOADD_ADDR_ADDR_2016          ((uint16_t)0x000000FCU)         /* !< 2016.0 */
#define USB_IDXRXFIFOADD_ADDR_ADDR_2024          ((uint16_t)0x000000FDU)         /* !< 2024.0 */
#define USB_IDXRXFIFOADD_ADDR_ADDR_2032          ((uint16_t)0x000000FEU)         /* !< 2032.0 */
#define USB_IDXRXFIFOADD_ADDR_ADDR_2040          ((uint16_t)0x000000FFU)         /* !< 2040.0 */

/* USB_EPINFO Bits */
/* USB_EPINFO[TXENDPOINTS] Bits */
#define USB_EPINFO_TXENDPOINTS_OFS               (0)                             /* !< TXENDPOINTS Offset */
#define USB_EPINFO_TXENDPOINTS_MASK              ((uint8_t)0x0000000FU)          /* !< The number of TX endpoints
                                                                                    implemented in the design. */
/* USB_EPINFO[RXENDPOINTS] Bits */
#define USB_EPINFO_RXENDPOINTS_OFS               (4)                             /* !< RXENDPOINTS Offset */
#define USB_EPINFO_RXENDPOINTS_MASK              ((uint8_t)0x000000F0U)          /* !< The number of Rx endpoints
                                                                                    implemented in the design. */

/* USB_RAMINFO Bits */
/* USB_RAMINFO[RAMBITS] Bits */
#define USB_RAMINFO_RAMBITS_OFS                  (0)                             /* !< RAMBITS Offset */
#define USB_RAMINFO_RAMBITS_MASK                 ((uint8_t)0x0000000FU)          /* !< The width of the RAM address bus. */
#define USB_RAMINFO_RAMBITS_RAM_1KB              ((uint8_t)0x00000008U)          /* !< USB FIFO RAM size is 1kB */
#define USB_RAMINFO_RAMBITS_RAM_2KB              ((uint8_t)0x00000009U)          /* !< USB FIFO RAM size is 2kB */
#define USB_RAMINFO_RAMBITS_RAM_4KB              ((uint8_t)0x0000000AU)          /* !< USB FIFO RAM size is 4kB */

/* USB_CONTIM Bits */
/* USB_CONTIM[WTID] Bits */
#define USB_CONTIM_WTID_OFS                      (0)                             /* !< WTID Offset */
#define USB_CONTIM_WTID_MASK                     ((uint8_t)0x0000000FU)          /* !< The wait ID field configures the
                                                                                    delay required from the enable of the
                                                                                    ID detection to when the ID value is
                                                                                    valid, in units of 4.369 ms. Note:
                                                                                    The default corresponds to 52.43 ms. */
/* USB_CONTIM[WTCON] Bits */
#define USB_CONTIM_WTCON_OFS                     (4)                             /* !< WTCON Offset */
#define USB_CONTIM_WTCON_MASK                    ((uint8_t)0x000000F0U)          /* !< The connect wait field configures
                                                                                    the wait required to allow for the
                                                                                    user's connect/disconnect filter, in
                                                                                    units of 533.3 ns. Note: The default
                                                                                    corresponds to 2.667 us. */

/* USB_FSEOF Bits */
/* USB_FSEOF[FSEOFG] Bits */
#define USB_FSEOF_FSEOFG_OFS                     (0)                             /* !< FSEOFG Offset */
#define USB_FSEOF_FSEOFG_MASK                    ((uint8_t)0x000000FFU)          /* !< The full-speed end-of-frame gap
                                                                                    field is used during full-speed
                                                                                    transactions to configure the gap
                                                                                    between the last transaction and the
                                                                                    End-of-Frame (EOF), in units of 533.3
                                                                                    ns.  Note: The default corresponds to
                                                                                    63.46 us. */

/* USB_LSEOF Bits */
/* USB_LSEOF[LSEOFG] Bits */
#define USB_LSEOF_LSEOFG_OFS                     (0)                             /* !< LSEOFG Offset */
#define USB_LSEOF_LSEOFG_MASK                    ((uint8_t)0x000000FFU)          /* !< The low-speed end-of-frame gap
                                                                                    field is used during low-speed
                                                                                    transactions to set the gap between
                                                                                    the last transaction and the
                                                                                    End-of-Frame (EOF), in units of 1.067
                                                                                    us. Note: The default corresponds to
                                                                                    121.6 us. */

/* USB_CSR0L Bits */
/* USB_CSR0L[RXRDY] Bits */
#define USB_CSR0L_RXRDY_OFS                      (0)                             /* !< RXRDY Offset */
#define USB_CSR0L_RXRDY_MASK                     ((uint8_t)0x00000001U)          /* !< Receive Packet Ready. Device mode:
                                                                                    This bit is set when a data packet
                                                                                    has been received. An interrupt is
                                                                                    generated when this bit is set. The
                                                                                    CPU clears this bit by setting the
                                                                                    RXRDYC_STATUS bit. Host mode:
                                                                                    Software must clear this bit after he
                                                                                    packet has been read from the FIFO to
                                                                                    acknowledge that the data has been
                                                                                    read from the FIFO. */
#define USB_CSR0L_RXRDY_NO_PACKET                ((uint8_t)0x00000000U)          /* !< No receive packet has been
                                                                                    received. */
#define USB_CSR0L_RXRDY_RECV                     ((uint8_t)0x00000001U)          /* !< Indicates that a data packet has
                                                                                    been received in the RX FIFO. The EP0
                                                                                    bit in the USBTXIS register is also
                                                                                    set in this situation. */
/* USB_CSR0L[TXRDY] Bits */
#define USB_CSR0L_TXRDY_OFS                      (1)                             /* !< TXRDY Offset */
#define USB_CSR0L_TXRDY_MASK                     ((uint8_t)0x00000002U)          /* !< Transmit Packet Ready. Device &
                                                                                    Host mode: The CPU sets this bit
                                                                                    after loading a data packet into the
                                                                                    FIFO. It is cleared automatically
                                                                                    when a data packet has been
                                                                                    transmitted. An interrupt is also
                                                                                    generated at this point (if enabled).
                                                                                    Host mode: If both the TXRDY and
                                                                                    DATAEND_SETUP bits are set, a setup
                                                                                    packet is sent. If just TXRDY is set,
                                                                                    an OUT packet is sent. */
#define USB_CSR0L_TXRDY_WAIT                     ((uint8_t)0x00000000U)          /* !< No transmit packet is ready. */
#define USB_CSR0L_TXRDY_READY                    ((uint8_t)0x00000002U)          /* !< Software sets this bit after
                                                                                    loading a data packet into the TX
                                                                                    FIFO. The EP0 bit in the USBTXIS
                                                                                    register is also set in this
                                                                                    situation. */
/* USB_CSR0L[STALLED] Bits */
#define USB_CSR0L_STALLED_OFS                    (2)                             /* !< STALLED Offset */
#define USB_CSR0L_STALLED_MASK                   ((uint8_t)0x00000004U)          /* !< Sent Stall / Receive Stall  Device
                                                                                    mode: This bit is set when a STALL
                                                                                    handshake is transmitted. The CPU
                                                                                    should clear this bit. Host mode:
                                                                                    This bit is set when a STALL
                                                                                    handshake is received. The CPU should
                                                                                    clear this bit. */
#define USB_CSR0L_STALLED_NO_HANDSHAKE           ((uint8_t)0x00000000U)          /* !< No handshake has been received. */
#define USB_CSR0L_STALLED_HANDSHAKE              ((uint8_t)0x00000004U)          /* !< A STALL handshake has been received
                                                                                    or transmitted */
/* USB_CSR0L[DATAEND_SETUP] Bits */
#define USB_CSR0L_DATAEND_SETUP_OFS              (3)                             /* !< DATAEND_SETUP Offset */
#define USB_CSR0L_DATAEND_SETUP_MASK             ((uint8_t)0x00000008U)          /* !< Data end / Setup Packet. Device
                                                                                    mode: Data end bit. The CPU sets this
                                                                                    bit:  1. When setting TXRDY for the
                                                                                    last data packet. 2. When clearing
                                                                                    RXRDY after unloading the last data
                                                                                    packet. 3. When setting TXRDY for a
                                                                                    zero length data packet. Host mode:
                                                                                    Setup packet. The CPU sets this bit,
                                                                                    at the same time as the TXRDY bit is
                                                                                    set, to send a SETUP token instead of
                                                                                    an OUT token for the transaction.
                                                                                    Note: Setting this bit always clears
                                                                                    the DT bit in the USBCSRH0 register. */
#define USB_CSR0L_DATAEND_SETUP_EFFECT           ((uint8_t)0x00000000U)          /* !< No effect */
/* USB_CSR0L[SETEND_ERROR] Bits */
#define USB_CSR0L_SETEND_ERROR_OFS               (4)                             /* !< SETEND_ERROR Offset */
#define USB_CSR0L_SETEND_ERROR_MASK              ((uint8_t)0x00000010U)          /* !< Setup end / Error. Device mode:
                                                                                    Setup end. This bit will be set when
                                                                                    a control transaction ends before the
                                                                                    DATAEND_SETUP bit has been set. An
                                                                                    interrupt will be generated and the
                                                                                    FIFO flushed at this time. The bit is
                                                                                    cleared by the CPU writing a 1 to the
                                                                                    SETENDC_NAKTO bit. Host mode: Error
                                                                                    indication. This bit will be set when
                                                                                    three attempts have been made to
                                                                                    perform a transaction with no
                                                                                    response from the peripheral. The CPU
                                                                                    should clear this bit. An interrupt
                                                                                    is generated when this bit is set. */
#define USB_CSR0L_SETEND_ERROR_NO_EFFECT         ((uint8_t)0x00000000U)          /* !< Clear Error. Note: Only effective
                                                                                    in host mode. */
#define USB_CSR0L_SETEND_ERROR_EFFECT            ((uint8_t)0x00000010U)          /* !< Setup end / Error indication. */
/* USB_CSR0L[STALL_RQPKT] Bits */
#define USB_CSR0L_STALL_RQPKT_OFS                (5)                             /* !< STALL_RQPKT Offset */
#define USB_CSR0L_STALL_RQPKT_MASK               ((uint8_t)0x00000020U)          /* !< Send Stall / Request Packet. Device
                                                                                    mode: The CPU writes a 1 to this bit
                                                                                    to terminate the current transaction.
                                                                                    The STALL handshake will be
                                                                                    transmitted and then this bit will be
                                                                                    cleared automatically. Host mode: The
                                                                                    CPU sets this bit to request an IN
                                                                                    transaction. It is cleared when RXRDY
                                                                                    bit is set. */
#define USB_CSR0L_STALL_RQPKT_NO_EFFECT          ((uint8_t)0x00000000U)          /* !< No effect */
#define USB_CSR0L_STALL_RQPKT_EFFECT             ((uint8_t)0x00000020U)          /* !< Send stall request or request
                                                                                    packet is in effect. */
/* USB_CSR0L[RXRDYC_STATUS] Bits */
#define USB_CSR0L_RXRDYC_STATUS_OFS              (6)                             /* !< RXRDYC_STATUS Offset */
#define USB_CSR0L_RXRDYC_STATUS_MASK             ((uint8_t)0x00000040U)          /* !< Receive ready clear / Status packet
                                                                                    Device mode: Receive ready clear. The
                                                                                    CPU writes a 1 to this bit to clear
                                                                                    the RXRDY bit. It is cleared
                                                                                    automatically Host mode: Status
                                                                                    Packet. The CPU sets this bit at the
                                                                                    same time as the TXRDY or STALL_RQPKT
                                                                                    bit is set, to perform a status stage
                                                                                    transaction. Setting this bit ensures
                                                                                    that the DT bit is set in the
                                                                                    USBCSRH0 register, so that a DATA1
                                                                                    packet is used for the Status Stage
                                                                                    transaction.  Setting this bit
                                                                                    ensures that the DT bit is set in the
                                                                                    USBCSRH0 register so that a DATA1
                                                                                    packet is used for the STATUS stage
                                                                                    transaction. */
#define USB_CSR0L_RXRDYC_STATUS_NO_EFFECT        ((uint8_t)0x00000000U)          /* !< No transaction */
#define USB_CSR0L_RXRDYC_STATUS_EFFECT           ((uint8_t)0x00000040U)          /* !< Clear RXRDY or sent status packet. */
/* USB_CSR0L[SETENDC_NAKTO] Bits */
#define USB_CSR0L_SETENDC_NAKTO_OFS              (7)                             /* !< SETENDC_NAKTO Offset */
#define USB_CSR0L_SETENDC_NAKTO_MASK             ((uint8_t)0x00000080U)          /* !< Setup End Clear / NAK Timeout
                                                                                    Device mode: The CPU writes a 1 to
                                                                                    this bit to clear the SETEND_ERROR
                                                                                    bit. It is cleared automatically.
                                                                                    Host mode: This bit will be set when
                                                                                    Endpoint 0 is halted following the
                                                                                    receipt of NAK responses for longer
                                                                                    than the time set by the USBNAKLMT
                                                                                    register. The CPU should clear this
                                                                                    bit to allow the endpoint to
                                                                                    continue. */
#define USB_CSR0L_SETENDC_NAKTO_NO_EFFECT        ((uint8_t)0x00000000U)          /* !< No effect */
#define USB_CSR0L_SETENDC_NAKTO_EFFECT           ((uint8_t)0x00000080U)          /* !< Clear SETEND_ERROR or SETENDC_KAKTO */

/* USB_CSR0H Bits */
/* USB_CSR0H[FLUSH] Bits */
#define USB_CSR0H_FLUSH_OFS                      (0)                             /* !< FLUSH Offset */
#define USB_CSR0H_FLUSH_MASK                     ((uint8_t)0x00000001U)          /* !< Flush FIFO. The CPU writes a 1 to
                                                                                    this bit to flush the next packet to
                                                                                    be transmitted/read from the Endpoint
                                                                                    0 FIFO. The FIFO pointer is reset and
                                                                                    the TXRDY/RXRDY bit in the USBCSRL0
                                                                                    register is cleared. Note: FLUSH
                                                                                    should only be used when TXRDY/RXRDY
                                                                                    is set. At other times, it may cause
                                                                                    data to be corrupted. */
#define USB_CSR0H_FLUSH_NO_EFFECT                ((uint8_t)0x00000000U)          /* !< No effect */
#define USB_CSR0H_FLUSH_EFFECT                   ((uint8_t)0x00000001U)          /* !< Flush the FIFO */
/* USB_CSR0H[DT] Bits */
#define USB_CSR0H_DT_OFS                         (1)                             /* !< DT Offset */
#define USB_CSR0H_DT_MASK                        ((uint8_t)0x00000002U)          /* !< Data Toggle. When read, this bit
                                                                                    indicates the current state of the
                                                                                    endpoint 0 data toggle. If DTWE is
                                                                                    set, this bit may be written with the
                                                                                    required setting of the data toggle.
                                                                                    If DTWE is Low, this bit cannot be
                                                                                    written. Care should be taken when
                                                                                    writing to this bit as it should only
                                                                                    be changed to RESET USB endpoint 0.
                                                                                    Note: This bit is only effective in
                                                                                    host mode. */
#define USB_CSR0H_DT_LOW                         ((uint8_t)0x00000000U)          /* !< Data toggle is low */
#define USB_CSR0H_DT_HIGH                        ((uint8_t)0x00000002U)          /* !< Data toggle is high */
/* USB_CSR0H[DTWE] Bits */
#define USB_CSR0H_DTWE_OFS                       (2)                             /* !< DTWE Offset */
#define USB_CSR0H_DTWE_MASK                      ((uint8_t)0x00000004U)          /* !< Data Toggle Write Enable. This bit
                                                                                    is automatically cleared once the new
                                                                                    value is written. Note: This bit is
                                                                                    only effective in host mode. */
#define USB_CSR0H_DTWE_DISABLE                   ((uint8_t)0x00000000U)          /* !< The DT bit cannot be written. */
#define USB_CSR0H_DTWE_ENABLE                    ((uint8_t)0x00000004U)          /* !< Enables the current state of the
                                                                                    endpoint 0 data toggle to be written
                                                                                    (see DT bit). */
/* USB_CSR0H[DISPING] Bits */
#define USB_CSR0H_DISPING_OFS                    (3)                             /* !< DISPING Offset */
#define USB_CSR0H_DISPING_MASK                   ((uint8_t)0x00000008U)          /* !< Diable PING tokens in data and
                                                                                    status phases of a high-speed Control
                                                                                    transfer. */
#define USB_CSR0H_DISPING_CLR                    ((uint8_t)0x00000000U)          /* !< Ping is allowed. */
#define USB_CSR0H_DISPING_SET                    ((uint8_t)0x00000008U)          /* !< Ping is not allowed. */

/* USB_COUNT0 Bits */
/* USB_COUNT0[COUNT] Bits */
#define USB_COUNT0_COUNT_OFS                     (0)                             /* !< COUNT Offset */
#define USB_COUNT0_COUNT_MASK                    ((uint8_t)0x0000007FU)          /* !< FIFO Count. COUNT is a read-only
                                                                                    value that indicates the number of
                                                                                    received data bytes in the endpoint 0
                                                                                    FIFO. The value returned changes as
                                                                                    the contents of the FIFO change and
                                                                                    is only valid while RXRDY
                                                                                    (USBCSRL.RXRDY) is set. */

/* USB_TYPE0 Bits */
/* USB_TYPE0[SPEED] Bits */
#define USB_TYPE0_SPEED_OFS                      (6)                             /* !< SPEED Offset */
#define USB_TYPE0_SPEED_MASK                     ((uint8_t)0x000000C0U)          /* !< Operating Speed specifies the
                                                                                    operating speed of the target Device.
                                                                                    If selected, the target is assumed
                                                                                    #br#to have the same connection speed
                                                                                    as the USB controller. */
#define USB_TYPE0_SPEED_UNUSED                   ((uint8_t)0x00000000U)          /* !< Unused (Note: If selected, the
                                                                                    target will be assumed to be using
                                                                                    the same connection speed as the USB
                                                                                    controller.) */
#define USB_TYPE0_SPEED_FULL                     ((uint8_t)0x00000080U)          /* !< Full-speed */
#define USB_TYPE0_SPEED_LOW                      ((uint8_t)0x000000C0U)          /* !< Low-speed */

/* USB_NAKLMT0 Bits */
/* USB_NAKLMT0[NAKLMT] Bits */
#define USB_NAKLMT0_NAKLMT_OFS                   (0)                             /* !< NAKLMT Offset */
#define USB_NAKLMT0_NAKLMT_MASK                  ((uint8_t)0x0000001FU)          /* !< EP0 NAK Limit specifies the number
                                                                                    of frames after receiving a stream of
                                                                                    NAK responses. */

/* USB_CONFIG Bits */
/* USB_CONFIG[UTMIDATWDTH] Bits */
#define USB_CONFIG_UTMIDATWDTH_OFS               (0)                             /* !< UTMIDATWDTH Offset */
#define USB_CONFIG_UTMIDATWDTH_MASK              ((uint8_t)0x00000001U)          /* !< Indicates selected UTMI+ data
                                                                                    width. Always 0 indicating 8 bits. */
/* USB_CONFIG[SOFTCON] Bits */
#define USB_CONFIG_SOFTCON_OFS                   (1)                             /* !< SOFTCON Offset */
#define USB_CONFIG_SOFTCON_MASK                  ((uint8_t)0x00000002U)          /* !< Always 1 . Indicates Soft
                                                                                    Connect/Disconnect is enabled. */
/* USB_CONFIG[DYNFIFO] Bits */
#define USB_CONFIG_DYNFIFO_OFS                   (2)                             /* !< DYNFIFO Offset */
#define USB_CONFIG_DYNFIFO_MASK                  ((uint8_t)0x00000004U)          /* !< When set to 1 indicates Dynamic
                                                                                    FIFO Sizing option selected. */
/* USB_CONFIG[HBTXE] Bits */
#define USB_CONFIG_HBTXE_OFS                     (3)                             /* !< HBTXE Offset */
#define USB_CONFIG_HBTXE_MASK                    ((uint8_t)0x00000008U)          /* !< When set to 1 indicates
                                                                                    High-bandwidth TX ISO Endpoint
                                                                                    Support selected. */
/* USB_CONFIG[HBRXE] Bits */
#define USB_CONFIG_HBRXE_OFS                     (4)                             /* !< HBRXE Offset */
#define USB_CONFIG_HBRXE_MASK                    ((uint8_t)0x00000010U)          /* !< When set to 1 indicates
                                                                                    High-bandwidth Rx ISO Endpoint
                                                                                    Support selected. */
/* USB_CONFIG[BIGENDIAN] Bits */
#define USB_CONFIG_BIGENDIAN_OFS                 (5)                             /* !< BIGENDIAN Offset */
#define USB_CONFIG_BIGENDIAN_MASK                ((uint8_t)0x00000020U)          /* !< Always '0'. Indicates Little Endian
                                                                                    ordering. */
/* USB_CONFIG[MPTXE] Bits */
#define USB_CONFIG_MPTXE_OFS                     (6)                             /* !< MPTXE Offset */
#define USB_CONFIG_MPTXE_MASK                    ((uint8_t)0x00000040U)          /* !< When set to 1, automatic splitting
                                                                                    of bulk packets is selected. */
/* USB_CONFIG[MPRXE] Bits */
#define USB_CONFIG_MPRXE_OFS                     (7)                             /* !< MPRXE Offset */
#define USB_CONFIG_MPRXE_MASK                    ((uint8_t)0x00000080U)          /* !< When set to 1, automatic
                                                                                    amalgamation of bulk packets is
                                                                                    selected. */

/* USB_RQPKTCOUNT Bits */
/* USB_RQPKTCOUNT[COUNT] Bits */
#define USB_RQPKTCOUNT_COUNT_OFS                 (0)                             /* !< COUNT Offset */
#define USB_RQPKTCOUNT_COUNT_MASK                ((uint32_t)0x0000FFFFU)         /* !< Block Transfer Packet Count sets
                                                                                    the number of packets of the size
                                                                                    defined by the MAXLOAD bit field that
                                                                                    are to be transferred in a block
                                                                                    transfer. Note: This is only used in
                                                                                    Host mode when AUTORQ is set. The bit
                                                                                    has no effect in Device mode or when
                                                                                    AUTORQ is not set. */

/* USB_RXDPKTBUFDIS Bits */
/* USB_RXDPKTBUFDIS[EP1] Bits */
#define USB_RXDPKTBUFDIS_EP1_OFS                 (1)                             /* !< EP1 Offset */
#define USB_RXDPKTBUFDIS_EP1_MASK                ((uint16_t)0x00000002U)         /* !< EP1 RX Double Packet Buffer Disable */
#define USB_RXDPKTBUFDIS_EP1_DISABLE             ((uint16_t)0x00000000U)         /* !< Disables double-packet buffering. */
#define USB_RXDPKTBUFDIS_EP1_ENABLE              ((uint16_t)0x00000002U)         /* !< Enables double-packet buffering. */
/* USB_RXDPKTBUFDIS[EP2] Bits */
#define USB_RXDPKTBUFDIS_EP2_OFS                 (2)                             /* !< EP2 Offset */
#define USB_RXDPKTBUFDIS_EP2_MASK                ((uint16_t)0x00000004U)         /* !< EP2 RX Double Packet Buffer Disable */
#define USB_RXDPKTBUFDIS_EP2_DISABLE             ((uint16_t)0x00000000U)         /* !< Disables double-packet buffering. */
#define USB_RXDPKTBUFDIS_EP2_ENABLE              ((uint16_t)0x00000004U)         /* !< Enables double-packet buffering. */
/* USB_RXDPKTBUFDIS[EP3] Bits */
#define USB_RXDPKTBUFDIS_EP3_OFS                 (3)                             /* !< EP3 Offset */
#define USB_RXDPKTBUFDIS_EP3_MASK                ((uint16_t)0x00000008U)         /* !< EP3 RX Double Packet Buffer Disable */
#define USB_RXDPKTBUFDIS_EP3_DISABLE             ((uint16_t)0x00000000U)         /* !< Disables double-packet buffering. */
#define USB_RXDPKTBUFDIS_EP3_ENABLE              ((uint16_t)0x00000008U)         /* !< Enables double-packet buffering. */
/* USB_RXDPKTBUFDIS[EP4] Bits */
#define USB_RXDPKTBUFDIS_EP4_OFS                 (4)                             /* !< EP4 Offset */
#define USB_RXDPKTBUFDIS_EP4_MASK                ((uint16_t)0x00000010U)         /* !< EP4 RX Double Packet Buffer Disable */
#define USB_RXDPKTBUFDIS_EP4_DISABLE             ((uint16_t)0x00000000U)         /* !< Disables double-packet buffering. */
#define USB_RXDPKTBUFDIS_EP4_ENABLE              ((uint16_t)0x00000010U)         /* !< Enables double-packet buffering. */
/* USB_RXDPKTBUFDIS[EP5] Bits */
#define USB_RXDPKTBUFDIS_EP5_OFS                 (5)                             /* !< EP5 Offset */
#define USB_RXDPKTBUFDIS_EP5_MASK                ((uint16_t)0x00000020U)         /* !< EP5 RX Double Packet Buffer Disable */
#define USB_RXDPKTBUFDIS_EP5_DISABLE             ((uint16_t)0x00000000U)         /* !< Disables double-packet buffering. */
#define USB_RXDPKTBUFDIS_EP5_ENABLE              ((uint16_t)0x00000020U)         /* !< Enables double-packet buffering. */
/* USB_RXDPKTBUFDIS[EP6] Bits */
#define USB_RXDPKTBUFDIS_EP6_OFS                 (6)                             /* !< EP6 Offset */
#define USB_RXDPKTBUFDIS_EP6_MASK                ((uint16_t)0x00000040U)         /* !< EP6 RX Double Packet Buffer Disable */
#define USB_RXDPKTBUFDIS_EP6_DISABLE             ((uint16_t)0x00000000U)         /* !< Disables double-packet buffering. */
#define USB_RXDPKTBUFDIS_EP6_ENABLE              ((uint16_t)0x00000040U)         /* !< Enables double-packet buffering. */
/* USB_RXDPKTBUFDIS[EP7] Bits */
#define USB_RXDPKTBUFDIS_EP7_OFS                 (7)                             /* !< EP7 Offset */
#define USB_RXDPKTBUFDIS_EP7_MASK                ((uint16_t)0x00000080U)         /* !< EP7 RX Double Packet Buffer Disable */
#define USB_RXDPKTBUFDIS_EP7_DISABLE             ((uint16_t)0x00000000U)         /* !< Disables double-packet buffering. */
#define USB_RXDPKTBUFDIS_EP7_ENABLE              ((uint16_t)0x00000080U)         /* !< Enables double-packet buffering. */

/* USB_TXDPKTBUFDIS Bits */
/* USB_TXDPKTBUFDIS[EP1] Bits */
#define USB_TXDPKTBUFDIS_EP1_OFS                 (1)                             /* !< EP1 Offset */
#define USB_TXDPKTBUFDIS_EP1_MASK                ((uint16_t)0x00000002U)         /* !< EP1 TX Double Packet Buffer Disable */
#define USB_TXDPKTBUFDIS_EP1_DISABLE             ((uint16_t)0x00000000U)         /* !< Disables double-packet buffering. */
#define USB_TXDPKTBUFDIS_EP1_ENABLE              ((uint16_t)0x00000002U)         /* !< Enables double-packet buffering. */
/* USB_TXDPKTBUFDIS[EP2] Bits */
#define USB_TXDPKTBUFDIS_EP2_OFS                 (2)                             /* !< EP2 Offset */
#define USB_TXDPKTBUFDIS_EP2_MASK                ((uint16_t)0x00000004U)         /* !< EP2 TX Double Packet Buffer Disable */
#define USB_TXDPKTBUFDIS_EP2_DISABLE             ((uint16_t)0x00000000U)         /* !< Disables double-packet buffering. */
#define USB_TXDPKTBUFDIS_EP2_ENABLE              ((uint16_t)0x00000004U)         /* !< Enables double-packet buffering. */
/* USB_TXDPKTBUFDIS[EP3] Bits */
#define USB_TXDPKTBUFDIS_EP3_OFS                 (3)                             /* !< EP3 Offset */
#define USB_TXDPKTBUFDIS_EP3_MASK                ((uint16_t)0x00000008U)         /* !< EP3 TX Double Packet Buffer Disable */
#define USB_TXDPKTBUFDIS_EP3_DISABLE             ((uint16_t)0x00000000U)         /* !< Disables double-packet buffering. */
#define USB_TXDPKTBUFDIS_EP3_ENABLE              ((uint16_t)0x00000008U)         /* !< Enables double-packet buffering. */
/* USB_TXDPKTBUFDIS[EP4] Bits */
#define USB_TXDPKTBUFDIS_EP4_OFS                 (4)                             /* !< EP4 Offset */
#define USB_TXDPKTBUFDIS_EP4_MASK                ((uint16_t)0x00000010U)         /* !< EP4 TX Double Packet Buffer Disable */
#define USB_TXDPKTBUFDIS_EP4_DISABLE             ((uint16_t)0x00000000U)         /* !< Disables double-packet buffering. */
#define USB_TXDPKTBUFDIS_EP4_ENABLE              ((uint16_t)0x00000010U)         /* !< Enables double-packet buffering. */
/* USB_TXDPKTBUFDIS[EP5] Bits */
#define USB_TXDPKTBUFDIS_EP5_OFS                 (5)                             /* !< EP5 Offset */
#define USB_TXDPKTBUFDIS_EP5_MASK                ((uint16_t)0x00000020U)         /* !< EP5 TX Double Packet Buffer Disable */
#define USB_TXDPKTBUFDIS_EP5_DISABLE             ((uint16_t)0x00000000U)         /* !< Disables double-packet buffering. */
#define USB_TXDPKTBUFDIS_EP5_ENABLE              ((uint16_t)0x00000020U)         /* !< Enables double-packet buffering. */
/* USB_TXDPKTBUFDIS[EP6] Bits */
#define USB_TXDPKTBUFDIS_EP6_OFS                 (6)                             /* !< EP6 Offset */
#define USB_TXDPKTBUFDIS_EP6_MASK                ((uint16_t)0x00000040U)         /* !< EP6 TX Double Packet Buffer Disable */
#define USB_TXDPKTBUFDIS_EP6_DISABLE             ((uint16_t)0x00000000U)         /* !< Disables double-packet buffering. */
#define USB_TXDPKTBUFDIS_EP6_ENABLE              ((uint16_t)0x00000040U)         /* !< Enables double-packet buffering. */
/* USB_TXDPKTBUFDIS[EP7] Bits */
#define USB_TXDPKTBUFDIS_EP7_OFS                 (7)                             /* !< EP7 Offset */
#define USB_TXDPKTBUFDIS_EP7_MASK                ((uint16_t)0x00000080U)         /* !< EP7 TX Double Packet Buffer Disable */
#define USB_TXDPKTBUFDIS_EP7_DISABLE             ((uint16_t)0x00000000U)         /* !< Disables double-packet buffering. */
#define USB_TXDPKTBUFDIS_EP7_ENABLE              ((uint16_t)0x00000080U)         /* !< Enables double-packet buffering. */

/* USB_IIDX Bits */
/* USB_IIDX[STAT] Bits */
#define USB_IIDX_STAT_OFS                        (0)                             /* !< STAT Offset */
#define USB_IIDX_STAT_MASK                       ((uint32_t)0x000000FFU)         /* !< Interrupt index status */
#define USB_IIDX_STAT_NO_INTR                    ((uint32_t)0x00000000U)         /* !< No bit is set means there is no
                                                                                    pending interrupt request */
#define USB_IIDX_STAT_INTRTX                     ((uint32_t)0x00000001U)         /* !< Endpoint 0 and the TX Endpoints
                                                                                    interrupt */
#define USB_IIDX_STAT_INTRRX                     ((uint32_t)0x00000002U)         /* !< RX Endpoints interrupt */
#define USB_IIDX_STAT_INTRUSB                    ((uint32_t)0x00000003U)         /* !< USB Interrupts */
#define USB_IIDX_STAT_VUSBPWRDN                  ((uint32_t)0x00000004U)         /* !< VUSB power down */
#define USB_IIDX_STAT_DMADONEARX                 ((uint32_t)0x00000005U)         /* !< DMA-Done interrupt for Trigger
                                                                                    USB-A-RX */
#define USB_IIDX_STAT_DMADONEATX                 ((uint32_t)0x00000006U)         /* !< DMA-Done interrupt for Trigger
                                                                                    USB-A-TX */
#define USB_IIDX_STAT_DMADONEBRX                 ((uint32_t)0x00000007U)         /* !< DMA-Done interrupt for Trigger
                                                                                    USB-B-RX */
#define USB_IIDX_STAT_DMADONEBTX                 ((uint32_t)0x00000008U)         /* !< DMA-Done interrupt for Trigger
                                                                                    USB-B-TX */
#define USB_IIDX_STAT_DMADONECRX                 ((uint32_t)0x00000009U)         /* !< DMA-Done interrupt for Trigger
                                                                                    USB-C-RX */
#define USB_IIDX_STAT_DMADONECTX                 ((uint32_t)0x0000000AU)         /* !< DMA-Done interrupt for Trigger
                                                                                    USB-C-TX */
#define USB_IIDX_STAT_DMADONEDRX                 ((uint32_t)0x0000000BU)         /* !< DMA-Done interrupt for Trigger
                                                                                    USB-D-RX */
#define USB_IIDX_STAT_DMADONEDTX                 ((uint32_t)0x0000000CU)         /* !< DMA-Done interrupt for Trigger
                                                                                    USB-D-TX */
#define USB_IIDX_STAT_DMAPREARX                  ((uint32_t)0x0000000DU)         /* !< DMA-Pre interrupt for Trigger
                                                                                    USB-A-RX */
#define USB_IIDX_STAT_DMAPREATX                  ((uint32_t)0x0000000EU)         /* !< DMA-Pre interrupt for Trigger
                                                                                    USB-A-TX */
#define USB_IIDX_STAT_DMAPREBRX                  ((uint32_t)0x0000000FU)         /* !< DMA-Pre interrupt for Trigger
                                                                                    USB-B-RX */
#define USB_IIDX_STAT_DMAPREBTX                  ((uint32_t)0x00000010U)         /* !< DMA-Pre interrupt for Trigger
                                                                                    USB-B-TX */
#define USB_IIDX_STAT_DMAPRECRX                  ((uint32_t)0x00000011U)         /* !< DMA-Pre interrupt for Trigger
                                                                                    USB-C-RX */
#define USB_IIDX_STAT_DMAPRECTX                  ((uint32_t)0x00000012U)         /* !< DMA-Pre interrupt for Trigger
                                                                                    USB-C-TX */
#define USB_IIDX_STAT_DMAPREDRX                  ((uint32_t)0x00000013U)         /* !< DMA-Pre interrupt for Trigger
                                                                                    USB-D-RX */
#define USB_IIDX_STAT_DMAPREDTX                  ((uint32_t)0x00000014U)         /* !< DMA-Pre interrupt for Trigger
                                                                                    USB-D-TX */

/* USB_IMASK Bits */
/* USB_IMASK[INTRTX] Bits */
#define USB_IMASK_INTRTX_OFS                     (0)                             /* !< INTRTX Offset */
#define USB_IMASK_INTRTX_MASK                    ((uint32_t)0x00000001U)         /* !< Endpoint 0 and the TX Endpoints
                                                                                    interrupt */
#define USB_IMASK_INTRTX_CLR                     ((uint32_t)0x00000000U)         /* !< Interrupt Disabled. */
#define USB_IMASK_INTRTX_SET                     ((uint32_t)0x00000001U)         /* !< Interrupt Enabled. */
/* USB_IMASK[INTRRX] Bits */
#define USB_IMASK_INTRRX_OFS                     (1)                             /* !< INTRRX Offset */
#define USB_IMASK_INTRRX_MASK                    ((uint32_t)0x00000002U)         /* !< RX Endpoints interrupt */
#define USB_IMASK_INTRRX_CLR                     ((uint32_t)0x00000000U)         /* !< Interrupt Disabled. */
#define USB_IMASK_INTRRX_SET                     ((uint32_t)0x00000002U)         /* !< Interrupt Enabled. */
/* USB_IMASK[INTRUSB] Bits */
#define USB_IMASK_INTRUSB_OFS                    (2)                             /* !< INTRUSB Offset */
#define USB_IMASK_INTRUSB_MASK                   ((uint32_t)0x00000004U)         /* !< USB Interrupts */
#define USB_IMASK_INTRUSB_CLR                    ((uint32_t)0x00000000U)         /* !< Interrupt Disabled. */
#define USB_IMASK_INTRUSB_SET                    ((uint32_t)0x00000004U)         /* !< Interrupt Enabled. */
/* USB_IMASK[VUSBPWRDN] Bits */
#define USB_IMASK_VUSBPWRDN_OFS                  (3)                             /* !< VUSBPWRDN Offset */
#define USB_IMASK_VUSBPWRDN_MASK                 ((uint32_t)0x00000008U)         /* !< VUSB Power Down */
#define USB_IMASK_VUSBPWRDN_CLR                  ((uint32_t)0x00000000U)         /* !< Interrupt Disabled. */
#define USB_IMASK_VUSBPWRDN_SET                  ((uint32_t)0x00000008U)         /* !< Interrupt Enabled. */
/* USB_IMASK[DMADONEARX] Bits */
#define USB_IMASK_DMADONEARX_OFS                 (4)                             /* !< DMADONEARX Offset */
#define USB_IMASK_DMADONEARX_MASK                ((uint32_t)0x00000010U)         /* !< DMA-Done interrupt for Trigger
                                                                                    USB-A-RX */
#define USB_IMASK_DMADONEARX_CLR                 ((uint32_t)0x00000000U)         /* !< Interrupt Disabled. */
#define USB_IMASK_DMADONEARX_SET                 ((uint32_t)0x00000010U)         /* !< Interrupt Enabled. */
/* USB_IMASK[DMADONEATX] Bits */
#define USB_IMASK_DMADONEATX_OFS                 (5)                             /* !< DMADONEATX Offset */
#define USB_IMASK_DMADONEATX_MASK                ((uint32_t)0x00000020U)         /* !< DMA-Done interrupt for Trigger
                                                                                    USB-A-TX */
#define USB_IMASK_DMADONEATX_CLR                 ((uint32_t)0x00000000U)         /* !< Interrupt Disabled. */
#define USB_IMASK_DMADONEATX_SET                 ((uint32_t)0x00000020U)         /* !< Interrupt Enabled. */
/* USB_IMASK[DMADONEBRX] Bits */
#define USB_IMASK_DMADONEBRX_OFS                 (6)                             /* !< DMADONEBRX Offset */
#define USB_IMASK_DMADONEBRX_MASK                ((uint32_t)0x00000040U)         /* !< DMA-Done interrupt for Trigger
                                                                                    USB-B-RX */
#define USB_IMASK_DMADONEBRX_CLR                 ((uint32_t)0x00000000U)         /* !< Interrupt Disabled. */
#define USB_IMASK_DMADONEBRX_SET                 ((uint32_t)0x00000040U)         /* !< Interrupt Enabled. */
/* USB_IMASK[DMADONEBTX] Bits */
#define USB_IMASK_DMADONEBTX_OFS                 (7)                             /* !< DMADONEBTX Offset */
#define USB_IMASK_DMADONEBTX_MASK                ((uint32_t)0x00000080U)         /* !< DMA-Done interrupt for Trigger
                                                                                    USB-B-TX */
#define USB_IMASK_DMADONEBTX_CLR                 ((uint32_t)0x00000000U)         /* !< Interrupt Disabled. */
#define USB_IMASK_DMADONEBTX_SET                 ((uint32_t)0x00000080U)         /* !< Interrupt Enabled. */
/* USB_IMASK[DMADONECRX] Bits */
#define USB_IMASK_DMADONECRX_OFS                 (8)                             /* !< DMADONECRX Offset */
#define USB_IMASK_DMADONECRX_MASK                ((uint32_t)0x00000100U)         /* !< DMA-Done interrupt for Trigger
                                                                                    USB-C-RX */
#define USB_IMASK_DMADONECRX_CLR                 ((uint32_t)0x00000000U)         /* !< Interrupt Disabled. */
#define USB_IMASK_DMADONECRX_SET                 ((uint32_t)0x00000100U)         /* !< Interrupt Enabled. */
/* USB_IMASK[DMADONECTX] Bits */
#define USB_IMASK_DMADONECTX_OFS                 (9)                             /* !< DMADONECTX Offset */
#define USB_IMASK_DMADONECTX_MASK                ((uint32_t)0x00000200U)         /* !< DMA-Done interrupt for Trigger
                                                                                    USB-C-TX */
#define USB_IMASK_DMADONECTX_CLR                 ((uint32_t)0x00000000U)         /* !< Interrupt Disabled. */
#define USB_IMASK_DMADONECTX_SET                 ((uint32_t)0x00000200U)         /* !< Interrupt Enabled. */
/* USB_IMASK[DMADONEDRX] Bits */
#define USB_IMASK_DMADONEDRX_OFS                 (10)                            /* !< DMADONEDRX Offset */
#define USB_IMASK_DMADONEDRX_MASK                ((uint32_t)0x00000400U)         /* !< DMA-Done interrupt for Trigger
                                                                                    USB-D-RX */
#define USB_IMASK_DMADONEDRX_CLR                 ((uint32_t)0x00000000U)         /* !< Interrupt Disabled. */
#define USB_IMASK_DMADONEDRX_SET                 ((uint32_t)0x00000400U)         /* !< Interrupt Enabled. */
/* USB_IMASK[DMADONEDTX] Bits */
#define USB_IMASK_DMADONEDTX_OFS                 (11)                            /* !< DMADONEDTX Offset */
#define USB_IMASK_DMADONEDTX_MASK                ((uint32_t)0x00000800U)         /* !< DMA-Done interrupt for Trigger
                                                                                    USB-D-TX */
#define USB_IMASK_DMADONEDTX_CLR                 ((uint32_t)0x00000000U)         /* !< Interrupt Disabled. */
#define USB_IMASK_DMADONEDTX_SET                 ((uint32_t)0x00000800U)         /* !< Interrupt Enabled. */
/* USB_IMASK[DMAPREARX] Bits */
#define USB_IMASK_DMAPREARX_OFS                  (12)                            /* !< DMAPREARX Offset */
#define USB_IMASK_DMAPREARX_MASK                 ((uint32_t)0x00001000U)         /* !< DMA-Pre interrupt for Trigger
                                                                                    USB-A-RX */
#define USB_IMASK_DMAPREARX_CLR                  ((uint32_t)0x00000000U)         /* !< Interrupt Disabled. */
#define USB_IMASK_DMAPREARX_SET                  ((uint32_t)0x00001000U)         /* !< Interrupt Enabled. */
/* USB_IMASK[DMAPREATX] Bits */
#define USB_IMASK_DMAPREATX_OFS                  (13)                            /* !< DMAPREATX Offset */
#define USB_IMASK_DMAPREATX_MASK                 ((uint32_t)0x00002000U)         /* !< DMA-Pre interrupt for Trigger
                                                                                    USB-A-TX */
#define USB_IMASK_DMAPREATX_CLR                  ((uint32_t)0x00000000U)         /* !< Interrupt Disabled. */
#define USB_IMASK_DMAPREATX_SET                  ((uint32_t)0x00002000U)         /* !< Interrupt Enabled. */
/* USB_IMASK[DMAPREBRX] Bits */
#define USB_IMASK_DMAPREBRX_OFS                  (14)                            /* !< DMAPREBRX Offset */
#define USB_IMASK_DMAPREBRX_MASK                 ((uint32_t)0x00004000U)         /* !< DMA-Pre interrupt for Trigger
                                                                                    USB-B-RX */
#define USB_IMASK_DMAPREBRX_CLR                  ((uint32_t)0x00000000U)         /* !< Interrupt Disabled. */
#define USB_IMASK_DMAPREBRX_SET                  ((uint32_t)0x00004000U)         /* !< Interrupt Enabled. */
/* USB_IMASK[DMAPREBTX] Bits */
#define USB_IMASK_DMAPREBTX_OFS                  (15)                            /* !< DMAPREBTX Offset */
#define USB_IMASK_DMAPREBTX_MASK                 ((uint32_t)0x00008000U)         /* !< DMA-Pre interrupt for Trigger
                                                                                    USB-B-TX */
#define USB_IMASK_DMAPREBTX_CLR                  ((uint32_t)0x00000000U)         /* !< Interrupt Disabled. */
#define USB_IMASK_DMAPREBTX_SET                  ((uint32_t)0x00008000U)         /* !< Interrupt Enabled. */
/* USB_IMASK[DMAPRECRX] Bits */
#define USB_IMASK_DMAPRECRX_OFS                  (16)                            /* !< DMAPRECRX Offset */
#define USB_IMASK_DMAPRECRX_MASK                 ((uint32_t)0x00010000U)         /* !< DMA-Pre interrupt for Trigger
                                                                                    USB-C-RX */
#define USB_IMASK_DMAPRECRX_CLR                  ((uint32_t)0x00000000U)         /* !< Interrupt Disabled. */
#define USB_IMASK_DMAPRECRX_SET                  ((uint32_t)0x00010000U)         /* !< Interrupt Enabled. */
/* USB_IMASK[DMAPRECTX] Bits */
#define USB_IMASK_DMAPRECTX_OFS                  (17)                            /* !< DMAPRECTX Offset */
#define USB_IMASK_DMAPRECTX_MASK                 ((uint32_t)0x00020000U)         /* !< DMA-Pre interrupt for Trigger
                                                                                    USB-C-TX */
#define USB_IMASK_DMAPRECTX_CLR                  ((uint32_t)0x00000000U)         /* !< Interrupt Disabled. */
#define USB_IMASK_DMAPRECTX_SET                  ((uint32_t)0x00020000U)         /* !< Interrupt Enabled. */
/* USB_IMASK[DMAPREDRX] Bits */
#define USB_IMASK_DMAPREDRX_OFS                  (18)                            /* !< DMAPREDRX Offset */
#define USB_IMASK_DMAPREDRX_MASK                 ((uint32_t)0x00040000U)         /* !< DMA-Pre interrupt for Trigger
                                                                                    USB-D-RX */
#define USB_IMASK_DMAPREDRX_CLR                  ((uint32_t)0x00000000U)         /* !< Interrupt Disabled. */
#define USB_IMASK_DMAPREDRX_SET                  ((uint32_t)0x00040000U)         /* !< Interrupt Enabled. */
/* USB_IMASK[DMAPREDTX] Bits */
#define USB_IMASK_DMAPREDTX_OFS                  (19)                            /* !< DMAPREDTX Offset */
#define USB_IMASK_DMAPREDTX_MASK                 ((uint32_t)0x00080000U)         /* !< DMA-Pre interrupt for Trigger
                                                                                    USB-D-TX */
#define USB_IMASK_DMAPREDTX_CLR                  ((uint32_t)0x00000000U)         /* !< Interrupt Disabled. */
#define USB_IMASK_DMAPREDTX_SET                  ((uint32_t)0x00080000U)         /* !< Interrupt Enabled. */

/* USB_RIS Bits */
/* USB_RIS[INTRTX] Bits */
#define USB_RIS_INTRTX_OFS                       (0)                             /* !< INTRTX Offset */
#define USB_RIS_INTRTX_MASK                      ((uint32_t)0x00000001U)         /* !< Endpoint 0 and the TX Endpoints
                                                                                    interrupt */
#define USB_RIS_INTRTX_CLR                       ((uint32_t)0x00000000U)         /* !< Interrupt did not occur */
#define USB_RIS_INTRTX_SET                       ((uint32_t)0x00000001U)         /* !< Interrupt occured */
/* USB_RIS[INTRRX] Bits */
#define USB_RIS_INTRRX_OFS                       (1)                             /* !< INTRRX Offset */
#define USB_RIS_INTRRX_MASK                      ((uint32_t)0x00000002U)         /* !< RX Endpoints interrupt */
#define USB_RIS_INTRRX_CLR                       ((uint32_t)0x00000000U)         /* !< Interrupt did not occur */
#define USB_RIS_INTRRX_SET                       ((uint32_t)0x00000002U)         /* !< Interrupt occured */
/* USB_RIS[INTRUSB] Bits */
#define USB_RIS_INTRUSB_OFS                      (2)                             /* !< INTRUSB Offset */
#define USB_RIS_INTRUSB_MASK                     ((uint32_t)0x00000004U)         /* !< USB Interrupts */
#define USB_RIS_INTRUSB_CLR                      ((uint32_t)0x00000000U)         /* !< Interrupt did not occur */
#define USB_RIS_INTRUSB_SET                      ((uint32_t)0x00000004U)         /* !< Interrupt occured */
/* USB_RIS[VUSBPWRDN] Bits */
#define USB_RIS_VUSBPWRDN_OFS                    (3)                             /* !< VUSBPWRDN Offset */
#define USB_RIS_VUSBPWRDN_MASK                   ((uint32_t)0x00000008U)         /* !< VUSB Power Down */
#define USB_RIS_VUSBPWRDN_CLR                    ((uint32_t)0x00000000U)         /* !< Interrupt did not occur */
#define USB_RIS_VUSBPWRDN_SET                    ((uint32_t)0x00000008U)         /* !< Interrupt occured */
/* USB_RIS[DMADONEARX] Bits */
#define USB_RIS_DMADONEARX_OFS                   (4)                             /* !< DMADONEARX Offset */
#define USB_RIS_DMADONEARX_MASK                  ((uint32_t)0x00000010U)         /* !< DMA-Done interrupt for Trigger
                                                                                    USB-A-RX */
#define USB_RIS_DMADONEARX_CLR                   ((uint32_t)0x00000000U)         /* !< Interrupt did not occur */
#define USB_RIS_DMADONEARX_SET                   ((uint32_t)0x00000010U)         /* !< Interrupt occured */
/* USB_RIS[DMADONEATX] Bits */
#define USB_RIS_DMADONEATX_OFS                   (5)                             /* !< DMADONEATX Offset */
#define USB_RIS_DMADONEATX_MASK                  ((uint32_t)0x00000020U)         /* !< DMA-Done interrupt for Trigger
                                                                                    USB-A-TX */
#define USB_RIS_DMADONEATX_CLR                   ((uint32_t)0x00000000U)         /* !< Interrupt did not occur */
#define USB_RIS_DMADONEATX_SET                   ((uint32_t)0x00000020U)         /* !< Interrupt occured */
/* USB_RIS[DMADONEBRX] Bits */
#define USB_RIS_DMADONEBRX_OFS                   (6)                             /* !< DMADONEBRX Offset */
#define USB_RIS_DMADONEBRX_MASK                  ((uint32_t)0x00000040U)         /* !< DMA-Done interrupt for Trigger
                                                                                    USB-B-RX */
#define USB_RIS_DMADONEBRX_CLR                   ((uint32_t)0x00000000U)         /* !< Interrupt did not occur */
#define USB_RIS_DMADONEBRX_SET                   ((uint32_t)0x00000040U)         /* !< Interrupt occured */
/* USB_RIS[DMADONEBTX] Bits */
#define USB_RIS_DMADONEBTX_OFS                   (7)                             /* !< DMADONEBTX Offset */
#define USB_RIS_DMADONEBTX_MASK                  ((uint32_t)0x00000080U)         /* !< DMA-Done interrupt for Trigger
                                                                                    USB-B-TX */
#define USB_RIS_DMADONEBTX_CLR                   ((uint32_t)0x00000000U)         /* !< Interrupt did not occur */
#define USB_RIS_DMADONEBTX_SET                   ((uint32_t)0x00000080U)         /* !< Interrupt occured */
/* USB_RIS[DMADONECRX] Bits */
#define USB_RIS_DMADONECRX_OFS                   (8)                             /* !< DMADONECRX Offset */
#define USB_RIS_DMADONECRX_MASK                  ((uint32_t)0x00000100U)         /* !< DMA-Done interrupt for Trigger
                                                                                    USB-C-RX */
#define USB_RIS_DMADONECRX_CLR                   ((uint32_t)0x00000000U)         /* !< Interrupt did not occur */
#define USB_RIS_DMADONECRX_SET                   ((uint32_t)0x00000100U)         /* !< Interrupt occured */
/* USB_RIS[DMADONECTX] Bits */
#define USB_RIS_DMADONECTX_OFS                   (9)                             /* !< DMADONECTX Offset */
#define USB_RIS_DMADONECTX_MASK                  ((uint32_t)0x00000200U)         /* !< DMA-Done interrupt for Trigger
                                                                                    USB-C-TX */
#define USB_RIS_DMADONECTX_CLR                   ((uint32_t)0x00000000U)         /* !< Interrupt did not occur */
#define USB_RIS_DMADONECTX_SET                   ((uint32_t)0x00000200U)         /* !< Interrupt occured */
/* USB_RIS[DMADONEDRX] Bits */
#define USB_RIS_DMADONEDRX_OFS                   (10)                            /* !< DMADONEDRX Offset */
#define USB_RIS_DMADONEDRX_MASK                  ((uint32_t)0x00000400U)         /* !< DMA-Done interrupt for Trigger
                                                                                    USB-D-RX */
#define USB_RIS_DMADONEDRX_CLR                   ((uint32_t)0x00000000U)         /* !< Interrupt did not occur */
#define USB_RIS_DMADONEDRX_SET                   ((uint32_t)0x00000400U)         /* !< Interrupt occured */
/* USB_RIS[DMADONEDTX] Bits */
#define USB_RIS_DMADONEDTX_OFS                   (11)                            /* !< DMADONEDTX Offset */
#define USB_RIS_DMADONEDTX_MASK                  ((uint32_t)0x00000800U)         /* !< DMA-Done interrupt for Trigger
                                                                                    USB-D-TX */
#define USB_RIS_DMADONEDTX_CLR                   ((uint32_t)0x00000000U)         /* !< Interrupt did not occur */
#define USB_RIS_DMADONEDTX_SET                   ((uint32_t)0x00000800U)         /* !< Interrupt occured */
/* USB_RIS[DMAPREARX] Bits */
#define USB_RIS_DMAPREARX_OFS                    (12)                            /* !< DMAPREARX Offset */
#define USB_RIS_DMAPREARX_MASK                   ((uint32_t)0x00001000U)         /* !< DMA-Pre interrupt for Trigger
                                                                                    USB-A-RX */
#define USB_RIS_DMAPREARX_CLR                    ((uint32_t)0x00000000U)         /* !< Interrupt did not occur */
#define USB_RIS_DMAPREARX_SET                    ((uint32_t)0x00001000U)         /* !< Interrupt occured */
/* USB_RIS[DMAPREATX] Bits */
#define USB_RIS_DMAPREATX_OFS                    (13)                            /* !< DMAPREATX Offset */
#define USB_RIS_DMAPREATX_MASK                   ((uint32_t)0x00002000U)         /* !< DMA-Pre interrupt for Trigger
                                                                                    USB-A-TX */
#define USB_RIS_DMAPREATX_CLR                    ((uint32_t)0x00000000U)         /* !< Interrupt did not occur */
#define USB_RIS_DMAPREATX_SET                    ((uint32_t)0x00002000U)         /* !< Interrupt occured */
/* USB_RIS[DMAPREBRX] Bits */
#define USB_RIS_DMAPREBRX_OFS                    (14)                            /* !< DMAPREBRX Offset */
#define USB_RIS_DMAPREBRX_MASK                   ((uint32_t)0x00004000U)         /* !< DMA-Pre interrupt for Trigger
                                                                                    USB-B-RX */
#define USB_RIS_DMAPREBRX_CLR                    ((uint32_t)0x00000000U)         /* !< Interrupt did not occur */
#define USB_RIS_DMAPREBRX_SET                    ((uint32_t)0x00004000U)         /* !< Interrupt occured */
/* USB_RIS[DMAPREBTX] Bits */
#define USB_RIS_DMAPREBTX_OFS                    (15)                            /* !< DMAPREBTX Offset */
#define USB_RIS_DMAPREBTX_MASK                   ((uint32_t)0x00008000U)         /* !< DMA-Pre interrupt for Trigger
                                                                                    USB-B-TX */
#define USB_RIS_DMAPREBTX_CLR                    ((uint32_t)0x00000000U)         /* !< Interrupt did not occur */
#define USB_RIS_DMAPREBTX_SET                    ((uint32_t)0x00008000U)         /* !< Interrupt occured */
/* USB_RIS[DMAPRECRX] Bits */
#define USB_RIS_DMAPRECRX_OFS                    (16)                            /* !< DMAPRECRX Offset */
#define USB_RIS_DMAPRECRX_MASK                   ((uint32_t)0x00010000U)         /* !< DMA-Pre interrupt for Trigger
                                                                                    USB-C-RX */
#define USB_RIS_DMAPRECRX_CLR                    ((uint32_t)0x00000000U)         /* !< Interrupt did not occur */
#define USB_RIS_DMAPRECRX_SET                    ((uint32_t)0x00010000U)         /* !< Interrupt occured */
/* USB_RIS[DMAPRECTX] Bits */
#define USB_RIS_DMAPRECTX_OFS                    (17)                            /* !< DMAPRECTX Offset */
#define USB_RIS_DMAPRECTX_MASK                   ((uint32_t)0x00020000U)         /* !< DMA-Pre interrupt for Trigger
                                                                                    USB-C-TX */
#define USB_RIS_DMAPRECTX_CLR                    ((uint32_t)0x00000000U)         /* !< Interrupt did not occur */
#define USB_RIS_DMAPRECTX_SET                    ((uint32_t)0x00020000U)         /* !< Interrupt occured */
/* USB_RIS[DMAPREDRX] Bits */
#define USB_RIS_DMAPREDRX_OFS                    (18)                            /* !< DMAPREDRX Offset */
#define USB_RIS_DMAPREDRX_MASK                   ((uint32_t)0x00040000U)         /* !< DMA-Pre interrupt for Trigger
                                                                                    USB-D-RX */
#define USB_RIS_DMAPREDRX_CLR                    ((uint32_t)0x00000000U)         /* !< Interrupt did not occur */
#define USB_RIS_DMAPREDRX_SET                    ((uint32_t)0x00040000U)         /* !< Interrupt occured */
/* USB_RIS[DMAPREDTX] Bits */
#define USB_RIS_DMAPREDTX_OFS                    (19)                            /* !< DMAPREDTX Offset */
#define USB_RIS_DMAPREDTX_MASK                   ((uint32_t)0x00080000U)         /* !< DMA-Pre interrupt for Trigger
                                                                                    USB-D-TX */
#define USB_RIS_DMAPREDTX_CLR                    ((uint32_t)0x00000000U)         /* !< Interrupt did not occur */
#define USB_RIS_DMAPREDTX_SET                    ((uint32_t)0x00080000U)         /* !< Interrupt occured */

/* USB_MIS Bits */
/* USB_MIS[INTRTX] Bits */
#define USB_MIS_INTRTX_OFS                       (0)                             /* !< INTRTX Offset */
#define USB_MIS_INTRTX_MASK                      ((uint32_t)0x00000001U)         /* !< Endpoint 0 and the TX Endpoints
                                                                                    interrupt */
#define USB_MIS_INTRTX_CLR                       ((uint32_t)0x00000000U)         /* !< Interrupt did not occur */
#define USB_MIS_INTRTX_SET                       ((uint32_t)0x00000001U)         /* !< Interrupt occured */
/* USB_MIS[INTRRX] Bits */
#define USB_MIS_INTRRX_OFS                       (1)                             /* !< INTRRX Offset */
#define USB_MIS_INTRRX_MASK                      ((uint32_t)0x00000002U)         /* !< RX Endpoints interrupt */
#define USB_MIS_INTRRX_CLR                       ((uint32_t)0x00000000U)         /* !< Interrupt did not occur */
#define USB_MIS_INTRRX_SET                       ((uint32_t)0x00000002U)         /* !< Interrupt occured */
/* USB_MIS[INTRUSB] Bits */
#define USB_MIS_INTRUSB_OFS                      (2)                             /* !< INTRUSB Offset */
#define USB_MIS_INTRUSB_MASK                     ((uint32_t)0x00000004U)         /* !< USB Interrupts */
#define USB_MIS_INTRUSB_CLR                      ((uint32_t)0x00000000U)         /* !< Interrupt did not occur */
#define USB_MIS_INTRUSB_SET                      ((uint32_t)0x00000004U)         /* !< Interrupt occured */
/* USB_MIS[VUSBPWRDN] Bits */
#define USB_MIS_VUSBPWRDN_OFS                    (3)                             /* !< VUSBPWRDN Offset */
#define USB_MIS_VUSBPWRDN_MASK                   ((uint32_t)0x00000008U)         /* !< VUSB Power Down */
#define USB_MIS_VUSBPWRDN_CLR                    ((uint32_t)0x00000000U)         /* !< Interrupt did not occur */
#define USB_MIS_VUSBPWRDN_SET                    ((uint32_t)0x00000008U)         /* !< Interrupt occured */
/* USB_MIS[DMADONEARX] Bits */
#define USB_MIS_DMADONEARX_OFS                   (4)                             /* !< DMADONEARX Offset */
#define USB_MIS_DMADONEARX_MASK                  ((uint32_t)0x00000010U)         /* !< DMA-Done interrupt for Trigger
                                                                                    USB-A-RX */
#define USB_MIS_DMADONEARX_CLR                   ((uint32_t)0x00000000U)         /* !< Interrupt did not occur */
#define USB_MIS_DMADONEARX_SET                   ((uint32_t)0x00000010U)         /* !< Interrupt occured */
/* USB_MIS[DMADONEATX] Bits */
#define USB_MIS_DMADONEATX_OFS                   (5)                             /* !< DMADONEATX Offset */
#define USB_MIS_DMADONEATX_MASK                  ((uint32_t)0x00000020U)         /* !< DMA-Done interrupt for Trigger
                                                                                    USB-A-TX */
#define USB_MIS_DMADONEATX_CLR                   ((uint32_t)0x00000000U)         /* !< Interrupt did not occur */
#define USB_MIS_DMADONEATX_SET                   ((uint32_t)0x00000020U)         /* !< Interrupt occured */
/* USB_MIS[DMADONEBRX] Bits */
#define USB_MIS_DMADONEBRX_OFS                   (6)                             /* !< DMADONEBRX Offset */
#define USB_MIS_DMADONEBRX_MASK                  ((uint32_t)0x00000040U)         /* !< DMA-Done interrupt for Trigger
                                                                                    USB-B-RX */
#define USB_MIS_DMADONEBRX_CLR                   ((uint32_t)0x00000000U)         /* !< Interrupt did not occur */
#define USB_MIS_DMADONEBRX_SET                   ((uint32_t)0x00000040U)         /* !< Interrupt occured */
/* USB_MIS[DMADONEBTX] Bits */
#define USB_MIS_DMADONEBTX_OFS                   (7)                             /* !< DMADONEBTX Offset */
#define USB_MIS_DMADONEBTX_MASK                  ((uint32_t)0x00000080U)         /* !< DMA-Done interrupt for Trigger
                                                                                    USB-B-TX */
#define USB_MIS_DMADONEBTX_CLR                   ((uint32_t)0x00000000U)         /* !< Interrupt did not occur */
#define USB_MIS_DMADONEBTX_SET                   ((uint32_t)0x00000080U)         /* !< Interrupt occured */
/* USB_MIS[DMADONECRX] Bits */
#define USB_MIS_DMADONECRX_OFS                   (8)                             /* !< DMADONECRX Offset */
#define USB_MIS_DMADONECRX_MASK                  ((uint32_t)0x00000100U)         /* !< DMA-Done interrupt for Trigger
                                                                                    USB-C-RX */
#define USB_MIS_DMADONECRX_CLR                   ((uint32_t)0x00000000U)         /* !< Interrupt did not occur */
#define USB_MIS_DMADONECRX_SET                   ((uint32_t)0x00000100U)         /* !< Interrupt occured */
/* USB_MIS[DMADONECTX] Bits */
#define USB_MIS_DMADONECTX_OFS                   (9)                             /* !< DMADONECTX Offset */
#define USB_MIS_DMADONECTX_MASK                  ((uint32_t)0x00000200U)         /* !< DMA-Done interrupt for Trigger
                                                                                    USB-C-TX */
#define USB_MIS_DMADONECTX_CLR                   ((uint32_t)0x00000000U)         /* !< Interrupt did not occur */
#define USB_MIS_DMADONECTX_SET                   ((uint32_t)0x00000200U)         /* !< Interrupt occured */
/* USB_MIS[DMADONEDRX] Bits */
#define USB_MIS_DMADONEDRX_OFS                   (10)                            /* !< DMADONEDRX Offset */
#define USB_MIS_DMADONEDRX_MASK                  ((uint32_t)0x00000400U)         /* !< DMA-Done interrupt for Trigger
                                                                                    USB-D-RX */
#define USB_MIS_DMADONEDRX_CLR                   ((uint32_t)0x00000000U)         /* !< Interrupt did not occur */
#define USB_MIS_DMADONEDRX_SET                   ((uint32_t)0x00000400U)         /* !< Interrupt occured */
/* USB_MIS[DMADONEDTX] Bits */
#define USB_MIS_DMADONEDTX_OFS                   (11)                            /* !< DMADONEDTX Offset */
#define USB_MIS_DMADONEDTX_MASK                  ((uint32_t)0x00000800U)         /* !< DMA-Done interrupt for Trigger
                                                                                    USB-D-TX */
#define USB_MIS_DMADONEDTX_CLR                   ((uint32_t)0x00000000U)         /* !< Interrupt did not occur */
#define USB_MIS_DMADONEDTX_SET                   ((uint32_t)0x00000800U)         /* !< Interrupt occured */
/* USB_MIS[DMAPREARX] Bits */
#define USB_MIS_DMAPREARX_OFS                    (12)                            /* !< DMAPREARX Offset */
#define USB_MIS_DMAPREARX_MASK                   ((uint32_t)0x00001000U)         /* !< DMA-Pre interrupt for Trigger
                                                                                    USB-A-RX */
#define USB_MIS_DMAPREARX_CLR                    ((uint32_t)0x00000000U)         /* !< Interrupt did not occur */
#define USB_MIS_DMAPREARX_SET                    ((uint32_t)0x00001000U)         /* !< Interrupt occured */
/* USB_MIS[DMAPREATX] Bits */
#define USB_MIS_DMAPREATX_OFS                    (13)                            /* !< DMAPREATX Offset */
#define USB_MIS_DMAPREATX_MASK                   ((uint32_t)0x00002000U)         /* !< DMA-Pre interrupt for Trigger
                                                                                    USB-A-TX */
#define USB_MIS_DMAPREATX_CLR                    ((uint32_t)0x00000000U)         /* !< Interrupt did not occur */
#define USB_MIS_DMAPREATX_SET                    ((uint32_t)0x00002000U)         /* !< Interrupt occured */
/* USB_MIS[DMAPREBRX] Bits */
#define USB_MIS_DMAPREBRX_OFS                    (14)                            /* !< DMAPREBRX Offset */
#define USB_MIS_DMAPREBRX_MASK                   ((uint32_t)0x00004000U)         /* !< DMA-Pre interrupt for Trigger
                                                                                    USB-B-RX */
#define USB_MIS_DMAPREBRX_CLR                    ((uint32_t)0x00000000U)         /* !< Interrupt did not occur */
#define USB_MIS_DMAPREBRX_SET                    ((uint32_t)0x00004000U)         /* !< Interrupt occured */
/* USB_MIS[DMAPREBTX] Bits */
#define USB_MIS_DMAPREBTX_OFS                    (15)                            /* !< DMAPREBTX Offset */
#define USB_MIS_DMAPREBTX_MASK                   ((uint32_t)0x00008000U)         /* !< DMA-Pre interrupt for Trigger
                                                                                    USB-B-TX */
#define USB_MIS_DMAPREBTX_CLR                    ((uint32_t)0x00000000U)         /* !< Interrupt did not occur */
#define USB_MIS_DMAPREBTX_SET                    ((uint32_t)0x00008000U)         /* !< Interrupt occured */
/* USB_MIS[DMAPRECRX] Bits */
#define USB_MIS_DMAPRECRX_OFS                    (16)                            /* !< DMAPRECRX Offset */
#define USB_MIS_DMAPRECRX_MASK                   ((uint32_t)0x00010000U)         /* !< DMA-Pre interrupt for Trigger
                                                                                    USB-C-RX */
#define USB_MIS_DMAPRECRX_CLR                    ((uint32_t)0x00000000U)         /* !< Interrupt did not occur */
#define USB_MIS_DMAPRECRX_SET                    ((uint32_t)0x00010000U)         /* !< Interrupt occured */
/* USB_MIS[DMAPRECTX] Bits */
#define USB_MIS_DMAPRECTX_OFS                    (17)                            /* !< DMAPRECTX Offset */
#define USB_MIS_DMAPRECTX_MASK                   ((uint32_t)0x00020000U)         /* !< DMA-Pre interrupt for Trigger
                                                                                    USB-C-TX */
#define USB_MIS_DMAPRECTX_CLR                    ((uint32_t)0x00000000U)         /* !< Interrupt did not occur */
#define USB_MIS_DMAPRECTX_SET                    ((uint32_t)0x00020000U)         /* !< Interrupt occured */
/* USB_MIS[DMAPREDRX] Bits */
#define USB_MIS_DMAPREDRX_OFS                    (18)                            /* !< DMAPREDRX Offset */
#define USB_MIS_DMAPREDRX_MASK                   ((uint32_t)0x00040000U)         /* !< DMA-Pre interrupt for Trigger
                                                                                    USB-D-RX */
#define USB_MIS_DMAPREDRX_CLR                    ((uint32_t)0x00000000U)         /* !< Interrupt did not occur */
#define USB_MIS_DMAPREDRX_SET                    ((uint32_t)0x00040000U)         /* !< Interrupt occured */
/* USB_MIS[DMAPREDTX] Bits */
#define USB_MIS_DMAPREDTX_OFS                    (19)                            /* !< DMAPREDTX Offset */
#define USB_MIS_DMAPREDTX_MASK                   ((uint32_t)0x00080000U)         /* !< DMA-Pre interrupt for Trigger
                                                                                    USB-D-TX */
#define USB_MIS_DMAPREDTX_CLR                    ((uint32_t)0x00000000U)         /* !< Interrupt did not occur */
#define USB_MIS_DMAPREDTX_SET                    ((uint32_t)0x00080000U)         /* !< Interrupt occured */

/* USB_ISET Bits */
/* USB_ISET[INTRTX] Bits */
#define USB_ISET_INTRTX_OFS                      (0)                             /* !< INTRTX Offset */
#define USB_ISET_INTRTX_MASK                     ((uint32_t)0x00000001U)         /* !< Endpoint 0 and the TX Endpoints
                                                                                    interrupt */
#define USB_ISET_INTRTX_NO_EFFECT                ((uint32_t)0x00000000U)         /* !< Writing 0 has No Effect */
#define USB_ISET_INTRTX_SET                      ((uint32_t)0x00000001U)         /* !< Write 1 to set the interrupt */
/* USB_ISET[INTRRX] Bits */
#define USB_ISET_INTRRX_OFS                      (1)                             /* !< INTRRX Offset */
#define USB_ISET_INTRRX_MASK                     ((uint32_t)0x00000002U)         /* !< RX Endpoints interrupt */
#define USB_ISET_INTRRX_NO_EFFECT                ((uint32_t)0x00000000U)         /* !< Writing 0 has No Effect */
#define USB_ISET_INTRRX_SET                      ((uint32_t)0x00000002U)         /* !< Write 1 to set the interrupt */
/* USB_ISET[INTRUSB] Bits */
#define USB_ISET_INTRUSB_OFS                     (2)                             /* !< INTRUSB Offset */
#define USB_ISET_INTRUSB_MASK                    ((uint32_t)0x00000004U)         /* !< USB Interrupts */
#define USB_ISET_INTRUSB_NO_EFFECT               ((uint32_t)0x00000000U)         /* !< Writing 0 has No Effect */
#define USB_ISET_INTRUSB_SET                     ((uint32_t)0x00000004U)         /* !< Write 1 to set the interrupt */
/* USB_ISET[VUSBPWRDN] Bits */
#define USB_ISET_VUSBPWRDN_OFS                   (3)                             /* !< VUSBPWRDN Offset */
#define USB_ISET_VUSBPWRDN_MASK                  ((uint32_t)0x00000008U)         /* !< VUSB Power Down */
#define USB_ISET_VUSBPWRDN_NO_EFFECT             ((uint32_t)0x00000000U)         /* !< Writing 0 has No Effect */
#define USB_ISET_VUSBPWRDN_SET                   ((uint32_t)0x00000008U)         /* !< Write 1 to set the interrupt */
/* USB_ISET[DMADONEARX] Bits */
#define USB_ISET_DMADONEARX_OFS                  (4)                             /* !< DMADONEARX Offset */
#define USB_ISET_DMADONEARX_MASK                 ((uint32_t)0x00000010U)         /* !< DMA-Done interrupt for Trigger
                                                                                    USB-A-RX */
#define USB_ISET_DMADONEARX_NO_EFFECT            ((uint32_t)0x00000000U)         /* !< Writing 0 has No Effect */
#define USB_ISET_DMADONEARX_SET                  ((uint32_t)0x00000010U)         /* !< Write 1 to set the interrupt */
/* USB_ISET[DMADONEATX] Bits */
#define USB_ISET_DMADONEATX_OFS                  (5)                             /* !< DMADONEATX Offset */
#define USB_ISET_DMADONEATX_MASK                 ((uint32_t)0x00000020U)         /* !< DMA-Done interrupt for Trigger
                                                                                    USB-A-TX */
#define USB_ISET_DMADONEATX_NO_EFFECT            ((uint32_t)0x00000000U)         /* !< Writing 0 has No Effect */
#define USB_ISET_DMADONEATX_SET                  ((uint32_t)0x00000020U)         /* !< Write 1 to set the interrupt */
/* USB_ISET[DMADONEBRX] Bits */
#define USB_ISET_DMADONEBRX_OFS                  (6)                             /* !< DMADONEBRX Offset */
#define USB_ISET_DMADONEBRX_MASK                 ((uint32_t)0x00000040U)         /* !< DMA-Done interrupt for Trigger
                                                                                    USB-B-RX */
#define USB_ISET_DMADONEBRX_NO_EFFECT            ((uint32_t)0x00000000U)         /* !< Writing 0 has No Effect */
#define USB_ISET_DMADONEBRX_SET                  ((uint32_t)0x00000040U)         /* !< Write 1 to set the interrupt */
/* USB_ISET[DMADONEBTX] Bits */
#define USB_ISET_DMADONEBTX_OFS                  (7)                             /* !< DMADONEBTX Offset */
#define USB_ISET_DMADONEBTX_MASK                 ((uint32_t)0x00000080U)         /* !< DMA-Done interrupt for Trigger
                                                                                    USB-B-TX */
#define USB_ISET_DMADONEBTX_NO_EFFECT            ((uint32_t)0x00000000U)         /* !< Writing 0 has No Effect */
#define USB_ISET_DMADONEBTX_SET                  ((uint32_t)0x00000080U)         /* !< Write 1 to set the interrupt */
/* USB_ISET[DMADONECRX] Bits */
#define USB_ISET_DMADONECRX_OFS                  (8)                             /* !< DMADONECRX Offset */
#define USB_ISET_DMADONECRX_MASK                 ((uint32_t)0x00000100U)         /* !< DMA-Done interrupt for Trigger
                                                                                    USB-C-RX */
#define USB_ISET_DMADONECRX_NO_EFFECT            ((uint32_t)0x00000000U)         /* !< Writing 0 has No Effect */
#define USB_ISET_DMADONECRX_SET                  ((uint32_t)0x00000100U)         /* !< Write 1 to set the interrupt */
/* USB_ISET[DMADONECTX] Bits */
#define USB_ISET_DMADONECTX_OFS                  (9)                             /* !< DMADONECTX Offset */
#define USB_ISET_DMADONECTX_MASK                 ((uint32_t)0x00000200U)         /* !< DMA-Done interrupt for Trigger
                                                                                    USB-C-TX */
#define USB_ISET_DMADONECTX_NO_EFFECT            ((uint32_t)0x00000000U)         /* !< Writing 0 has No Effect */
#define USB_ISET_DMADONECTX_SET                  ((uint32_t)0x00000200U)         /* !< Write 1 to set the interrupt */
/* USB_ISET[DMADONEDRX] Bits */
#define USB_ISET_DMADONEDRX_OFS                  (10)                            /* !< DMADONEDRX Offset */
#define USB_ISET_DMADONEDRX_MASK                 ((uint32_t)0x00000400U)         /* !< DMA-Done interrupt for Trigger
                                                                                    USB-D-RX */
#define USB_ISET_DMADONEDRX_NO_EFFECT            ((uint32_t)0x00000000U)         /* !< Writing 0 has No Effect */
#define USB_ISET_DMADONEDRX_SET                  ((uint32_t)0x00000400U)         /* !< Write 1 to set the interrupt */
/* USB_ISET[DMADONEDTX] Bits */
#define USB_ISET_DMADONEDTX_OFS                  (11)                            /* !< DMADONEDTX Offset */
#define USB_ISET_DMADONEDTX_MASK                 ((uint32_t)0x00000800U)         /* !< DMA-Done interrupt for Trigger
                                                                                    USB-D-TX */
#define USB_ISET_DMADONEDTX_NO_EFFECT            ((uint32_t)0x00000000U)         /* !< Writing 0 has No Effect */
#define USB_ISET_DMADONEDTX_SET                  ((uint32_t)0x00000800U)         /* !< Write 1 to set the interrupt */
/* USB_ISET[DMAPREARX] Bits */
#define USB_ISET_DMAPREARX_OFS                   (12)                            /* !< DMAPREARX Offset */
#define USB_ISET_DMAPREARX_MASK                  ((uint32_t)0x00001000U)         /* !< DMA-Pre interrupt for Trigger
                                                                                    USB-A-RX */
#define USB_ISET_DMAPREARX_NO_EFFECT             ((uint32_t)0x00000000U)         /* !< Writing 0 has No Effect */
#define USB_ISET_DMAPREARX_SET                   ((uint32_t)0x00001000U)         /* !< Write 1 to set the interrupt */
/* USB_ISET[DMAPREATX] Bits */
#define USB_ISET_DMAPREATX_OFS                   (13)                            /* !< DMAPREATX Offset */
#define USB_ISET_DMAPREATX_MASK                  ((uint32_t)0x00002000U)         /* !< DMA-Pre interrupt for Trigger
                                                                                    USB-A-TX */
#define USB_ISET_DMAPREATX_NO_EFFECT             ((uint32_t)0x00000000U)         /* !< Writing 0 has No Effect */
#define USB_ISET_DMAPREATX_SET                   ((uint32_t)0x00002000U)         /* !< Write 1 to set the interrupt */
/* USB_ISET[DMAPREBRX] Bits */
#define USB_ISET_DMAPREBRX_OFS                   (14)                            /* !< DMAPREBRX Offset */
#define USB_ISET_DMAPREBRX_MASK                  ((uint32_t)0x00004000U)         /* !< DMA-Pre interrupt for Trigger
                                                                                    USB-B-RX */
#define USB_ISET_DMAPREBRX_NO_EFFECT             ((uint32_t)0x00000000U)         /* !< Writing 0 has No Effect */
#define USB_ISET_DMAPREBRX_SET                   ((uint32_t)0x00004000U)         /* !< Write 1 to set the interrupt */
/* USB_ISET[DMAPREBTX] Bits */
#define USB_ISET_DMAPREBTX_OFS                   (15)                            /* !< DMAPREBTX Offset */
#define USB_ISET_DMAPREBTX_MASK                  ((uint32_t)0x00008000U)         /* !< DMA-Pre interrupt for Trigger
                                                                                    USB-B-TX */
#define USB_ISET_DMAPREBTX_NO_EFFECT             ((uint32_t)0x00000000U)         /* !< Writing 0 has No Effect */
#define USB_ISET_DMAPREBTX_SET                   ((uint32_t)0x00008000U)         /* !< Write 1 to set the interrupt */
/* USB_ISET[DMAPRECRX] Bits */
#define USB_ISET_DMAPRECRX_OFS                   (16)                            /* !< DMAPRECRX Offset */
#define USB_ISET_DMAPRECRX_MASK                  ((uint32_t)0x00010000U)         /* !< DMA-Pre interrupt for Trigger
                                                                                    USB-C-RX */
#define USB_ISET_DMAPRECRX_NO_EFFECT             ((uint32_t)0x00000000U)         /* !< Writing 0 has No Effect */
#define USB_ISET_DMAPRECRX_SET                   ((uint32_t)0x00010000U)         /* !< Write 1 to set the interrupt */
/* USB_ISET[DMAPRECTX] Bits */
#define USB_ISET_DMAPRECTX_OFS                   (17)                            /* !< DMAPRECTX Offset */
#define USB_ISET_DMAPRECTX_MASK                  ((uint32_t)0x00020000U)         /* !< DMA-Pre interrupt for Trigger
                                                                                    USB-C-TX */
#define USB_ISET_DMAPRECTX_NO_EFFECT             ((uint32_t)0x00000000U)         /* !< Writing 0 has No Effect */
#define USB_ISET_DMAPRECTX_SET                   ((uint32_t)0x00020000U)         /* !< Write 1 to set the interrupt */
/* USB_ISET[DMAPREDRX] Bits */
#define USB_ISET_DMAPREDRX_OFS                   (18)                            /* !< DMAPREDRX Offset */
#define USB_ISET_DMAPREDRX_MASK                  ((uint32_t)0x00040000U)         /* !< DMA-Pre interrupt for Trigger
                                                                                    USB-D-RX */
#define USB_ISET_DMAPREDRX_NO_EFFECT             ((uint32_t)0x00000000U)         /* !< Writing 0 has No Effect */
#define USB_ISET_DMAPREDRX_SET                   ((uint32_t)0x00040000U)         /* !< Write 1 to set the interrupt */
/* USB_ISET[DMAPREDTX] Bits */
#define USB_ISET_DMAPREDTX_OFS                   (19)                            /* !< DMAPREDTX Offset */
#define USB_ISET_DMAPREDTX_MASK                  ((uint32_t)0x00080000U)         /* !< DMA-Pre interrupt for Trigger
                                                                                    USB-D-TX */
#define USB_ISET_DMAPREDTX_NO_EFFECT             ((uint32_t)0x00000000U)         /* !< Writing 0 has No Effect */
#define USB_ISET_DMAPREDTX_SET                   ((uint32_t)0x00080000U)         /* !< Write 1 to set the interrupt */

/* USB_ICLR Bits */
/* USB_ICLR[INTRTX] Bits */
#define USB_ICLR_INTRTX_OFS                      (0)                             /* !< INTRTX Offset */
#define USB_ICLR_INTRTX_MASK                     ((uint32_t)0x00000001U)         /* !< Endpoint 0 and the TX Endpoints
                                                                                    interrupt */
#define USB_ICLR_INTRTX_NO_EFFECT                ((uint32_t)0x00000000U)         /* !< Writing 0 has No Effect */
#define USB_ICLR_INTRTX_CLR                      ((uint32_t)0x00000001U)         /* !< Write 1 to clear the interrupt */
/* USB_ICLR[INTRRX] Bits */
#define USB_ICLR_INTRRX_OFS                      (1)                             /* !< INTRRX Offset */
#define USB_ICLR_INTRRX_MASK                     ((uint32_t)0x00000002U)         /* !< RX Endpoints interrupt */
#define USB_ICLR_INTRRX_NO_EFFECT                ((uint32_t)0x00000000U)         /* !< Writing 0 has No Effect */
#define USB_ICLR_INTRRX_CLR                      ((uint32_t)0x00000002U)         /* !< Write 1 to clear the interrupt */
/* USB_ICLR[INTRUSB] Bits */
#define USB_ICLR_INTRUSB_OFS                     (2)                             /* !< INTRUSB Offset */
#define USB_ICLR_INTRUSB_MASK                    ((uint32_t)0x00000004U)         /* !< USB Interrupts */
#define USB_ICLR_INTRUSB_NO_EFFECT               ((uint32_t)0x00000000U)         /* !< Writing 0 has No Effect */
#define USB_ICLR_INTRUSB_CLR                     ((uint32_t)0x00000004U)         /* !< Write 1 to clear the interrupt */
/* USB_ICLR[VUSBPWRDN] Bits */
#define USB_ICLR_VUSBPWRDN_OFS                   (3)                             /* !< VUSBPWRDN Offset */
#define USB_ICLR_VUSBPWRDN_MASK                  ((uint32_t)0x00000008U)         /* !< VUSB Power Down */
#define USB_ICLR_VUSBPWRDN_NO_EFFECT             ((uint32_t)0x00000000U)         /* !< Writing 0 has No Effect */
#define USB_ICLR_VUSBPWRDN_CLR                   ((uint32_t)0x00000008U)         /* !< Write 1 to clear the interrupt */
/* USB_ICLR[DMADONEARX] Bits */
#define USB_ICLR_DMADONEARX_OFS                  (4)                             /* !< DMADONEARX Offset */
#define USB_ICLR_DMADONEARX_MASK                 ((uint32_t)0x00000010U)         /* !< DMA-Done interrupt for Trigger
                                                                                    USB-A-RX */
#define USB_ICLR_DMADONEARX_NO_EFFECT            ((uint32_t)0x00000000U)         /* !< Writing 0 has No Effect */
#define USB_ICLR_DMADONEARX_CLR                  ((uint32_t)0x00000010U)         /* !< Write 1 to clear the interrupt */
/* USB_ICLR[DMADONEATX] Bits */
#define USB_ICLR_DMADONEATX_OFS                  (5)                             /* !< DMADONEATX Offset */
#define USB_ICLR_DMADONEATX_MASK                 ((uint32_t)0x00000020U)         /* !< DMA-Done interrupt for Trigger
                                                                                    USB-A-TX */
#define USB_ICLR_DMADONEATX_NO_EFFECT            ((uint32_t)0x00000000U)         /* !< Writing 0 has No Effect */
#define USB_ICLR_DMADONEATX_CLR                  ((uint32_t)0x00000020U)         /* !< Write 1 to clear the interrupt */
/* USB_ICLR[DMADONEBRX] Bits */
#define USB_ICLR_DMADONEBRX_OFS                  (6)                             /* !< DMADONEBRX Offset */
#define USB_ICLR_DMADONEBRX_MASK                 ((uint32_t)0x00000040U)         /* !< DMA-Done interrupt for Trigger
                                                                                    USB-B-RX */
#define USB_ICLR_DMADONEBRX_NO_EFFECT            ((uint32_t)0x00000000U)         /* !< Writing 0 has No Effect */
#define USB_ICLR_DMADONEBRX_CLR                  ((uint32_t)0x00000040U)         /* !< Write 1 to clear the interrupt */
/* USB_ICLR[DMADONEBTX] Bits */
#define USB_ICLR_DMADONEBTX_OFS                  (7)                             /* !< DMADONEBTX Offset */
#define USB_ICLR_DMADONEBTX_MASK                 ((uint32_t)0x00000080U)         /* !< DMA-Done interrupt for Trigger
                                                                                    USB-B-TX */
#define USB_ICLR_DMADONEBTX_NO_EFFECT            ((uint32_t)0x00000000U)         /* !< Writing 0 has No Effect */
#define USB_ICLR_DMADONEBTX_CLR                  ((uint32_t)0x00000080U)         /* !< Write 1 to clear the interrupt */
/* USB_ICLR[DMADONECRX] Bits */
#define USB_ICLR_DMADONECRX_OFS                  (8)                             /* !< DMADONECRX Offset */
#define USB_ICLR_DMADONECRX_MASK                 ((uint32_t)0x00000100U)         /* !< DMA-Done interrupt for Trigger
                                                                                    USB-C-RX */
#define USB_ICLR_DMADONECRX_NO_EFFECT            ((uint32_t)0x00000000U)         /* !< Writing 0 has No Effect */
#define USB_ICLR_DMADONECRX_CLR                  ((uint32_t)0x00000100U)         /* !< Write 1 to clear the interrupt */
/* USB_ICLR[DMADONECTX] Bits */
#define USB_ICLR_DMADONECTX_OFS                  (9)                             /* !< DMADONECTX Offset */
#define USB_ICLR_DMADONECTX_MASK                 ((uint32_t)0x00000200U)         /* !< DMA-Done interrupt for Trigger
                                                                                    USB-C-TX */
#define USB_ICLR_DMADONECTX_NO_EFFECT            ((uint32_t)0x00000000U)         /* !< Writing 0 has No Effect */
#define USB_ICLR_DMADONECTX_CLR                  ((uint32_t)0x00000200U)         /* !< Write 1 to clear the interrupt */
/* USB_ICLR[DMADONEDRX] Bits */
#define USB_ICLR_DMADONEDRX_OFS                  (10)                            /* !< DMADONEDRX Offset */
#define USB_ICLR_DMADONEDRX_MASK                 ((uint32_t)0x00000400U)         /* !< DMA-Done interrupt for Trigger
                                                                                    USB-D-RX */
#define USB_ICLR_DMADONEDRX_NO_EFFECT            ((uint32_t)0x00000000U)         /* !< Writing 0 has No Effect */
#define USB_ICLR_DMADONEDRX_CLR                  ((uint32_t)0x00000400U)         /* !< Write 1 to clear the interrupt */
/* USB_ICLR[DMADONEDTX] Bits */
#define USB_ICLR_DMADONEDTX_OFS                  (11)                            /* !< DMADONEDTX Offset */
#define USB_ICLR_DMADONEDTX_MASK                 ((uint32_t)0x00000800U)         /* !< DMA-Done interrupt for Trigger
                                                                                    USB-D-TX */
#define USB_ICLR_DMADONEDTX_NO_EFFECT            ((uint32_t)0x00000000U)         /* !< Writing 0 has No Effect */
#define USB_ICLR_DMADONEDTX_CLR                  ((uint32_t)0x00000800U)         /* !< Write 1 to clear the interrupt */
/* USB_ICLR[DMAPREARX] Bits */
#define USB_ICLR_DMAPREARX_OFS                   (12)                            /* !< DMAPREARX Offset */
#define USB_ICLR_DMAPREARX_MASK                  ((uint32_t)0x00001000U)         /* !< DMA-Pre interrupt for Trigger
                                                                                    USB-A-RX */
#define USB_ICLR_DMAPREARX_NO_EFFECT             ((uint32_t)0x00000000U)         /* !< Writing 0 has No Effect */
#define USB_ICLR_DMAPREARX_CLR                   ((uint32_t)0x00001000U)         /* !< Write 1 to clear the interrupt */
/* USB_ICLR[DMAPREATX] Bits */
#define USB_ICLR_DMAPREATX_OFS                   (13)                            /* !< DMAPREATX Offset */
#define USB_ICLR_DMAPREATX_MASK                  ((uint32_t)0x00002000U)         /* !< DMA-Pre interrupt for Trigger
                                                                                    USB-A-TX */
#define USB_ICLR_DMAPREATX_NO_EFFECT             ((uint32_t)0x00000000U)         /* !< Writing 0 has No Effect */
#define USB_ICLR_DMAPREATX_CLR                   ((uint32_t)0x00002000U)         /* !< Write 1 to clear the interrupt */
/* USB_ICLR[DMAPREBRX] Bits */
#define USB_ICLR_DMAPREBRX_OFS                   (14)                            /* !< DMAPREBRX Offset */
#define USB_ICLR_DMAPREBRX_MASK                  ((uint32_t)0x00004000U)         /* !< DMA-Pre interrupt for Trigger
                                                                                    USB-B-RX */
#define USB_ICLR_DMAPREBRX_NO_EFFECT             ((uint32_t)0x00000000U)         /* !< Writing 0 has No Effect */
#define USB_ICLR_DMAPREBRX_CLR                   ((uint32_t)0x00004000U)         /* !< Write 1 to clear the interrupt */
/* USB_ICLR[DMAPREBTX] Bits */
#define USB_ICLR_DMAPREBTX_OFS                   (15)                            /* !< DMAPREBTX Offset */
#define USB_ICLR_DMAPREBTX_MASK                  ((uint32_t)0x00008000U)         /* !< DMA-Pre interrupt for Trigger
                                                                                    USB-B-TX */
#define USB_ICLR_DMAPREBTX_NO_EFFECT             ((uint32_t)0x00000000U)         /* !< Writing 0 has No Effect */
#define USB_ICLR_DMAPREBTX_CLR                   ((uint32_t)0x00008000U)         /* !< Write 1 to clear the interrupt */
/* USB_ICLR[DMAPRECRX] Bits */
#define USB_ICLR_DMAPRECRX_OFS                   (16)                            /* !< DMAPRECRX Offset */
#define USB_ICLR_DMAPRECRX_MASK                  ((uint32_t)0x00010000U)         /* !< DMA-Pre interrupt for Trigger
                                                                                    USB-C-RX */
#define USB_ICLR_DMAPRECRX_NO_EFFECT             ((uint32_t)0x00000000U)         /* !< Writing 0 has No Effect */
#define USB_ICLR_DMAPRECRX_CLR                   ((uint32_t)0x00010000U)         /* !< Write 1 to clear the interrupt */
/* USB_ICLR[DMAPRECTX] Bits */
#define USB_ICLR_DMAPRECTX_OFS                   (17)                            /* !< DMAPRECTX Offset */
#define USB_ICLR_DMAPRECTX_MASK                  ((uint32_t)0x00020000U)         /* !< DMA-Pre interrupt for Trigger
                                                                                    USB-C-TX */
#define USB_ICLR_DMAPRECTX_NO_EFFECT             ((uint32_t)0x00000000U)         /* !< Writing 0 has No Effect */
#define USB_ICLR_DMAPRECTX_CLR                   ((uint32_t)0x00020000U)         /* !< Write 1 to clear the interrupt */
/* USB_ICLR[DMAPREDRX] Bits */
#define USB_ICLR_DMAPREDRX_OFS                   (18)                            /* !< DMAPREDRX Offset */
#define USB_ICLR_DMAPREDRX_MASK                  ((uint32_t)0x00040000U)         /* !< DMA-Pre interrupt for Trigger
                                                                                    USB-D-RX */
#define USB_ICLR_DMAPREDRX_NO_EFFECT             ((uint32_t)0x00000000U)         /* !< Writing 0 has No Effect */
#define USB_ICLR_DMAPREDRX_CLR                   ((uint32_t)0x00040000U)         /* !< Write 1 to clear the interrupt */
/* USB_ICLR[DMAPREDTX] Bits */
#define USB_ICLR_DMAPREDTX_OFS                   (19)                            /* !< DMAPREDTX Offset */
#define USB_ICLR_DMAPREDTX_MASK                  ((uint32_t)0x00080000U)         /* !< DMA-Pre interrupt for Trigger
                                                                                    USB-D-TX */
#define USB_ICLR_DMAPREDTX_NO_EFFECT             ((uint32_t)0x00000000U)         /* !< Writing 0 has No Effect */
#define USB_ICLR_DMAPREDTX_CLR                   ((uint32_t)0x00080000U)         /* !< Write 1 to clear the interrupt */

/* USB_PWREN Bits */
/* USB_PWREN[ENABLE] Bits */
#define USB_PWREN_ENABLE_OFS                     (0)                             /* !< ENABLE Offset */
#define USB_PWREN_ENABLE_MASK                    ((uint32_t)0x00000001U)         /* !< Enable the power */
#define USB_PWREN_ENABLE_DISABLE                 ((uint32_t)0x00000000U)         /* !< Disable Power */
#define USB_PWREN_ENABLE_ENABLE                  ((uint32_t)0x00000001U)         /* !< Enable Power */
/* USB_PWREN[KEY] Bits */
#define USB_PWREN_KEY_OFS                        (24)                            /* !< KEY Offset */
#define USB_PWREN_KEY_MASK                       ((uint32_t)0xFF000000U)         /* !< KEY to allow Power State Change */
#define USB_PWREN_KEY_UNLOCK_W                   ((uint32_t)0x26000000U)         /* !< KEY to allow write access to this
                                                                                    register */

/* USB_RSTCTL Bits */
/* USB_RSTCTL[RESETSTKYCLR] Bits */
#define USB_RSTCTL_RESETSTKYCLR_OFS              (1)                             /* !< RESETSTKYCLR Offset */
#define USB_RSTCTL_RESETSTKYCLR_MASK             ((uint32_t)0x00000002U)         /* !< Clear the RESETSTKY bit in the STAT
                                                                                    register */
#define USB_RSTCTL_RESETSTKYCLR_NOP              ((uint32_t)0x00000000U)         /* !< Writing 0 has no effect */
#define USB_RSTCTL_RESETSTKYCLR_CLR              ((uint32_t)0x00000002U)         /* !< Clear reset sticky bit */
/* USB_RSTCTL[RESETASSERT] Bits */
#define USB_RSTCTL_RESETASSERT_OFS               (0)                             /* !< RESETASSERT Offset */
#define USB_RSTCTL_RESETASSERT_MASK              ((uint32_t)0x00000001U)         /* !< Assert reset to the peripheral */
#define USB_RSTCTL_RESETASSERT_NOP               ((uint32_t)0x00000000U)         /* !< Writing 0 has no effect */
#define USB_RSTCTL_RESETASSERT_ASSERT            ((uint32_t)0x00000001U)         /* !< Assert reset */
/* USB_RSTCTL[KEY] Bits */
#define USB_RSTCTL_KEY_OFS                       (24)                            /* !< KEY Offset */
#define USB_RSTCTL_KEY_MASK                      ((uint32_t)0xFF000000U)         /* !< Unlock key */
#define USB_RSTCTL_KEY_UNLOCK_W                  ((uint32_t)0xB1000000U)         /* !< KEY to allow write access to this
                                                                                    register */

/* USB_CLKCTL Bits */
/* USB_CLKCTL[CLKDIV] Bits */
#define USB_CLKCTL_CLKDIV_OFS                    (0)                             /* !< CLKDIV Offset */
#define USB_CLKCTL_CLKDIV_MASK                   ((uint32_t)0x0000000FU)         /* !< Divider settings for the functional
                                                                                    clock */
#define USB_CLKCTL_CLKDIV_DIV_BY_1               ((uint32_t)0x00000000U)         /* !< Divide by 1 */
#define USB_CLKCTL_CLKDIV_DIV_BY_2               ((uint32_t)0x00000001U)         /* !< Divide by 2 */
#define USB_CLKCTL_CLKDIV_DIV_BY_3               ((uint32_t)0x00000002U)         /* !< Divide by 3 */
#define USB_CLKCTL_CLKDIV_DIV_BY_4               ((uint32_t)0x00000003U)         /* !< Divide by 4 */
#define USB_CLKCTL_CLKDIV_DIV_BY_5               ((uint32_t)0x00000004U)         /* !< Divide by 5 */
#define USB_CLKCTL_CLKDIV_DIV_BY_6               ((uint32_t)0x00000005U)         /* !< Divide by 6 */
#define USB_CLKCTL_CLKDIV_DIV_BY_10              ((uint32_t)0x00000006U)         /* !< Divide by 10 */
#define USB_CLKCTL_CLKDIV_DIV_BY_12              ((uint32_t)0x00000007U)         /* !< Divide by 12 */
#define USB_CLKCTL_CLKDIV_DIV_BY_15              ((uint32_t)0x00000008U)         /* !< Divide by 15 */
#define USB_CLKCTL_CLKDIV_DIV_BY_20              ((uint32_t)0x00000009U)         /* !< Divide by 20 */
#define USB_CLKCTL_CLKDIV_DIV_BY_30              ((uint32_t)0x0000000AU)         /* !< Divide by 30 */
#define USB_CLKCTL_CLKDIV_DIV_BY_60              ((uint32_t)0x0000000BU)         /* !< Divide by 60 */

/* USB_STAT Bits */
/* USB_STAT[ENABLE] Bits */
#define USB_STAT_ENABLE_OFS                      (8)                             /* !< ENABLE Offset */
#define USB_STAT_ENABLE_MASK                     ((uint32_t)0x00000100U)         /* !< Peripheral enable status */
#define USB_STAT_ENABLE_OFF                      ((uint32_t)0x00000000U)         /* !< Peripheral is disabled */
#define USB_STAT_ENABLE_ON                       ((uint32_t)0x00000100U)         /* !< Peripheral is enabled */
/* USB_STAT[RESETSTKY] Bits */
#define USB_STAT_RESETSTKY_OFS                   (16)                            /* !< RESETSTKY Offset */
#define USB_STAT_RESETSTKY_MASK                  ((uint32_t)0x00010000U)         /* !< This bit indicates, if the
                                                                                    peripheral was reset, since this bit
                                                                                    was cleared by RESETSTKYCLR in the
                                                                                    RSTCTL register */
#define USB_STAT_RESETSTKY_NORES                 ((uint32_t)0x00000000U)         /* !< The peripheral has not been reset
                                                                                    since this bit was last cleared by
                                                                                    RESETSTKYCLR in the RSTCTL register */
#define USB_STAT_RESETSTKY_RESET                 ((uint32_t)0x00010000U)         /* !< The peripheral was reset since the
                                                                                    last bit clear */
/* USB_STAT[SWENABLE] Bits */
#define USB_STAT_SWENABLE_OFS                    (0)                             /* !< SWENABLE Offset */
#define USB_STAT_SWENABLE_MASK                   ((uint32_t)0x00000001U)         /* !< Software enable status. Shows if
                                                                                    enabled via PWREN.ENABLE bit */
#define USB_STAT_SWENABLE_DISABLED               ((uint32_t)0x00000000U)         /* !< Not enabled via PWREN.ENABLE bit */
#define USB_STAT_SWENABLE_ENABLED                ((uint32_t)0x00000001U)         /* !< Enabled via PWREN.ENABLE bit */

/* USB_EVT_MODE Bits */
/* USB_EVT_MODE[EVT0_CFG] Bits */
#define USB_EVT_MODE_EVT0_CFG_OFS                (0)                             /* !< EVT0_CFG Offset */
#define USB_EVT_MODE_EVT0_CFG_MASK               ((uint32_t)0x00000003U)         /* !< Event line mode select for event
                                                                                    corresponding to
                                                                                    (IPSTANDARD.INT_EVENT)(0) */
#define USB_EVT_MODE_EVT0_CFG_DISABLE            ((uint32_t)0x00000000U)         /* !< The interrupt or event line is
                                                                                    disabled. */
#define USB_EVT_MODE_EVT0_CFG_SOFTWARE           ((uint32_t)0x00000001U)         /* !< The interrupt or event line is in
                                                                                    software mode. Software must clear
                                                                                    the RIS. */
#define USB_EVT_MODE_EVT0_CFG_HARDWARE           ((uint32_t)0x00000002U)         /* !< The interrupt or event line is in
                                                                                    hardware mode. The hardware (another
                                                                                    module) clears automatically the
                                                                                    associated RIS flag. */

/* USB_DESC Bits */
/* USB_DESC[MINREV] Bits */
#define USB_DESC_MINREV_OFS                      (0)                             /* !< MINREV Offset */
#define USB_DESC_MINREV_MASK                     ((uint32_t)0x0000000FU)         /* !< Minor rev of the IP */
#define USB_DESC_MINREV_MINIMUM                  ((uint32_t)0x00000000U)         /* !< Smallest value */
#define USB_DESC_MINREV_MAXIMUM                  ((uint32_t)0x0000000FU)         /* !< Highest possible value */
/* USB_DESC[MAJREV] Bits */
#define USB_DESC_MAJREV_OFS                      (4)                             /* !< MAJREV Offset */
#define USB_DESC_MAJREV_MASK                     ((uint32_t)0x000000F0U)         /* !< Major rev of the IP */
#define USB_DESC_MAJREV_MINIMUM                  ((uint32_t)0x00000000U)         /* !< Smallest value */
#define USB_DESC_MAJREV_MAXIMUM                  ((uint32_t)0x000000F0U)         /* !< Highest possible value */
/* USB_DESC[FEATUREVER] Bits */
#define USB_DESC_FEATUREVER_OFS                  (12)                            /* !< FEATUREVER Offset */
#define USB_DESC_FEATUREVER_MASK                 ((uint32_t)0x0000F000U)         /* !< Feature Set for the module
                                                                                    *instance* */
#define USB_DESC_FEATUREVER_MINIMUM              ((uint32_t)0x00000000U)         /* !< Smallest value */
#define USB_DESC_FEATUREVER_MAXIMUM              ((uint32_t)0x0000F000U)         /* !< Highest possible value */
/* USB_DESC[MODULEID] Bits */
#define USB_DESC_MODULEID_OFS                    (16)                            /* !< MODULEID Offset */
#define USB_DESC_MODULEID_MASK                   ((uint32_t)0xFFFF0000U)         /* !< Module identification contains a
                                                                                    unique peripheral identification
                                                                                    number. The assignments are
                                                                                    maintained in a central database for
                                                                                    all of the platform modules to ensure
                                                                                    uniqueness. */
#define USB_DESC_MODULEID_MINIMUM                ((uint32_t)0x00000000U)         /* !< Smallest value */
#define USB_DESC_MODULEID_MAXIMUM                ((uint32_t)0xFFFF0000U)         /* !< Highest possible value */

/* USB_USBMODE Bits */
/* USB_USBMODE[HOST] Bits */
#define USB_USBMODE_HOST_OFS                     (0)                             /* !< HOST Offset */
#define USB_USBMODE_HOST_MASK                    ((uint32_t)0x00000001U)         /* !< USB is configured as HOST mode */
#define USB_USBMODE_HOST_DISABLE                 ((uint32_t)0x00000000U)         /* !< USB is configured as DEVICE or
                                                                                    HOST. */
#define USB_USBMODE_HOST_ENABLE                  ((uint32_t)0x00000001U)         /* !< USB is configured as HOST */
/* USB_USBMODE[DEVICEONLY] Bits */
#define USB_USBMODE_DEVICEONLY_OFS               (1)                             /* !< DEVICEONLY Offset */
#define USB_USBMODE_DEVICEONLY_MASK              ((uint32_t)0x00000002U)         /* !< USB is configured as DEVICE_ONLY
                                                                                    mode */
#define USB_USBMODE_DEVICEONLY_DISABLE           ((uint32_t)0x00000000U)         /* !< USB is configured as DEVICE or
                                                                                    HOST. */
#define USB_USBMODE_DEVICEONLY_ENABLE            ((uint32_t)0x00000002U)         /* !< USB is configured as DEVICE only
                                                                                    mode. */
/* USB_USBMODE[PHYMODE] Bits */
#define USB_USBMODE_PHYMODE_OFS                  (4)                             /* !< PHYMODE Offset */
#define USB_USBMODE_PHYMODE_MASK                 ((uint32_t)0x00000010U)         /* !< USB PHY mode.  Defines whether
                                                                                    DP/DM pins are controled by USB
                                                                                    module or IOMUX/GPIO module. */
#define USB_USBMODE_PHYMODE_GPIO                 ((uint32_t)0x00000000U)         /* !< The DP/DM pins are assighed to
                                                                                    IOMUX/GPIO module, */
#define USB_USBMODE_PHYMODE_USB                  ((uint32_t)0x00000010U)         /* !< The DP/DM pins are assighed to USB
                                                                                    module. */

/* USB_USBMONITOR Bits */
/* USB_USBMONITOR[VUSBEN] Bits */
#define USB_USBMONITOR_VUSBEN_OFS                (0)                             /* !< VUSBEN Offset */
#define USB_USBMONITOR_VUSBEN_MASK               ((uint32_t)0x00000001U)         /* !< Enable the 1/3 resistor divider for
                                                                                    VUSB supply. Set this bit to monitor
                                                                                    the VUSB supply with the ADC. */
#define USB_USBMONITOR_VUSBEN_DISABLE            ((uint32_t)0x00000000U)         /* !< VUSB monitor disabled */
#define USB_USBMONITOR_VUSBEN_ENABLE             ((uint32_t)0x00000001U)         /* !< VUSB monitor enabled */

/* USB_USBDMASEL Bits */
/* USB_USBDMASEL[TRIGARX] Bits */
#define USB_USBDMASEL_TRIGARX_OFS                (0)                             /* !< TRIGARX Offset */
#define USB_USBDMASEL_TRIGARX_MASK               ((uint32_t)0x00000007U)         /* !< TRIG A RX select specifies the RX
                                                                                    mapping of the USB endpoint on DMA
                                                                                    trigger USB-A-RX */
#define USB_USBDMASEL_TRIGARX_DIS                ((uint32_t)0x00000000U)         /* !< Disabled */
#define USB_USBDMASEL_TRIGARX_EP1                ((uint32_t)0x00000001U)         /* !< Endpoint 1 */
#define USB_USBDMASEL_TRIGARX_EP2                ((uint32_t)0x00000002U)         /* !< Endpoint 2 */
#define USB_USBDMASEL_TRIGARX_EP3                ((uint32_t)0x00000003U)         /* !< Endpoint 3 */
#define USB_USBDMASEL_TRIGARX_EP4                ((uint32_t)0x00000004U)         /* !< Endpoint 4 */
#define USB_USBDMASEL_TRIGARX_EP5                ((uint32_t)0x00000005U)         /* !< Endpoint 5 */
#define USB_USBDMASEL_TRIGARX_EP6                ((uint32_t)0x00000006U)         /* !< Endpoint 6 */
#define USB_USBDMASEL_TRIGARX_EP7                ((uint32_t)0x00000007U)         /* !< Endpoint 7 */
/* USB_USBDMASEL[TRIGATX] Bits */
#define USB_USBDMASEL_TRIGATX_OFS                (4)                             /* !< TRIGATX Offset */
#define USB_USBDMASEL_TRIGATX_MASK               ((uint32_t)0x00000070U)         /* !< TRIG A TX select specifies the TX
                                                                                    mapping of the USB endpoint on DMA
                                                                                    trigger USB-A-TX */
#define USB_USBDMASEL_TRIGATX_DIS                ((uint32_t)0x00000000U)         /* !< Disabled */
#define USB_USBDMASEL_TRIGATX_EP1                ((uint32_t)0x00000010U)         /* !< Endpoint 1 */
#define USB_USBDMASEL_TRIGATX_EP2                ((uint32_t)0x00000020U)         /* !< Endpoint 2 */
#define USB_USBDMASEL_TRIGATX_EP3                ((uint32_t)0x00000030U)         /* !< Endpoint 3 */
#define USB_USBDMASEL_TRIGATX_EP4                ((uint32_t)0x00000040U)         /* !< Endpoint 4 */
#define USB_USBDMASEL_TRIGATX_EP5                ((uint32_t)0x00000050U)         /* !< Endpoint 5 */
#define USB_USBDMASEL_TRIGATX_EP6                ((uint32_t)0x00000060U)         /* !< Endpoint 6 */
#define USB_USBDMASEL_TRIGATX_EP7                ((uint32_t)0x00000070U)         /* !< Endpoint 7 */
/* USB_USBDMASEL[TRIGBRX] Bits */
#define USB_USBDMASEL_TRIGBRX_OFS                (8)                             /* !< TRIGBRX Offset */
#define USB_USBDMASEL_TRIGBRX_MASK               ((uint32_t)0x00000700U)         /* !< TRIG B RX select specifies the RX
                                                                                    mapping of the USB endpoint on DMA
                                                                                    trigger USB-B-RX */
#define USB_USBDMASEL_TRIGBRX_DIS                ((uint32_t)0x00000000U)         /* !< Disabled */
#define USB_USBDMASEL_TRIGBRX_EP1                ((uint32_t)0x00000100U)         /* !< Endpoint 1 */
#define USB_USBDMASEL_TRIGBRX_EP2                ((uint32_t)0x00000200U)         /* !< Endpoint 2 */
#define USB_USBDMASEL_TRIGBRX_EP3                ((uint32_t)0x00000300U)         /* !< Endpoint 3 */
#define USB_USBDMASEL_TRIGBRX_EP4                ((uint32_t)0x00000400U)         /* !< Endpoint 4 */
#define USB_USBDMASEL_TRIGBRX_EP5                ((uint32_t)0x00000500U)         /* !< Endpoint 5 */
#define USB_USBDMASEL_TRIGBRX_EP6                ((uint32_t)0x00000600U)         /* !< Endpoint 6 */
#define USB_USBDMASEL_TRIGBRX_EP7                ((uint32_t)0x00000700U)         /* !< Endpoint 7 */
/* USB_USBDMASEL[TRIGBTX] Bits */
#define USB_USBDMASEL_TRIGBTX_OFS                (12)                            /* !< TRIGBTX Offset */
#define USB_USBDMASEL_TRIGBTX_MASK               ((uint32_t)0x00007000U)         /* !< TRIG B TX select specifies the TX
                                                                                    mapping of the USB endpoint on DMA
                                                                                    trigger USB-B-TX */
#define USB_USBDMASEL_TRIGBTX_DIS                ((uint32_t)0x00000000U)         /* !< Disabled */
#define USB_USBDMASEL_TRIGBTX_EP1                ((uint32_t)0x00001000U)         /* !< Endpoint 1 */
#define USB_USBDMASEL_TRIGBTX_EP2                ((uint32_t)0x00002000U)         /* !< Endpoint 2 */
#define USB_USBDMASEL_TRIGBTX_EP3                ((uint32_t)0x00003000U)         /* !< Endpoint 3 */
#define USB_USBDMASEL_TRIGBTX_EP4                ((uint32_t)0x00004000U)         /* !< Endpoint 4 */
#define USB_USBDMASEL_TRIGBTX_EP5                ((uint32_t)0x00005000U)         /* !< Endpoint 5 */
#define USB_USBDMASEL_TRIGBTX_EP6                ((uint32_t)0x00006000U)         /* !< Endpoint 6 */
#define USB_USBDMASEL_TRIGBTX_EP7                ((uint32_t)0x00007000U)         /* !< Endpoint 7 */
/* USB_USBDMASEL[TRIGCRX] Bits */
#define USB_USBDMASEL_TRIGCRX_OFS                (16)                            /* !< TRIGCRX Offset */
#define USB_USBDMASEL_TRIGCRX_MASK               ((uint32_t)0x00070000U)         /* !< TRIG C RX select specifies the RX
                                                                                    mapping of the USB endpoint on DMA
                                                                                    trigger USB-C-RX */
#define USB_USBDMASEL_TRIGCRX_DIS                ((uint32_t)0x00000000U)         /* !< Disabled */
#define USB_USBDMASEL_TRIGCRX_EP1                ((uint32_t)0x00010000U)         /* !< Endpoint 1 */
#define USB_USBDMASEL_TRIGCRX_EP2                ((uint32_t)0x00020000U)         /* !< Endpoint 2 */
#define USB_USBDMASEL_TRIGCRX_EP3                ((uint32_t)0x00030000U)         /* !< Endpoint 3 */
#define USB_USBDMASEL_TRIGCRX_EP4                ((uint32_t)0x00040000U)         /* !< Endpoint 4 */
#define USB_USBDMASEL_TRIGCRX_EP5                ((uint32_t)0x00050000U)         /* !< Endpoint 5 */
#define USB_USBDMASEL_TRIGCRX_EP6                ((uint32_t)0x00060000U)         /* !< Endpoint 6 */
#define USB_USBDMASEL_TRIGCRX_EP7                ((uint32_t)0x00070000U)         /* !< Endpoint 7 */
/* USB_USBDMASEL[TRIGCTX] Bits */
#define USB_USBDMASEL_TRIGCTX_OFS                (20)                            /* !< TRIGCTX Offset */
#define USB_USBDMASEL_TRIGCTX_MASK               ((uint32_t)0x00700000U)         /* !< TRIG C TX select specifies the TX
                                                                                    mapping of the USB endpoint on DMA
                                                                                    trigger USB-C-TX */
#define USB_USBDMASEL_TRIGCTX_DIS                ((uint32_t)0x00000000U)         /* !< Disabled */
#define USB_USBDMASEL_TRIGCTX_EP1                ((uint32_t)0x00100000U)         /* !< Endpoint 1 */
#define USB_USBDMASEL_TRIGCTX_EP2                ((uint32_t)0x00200000U)         /* !< Endpoint 2 */
#define USB_USBDMASEL_TRIGCTX_EP3                ((uint32_t)0x00300000U)         /* !< Endpoint 3 */
#define USB_USBDMASEL_TRIGCTX_EP4                ((uint32_t)0x00400000U)         /* !< Endpoint 4 */
#define USB_USBDMASEL_TRIGCTX_EP5                ((uint32_t)0x00500000U)         /* !< Endpoint 5 */
#define USB_USBDMASEL_TRIGCTX_EP6                ((uint32_t)0x00600000U)         /* !< Endpoint 6 */
#define USB_USBDMASEL_TRIGCTX_EP7                ((uint32_t)0x00700000U)         /* !< Endpoint 7 */
/* USB_USBDMASEL[TRIGDRX] Bits */
#define USB_USBDMASEL_TRIGDRX_OFS                (24)                            /* !< TRIGDRX Offset */
#define USB_USBDMASEL_TRIGDRX_MASK               ((uint32_t)0x07000000U)         /* !< TRIG D RX select specifies the RX
                                                                                    mapping of the USB endpoint on DMA
                                                                                    trigger USB-D-RX */
#define USB_USBDMASEL_TRIGDRX_DIS                ((uint32_t)0x00000000U)         /* !< Disabled */
#define USB_USBDMASEL_TRIGDRX_EP1                ((uint32_t)0x01000000U)         /* !< Endpoint 1 */
#define USB_USBDMASEL_TRIGDRX_EP2                ((uint32_t)0x02000000U)         /* !< Endpoint 2 */
#define USB_USBDMASEL_TRIGDRX_EP3                ((uint32_t)0x03000000U)         /* !< Endpoint 3 */
#define USB_USBDMASEL_TRIGDRX_EP4                ((uint32_t)0x04000000U)         /* !< Endpoint 4 */
#define USB_USBDMASEL_TRIGDRX_EP5                ((uint32_t)0x05000000U)         /* !< Endpoint 5 */
#define USB_USBDMASEL_TRIGDRX_EP6                ((uint32_t)0x06000000U)         /* !< Endpoint 6 */
#define USB_USBDMASEL_TRIGDRX_EP7                ((uint32_t)0x07000000U)         /* !< Endpoint 7 */
/* USB_USBDMASEL[TRIGDTX] Bits */
#define USB_USBDMASEL_TRIGDTX_OFS                (28)                            /* !< TRIGDTX Offset */
#define USB_USBDMASEL_TRIGDTX_MASK               ((uint32_t)0x70000000U)         /* !< TRIG D TX select specifies the TX
                                                                                    mapping of the USB endpoint on DMA
                                                                                    trigger USB-D-TX */
#define USB_USBDMASEL_TRIGDTX_DIS                ((uint32_t)0x00000000U)         /* !< Disabled */
#define USB_USBDMASEL_TRIGDTX_EP1                ((uint32_t)0x10000000U)         /* !< Endpoint 1 */
#define USB_USBDMASEL_TRIGDTX_EP2                ((uint32_t)0x20000000U)         /* !< Endpoint 2 */
#define USB_USBDMASEL_TRIGDTX_EP3                ((uint32_t)0x30000000U)         /* !< Endpoint 3 */
#define USB_USBDMASEL_TRIGDTX_EP4                ((uint32_t)0x40000000U)         /* !< Endpoint 4 */
#define USB_USBDMASEL_TRIGDTX_EP5                ((uint32_t)0x50000000U)         /* !< Endpoint 5 */
#define USB_USBDMASEL_TRIGDTX_EP6                ((uint32_t)0x60000000U)         /* !< Endpoint 6 */
#define USB_USBDMASEL_TRIGDTX_EP7                ((uint32_t)0x70000000U)         /* !< Endpoint 7 */


#ifdef __cplusplus
}
#endif

#endif /* ti_devices_msp_peripherals_hw_usb__include */
