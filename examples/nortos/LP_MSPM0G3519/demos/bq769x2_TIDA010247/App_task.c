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

#include "App_task.h"
#include <math.h>
#include "BQ769x2_Configs/BQ769x2_protocol.h"
#include "Communications/CAN_communication.h"
#include "Communications/UART_communication.h"

/******************************************************************
 * Variables:
 */
volatile uint8_t Working_mode = 3;
volatile uint8_t FET_TEST     = 0;

volatile uint32_t Systick_counter = 1;

volatile bool gWAKEINMCU = false;
volatile bool gCANIDSet  = false;
volatile bool gTIMER0    = false;

uint8_t BOTFULLSCANCycle  = 0;
uint8_t BOTFULLSCANCycle2 = 0;
uint8_t TOPFULLSCANCycle  = 0;
uint8_t TOPFULLSCANCycle2 = 0;
uint8_t PASSQ_TIME_MIN    = 0;

uint16_t TOPStack_Voltage = 0x00;
uint16_t TOPPack_Voltage  = 0x00;
uint16_t TOPLD_Voltage    = 0x00;
uint16_t BOTStack_Voltage = 0x00;
uint16_t BOTPack_Voltage  = 0x00;
uint16_t BOTLD_Voltage    = 0x00;
uint16_t gADCResult;
uint16_t BOTBatteryStatus  = 0x00;
uint16_t BOTProtectionFlag = 0x00;
uint16_t TOPBatteryStatus  = 0x00;
uint16_t TOPProtectionFlag = 0x00;

bool send_success;
bool PASSQ_read;

