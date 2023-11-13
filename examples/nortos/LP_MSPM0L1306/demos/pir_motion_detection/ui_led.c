
#include <ti_msp_dl_config.h>
#include "ui_led.h"
#include "bsp.h"

/**** Functions **************************************************************/

/*! \brief Use TIMG1 to create 0.5 s time delay for LED.
 *
 *  \return none
 */
void
led_motion_detected (void)
{

    Green_LED_on;

    DL_TimerG_startCounter(TIMER_3_INST);

}   /* led_motion_detected() */

/*** end of file ***/
