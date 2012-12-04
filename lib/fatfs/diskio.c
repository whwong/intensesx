/*-----------------------------------------------------------------------*/
/* Low level disk I/O module skeleton for FatFs     (C)ChaN, 2012        */
/*-----------------------------------------------------------------------*/
/* If a working storage control module is available, it should be        */
/* attached to the FatFs via a glue function rather than modifying it.   */
/* This is an example of glue functions to attach various exsisting      */
/* storage control module to the FatFs module with a defined API.        */
/*-----------------------------------------------------------------------*/

#include "diskio.h"		/* FatFs lower layer API */

/* Definitions of physical drive number for each media */
#define ATA		0
#define MMC		1
#define USB		2


/*-----------------------------------------------------------------------*/
/* Inidialize a Drive                                                    */

/*-----------------------------------------------------------------------*/

DSTATUS disk_initialize(
        BYTE drv /* Physical drive nmuber (0..) */
        )
{
    struct hldDiskDevice *dev = hldDeviceGetById(HLD_DEVICE_TYPE_DISK, drv);
    if (dev != NULL)
    {
        return dev->open(dev);
    }
    else
        return STA_NOINIT;
}



/*-----------------------------------------------------------------------*/
/* Get Disk Status                                                       */

/*-----------------------------------------------------------------------*/

DSTATUS disk_status(
        BYTE drv /* Physical drive nmuber (0..) */
        )
{
    struct hldDiskDevice *dev = hldDeviceGetById(HLD_DEVICE_TYPE_DISK, drv);
    if (dev != NULL)
    {
        return dev->status(dev);
    }
    else
        return STA_NOINIT;
}



/*-----------------------------------------------------------------------*/
/* Read Sector(s)                                                        */

/*-----------------------------------------------------------------------*/

DRESULT disk_read(
        BYTE drv, /* Physical drive nmuber (0..) */
        BYTE *buff, /* Data buffer to store read data */
        DWORD sector, /* Sector address (LBA) */
        BYTE count /* Number of sectors to read (1..128) */
        )
{
    struct hldDiskDevice *dev = hldDeviceGetById(HLD_DEVICE_TYPE_DISK, drv);
    if (dev != NULL)
    {
        return dev->read(dev, buff, sector, count);
    }
    else
        return RES_PARERR;
}



/*-----------------------------------------------------------------------*/
/* Write Sector(s)                                                       */
/*-----------------------------------------------------------------------*/

#if	_READONLY == 0

DRESULT disk_write(
        BYTE drv, /* Physical drive nmuber (0..) */
        const BYTE *buff, /* Data to be written */
        DWORD sector, /* Sector address (LBA) */
        BYTE count /* Number of sectors to write (1..128) */
        )
{
    struct hldDiskDevice *dev = hldDeviceGetById(HLD_DEVICE_TYPE_DISK, drv);
    if (dev != NULL)
    {
        return dev->write(dev, buff, sector, count);
    }
    else
        return RES_PARERR;
}
#endif


/*-----------------------------------------------------------------------*/
/* Miscellaneous Functions                                               */
/*-----------------------------------------------------------------------*/

#if _USE_IOCTL

DRESULT disk_ioctl(
        BYTE drv, /* Physical drive nmuber (0..) */
        BYTE ctrl, /* Control code */
        void *buff /* Buffer to send/receive control data */
        )
{
    struct hldDiskDevice *dev = hldDeviceGetById(HLD_DEVICE_TYPE_DISK, drv);
    if (dev != NULL)
    {
        return dev->ioctl(dev, ctrl, (UINT32*)buff);
    }
    else
        return RES_PARERR;
}
#endif
