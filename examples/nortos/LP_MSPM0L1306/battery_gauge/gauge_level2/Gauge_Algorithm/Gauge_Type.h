/*
 * Copyright (c) 2023, Texas Instruments Incorporated
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

#ifndef GAUGE_ALGORITHM_GAUGE_TYPE_H_
#define GAUGE_ALGORITHM_GAUGE_TYPE_H_

#include <stdbool.h>
#include <stdint.h>
#include <ti/iqmath/include/IQmathLib.h>

#define FLASH_DATA_INPUT (0)
#define COMMUNICATION_DATA_INPUT (1)
#define DETECTION_DATA_INPUT (2)
#define NO_OUTPUT (0)
#define UART_OUTPUT (1)

#define CT_Y_THD_NUM (2)  //Temperature threshold numbers
#define CT_X_THD_NUM (2)  //Current threshold numbers
#define CT_LEGTH \
    (9)  //CT table numbers CT_LEGTH=(CT_Y_THD_NUM+1)*(CT_X_THD_NUM+1)
//*****************************************************************************
// Describe OPA test mode
//*****************************************************************************
enum {
    eNormalMode = 0,
    eOffsetCali = 1,
};
typedef uint8_t tOPACali;

//*****************************************************************************
// Describe the status
//*****************************************************************************
enum {
    eFalse = 0,
    eTrue  = 1,
};
typedef uint8_t tStatus;

//*****************************************************************************
// Describe the status of Battery state
//*****************************************************************************
enum {
    eUnload,
    eCHGUnfull,
    eCHGFull,
    eDHG,
};
typedef uint8_t tBattState;

//*****************************************************************************
// Describe the algorithm one running cycle time
//*****************************************************************************
enum {
    eSystemTik_32000MS = 64,
    eSystemTik_16000MS = 32,
    eSystemTik_8000MS  = 16,
    eSystemTik_4000MS  = 8,
    eSystemTik_2000MS  = 4,
    eSystemTik_1000MS  = 2,
    eSystemTik_e500MS  = 1,
};
typedef uint8_t tSystemTikFreq;

//*****************************************************************************
// Describe different Sum Capacity type, used in CurtGauge
//*****************************************************************************
enum {
    eTempQsum,
    eChgQsum,
    eDhgQsum,
    eDeltaQsum,
};
typedef uint8_t tQsumType;

//*****************************************************************************
// Describe different OPA setting
//*****************************************************************************
enum {
    eGAIN16,
    eGAIN32,
};
typedef uint8_t eOpaGain;

//*****************************************************************************
// Warning flags
//*****************************************************************************
typedef struct {
    tStatus VcellHighWarnFlg : 1;
    tStatus VcellLowWarnFlg : 1;
    tStatus IcellHighWarnFlg : 1;
    tStatus IcellLowWarnFlg : 1;
    tStatus TcellChgHighWarnFlg : 1;
    tStatus TcellChgLowWarnFlg : 1;
    tStatus TcellDhgHighWarnFlg : 1;
    tStatus TcellDhgLowWarnFlg : 1;
    tStatus CircuitTableOverRangeFlg : 1;
    tStatus DhgChgEfcyGapFlg : 1;
} tWarningFlags;

//*****************************************************************************
// Elements used in the battery circuit model, which is used to characterize the battery
// These parameters need to input ahead.
// The most care the accurate of discharging parameters, especially iq15DhgNomRcell_R.
// For other parameters we can input by experience.
//*****************************************************************************
typedef struct {
    uint16_t u16ChgOcv_mV;  //Open circuit voltage, unit is mV
    _iq15 iq15Soc_DEC;      //State of charge, unit is decimal
    _iq15
        iq15CapFactor;  //C = dSoc/dOcv(V)*Qmax(As) = dSoc/dOcv(V)*3.6*Qmax(mAh)
                        //capFactor = dSoc/dOcv*3.6
    _iq15 iq15DhgNomRcell_R;
} tBattCircuitParams;

//*****************************************************************************
// Parameters used in VGauge
//*****************************************************************************
typedef struct {
    // TimeConstant factor Ts/(Ts + R*C)
    _iq15 iq15SmoothFactor;
    _iq15 iq15CalculatedOcv[2];
    _iq15 iq15CalRcell_R;
    _iq15 iq15CalCcell_F;  //Calculated battery Capacitor
    _iq15 iq15VFAbsSoc_DEC;
} tVGaugeFilterParams;

//*****************************************************************************
// Parameters used in IGauge
//When i16mA meet +-256, i16mAs will add. When i16mAs meet 3600, i16mAh will add.
//When i16mA meet +-3600, i16mAs will add
//*****************************************************************************
typedef struct {
    int16_t i16Qsum_mA;
    int16_t i16Qsum_mAs;
    int16_t i16Qsum_mAh;
} tQsum;

typedef struct {
    tQsum tempQsum;   //Used to update u16IF_NomRemCap_mAh, will be calibrated
    tQsum deltaQsum;  //Used to update u16IF_NomRemCap_mAh, will be calibrated
    tQsum
        backupDeltaQsum;  //Used to solve the time frequency gap between avg data and unavg data
    tQsum chgQsum;                //Positive
    tQsum dhgQsum;                //Negative
    int16_t i16IF_NomRemCap_mAh;  //Will be update in every OCV calibration,
        //because it can show the performance of OCV calibration method
    uint16_t u16DhgCycles;
    uint16_t u16ChgCycles;
} tIGaugeCoulometerParams;

//*****************************************************************************
// Parameters inputed by customer
//*****************************************************************************
typedef const struct {
    //******************************Generation configuration parameters********************************//
    tBattCircuitParams *pBattCircuitParams;
    uint16_t
        u16DesignCap_mAh;  //Battery default battery Cell Size, unit is  mAh
    uint16_t
        u16MinBattVoltThd_mV;  //Need to ensure the battery run time voltage all larger that it.
    //When voltage bellow than this, the SmoothSOC will turns to 0% at
    // once and the warning flag will be set.
    uint16_t
        u16MaxBattVoltThd_mV;  //Need to ensure the battery run time voltage all lower that it.
    //When voltage above than this,the warning flag will be set.
    uint16_t
        u16MinFullChgVoltThd_mV;  //The minimum battery constant charge voltage, need to consider to voltage drop caused from line resistance.
    //Should be a little lower than real battery constant charge voltage, like 1000mV
    uint16_t
        u16MaxFullChgVoltThd_mV;  //The maximum battery constant charge voltage
    //Should be a little higher than real battery constant charge voltage, like 100mV
    //Should be lower than the max volt in circuit table
    uint16_t
        u16EmptyDhgVoltThd_mV;  // Empty voltage, treat as the CusSoc = 0%, should be higher than u16MinBattVoltThd_mV
    //Should be higher than the lowest volt in circuit table
    int16_t
        i16FullChgCurtThd_mA;  // Battery charge termination current threshold when it is full, Normally it is 1/20~1/15C
    //Used to tell Gauge the battery is full. Need a little higher than the real charge
    //terminate current. Otherwise the judgement of CHG full will have a problem
    int16_t
        i16MaxChgCurtThd_mA;  // Battery fast charge current threshold, Above will disable MOSFET
    int16_t
        i16MinDhgCurtThd_mA;  // Battery fast discharge current threshold, Above will disable MOSFET
    int8_t
        i8MaxChgTempThd_C;  // Battery maximum temperature threshold, Above will disable MOSFET
    int8_t
        i8MinChgTempThd_C;  // Battery minimum temperature threshold, Bellow will disable MOSFET
    int8_t
        i8MaxDhgTempThd_C;  // Battery maximum temperature threshold, Above will disable MOSFET
    int8_t
        i8MinDhgTempThd_C;  // Battery minimum temperature threshold, Bellow will disable MOSFET
    uint8_t
        u8AvgBattParamsUpdateCount;  //Average battery parameters update time:u8AvgBattParamsUpdateCount*sysTikFreq
    uint8_t u8SysTikShift;  //Real sysTik time = sysTikFreq/u8SysTikShift
    tSystemTikFreq
        sysTikFreq;  //Used as a clock to run algorithm, can be changed by customer to reduce power consumption

    //******************************Mix algorithm related parameters************************************//
    int8_t i8TempThd_C
        [CT_Y_THD_NUM];  //Used for AmptySoc and FullSoc calculation, low temperature threshold
    int16_t i16CurtThd_mA
        [CT_X_THD_NUM];  //Used for AmptySoc calculation, low current threshold
    _iq15 iq15AbsEmptySocMatrixInput[CT_LEGTH];

    //***********************Capacity learn algorithm related parameters********************************//
    int16_t
        i16UnloadCurtLowThd_mA;  //Used to judge the battery unload status to do SOC calibration, paired with UnloadCurtHighThd_mA.
    int16_t
        i16UnloadCurtHighThd_mA;  //Used to judge the battery unload status to do SOC calibration, paired with i16UnloadCurtLowThd_mA.
    uint16_t
        u16DeltaAvgVellForCaliThd_mV;  //Used to judge the battery voltage comes to rest in order to do SOC calibration,
    //if the voltage only changes DeltaAvgVellForCaliThd_mV in AvgBattParamsUpdateCount*u8AvgVcellDetectPeriod cycles.
    uint8_t
        u8AvgVcellDetectPeriod;  //Used to judge the battery voltage comes to rest in order to do SOC calibration, paired with u16DeltaAvgVellForCaliThd_mV
    uint8_t u8SOHCalcCycleThd;
    _iq15 iq15DefaultSOH_DEC;

    //****************************VoltGuage algorithm related parameters********************************//
    uint8_t u8CircuitTableLength;  //Battery circuit table length
    uint8_t
        u8CircuitTableTestTemp_C;  //Battery circuit table test temperature, used to evaluate runtime Rcell
    // paired with RcellNegTshift and RcellPosTshift
    _iq15
        iq15RcellNegTshift_R;  //Resolution is 0.000030518R or 0.03mR, changed based on u8CircuitTableTestTemp_C
    _iq15
        iq15RcellPosTshift_R;  //Resolution is 0.000030518R or 0.03mR, changed based on u8CircuitTableTestTemp_C

    //****************************CurrentGuage algorithm related parameters********************************//
    int16_t i16AvgLeckageCurt_mA;  //Leckage current
} tBattParamsConfig;

