/*!****************************************************************************
 *  @file       _IQNmpy_rts.c
 *  @brief      Functions to multiply two values of IQN type.
 *
 *  <hr>
 ******************************************************************************/
#ifndef _IQNMPY_RTS_H_
#define _IQNMPY_RTS_H_
#include <stdint.h>

#include <ti/devices/msp/msp.h>

/**
 * @brief Multiply two values of IQN type.
 *
 * @param iqNInput1       IQN type value input to be multiplied.
 * @param iqNInput2       IQN type value input to be multiplied.
 * @param q_value         IQ format.
 *
 * @return                IQN type result of the multiplication.
 */
#if defined (__TI_COMPILER_VERSION__)
    #pragma FUNC_ALWAYS_INLINE(__IQNmpy)
#elif defined(__IAR_SYSTEMS_ICC__)
    #pragma inline=forced
#endif
__STATIC_INLINE int_fast32_t __IQNmpy_rts(int_fast32_t iqNInput1, int_fast32_t iqNInput2, const int8_t q_value)
{
    int_fast64_t iqNResult;
    
    iqNResult = (int_fast64_t)iqNInput1 * (int_fast64_t)iqNInput2;
    iqNResult = iqNResult >> q_value;

    return (int_fast32_t)iqNResult;
}
#endif
