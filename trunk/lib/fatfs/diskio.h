/*-----------------------------------------------------------------------
/  Low level disk interface modlue include file
/-----------------------------------------------------------------------*/

#ifndef _DISKIO

#define _READONLY	0	/* 1: Remove write functions */
#define _USE_IOCTL	1	/* 1: Use disk_ioctl fucntion */

#include "lib/retcode.h"
#include "hal/hld/hld.h"
#include "hal/hld/disk.h"
#include "integer.h"

/* Results of Disk Functions */
#define DRESULT retcode

/* Status of Disk Functions */
#define DSTATUS diskstatus

/*---------------------------------------*/
/* Prototypes for disk control functions */

int assign_drives (int, int);
DSTATUS disk_initialize (BYTE);
DSTATUS disk_status (BYTE);
DRESULT disk_read (BYTE, BYTE*, DWORD, BYTE);
static BYTE send_cmd (BYTE cmd, DWORD arg);
#if _READONLY == 0
DRESULT disk_write (BYTE, const BYTE*, DWORD, BYTE);
static int xmit_datablock (const BYTE *buff,BYTE token);
#endif
DRESULT disk_ioctl (BYTE, BYTE, void*);

/* Command code for disk_ioctrl fucntion */

/* Generic command (defined for FatFs) */
#define CTRL_SYNC			0	/* Flush disk cache (for write functions) */
#define GET_SECTOR_COUNT	1	/* Get media size (for only f_mkfs()) */
#define GET_SECTOR_SIZE		2	/* Get sector size (for multiple sector size (_MAX_SS >= 1024)) */
#define GET_BLOCK_SIZE		3	/* Get erase block size (for only f_mkfs()) */
#define CTRL_ERASE_SECTOR	4	/* Force erased a block of sectors (for only _USE_ERASE) */

/* Generic command */
#define CTRL_POWER			5	/* Get/Set power status */
#define CTRL_LOCK			6	/* Lock/Unlock media removal */
#define CTRL_EJECT			7	/* Eject media */

#define _DISKIO
#endif
