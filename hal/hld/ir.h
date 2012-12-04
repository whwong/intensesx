/**
 * @file ir.h
 * @brief Infra red receiver driver
 * Designed to cooperate with TSOP1138, not tested od other receivers
 * btu should work with most.
 * @author Daniel Szot
 */

#ifndef _HLD_IR_H_
#define _HLD_IR_H_

#include "hld.h"
#include "lib/common.h"

union hldIrFlags
{
    struct
    {
        UINT8 first :1;
        UINT8 edge :1;
        UINT8 timeout :1;
        UINT8 decoded :1;
    };
    UINT8 raw;
};

struct hldIrConfig
{
    UINT32 (*decode)(UINT32 pLastPulseTime, union hldIrFlags *pFlags, UINT32 pTicks);
};

struct hldIrDevice
{
    struct hldDevice head;
    struct hldIrConfig config;

    retcode (*attach)(struct hldIrConfig *pCfg);
    retcode (*open)(struct hldIrDevice *pIrDev);
    retcode (*close)(struct hldIrDevice *pIrDev);

    retcode (*read)(struct hldIrDevice *pIrDev, UINT32 *pLastCode);
};

#endif
