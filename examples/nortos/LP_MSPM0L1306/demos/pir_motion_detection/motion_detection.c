
#include "ti_msp_dl_config.h"
#include "motion_detection.h"
#include <stdint.h>
#include "ui_led.h"


/**** Global Variable Extern Declarations **************************************/
//


/**** Local Function Prototypes ***********************************************/

static inline uint32_t ema_inst_val(uint32_t);
static inline uint32_t rate_inst_val(uint32_t);
static inline uint32_t KalmanFilter_inst_val(double xn);
static inline uint32_t abs_inst_dev(int32_t);
static inline uint32_t inst_detn_th(uint32_t);

/**** Local Variables ********************************************************/

uint32_t reference_level = 0;
int32_t inst_dev = 0;
uint32_t abs_inst_dev_value;

uint32_t motion_detected_count = 0;
uint32_t  abs_inst_dev_value_max;
uint32_t  abs_inst_dev_value_min;

uint32_t inst_detn_th_val;
uint16_t motion_detected;

uint32_t PIR_motion_detection_first_value = 0;
bool PIR_motion_detection_first = false;

unsigned int PIR_motion_count_value = 0;

static uint32_t inst_xn[5];    // inst_value samples xn(0)...xn(4)
static uint8_t rate_init_count = 0;

static double x_last=0;
static double p_last=0;


/**** Functions **************************************************************/

/*!
 *
 */
void
signal_analysis (uint32_t inst_value)
{
    reference_level = (uint32_t)KalmanFilter_inst_val(inst_value);

    inst_dev = rate_inst_val(reference_level);

    abs_inst_dev_value = abs_inst_dev(inst_dev);

#ifdef DATALOGGING_ENABLE
    //values leading to Motion Detected
    PIR_Inst_Detn_th_val[PIR_Data_pointer] = reference_level;  // Just testing the datalogging
    PIR_abs_inst_dev_value[PIR_Data_pointer] = abs_inst_dev_value;  // Just testing the datalogging
#endif

       if (abs_inst_dev_value > 30)
       {
           led_motion_detected();
       }

#ifdef DATALOGGING_ENABLE
    //Logging the Gained Cascade Input Voltage
    PIR_motion_detection_logging[PIR_Data_pointer] = motion_detected;  // Just testing the datalogging
#endif

}   /* signal_analysis() */


/*! \brief Calculates instantaneous detection threshold value.
 *
 *  \return y(n) = |x(n)|
 */
static inline uint32_t
inst_detn_th (uint32_t detn_th_xn)
{
#   define A 2;
//#   define A 1.5;

    static bool detn_th_init = false;   // initial value of EMA filter
    /*
     *  EMA filter coefficient alpha = 1 / 2^n, for 1/32 => n = 5
     *  Reciprocal of alpha, 1 / alpha = 2^n
     *
     *  Note: n depends on the cutoff frequency and sampling time.
     */
#   define  ALPHA_SHIFT      5      // Shift applied after accumulation, n
    /*
     * From previous conversion, replacing (n) with (n-1) accumulator has
     * following value: result = x(n-1) + (1 / alpha) * y(n-2) - y(n-2).
     * As this is recursive filter using output value from previous
     * conversion, that is same as: result = (1 / alpha) * y(n-1).
     *
     * x(n)             = ema_xn
     * y(n-1) = y(n)    = ema_yn
     * [(1/alpha)*y(n)] = ema_alpha_yn
     */
    static uint32_t detn_th_ema_yn;
    static uint32_t detn_th_ema_alpha_yn;
    uint32_t detn_th_ema_yn_temp;

    if ( detn_th_init == false)   // First pass after reset, need to initialize EMA
    {
        // y(0) = x(0)
        detn_th_ema_yn = detn_th_xn;

        // [(1/alpha)*y(0)] = (1/alpha)*y(0)
        detn_th_ema_alpha_yn = detn_th_ema_yn << ALPHA_SHIFT;

        detn_th_init = true;     // EMA is initialized.
    }
    else
    {
        // [(1/alpha)*y(n)] = [(1/alpha)*y(n-1)] - y(n-1)
        detn_th_ema_alpha_yn -= detn_th_ema_yn;

        // [(1/alpha)*y(n)] = [(1/alpha)*y(n)] + x(n)
        detn_th_ema_alpha_yn += detn_th_xn;

        // y(n-1) <= y(n) = [1/alpha*y(n)] / (1/alpha)
        detn_th_ema_yn = detn_th_ema_alpha_yn >> ALPHA_SHIFT;

    }
    return (detn_th_ema_yn << 1);   // A = 2

}   /* inst_detn_th() */


/*! \brief Calculates absolute value of instantaneous deviation of the signal.
 *
 *  \return y(n) = |x(n)|
 */
static inline uint32_t
abs_inst_dev (int32_t xn)
{
    uint32_t yn;

    if (xn<0)
    {
        yn = (uint32_t) ~xn + 1;
    }
    else
    {
        yn = (uint32_t) xn;
    }

    return(yn);

}   /* abs_inst_dev() */


/*! \brief Calculates EMA on instantaneous value x(n).
 *
 *  Exponential moving average filter EMA is define as
 *  y(n) = alpha*x(n) + (1-alpha)*y(n-1)
 *
 *  \return y(n) = EMA(x(n))
 */
