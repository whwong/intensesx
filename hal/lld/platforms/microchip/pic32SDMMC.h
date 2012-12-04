/**
 * @file pic32SDMMC.h
 * @brief Driver for SDMMC card connected to the SPI1 module
 * @author Daniel Szot
 * @note Based on mmcPIC32.c port to FatFS for PIC32
 * 
 * Oryginal file header:
 * Edited by A. Morrison to function on PIC32.
 * Ported by Riccardo Leonardi to PIC32MX795F512L  (22/11/2011)
 * Many thanks to Aiden Morrison's good work!
 * changes: parametrization of SPI port number
 */

#ifndef _PIC32_PIC32SDMMC_H
#define _PIC32_PIC32SDMMC_H

#include "lib/common.h"
#include "board.h"
#include "hal/hld/disk.h"

/* MMC/SDC specific ioctl command */
#define MMC_GET_TYPE		10	/* Get card type */
#define MMC_GET_CSD			11	/* Get CSD */
#define MMC_GET_CID			12	/* Get CID */
#define MMC_GET_OCR			13	/* Get OCR */
#define MMC_GET_SDSTAT		14	/* Get SD status */

/* MMC/SDC card type definitions (CardType) */
#define CT_MMC				0x01
#define CT_SD1				0x02
#define CT_SD2				0x04
#define CT_SDC				(CT_SD1|CT_SD2)
#define CT_BLOCK			0x08

retcode lldPic32SDMMCAttach();

static diskstatus lldPic32SDMMCOpen(struct hldDiskDevice *pDiskDev);
static retcode lldPic32SDMMCClose(struct hldDiskDevice *pDiskDev);

static retcode lldPic32SDMMCRead(struct hldDiskDevice *pDiskDev, 
        UINT8 *pBuf, UINT32 pSector, UINT8 pCount);

static retcode lldPic32SDMMCWrite(struct hldDiskDevice *pDiskDev, 
        const UINT8 *pBuf, UINT32 pSector, UINT8 pCount);

static retcode lldPic32SDMMCIoctl(struct hldDiskDevice *pDiskDev, 
        UINT32 pCmd, UINT32 *pParam);

static diskstatus lldPic32SDMMCStatus(struct hldDiskDevice *pDiskDev);

#endif
