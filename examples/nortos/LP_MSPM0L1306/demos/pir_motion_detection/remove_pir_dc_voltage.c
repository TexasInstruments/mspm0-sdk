
#include "remove_pir_dc_voltage.h"
#include "ti_msp_dl_config.h"
#include <stdint.h>


// For G1 = 17, G1 -1 = 16 = 2^4
#define    G1_SHIFT       4        // Shift applied for (G1-1) multiplication


/**** Global Variable Declarations **************************************/
uint32_t PWM_DAC5_DATA = 0;
uint32_t PWM_setup_value = 0;
//**********************************

/**** Global Variable Extern Declarations **************************************/
extern bool gCheckADC_0;
extern uint16_t v_out2_value;
extern uint32_t v_out1_value;
//**********************************

/**** Local Function Prototypes ***********************************************/
static inline void initial_PWM_DAC5(void);
static inline void calc_pir_voltage(void);
static inline void ema_gained_input_voltage(void);
static inline void calc_pir_dac_dc_voltage(void);

/**** Local Variables ********************************************************/

uint32_t gained_input_voltage_value = 0;    // Calculated G1*Vin1(n)
//static
uint32_t ema_output_value_yn;        // y(n) <= EMA(G1*Vin1(n))

uint32_t temp,temp1,temp2,temp_DATA5 = 0;

uint16_t v_out_value;           // ADC0.4

uint32_t pir_dac_dc_value;

bool calc_pir_dac_dc_voltage_ff = false;

/**** Functions **************************************************************/

void
remove_pir_dc_voltage (void)
{
    initial_PWM_DAC5();
    calc_pir_voltage();
    ema_gained_input_voltage();
    calc_pir_dac_dc_voltage();

}   /* remove_pir_dc_voltage() */


/*! \brief Calculate Vin1 (PIR voltage, OA1-) from Vout1 (PA22, OA0O)
 *
 *  We use Vout1 and Vdac1 (DAC5) values to calculate equivalent input voltage.
 *  This is done for the fix gain of the first stage, G1 = 16.
 *      1) Measure first gain stage output voltage Vout1, PA22.
 *      2) Calculate input voltage as: Vout1 = G1*(Vdac1-Vin1) + Vdac1
 *                                     G1*Vin1 = -Vout1 + (1+G1)*Vdac1
 *
 *
 *  \return G1*Vin1
 */
static inline void
calc_pir_voltage (void)
{
    uint16_t v_dac_value;           // PWM_DAC5
    uint16_t temp_dac_value;
/* ------------------------------------------------------------------------- */
    // ADC Channel ADC0.4 (OPA0 output) signal)
    //
    DL_ADC12_enableConversions(ADC12_0_INST);
    DL_ADC12_startConversion(ADC12_0_INST);

    DL_SYSCTL_setPowerPolicyRUN0SLEEP0();

    while (false == gCheckADC_0) {
          __WFE();
        }

    v_out_value = DL_ADC12_getMemResult(ADC12_0_INST, DL_ADC12_MEM_IDX_0);

    temp_dac_value = DL_ADC12_getMemResult(ADC12_0_INST, DL_ADC12_MEM_IDX_1);

    if ((temp_dac_value > PWM_DAC5_DATA)&&((temp_dac_value - PWM_DAC5_DATA) < 5))
               ;
    else if ((temp_dac_value < PWM_DAC5_DATA)&&((PWM_DAC5_DATA - temp_dac_value) < 5))
               ;
    else
        PWM_DAC5_DATA = temp_dac_value;


    v_out2_value = DL_ADC12_getMemResult(ADC12_0_INST, DL_ADC12_MEM_IDX_2);

    v_out1_value = v_out_value;

    gCheckADC_0 = false;


/* ------------------------------------------------------------------------- */
    // Read output voltage:

    v_dac_value = PWM_DAC5_DATA;

    /*
     *
     * (G*Vin(n)/4096)*VDDA = (Vout/4096)*VDDA +((G-1)*Vdac/4096)*VDDA
     *
     * G*Vin(n) = Vout + (G-1)*Vdac
     *
     *
     * Vout1 = ADC0.4
     * Vdac = PWM DAC8
     * G = 17 => G - 1 = 16 = 2^4
     *
     */

    // G*Vin(n) = (G - 1)*Vdac
    gained_input_voltage_value = (uint32_t) v_dac_value << G1_SHIFT;

    // G*Vin(n) = G*Vin(n) + Vout
    gained_input_voltage_value += (uint32_t) v_out_value;

}   /* calc_pir_voltage() */


/*! \brief Calculates EMA on G*Vin(n).
 *
 *  Exponential moving average filter EMA is define as
 *  y(n) = alpha*x(n) + (1-alpha)*y(n-1)
 *
 *  \return EMA(G1*Vin1(n))
 */
