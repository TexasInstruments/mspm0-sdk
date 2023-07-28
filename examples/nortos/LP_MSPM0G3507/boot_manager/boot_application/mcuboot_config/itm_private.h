/******************************************************************************

 @file  itm_private.h

 @brief Provides data logging

 Group: WCS, LPC
 $Target Device: DEVICES $

 ******************************************************************************
 $License: TISD 2011 $
 ******************************************************************************
 $Release Name: PACKAGE NAME $
 $Release Date: PACKAGE RELEASE DATE $
 *****************************************************************************/

#ifndef ITM_PRIVATE_H_
#define ITM_PRIVATE_H_

// Define for major and minor version
#define ITM_LIB_MAJOR_VER 0
#define ITM_LIB_MINOR_VER 5

#define SCS_DEMCR (*(volatile uint32_t *) 0xE000EDFC)
#define SCS_DEMCR_TRCEN (0x01000000)

#define CS_LAR_UNLOCK (0xC5ACCE55)

#define ITM_STIM_PORT(x) (0xE0000000 + 4 * x)

#define ITM_TER (*(volatile uint32_t *) 0xE0000E00)
#define ITM_TER_ENABLE_ALL (0xFFFFFFFF)

#define ITM_TCR (*(volatile uint32_t *) 0xE0000E80)
#define ITM_TCR_ENABLE_ITM (0x00000001)
#define ITM_TCR_ENABLE_TS (0x00000002)
#define ITM_TCR_ENABLE_SYNC (0x00000004)
#define ITM_TCR_ENABLE_DWT_TX (0x00000008)
#define ITM_TCR_ENABLE_SWO (0x00000010)
#define ITM_TCR_BUSY (0x00800000)
#define ITM_TCR_TS_PRESCALE_SHIFT (8)
#define ITM_TCR_TS_PRESCALE_MASK (0x00000300)

#define ITM_TPR (*(volatile uint32_t *) 0xE0000E40)
#define ITM_TPR_ENABLE_USER_ALL (0x0000000F)

#define ITM_LAR (*(volatile uint32_t *) 0xE0000FB0)

#define TPIU_ACPR (*(volatile uint32_t *) 0xE0040010)
#define TPIU_ACPR_MASK (0x00001FFF)

#define TPIU_SPPR (*(volatile uint32_t *) 0xE00400F0)
#define TPIU_SPPR_SWO_UART (0x00000002)

#define TPIU_FFCR (*(volatile uint32_t *) 0xE0040304)
#define TPIU_FFCR_DISABLE (0x00000100)

#define TPIU_LAR (*(volatile uint32_t *) 0xE0040FB0)

#define TPIU_CSPSR (*(volatile uint32_t *) 0xE0040004)
#define TPIU_CSPSR_PIN_1 (0x00000001)

#define DWT_CTRL (*(volatile uint32_t *) 0xE0001000)
#define DWT_CTRL_MASK_NUM_COMP (0xF0000000)
#define DWT_CTRL_SHIFT_NUM_COMP (28)
#define DWT_CTRL_ENABLE_PC_SAMP (0x00001000)
#define DWT_CTRL_ENABLE_EXC_TRC (0x00010000)
#define DWT_CTRL_ENABLE_CYC_EVT (0x00400000)
#define DWT_CTRL_ENABLE_CYC_CNT (0x00000001)
#define DWT_CTRL_CYC_CNT_1024 (0x0000001E)
#define DWT_CTRL_MASK_SYNCTAP (0x00000C00)
#define DWT_CTRL_SHIFT_SYNCTAP (10)

#define DWT_LAR (*(volatile uint32_t *) 0xE0001FB0)

#define DWT_COMP(x) (*(volatile uint32_t *) (0xE0001020 + 16 * (x)))
#define DWT_MASK(x) (*(volatile uint32_t *) (0xE0001024 + 16 * (x)))
#define DWT_FUNC(x) (*(volatile uint32_t *) (0xE0001028 + 16 * (x)))
#define DWT_FUNC_DATA_SIZE_32 (0x00000800)
#define DWT_FUNC_ENABLE_DATA_MATCH (0x00000100)
#define DWT_FUNC_ENABLE_ADDR_OFFSET (0x00000020)
#define DWT_FUNC_ENABLE_COMP_RW (0x00000002)

#endif /* ITM_PRIVATE_H_ */
