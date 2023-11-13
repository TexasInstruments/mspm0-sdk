/*!****************************************************************************
 *  @file       iqmath_rts.h
 *  @brief      Cutdown version of the RTS version of the IQmathLib.h.
 *
 *  <hr>
 ******************************************************************************/
#ifndef __IQMATH_RTS_H__
#define __IQMATH_RTS_H__

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
extern _iq30 _IQ30mpy_rts(_iq30 A, _iq30 B);
extern _iq29 _IQ29mpy_rts(_iq29 A, _iq29 B);
extern _iq28 _IQ28mpy_rts(_iq28 A, _iq28 B);
extern _iq27 _IQ27mpy_rts(_iq27 A, _iq27 B);
extern _iq26 _IQ26mpy_rts(_iq26 A, _iq26 B);
extern _iq25 _IQ25mpy_rts(_iq25 A, _iq25 B);
extern _iq24 _IQ24mpy_rts(_iq24 A, _iq24 B);
extern _iq23 _IQ23mpy_rts(_iq23 A, _iq23 B);
extern _iq22 _IQ22mpy_rts(_iq22 A, _iq22 B);
extern _iq21 _IQ21mpy_rts(_iq21 A, _iq21 B);
extern _iq20 _IQ20mpy_rts(_iq20 A, _iq20 B);
extern _iq19 _IQ19mpy_rts(_iq19 A, _iq19 B);
extern _iq18 _IQ18mpy_rts(_iq18 A, _iq18 B);
extern _iq17 _IQ17mpy_rts(_iq17 A, _iq17 B);
extern _iq16 _IQ16mpy_rts(_iq16 A, _iq16 B);
extern _iq15 _IQ15mpy_rts(_iq15 A, _iq15 B);
extern _iq14 _IQ14mpy_rts(_iq14 A, _iq14 B);
extern _iq13 _IQ13mpy_rts(_iq13 A, _iq13 B);
extern _iq12 _IQ12mpy_rts(_iq12 A, _iq12 B);
extern _iq11 _IQ11mpy_rts(_iq11 A, _iq11 B);
extern _iq10 _IQ10mpy_rts(_iq10 A, _iq10 B);
extern _iq9 _IQ9mpy_rts(_iq9 A, _iq9 B);
extern _iq8 _IQ8mpy_rts(_iq8 A, _iq8 B);
extern _iq7 _IQ7mpy_rts(_iq7 A, _iq7 B);
extern _iq6 _IQ6mpy_rts(_iq6 A, _iq6 B);
extern _iq5 _IQ5mpy_rts(_iq5 A, _iq5 B);
extern _iq4 _IQ4mpy_rts(_iq4 A, _iq4 B);
extern _iq3 _IQ3mpy_rts(_iq3 A, _iq3 B);
extern _iq2 _IQ2mpy_rts(_iq2 A, _iq2 B);
extern _iq1 _IQ1mpy_rts(_iq1 A, _iq1 B);

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
#define _IQmpy_rts(A, B)            _IQ30mpy_rts(A, B)
#endif
#if GLOBAL_IQ == 29
#define _IQmpy_rts(A, B)            _IQ29mpy_rts(A, B)
#endif
#if GLOBAL_IQ == 28
#define _IQmpy_rts(A, B)            _IQ28mpy_rts(A, B)
#endif
#if GLOBAL_IQ == 27
#define _IQmpy_rts(A, B)            _IQ27mpy_rts(A, B)
#endif
#if GLOBAL_IQ == 26
#define _IQmpy_rts(A, B)            _IQ26mpy_rts(A, B)
#endif
#if GLOBAL_IQ == 25
#define _IQmpy_rts(A, B)            _IQ25mpy_rts(A, B)
#endif
#if GLOBAL_IQ == 24
#define _IQmpy_rts(A, B)            _IQ24mpy_rts(A, B)
#endif
#if GLOBAL_IQ == 23
#define _IQmpy_rts(A, B)            _IQ23mpy_rts(A, B)
#endif
#if GLOBAL_IQ == 22
#define _IQmpy_rts(A, B)            _IQ22mpy_rts(A, B)
#endif
#if GLOBAL_IQ == 21
#define _IQmpy_rts(A, B)            _IQ21mpy_rts(A, B)
#endif
#if GLOBAL_IQ == 20
#define _IQmpy_rts(A, B)            _IQ20mpy_rts(A, B)
#endif
#if GLOBAL_IQ == 19
#define _IQmpy_rts(A, B)            _IQ19mpy_rts(A, B)
#endif
#if GLOBAL_IQ == 18
#define _IQmpy_rts(A, B)            _IQ18mpy_rts(A, B)
#endif
#if GLOBAL_IQ == 17
#define _IQmpy_rts(A, B)            _IQ17mpy_rts(A, B)
#endif
#if GLOBAL_IQ == 16
#define _IQmpy_rts(A, B)            _IQ16mpy_rts(A, B)
#endif
#if GLOBAL_IQ == 15
#define _IQmpy_rts(A, B)            _IQ15mpy_rts(A, B)
#endif
#if GLOBAL_IQ == 14
#define _IQmpy_rts(A, B)            _IQ14mpy_rts(A, B)
#endif
#if GLOBAL_IQ == 13
#define _IQmpy_rts(A, B)            _IQ13mpy_rts(A, B)
#endif
#if GLOBAL_IQ == 12
#define _IQmpy_rts(A, B)            _IQ12mpy_rts(A, B)
#endif
#if GLOBAL_IQ == 11
#define _IQmpy_rts(A, B)            _IQ11mpy_rts(A, B)
#endif
#if GLOBAL_IQ == 10
#define _IQmpy_rts(A, B)            _IQ10mpy_rts(A, B)
#endif
#if GLOBAL_IQ == 9
#define _IQmpy_rts(A, B)            _IQ9mpy_rts(A, B)
#endif
#if GLOBAL_IQ == 8
#define _IQmpy_rts(A, B)            _IQ8mpy_rts(A, B)
#endif
#if GLOBAL_IQ == 7
#define _IQmpy_rts(A, B)            _IQ7mpy_rts(A, B)
#endif
#if GLOBAL_IQ == 6
#define _IQmpy_rts(A, B)            _IQ6mpy_rts(A, B)
#endif
#if GLOBAL_IQ == 5
#define _IQmpy_rts(A, B)            _IQ5mpy_rts(A, B)
#endif
#if GLOBAL_IQ == 4
#define _IQmpy_rts(A, B)            _IQ4mpy_rts(A, B)
#endif
#if GLOBAL_IQ == 3
#define _IQmpy_rts(A, B)            _IQ3mpy_rts(A, B)
#endif
#if GLOBAL_IQ == 2
#define _IQmpy_rts(A, B)            _IQ2mpy_rts(A, B)
#endif
#if GLOBAL_IQ == 1
#define _IQmpy_rts(A, B)            _IQ1mpy_rts(A, B)
#endif

