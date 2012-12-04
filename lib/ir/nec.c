/**
 * @file nec.c
 * @brief IR NEC decoder
 * Based on ir_decoder by Frederic Vecoven (www.vecoven.com)
 * @author Daniel Szot
 */

#include "lib/ir/nec.h"

UINT32 irNecDecode(UINT32 pPulseTime, union hldIrFlags *pFlags, UINT32 pTicks)
{
    static struct necDecoderFlags nec;
    static UINT32 lastCode = 0x0;
    static UINT32 repeatTick = 0;
    
    if (pFlags->first)
    {
        nec.flags.raw = 0;
        nec.flags.first = 1;
        nec.bitcount = 0;
        nec.data = 0;
        return 0;
    }

    if (nec.flags.invalid)
        return 0;

    if (nec.flags.last)
    {
        if (pFlags->timeout || pPulseTime > NEC_TIMEOUT)
        {
            // valid NEC !
            pFlags->decoded = 1;
        }
        else
        {
            nec.flags.invalid = 1;
        }
        return 0;
    }

    if (nec.flags.first)
    {
        if (pPulseTime < NEC_FIRST_MIN || pPulseTime > NEC_FIRST_MAX)
        {
            nec.flags.invalid = 1;
        }
        else
        {
            nec.flags.first = 0;
            nec.flags.second = 1;
        }
        return 0;
    }

    if (nec.flags.second)
    {
        if (pPulseTime > NEC_REPEAT_MIN && pPulseTime < NEC_REPEAT_MAX)
        {
            nec.flags.second = 0;
            nec.flags.repeat = 1;
            nec.flags.pulse = 1;
        }
        else if (pPulseTime < NEC_SECOND_MIN || pPulseTime > NEC_SECOND_MAX)
        {
            nec.flags.invalid = 1;
        }
        else
        {
            nec.flags.second = 0;
            nec.flags.pulse = 1;
        }
        return 0;
    }

    if (pPulseTime < NEC_SHORT_MIN)
    {
        nec.flags.invalid = 1;
        return 0;
    }

    if (nec.flags.pulse)
    {
        if (pPulseTime > NEC_SHORT_MAX)
        {
            nec.flags.invalid = 1;
            return 0;
        }
        nec.flags.pulse = 0;

        if (nec.flags.repeat)
        {

            if (repeatTick+NEC_REPEAT_TIMEOUT > pTicks)
            {
                repeatTick = pTicks;
                return lastCode;
            }
        }

        if (nec.bitcount == 32)
        {
            lastCode = nec.data;
            repeatTick = pTicks;
            nec.flags.last = 1;
            return nec.data;
        }
        return 0;
    }

    nec.flags.pulse = 1;

    if (pPulseTime > NEC_LONG_MAX)
    {
        nec.flags.invalid = 1;
        return 0;
    }

    nec.data = nec.data << 1;
    if (pPulseTime >= NEC_LONG_MIN)
    {
        nec.data |= 0x00000001;
    }
    nec.bitcount++;

    return 0;
}