static inline uint32_t
ema_inst_val (uint32_t xn)
{
    static bool ema_init = false;   // initial value of EMA filter
    /*
     *  EMA filter coefficient alpha = 1 / 2^n, for 1/32 => n = 5
     *  Reciprocal of alpha, 1 / alpha = 2^n
     *
     *  Note: n depends on the cutoff frequency and sampling time.
     */
#   define  ALPHA_SHIFT      5      // Shift applied after accumulation, n
    /*
     * From previous conversion, replacing (n) with (n-1) accumulator has
     * following value: result = x(n-1) + (1 / alpha) * y(n-2) - y(n-2).
     * As this is recursive filter using output value from previous
     * conversion, that is same as: result = (1 / alpha) * y(n-1).
     *
     * x(n)             = ema_xn
     * y(n-1) = y(n)    = ema_yn
     * [(1/alpha)*y(n)] = ema_alpha_yn
     */
    static uint32_t ema_yn;
    static uint32_t ema_alpha_yn;

    if ( ema_init == false)   // First pass after reset, need to initialize EMA
    {
        // y(0) = x(0)
        ema_yn = xn;

        // [(1/alpha)*y(0)] = (1/alpha)*y(0)
        ema_alpha_yn = ema_yn << ALPHA_SHIFT;

        ema_init = true;     // EMA is initialized.
   }
   else
   {
        // [(1/alpha)*y(n)] = [(1/alpha)*y(n-1)] - y(n-1)
        ema_alpha_yn -= ema_yn;

        // [(1/alpha)*y(n)] = [(1/alpha)*y(n)] + x(n)
        ema_alpha_yn += xn;

        // y(n-1) <= y(n) = [1/alpha*y(n)] / (1/alpha)
        ema_yn = ema_alpha_yn >> ALPHA_SHIFT;
    }
    return (ema_yn);

}   /*ema_inst_val */



/*! \brief Calculates rate on instantaneous value x(n).
 *
 *  Rate of curves is define as
 *  y(n) = x(n) - (x(n-1) + x(n-2) + x(n-3))/3
 *
 *  \return y(n) = rate(x(n))
 */
static inline uint32_t
rate_inst_val (uint32_t xn)
{
    static bool rate_init = false;   // initial value of Rate filter

    static uint32_t rate_yn;

    if ( rate_init == false)   // First 3 inst value pass after reset, need to initialize rate calculation
    {
        // discard inst_xn[0], store inst_xn[1] or inst_xn[2] or inst_xn[3]
        if (rate_init_count > 0)
            inst_xn[rate_init_count] = xn;

        // rate_yn = 0;
        rate_yn = 0;

        rate_init_count++;

        if (rate_init_count == 3)
            rate_init = true;     // rate is initialized.
   }
   else
   {

       //y(n) = x(n) - (x(n-1) + x(n-2) + x(n-3))/3
       rate_yn = xn - (inst_xn[0] + inst_xn[1] + inst_xn[2])/3;

       //y(n) = x(n) - (x(n-1) + x(n-2))/2
       //rate_yn = xn - (inst_xn[1] + inst_xn[2])/2;

       //y(n) = x(n) - x(n-1)
       //rate_yn = xn - (inst_xn[2]);

       // shift inst_xn[]
       inst_xn[0] = inst_xn[1];
       inst_xn[1] = inst_xn[2];
       //inst_xn[2] = inst_xn[3];
       //inst_xn[3] = inst_xn[4];

       // store inst_xn[2]
       inst_xn[2] = xn;

    }
    return (rate_yn);

}   /*rate_inst_val */


/*! \brief Calculates Kalman filter on instantaneous value x(n).
 *
 *  Kalman filter calculations are define as
 *  X(k|k-1)=X(k-1|k-1)
 *  P(k|k-1)=P(k-1|k-1)+Q
 *  K(k)=P(k|k-1)/(P(k|k-1)+R)
 *  X(k|k)=X(k|k-1)+K(k)*(Z(k)-X(k|k-1))
 *  P(k|k)=(1-K(k))*P(k|k-1)
 *
 *  See https://en.wikipedia.org/wiki/Kalman_filter
 *
 *  \return y(n) = Kalmanfilter(x(n))
 */
static inline uint32_t
KalmanFilter_inst_val(double xn)
{

    //Process Noise
    #define ProcessNoise_Q 0.6   //ProcessNoise_Q initial value could get anyone but 0.
    //Measure Noise
    #define MeasureNoise_R 15
    /*
     *  Q:ProcessNoise_Q,increase Q,Faster dynamic response and worse convergence stability.
     *  R:MeasureNoise_R,increase R,Slower dynamic response and better convergence stability.
     *
     *  R:Adjust the similarity between the filtered curve and the measured curve,the smaller the R,the closer it is.
     *  Q:Adjust the smoothness of the filtered curve,the smaller the Q, the smoother.
     */

    double R = MeasureNoise_R;
    double Q = ProcessNoise_Q;

    double x_mid;
    double x_now;

    double p_mid;
    double p_now;

    double kg;


    /*
     *  Kalman filter's 5 important calculations
     */
    x_mid=x_last;                       //x_last=x(k-1|k-1),x_mid=x(k|k-1), X(k|k-1)=X(k-1|k-1)
    p_mid=p_last+Q;                     //p_mid=p(k|k-1),p_last=p(k-1|k-1),Q=noise, P(k|k-1)=P(k-1|k-1)+Q
    kg=p_mid/(p_mid+R);                 //kg=K(k),p_mid=P(k|k-1),R=noise,K(k)=P(k|k-1)/(P(k|k-1)+R)
    x_now=x_mid+kg*(xn-x_mid);          //x_now=X(k|k),Kalman filter estimate optimize value,xn=Z(k),X(k|k)=X(k|k-1)+K(k)*(Z(k)-X(k|k-1))
    p_now=(1-kg)*p_mid;                 //X(k|k)'s covariance, p_now=P(k|k),P(k|k)=(1-K(k))*P(k|k-1)

    // Update covariance value p_last and system control status value x_last
    p_last = p_now;                     //Update covariance value
    x_last = x_now;                     //Update system control status value


    return (x_now);

}   /*KalmanFilter_inst_val */



/*** end of file ***/