//*****************************************************************************
//
// Divides two IQ numbers.
//
//*****************************************************************************
#ifndef DOXYGEN_SHOULD_SKIP_THIS
extern _iq30 _IQ30div_rts(_iq30 A, _iq30 B);
extern _iq29 _IQ29div_rts(_iq29 A, _iq29 B);
extern _iq28 _IQ28div_rts(_iq28 A, _iq28 B);
extern _iq27 _IQ27div_rts(_iq27 A, _iq27 B);
extern _iq26 _IQ26div_rts(_iq26 A, _iq26 B);
extern _iq25 _IQ25div_rts(_iq25 A, _iq25 B);
extern _iq24 _IQ24div_rts(_iq24 A, _iq24 B);
extern _iq23 _IQ23div_rts(_iq23 A, _iq23 B);
extern _iq22 _IQ22div_rts(_iq22 A, _iq22 B);
extern _iq21 _IQ21div_rts(_iq21 A, _iq21 B);
extern _iq20 _IQ20div_rts(_iq20 A, _iq20 B);
extern _iq19 _IQ19div_rts(_iq19 A, _iq19 B);
extern _iq18 _IQ18div_rts(_iq18 A, _iq18 B);
extern _iq17 _IQ17div_rts(_iq17 A, _iq17 B);
extern _iq16 _IQ16div_rts(_iq16 A, _iq16 B);
extern _iq15 _IQ15div_rts(_iq15 A, _iq15 B);
extern _iq14 _IQ14div_rts(_iq14 A, _iq14 B);
extern _iq13 _IQ13div_rts(_iq13 A, _iq13 B);
extern _iq12 _IQ12div_rts(_iq12 A, _iq12 B);
extern _iq11 _IQ11div_rts(_iq11 A, _iq11 B);
extern _iq10 _IQ10div_rts(_iq10 A, _iq10 B);
extern _iq9 _IQ9div_rts(_iq9 A, _iq9 B);
extern _iq8 _IQ8div_rts(_iq8 A, _iq8 B);
extern _iq7 _IQ7div_rts(_iq7 A, _iq7 B);
extern _iq6 _IQ6div_rts(_iq6 A, _iq6 B);
extern _iq5 _IQ5div_rts(_iq5 A, _iq5 B);
extern _iq4 _IQ4div_rts(_iq4 A, _iq4 B);
extern _iq3 _IQ3div_rts(_iq3 A, _iq3 B);
extern _iq2 _IQ2div_rts(_iq2 A, _iq2 B);
extern _iq1 _IQ1div_rts(_iq1 A, _iq1 B);
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
#define _IQdiv_rts(A, B)            _IQ30div_rts(A, B)
#endif
#if GLOBAL_IQ == 29
#define _IQdiv_rts(A, B)            _IQ29div_rts(A, B)
#endif
#if GLOBAL_IQ == 28
#define _IQdiv_rts(A, B)            _IQ28div_rts(A, B)
#endif
#if GLOBAL_IQ == 27
#define _IQdiv_rts(A, B)            _IQ27div_rts(A, B)
#endif
#if GLOBAL_IQ == 26
#define _IQdiv_rts(A, B)            _IQ26div_rts(A, B)
#endif
#if GLOBAL_IQ == 25
#define _IQdiv_rts(A, B)            _IQ25div_rts(A, B)
#endif
#if GLOBAL_IQ == 24
#define _IQdiv_rts(A, B)            _IQ24div_rts(A, B)
#endif
#if GLOBAL_IQ == 23
#define _IQdiv_rts(A, B)            _IQ23div_rts(A, B)
#endif
#if GLOBAL_IQ == 22
#define _IQdiv_rts(A, B)            _IQ22div_rts(A, B)
#endif
#if GLOBAL_IQ == 21
#define _IQdiv_rts(A, B)            _IQ21div_rts(A, B)
#endif
#if GLOBAL_IQ == 20
#define _IQdiv_rts(A, B)            _IQ20div_rts(A, B)
#endif
#if GLOBAL_IQ == 19
#define _IQdiv_rts(A, B)            _IQ19div_rts(A, B)
#endif
#if GLOBAL_IQ == 18
#define _IQdiv_rts(A, B)            _IQ18div_rts(A, B)
#endif
#if GLOBAL_IQ == 17
#define _IQdiv_rts(A, B)            _IQ17div_rts(A, B)
#endif
#if GLOBAL_IQ == 16
#define _IQdiv_rts(A, B)            _IQ16div_rts(A, B)
#endif
#if GLOBAL_IQ == 15
#define _IQdiv_rts(A, B)            _IQ15div_rts(A, B)
#endif
#if GLOBAL_IQ == 14
#define _IQdiv_rts(A, B)            _IQ14div_rts(A, B)
#endif
#if GLOBAL_IQ == 13
#define _IQdiv_rts(A, B)            _IQ13div_rts(A, B)
#endif
#if GLOBAL_IQ == 12
#define _IQdiv_rts(A, B)            _IQ12div_rts(A, B)
#endif
#if GLOBAL_IQ == 11
#define _IQdiv_rts(A, B)            _IQ11div_rts(A, B)
#endif
#if GLOBAL_IQ == 10
#define _IQdiv_rts(A, B)            _IQ10div_rts(A, B)
#endif
#if GLOBAL_IQ == 9
#define _IQdiv_rts(A, B)            _IQ9div_rts(A, B)
#endif
#if GLOBAL_IQ == 8
#define _IQdiv_rts(A, B)            _IQ8div_rts(A, B)
#endif
#if GLOBAL_IQ == 7
#define _IQdiv_rts(A, B)            _IQ7div_rts(A, B)
#endif
#if GLOBAL_IQ == 6
#define _IQdiv_rts(A, B)            _IQ6div_rts(A, B)
#endif
#if GLOBAL_IQ == 5
#define _IQdiv_rts(A, B)            _IQ5div_rts(A, B)
#endif
#if GLOBAL_IQ == 4
#define _IQdiv_rts(A, B)            _IQ4div_rts(A, B)
#endif
#if GLOBAL_IQ == 3
#define _IQdiv_rts(A, B)            _IQ3div_rts(A, B)
#endif
#if GLOBAL_IQ == 2
#define _IQdiv_rts(A, B)            _IQ2div_rts(A, B)
#endif
#if GLOBAL_IQ == 1
#define _IQdiv_rts(A, B)            _IQ1div_rts(A, B)
#endif


