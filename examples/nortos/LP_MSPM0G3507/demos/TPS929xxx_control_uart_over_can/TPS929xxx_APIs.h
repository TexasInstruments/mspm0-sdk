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

#ifndef TPS929XXX_APIS_H_
#define TPS929XXX_APIS_H_

extern unsigned int read_offset;

// Channel APIs
void enableCh(
    unsigned int dev_addr_x, unsigned int *ch_list, unsigned int channel_cnt);
void enableAllCh(unsigned int dev_addr_x);
void disableAllCh(unsigned int dev_addr_x);
void setCh(
    unsigned int dev_addr_x, unsigned int *ch_list, unsigned int channel_cnt);
void unsetCh(
    unsigned int dev_addr_x, unsigned int *ch_list, unsigned int channel_cnt);

// PWM APIs
void setPWMAllCh(
    unsigned int dev_addr_x, unsigned int *pwm_val, unsigned int same_val);
void setPWMhighAllCh(
    unsigned int dev_addr_x, unsigned int *pwm_val, unsigned int same_val);
void setPWMlowAllCh(
    unsigned int dev_addr_x, unsigned int *pwm_val, unsigned int same_val);
void setPWMoneCh(
    unsigned int dev_addr_x, unsigned int *pwm_val, unsigned int channel);
void setPWMhighOneCh(
    unsigned int dev_addr_x, unsigned int *pwm_val, unsigned int channel);
void setPWMlowOneCh(
    unsigned int dev_addr_x, unsigned int *pwm_val, unsigned int channel);
void enableExpPWM(unsigned int dev_addr_x);
void disableExpPWM(unsigned int dev_addr_x);
void setPWMfreq(unsigned int dev_addr_x, unsigned int pwm_freq);
void enablePWMshare(unsigned int dev_addr_x);
void disablePWMshare(unsigned int dev_addr_x);

// Current setting APIs
void setRefRange(unsigned int dev_addr_x, unsigned int ref_range);
void setIOUTAllCh(
    unsigned int dev_addr_x, unsigned int *iout_val, unsigned int same_val);
void setIOUToneCh(
    unsigned int dev_addr_x, unsigned int *iout_val, unsigned int channel);

// Diagnostics APIs
void LED_Update_Chip_Status(unsigned int dev_addr_x);
void enableDiagCh(
    unsigned int dev_addr_x, unsigned int *ch_list, unsigned int channel_cnt);
void enableDiagAllCh(unsigned int dev_addr_x);
void disableDiagAllCh(unsigned int dev_addr_x);
void setDiagCh(
    unsigned int dev_addr_x, unsigned int *ch_list, unsigned int channel_cnt);
void unsetDiagCh(
    unsigned int dev_addr_x, unsigned int *ch_list, unsigned int channel_cnt);
void enableDiagMask(unsigned int dev_addr_x, unsigned int mask);
void enableOutMask(unsigned int dev_addr_x, unsigned int mask);
void enableSLS(unsigned int dev_addr_x);
void disableSLS(unsigned int dev_addr_x);
void setSLSth(unsigned int dev_addr_x, unsigned int threshold_val,
    unsigned int threshold_set);
void setLowSupplyTh(unsigned int dev_addr_x, unsigned int threshold_val);
unsigned int checkErrFlag(unsigned int dev_addr_x);
unsigned int getErrFlags(unsigned int dev_addr_x);
unsigned int checkPorFlag(unsigned int dev_addr_x);
unsigned int checkSupFlags(unsigned int dev_addr_x);
unsigned int checkEepcrcFlag(unsigned int dev_addr_x);
void getOpenCh(unsigned int dev_addr_x, unsigned int *ch_list);
void getShortCh(unsigned int dev_addr_x, unsigned int *ch_list);
void getSLSCh(unsigned int dev_addr_x, unsigned int *ch_list);

// Miscellaneous APIs
void setLock(unsigned int dev_addr_x, unsigned int lock);
void clearLock(unsigned int dev_addr_x, unsigned int lock);
void clearLockAll(unsigned int dev_addr_x);
void setClr(unsigned int dev_addr_x, unsigned int clr_val);
void defaultReg(unsigned int dev_addr_x);
unsigned int checkFlag(
    unsigned int dev_addr_x, unsigned int reg_addr, unsigned int flag);
unsigned int getADCvalue(
    unsigned int dev_addr_x, unsigned int adc_in, unsigned int convert_res);
void writeSingleReg(unsigned int dev_addr_x, unsigned int reg_addr,
    unsigned int value, unsigned int enable, unsigned int writeWOread);
void forceFailSafe(unsigned int dev_addr_x);
void forceErr(unsigned int dev_addr_x);

// EEPROM APIs
void enableEepMode(unsigned int dev_addr_x);
void disableEepMode(unsigned int dev_addr_x);
void progEepStart(unsigned int dev_addr_x);
unsigned int checkEepProgReady(unsigned int dev_addr_x);
void enableEepSR(unsigned int dev_addr_x);
void disableEepSR(unsigned int dev_addr_x);

#endif /* TPS929XXX_APIS_H_ */