static inline void
ema_gained_input_voltage (void)
{
   static bool ema_init = false;   // initial value of EMA filter
   /*
    *  EMA filter coefficient alpha = 1 / 2^n, for 1/256 => n = 8
    *  Reciprocal of alpha, 1 / alpha = 2^n
    *
    *  Note: n depends on the cutoff frequency and sampling time.
    */
#   define  ALPHA_SHIFT      8      // Shift applied after accumulation, n
   /*
    * From previous conversion, replacing (n) with (n-1) accumulator has
    * following value: result = x(n-1) + (1 / alpha) * y(n-2) - y(n-2).
    * As this is recursive filter using output value from previous
    * conversion, that is same as: result = (1 / alpha) * y(n-1).
    *
    * x(n)             = gained_input_voltage_value
    * y(n-1) = y(n)    = ema_output_value_yn
    * [(1/alpha)*y(n)] = ema_alpha_output_value
    */
   static uint32_t ema_alpha_output_value = 0;

   if ( ema_init == false)   // First pass after reset, need to initialize EMA
   {
       // y(0) = x(0)
       ema_output_value_yn = gained_input_voltage_value;

       // [(1/alpha)*y(0)] = (1/alpha)*y(0)
       ema_alpha_output_value = ema_output_value_yn << ALPHA_SHIFT;

       ema_init = true;     // EMA is initialized.
   }
   else
   {
        // [(1/alpha)*y(n)] = [(1/alpha)*y(n-1)] - y(n-1)
        ema_alpha_output_value -= ema_output_value_yn;

        // [(1/alpha)*y(n)] = [(1/alpha)*y(n)] + x(n)
        ema_alpha_output_value += gained_input_voltage_value;

        // y(n-1) <= y(n) = [1/alpha*y(n)] / (1/alpha)
        ema_output_value_yn = ema_alpha_output_value >> ALPHA_SHIFT;
   }
}   /* ema_calc_pir_voltage() */


/*! \brief Calc DAC
 *
 * Vdac(n) = (G*Vin(n) + Vout(n)) / (1 + G)
 *
 * G*Vin(n) = ema_output_value_yn
 * Vout = FSR/2
 *
 *  \return none
 */
static inline void
calc_pir_dac_dc_voltage (void)
{

#   define HALF_FSR     2048    // Full Scale Range is 4096, 12 bit ADC
    uint32_t PWM_dac_dc_value;

    if ((v_out_value < 1200)||(v_out_value > 3300))
    {
        pir_dac_dc_value = ema_output_value_yn;     // EMA(G1*Vin(n))
        pir_dac_dc_value -= (uint32_t) HALF_FSR;    // -Vout(n) = - FSR/2, desired

        pir_dac_dc_value >>= G1_SHIFT;      // pir_dac_dc_value / (G1 - 1)

        if (pir_dac_dc_value < 5)
            pir_dac_dc_value = 5;
        else if (pir_dac_dc_value > 4091)
        {
            pir_dac_dc_value =4090;
        }

        if (PWM_DAC5_DATA >= pir_dac_dc_value)
        {
            PWM_setup_value++;
        }

        else
        {
            PWM_setup_value--;
        }

        //Set output voltage:
        DL_TimerG_setCaptureCompareValue(PWM_0_INST, PWM_setup_value, DL_TIMER_CC_0_INDEX);
      }

}   /* calc_pir_dac_dc_voltage */


/*! \brief Initial PWM_DAC8.
 *
 *  1) Check if PWM_DAC8 value was initialized.
 *      If not:
 *          3.1 Measure PIR voltage, ADC0.2.
 *          3.2 Update PWM_DAC8 value with ADC0.3.
 *
 *  \return none
 */
static inline void
initial_PWM_DAC5 (void)
{

    static bool pwm_dac5_init = false;          // initial value of DAC8
    uint32_t pir_dac_value, OPA0_output_value;

    OPA0_output_value =0;

    if(pwm_dac5_init == false)  // First pass after reset, need to initialize DAC value
    {
        //Initial PWM DAC5 value
        pir_dac_value = 18;

        //while ((OPA0_output_value < 2500 )|| (OPA0_output_value > 4000))
        while ((OPA0_output_value < 1500 )|| (OPA0_output_value > 3300))
        {
            // ADC Channel ADC0.4 (OPA0 output)
            //
            DL_ADC12_enableConversions(ADC12_0_INST);
            DL_ADC12_startConversion(ADC12_0_INST);

            DL_SYSCTL_setPowerPolicyRUN0SLEEP0();

            while (false == gCheckADC_0) {
                __WFE();
            }

            OPA0_output_value = DL_ADC12_getMemResult(ADC12_0_INST, DL_ADC12_MEM_IDX_0);

            temp1 = OPA0_output_value;

            gCheckADC_0 = false;

            if (OPA0_output_value < 1500)
                pir_dac_value++;
            else if (OPA0_output_value > 3300)
                pir_dac_value--;

            PWM_setup_value = pir_dac_value;

            // Set output voltage:
            DL_TimerG_setCaptureCompareValue(PWM_0_INST, pir_dac_value, DL_TIMER_CC_0_INDEX);

            //software delay:  10ms
            delay_cycles(300000);     // Wait 10ms for DAC signals to settle.

        }

        pwm_dac5_init = true;
    }

}   /*     initial_PWM_DAC8() */


/*** end of file ***/