//*****************************************************************************
//
// Multiplies two IQ numbers in the specified iQ formats, returning the result
// in another IQ format.
//
//*****************************************************************************
#ifndef DOXYGEN_SHOULD_SKIP_THIS
extern int32_t _IQ30mpyIQX_rts(int32_t A, int n1, int32_t B, int n2);
extern int32_t _IQ29mpyIQX_rts(int32_t A, int n1, int32_t B, int n2);
extern int32_t _IQ28mpyIQX_rts(int32_t A, int n1, int32_t B, int n2);
extern int32_t _IQ27mpyIQX_rts(int32_t A, int n1, int32_t B, int n2);
extern int32_t _IQ26mpyIQX_rts(int32_t A, int n1, int32_t B, int n2);
extern int32_t _IQ25mpyIQX_rts(int32_t A, int n1, int32_t B, int n2);
extern int32_t _IQ24mpyIQX_rts(int32_t A, int n1, int32_t B, int n2);
extern int32_t _IQ23mpyIQX_rts(int32_t A, int n1, int32_t B, int n2);
extern int32_t _IQ22mpyIQX_rts(int32_t A, int n1, int32_t B, int n2);
extern int32_t _IQ21mpyIQX_rts(int32_t A, int n1, int32_t B, int n2);
extern int32_t _IQ20mpyIQX_rts(int32_t A, int n1, int32_t B, int n2);
extern int32_t _IQ19mpyIQX_rts(int32_t A, int n1, int32_t B, int n2);
extern int32_t _IQ18mpyIQX_rts(int32_t A, int n1, int32_t B, int n2);
extern int32_t _IQ17mpyIQX_rts(int32_t A, int n1, int32_t B, int n2);
extern int32_t _IQ16mpyIQX_rts(int32_t A, int n1, int32_t B, int n2);
extern int32_t _IQ15mpyIQX_rts(int32_t A, int n1, int32_t B, int n2);
extern int32_t _IQ14mpyIQX_rts(int32_t A, int n1, int32_t B, int n2);
extern int32_t _IQ13mpyIQX_rts(int32_t A, int n1, int32_t B, int n2);
extern int32_t _IQ12mpyIQX_rts(int32_t A, int n1, int32_t B, int n2);
extern int32_t _IQ11mpyIQX_rts(int32_t A, int n1, int32_t B, int n2);
extern int32_t _IQ10mpyIQX_rts(int32_t A, int n1, int32_t B, int n2);
extern int32_t _IQ9mpyIQX_rts(int32_t A, int n1, int32_t B, int n2);
extern int32_t _IQ8mpyIQX_rts(int32_t A, int n1, int32_t B, int n2);
extern int32_t _IQ7mpyIQX_rts(int32_t A, int n1, int32_t B, int n2);
extern int32_t _IQ6mpyIQX_rts(int32_t A, int n1, int32_t B, int n2);
extern int32_t _IQ5mpyIQX_rts(int32_t A, int n1, int32_t B, int n2);
extern int32_t _IQ4mpyIQX_rts(int32_t A, int n1, int32_t B, int n2);
extern int32_t _IQ3mpyIQX_rts(int32_t A, int n1, int32_t B, int n2);
extern int32_t _IQ2mpyIQX_rts(int32_t A, int n1, int32_t B, int n2);
extern int32_t _IQ1mpyIQX_rts(int32_t A, int n1, int32_t B, int n2);
#endif /* DOXYGEN_SHOULD_SKIP_THIS */

