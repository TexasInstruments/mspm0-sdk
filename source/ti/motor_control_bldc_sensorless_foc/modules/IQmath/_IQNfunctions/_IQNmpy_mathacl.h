/*!****************************************************************************
 *  @file       _IQNmpy_mathacl.c
 *  @brief      Functions to multiply two values of IQN type.
 *
 *  <hr>
 ******************************************************************************/
#ifndef IQNMPY_MATHACL
#define IQNMPY_MATHACL
#include <stdint.h>

#include <ti/devices/msp/msp.h>

/**
 * @brief Multiply two values of IQN type, using MathACL.
 *
 * @param iqNInput1       IQN type value input to be multiplied.
 * @param iqNInput2       IQN type value input to be multiplied.
 * @param q_value         IQ format.
 *
 * @return                IQN type result of the multiplication.
 */
__STATIC_INLINE int_fast32_t __IQNmpy_mathacl(int_fast32_t iqNInput1, int_fast32_t iqNInput2, const int8_t q_value)
{
    /* write control */
    MATHACL->CTL = 6 | (q_value<<8) | (1 << 5);
    /* write operands to HWA */
    MATHACL->OP2 = iqNInput2;
    /* write trigger word last */
    MATHACL->OP1 = iqNInput1;
    /* read iqmpy product */
    return MATHACL->RES1;
}
#endif