//*****************************************************************************
// Gauge  detection parameters(V, I, T)
//*****************************************************************************
typedef struct {
    tStatus avgBattParamsUpdateFlg : 1;  //The average data is prepared
    tStatus passiveAvgFlg : 1;
    uint16_t u16Vcell_mV;  //Battery voltage, unit is mV
    int16_t
        i16Icell_mA;  //Battery current, unit is mA, discharge is negative charge is positive
    int8_t i8Tcell_C;      //Battery temperature
    uint32_t u32SumVcell;  //Temp value used to calculate u16AvgVcell_mV
    int32_t i32SumIcell;   //Temp value used to calculate u16AvgIcell_mV
    int16_t i16SumTcell;   //Temp value used to calculate u16AvgTcell_mV
    uint8_t u8SumCount;    //Used to count the sum
    uint16_t u16AvgVcell_mV;
    int16_t i16AvgIcell_mA;
    int8_t i8AvgTcell_C;
} tBattInfoInput;

//*****************************************************************************
// Current calibration factors
//*****************************************************************************
typedef struct {
    uint8_t u8R12toR2FactorCaliValue;
    uint8_t u8R34toR3FactorCaliValue_Gain32;
    uint8_t u8R34toR3FactorCaliValue_Gain16;
    uint8_t u8DefaultValue_Gain32;
    uint8_t u8DefaultValue_Gain16;
} tDacSetValue;

