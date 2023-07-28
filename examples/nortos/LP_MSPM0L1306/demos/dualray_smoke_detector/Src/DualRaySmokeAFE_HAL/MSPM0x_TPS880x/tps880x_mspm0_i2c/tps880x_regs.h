/* --COPYRIGHT--,BSD
 * Copyright (c) 2019, Texas Instruments Incorporated
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
 * --/COPYRIGHT--*/
//*****************************************************************************
//         Definitions for driver using TPS880x
//
// Driver using TPS880x to obtain Smoke data
// *****************************************************************************
#ifndef __DUALRAYSMOKESWLIB_HAL_FR235X_TPS880x_H__
#define __DUALRAYSMOKESWLIB_HAL_FR235X_TPS880x_H__

#include <Src/include/DualRaySmokeAFE_Config_Common.h>

// Register Definition for TPS880x
#define TPS880X_REG_REVID (0x00)
#define TPS880X_REG_STATUS1 (0x01)
#define TPS880X_REG_STATUS2 (0x02)
#define TPS880X_REG_MASK (0x03)
#define TPS880X_REG_CONFIG1 (0x04)
#define TPS880X_REG_CONFIG2 (0x05)
#define TPS880X_REG_ENABLE1 (0x06)
#define TPS880X_REG_ENABLE2 (0x07)
#define TPS880X_REG_CONTROL (0x08)
#define TPS880X_REG_SLPTMR1 (0x09)
#define TPS880X_REG_SLPTMR2 (0x0A)
#define TPS880X_REG_GPIO_AMUX (0x0B)
#define TPS880X_REG_CO_BATTEST (0x0C)
#define TPS880X_REG_CO (0x0D)
#define TPS880X_REG_VBOOST (0x0E)
#define TPS880X_REG_LEDLDO (0x0F)
#define TPS880X_REG_PH_CTRL (0x10)
#define TPS880X_REG_LED_DAC_A (0x11)
#define TPS880X_REG_LED_DAC_B (0x12)

// Generic bit field definitions
#ifndef BIT0
#define BIT0 (0x01)
#endif
#ifndef BIT1
#define BIT1 (0x02)
#endif
#ifndef BIT2
#define BIT2 (0x04)
#endif
#ifndef BIT3
#define BIT3 (0x08)
#endif
#ifndef BIT4
#define BIT4 (0x10)
#endif
#ifndef BIT5
#define BIT5 (0x20)
#endif
#ifndef BIT6
#define BIT6 (0x40)
#endif
#ifndef BIT7
#define BIT7 (0x80)
#endif

// STATUS1 Field definitions
#define SLP_DONE BIT7
#define VCCLOW BIT6
#define MCULDO_ERR BIT5
#define OTS_ERR BIT4
#define OTS_WRN BIT3
#define BST_NACT BIT2
#define BST_ERR BIT1
#define INT_UNIT BIT0

// STATUS2 Field definitions
#define SLP_DONEM BIT7
#define VCCLOWM BIT6
#define MCULDO_ERRM BIT5
#define OTS_ERRM BIT4
#define OTS_WRNM BIT3
#define BST_NACTM BIT2
#define BST_ERRM BIT1
#define STATUS_INT BIT0

// CONFIG1 Field definitions
#define INT_DEG_1 BIT7
#define INT_DEG_0 BIT6
#define INT_DEG__NONE (0)
#define INT_DEG__125US (INT_DEG_0)
#define INT_DEG__1MS (INT_DEG_1)
#define INT_DEG__20MS (INT_DEG_1 | INT_DEG_0)
#define INT_PD BIT5
#define VMCUSET_1 BIT4
#define VMCUSET_0 BIT3
#define VMCUSET__1_5V (0)
#define VMCUSET__1_8V (VMCUSET_0)
#define VMCUSET__2_5V (VMCUSET_1)
#define VMCUSET__3_3V (VMCUSET_1 | VMCUSET_0)
#define SLP_BST BIT2
#define SLP_ANALOG BIT1
#define SLP_MCU BIT0

// CONFIG2 Field definitions
#define INT_HYS BIT5
#define HORN_SEL BIT4
#define HORN_THR_1 BIT3
#define HORN_THR_0 BIT2
#define HORN_THR__n6 (0)
#define HORN_THR__n3 (HORN_THR_0)
#define HORN_THR__NOM (HORN_THR_1)
#define HORN_THR__p3 (HORN_THR_1 | HORN_THR_0)
#define T_BSTACT_1 BIT1
#define T_BSTACT_0 BIT0
#define T_BSTACT__100uS (0)
#define T_BSTACT__1MS (T_BSTACT_0)
#define T_BSTACT__10MS (T_BSTACT_1)
#define T_BSTACT__100MS (T_BSTACT_1 | T_BSTACT_0)

