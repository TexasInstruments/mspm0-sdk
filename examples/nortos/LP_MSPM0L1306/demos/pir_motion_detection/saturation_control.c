
#include "saturation_control.h"
#include "ti_msp_dl_config.h"

/**** Local Function Prototypes ***********************************************/


/**** Local Variables ********************************************************/
int8_t delta_v_out2;
uint16_t v_dac2_new;

/**** Global Variables ********************************************************/
extern uint32_t PWM_DAC8_DATA;
extern uint32_t DAC8_DATA;
extern uint16_t motion_detected;
/**** Local Variables ********************************************************/


/**** Functions **************************************************************/

/*! \brief Calculate the DAC8 value of 2nd stage OPA.
 *
 * can work with AFE disabled
 *
 *  \return none
 */
void
out_sig_sat_ctrl (uint16_t v_out2)
{

    static bool out_sig_sat_ctrl = false;
    static uint16_t out_sig_sat_ctrl_count = 0;


#   define HALF_FSR 128
#   define G2_SHIFT 4
#   define G2       16

    //v_out2 is 12bit, DAC8.1 is 8bit. Re-caculate the v_out2 for changing DAC8.1
    //v_out2 = ( v_out2 * 256 ) / 4096;
    //       = v_out2 / 2^4;

    if ((v_out2 < 1500)||(v_out2 > 3300))
    {
        v_out2 = v_out2 >> 4;
        delta_v_out2 = v_out2 - HALF_FSR;

        v_dac2_new = DAC8_DATA;

        v_dac2_new -= (delta_v_out2 >> G2_SHIFT)>>1;

        if (v_dac2_new>253)
            v_dac2_new = 253;
        else if (v_dac2_new <3)
            v_dac2_new = 3;

        DAC8_DATA = v_dac2_new;

        DL_COMP_setDACCode0(COMP_0_INST, DAC8_DATA);
        DL_COMP_enable(COMP_0_INST);

    }

}   /* out_sig_sat_ctrl() */


/*** end of file ***/
