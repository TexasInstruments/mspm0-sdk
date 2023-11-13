
#include "ti_msp_dl_config.h"
#include "despiking_derippling.h"
#include <stdint.h>



  //#   define  ALPHA_SHIFT      8      // Shift applied after accumulation, n

#   define  NUM_SAMPLES     6   // Number of the input samples x(n)



/**** Local Function Prototypes ***********************************************/

static inline void spike_detection(uint32_t);
inline void spike_exision_smoothing(void);
inline void samples_arr_ud(void);


/**** Local Variables ********************************************************/

static uint32_t xn[NUM_SAMPLES];    // Input signal samples xn(0)...xn(5)
uint32_t xn_avg;                    // Average value of 4 samples, without positive and negative spikes
uint16_t pos_spike_index;           // Positive spike index
uint16_t neg_spike_index;           // Negative spike position


/**** Functions **************************************************************/

/*!
 * Need description
 *
 * \return xn_avg
 */
uint32_t
despiking_derippling_calc (uint32_t new_sample)
{
    spike_detection(new_sample);

    spike_exision_smoothing ();     // Average 4 of 6 samples.

    samples_arr_ud();

    return (xn_avg);

}   /* despiking_derippling_calc() */


/*! \brief Updates samples, apply z^-1
 *
 *  \return xn(i)
 */
void
samples_arr_ud (void)
{
    uint16_t cnt;

    for (cnt = 1 ; cnt < NUM_SAMPLES; cnt++)
    {
        xn[cnt - 1] = xn[cnt];        // xn(i-1) = xn(i)
    }
}   /* samples_arr_ud() */


/*! \brief Removes Spikes and smoothen region.
 *
 *  \return xn_avg
 */
void
spike_exision_smoothing (void)
{
    xn_avg = 0;
    uint16_t cnt;
    for (cnt = NUM_SAMPLES ; cnt > 0; cnt--)
    {
        if ((cnt-1 != pos_spike_index) && (cnt-1 != neg_spike_index))
        {
            xn_avg += xn[cnt - 1];      // Average only not spike samples
        }
    }
    xn_avg >>= 2;      // Rounding? +0.5
                       // xn_avg = xn_avg / 4

}   /* spike_exision_smoothing() */


/*! \brief Detects position of the spikes
 *
 *  Detects position of the most positive and most negative spike
 *  in the array xn(i).
 *
 *  \return pos_spike_index, neg_spike_index
 */
static inline void
spike_detection (uint32_t new_sample)
{
    static bool despiking_init = false;   // initial values of samples array xn(i)
    uint32_t max_xn;         // Maximum value of the input signal samples x(n)
    uint32_t min_xn;         // Maximum value of the input signal samples x(n)
    uint16_t cnt;

    if ( despiking_init == false)   // First pass after reset, need to initialize EMA
    {
        for (cnt = NUM_SAMPLES ; cnt > 0; cnt--)
        {
            xn[cnt - 1] = new_sample;
        }
        xn_avg = new_sample;
        despiking_init = true;     // Despiking is initialized.
    }
    else
    {
        xn[NUM_SAMPLES - 1] = new_sample;         // New signal sample
        max_xn = xn[NUM_SAMPLES - 1];
        min_xn = xn[0];
        pos_spike_index = NUM_SAMPLES - 1;
        neg_spike_index = 0;
        /*
         * Find position of the spikes
         */
        for (cnt = NUM_SAMPLES ; cnt > 0; cnt--)
        {
            /*
             * Detect positive spike
             */
            if (xn[cnt-1] > max_xn)
            {
                max_xn = xn[cnt - 1];
                pos_spike_index = cnt - 1;
            }
            /*
             * Detect negative spike
             */
            if (xn[cnt-1]< min_xn)
            {
                min_xn = xn[cnt - 1];
                neg_spike_index = cnt - 1;
            }
        }
    }
}   /* spike_detection() */

/*** end of file ***/
