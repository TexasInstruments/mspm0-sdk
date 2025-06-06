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

#ifndef B0_REG_H_
#define B0_REG_H_

#include <stdint.h>

/**
 * @brief BQ78706 register definition
 *
 * @note Verified based on Datasheet SLUSFQ3, Oct 2024
 *      Please check datasheet before using.
 */
#define CUST_CRC_HI 0x000
#define CUST_CRC_LO 0x001
#define DEV_CONF1 0x002
typedef union {
    struct {
        uint8_t hb_en : 1;
        uint8_t ftone_en : 1;
        uint8_t nfault_en : 1;
        uint8_t no_adj_cb : 1;
        uint8_t cbtwarn_dis : 1;
        uint8_t twarn_thr : 1;
        uint8_t prot_slp_cycle : 2;
    } fs;
    uint8_t reg;
} dev_conf1_t;
#define DEV_CONF2 0x003
typedef union {
    struct {
        uint8_t num_cell : 4;
        uint8_t bb_pin_loc : 2;
        uint8_t bb_pin_en : 1;
        uint8_t ptc_en : 1;
    } fs;
    uint8_t reg;
} dev_conf2_t;
#define COMM_CONF 0x004
#define BBVC_POSN2 0x006
typedef union {
    struct {
        uint8_t cell9 : 1;
        uint8_t cell10 : 1;
        uint8_t cell11 : 1;
        uint8_t cell12 : 1;
        uint8_t cell13 : 1;
        uint8_t cell14 : 1;
        uint8_t rsvd : 2;
    } fs;
    uint8_t reg;
} bbvc_posn2_t;
#define BBVC_POSN3 0x007
typedef union {
    struct {
        uint8_t cell1 : 1;
        uint8_t cell2 : 1;
        uint8_t cell3 : 1;
        uint8_t cell4 : 1;
        uint8_t cell5 : 1;
        uint8_t cell6 : 1;
        uint8_t cell7 : 1;
        uint8_t cell8 : 1;
    } fs;
    uint8_t reg;
} bbvc_posn3_t;
#define ADC_CONF 0x008
typedef union {
    struct {
        uint8_t adc_dly : 8;

    } fs;
    uint8_t reg;
} adc_conf_t;
#define OV_THRESH 0x009
#define UV_THRESH 0x00A
#define OTUT_THRESH 0x00B
#define UV_DISABLE1 0x00C
#define UV_DISABLE2 0x00D
typedef union {
    struct {
        uint8_t cell1 : 1;
        uint8_t cell2 : 1;
        uint8_t cell3 : 1;
        uint8_t cell4 : 1;
        uint8_t cell5 : 1;
        uint8_t cell6 : 1;
        uint8_t cell7 : 1;
        uint8_t cell8 : 1;
    } fs;
    uint8_t reg;
} uv_disable2_t;
#define UV_DISABLE3 0x00E
#define FAULT_MSK1 0x00F
typedef union {
    struct {
        uint8_t msk_pwr : 1;
        uint8_t msk_sys : 1;
        uint8_t msk_cb : 1;
        uint8_t msk_ov : 1;
        uint8_t msk_uv : 1;
        uint8_t msk_ot : 1;
        uint8_t msk_ut : 1;
        uint8_t rsvd : 1;
    } fs;
    uint8_t reg;
} fault_msk1_t;
#define FAULT_MSK2 0x010
typedef union {
    struct {
        uint8_t msk_comm_uart : 1;
        uint8_t msk_comm_dsy : 1;
        uint8_t msk_comm_hb : 1;
        uint8_t msk_comm_ftone : 1;
        uint8_t msk_comm_fcomm : 1;
        uint8_t msk_otp_data : 1;
        uint8_t msk_otp_crc : 1;
        uint8_t msk_adc : 1;
    } fs;
    uint8_t reg;
} fault_msk2_t;
#define CUST_MISC1 0x015
#define CUST_MISC2 0x016
#define CUST_MISC3 0x017
#define CUST_MISC4 0x018
#define CUST_MISC5 0x019
#define CUST_MISC6 0x01A
#define CUST_MISC7 0x01B
#define CUST_MISC8 0x01C
#define IDDQ_CONF 0x01D
// GPIO CONF definition
#define HIGHZ 0b000
#define ADC_RATIO 0b001
#define ADC_ABS 0b010
#define DIGIT_INPUT 0b011
#define OUTPUT_H 0b100
#define OUTPUT_L 0b101
#define ADC_ABS_PU 0b110
#define ADC_ABS_PD 0b111
#define GPIO_CONF1 0x01E
typedef union {
    struct {
        uint8_t gpio1 : 3;
        uint8_t gpio2 : 3;
        uint8_t spi_en : 1;
        uint8_t rsvd : 1;
    } fs;
    uint8_t reg;
} gpio_conf1_t;
#define GPIO_CONF2 0x01F
typedef union {
    struct {
        uint8_t gpio3 : 3;
        uint8_t gpio4 : 3;
        uint8_t rsvd : 2;
    } fs;
    uint8_t reg;
} gpio_conf2_t;
#define GPIO_CONF3 0x020
typedef union {
    struct {
        uint8_t gpio5 : 3;
        uint8_t gpio6 : 3;
        uint8_t rsvd : 2;
    } fs;
    uint8_t reg;
} gpio_conf3_t;
#define GPIO_CONF4 0x021
typedef union {
    struct {
        uint8_t gpio7 : 3;
        uint8_t gpio8 : 3;
        uint8_t rsvd : 2;
    } fs;
    uint8_t reg;
} gpio_conf4_t;
#define GPIO_CONF5 0x022
typedef union {
    struct {
        uint8_t gpio9 : 3;
        uint8_t gpio10 : 3;
        uint8_t rsvd : 2;
    } fs;
    uint8_t reg;
} gpio_conf5_t;
#define GPIO_CONF6 0x023
typedef union {
    struct {
        uint8_t gpio11 : 3;
        uint8_t rsvd : 5;
    } fs;
    uint8_t reg;
} gpio_conf6_t;
#define VCELL_OFFSET1 0x024
#define VCELL_OFFSET2 0x025
#define VCELL_OFFSET3 0x026
#define VCELL_OFFSET4 0x027
#define VCELL_OFFSET5 0x028
#define VCELL_OFFSET6 0x029
#define VCELL_OFFSET7 0x02A
typedef union {
    struct {
        uint8_t offset_lo : 4;
        uint8_t offset_hi : 4;
    } fs;
    uint8_t reg;
} vcell_offsetx_t;
#define DEV_CONF3 0x02D
#define DEV_CONF5 0x02E
typedef union {
    struct {
        uint8_t bb_pin_loc2 : 3;
        uint8_t bb_pin_en2 : 1;
        uint8_t rsvd : 4;
    } fs;
    uint8_t reg;
} dev_conf5_t;
// CUSTOMER CONTROL REGISTERS
#define OTP_PROG_UNLOCK1 0x300
typedef union {
    struct {
        uint8_t code : 8;
    } fs;
    uint8_t reg;
} otp_prog_unlock1_t;
#define DIR0_ADDR 0x306
#define DIR1_ADDR 0x307
#define COMM_CTRL 0x308
#define CONTROL1 0x309
typedef union {
    struct {
        uint8_t addr_wr : 1;
        uint8_t soft_reset : 1;
        uint8_t goto_sleep : 1;
        uint8_t goto_shutdown : 1;
        uint8_t send_slptoact : 1;
        uint8_t send_wake : 1;
        uint8_t send_sd_hw_rst : 1;
        uint8_t dir_sel : 1;
    } fs;
    uint8_t reg;
} control1_t;
#define CONTROL2 0x30A
typedef union {
    struct {
        uint8_t tsref_en : 1;
        uint8_t rsvd : 1;
        uint8_t i2c_en : 1;
        uint8_t prog_go : 1;
        uint8_t rsvd2 : 4;
    } fs;
    uint8_t reg;
} control2_t;
#define ADC_CTRL1 0x310
typedef union {
    struct {
        uint8_t gp_dr : 1;
        uint8_t rsvd : 8;
    } fs;
    uint8_t reg;
} adc_ctrl1_t;
#define ADC_CTRL2 0x311
typedef union {
    struct {
        uint8_t adc_go : 1;
        uint8_t adc_mode : 2;
        uint8_t lpf_vcell_en : 1;
        uint8_t lpf_vcell : 3;
        uint8_t freeze_en : 1;
    } fs;
    uint8_t reg;
} adc_ctrl2_t;
#define ADC_CTRL3 0x312
typedef union {
    struct {
        uint8_t diag_ana_sel : 5;
        uint8_t diag_vcell_gpio : 1;
        uint8_t rsvd : 1;
        uint8_t freeze_lpf_en : 1;
    } fs;
    uint8_t reg;
} adc_ctrl3_t;
#define ADC_CTRL4 0x313
typedef union {
    struct {
        uint8_t diag_meas_go : 1;
        uint8_t diag_d1d2_sel : 3;
        uint8_t rsvd : 4;
    } fs;
    uint8_t reg;
} adc_ctrl4_t;
#define DIAG_ADC_CTRL1 0x314
typedef union {
    struct {
        uint8_t vcell_thr : 6;
        uint8_t rsvd : 1;
        uint8_t vcell_filt : 1;
    } fs;
    uint8_t reg;
} diag_adc_ctrl1_t;
#define DIAG_ADC_CTRL2 0x315
typedef union {
    struct {
        uint8_t gpio_thr : 3;
        uint8_t rsvd : 1;
        uint8_t bb_thr : 2;
        uint8_t rsvd2 : 2;
    } fs;
    uint8_t reg;
} diag_adc_ctrl2_t;
#define DIAG_ADC_CTRL3 0x316
typedef union {
    struct {
        uint8_t diag_ana_go : 1;
        uint8_t diag_ana_mode : 2;
        uint8_t bb_meas_mode : 1;
        uint8_t diag_dig_en : 1;
        uint8_t rsvd : 3;
    } fs;
    uint8_t reg;
} diag_adc_ctrl3_t;
#define DIAG_MISC_CTRL1 0x317
typedef union {
    struct {
        uint8_t margin_go : 1;
        uint8_t margin_mode : 3;
        uint8_t flip_fact_crc : 1;
        uint8_t flip_tr_crc : 1;
        uint8_t spi_loopback : 1;
        uint8_t rsvd : 1;
    } fs;
    uint8_t reg;
} diag_misc_ctrl1_t;
#define DIAG_MISC_CTRL2 0x318
typedef union {
    struct {
        uint8_t pwrbist_go : 1;
        uint8_t pwrbist_no_rst : 1;
        uint8_t diag_cbfetow_go : 1;
        uint8_t rsvd : 5;
    } fs;
    uint8_t reg;
} diag_misc_ctrl2_t;
#define CB_CELL14_CTRL 0x324
#define CB_CELL13_CTRL 0x325
#define CB_CELL12_CTRL 0x326
#define CB_CELL11_CTRL 0x327
#define CB_CELL10_CTRL 0x328
#define CB_CELL9_CTRL 0x329
#define CB_CELL8_CTRL 0x32A
#define CB_CELL7_CTRL 0x32B
#define CB_CELL6_CTRL 0x32C
#define CB_CELL5_CTRL 0x32D
#define CB_CELL4_CTRL 0x32E
#define CB_CELL3_CTRL 0x32F
#define CB_CELL2_CTRL 0x330
#define CB_CELL1_CTRL 0x331
typedef union {
    struct {
        uint8_t time : 5;
        uint8_t rsvd : 3;
    } fs;
    uint8_t reg;
} cb_cellx_ctrl_t;
#define VCBDONE_THRESH 0x332
typedef union {
    struct {
        uint8_t cb_thr : 7;
        uint8_t rsvd : 1;
    } fs;
    uint8_t reg;
} vcbdone_thresh_t;
#define OTCB_THRESH 0x333
typedef union {
    struct {
        uint8_t otcb_thr : 4;
        uint8_t rsvd : 4;
    } fs;
    uint8_t reg;
} otcb_thresh_t;
#define OVUV_CTRL1 0x334
#define OVUV_CTRL2 0x335
typedef union {
    struct {
        uint8_t ovuv_go : 1;
        uint8_t ovuv_mode : 2;
        uint8_t rsvd : 5;
    } fs;
    uint8_t reg;
} ovuv_ctrl2_t;
#define OTUT_CTRL1 0x336
#define OTUT_CTRL2 0x337
typedef union {
    struct {
        uint8_t otut_go : 1;
        uint8_t otut_mode : 2;
        uint8_t rsvd : 5;
    } fs;
    uint8_t reg;
} otut_ctrl2_t;
#define BAL_CTRL1 0x338
typedef union {
    struct {
        uint8_t pwm : 6;
        uint8_t rsvd : 2;
    } fs;
    uint8_t reg;
} bal_ctrl1_t;
#define BAL_CTRL2 0x339
typedef union {
    struct {
        uint8_t bal_go : 1;
        uint8_t auto_bal : 1;
        uint8_t bal_act : 1;
        uint8_t otcb_en : 1;
        uint8_t fltstop_en : 1;
        uint8_t cb_pause : 1;
        uint8_t rsvd : 2;
    } fs;
    uint8_t reg;
} bal_ctrl2_t;
#define BAL_CTRL3 0x33A
typedef union {
    struct {
        uint8_t bal_time_go : 1;
        uint8_t bal_time_sel : 5;
        uint8_t rsvd : 2;
    } fs;
    uint8_t reg;
} bal_ctrl3_t;
#define FAULT_RST1 0x340
typedef union {
    struct {
        uint8_t rst_pwr : 1;
        uint8_t rst_sys : 1;
        uint8_t rst_cb : 1;
        uint8_t rst_ov : 1;
        uint8_t rst_uv : 1;
        uint8_t rst_ot : 1;
        uint8_t rst_ut : 1;
        uint8_t rsvd : 1;
    } fs;
    uint8_t reg;
} fault_rst1_t;
#define FAULT_RST2 0x341
typedef union {
    struct {
        uint8_t rst_comm_uart : 1;
        uint8_t rst_comm_dsy : 1;
        uint8_t rst_comm_hb : 1;
        uint8_t rst_comm_ftone : 1;
        uint8_t rst_comm_fcomm : 1;
        uint8_t rst_otp_data : 1;
        uint8_t rst_otp_crc : 1;
        uint8_t rst_adc : 1;
    } fs;
    uint8_t reg;
} fault_rst2_t;
#define OTP_ECC_DATAIN1 0x343
#define OTP_ECC_DATAIN2 0x344
#define OTP_ECC_DATAIN3 0x345
#define OTP_ECC_DATAIN4 0x346
#define OTP_ECC_DATAIN5 0x347
#define OTP_ECC_DATAIN6 0x348
#define OTP_ECC_DATAIN7 0x349
#define OTP_ECC_DATAIN8 0x34A
#define OTP_ECC_DATAIN9 0x34B
#define OTP_ECC_TEST 0x350
#define I2C_WR_DATA 0x370
#define I2C_CTRL 0x371
#define SPI_CONF 0x380
#define SPI_TX3 0x381
#define SPI_TX2 0x382
#define SPI_TX1 0x383
#define SPI_EXE 0x384
#define MSPI_CONF 0x380
#define MSPI_TX3 0x381
#define MSPI_TX2 0x382
#define MSPI_TX1 0x383
#define MSPI_EXE 0x384
#define OTP_PROG_UNLOCK2 0x4FF
typedef union {
    struct {
        uint8_t code : 8;
    } fs;
    uint8_t reg;
} otp_prog_unlock2_t;
// CUSTOMER STATUS REGISTERS
#define PARTID 0x500
#define TAPEOUT_REV 0x501
#define DIE_ID1 0x502
#define DIE_ID2 0x503
#define DIE_ID3 0x504
#define DIE_ID4 0x505
#define DIE_ID5 0x506
#define DIE_ID6 0x507
#define DIE_ID7 0x508
#define DIE_ID8 0x509
#define CUST_CRC_RSLT_HI 0x50C
#define CUST_CRC_RSLT_LO 0x50D
#define OTP_STAT 0x510
typedef union {
    struct {
        uint8_t done : 1;
        uint8_t progerr : 1;
        uint8_t suv_soverr : 1;
        uint8_t uv_overr : 1;
        uint8_t unlock : 1;
        uint8_t _try : 1;
        uint8_t uv_ovok : 1;
        uint8_t loader : 1;
    } fs;
    uint8_t reg;
} otp_stat_t;
#define GPIO_STAT1 0x520
typedef union {
    struct {
        uint8_t GPIO9 : 1;
        uint8_t GPIO10 : 1;
        uint8_t GPIO11 : 1;
        uint8_t rsvd : 5;
    } fs;
    uint8_t reg;
} gpio_stat1_t;
#define GPIO_STAT2 0x521
#define BAL_STAT 0x522
typedef union {
    struct {
        uint8_t cb_done : 1;
        uint8_t abortflt : 1;
        uint8_t cb_run : 1;
        uint8_t cb_inpause : 1;
        uint8_t ot_pause_det : 1;
        uint8_t invalid_cbconf : 1;
        uint8_t rsvd : 2;
    } fs;
    uint8_t reg;
} bal_stat_t;
#define BAL_SW_STAT2 0x526
#define BAL_SW_STAT3 0x527
#define BAL_DONE2 0x52A
#define BAL_DONE3 0x52B
#define BAL_TIME 0x52C
typedef union {
    struct {
        uint8_t time : 7;
        uint8_t time_unit : 1;
    } fs;
    uint8_t reg;
} bal_time_t;
#define DIAG_STAT1 0x52D
typedef union {
    struct {
        uint8_t drdy_pwrbist : 1;
        uint8_t drdy_ovuv : 1;
        uint8_t drdy_otut : 1;
        uint8_t ecc_test_ok : 1;
        uint8_t freeze_active : 1;
        uint8_t freeze_lpf_active : 1;
        uint8_t rsvd : 2;
    } fs;
    uint8_t reg;
} diag_stat1_t;
#define DIAG_STAT2 0x52E
typedef union {
    struct {
        uint8_t drdy_ana_vcell : 1;
        uint8_t drdy_ana_gpio : 1;
        uint8_t drdy_dig : 1;
        uint8_t drdy_cbfetow : 1;
        uint8_t rsvd : 4;
    } fs;
    uint8_t reg;
} diag_stat2_t;
#define ADC_DATA_RDY 0x52F
typedef union {
    struct {
        uint8_t drdy_vcelladc : 1;
        uint8_t drdy_gpadc : 1;
        uint8_t drdy_diag : 1;
        uint8_t drdy_diag_d1d2 : 1;
        uint8_t rsvd : 4;
    } fs;
    uint8_t reg;
} adc_data_rdy_t;
#define DEV_STAT1 0x530
typedef union {
    struct {
        uint8_t vcelladc_run : 1;
        uint8_t gpadc_run : 1;
        uint8_t diag_ana_run : 1;
        uint8_t ovuv_run : 1;
        uint8_t otut_run : 1;
        uint8_t cust_crc_done : 1;
        uint8_t fact_crc_done : 1;
        uint8_t diag_meas_run : 1;
    } fs;
    uint8_t reg;
} dev_stat1_t;
#define DEV_STAT2 0x531
typedef union {
    struct {
        uint8_t avdd_on : 1;
        uint8_t diag_dig_run : 1;
        uint8_t rsvd : 6;
    } fs;
    uint8_t reg;
} dev_stat2_t;
#define FAULT_SUMMARY 0x532
typedef union {
    struct {
        uint8_t fault_pwr : 1;
        uint8_t fault_comm : 1;
        uint8_t fault_otp : 1;
        uint8_t fault_sys : 1;
        uint8_t fault_ovuv : 1;
        uint8_t fault_otut : 1;
        uint8_t fault_adc_cb : 1;
        uint8_t rsvd : 1;
    } fs;
    uint8_t reg;
} fault_summary_t;
#define FAULT_PWR1 0x535
typedef union {
    struct {
        uint8_t avdd_ov : 1;
        uint8_t avdd_osc : 1;
        uint8_t dvdd_ov : 1;
        uint8_t tsref_ov : 1;
        uint8_t tsref_uv : 1;
        uint8_t tsref_osc : 1;
        uint8_t vss_open : 1;
        uint8_t pwrbist_fail : 1;
    } fs;
    uint8_t reg;
} fault_pwr1_t;
#define FAULT_PWR2 0x536
typedef union {
    struct {
        uint8_t neg_cpump : 1;
        uint8_t rsvd : 7;
    } fs;
    uint8_t reg;
} fault_pwr2_t;
#define FAULT_COMM 0x538
typedef union {
    struct {
        uint8_t stop_det : 1;
        uint8_t commclr_det : 1;
        uint8_t uart_frame : 1;
        uint8_t comh : 1;
        uint8_t coml : 1;
        uint8_t hb_fail : 1;
        uint8_t ftone_det : 1;
        uint8_t fcomm_det : 1;
    } fs;
    uint8_t reg;
} fault_comm_t;
#define FAULT_OTP 0x53B
typedef union {
    struct {
        uint8_t gbloverr : 1;
        uint8_t loaderr : 1;
        uint8_t fact_crc : 1;
        uint8_t cust_crc : 1;
        uint8_t sec_det : 1;
        uint8_t ded_det : 1;
        uint8_t revd : 2;
    } fs;
    uint8_t reg;
} fault_otp_t;
#define FAULT_SYS 0x53d
typedef union {
    struct {
        uint8_t twarn : 1;
        uint8_t tshut : 1;
        uint8_t drst : 1;
        uint8_t lfo : 1;
        uint8_t i2c_nack : 1;
        uint8_t i2c_low : 1;
        uint8_t avdd_on : 2;
        uint8_t revd : 1;
    } fs;
    uint8_t reg;
} fault_sys_t;
#define FAULT_OV2 0x541
#define FAULT_OV3 0x542
#define FAULT_UV2 0x544
#define FAULT_UV3 0x545
#define FAULT_OT1 0x547
#define FAULT_OT2 0x548
#define FAULT_UT1 0x54A
#define FAULT_UT2 0x54B
#define FAULT_ADC_GPIO1 0x54D
typedef union {
    struct {
        uint8_t gpio9_afail : 1;
        uint8_t gpio10_afail : 1;
        uint8_t gpio11_afail : 1;
        uint8_t revd : 5;
    } fs;
    uint8_t reg;
} fault_adc_gpio1_t;
#define FAULT_ADC_GPIO2 0x54E
typedef union {
    struct {
        uint8_t gpio1_afail : 1;
        uint8_t gpio2_afail : 1;
        uint8_t gpio3_afail : 1;
        uint8_t gpio4_afail : 1;
        uint8_t gpio5_afail : 1;
        uint8_t gpio6_afail : 1;
        uint8_t gpio7_afail : 1;
        uint8_t gpio8_afail : 1;
    } fs;
    uint8_t reg;
} fault_adc_gpio2_t;
#define FAULT_ADC_VCELL1 0x54F
typedef union {
    struct {
        uint8_t cell17_afail : 1;
        uint8_t cell18_afail : 1;
        uint8_t revd : 6;
    } fs;
    uint8_t reg;
} fault_adc_vcell1_t;
#define FAULT_ADC_VCELL2 0x550
typedef union {
    struct {
        uint8_t cell9_afail : 1;
        uint8_t cell10_afail : 1;
        uint8_t cell11_afail : 1;
        uint8_t cell12_afail : 1;
        uint8_t cell13_afail : 1;
        uint8_t cell14_afail : 1;
        uint8_t cell15_afail : 1;
        uint8_t cell16_afail : 1;
    } fs;
    uint8_t reg;
} fault_adc_vcell2_t;
#define FAULT_ADC_VCELL3 0x551
typedef union {
    struct {
        uint8_t cell1_afail : 1;
        uint8_t cell2_afail : 1;
        uint8_t cell3_afail : 1;
        uint8_t cell4_afail : 1;
        uint8_t cell5_afail : 1;
        uint8_t cell6_afail : 1;
        uint8_t cell7_afail : 1;
        uint8_t cell8_afail : 1;
    } fs;
    uint8_t reg;
} fault_adc_vcell3_t;
#define FAULT_ADC_DIG2 0x553
typedef union {
    struct {
        uint8_t cell9_dfail : 1;
        uint8_t cell10_dfail : 1;
        uint8_t cell11_dfail : 1;
        uint8_t cell12_dfail : 1;
        uint8_t cell13_dfail : 1;
        uint8_t cell14_dfail : 1;
        uint8_t rsvd : 2;
    } fs;
    uint8_t reg;
} fault_adc_dig2_t;
#define FAULT_ADC_DIG3 0x554
typedef union {
    struct {
        uint8_t cell1_dfail : 1;
        uint8_t cell2_dfail : 1;
        uint8_t cell3_dfail : 1;
        uint8_t cell4_dfail : 1;
        uint8_t cell5_dfail : 1;
        uint8_t cell6_dfail : 1;
        uint8_t cell7_dfail : 1;
        uint8_t cell8_dfail : 1;
    } fs;
    uint8_t reg;
} fault_adc_dig3_t;
#define FAULT_ADC_MISC 0x555
typedef union {
    struct {
        uint8_t diag_ana_abort : 1;
        uint8_t diag_ana_pfail : 1;
        uint8_t diag_meas_pfail : 1;
        uint8_t adc_pfail : 1;
        uint8_t revd : 4;
    } fs;
    uint8_t reg;
} fault_adc_misc_t;
#define FAULT_CB_FETOW1 0x557
typedef union {
    struct {
        uint8_t cb17_fail : 1;
        uint8_t cb18_fail : 1;
        uint8_t revd : 6;
    } fs;
    uint8_t reg;
} fault_cb_fetow1_t;
#define FAULT_CB_FETOW2 0x558
typedef union {
    struct {
        uint8_t cb9_fail : 1;
        uint8_t cb10_fail : 1;
        uint8_t cb11_fail : 1;
        uint8_t cb12_fail : 1;
        uint8_t cb13_fail : 1;
        uint8_t cb14_fail : 1;
        uint8_t cb15_fail : 1;
        uint8_t cb16_fail : 1;
    } fs;
    uint8_t reg;
} fault_cb_fetow2_t;
#define FAULT_CB_FETOW3 0x559
typedef union {
    struct {
        uint8_t cb1_fail : 1;
        uint8_t cb2_fail : 1;
        uint8_t cb3_fail : 1;
        uint8_t cb4_fail : 1;
        uint8_t cb5_fail : 1;
        uint8_t cb6_fail : 1;
        uint8_t cb7_fail : 1;
        uint8_t cb8_fail : 1;
    } fs;
    uint8_t reg;
} fault_cb_fetow3_t;
#define VCELL18_HI 0x574
#define VCELL18_LO 0x575
#define VCELL17_HI 0x576
#define VCELL17_LO 0x577
#define VCELL16_HI 0x578
#define VCELL16_LO 0x579
#define VCELL15_HI 0x57A
#define VCELL15_LO 0x57B
#define VCELL14_HI 0x57C
#define VCELL14_LO 0x57D
#define VCELL13_HI 0x57E
#define VCELL13_LO 0x57F
#define VCELL12_HI 0x580
#define VCELL12_LO 0x581
#define VCELL11_HI 0x582
#define VCELL11_LO 0x583
#define VCELL10_HI 0x584
#define VCELL10_LO 0x585
#define VCELL9_HI 0x586
#define VCELL9_LO 0x587
#define VCELL8_HI 0x588
#define VCELL8_LO 0x589
#define VCELL7_HI 0x58A
#define VCELL7_LO 0x58B
#define VCELL6_HI 0x58C
#define VCELL6_LO 0x58D
#define VCELL5_HI 0x58E
#define VCELL5_LO 0x58F
#define VCELL4_HI 0x590
#define VCELL4_LO 0x591
#define VCELL3_HI 0x592
#define VCELL3_LO 0x593
#define VCELL2_HI 0x594
#define VCELL2_LO 0x595
#define VCELL1_HI 0x596
#define VCELL1_LO 0x597
#define VCELL_ACT_SUM_HI 0x598
#define VCELL_ACT_SUM_LO 0x599
#define BAT_HI 0x59A
#define BAT_LO 0x59B
#define GPIO1_HI 0x5A8
#define GPIO1_LO 0x5A9
#define GPIO2_HI 0x5AA
#define GPIO2_LO 0x5AB
#define GPIO3_HI 0x5AC
#define GPIO3_LO 0x5AD
#define GPIO4_HI 0x5AE
#define GPIO4_LO 0x5AF
#define GPIO5_HI 0x5B0
#define GPIO5_LO 0x5B1
#define GPIO6_HI 0x5B2
#define GPIO6_LO 0x5B3
#define GPIO7_HI 0x5B4
#define GPIO7_LO 0x5B5
#define GPIO8_HI 0x5B6
#define GPIO8_LO 0x5B7
#define GPIO9_HI 0x5B8
#define GPIO9_LO 0x5B9
#define GPIO10_HI 0x5BA
#define GPIO10_LO 0x5BB
#define GPIO11_HI 0x5BC
#define GPIO11_LO 0x5BD
#define DIAG_MAIN_HI 0x5EA
#define DIAG_MAIN_MID 0x5EB
#define DIAG_MAIN_LO 0x5EC
#define DIAG_RDNT_HI 0x5ED
#define DIAG_RDNT_MID 0x5EE
#define DIAG_RDNT_LO 0x5EF
#define DIETEMP1_HI 0x5F0
#define DIETEMP1_LO 0x5F1
#define DIETEMP2_HI 0x5F2
#define DIETEMP2_LO 0x5F3
#define REF_CAP_HI 0x5F4
#define REF_CAP_LO 0x5F5
#define DIAG_D1_HI 0x5F6
#define DIAG_D1_LO 0x5F7
#define DIAG_D2_HI 0x5F8
#define DIAG_D2_LO 0x5F9
#define REF_CAP_T0_HI 0x5FA
#define REF_CAP_T0_LO 0x5FB
#define I2C_RD_DATA 0x610
#define SPI_RX3 0x620
#define SPI_RX2 0x621
#define SPI_RX1 0x622
#define MSPI_RX3 0x620
#define MSPI_RX2 0x621
#define MSPI_RX1 0x622
// CUSTOMER DEBUG CONTROL REGISTERS
#define DEBUG_CTRL_UNLOCK 0x700
#define DEBUG_COMM_CTRL1 0x701
#define DEBUG_COMM_CTRL2 0x702
// CUSTOMER DEBUG STATUS REGISTERS
#define DEBUG_COMM_STAT 0x780
#define DEBUG_UART_RC_TR 0x781
#define DEBUG_COMH_BIT 0x782
#define DEBUG_COMH_RC_TR 0x783
#define DEBUG_COMH_RR 0x784
#define DEBUG_COML_BIT 0x785
#define DEBUG_COML_RC_TR 0x786
#define DEBUG_COML_RR 0x787
#define DEBUG_SEC_DED_BLK 0x788

#endif /* B0_REG_H_ */
