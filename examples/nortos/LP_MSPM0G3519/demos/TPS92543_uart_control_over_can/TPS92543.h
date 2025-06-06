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

#ifndef TPS92543_H_
#define TPS92543_H_

#include "ti_msp_dl_config.h"

/* INIT byte */
#define WRITE1 (0x87)
#define WRITE2 (0x99)
#define WRITE3 (0x1E)
#define WRITE4 (0xAA)
#define WRITE5 (0xAD)
#define WRITE8 (0x13)
#define WRITE12 (0x2D)
#define WRITE16 (0x33)
#define WRITE20 (0xB5)
#define WRITE32 (0xB4)
#define WRITE64 (0xA4)
#define READ1 (0x4B)
#define READ2 (0xCC)
#define READ3 (0xD2)
#define READ4 (0x55)
#define READ5 (0xE5)
#define READ8 (0x26)
#define READ12 (0xE1)
#define READ16 (0x66)
#define READ20 (0x7A)
#define READ32 (0x78)
#define READ64 (0x68)

/* Protocol ACK */
#define TPSACK (0x7F)
/*COMMAND Register data*/
#define EnterLoad (0x55)   //	 Leave Limp-Home and enter LOAD Mode
#define EnterLimp (0xAA)   //	 Enter Limp-Home
#define EnterCTM (0xFF)    //	 Enter Customer Test Mode (CTM)
#define EnterSleep (0xCC)  //	 Enter Sleep
#define EnterRun (0x33)    //	 Enter Run Mode
#define LeaveCTM (0x96)    //	 Leave CTM
#define ProgEEPROM (0x77)  //	 Program EEPROM
#define LockEEPROM (0x11)  //	 Lock EEPROM
#define OpenPswrd (0x99)   //	 Open NVM Password to allow password updates
#define ClosePswrd (0x66)  //	 Close NVM Password to prevent password updates

/*DEVADDR Configuration*/
#define P0_INV_DIS (0x00)
#define P0_INV_EN (0x20)
#define P1_INV_DIS (0x00)
#define P1_INV_EN (0x40)

/*UART Configuration*/
#define SEPTR_CAN (0x00)
#define SEPTR_UART (0x80)
#define ACKEN_DIS (0x00)
#define ACKEN_EN (0x20)
#define PSON_DIS (0x00)
#define PSON_EN (0x10)
#define CMWEN_DIS (0x00)
#define CMWEN_EN (0x08)
#define BAUD_0_5M (0x00)
#define BAUD_1_0M (0x01)
#define BAUD_1_33M (0x02)
#define BAUD_1_6M (0x03)
#define BAUD_2_0M (0x04)

/*Multicast Configuration*/
#define MLTCST_VLD_DIS (0x00)
#define MLTCST_VLD_EN (0x80)

/*SYSCFG1 Configuration*/
#define BCK1EN_DIS (0x00)
#define BCK1EN_EN (0x01)
#define BSTEN_DIS (0x00)
#define BSTEN_EN (0x04)

/* Register address */
#define COMMAND (0x00)
#define SYSCFG1 (0x01)
#define SYSCFG2 (0x02)
#define BCKCFG2 (0x04)
#define BCKCFG4 (0x06)
#define BCKCFG6 (0x08)
#define BCKCFG7 (0x09)
#define BSTCFG1 (0x0A)
#define BSTCFG2 (0x0B)
#define BSTCFG3 (0x0C)
#define BSTCFG4 (0x0D)
#define BSTCFG5 (0x0E)
#define BCKREFLS (0x0F)
#define BCKREFMS1 (0x10)
#define BSTREF (0x12)
#define TWLMT (0x13)
#define BSTVINDROP1_1 (0x14)
#define BSTVINDROP1_2 (0x15)
#define MPIOCFG1 (0x18)
#define MPIOCFG2 (0x19)
#define MPIODIGOUT (0x1B)
#define STATUS1 (0x1C)
#define BSTSTATUS (0x1D)
#define BCK1STATUS (0x1E)
#define CERRCNT (0x20)
#define MPIO0MS (0x21)
#define MPIO1MS (0x22)
#define MPIO2MS (0x23)
#define MPIO3MS (0x24)
#define MPIOLS1 (0x25)
#define TEMPH (0x28)
#define TEMPL (0x29)
#define VISPMEAS (0x2A)
#define VISDIFFX5 (0x2B)
#define VDD (0x2C)
#define VBCK1A (0x2D)
#define BCK1VLEDON (0x2F)
#define BCK1VLEDOFF (0x30)
#define BCKPWM (0x33)
#define BCK1PWML (0x34)
#define BCK1PWMH (0x35)
#define WIDTH3_1L (0x38)
#define WIDTH6_4L (0x39)
#define WIDTH1H (0x3A)
#define WIDTH2H (0x3B)
#define WIDTH3H (0x3C)
#define WIDTH4H (0x3D)
#define WIDTH5H (0x3E)
#define WIDTH6H (0x3F)
#define PHASE3_1L (0x40)
#define PHASE6_4L (0x41)
#define PHASE1H (0x42)
#define PHASE2H (0x43)
#define PHASE3H (0x44)
#define PHASE4H (0x45)
#define PHASE5H (0x46)
#define PHASE6H (0x47)
#define FLTOV (0x48)
#define FLTSHORT (0x49)
#define LED1ADC (0x4A)
#define LED2ADC (0x4B)
#define LED3ADC (0x4C)
#define LED4ADC (0x4D)
#define LED5ADC (0x4E)
#define LED6ADC (0x4F)
#define LEDOVTH1 (0x50)
#define LEDOVTH2 (0x51)
#define LEDOVTH3 (0x52)
#define DIGIN (0x53)
#define UARTCFG (0x80)
#define CMWTAP (0x81)
#define PWMTICK (0x82)
#define PAGE (0xE9)
#define NVM_PASSWD (0xEA)
#define PG_VERSION (0xFB)
#define MLTCST1 (0xFC)
#define MLTCST2 (0xFD)
#define DEVADDR (0xFE)
#define ICID (0xFF)

#endif