// ENABLE1 Field definitions
#define BATTEST_EN BIT6
#define INT_EN BIT5
#define BST_EN BIT4
#define PAMP_EN BIT3
#define PGAIN_EN BIT2
#define LEDLDO_EN BIT0

// ENABLE2 Field definitions
#define LEDSEL BIT7
#define BST_CHARGE BIT6
#define INT_DIR BIT3
#define LEDPIN_EN BIT2
#define HORN_EN BIT1
#define SLP_EN BIT0

// CONTROL Field definitions
#define MCU_DIS BIT5
#define VCCLOW_DIS BIT4
#define MCUERR_DIS BIT3
#define OTS_EN BIT2
#define SOFTRESET BIT1
#define VCCLOW_BST BIT0

// SLPTMR1 Field definitions
#define SLPTMR15 BIT7
#define SLPTMR14 BIT6
#define SLPTMR13 BIT5
#define SLPTMR12 BIT4
#define SLPTMR11 BIT3
#define SLPTMR10 BIT2
#define SLPTMR9 BIT1
#define SLPTMR8 BIT0

// SLPTMR2 Field definitions
#define SLPTMR7 BIT7
#define SLPTMR6 BIT6
#define SLPTMR5 BIT5
#define SLPTMR4 BIT4
#define SLPTMR3 BIT3
#define SLPTMR2 BIT2
#define SLPTMR1 BIT1
#define SLPTMR0 BIT0

// GPIO_AMUX Field definitions
#define AMUX_BYP BIT7
#define AMUX_SEL_1 BIT5
#define AMUX_SEL_0 BIT4
#define AMUX_SEL__OFF (0)
#define AMUX_SEL__COO (AMUX_SEL_0)
#define AMUX_SEL__AOUT_PH (AMUX_SEL_1)
#define AMUX_SEL__PDO (AMUX_SEL_1 | AMUX_SEL_0)
#define GPIO_2 BIT2
#define GPIO_1 BIT1
#define GPIO_0 BIT0
#define GPIO__HIZ (0)
#define GPIO__STATUS (GPIO_1)
#define GPIO__LEDENA (GPIO_2)
#define GPIO__LEDENB (GPIO_2 | GPIO_0)
#define GPIO__BATTESTEN (GPIO_2 | GPIO_1 | GPIO_0)

// CO_BATTEST Field definitions
#define COSWRO BIT7
#define COSWRG BIT6
#define COSWRI BIT5
#define COSWREF BIT4
#define I_BATTEST_2 BIT2
#define I_BATTEST_1 BIT1
#define I_BATTEST_0 BIT0
#define I_BATTEST__10MA (0)
#define I_BATTEST__12MA (I_BATTEST_0)
#define I_BATTEST__14MA (I_BATTEST_1)
#define I_BATTEST__16MA (I_BATTEST_1 | I_BATTEST_0)
#define I_BATTEST__18MA (I_BATTEST_2)
#define I_BATTEST__20MA (I_BATTEST_2 | I_BATTEST_0)

// CO Field definitions
#define REF0P3_EN BIT7
#define COREF_1 BIT6
#define COREF_0 BIT5
#define COREF___1_2MV (0)
#define COREF___2_5MV (COREF_0)
#define COREF___3_75MV (COREF_1)
#define COREF___5MV (COREF_1 | COREF_0)
#define COGAIN_1 BIT4
#define COGAIN_0 BIT3
#define COGAIN___1100K (0)
#define COGAIN___300K (COGAIN_0)
#define COGAIN___500K (COGAIN_1)
#define COGAIN___800K (COGAIN_1 | COGAIN_0)
#define COTEST_DIR BIT2
#define COTEST_EN BIT1
#define COAMP_EN BIT0

