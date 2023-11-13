/*
 * Copyright (c) 2023, Texas Instruments Incorporated
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * *  Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * *  Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * *  Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
#include "angle_math.h"

/** @brief Resolution of the angle from sine table */
#define ANGLE_MATH_BIT_RES      (8)

/** @brief Number of steps in sine table */
#define ANGLE_STEPS             (1 << ANGLE_MATH_BIT_RES)

/** @brief Mask for steps */
#define ANGLE_STEPS_MASK        (ANGLE_STEPS - 1)

/** @brief Max value of the sine table */
#define ANGLE_STEPS_MAX         (ANGLE_STEPS + 1)

/** @brief Positive magnitude mask for angle */
#define ANGLE_STEPS_MAG_MASK    (ANGLE_STEPS << 1)

/** @brief Convert angle to 10 bit resolution */
#define ANGLE_SHIFT             (32 - 2 - ANGLE_MATH_BIT_RES)

/** @brief Sine lookup table in 24 fixed point*/
int32_t sine_table[ANGLE_STEPS_MAX] =
{
    0,
    102944,
    205882,
    308814,
    411734,
    514638,
    617522,
    720384,
    823218,
    926022,
    1028792,
    1131522,
    1234208,
    1336850,
    1439440,
    1541976,
    1644454,
    1746870,
    1849222,
    1951502,
    2053710,
    2155840,
    2257890,
    2359854,
    2461728,
    2563512,
    2665198,
    2766782,
    2868264,
    2969638,
    3070900,
    3172046,
    3273072,
    3373976,
    3474752,
    3575398,
    3675908,
    3776280,
    3876512,
    3976596,
    4076530,
    4176312,
    4275936,
    4375398,
    4474698,
    4573826,
    4672784,
    4771566,
    4870168,
    4968586,
    5066818,
    5164860,
    5262706,
    5360354,
    5457800,
    5555042,
    5652074,
    5748892,
    5845494,
    5941878,
    6038036,
    6133968,
    6229668,
    6325134,
    6420362,
    6515348,
    6610088,
    6704580,
    6798820,
    6892804,
    6986528,
    7079988,
    7173184,
    7266108,
    7358758,
    7451132,
    7543226,
    7635034,
    7726556,
    7817788,
    7908724,
    7999362,
    8089700,
    8179734,
    8269458,
    8358872,
    8447972,
    8536752,
    8625212,
    8713346,
    8801154,
    8888628,
    8975770,
    9062572,
    9149034,
    9235152,
    9320920,
    9406340,
    9491404,
    9576110,
    9660458,
    9744440,
    9828056,
    9911302,
    9994174,
    10076672,
    10158788,
    10240522,
    10321872,
    10402832,
    10483402,
    10563576,
    10643352,
    10722728,
    10801700,
    10880264,
    10958420,
    11036164,
    11113492,
    11190400,
    11266888,
    11342952,
    11418588,
    11493796,
    11568570,
    11642908,
    11716808,
    11790266,
    11863282,
    11935850,
    12007970,
    12079636,
    12150848,
    12221602,
    12291898,
    12361730,
    12431096,
    12499994,
    12568422,
    12636376,
    12703854,
    12770856,
    12837374,
    12903412,
    12968962,
    13034024,
    13098596,
    13162674,
    13226256,
    13289342,
    13351926,
    13414008,
    13475584,
    13536654,
    13597214,
    13657262,
    13716796,
    13775812,
    13834312,
    13892288,
    13949744,
    14006674,
    14063076,
    14118948,
    14174290,
    14229096,
    14283368,
    14337102,
    14390296,
    14442948,
    14495058,
    14546620,
    14597636,
    14648102,
    14698016,
    14747376,
    14796182,
    14844430,
    14892120,
    14939248,
    14985816,
    15031818,
    15077254,
    15122122,
    15166422,
    15210150,
    15253306,
    15295888,
    15337892,
    15379320,
    15420170,
    15460438,
    15500124,
    15539228,
    15577746,
    15615676,
    15653020,
    15689774,
    15725938,
    15761508,
    15796486,
    15830870,
    15864656,
    15897846,
    15930438,
    15962428,
    15993820,
    16024608,
    16054792,
    16084374,
    16113348,
    16141716,
    16169478,
    16196630,
    16223172,
    16249102,
    16274422,
    16299128,
    16323222,
    16346700,
    16369564,
    16391810,
    16413440,
    16434452,
    16454844,
    16474618,
    16493772,
    16512304,
    16530214,
    16547502,
    16564168,
    16580208,
    16595626,
    16610418,
    16624586,
    16638126,
    16651042,
    16663330,
    16674990,
    16686022,
    16696428,
    16706204,
    16715350,
    16723868,
    16731756,
    16739014,
    16745642,
    16751638,
    16757006,
    16761740,
    16765846,
    16769318,
    16772162,
    16774372,
    16775950,
    16776898,
    16777214
};

int32_t ANGLE_getSine( uint32_t phase )
{
    int32_t res;
    uint32_t step;
    phase >>= ANGLE_SHIFT;
    step = phase & (ANGLE_STEPS_MASK);
    if ((phase & ANGLE_STEPS))
    {
        step = ANGLE_STEPS - step;
    }
    res = sine_table[step];
    if (phase & ANGLE_STEPS_MAG_MASK)
    {
        res = -res;
    }
    return res;
}
