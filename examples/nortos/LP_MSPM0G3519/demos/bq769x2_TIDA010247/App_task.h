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

#ifndef APP_TASK_H_
#define APP_TASK_H_

#include "Communications/I2C_communication.h"
#include "ti_msp_dl_config.h"

/******************************************************************
 * Variables:
 */
//Working mode
extern volatile uint8_t Working_mode;
extern volatile uint32_t Systick_counter;
extern volatile bool gWAKEINMCU;
extern volatile bool gCANIDSet;
extern volatile bool gTIMER0;
extern volatile bool gTOP_Volt_Updated;
extern volatile bool gBOT_Volt_Updated;
extern volatile bool Temp_polling;
extern volatile uint8_t FET_TEST;
extern volatile uint8_t TOPFULLSCANCycle;
extern volatile uint8_t BOTFULLSCANCycle;
//extern volatile uint16_t TopCellVoltage[16];
extern volatile uint16_t BOTBatteryStatus;
extern volatile uint16_t BOTProtectionFlag;
extern volatile uint16_t TOPBatteryStatus;
extern volatile uint16_t TOPProtectionFlag;
extern volatile uint16_t gADCResult;
/******************************************************************
 * Define:
 */
#define Ship_mode 0
#define Shutdown_mode 1
#define Standby_mode 2
#define Normal_mode 3

#define AFE_Default_I2C_ADDR 0x08;
#define BOTAFE_I2C_ADDR 0x10;
#define TOPAFE_I2C_ADDR 0x08;

#define DEBOUNCE_TIME 1
#define TMP_MUX_Enabled \
    0  //Configure 9 GPIOs to thermistor input to enable temp mux feature, Note: need modify hardware at same time.

#define BOT_AFEID 0x00
#define TOP_AFEID 0x01

#define Send_BOT_Voltage 0x00
#define Send_TOP_Voltage 0x01

#define FILTER_N 4

#define ADC_16BITS \
    65536  // set the number of bits based on you ADC (2^# of ADC Bit Value)
#define ADC_12BITS 4096
#define VBias_1V8 1.80  // set the VBIAS voltage
#define VBias_3V3 3.30
#define Rbias 18000

#define AFEOTP 0
#define AFE_MOS_TEST 0
/******************************************************************
 * Function:
 */
void Variables_Init(void);
void Gpio_Init(void);
void BatteryDataUpdate_32s(void);
void Temp_Mux_Polling(void);
void TempDataUpdate_TOP(void);
void TempDataUpdate_BOT(void);
void TempUpdate(void);
void BMU_Normal_Mode(void);
void BMU_Standby_Mode(void);
void BMU_Shutdown_Mode(void);
void BMU_Ship_Mode(void);
void BMU_Latch_Mode(void);
void CAN_ID_Init_on_Startup(void);
bool check_signal_pattern();
void BMU_FET_Test(void);
float IIR_Filtering(int16_t input);
int Moving_Averag(int16_t input);

float Lookup_THRM_L(float raw_Voltage);  //LUT for BQ769x2 + TMP61
float TMP61_GetTemp(
    int raw_Voltage, int ADC_BITS, float Vbias, float *THRM_Coefficients);
float TMP61_Offset_Calibration(
    int raw_Voltage, int ADC_BITS, float Vbias, float *THRM_Coefficients);
float TMP61_Averaging_and_Filtering(
    int raw_Voltage, int ADC_BITS, float Vbias, float *THRM_Coefficients);

#endif /* APP_TASK_H_ */
