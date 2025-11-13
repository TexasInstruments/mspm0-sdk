/*
 * comp_hal.c
 *
 *  Created on: Jun 4, 2024
 *      Author: a0500536
 */

#include <comparator_hal.h>

static const DL_COMP_Config gCOMPConfigA = {
        .channelEnable = DL_COMP_ENABLE_CHANNEL_POS,
        .mode          = DL_COMP_MODE_FAST,
        .negChannel    = DL_COMP_IMSEL_CHANNEL_1,
        .posChannel    = DL_COMP_IPSEL_CHANNEL_0,
        .hysteresis    = DL_COMP_HYSTERESIS_30,
        .polarity      = DL_COMP_POLARITY_NON_INV
};
static const DL_COMP_RefVoltageConfig gCOMPVRefConfigA = {
    .mode           = DL_COMP_REF_MODE_STATIC,
    .source         = DL_COMP_REF_SOURCE_VDDA_DAC,
    .terminalSelect = DL_COMP_REF_TERMINAL_SELECT_NEG,
    .controlSelect  = DL_COMP_DAC_CONTROL_SW,
    .inputSelect    = DL_COMP_DAC_INPUT_DACCODE0
};


static const DL_COMP_Config gCOMPConfigB = {
        .channelEnable = DL_COMP_ENABLE_CHANNEL_NEG,
        .mode          = DL_COMP_MODE_FAST,
        .negChannel    = DL_COMP_IMSEL_CHANNEL_0,
        .posChannel    = DL_COMP_IPSEL_CHANNEL_0,
        .hysteresis    = DL_COMP_HYSTERESIS_30,
        .polarity      = DL_COMP_POLARITY_INV
};
static const DL_COMP_RefVoltageConfig gCOMPVRefConfigB = {
    .mode           = DL_COMP_REF_MODE_STATIC,
    .source         = DL_COMP_REF_SOURCE_VDDA_DAC,
    .terminalSelect = DL_COMP_REF_TERMINAL_SELECT_POS,
    .controlSelect  = DL_COMP_DAC_CONTROL_SW,
    .inputSelect    = DL_COMP_DAC_INPUT_DACCODE0
};


static const DL_COMP_Config gCOMPConfigC = {
    .channelEnable = DL_COMP_ENABLE_CHANNEL_NEG,
    .mode          = DL_COMP_MODE_FAST,
    .negChannel    = DL_COMP_IMSEL_CHANNEL_1,
    .posChannel    = DL_COMP_IPSEL_CHANNEL_0,
    .hysteresis    = DL_COMP_HYSTERESIS_30,
    .polarity      = DL_COMP_POLARITY_INV
};
static const DL_COMP_RefVoltageConfig gCOMPVRefConfigC = {
    .mode           = DL_COMP_REF_MODE_STATIC,
    .source         = DL_COMP_REF_SOURCE_VDDA_DAC,
    .terminalSelect = DL_COMP_REF_TERMINAL_SELECT_POS,
    .controlSelect  = DL_COMP_DAC_CONTROL_SW,
    .inputSelect    = DL_COMP_DAC_INPUT_DACCODE0
};


//comp A (IN0+ vs 0.5V)
void update_comp_configA (void){

        DL_COMP_init(COMP_INST, (DL_COMP_Config *) &gCOMPConfigA);
        DL_COMP_enableOutputFilter(COMP_INST,DL_COMP_FILTER_DELAY_1200);
        DL_COMP_refVoltageInit(COMP_INST, (DL_COMP_RefVoltageConfig *) &gCOMPVRefConfigA);
        DL_COMP_setDACCode0(COMP_INST, COMP_DACCODE0);
        DL_COMP_enable(COMP_INST);

}

//comp B (IN0- vs 1.0V)
void update_comp_configB (void){
    //dump sysconfig functions + delay

        DL_COMP_init(COMP_INST, (DL_COMP_Config *) &gCOMPConfigB);
        DL_COMP_enableOutputFilter(COMP_INST,DL_COMP_FILTER_DELAY_1200);
        DL_COMP_refVoltageInit(COMP_INST, (DL_COMP_RefVoltageConfig *) &gCOMPVRefConfigB);
        DL_COMP_setDACCode0(COMP_INST, COMP_DACCODE1);
        DL_COMP_enable(COMP_INST);

}

//comp C (IN1- vs 1.5V)
void update_comp_configC (void){
    //dump sysconfig functions + delay

            DL_COMP_init(COMP_INST, (DL_COMP_Config *) &gCOMPConfigC);
            DL_COMP_enableOutputFilter(COMP_INST,DL_COMP_FILTER_DELAY_1200);
            DL_COMP_refVoltageInit(COMP_INST, (DL_COMP_RefVoltageConfig *) &gCOMPVRefConfigC);
            DL_COMP_setDACCode0(COMP_INST, COMP_DACCODE2);
            DL_COMP_enable(COMP_INST);

}

