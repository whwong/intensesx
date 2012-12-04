/**
 * @file disk.h
 * @brief Disk driver abstraction for FatFS
 * @author Daniel Szot
 */

#ifndef _HLD_DISK_H_
#define _HLD_DISK_H_

#include "hld.h"
#include "lib/common.h"

typedef UINT8 diskstatus;

/* Disk Status Bits (DSTATUS) */
#define STA_NOINIT		0x01	/* Drive not initialized */
#define STA_NODISK		0x02	/* No medium in the drive */
#define STA_PROTECT		0x04	/* Write protected */

struct hldDiskDevice
{
    struct hldDevice head;

    retcode (*attach)();
    diskstatus (*open)(struct hldDiskDevice *pDiskDev);
    retcode (*close)(struct hldDiskDevice *pDiskDev);

    retcode (*read)(struct hldDiskDevice *pDiskDev, UINT8 *pBuf,
        UINT32 pSector, UINT8 pCount);
    retcode (*write)(struct hldDiskDevice *pDiskDev, const UINT8 *pBuf,
        UINT32 pSector, UINT8 pCount);
    retcode (*ioctl)(struct hldDiskDevice *pDiskDev, UINT32 pCmd, UINT32 *pParam);
    diskstatus (*status)(struct hldDiskDevice *pDiskDev);
};

#endif