/**
 * @brief Multiply two IQ numbers in different IQ formats, 
 * returning the product in global IQ format.
 *
 * @param A               IQN1 format input to be multiplied.
 * @param n1              IQ format for first value.
 * @param B               IQN2 format input to be multiplied.
 * @param n2              IQ format for second value.
 * 
 *
 * @return                Global IQ format result of the multiplication.
 */
#if GLOBAL_IQ == 30
#define _IQmpyIQX_rts(A, n1, B, n2)              _IQ30mpyIQX_rts(A, n1, B, n2)
#endif
#if GLOBAL_IQ == 29
#define _IQmpyIQX_rts(A, n1, B, n2)               _IQ29mpyIQX_rts(A, n1, B, n2)
#endif
#if GLOBAL_IQ == 28
#define _IQmpyIQX_rts(A, n1, B, n2)              _IQ28mpyIQX_rts(A, n1, B, n2)
#endif
#if GLOBAL_IQ == 27
#define _IQmpyIQX_rts(A, n1, B, n2)              _IQ27mpyIQX_rts(A, n1, B, n2)
#endif
#if GLOBAL_IQ == 26
#define _IQmpyIQX_rts(A, n1, B, n2)              _IQ26mpyIQX_rts(A, n1, B, n2)
#endif
#if GLOBAL_IQ == 25
#define _IQmpyIQX_rts(A, n1, B, n2)              _IQ25mpyIQX_rts(A, n1, B, n2)
#endif
#if GLOBAL_IQ == 24
#define _IQmpyIQX_rts(A, n1, B, n2)              _IQ24mpyIQX_rts(A, n1, B, n2)
#endif
#if GLOBAL_IQ == 23
#define _IQmpyIQX_rts(A, n1, B, n2)              _IQ23mpyIQX_rts(A, n1, B, n2)
#endif
#if GLOBAL_IQ == 22
#define _IQmpyIQX_rts(A, n1, B, n2)              _IQ22mpyIQX_rts(A, n1, B, n2)
#endif
#if GLOBAL_IQ == 21
#define _IQmpyIQX_rts(A, n1, B, n2)              _IQ21mpyIQX_rts(A, n1, B, n2)
#endif
#if GLOBAL_IQ == 20
#define _IQmpyIQX_rts(A, n1, B, n2)              _IQ20mpyIQX_rts(A, n1, B, n2)
#endif
#if GLOBAL_IQ == 19
#define _IQmpyIQX_rts(A, n1, B, n2)              _IQ19mpyIQX_rts(A, n1, B, n2)
#endif
#if GLOBAL_IQ == 18
#define _IQmpyIQX_rts(A, n1, B, n2)              _IQ18mpyIQX_rts(A, n1, B, n2)
#endif
#if GLOBAL_IQ == 17
#define _IQmpyIQX_rts(A, n1, B, n2)              _IQ17mpyIQX_rts(A, n1, B, n2)
#endif
#if GLOBAL_IQ == 16
#define _IQmpyIQX_rts(A, n1, B, n2)              _IQ16mpyIQX_rts(A, n1, B, n2)
#endif
#if GLOBAL_IQ == 15
#define _IQmpyIQX_rts(A, n1, B, n2)              _IQ15mpyIQX_rts(A, n1, B, n2)
#endif
#if GLOBAL_IQ == 14
#define _IQmpyIQX_rts(A, n1, B, n2)              _IQ14mpyIQX_rts(A, n1, B, n2)
#endif
#if GLOBAL_IQ == 13
#define _IQmpyIQX_rts(A, n1, B, n2)              _IQ13mpyIQX_rts(A, n1, B, n2)
#endif
#if GLOBAL_IQ == 12
#define _IQmpyIQX_rts(A, n1, B, n2)              _IQ12mpyIQX_rts(A, n1, B, n2)
#endif
#if GLOBAL_IQ == 11
#define _IQmpyIQX_rts(A, n1, B, n2)              _IQ11mpyIQX_rts(A, n1, B, n2)
#endif
#if GLOBAL_IQ == 10
#define _IQmpyIQX_rts(A, n1, B, n2)              _IQ10mpyIQX_rts(A, n1, B, n2)
#endif
#if GLOBAL_IQ == 9
#define _IQmpyIQX_rts(A, n1, B, n2)              _IQ9mpyIQX_rts(A, n1, B, n2)
#endif
#if GLOBAL_IQ == 8
#define _IQmpyIQX_rts(A, n1, B, n2)              _IQ8mpyIQX_rts(A, n1, B, n2)
#endif
#if GLOBAL_IQ == 7
#define _IQmpyIQX_rts(A, n1, B, n2)              _IQ7mpyIQX_rts(A, n1, B, n2)
#endif
#if GLOBAL_IQ == 6
#define _IQmpyIQX_rts(A, n1, B, n2)              _IQ6mpyIQX_rts(A, n1, B, n2)
#endif
#if GLOBAL_IQ == 5
#define _IQmpyIQX_rts(A, n1, B, n2)              _IQ5mpyIQX_rts(A, n1, B, n2)
#endif
#if GLOBAL_IQ == 4
#define _IQmpyIQX_rts(A, n1, B, n2)              _IQ4mpyIQX_rts(A, n1, B, n2)
#endif
#if GLOBAL_IQ == 3
#define _IQmpyIQX_rts(A, n1, B, n2)              _IQ3mpyIQX_rts(A, n1, B, n2)
#endif
#if GLOBAL_IQ == 2
#define _IQmpyIQX_rts(A, n1, B, n2)              _IQ2mpyIQX_rts(A, n1, B, n2)
#endif
#if GLOBAL_IQ == 1
#define _IQmpyIQX_rts(A, n1, B, n2)              _IQ1mpyIQX_rts(A, n1, B, n2)
#endif