typedef struct {
    tDacSetValue dacSetValue;
    eOpaGain opaGainSet : 2;
    _iq15 iq15R34toR3Factor_Gain32;   //1/GAIN
    _iq5 iq5VadcinOffsetCode_Gain32;  //ADC in code with current equals to 0mA
    _iq5 iq5OpaOffsetVolt_Gain32;     //OPA output offset with Gain 32x
    _iq15 iq15ResFactor_Gain32;
    _iq15 iq15R34toR3Factor_Gain16;
    _iq5 iq5VadcinOffsetCode_Gain16;
    _iq5 iq5OpaOffsetVolt_Gain16;  //OPA output offset with Gain 16x
    _iq15 iq15ResFactor_Gain16;
    _iq15 iq15R12toR2Factor;
    _iq15 iq15R12toR1Factor;
    int16_t i16McuTemp;  //Use to tell MCU when to do run time calibration
} tCurtCalcFactors;

//*****************************************************************************
// Capacity learn factors
//*****************************************************************************
typedef struct {
    tStatus lowDhgOcvCaliFlg : 1;
    tStatus capLearnStartFlg : 1;
    tStatus ocvCaliFlg : 1;
    tStatus socCapAccumulationFlg : 1;
    tStatus iGuageDominationFlg : 1;
    tStatus fullCapCaliFlg : 1;
    uint16_t u16TimeExtendCounter;
    tBattState battStateRecord[3];
    _iq15 iq15CaliSoc_DEC;
    uint16_t u16CaliCycleRecord[2];
    uint16_t u16CaliOcv_mV;
    uint16_t u16AvgVcellRecord[3];
    _iq15 iq15CaliAbsSocRecord[2];
    _iq15 iq15SumAbsDeltaSoc_DEC;
    uint16_t u16RawNomFullCap_mAh;
    uint16_t u16FilteredNomFullCap_mAh;
    uint16_t u16MaxNomFullCap_mAh;  //Used to calculate SOH
    _iq15 iq15SumDeltaCap_mAh;
    _iq15 iq15SOH_DEC;           //Battery state of health
    uint16_t DHGCycleRecord[2];  //Used to calculate coulomb efficiency
    tBattState battState;
} tBattCapLearnParams;

