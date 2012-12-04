/**
 * @file char.h
 * @brief Char driver abstraction layer
 * @author Daniel Szot
 */

#ifndef _HLD_CHAR_H_
#define _HLD_CHAR_H_

#include "hld.h"
#include "lib/common.h"

struct hldCharDevice
{
    struct hldDevice head;

    retcode (*write)(struct hldCharDevice *pCharDev, UINT8 pByte, UINT32 pTimeout);
    retcode (*read)(struct hldCharDevice *pCharDev, UINT8 *pByte, UINT32 pTimeout);
};

#endif