#ifndef DOXYGEN_SHOULD_SKIP_THIS
//*****************************************************************************
//
// Multiplies an IQ number by an integer.
//
//*****************************************************************************
#define _IQ30mpyI32_rts(A, B)       ((A) * (B))
#define _IQ29mpyI32_rts(A, B)       ((A) * (B))
#define _IQ28mpyI32_rts(A, B)       ((A) * (B))
#define _IQ27mpyI32_rts(A, B)       ((A) * (B))
#define _IQ26mpyI32_rts(A, B)       ((A) * (B))
#define _IQ25mpyI32_rts(A, B)       ((A) * (B))
#define _IQ24mpyI32_rts(A, B)       ((A) * (B))
#define _IQ23mpyI32_rts(A, B)       ((A) * (B))
#define _IQ22mpyI32_rts(A, B)       ((A) * (B))
#define _IQ21mpyI32_rts(A, B)       ((A) * (B))
#define _IQ20mpyI32_rts(A, B)       ((A) * (B))
#define _IQ19mpyI32_rts(A, B)       ((A) * (B))
#define _IQ18mpyI32_rts(A, B)       ((A) * (B))
#define _IQ17mpyI32_rts(A, B)       ((A) * (B))
#define _IQ16mpyI32_rts(A, B)       ((A) * (B))
#define _IQ15mpyI32_rts(A, B)       ((A) * (B))
#define _IQ14mpyI32_rts(A, B)       ((A) * (B))
#define _IQ13mpyI32_rts(A, B)       ((A) * (B))
#define _IQ12mpyI32_rts(A, B)       ((A) * (B))
#define _IQ11mpyI32_rts(A, B)       ((A) * (B))
#define _IQ10mpyI32_rts(A, B)       ((A) * (B))
#define _IQ9mpyI32_rts(A, B)        ((A) * (B))
#define _IQ8mpyI32_rts(A, B)        ((A) * (B))
#define _IQ7mpyI32_rts(A, B)        ((A) * (B))
#define _IQ6mpyI32_rts(A, B)        ((A) * (B))
#define _IQ5mpyI32_rts(A, B)        ((A) * (B))
#define _IQ4mpyI32_rts(A, B)        ((A) * (B))
#define _IQ3mpyI32_rts(A, B)        ((A) * (B))
#define _IQ2mpyI32_rts(A, B)        ((A) * (B))
#define _IQ1mpyI32_rts(A, B)        ((A) * (B))
#define _IQmpyI32_rts(A, B)         ((A) * (B))
#endif /* DOXYGEN_SHOULD_SKIP_THIS */