//*****************************************************************************
// Mixing factors
//*****************************************************************************
typedef struct {
    _iq15 iq15CusRltSoc
        [2];  //The absolute Soc according to the setting of CusEmptyOcv and CusFullOcv_mV
    _iq15 iq15SmoothRltSoc_DEC;  //The final Soc value send to customer
    _iq15
        iq15NomAbsSoc_DEC;  //The absolute Soc related to the circuitParamsTable input range
    _iq15 iq15CusAbsFullSocRange_DEC;
    uint16_t i16CusRemCap_mAh;
    uint16_t u16CusFullCap_mAh;
    int16_t i16Mix_NomRemCap_mAh;
    uint16_t u16NomFullCap_mAh;
    _iq15 iq15AbsEmptySocMatrix[2][9];
    _iq15 iq15AbsFullSoc[2];
    _iq15 iq15AbsEmptySoc[2];
    uint8_t u8EmptySocMatrixCount;
    uint16_t u16VoltRecord[2];
    int16_t i16CurtRecord[2];
    _iq15 iq15AbsSocRecord[2];
} tMixingParams;

//*****************************************************************************
// Output Parameters
//*****************************************************************************
typedef struct {
    //Battery input parameters
    uint16_t u16Dr_Vcell_mV;  //Battery voltage, unit is mV
    int16_t i16Dr_Icell_mA;   //Battery current, unit is mA
        //Discharge current is negative. Charge current is positive.
    int8_t i8Dr_Tcell_C;  //Battery temperature
    uint16_t u16Dr_AvgVcell_mV;
    int16_t i16Dr_AvgIcell_mA;
    int8_t i8Dr_AvgTcell_C;

    //VGauge parameters
    _iq15 iq15VF_CurCalcOcv_mV;  //Current calculated Ocv
    _iq15 iq15VFAbsSoc_DEC;

    //IGauge parameters
    int16_t i16IF_NomRemCap_mAh;
    int16_t i16IF_DeltaQsum_mAh;
    uint16_t u16IF_Cycles;

    //Capacity learn parameters
    uint16_t u16CL_CaliOcv_mV;
    uint16_t u16CL_RawNomFullCap_mAh;
    _iq15 iq15CL_SOH_DEC;  //Battery state of health
    _iq15 iq15CL_CaliSoc_DEC;
    _iq15 iq15CL_SumAbsDeltaSoc_DEC;
    tBattState CL_BattState;

    //Mix algorithm
    _iq15 iq15Mix_AbsFullSoc_DEC;
    _iq15 iq15Mix_SmoothRltSoc_DEC;
    _iq15 iq15Mix_CusRltSoc_DEC;
    int16_t i16Mix_CusRemCap_mAh;
    uint16_t u16Mix_CusFullCap_mAh;
    _iq15 iq15Mix_NomAbsSoc_DEC;
    int16_t i16Mix_NomRemCap_mAh;
    uint16_t u16Mix_NomFullCap_mAh;
    _iq15 iq15Mix_AbsEmptySoc;
    _iq15 iq15Mix_AbsEmptySoc_DEC;
    uint8_t u8Mix_CurFullMatrixCount;
    uint8_t u8Mix_CurEmptyMatrixCount;
    //APP
    tWarningFlags App_WarningFlag;
} tOutputParams;

//*****************************************************************************
// Input Parameters
//*****************************************************************************
typedef union {
    struct {
        //battery input parameters
        uint16_t u16Vcell_mV;  //Battery voltage, unit is mV
        int16_t i16Icell_mA;   //Battery current, unit is mA.
            //Discharge current is negative. Charge current is positive.
        int8_t i8Tcell_C;  //Battery temperature
    } sInputParams;
    uint8_t u8Table[5];
} tInputParams;

//*****************************************************************************
// One battery related parameters
//*****************************************************************************
typedef struct {
    tBattParamsConfig *pBattParamsCfg;
    tBattInfoInput *pBattInfoInput;
    tBattCapLearnParams *pCapLearnParams;
    tVGaugeFilterParams *pVGaugeParams;
    tIGaugeCoulometerParams *pIGaugeParams;
    tMixingParams *pMixingParams;
    tOutputParams *pOutputParams;
    tWarningFlags *pWarningFlag;
} tBattGlobalParams;

//*****************************************************************************
// Global parameters
//*****************************************************************************
typedef struct {
    tStatus sysTikFlag : 1;
    tBattGlobalParams *pBattGlobalParams;
    tCurtCalcFactors *pCurtCalcFactors;
} tGaugeApplication;

#endif