uint16_t TopCellVoltage_raw[16] = {0x01, 0x02, 0x03, 0x04, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
uint16_t BotCellVoltage_raw[16] = {0x02, 0x03, 0x04, 0x05, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

uint32_t Qtime_30min[30] = {0x0000};
/******************************************************************
 * array:
 */
float Temperature_BOT_TS3[200]   = {0x0000};
float Temperature_BOT_DDSG[200]  = {0x0000};
float Temperature_BOT_DCHG[200]  = {0x0000};
float Temperature_BOT_CFET[200]  = {0x0000};
float Temperature_BOT_HDQ[200]   = {0x0000};
float Temperature_BOT_ALERT[200] = {0x0000};

float BOT_CFET_Volt[4]  = {0x0000};
float BOT_Alert_Volt[4] = {0x0000};
float BOT_TS3_Volt[4]   = {0x0000};
float BOT_HDQ_Volt[4]   = {0x0000};
float BOT_DCHG_Volt[4]  = {0x0000};
float BOT_DDSG_Volt[4]  = {0x0000};

float Temperature_TOP_TS3[200]   = {0x0000};
float Temperature_TOP_DDSG[200]  = {0x0000};
float Temperature_TOP_DCHG[200]  = {0x0000};
float Temperature_TOP_CFET[200]  = {0x0000};
float Temperature_TOP_HDQ[200]   = {0x0000};
float Temperature_TOP_ALERT[200] = {0x0000};

float TOP_CFET_Volt[4]  = {0x0000};
float TOP_Alert_Volt[4] = {0x0000};
float TOP_TS3_Volt[4]   = {0x0000};
float TOP_HDQ_Volt[4]   = {0x0000};
float TOP_DCHG_Volt[4]  = {0x0000};
float TOP_DDSG_Volt[4]  = {0x0000};

float PASSQ_30min[30] = {0x0000};

float THRM_Coefficients_10k_3V3[5] = {-4.232811E+02, 4.728797E+02,
    -1.988841E+02, 4.869521E+01, -1.158754E+00};  //3V3, 10K, 12bits ADC
float THRM_Coefficients_18k_1V8[5] = {-4.232811E+02, 4.728797E+02,
    -1.988841E+02, 4.869521E+01, -1.158754E+00};  //1V8, 18k, 16bits ADC
float Cell_fixed_offset_LFP[16]    = {-0.6, -0.8, -0.8, -0.8, -0.5, -0.6, -0.6,
    -0.6, -0.3, -0.4, -0.1, -0.1, -1.3, -1.1, -1.2, -0.7};
float TopCellVoltage_cali[16]      = {0x00};
float BotCellVoltage_cali[16]      = {0x00};
float THRM_Res_L_Table_1V8_18k[166][2] = {0x00};

float TMP61_MCU;
float TMP61_MCU_filtered;
float inttemp;
float BOTCFETTEMP;

/******************************************************************
 * Functions:
 */

void BMU_Normal_Mode(void)
{
    //In Normal Mode, refresh 32s cells voltage, battery voltage, pack voltage, 32 PTCs temperature and pack current every 100ms and transmit it out through RS485/CAN
    DL_GPIO_clearPins(LED_PORT, LED_LED1_Blue_PIN);
    DL_GPIO_clearPins(LED_PORT, LED_LED2_Red_PIN);
    DL_GPIO_setPins(LED_PORT,
        LED_LED3_Green_PIN);  // Set Green LED to indicate Normal Mode

    if (BOTBatteryStatus & 0x8000)  //If BOTAFE is in Sleep mode
    {
        I2C_TARGET_ADDRESS = TOPAFE_I2C_ADDR;
        CommandSubcommands(SLEEP_ENABLE);  //Enable TOPAFE sleep mode
        Working_mode = Standby_mode;
    }

    if (TOPProtectionFlag |
        BOTProtectionFlag)  //If any protection triggered, enter shutdown mode
    {
        Working_mode = Shutdown_mode;
    }

#if Current_Calibration
    BQ769x2_Current_BoardOffset_Calibration();
    BQ769x2_Current_Gain_Calibration();
#else
#if !TMP_MUX_Enabled
    BatteryDataUpdate_32s();
#else
    Temp_Mux_Polling();
#endif
#endif

    if (Systick_counter % 10 == 0)  // run every 1s
    {
        if (send_success) {
            RS485_Send(TOP_AFEID, Send_TOP_Voltage, TopCellVoltage_raw,
                sizeof(TopCellVoltage_raw) / 2);
            CAN_Write(gCANNodeId, TOP_AFEID, Send_TOP_Voltage,
                sizeof(TopCellVoltage_raw) / 2, TopCellVoltage_raw);
            send_success = false;
        }
    } else {
        send_success = true;
    }

    if (Systick_counter % 600 == 0)  // run every 1min
    {
        if (PASSQ_read) {
            I2C_TARGET_ADDRESS = BOTAFE_I2C_ADDR;
            BQ769x2_ReadPassQ();
            PASSQ_read = false;

            PASSQ_30min[PASSQ_TIME_MIN] = PASS_Q;  //report by mAh
            Qtime_30min[PASSQ_TIME_MIN] =
                AccumulatedCharge_Time;  // report by second
            PASSQ_TIME_MIN++;
            //            if (PASSQ_TIME_MIN == 30) PASSQ_TIME_MIN = 0;
            if (PASSQ_TIME_MIN == 30)
                while (1)
                    ;  //Stop at 30min
        }
    } else {
        PASSQ_read = true;
    }
}

void BMU_Standby_Mode(void)
{
    DL_GPIO_clearPins(LED_PORT, LED_LED3_Green_PIN);
    DL_GPIO_clearPins(LED_PORT, LED_LED2_Red_PIN);
    DL_GPIO_setPins(
        LED_PORT, LED_LED1_Blue_PIN);  // Set Blue LED to indicate Standby Mode
    DL_GPIO_clearPins(Power_Enable_PORT, Power_Enable_PIN);  // Disable DC/DC

    if (Systick_counter % 50 == 0)  //run every 5s
    {
        I2C_TARGET_ADDRESS = BOTAFE_I2C_ADDR;
        BOTBatteryStatus   = BQ769x2_ReadBatteryStatus();
        BOTProtectionFlag  = BQ769x2_ReadSafetyStatus();

        I2C_TARGET_ADDRESS = TOPAFE_I2C_ADDR;
        TOPBatteryStatus   = BQ769x2_ReadBatteryStatus();
        TOPProtectionFlag  = BQ769x2_ReadSafetyStatus();
    }

    if ((BOTBatteryStatus & 0x8000) == 0)  //If BOTAFE exit Sleep mode
    {
        if (TOPProtectionFlag == 0 & BOTProtectionFlag == 0) {
            Working_mode       = Normal_mode;
            I2C_TARGET_ADDRESS = TOPAFE_I2C_ADDR;
            CommandSubcommands(SLEEP_DISABLE);  //Disable TOPAFE sleep mode
        }
    }

    if (TOPProtectionFlag |
        BOTProtectionFlag)  //If any protection triggered, enter shutdown mode
    {
        Working_mode = Shutdown_mode;
    }
}

void BMU_Shutdown_Mode(void)
{
    DL_GPIO_clearPins(LED_PORT, LED_LED3_Green_PIN);
    DL_GPIO_setPins(
        LED_PORT, LED_LED2_Red_PIN);  // Set Red LED to indicate Shutdown Mode
    DL_GPIO_clearPins(LED_PORT, LED_LED1_Blue_PIN);

    I2C_TARGET_ADDRESS = BOTAFE_I2C_ADDR;
    BOTBatteryStatus   = BQ769x2_ReadBatteryStatus();
    BOTProtectionFlag  = BQ769x2_ReadSafetyStatus();

    I2C_TARGET_ADDRESS = TOPAFE_I2C_ADDR;
    TOPBatteryStatus   = BQ769x2_ReadBatteryStatus();
    TOPProtectionFlag  = BQ769x2_ReadSafetyStatus();

    if ((BOTBatteryStatus & 0x8000) == 0)  //If BOTAFE exit Sleep mode
    {
        I2C_TARGET_ADDRESS = TOPAFE_I2C_ADDR;
        CommandSubcommands(SLEEP_DISABLE);  //Disable TOPAFE sleep mode

        if (TOPProtectionFlag == 0 & BOTProtectionFlag == 0) {
            Working_mode = Normal_mode;
        }
    }
}

void BMU_Ship_Mode(void)
{
    DL_GPIO_clearPins(LED_PORT, LED_LED3_Green_PIN);
    DL_GPIO_clearPins(Power_Enable_PORT, Power_Enable_PIN);  // Disable DC/DC
    DL_GPIO_clearPins(Isolator_PORT, Isolator_EN_PIN);

    I2C_TARGET_ADDRESS = TOPAFE_I2C_ADDR;
    CommandSubcommands(SHUTDOWN);
    I2C_TARGET_ADDRESS = BOTAFE_I2C_ADDR;
    CommandSubcommands(SHUTDOWN);
}

void BMU_Latch_Mode(void)
{
    while (1)
        ;
}

/* @brief Update cell voltage, battery voltage, pack voltage, pack current
 * FASTADC=1; ADCSCAN period= 31.5ms; FULLSCAN period= 94.5ms
 */
void BatteryDataUpdate_32s(void)
{
    //Update Voltage and Current,take 13.6ms@400khz I2C
    if (gTIMER0) {
        //        DL_GPIO_setPins(Test_Port_PORT, Test_Port_T1_PIN);
        //BOT AFE
        I2C_TARGET_ADDRESS = BOTAFE_I2C_ADDR;
        BQ769x2_ReadCurrent();  //Update Pack Current,read CC3 average current, default 80 average samples
        BQ769x2_ReadAllCellVoltage(BotCellVoltage_raw);
        for (int x = 0; x < 16; x++) {  //Cali all cell voltages
            BotCellVoltage_cali[x] =
                BotCellVoltage_raw[x] - Cell_fixed_offset_LFP[x];
        }

        //TOP AFE
        I2C_TARGET_ADDRESS = TOPAFE_I2C_ADDR;
        BQ769x2_ReadAllCellVoltage(TopCellVoltage_raw);
        for (int x = 0; x < 16; x++) {  //Cali all cell voltages
            TopCellVoltage_cali[x] =
                TopCellVoltage_raw[x] - Cell_fixed_offset_LFP[x];
        }

        gTIMER0 = false;
        //        DL_GPIO_clearPins(Test_Port_PORT, Test_Port_T1_PIN);
    }

#if 0
    if (Systick_counter % 5 == 0) {
        I2C_TARGET_ADDRESS = BOTAFE_I2C_ADDR;
        BOTBatteryStatus   = BQ769x2_ReadBatteryStatus();
        BOTProtectionFlag  = BQ769x2_ReadSafetyStatus();

        I2C_TARGET_ADDRESS = TOPAFE_I2C_ADDR;
        TOPBatteryStatus   = BQ769x2_ReadBatteryStatus();
        TOPProtectionFlag  = BQ769x2_ReadSafetyStatus();
        BQ769x2_ReadMANUFACTURINGStatus();
        BQ769x2_ReadFETStatus();  //0x45=CHG/DSG ON alert pin on

        DL_ADC12_startConversion(ADC12_0_INST);
        gADCResult = DL_ADC12_getMemResult(ADC12_0_INST, DL_ADC12_MEM_IDX_0);
        TMP61_MCU  = TMP61_GetTemp(
            gADCResult, ADC_12BITS, VBias_3V3, THRM_Coefficients_10k_3V3);
        TMP61_MCU_filtered = TMP61_Averaging_and_Filtering(
            gADCResult, ADC_12BITS, VBias_3V3, THRM_Coefficients_10k_3V3);
        DL_ADC12_enableConversions(ADC12_0_INST);
    }
#endif
}

/* @brief Update 18 thermistors value
 * FASTADC=1; ADCSCAN period= 31.5ms; FULLSCAN period= 94.5ms
 * The pins are measured in the sequence of CFETOFF, DFETOFF, ALERT, TS1, TS2, TS3, HDQ, DCHG, DDSG
 * but it only measures the pins that are configured as thermistor inputs.
 * TS1 is used as the counter clock, TS2 is used as wake pin.
 *
 * BOT AFE Thermistors mapping         AFE GPIOs
 *  - CELL0/1/2                         TS3_BOTAFE
 *  - CELL3/4/5                         DDSG_BOTAFE
 *  - CELL6/7/8                         DCHG_BOTAFE
 *  - CELL9/10/11                       CFETOFF_BOTAFE
 *  - CELL12/13/14                      HDQ_BOTAFE
 *  - CELL15/16/AFE Backfaces           ALERT_BOTAFE
 *
 *  TOPAFE Thermistors mapping:         AFE GPIOs
 *  - CELL16B/17/18                     TS3_TOPAFE
 *  - CELL19/20/21                      CFETOFF_TOPAFE
 *  - CELL22/23/24                      ALERT_TOPAFE
 *  - CELL25/26/27                      DCHG_TOPAFE
 *  - CELL28/29/30                      DDSG_TOPAFE
 *  - CELL31/32/BAT+                    HDQ_TOPAFE
 */

void TempDataUpdate_TOP(void)
{
    //Update Temperature
    BQ769x2_ReadAllTemperature_Count();  //24bit data, LSB=0.358uV

    TOP_CFET_Volt[TOPFULLSCANCycle]  = CFETOFF_Count * 0.000358;
    TOP_Alert_Volt[TOPFULLSCANCycle] = ALERT_Count * 0.000358;
    TOP_TS3_Volt[TOPFULLSCANCycle]   = TS3_Count * 0.000358;
    TOP_DCHG_Volt[TOPFULLSCANCycle]  = DCHG_Count * 0.000358;
    TOP_DDSG_Volt[TOPFULLSCANCycle]  = DDSG_Count * 0.000358;
    TOP_HDQ_Volt[TOPFULLSCANCycle]   = HDQ_Count * 0.000358;

    TOPFULLSCANCycle++;
    if (TOPFULLSCANCycle == 4) TOPFULLSCANCycle = 0;
}

void TempDataUpdate_BOT(void)
{
    //Update Temperature
    BQ769x2_ReadAllTemperature_Count();  //24bit data, LSB=0.358uV

    BOT_CFET_Volt[BOTFULLSCANCycle] = CFETOFF_Count * 0.000358;  //report by mV
    //    BOTCFETTEMP=BQ769x2_ReadTemperature(CFETOFFTemperature);
    BOT_Alert_Volt[BOTFULLSCANCycle] = ALERT_Count * 0.000358;
    BOT_TS3_Volt[BOTFULLSCANCycle]   = TS3_Count * 0.000358;
    BOT_HDQ_Volt[BOTFULLSCANCycle]   = HDQ_Count * 0.000358;
    BOT_DCHG_Volt[BOTFULLSCANCycle]  = DCHG_Count * 0.000358;
    BOT_DDSG_Volt[BOTFULLSCANCycle]  = DDSG_Count * 0.000358;

    BOTFULLSCANCycle++;
    if (BOTFULLSCANCycle == 4) BOTFULLSCANCycle = 0;
}

void Temp_Mux_Polling(void)
{
    BQ769x2_ReadFULLSCAN();
    //TOP AFE
    I2C_TARGET_ADDRESS = TOPAFE_I2C_ADDR;
    if (TOP_FULLSCAN) {
        DirectCommands(AlarmStatus, 0x0080, W);
        TempDataUpdate_TOP();
    }
    //BOT AFE
    I2C_TARGET_ADDRESS = BOTAFE_I2C_ADDR;
    if (BOT_FULLSCAN) {
        DirectCommands(AlarmStatus, 0x0080, W);
        TempDataUpdate_BOT();
    }
}

void BMU_FET_Test(void)
{
    I2C_TARGET_ADDRESS = TOPAFE_I2C_ADDR;
    switch (FET_TEST) {
        case 0:
            gWAKEINMCU = true;
            DL_GPIO_clearPins(LED_PORT, LED_LED3_Green_PIN);
            DL_GPIO_clearPins(
                FET_Control_DSG_Driver_PORT, FET_Control_DSG_Driver_PIN);
            CommandSubcommands(ALL_FETS_ON);
            FET_TEST = 1;
            break;

        case 1:
            gWAKEINMCU = false;
            DL_GPIO_setPins(LED_PORT, LED_LED3_Green_PIN);
            DL_GPIO_setPins(FET_Control_DSG_Driver_PORT,
                FET_Control_DSG_Driver_PIN);  //PB4
            CommandSubcommands(DSG_PDSG_OFF);
            FET_TEST = 0;
            break;
        default:
            break;
    }
}

/*@brief 4th order polynomial equations to calculate the temperature of the thermistor
 * For TMP6131DECR only
 */
float TMP61_GetTemp(int raw_Voltage, int ADC_BITS, float Vbias,
    float *
        THRM_Coefficients)  // send the ADC bit value to the calculation function
{
    float VTEMP     = 0;  // set up the variable for the measured voltage
    float THRM_TEMP = 0;  // setup the variable for the calculated temperature
    VTEMP           = 0;
    VTEMP =
        (Vbias / ADC_BITS) *
        raw_Voltage;  // calculate volts per bit then multiply that times the ADV value
    THRM_TEMP =
        (THRM_Coefficients[4] * powf(VTEMP, 4)) +
        (THRM_Coefficients[3] * powf(VTEMP, 3)) +
        (THRM_Coefficients[2] * powf(VTEMP, 2)) +
        (THRM_Coefficients[1] * VTEMP) +
        THRM_Coefficients[0];  // 4th order regression to get temperature
    return THRM_TEMP;
}

/*@brief Thermistor single-point offset calibration
 * For TMP6131DECR only
 */
float TMP61_Offset_Calibration(
    int raw_Voltage, int ADC_BITS, float Vbias, float *THRM_Coefficients)
{
    float VTEMP = 0;      // set up the variable for the measured voltage
    float offset;         // variable to store offset value
    float VTEMPfiltered;  // variable for filtered voltage
    int samples =
        5000;  // recommend at least 5,000 samples to give software filter time to stabilize before calculating offset
    float
        TMP6filtered;  // variable to hold the filtered temperature measurement
    float
        referenceTemp;  // temperature measurement from high-accuracy temperature reference (ex.TMP119)

    for (int i = 0; i < samples; i++) {
        VTEMP = Vbias / ADC_BITS * raw_Voltage;
        VTEMPfiltered =
            VTEMPfiltered -
            (0.001 *
                (VTEMPfiltered -
                    VTEMP));  // continually filter VTEMP over 5,000 samples
    }
    TMP6filtered =
        TMP61_GetTemp(VTEMPfiltered, ADC_BITS, Vbias, THRM_Coefficients);
    referenceTemp = 30;  // type in reference temp here
    return offset =
               TMP6filtered -
               referenceTemp;  // calculate offset as difference between TMP6filtered and referenceTemp
}
float VTEMP     = 0;    // set up the variable for the measured voltage
int oversamples = 16;   // EDIT number of oversamples
float TMP6oversampled;  // variable for oversampled TMP6 temperature reading
float TMP6oversampled_filtered;  // variable for filtered temperature reading
float VTEMP_averaged;
/*@brief Oversampling, averaging and filtering
 * For TMP6131DECR only
 */
float TMP61_Averaging_and_Filtering(
    int raw_Voltage, int ADC_BITS, float Vbias, float *THRM_Coefficients)
{
    for (int i = 0; i < oversamples; i++) {
        //read raw voltage every intp
        VTEMP +=
            raw_Voltage;  // convert to voltage and sum for desired number of oversamples
    }
    VTEMP_averaged = VTEMP / oversamples;  // average the summed VTEMP
    // convert voltage to temperature
    TMP6oversampled =
        TMP61_GetTemp(VTEMP_averaged, ADC_BITS, Vbias, THRM_Coefficients);
    // calculate oversampled_filtered temperature using previous oversampled_filtered temp and oversampled temp
    TMP6oversampled_filtered =
        TMP6oversampled_filtered -
        (0.05 * (TMP6oversampled_filtered - TMP6oversampled));
    VTEMP = 0;  // reset VTEMP to 0 for next sample to be taken
    return TMP6oversampled_filtered;
}

float THRM_RES = 0;

float Lookup_THRM_L(float
        raw_Voltage)  // Lookup table (long) 1 degrees C, no math just find the first value
{
    float THRM_TEMP = 0;
    THRM_RES =
        Rbias * raw_Voltage /
        (VBias_1V8 -
            raw_Voltage);  // THRM Resistance - calculate the resistance of the thermistor

    int i       = 0;  // counter for stepping through the table
    float res_l = 0;  // the lower resistance value in the table
    float res_h = 0;  // the higher resistance value in the table
    float diff_l =
        0;  // the difference between the lower resistance value from the table from the actual resistance value
    float diff_h =
        0;  // the difference between the higher resistance value from the table from the actual resistance value

    for (i = 0; i < 166; i++) {
        if (THRM_Res_L_Table_1V8_18k[i][1] >= THRM_RES) {
            res_l = THRM_Res_L_Table_1V8_18k
                [i - 1][1];  // find the lower resistance value in the table
            res_h = THRM_Res_L_Table_1V8_18k
                [i][1];  // find the higher resistance value in the table
            diff_l =
                THRM_RES -
                res_l;  // find the difference by subtracting the lower resistance value from the table from the actual resistance value
            diff_h =
                THRM_RES -
                res_h;  // find the difference by subtracting the higher resistance value from the table from the actual resistance value

            if (i > 0) {
                if (diff_h <=
                    diff_l)  // if the high resistance is closer to the actual resistance
                {
                    THRM_TEMP = THRM_Res_L_Table_1V8_18k
                        [i]
                        [0];  // Use the high value resistance and temperature
                } else  // else the low resistance is closer to the actual resistance
                {
                    THRM_TEMP = THRM_Res_L_Table_1V8_18k
                        [i - 1]
                        [0];  // Use the low value resistance and temperature
                };
            } else
                THRM_TEMP = THRM_Res_L_Table_1V8_18k[0][0];

            break;
        }
    }
    return THRM_TEMP;
}

/*@brief Check WAKEIN signal pattern
 *high(>10ms)-low(>10ms)-high(>10ms) wake up pattern will initiate addressing function
 */
bool check_signal_pattern()
{
    static int state                = 0;
    static uint32_t last_time       = 0;
    static uint32_t current_time    = 0;
    static uint32_t current_counter = 0;
    static uint32_t last_counter    = 0;

    double elapsed_time = 0;
    int input_level     = DL_GPIO_readPins(WAKE_IN_MCU_PORT, WAKE_IN_MCU_PIN);

    current_time    = SysTick->VAL;     //12.5ns per Tick, max 4000000
    current_counter = Systick_counter;  //50ms per Counter

    if (current_counter == last_counter) {
        elapsed_time = (double) (current_time - last_time) * 12.5 /
                       1000000;  // convert to ms
    } else if (current_counter > last_counter) {
        elapsed_time = (double) (4000000 * (current_counter - last_counter) -
                                 last_time + current_time) *
                       12.5 / 1000000;  // convert to ms
    } else {
        elapsed_time =
            (double) (4000000 * (1200 - last_counter + current_counter) -
                      last_time + current_time) *
            12.5 / 1000000;  // convert to ms
    }

    switch (state) {
        case 0:
            if (input_level == WAKE_IN_MCU_PIN) {
                state        = 1;
                last_time    = current_time;
                last_counter = current_counter;
            }
            break;
        case 1:
            if (elapsed_time >=
                DEBOUNCE_TIME) {  //DEBOUNCE TIME is defined to 1ms
                if (input_level == 0) {
                    state        = 2;
                    last_time    = current_time;
                    last_counter = current_counter;
                } else {
                    state = 0;
                }
            }

            break;
        case 2:
            if (elapsed_time >= DEBOUNCE_TIME) {
                if (input_level == WAKE_IN_MCU_PIN) {
                    state        = 3;
                    last_time    = current_time;
                    last_counter = current_counter;
                } else {
                    state = 0;
                }
            }
            break;
        case 3:
            if (elapsed_time >= DEBOUNCE_TIME) {
                if (input_level == 0) {
                    gWAKEINMCU = true;
                    state      = 0;
                } else {
                    state = 0;
                }
            }
            break;
    }

    return gWAKEINMCU;
}

/*@brief CAN Addressing Process
 *1.Host(BCU) set node #1 wake_in pin high.
 *2.Node#1 receive wake_in IO high Address itself to CANID
 *3.wake_out highï¼Œsend CAN frame contain ID+1 data
 *4.Node#2 receive wake_in IO high Address itself to CANID+1
 */
void CAN_ID_Init_on_Startup(void)
{
    //Enable Isolator
    DL_GPIO_setPins(Isolator_PORT, Isolator_EN_PIN);
    //Disbale WAKE_IN_MCU interrupt
    DL_GPIO_disableInterrupt(GPIOB, WAKE_IN_MCU_PIN);

    //Enable CAN Power
    DL_MCAN_reset(MCAN0_INST);  //reset FIFO
    DL_MCAN_enablePower(MCAN0_INST);
    delay_cycles(1600);  //800 can't, 1600 can!
    //CAN Init
    SYSCFG_DL_MCAN0_init();

    gCANServiceInt     = false;
    gCANIntLine1Status = 0;

    NVIC_EnableIRQ(MCAN0_INST_INT_IRQN);  //Enable CAN interrupt
    while (DL_MCAN_OPERATION_MODE_NORMAL != DL_MCAN_getOpMode(MCAN0_INST))
        ;
    //Waiting for CAN message
    //DL_MCAN_INTERRUPT_RF0N
    while (false == gCANServiceInt) {
        __WFE();
    }
    gCANServiceInt   = false;
    gCANRxFS.fillLvl = 0;

    CANprocessCANRxMsg();  //set ID command is 0x7D

    gCANServiceInt = false;
    //WAKEINMCU=false, send wake out pattern to next node
    gWAKEINMCU = false;
    DL_GPIO_setPins(WAKE_OUT_MCU_PORT, WAKE_OUT_MCU_PIN);
    delayUS(2000);
    DL_GPIO_clearPins(WAKE_OUT_MCU_PORT, WAKE_OUT_MCU_PIN);
    delayUS(2000);
    DL_GPIO_setPins(WAKE_OUT_MCU_PORT, WAKE_OUT_MCU_PIN);
    delayUS(2000);
    DL_GPIO_clearPins(WAKE_OUT_MCU_PORT, WAKE_OUT_MCU_PIN);
    delayUS(20000);

    /* Initialize message to transmit. */
    CANTxMsgSendParamInitDefault();
    gCANTxMsg.id      = ((uint32_t) CAN_ID_DEFAULT) << 18;
    gCANTxMsg.dlc     = CAN_DLC_SETID;
    gCANTxMsg.mm      = CAN_MM_SETID;
    gCANTxMsg.data[0] = CAN_CMD_SETID;
    gCANTxMsg.data[1] = (uint8_t)((gCANNodeId + 1) & 0xFF);
    gCANTxMsg.data[2] = (uint8_t)(((gCANNodeId + 1) >> 8) & 0x07);

    /* Write Tx Message to the Message RAM. */
    DL_MCAN_writeMsgRam(MCAN0_INST, DL_MCAN_MEM_TYPE_BUF, 0U, &gCANTxMsg);
    /* Add request for transmission. */
    DL_MCAN_TXBufAddReq(MCAN0_INST, 0U);

    gCANServiceInt = false;
    //Waiting for CAN Tx done
    //Why can not trigger TC interrupt here?
    //DL_MCAN_INTERRUPT_TEFN
    while (false == gCANServiceInt) {
        __WFE();
    }

    gCANServiceInt = false;
    gCANIDSet      = true;  //
}

float IIR_Filtering(int16_t input)
{
    //used to filter low frequency noise
    int64_t a[3] = {16384, -27705, 12005};
    int64_t b[3] = {342, 684, 342};
    float input_n;
    float input_n_minus_1;
    float input_n_minus_2;
    float Result_filter_n;
    float Result_filter_n_minus_1;
    float Result_filter_n_minus_2;

    input_n         = input;
    Result_filter_n = b[0] * input_n_minus_2 / 32768 +
                      b[1] * input_n_minus_1 / 32768 + b[2] * input_n / 32768 -
                      a[2] * Result_filter_n_minus_2 / 16384 -
                      a[1] * Result_filter_n_minus_1 / 16384;

    Result_filter_n_minus_2 = Result_filter_n_minus_1;
    Result_filter_n_minus_1 = Result_filter_n;
    input_n_minus_2         = input_n_minus_1;
    input_n_minus_1         = input_n;

    return Result_filter_n;
}

void Variables_Init(void)
{
    FET_TEST          = 1;
    gWAKEINMCU        = false;
    gCANIDSet         = false;
    PASSQ_read        = false;
    Systick_counter   = 1;
    TOPFULLSCANCycle  = 0;
    TOPFULLSCANCycle2 = 0;
    BOTFULLSCANCycle  = 0;
    BOTFULLSCANCycle2 = 0;
    PASSQ_TIME_MIN    = 0;
    Working_mode      = Normal_mode;
}

void Gpio_Init(void)
{
    DL_GPIO_setPins(Power_Enable_PORT, Power_Enable_PIN);  // Enable DC/DC
    delayUS(60000);
    delayUS(60000);  //delay 12ms for start-up
    DL_GPIO_setPins(
        Power_UCC_EN2_PORT, Power_UCC_EN2_PIN);  // Enable ISO1640 power
    DL_GPIO_setPins(
        Power_UCC_EN1_PORT, Power_UCC_EN1_PIN);  // Enable Communications power
    DL_GPIO_setPins(Isolator_PORT, Isolator_EN_PIN);
    DL_GPIO_setPins(PTC_MCU_PORT, PTC_MCU_En_PIN);  //Enable TMP61 MSMT
    DL_GPIO_setPins(LED_PORT, LED_LED3_Green_PIN);  //Set Green LED

#if TMP_MUX_Enabled
    DL_GPIO_clearPins(Temp_Inhibit_IN_PORT,
        Temp_Inhibit_IN_BOT_PIN);  // Clear BOT MUX INH pin
    DL_GPIO_clearPins(Temp_Inhibit_IN_PORT,
        Temp_Inhibit_IN_TOP_PIN);  // Clear BOT MUX INH pin
#else
    DL_GPIO_setPins(Temp_Inhibit_IN_PORT,
        Temp_Inhibit_IN_BOT_PIN);  // Clear BOT MUX INH pin
    DL_GPIO_setPins(Temp_Inhibit_IN_PORT,
        Temp_Inhibit_IN_TOP_PIN);  // Clear BOT MUX INH pin
#endif
}