// VBOOST Field definitions
#define BST_CLIM_3 BIT7
#define BST_CLIM_2 BIT6
#define BST_CLIM_1 BIT5
#define BST_CLIM_0 BIT4
#define BST_CLIM__30MA (0)
#define BST_CLIM__40MA (BST_CLIM_0)
#define BST_CLIM__50MA (BST_CLIM_1)
#define BST_CLIM__60MA (BST_CLIM_1 | BST_CLIM_0)
#define BST_CLIM__80MA (BST_CLIM_2)
#define BST_CLIM__100MA (BST_CLIM_2 | BST_CLIM_0)
#define BST_CLIM__130MA (BST_CLIM_2 | BST_CLIM_1)
#define BST_CLIM__160MA (BST_CLIM_2 | BST_CLIM_1 | BST_CLIM_0)
#define BST_CLIM__200MA (BST_CLIM_3)
#define BST_CLIM__240MA (BST_CLIM_3 | BST_CLIM_0)
#define BST_CLIM__280MA (BST_CLIM_3 | BST_CLIM_1)
#define BST_CLIM__320MA (BST_CLIM_3 | BST_CLIM_1 | BST_CLIM_0)
#define BST_CLIM__360MA (BST_CLIM_3 | BST_CLIM_2)
#define BST_CLIM__400MA (BST_CLIM_3 | BST_CLIM_2 | BST_CLIM_0)
#define BST_CLIM__450MA (BST_CLIM_3 | BST_CLIM_2 | BST_CLIM_1)
#define BST_CLIM__500MA (BST_CLIM_3 | BST_CLIM_2 | BST_CLIM_1 | BST_CLIM_0)
#define VBST_3 BIT3
#define VBST_2 BIT2
#define VBST_1 BIT1
#define VBST_0 BIT0
#define VBST__2_7V (0)
#define VBST__3_8V (VBST_0)
#define VBST__4_7V (VBST_1)
#define VBST__6V (VBST_1 | VBST_0)
#define VBST__9V (VBST_2)
#define VBST__10V (VBST_2 | VBST_0)
#define VBST__10_5V (VBST_2 | VBST_1)
#define VBST__11V (VBST_2 | VBST_1 | VBST_0)
#define VBST__11_5V (VBST_3)
#define VBST__15V (VBST_3 | VBST_0)

// LEDLDO Field definitions
#define LEDLDO_2 BIT3
#define LEDLDO_1 BIT2
#define LEDLDO_0 BIT1
#define LEDLDO__7_5V (0)
#define LEDLDO__8V (LEDLDO_0)
#define LEDLDO__8_5V (LEDLDO_1)
#define LEDLDO__9V (LEDLDO_1 | LEDLDO_0)
#define LEDLDO__9_5V (LEDLDO_2)
#define LEDLDO__10V (LEDLDO_2 | LEDLDO_0)

// PH_CTRL Field definitions
#define TEMPCOB_1 BIT6
#define TEMPCOB_0 BIT5
#define TEMPCOB__347UV_C (0)
#define TEMPCOB__416UV_C (TEMPCOB_0)
#define TEMPCOB__693UV_C (TEMPCOB_1)
#define TEMPCOB__1040UV_C (TEMPCOB_1 | TEMPCOB_0)
#define TEMPCOA_1 BIT4
#define TEMPCOA_0 BIT3
#define TEMPCOA__347MV_C (0)
#define TEMPCOA__416MV_C (TEMPCOA_0)
#define TEMPCOA__693MV_C (TEMPCOA_1)
#define TEMPCOA__1040MV_C (TEMPCOA_1 | TEMPCOA_0)
#define PREF_SEL BIT2
#define PGAIN_1 BIT1
#define PGAIN_0 BIT0
#define PGAIN__5 (0)
#define PGAIN__11 (PGAIN_0)
#define PGAIN__20 (PGAIN_1)
#define PGAIN__35 (PGAIN_1 | PGAIN_0)

// LED_DAC_A Field definitions
#define PDAC_A_7 BIT7
#define PDAC_A_6 BIT6
#define PDAC_A_5 BIT5
#define PDAC_A_4 BIT4
#define PDAC_A_3 BIT3
#define PDAC_A_2 BIT2
#define PDAC_A_1 BIT1
#define PDAC_A_0 BIT0

// LED_DAC_B Field definitions
#define PDAC_B_7 BIT7
#define PDAC_B_6 BIT6
#define PDAC_B_5 BIT5
#define PDAC_B_4 BIT4
#define PDAC_B_3 BIT3
#define PDAC_B_2 BIT2
#define PDAC_B_1 BIT1
#define PDAC_B_0 BIT0

#endif  //__DUALRAYSMOKESWLIB_HAL_FR235X_TPS880x_H__
