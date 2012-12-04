/**
 * @file nec.h
 * @brief IR NEC decoder
 * Based on ir_decoder by Frederic Vecoven (www.vecoven.com)
 * @author Daniel Szot
 */

#ifndef _HLD_NEC_DECODER_H_
#define _HLD_NEC_DECODER_H_

#include "hal/hld/ir.h"

// High precision time defs in timer iteration unit
// 9 ms
#define	NEC_FIRST_MIN   43520
#define	NEC_FIRST_MAX   46080
// 4.5 ms
#define	NEC_SECOND_MIN  20480
#define	NEC_SECOND_MAX  23040
// 2.25 ms
#define NEC_REPEAT_MIN  10250
#define NEC_REPEAT_MAX  12250
// 562.5 us
#define	NEC_SHORT_MIN   1920
#define	NEC_SHORT_MAX   3520
// 1.687 5ms
#define	NEC_LONG_MIN    7040
#define	NEC_LONG_MAX    8640

#define	NEC_TIMEOUT     64000

// Low precision long time defs in ms
#define NEC_REPEAT_TIMEOUT 130

struct necDecoderFlags
{
    union
    {
        struct
        {
            unsigned invalid :1;
            unsigned last :1;
            unsigned first :1;
            unsigned second :1;
            unsigned repeat : 1;
            unsigned pulse :1;
        };
        uint8_t raw;
    } flags;
    uint8_t bitcount;
    uint32_t data;
};

UINT32 irNecDecode(UINT32 pPulseTime, union hldIrFlags *pFlags, UINT32 pTicks);

#endif