//*****************************************************************************
//
// Computes the absolute value of an IQ number.
//
//*****************************************************************************
/**
 * @brief Computes the absolute value of an IQ30 number.
 *
 * @param A               IQ30 type input.
 * 
 * @return                IQ30 type absolute value of input.
 */
#define _IQ30abs_rts(A)             (((A) < 0) ? - (A) : (A))
/**
 * @brief Computes the absolute value of an IQ29 number.
 *
 * @param A               IQ29 type input.
 * 
 * @return                IQ29 type absolute value of input.
 */
#define _IQ29abs_rts(A)             (((A) < 0) ? - (A) : (A))
/**
 * @brief Computes the absolute value of an IQ28 number.
 *
 * @param A               IQ28 type input.
 * 
 * @return                IQ28 type absolute value of input.
 */
#define _IQ28abs_rts(A)             (((A) < 0) ? - (A) : (A))
/**
 * @brief Computes the absolute value of an IQ27 number.
 *
 * @param A               IQ27 type input.
 * 
 * @return                IQ27 type absolute value of input.
 */
#define _IQ27abs_rts(A)             (((A) < 0) ? - (A) : (A))
/**
 * @brief Computes the absolute value of an IQ26 number.
 *
 * @param A               IQ26 type input.
 * 
 * @return                IQ26 type absolute value of input.
 */
