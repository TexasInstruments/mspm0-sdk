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

#include "ti_msp_dl_config.h"

// motor configure
#define target_speed (150)         // speed (MAX:333)
#define run_direction (1)          // 1:up 0:down
#define ENCODER_RESOLUTION (1320)  // 11*4*30
#define RECORD_TIME (0.05)         // record time(s)
#define CC_value_MAX (475)         // D=97%
#define CC_value_MIN (25)          // D=3%
#define current_MAX (0.98)

// Speed PID controller parameters
#define PIDSPEED_KP (0.01)
#define PIDSPEED_KI (0.0005)
#define PIDSPEED_KD (0)
volatile float speed_sum_error  = 0.0;
volatile float speed_error_last = 0.0;
volatile int Capture_count      = 0;
volatile int Last_count         = 0;
volatile int distance           = 0;
volatile float actual_speed     = 0;

// current PID controller parameters
#define PIDCURRENT_KP (250)
#define PIDCURRENT_KI (100)
#define PIDCURRENT_KD (0)
volatile float current_sum_error  = 0.0;
volatile float current_error_last = 0.0;
volatile float actual_current     = 0;
volatile float target_current     = 0;
volatile int PRE_ADC              = 0;
volatile int CC_value             = 25;

//main function
int main(void)
{
    SYSCFG_DL_init();
    switch (run_direction) {
        case 1:  // UP
            DL_GPIO_clearPins(GPIO_GRP_0_PORT, GPIO_GRP_0_PIN_0_PIN);
            DL_GPIO_setPins(GPIO_GRP_0_PORT, GPIO_GRP_1_PIN_1_PIN);
            DL_TimerA_setCaptureCompareValue(
                PWM_0_INST, 500, DL_TIMER_CC_0_INDEX);
            break;
        case 0:  // DOWM
            DL_GPIO_setPins(GPIO_GRP_0_PORT, GPIO_GRP_0_PIN_0_PIN);
            DL_GPIO_clearPins(GPIO_GRP_0_PORT, GPIO_GRP_1_PIN_1_PIN);
            DL_TimerA_setCaptureCompareValue(
                PWM_0_INST, 0, DL_TIMER_CC_0_INDEX);
            break;
    }
    NVIC_EnableIRQ(TIMER_0_INST_INT_IRQN);
}

float speed_PID_realize(float Kp, float Ki, float Kd)
{
    float PID_out     = 0;
    float speed_error = 0;
    // get speed from encoder
    switch (DL_TimerG_getQEIDirection(QEI_0_INST)) {
        case 1:  // UP
            Capture_count = (QEI_0_INST->COUNTERREGS.CTR);
            distance      = Capture_count - Last_count;
            distance      = (distance < 0) ? (0xFFFF + distance) : distance;
            actual_speed =
                (float) distance * 60 / (ENCODER_RESOLUTION * RECORD_TIME);
            break;
        case 0:  // DOWM
            Capture_count = (QEI_0_INST->COUNTERREGS.CTR);
            distance      = Last_count - Capture_count;
            distance      = (distance < 0) ? (0xFFFF + distance) : distance;
            actual_speed =
                (float) distance * 60 / (ENCODER_RESOLUTION * RECORD_TIME);
            break;
    }
    Last_count  = Capture_count;
    speed_error = target_speed - actual_speed;
    //limiting closed-loop deadband
    speed_error = (speed_error < 3 && speed_error > -3) ? 0 : speed_error;
    // PID realize
    speed_sum_error += speed_error;  // error integrate
    //Preventing integral saturation
    if (speed_sum_error > 6000)
        speed_sum_error = 6000;
    else if (speed_sum_error < -6000)
        speed_sum_error = -6000;
    PID_out = Kp * speed_error + Ki * speed_sum_error +
              Kd * (speed_error - speed_error_last);  // PID calculation
    speed_error_last = speed_error;                   // Error propagation
    return PID_out;
}

float current_PID_realize(float target_current, float Kp, float Ki, float Kd)
{
    float PID_out;
    float current_error = 0;
    int SUM_ADC         = 0;
    //get current from ADC
    for (int i = 0; i < 30; i++) {
        DL_ADC12_startConversion(ADC12_0_INST);
        SUM_ADC += DL_ADC12_getMemResult(ADC12_0_INST, DL_ADC12_MEM_IDX_0);
        DL_ADC12_enableConversions(ADC12_0_INST);
    }
    //average filter
    PRE_ADC = SUM_ADC / 30;
    //10 bit ADC number convert to real current value
    actual_current = 3.3 * 2.5 * PRE_ADC / 1024;
    current_error  = target_current - actual_current;
    //limiting closed-loop deadband
    current_error =
        (current_error / target_current < 0.1 && current_error > -0.1)
            ? 0
            : current_error;
    current_sum_error += current_error;  // error integrate
    //Preventing integral saturation
    if (current_sum_error > 10)
        current_sum_error = 10;
    else if (current_sum_error < -10)
        current_sum_error = -10;
    //PID calculate
    PID_out = Kp * current_error + Ki * current_sum_error +
              Kd * (current_error - current_error_last);  // PID calculation
    current_error_last = current_error;                   // Error propagationA
    return PID_out;
}

void TIMER_0_INST_IRQHandler(void)
{
    switch (DL_TimerG_getPendingInterrupt(TIMER_0_INST)) {
        case DL_TIMER_IIDX_ZERO:
            //speed loop
            target_current =
                speed_PID_realize(PIDSPEED_KP, PIDSPEED_KI, PIDSPEED_KD);
            //current loop
            target_current = (target_current > current_MAX)
                                 ? current_MAX
                                 : target_current;  // limit max current
            target_current = (target_current < 0)
                                 ? 0
                                 : target_current;  // limit min current
            switch (DL_TimerG_getQEIDirection(QEI_0_INST)) {
                case 1:  // UP
                    CC_value =
                        500 - current_PID_realize(target_current,
                                  PIDCURRENT_KP, PIDCURRENT_KI, PIDCURRENT_KD);
                    break;
                case 0:  // DOWM
                    CC_value = current_PID_realize(target_current,
                        PIDCURRENT_KP, PIDCURRENT_KI, PIDCURRENT_KD);
                    break;
            }
            //limit D according to dead time
            CC_value = (CC_value > CC_value_MAX) ? CC_value_MAX : CC_value;
            CC_value = (CC_value < CC_value_MIN) ? CC_value_MIN : CC_value;
            //update duty cycle
            DL_TimerA_setCaptureCompareValue(
                PWM_0_INST, CC_value, DL_TIMER_CC_0_INDEX);
            break;
        default:
            break;
    }
}
