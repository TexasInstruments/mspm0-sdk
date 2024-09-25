/*!****************************************************************************
 *  @file       _IQNdiv_mathacl.c
 *  @brief      Functions to divide two values of IQN type.
 *
 *  <hr>
 ******************************************************************************/

#ifndef IQNDIV_MATHACL
#define IQNDIV_MATHACL
#include <stdint.h>

#include <ti/devices/msp/msp.h>

/**
 * @brief Divide two values of IQN type, using MathACL
 *
 * @param iqNInput1       IQN type value numerator to be divided.
 * @param iqNInput2       IQN type value denominator to divide by.
 * @param q_value         IQ format.
 *
 * @return                IQN type result of the multiplication.
 */
__STATIC_INLINE int_fast32_t __IQNdiv_mathacl(int_fast32_t iqNInput1, int_fast32_t iqNInput2, const int8_t q_value)
{
    /* write control */
    MATHACL->CTL = 4 | (q_value<<8) | (1 << 5);
    /* write operands to HWA. OP2 = divisor, OP1 = dividend */
    MATHACL->OP2 = iqNInput2;
    /* trigger is write to OP1 */
    MATHACL->OP1 = iqNInput1;
    /* read quotient and remainder */
    return MATHACL->RES1;
}
#endif