#define _IQ26abs_rts(A)             (((A) < 0) ? - (A) : (A))
/**
 * @brief Computes the absolute value of an IQ25 number.
 *
 * @param A               IQ25 type input.
 * 
 * @return                IQ25 type absolute value of input.
 */
#define _IQ25abs_rts(A)             (((A) < 0) ? - (A) : (A))
/**
 * @brief Computes the absolute value of an IQ24 number.
 *
 * @param A               IQ24 type input.
 * 
 * @return                IQ24 type absolute value of input.
 */
#define _IQ24abs_rts(A)             (((A) < 0) ? - (A) : (A))
/**
 * @brief Computes the absolute value of an IQ23 number.
 *
 * @param A               IQ23 type input.
 * 
 * @return                IQ23 type absolute value of input.
 */
#define _IQ23abs_rts(A)             (((A) < 0) ? - (A) : (A))
/**
 * @brief Computes the absolute value of an IQ22 number.
 *
 * @param A               IQ22 type input.
 * 
 * @return                IQ22 type absolute value of input.
 */
#define _IQ22abs_rts(A)             (((A) < 0) ? - (A) : (A))
/**
 * @brief Computes the absolute value of an IQ21 number.
 *
 * @param A               IQ21 type input.
 * 
 * @return                IQ21 type absolute value of input.
 */
#define _IQ21abs_rts(A)             (((A) < 0) ? - (A) : (A))
/**
 * @brief Computes the absolute value of an IQ20 number.
 *
 * @param A               IQ20 type input.
 * 
 * @return                IQ20 type absolute value of input.
 */
#define _IQ20abs_rts(A)             (((A) < 0) ? - (A) : (A))
/**
 * @brief Computes the absolute value of an IQ19 number.
 *
 * @param A               IQ19 type input.
 * 
 * @return                IQ19 type absolute value of input.
 */
#define _IQ19abs_rts(A)             (((A) < 0) ? - (A) : (A))
/**
 * @brief Computes the absolute value of an IQ18 number.
 *
 * @param A               IQ18 type input.
 * 
 * @return                IQ18 type absolute value of input.
 */
#define _IQ18abs_rts(A)             (((A) < 0) ? - (A) : (A))
/**
 * @brief Computes the absolute value of an IQ17 number.
 *
 * @param A               IQ17 type input.
 * 
 * @return                IQ17 type absolute value of input.
 */
#define _IQ17abs_rts(A)             (((A) < 0) ? - (A) : (A))
/**
 * @brief Computes the absolute value of an IQ16 number.
 *
 * @param A               IQ16 type input.
 * 
 * @return                IQ16 type absolute value of input.
 */
#define _IQ16abs_rts(A)             (((A) < 0) ? - (A) : (A))
/**
 * @brief Computes the absolute value of an IQ15 number.
 *
 * @param A               IQ15 type input.
 * 
 * @return                IQ15 type absolute value of input.
 */
