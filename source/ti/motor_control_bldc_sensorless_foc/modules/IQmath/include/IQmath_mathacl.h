/*!****************************************************************************
 *  @file       IQmath_mathacl.h
 *  @brief      Library of IQMath operations.
 *
 *  <hr>
 ******************************************************************************/
#ifndef __IQMATH_MATHACL_H__
#define __IQMATH_MATHACL_H__

//*****************************************************************************
//
// If building with a C++ compiler, make all of the definitions in this header
// have a C binding.
//
//*****************************************************************************
#ifdef __cplusplus
extern "C"
{
#endif

//*****************************************************************************
/*!
 * @brief The IQ format to be used when the IQ format is not explicitly specified
 * (such as _IQcos instead of _IQ16cos).  This value must be between 1 and 30,
 * inclusive.
 */
//*****************************************************************************
#ifndef GLOBAL_IQ
#define GLOBAL_IQ                24
#endif

//*****************************************************************************
//
// Include some standard headers.
//
//*****************************************************************************
#include <limits.h>
#include <stdlib.h>
#include <stdint.h>

#ifndef DOXYGEN_SHOULD_SKIP_THIS
//*****************************************************************************
//
// Various Useful Constant Definitions:
//
//*****************************************************************************
#define Q30                     30
#define Q29                     29
#define Q28                     28
#define Q27                     27
#define Q26                     26
#define Q25                     25
#define Q24                     24
#define Q23                     23
#define Q22                     22
#define Q21                     21
#define Q20                     20
#define Q19                     19
#define Q18                     18
#define Q17                     17
#define Q16                     16
#define Q15                     15
#define Q14                     14
#define Q13                     13
#define Q12                     12
#define Q11                     11
#define Q10                     10
#define Q9                      9
#define Q8                      8
#define Q7                      7
#define Q6                      6
#define Q5                      5
#define Q4                      4
#define Q3                      3
#define Q2                      2
#define Q1                      1
#ifndef QG
#define QG                      GLOBAL_IQ
#endif

#define MAX_IQ_POS              LONG_MAX
#define MAX_IQ_NEG              LONG_MIN
#define MIN_IQ_POS              1
#define MIN_IQ_NEG              -1

//*****************************************************************************
//
// The types for the various IQ formats.
//
//*****************************************************************************
typedef int32_t _iq30;
typedef int32_t _iq29;
typedef int32_t _iq28;
typedef int32_t _iq27;
typedef int32_t _iq26;
typedef int32_t _iq25;
typedef int32_t _iq24;
typedef int32_t _iq23;
typedef int32_t _iq22;
typedef int32_t _iq21;
typedef int32_t _iq20;
typedef int32_t _iq19;
typedef int32_t _iq18;
typedef int32_t _iq17;
typedef int32_t _iq16;
typedef int32_t _iq15;
typedef int32_t _iq14;
typedef int32_t _iq13;
typedef int32_t _iq12;
typedef int32_t _iq11;
typedef int32_t _iq10;
typedef int32_t _iq9;
typedef int32_t _iq8;
typedef int32_t _iq7;
typedef int32_t _iq6;
typedef int32_t _iq5;
typedef int32_t _iq4;
typedef int32_t _iq3;
typedef int32_t _iq2;
typedef int32_t _iq1;
typedef int32_t _iq;

#endif /* DOXYGEN_SHOULD_SKIP_THIS */

//*****************************************************************************
//
// Multiplies two IQ numbers.
//
//*****************************************************************************
#ifndef DOXYGEN_SHOULD_SKIP_THIS
extern _iq30 _IQ30mpy_mathacl(_iq30 A, _iq30 B);
extern _iq29 _IQ29mpy_mathacl(_iq29 A, _iq29 B);
extern _iq28 _IQ28mpy_mathacl(_iq28 A, _iq28 B);
extern _iq27 _IQ27mpy_mathacl(_iq27 A, _iq27 B);
extern _iq26 _IQ26mpy_mathacl(_iq26 A, _iq26 B);
extern _iq25 _IQ25mpy_mathacl(_iq25 A, _iq25 B);
extern _iq24 _IQ24mpy_mathacl(_iq24 A, _iq24 B);
extern _iq23 _IQ23mpy_mathacl(_iq23 A, _iq23 B);
extern _iq22 _IQ22mpy_mathacl(_iq22 A, _iq22 B);
extern _iq21 _IQ21mpy_mathacl(_iq21 A, _iq21 B);
extern _iq20 _IQ20mpy_mathacl(_iq20 A, _iq20 B);
extern _iq19 _IQ19mpy_mathacl(_iq19 A, _iq19 B);
extern _iq18 _IQ18mpy_mathacl(_iq18 A, _iq18 B);
extern _iq17 _IQ17mpy_mathacl(_iq17 A, _iq17 B);
extern _iq16 _IQ16mpy_mathacl(_iq16 A, _iq16 B);
extern _iq15 _IQ15mpy_mathacl(_iq15 A, _iq15 B);
extern _iq14 _IQ14mpy_mathacl(_iq14 A, _iq14 B);
extern _iq13 _IQ13mpy_mathacl(_iq13 A, _iq13 B);
extern _iq12 _IQ12mpy_mathacl(_iq12 A, _iq12 B);
extern _iq11 _IQ11mpy_mathacl(_iq11 A, _iq11 B);
extern _iq10 _IQ10mpy_mathacl(_iq10 A, _iq10 B);
extern _iq9 _IQ9mpy_mathacl(_iq9 A, _iq9 B);
extern _iq8 _IQ8mpy_mathacl(_iq8 A, _iq8 B);
extern _iq7 _IQ7mpy_mathacl(_iq7 A, _iq7 B);
extern _iq6 _IQ6mpy_mathacl(_iq6 A, _iq6 B);
extern _iq5 _IQ5mpy_mathacl(_iq5 A, _iq5 B);
extern _iq4 _IQ4mpy_mathacl(_iq4 A, _iq4 B);
extern _iq3 _IQ3mpy_mathacl(_iq3 A, _iq3 B);
extern _iq2 _IQ2mpy_mathacl(_iq2 A, _iq2 B);
extern _iq1 _IQ1mpy_mathacl(_iq1 A, _iq1 B);

#endif /* DOXYGEN_SHOULD_SKIP_THIS */
/**
 * @brief Multiplies two global IQ format numbers.
 *
 * @param A               Global IQ format number to be multiplied.
 * @param B               Global IQ format number to be multiplied. 
 *
 * @return                Global IQ type result of multiplication.
 */
#if GLOBAL_IQ == 30
#define _IQmpy_mathacl(A, B)            _IQ30mpy_mathacl(A, B)
#endif
#if GLOBAL_IQ == 29
#define _IQmpy_mathacl(A, B)            _IQ29mpy_mathacl(A, B)
#endif
#if GLOBAL_IQ == 28
#define _IQmpy_mathacl(A, B)            _IQ28mpy_mathacl(A, B)
#endif
#if GLOBAL_IQ == 27
#define _IQmpy_mathacl(A, B)            _IQ27mpy_mathacl(A, B)
#endif
#if GLOBAL_IQ == 26
#define _IQmpy_mathacl(A, B)            _IQ26mpy_mathacl(A, B)
#endif
#if GLOBAL_IQ == 25
#define _IQmpy_mathacl(A, B)            _IQ25mpy_mathacl(A, B)
#endif
#if GLOBAL_IQ == 24
#define _IQmpy_mathacl(A, B)            _IQ24mpy_mathacl(A, B)
#endif
#if GLOBAL_IQ == 23
#define _IQmpy_mathacl(A, B)            _IQ23mpy_mathacl(A, B)
#endif
#if GLOBAL_IQ == 22
#define _IQmpy_mathacl(A, B)            _IQ22mpy_mathacl(A, B)
#endif
#if GLOBAL_IQ == 21
#define _IQmpy_mathacl(A, B)            _IQ21mpy_mathacl(A, B)
#endif
#if GLOBAL_IQ == 20
#define _IQmpy_mathacl(A, B)            _IQ20mpy_mathacl(A, B)
#endif
#if GLOBAL_IQ == 19
#define _IQmpy_mathacl(A, B)            _IQ19mpy_mathacl(A, B)
#endif
#if GLOBAL_IQ == 18
#define _IQmpy_mathacl(A, B)            _IQ18mpy_mathacl(A, B)
#endif
#if GLOBAL_IQ == 17
#define _IQmpy_mathacl(A, B)            _IQ17mpy_mathacl(A, B)
#endif
#if GLOBAL_IQ == 16
#define _IQmpy_mathacl(A, B)            _IQ16mpy_mathacl(A, B)
#endif
#if GLOBAL_IQ == 15
#define _IQmpy_mathacl(A, B)            _IQ15mpy_mathacl(A, B)
#endif
#if GLOBAL_IQ == 14
#define _IQmpy_mathacl(A, B)            _IQ14mpy_mathacl(A, B)
#endif
#if GLOBAL_IQ == 13
#define _IQmpy_mathacl(A, B)            _IQ13mpy_mathacl(A, B)
#endif
#if GLOBAL_IQ == 12
#define _IQmpy_mathacl(A, B)            _IQ12mpy_mathacl(A, B)
#endif
#if GLOBAL_IQ == 11
#define _IQmpy_mathacl(A, B)            _IQ11mpy_mathacl(A, B)
#endif
#if GLOBAL_IQ == 10
#define _IQmpy_mathacl(A, B)            _IQ10mpy_mathacl(A, B)
#endif
#if GLOBAL_IQ == 9
#define _IQmpy_mathacl(A, B)            _IQ9mpy_mathacl(A, B)
#endif
#if GLOBAL_IQ == 8
#define _IQmpy_mathacl(A, B)            _IQ8mpy_mathacl(A, B)
#endif
#if GLOBAL_IQ == 7
#define _IQmpy_mathacl(A, B)            _IQ7mpy_mathacl(A, B)
#endif
#if GLOBAL_IQ == 6
#define _IQmpy_mathacl(A, B)            _IQ6mpy_mathacl(A, B)
#endif
#if GLOBAL_IQ == 5
#define _IQmpy_mathacl(A, B)            _IQ5mpy_mathacl(A, B)
#endif
#if GLOBAL_IQ == 4
#define _IQmpy_mathacl(A, B)            _IQ4mpy_mathacl(A, B)
#endif
#if GLOBAL_IQ == 3
#define _IQmpy_mathacl(A, B)            _IQ3mpy_mathacl(A, B)
#endif
#if GLOBAL_IQ == 2
#define _IQmpy_mathacl(A, B)            _IQ2mpy_mathacl(A, B)
#endif
#if GLOBAL_IQ == 1
#define _IQmpy_mathacl(A, B)            _IQ1mpy_mathacl(A, B)
#endif

//*****************************************************************************
//
// Divides two IQ numbers.
//
//*****************************************************************************
#ifndef DOXYGEN_SHOULD_SKIP_THIS
extern _iq30 _IQ30div_mathacl(_iq30 A, _iq30 B);
extern _iq29 _IQ29div_mathacl(_iq29 A, _iq29 B);
extern _iq28 _IQ28div_mathacl(_iq28 A, _iq28 B);
extern _iq27 _IQ27div_mathacl(_iq27 A, _iq27 B);
extern _iq26 _IQ26div_mathacl(_iq26 A, _iq26 B);
extern _iq25 _IQ25div_mathacl(_iq25 A, _iq25 B);
extern _iq24 _IQ24div_mathacl(_iq24 A, _iq24 B);
extern _iq23 _IQ23div_mathacl(_iq23 A, _iq23 B);
extern _iq22 _IQ22div_mathacl(_iq22 A, _iq22 B);
extern _iq21 _IQ21div_mathacl(_iq21 A, _iq21 B);
extern _iq20 _IQ20div_mathacl(_iq20 A, _iq20 B);
extern _iq19 _IQ19div_mathacl(_iq19 A, _iq19 B);
extern _iq18 _IQ18div_mathacl(_iq18 A, _iq18 B);
extern _iq17 _IQ17div_mathacl(_iq17 A, _iq17 B);
extern _iq16 _IQ16div_mathacl(_iq16 A, _iq16 B);
extern _iq15 _IQ15div_mathacl(_iq15 A, _iq15 B);
extern _iq14 _IQ14div_mathacl(_iq14 A, _iq14 B);
extern _iq13 _IQ13div_mathacl(_iq13 A, _iq13 B);
extern _iq12 _IQ12div_mathacl(_iq12 A, _iq12 B);
extern _iq11 _IQ11div_mathacl(_iq11 A, _iq11 B);
extern _iq10 _IQ10div_mathacl(_iq10 A, _iq10 B);
extern _iq9 _IQ9div_mathacl(_iq9 A, _iq9 B);
extern _iq8 _IQ8div_mathacl(_iq8 A, _iq8 B);
extern _iq7 _IQ7div_mathacl(_iq7 A, _iq7 B);
extern _iq6 _IQ6div_mathacl(_iq6 A, _iq6 B);
extern _iq5 _IQ5div_mathacl(_iq5 A, _iq5 B);
extern _iq4 _IQ4div_mathacl(_iq4 A, _iq4 B);
extern _iq3 _IQ3div_mathacl(_iq3 A, _iq3 B);
extern _iq2 _IQ2div_mathacl(_iq2 A, _iq2 B);
extern _iq1 _IQ1div_mathacl(_iq1 A, _iq1 B);
#endif /* DOXYGEN_SHOULD_SKIP_THIS */

/**
 * @brief Divides two global IQ format numbers.
 *
 * @param A               Global IQ format numerator to be divided.
 * @param B               Global IQ format denominator to divide by. 
 *
 * @return                Global IQ type result of division.
 */
#if GLOBAL_IQ == 30
#define _IQdiv_mathacl(A, B)            _IQ30div_mathacl(A, B)
#endif
#if GLOBAL_IQ == 29
#define _IQdiv_mathacl(A, B)            _IQ29div_mathacl(A, B)
#endif
#if GLOBAL_IQ == 28
#define _IQdiv_mathacl(A, B)            _IQ28div_mathacl(A, B)
#endif
#if GLOBAL_IQ == 27
#define _IQdiv_mathacl(A, B)            _IQ27div_mathacl(A, B)
#endif
#if GLOBAL_IQ == 26
#define _IQdiv_mathacl(A, B)            _IQ26div_mathacl(A, B)
#endif
#if GLOBAL_IQ == 25
#define _IQdiv_mathacl(A, B)            _IQ25div_mathacl(A, B)
#endif
#if GLOBAL_IQ == 24
#define _IQdiv_mathacl(A, B)            _IQ24div_mathacl(A, B)
#endif
#if GLOBAL_IQ == 23
#define _IQdiv_mathacl(A, B)            _IQ23div_mathacl(A, B)
#endif
#if GLOBAL_IQ == 22
#define _IQdiv_mathacl(A, B)            _IQ22div_mathacl(A, B)
#endif
#if GLOBAL_IQ == 21
#define _IQdiv_mathacl(A, B)            _IQ21div_mathacl(A, B)
#endif
#if GLOBAL_IQ == 20
#define _IQdiv_mathacl(A, B)            _IQ20div_mathacl(A, B)
#endif
#if GLOBAL_IQ == 19
#define _IQdiv_mathacl(A, B)            _IQ19div_mathacl(A, B)
#endif
#if GLOBAL_IQ == 18
#define _IQdiv_mathacl(A, B)            _IQ18div_mathacl(A, B)
#endif
#if GLOBAL_IQ == 17
#define _IQdiv_mathacl(A, B)            _IQ17div_mathacl(A, B)
#endif
#if GLOBAL_IQ == 16
#define _IQdiv_mathacl(A, B)            _IQ16div_mathacl(A, B)
#endif
#if GLOBAL_IQ == 15
#define _IQdiv_mathacl(A, B)            _IQ15div_mathacl(A, B)
#endif
#if GLOBAL_IQ == 14
#define _IQdiv_mathacl(A, B)            _IQ14div_mathacl(A, B)
#endif
#if GLOBAL_IQ == 13
#define _IQdiv_mathacl(A, B)            _IQ13div_mathacl(A, B)
#endif
#if GLOBAL_IQ == 12
#define _IQdiv_mathacl(A, B)            _IQ12div_mathacl(A, B)
#endif
#if GLOBAL_IQ == 11
#define _IQdiv_mathacl(A, B)            _IQ11div_mathacl(A, B)
#endif
#if GLOBAL_IQ == 10
#define _IQdiv_mathacl(A, B)            _IQ10div_mathacl(A, B)
#endif
#if GLOBAL_IQ == 9
#define _IQdiv_mathacl(A, B)            _IQ9div_mathacl(A, B)
#endif
#if GLOBAL_IQ == 8
#define _IQdiv_mathacl(A, B)            _IQ8div_mathacl(A, B)
#endif
#if GLOBAL_IQ == 7
#define _IQdiv_mathacl(A, B)            _IQ7div_mathacl(A, B)
#endif
#if GLOBAL_IQ == 6
#define _IQdiv_mathacl(A, B)            _IQ6div_mathacl(A, B)
#endif
#if GLOBAL_IQ == 5
#define _IQdiv_mathacl(A, B)            _IQ5div_mathacl(A, B)
#endif
#if GLOBAL_IQ == 4
#define _IQdiv_mathacl(A, B)            _IQ4div_mathacl(A, B)
#endif
#if GLOBAL_IQ == 3
#define _IQdiv_mathacl(A, B)            _IQ3div_mathacl(A, B)
#endif
#if GLOBAL_IQ == 2
#define _IQdiv_mathacl(A, B)            _IQ2div_mathacl(A, B)
#endif
#if GLOBAL_IQ == 1
#define _IQdiv_mathacl(A, B)            _IQ1div_mathacl(A, B)
#endif

//*****************************************************************************
//
// Computes the square root of an IQ number.
//
//*****************************************************************************
#ifndef DOXYGEN_SHOULD_SKIP_THIS
extern _iq30 _IQ30sqrt_mathacl(_iq30 A);
extern _iq29 _IQ29sqrt_mathacl(_iq29 A);
extern _iq28 _IQ28sqrt_mathacl(_iq28 A);
extern _iq27 _IQ27sqrt_mathacl(_iq27 A);
extern _iq26 _IQ26sqrt_mathacl(_iq26 A);
extern _iq25 _IQ25sqrt_mathacl(_iq25 A);
extern _iq24 _IQ24sqrt_mathacl(_iq24 A);
extern _iq23 _IQ23sqrt_mathacl(_iq23 A);
extern _iq22 _IQ22sqrt_mathacl(_iq22 A);
extern _iq21 _IQ21sqrt_mathacl(_iq21 A);
extern _iq20 _IQ20sqrt_mathacl(_iq20 A);
extern _iq19 _IQ19sqrt_mathacl(_iq19 A);
extern _iq18 _IQ18sqrt_mathacl(_iq18 A);
extern _iq17 _IQ17sqrt_mathacl(_iq17 A);
extern _iq16 _IQ16sqrt_mathacl(_iq16 A);
extern _iq15 _IQ15sqrt_mathacl(_iq15 A);
extern _iq14 _IQ14sqrt_mathacl(_iq14 A);
extern _iq13 _IQ13sqrt_mathacl(_iq13 A);
extern _iq12 _IQ12sqrt_mathacl(_iq12 A);
extern _iq11 _IQ11sqrt_mathacl(_iq11 A);
extern _iq10 _IQ10sqrt_mathacl(_iq10 A);
extern _iq9 _IQ9sqrt_mathacl(_iq9 A);
extern _iq8 _IQ8sqrt_mathacl(_iq8 A);
extern _iq7 _IQ7sqrt_mathacl(_iq7 A);
extern _iq6 _IQ6sqrt_mathacl(_iq6 A);
extern _iq5 _IQ5sqrt_mathacl(_iq5 A);
extern _iq4 _IQ4sqrt_mathacl(_iq4 A);
extern _iq3 _IQ3sqrt_mathacl(_iq3 A);
extern _iq2 _IQ2sqrt_mathacl(_iq2 A);
extern _iq1 _IQ1sqrt_mathacl(_iq1 A);
#endif /* DOXYGEN_SHOULD_SKIP_THIS */

/**
 * @brief Calculate square root of a global IQ format input.
 *
 * @param A               Global IQ format input.
 *
 * @return                Global IQ type result of the square root operation.
 */
#if GLOBAL_IQ == 30
#define _IQsqrt_mathacl(A)              _IQ30sqrt_mathacl(A)
#endif
#if GLOBAL_IQ == 29
#define _IQsqrt_mathacl(A)              _IQ29sqrt_mathacl(A)
#endif
#if GLOBAL_IQ == 28
#define _IQsqrt_mathacl(A)              _IQ28sqrt_mathacl(A)
#endif
#if GLOBAL_IQ == 27
#define _IQsqrt_mathacl(A)              _IQ27sqrt_mathacl(A)
#endif
#if GLOBAL_IQ == 26
#define _IQsqrt_mathacl(A)              _IQ26sqrt_mathacl(A)
#endif
#if GLOBAL_IQ == 25
#define _IQsqrt_mathacl(A)              _IQ25sqrt_mathacl(A)
#endif
#if GLOBAL_IQ == 24
#define _IQsqrt_mathacl(A)              _IQ24sqrt_mathacl(A)
#endif
#if GLOBAL_IQ == 23
#define _IQsqrt_mathacl(A)              _IQ23sqrt_mathacl(A)
#endif
#if GLOBAL_IQ == 22
#define _IQsqrt_mathacl(A)              _IQ22sqrt_mathacl(A)
#endif
#if GLOBAL_IQ == 21
#define _IQsqrt_mathacl(A)              _IQ21sqrt_mathacl(A)
#endif
#if GLOBAL_IQ == 20
#define _IQsqrt_mathacl(A)              _IQ20sqrt_mathacl(A)
#endif
#if GLOBAL_IQ == 19
#define _IQsqrt_mathacl(A)              _IQ19sqrt_mathacl(A)
#endif
#if GLOBAL_IQ == 18
#define _IQsqrt_mathacl(A)              _IQ18sqrt_mathacl(A)
#endif
#if GLOBAL_IQ == 17
#define _IQsqrt_mathacl(A)              _IQ17sqrt_mathacl(A)
#endif
#if GLOBAL_IQ == 16
#define _IQsqrt_mathacl(A)              _IQ16sqrt_mathacl(A)
#endif
#if GLOBAL_IQ == 15
#define _IQsqrt_mathacl(A)              _IQ15sqrt_mathacl(A)
#endif
#if GLOBAL_IQ == 14
#define _IQsqrt_mathacl(A)              _IQ14sqrt_mathacl(A)
#endif
#if GLOBAL_IQ == 13
#define _IQsqrt_mathacl(A)              _IQ13sqrt_mathacl(A)
#endif
#if GLOBAL_IQ == 12
#define _IQsqrt_mathacl(A)              _IQ12sqrt_mathacl(A)
#endif
#if GLOBAL_IQ == 11
#define _IQsqrt_mathacl(A)              _IQ11sqrt_mathacl(A)
#endif
#if GLOBAL_IQ == 10
#define _IQsqrt_mathacl(A)              _IQ10sqrt_mathacl(A)
#endif
#if GLOBAL_IQ == 9
#define _IQsqrt_mathacl(A)              _IQ9sqrt_mathacl(A)
#endif
#if GLOBAL_IQ == 8
#define _IQsqrt_mathacl(A)              _IQ8sqrt_mathacl(A)
#endif
#if GLOBAL_IQ == 7
#define _IQsqrt_mathacl(A)              _IQ7sqrt_mathacl(A)
#endif
#if GLOBAL_IQ == 6
#define _IQsqrt_mathacl(A)              _IQ6sqrt_mathacl(A)
#endif
#if GLOBAL_IQ == 5
#define _IQsqrt_mathacl(A)              _IQ5sqrt_mathacl(A)
#endif
#if GLOBAL_IQ == 4
#define _IQsqrt_mathacl(A)              _IQ4sqrt_mathacl(A)
#endif
#if GLOBAL_IQ == 3
#define _IQsqrt_mathacl(A)              _IQ3sqrt_mathacl(A)
#endif
#if GLOBAL_IQ == 2
#define _IQsqrt_mathacl(A)              _IQ2sqrt_mathacl(A)
#endif
#if GLOBAL_IQ == 1
#define _IQsqrt_mathacl(A)              _IQ1sqrt_mathacl(A)
#endif

//*****************************************************************************
//
// Mark the end of the C bindings section for C++ compilers.
//
//*****************************************************************************
#ifdef __cplusplus
}
#endif

#endif // __IQMATH_MATHACL_H__
