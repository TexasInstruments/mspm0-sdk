
#include "ti_msp_dl_config.h"

int pwm_count = 1800; // initial ccr count based on 10% duty cycle
int dc = 10;          // initialized to 10 % duty cycle
int mode = 1; // 1 = up (will indicate you need to increase dc), 0 = down (will
              // decrease dc)

int main(void) {
  SYSCFG_DL_init();

  NVIC_EnableIRQ(PWM_0_INST_INT_IRQN);
  DL_TimerG_startCounter(PWM_0_INST);

  while (1) {
    __WFI();
  }
}

void PWM_0_INST_IRQHandler(void) {
  switch (DL_TimerG_getPendingInterrupt(PWM_0_INST)) {
  case DL_TIMER_IIDX_LOAD:
    if (dc <= 10) {
      mode = 1;
    } // if reached lowest dc (10%), increase dc
    else if (dc >= 90) {
      mode = 0;
    } // if reached highest dc (90%), decrease dc
    if (mode) {
      pwm_count -= 20;
      dc += 1;
    } // up
    if (!mode) {
      pwm_count += 20;
      dc -= 1;
    } // down
    DL_TimerG_setCaptureCompareValue(PWM_0_INST, pwm_count,
                                     DL_TIMER_CC_1_INDEX); // update ccr1 value
    break;
  default:
    break;
  }
}