#define _IQ15abs_rts(A)             (((A) < 0) ? - (A) : (A))
/**
 * @brief Computes the absolute value of an IQ14 number.
 *
 * @param A               IQ14 type input.
 * 
 * @return                IQ14 type absolute value of input.
 */
#define _IQ14abs_rts(A)             (((A) < 0) ? - (A) : (A))
/**
 * @brief Computes the absolute value of an IQ13 number.
 *
 * @param A               IQ13 type input.
 * 
 * @return                IQ13 type absolute value of input.
 */
#define _IQ13abs_rts(A)             (((A) < 0) ? - (A) : (A))
/**
 * @brief Computes the absolute value of an IQ12 number.
 *
 * @param A               IQ12 type input.
 * 
 * @return                IQ12 type absolute value of input.
 */
#define _IQ12abs_rts(A)             (((A) < 0) ? - (A) : (A))
/**
 * @brief Computes the absolute value of an IQ11 number.
 *
 * @param A               IQ11 type input.
 * 
 * @return                IQ11 type absolute value of input.
 */
#define _IQ11abs_rts(A)             (((A) < 0) ? - (A) : (A))
/**
 * @brief Computes the absolute value of an IQ10 number.
 *
 * @param A               IQ10 type input.
 * 
 * @return                IQ10 type absolute value of input.
 */
#define _IQ10abs_rts(A)             (((A) < 0) ? - (A) : (A))
/**
 * @brief Computes the absolute value of an IQ9 number.
 *
 * @param A               IQ9 type input.
 * 
 * @return                IQ9 type absolute value of input.
 */
#define _IQ9abs_rts(A)              (((A) < 0) ? - (A) : (A))
/**
 * @brief Computes the absolute value of an IQ8 number.
 *
 * @param A               IQ8 type input.
 * 
 * @return                IQ8 type absolute value of input.
 */
#define _IQ8abs_rts(A)              (((A) < 0) ? - (A) : (A))
/**
 * @brief Computes the absolute value of an IQ7 number.
 *
 * @param A               IQ7 type input.
 * 
 * @return                IQ7 type absolute value of input.
 */
#define _IQ7abs_rts(A)              (((A) < 0) ? - (A) : (A))
/**
 * @brief Computes the absolute value of an IQ6 number.
 *
 * @param A               IQ6 type input.
 * 
 * @return                IQ6 type absolute value of input.
 */
#define _IQ6abs_rts(A)              (((A) < 0) ? - (A) : (A))
/**
 * @brief Computes the absolute value of an IQ5 number.
 *
 * @param A               IQ5 type input.
 * 
 * @return                IQ5 type absolute value of input.
 */
#define _IQ5abs_rts(A)              (((A) < 0) ? - (A) : (A))
/**
 * @brief Computes the absolute value of an IQ4 number.
 *
 * @param A               IQ4 type input.
 * 
 * @return                IQ4 type absolute value of input.
 */
#define _IQ4abs_rts(A)              (((A) < 0) ? - (A) : (A))
/**
 * @brief Computes the absolute value of an IQ3 number.
 *
 * @param A               IQ3 type input.
 * 
 * @return                IQ3 type absolute value of input.
 */
#define _IQ3abs_rts(A)              (((A) < 0) ? - (A) : (A))
/**
 * @brief Computes the absolute value of an IQ2 number.
 *
 * @param A               IQ2 type input.
 * 
 * @return                IQ2 type absolute value of input.
 */
#define _IQ2abs_rts(A)              (((A) < 0) ? - (A) : (A))
/**
 * @brief Computes the absolute value of an IQ1 number.
 *
 * @param A               IQ1 type input.
 * 
 * @return                IQ1 type absolute value of input.
 */
#define _IQ1abs_rts(A)              (((A) < 0) ? - (A) : (A))
/**
 * @brief Computes the absolute value of an global IQ format number.
 *
 * @param A               Global IQ format input.
 * 
 * @return                GlobalIQ format absolute value of input.
 */
#define _IQabs_rts(A)               (((A) < 0) ? - (A) : (A))

//*****************************************************************************
//
// Mark the end of the C bindings section for C++ compilers.
//
//*****************************************************************************
#ifdef __cplusplus
}
#endif

#endif // __IQMATH_RTS_H__
