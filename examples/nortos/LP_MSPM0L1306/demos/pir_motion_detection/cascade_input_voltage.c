
#include "cascade_input_voltage.h"
#include "ti_msp_dl_config.h"

#include <stdint.h>
#include <stdbool.h>


/**** Local Function Prototypes ***********************************************/

static inline uint16_t read_v_out2(void);
static inline uint32_t calc_cascade_input_voltage(uint16_t);

/**** Local Variables ********************************************************/

uint32_t DAC8_DATA = 0;
uint32_t v_out1_value = 0;

/**** Global Variables ********************************************************/
extern uint32_t PWM_DAC5_DATA;
extern volatile bool gCheckADC_0;

uint16_t v_out2_value;           // ADC0.8
extern uint16_t v_out_value;     // ADC0.4

uint32_t temp_v_dac1_value,temp_v_dac2_value;

/**** Functions **************************************************************/

/*!
 *
 */

uint32_t cascade_input_voltage (void)
{
    uint16_t v_out2;
    uint32_t v_in1;

    v_out2 = read_v_out2();
    v_in1 = calc_cascade_input_voltage(v_out2);

    return (v_in1);

}   /* calc_cascade_input_voltage() */



/*! \brief Calculates G1*G2*Vin1
 *
 *  G1*G2*Vin1 = Vout2 - (1-G1)*G2*Vdac1 - (1-G2)*Vdac2
 *
 *  \return G1*G2*Vin1
 */
static inline uint32_t
calc_cascade_input_voltage (uint16_t v_out2_value)
{
    uint16_t v_dac1_value;
    uint16_t v_dac2_value;
    uint32_t result;

    v_dac1_value = PWM_DAC5_DATA;
    v_dac2_value = DAC8_DATA;

    /*
     * -(G1*G2*Vin1/4096)*VDDA = -(Vout2/4096)*VDDA + ((1-G1)*G2*Vdac1/4096)*VDDA + ((1-G2)*Vdac2/256)*VDDA
     * -(G1*G2*Vin1/4096) = -(Vout2/4096) + ((1-G1)*G2*Vdac1/4096) + ((1-G2)*Vdac2/256)
     * -G1*G2*Vin1 = -Vout2 + (1-G1)*G2*Vdac1 + ((1-G2)*Vdac2/256)*4096
     *
     * -G1*G2*Vin1 = -Vout2 + (1-G1)*G2*Vdac1 + (1-G2)*Vdac2*(2^4)
     * -G1*G2*Vin1 = -Vout2 + M1*Vdac1 + M2*Vdac2*(2^4)
     * where:
     *      M1 = (1-G1)*G2 = 2^8
     *      M2 = (1-G2)    = 2^4 + 1
     * for:
     *      G1 = 17
     *      G2 = -16
     */
#   define M1_SHIFT     8       // Shift applied for G1*G2 multiplication
#   define M2_SHIFT     4       // Shift applied for G2 multiplication

    result = (uint32_t) v_dac1_value << M1_SHIFT;       // (1-G1)*G2*Vdac1
    v_dac2_value = v_dac2_value << 4;
    result += (uint32_t) v_dac2_value << M2_SHIFT;      //
    result += (uint32_t) v_dac2_value;                  // +(1-G2)*Vdac2
    result -= (uint32_t) v_out2_value;                  // -Vout2

    return (result);

}   /* calc_cascade_input_voltage() */


static inline uint16_t
read_v_out2 (void)
{

#   define G2_SHIFT 4
#   define G2       16

    static bool DAC8_1_init = false;          // initial value of DAC8.1

        if (DAC8_1_init == false)  // First pass after reset, need to initialize DAC8.1 value
        {
            //DAC8.1 initial value calculation
            DAC8_DATA = 120;
            DL_COMP_setDACCode0(COMP_0_INST, DAC8_DATA);
            DL_COMP_enable(COMP_0_INST);


            //software delay:  100ms
            delay_cycles(3000000);     // Wait 100ms for DAC signals to settle.


            // ADC Channel ADC0.8 (OPA1 output signal)
            //
            DL_ADC12_enableConversions(ADC12_0_INST);
            DL_ADC12_startConversion(ADC12_0_INST);

            DL_SYSCTL_setPowerPolicyRUN0SLEEP0();

            while (false == gCheckADC_0) {
                   __WFE();
                }

            v_out2_value = DL_ADC12_getMemResult(ADC12_0_INST, DL_ADC12_MEM_IDX_2);

            while ((v_out2_value < 1900 )|| (v_out2_value > 2200))
               {
                   // ADC Channel ADC0.8 (OPA1 output signal)
                   //
                   DL_ADC12_enableConversions(ADC12_0_INST);
                   DL_ADC12_startConversion(ADC12_0_INST);

                   DL_SYSCTL_setPowerPolicyRUN0SLEEP0();

                   while (false == gCheckADC_0) {
                       __WFE();
                   }

                   v_out2_value = DL_ADC12_getMemResult(ADC12_0_INST, DL_ADC12_MEM_IDX_2);

                   PWM_DAC5_DATA = DL_ADC12_getMemResult(ADC12_0_INST, DL_ADC12_MEM_IDX_1);
                   v_out1_value = DL_ADC12_getMemResult(ADC12_0_INST, DL_ADC12_MEM_IDX_0);

                   gCheckADC_0 = false;

                   if (v_out2_value < 1900)
                       DAC8_DATA++;
                   else if (v_out2_value > 2200)
                       DAC8_DATA--;

                   // Set output voltage:
                   DL_COMP_setDACCode0(COMP_0_INST, DAC8_DATA);
                   DL_COMP_enable(COMP_0_INST);

                   //software delay:  40ms
                   delay_cycles(1000000);     // Wait 40ms for DAC signals to settle.

               }

            DAC8_1_init = true;
        }
        else {
        }

    return (v_out2_value);

}   /* read_v_out2() */


/*** end of file ***/
