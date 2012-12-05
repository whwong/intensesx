/**
 * @file pic32SDMMC.c
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

#include <p32xxxx.h>

#include "lib/common.h"
#include "os/timers.h"
#include "hal/hld/hld.h"
#include "hal/hld/disk.h"
#include "pic32SDMMC.h"
#include "lib/fatfs/diskio.h"
#include "lib/input/inputEvent.h"

static char lldPic32SDMMCName[HLD_DEVICE_MAX_NAME_LEN] =
    "PIC32 SDMMC SPI1 Driver";
static struct hldDiskDevice *currentDevice = NULL;

/* DEFINES */

/* Timer period - should be smth about 1-10ms */
#define TIMER_PERIOD_MS 1
/* Just random value :) */
#define TIMER_ID 122
/* RTOS timeout to add/del timer from timer queue */
#define TIMER_TIMEOUT 1000

/* Definitions for MMC/SDC command */
#define CMD0   (0)          /* GO_IDLE_STATE */
#define CMD1   (1)          /* SEND_OP_COND */
#define ACMD41 (41|0x80)    /* SEND_OP_COND (SDC) */
#define CMD8   (8)          /* SEND_IF_COND */
#define CMD9   (9)          /* SEND_CSD */
#define CMD10  (10)         /* SEND_CID */
#define CMD12  (12)         /* STOP_TRANSMISSION */
#define ACMD13 (13|0x80)    /* SD_STATUS (SDC) */
#define CMD16  (16)         /* SET_BLOCKLEN */
#define CMD17  (17)         /* READ_SINGLE_BLOCK */
#define CMD18  (18)         /* READ_MULTIPLE_BLOCK */
#define CMD23  (23)         /* SET_BLOCK_COUNT */
#define ACMD23 (23|0x80)    /* SET_WR_BLK_ERASE_COUNT (SDC) */
#define CMD24  (24)         /* WRITE_BLOCK */
#define CMD25  (25)         /* WRITE_MULTIPLE_BLOCK */
#define CMD41  (41)         /* SEND_OP_COND (ACMD) */
#define CMD55  (55)         /* APP_CMD */
#define CMD58  (58)         /* READ_OCR */

/* Port Controls  (Platform dependent) */
#define CS_SETOUT() TRISAbits.TRISA9 = 0
#define CS_LOW()  _LATA9 = 0	/* MMC CS = L */
#define CS_HIGH() _LATA9 = 1	/* MMC CS = H */

// Change the SPI port number as needed on the following 5 lines
#define SPIBRG  SPI1BRG
#define SPIBUF  SPI1BUF
#define SPISTATbits SPI1STATbits
#define SPI_CHANNEL SPI_CHANNEL1
#define SPICONbits SPI1CONbits

//REVEIW THIS LATER!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
// Makes assumptions that sockwp and sockins are on the same port...
// Should probably remove sockport define and then go fix what used it to be general.
/* Socket contact port */
#define SOCKPORT    PORTG
/* Write protect switch (RB10) */
#define SOCKWP      (1<<6)
/* Card detect switch (RB11) */
#define SOCKINS     (1<<7)

/* Set slow clock (100k-400k)
 * PBclk = 80Mhz
 * 99 - 400kHz
 * 127 - 312.5kHz */
#define	FCLK_SLOW() SPIBRG = 127
/* Set fast clock (depends on the CSD)
 * PBclk = 80Mhz
 * 1 - 20MHz  */
#define	FCLK_FAST() SPIBRG = 1

/* STATIC VARIABLES */
static volatile diskstatus Stat = STA_NOINIT; /* Disk status */
static volatile UINT Timer1, Timer2; /* 1000Hz decrement timer */
static UINT CardType;
static xTimerHandle hTimer;

/* LOW LEVEL SPI SUPPORT */

/* Exchange a byte between PIC and MMC via SPI  (Platform dependent) */
#define xmit_spi(dat) 	xchg_spi(dat)
#define rcvr_spi()      xchg_spi(0xFF)

#define rcvr_spi_m(p) \
{ \
    volatile UINT32 n = 100000000; \
    SPIBUF = 0xFF; \
    while ((!SPISTATbits.SPIRBF) && (--n)); \
    *(p) = (BYTE)SPIBUF; \
}

static BYTE xchg_spi (BYTE dat)
{
    volatile UINT32 n = 100000000;
    SPIBUF = dat;
    while ((!SPISTATbits.SPIRBF) && (--n));
    return (BYTE) SPIBUF;
}

static BYTE wait_ready (void)
{
    BYTE res;

    Timer2 = 500; /* Wait for ready in timeout of 500ms */
    rcvr_spi();

    do
    {
        res = rcvr_spi();
    }
    while ((res != 0xFF) && Timer2);

    return res;
}

/**
 * Deselect the card and release SPI bus
 */
static void deselect (void)
{
    CS_HIGH();
    rcvr_spi();
}
/**
 * Select the card and wait ready
 * @return Result
 * @retval 1 Successful
 * @retval 0 Timeout
 */
static int select(void)
{
    CS_LOW();
    if (wait_ready() != 0xFF)
    {
        deselect();
        return 0;
    }
    return 1;
}

/* HI LEVEL SD CARD SUPPORT */

/*
 * Power Control  (Platform dependent)
 * When the target system does not support socket power control, there
 * is nothing to do in these functions and chk_power always returns 1.
 */

static void power_on (void)
{
    // Setup CS as output
    CS_SETOUT();
    // configured for ~400 kHz operation - reset later to 20 MHz
    SpiChnOpen(SPI_CHANNEL, SPI_OPEN_MSTEN | SPI_OPEN_CKP_HIGH |
            SPI_OPEN_SMP_END | SPI_OPEN_MODE8, 128);
    SPICONbits.ON = 1;
}

static void power_off (void)
{
    select(); /* Wait for card ready */
    deselect();

    SPICONbits.ON = 0; /* Disable SPI */

    Stat |= STA_NOINIT; /* Set STA_NOINIT */
    LOG("Stat |= STA_NOINIT");
}

/**
 * Receive a data packet from MMC
 * @param buff Data buffer to store received data
 * @param btr Byte count (must be multiple of 4)
 * @return Result code
 * @retval 1 OK
 * @retval 0 Failed
 */
static int rcvr_datablock(BYTE *buff, UINT btr)
{
    BYTE token;

    Timer1 = 100;
    do
    { /* Wait for data packet in timeout of 100ms */
        token = rcvr_spi();
    }
    while ((token == 0xFF) && Timer1);

    if (token != 0xFE) return 0; /* If not valid data token, retutn with error */

    do
    { /* Receive the data block into buffer */
        rcvr_spi_m(buff++);
        rcvr_spi_m(buff++);
        rcvr_spi_m(buff++);
        rcvr_spi_m(buff++);
    }
    while (btr -= 4);
    rcvr_spi(); /* Discard CRC */
    rcvr_spi();

    return 1; /* Return with success */
}

/**
 * Send a data packet to MMC
 * @param buff 512 byte data block to be transmitted
 * @param token Data token
 * @return Result code
 * @retval 1 OK
 * @retval 0 Failed
 */
static int xmit_datablock(const BYTE *buff, BYTE token)
{
    BYTE resp;
    UINT bc = 512;

    if (wait_ready() != 0xFF) return 0;

    xmit_spi(token); /* Xmit a token */
    if (token != 0xFD)
    { /* Not StopTran token */
        do
        { /* Xmit the 512 byte data block to the MMC */
            xmit_spi(*buff++);
            xmit_spi(*buff++);
        }
        while (bc -= 2);
        xmit_spi(0xFF); /* CRC (Dummy) */
        xmit_spi(0xFF);
        resp = rcvr_spi(); /* Receive a data response */
        if ((resp & 0x1F) != 0x05) /* If not accepted, return with error */
            return 0;
    }

    return 1;
}

/**
 * Send a command packet to MMC
 * @param cmd Command byte
 * @param arg Argument
 * @return Response value
 */
static BYTE send_cmd (BYTE cmd, DWORD arg)
{
    BYTE n, res;

    if (cmd & 0x80)
    { /* ACMD is the command sequense of CMD55-CMD */
        cmd &= 0x7F;
        res = send_cmd(CMD55, 0);
        if (res > 1) return res;
    }

    /* Select the card and wait for ready */
    deselect();
    if (!select()) return 0xFF;

    /* Send command packet */
    xmit_spi(0x40 | cmd); /* Start + Command index */
    xmit_spi((BYTE) (arg >> 24)); /* Argument[31..24] */
    xmit_spi((BYTE) (arg >> 16)); /* Argument[23..16] */
    xmit_spi((BYTE) (arg >> 8)); /* Argument[15..8] */
    xmit_spi((BYTE) arg); /* Argument[7..0] */
    n = 0x01; /* Dummy CRC + Stop */
    if (cmd == CMD0) n = 0x95; /* Valid CRC for CMD0(0) */
    if (cmd == CMD8) n = 0x87; /* Valid CRC for CMD8(0x1AA) */
    xmit_spi(n);

    /* Receive command response */
    if (cmd == CMD12) rcvr_spi(); /* Skip a stuff byte when stop reading */
    n = 10; /* Wait for a valid response in timeout of 10 attempts */
    do
        res = rcvr_spi();
    while ((res & 0x80) && --n);

    return res; /* Return with the response value */
}

static void lldPic32SDMMCTimerProc(xTimerHandle pxTimer)
{
    volatile static WORD pv;
    volatile WORD p;
    BYTE s;
    UINT n;

    n = Timer1; /* 1000Hz decrement timer */
    if (n) Timer1 = --n;
    n = Timer2;
    if (n) Timer2 = --n;

    p = pv;
    pv = SOCKPORT & (SOCKWP | SOCKINS); /* Sample socket switch */

    if (p == pv)
    { /* Have contacts stabled? */
        s = Stat;

        if (p & SOCKWP) /* WP is H (write protected) */
        {
            s |= STA_PROTECT;
        }
        else /* WP is L (write enabled) */
        {
            s &= ~STA_PROTECT;
        }

        if (p & SOCKINS) /* INS = H (Socket empty) */
        {
            s |= (STA_NODISK | STA_NOINIT);
        }
        else /* INS = L (Card inserted) */
        {
            s &= ~STA_NODISK;
        }

        if (Stat != s)
        {
            WORD change = s ^ Stat;

            if ((change & STA_NODISK) == STA_NODISK)
            {
                LOG("Stat = %x",s);
                if ((s & STA_NODISK) == STA_NODISK)
                {
                    LOG("SD Card Removed");
                    inputDiskEventNotify(EVENT_DISK_REMOVED, currentDevice->head.id);
                }
                else
                {
                    LOG("SD Card Inserted");
                    inputDiskEventNotify(EVENT_DISK_INSERTED, currentDevice->head.id);
                }
            }
        }

        Stat = s;
    }
}

/**
 * @brief Attach WM8731 device
 *
 * @return Result value
 */
retcode lldPic32SDMMCAttach()
{
    struct hldDiskDevice *dev;

    // Only one device could be opened - this is guaranteed by unique name
    // requirement
    dev = hldDeviceAlloc(lldPic32SDMMCName, HLD_DEVICE_TYPE_DISK,
            sizeof(struct hldDiskDevice));

    if (dev == NULL)
        return ERR_NO_MEMMORY;

    dev->attach =   lldPic32SDMMCAttach;
    dev->open =     lldPic32SDMMCOpen;
    dev->close =    lldPic32SDMMCClose;
    dev->ioctl =    lldPic32SDMMCIoctl;
    dev->status =   lldPic32SDMMCStatus;
    dev->read =     lldPic32SDMMCRead;
    dev->write =    lldPic32SDMMCWrite;
    currentDevice = dev;

    hTimer = xTimerCreate("SD", TIMER_PERIOD_MS / portTICK_RATE_MS,
                pdTRUE, (void *)TIMER_ID, lldPic32SDMMCTimerProc);

    if (hTimer == NULL)
    {
        hldDeviceFree(dev);
        return ERR_NO_MEMMORY;
    }
    else
    {
        if (xTimerStart(hTimer, TIMER_TIMEOUT) != pdPASS)
        {
            xTimerDelete(hTimer, TIMER_TIMEOUT);
            hldDeviceFree(dev);
            return ERR_NO_MEMMORY;
        }
    }

    retcode result = hldDeviceRegister(dev);
    if (result != SUCCESS)
    {
        hldDeviceFree(dev);
    }
    else
    {
        dev->head.state |= HLD_DEVICE_STATE_TURNED_ON;
    }

    DEBUG("PIC32 SDMMC Attached result: %x", result);
    
    return result;
}

/**
 * @brief Open PIC32 SDMMC device
 *
 * @param pDiskDev Pointer to device which will be opened
 * @return Result value
 */
static diskstatus lldPic32SDMMCOpen(struct hldDiskDevice *pDiskDev)
{
    assert(pDiskDev != NULL);

    BYTE n, cmd, ty, ocr[4];

    if (Stat & STA_NODISK) return Stat; /* No card in the socket */

    power_on(); /* Force socket power on */
    FCLK_SLOW();
    for (n = 10; n; n--) rcvr_spi(); /* 80 dummy clocks */

    ty = 0;
    if (send_cmd(CMD0, 0) == 1)
    { /* Enter Idle state */
        Timer1 = 1000; /* Initialization timeout of 1000 msec */
        if (send_cmd(CMD8, 0x1AA) == 1)
        { /* SDv2? */
            for (n = 0; n < 4; n++) ocr[n] = rcvr_spi(); /* Get trailing return value of R7 resp */
            if (ocr[2] == 0x01 && ocr[3] == 0xAA)
            { /* The card can work at vdd range of 2.7-3.6V */
                while (Timer1 && send_cmd(ACMD41, 0x40000000)); /* Wait for leaving idle state (ACMD41 with HCS bit) */
                if (Timer1 && send_cmd(CMD58, 0) == 0)
                { /* Check CCS bit in the OCR */
                    for (n = 0; n < 4; n++) ocr[n] = rcvr_spi();
                    ty = (ocr[0] & 0x40) ? CT_SD2 | CT_BLOCK : CT_SD2; /* SDv2 */
                }
            }
        }
        else
        { /* SDv1 or MMCv3 */
            if (send_cmd(ACMD41, 0) <= 1)
            {
                ty = CT_SD1;
                cmd = ACMD41; /* SDv1 */
            }
            else
            {
                ty = CT_MMC;
                cmd = CMD1; /* MMCv3 */
            }
            while (Timer1 && send_cmd(cmd, 0)); /* Wait for leaving idle state */
            if (!Timer1 || send_cmd(CMD16, 512) != 0) /* Set read/write block length to 512 */
                ty = 0;
        }
    }
    CardType = ty;
    deselect();

    if (ty)
    {
        // Init end - success
        Stat &= ~STA_NOINIT; /* Clear STA_NOINIT */
        FCLK_FAST();
        pDiskDev->head.state |= HLD_DEVICE_STATE_RUNNING;

        DONE("%s device opened", lldPic32SDMMCName);
        return Stat;
    }
    else
    { /* Initialization failed */
        power_off();
    }

    ERROR("Opening %s device failed", lldPic32SDMMCName);

    return Stat;
}

/**
 * @brief Close PIC32 SDMMC device
 *
 * @param pDiskDev Pointer to device which will be closed
 * @return Result value
 */
static retcode lldPic32SDMMCClose(struct hldDiskDevice *pDiskDev)
{
    assert(pDiskDev != NULL);

    pDiskDev->head.state &= ~(HLD_DEVICE_STATE_RUNNING |
            HLD_DEVICE_STATE_TURNED_ON);

    /*if ((Stat & STA_NODISK) == 0)
    {
        deselect();
        power_off();
    }*/
    SPICONbits.ON = 0;
    Stat |= STA_NOINIT;
    LOG("Stat |= STA_NOINIT");

    DONE("%s device closed", lldPic32SDMMCName);
    return SUCCESS;
}

static retcode
lldPic32SDMMCIoctl(struct hldDiskDevice *pDiskDev, UINT32 pCmd, UINT32 *pParam)
{
    retcode res;
    BYTE n, csd[16], *ptr = (BYTE *)pParam;
    DWORD csize;

    if (Stat & STA_NOINIT) return RES_NOTRDY;

    res = RES_ERROR;
    switch (pCmd)
    {
        case CTRL_SYNC: /* Flush dirty buffer if present */
            if (select())
            {
                deselect();
                res = RES_OK;
            }
            break;

        case GET_SECTOR_COUNT: /* Get number of sectors on the disk (WORD) */
            if ((send_cmd(CMD9, 0) == 0) && rcvr_datablock(csd, 16))
            {
                if ((csd[0] >> 6) == 1)
                { /* SDv2? */
                    csize = csd[9] + ((WORD) csd[8] << 8) + 1;
                    *(DWORD*) pParam = (DWORD) csize << 10;
                }
                else
                { /* SDv1 or MMCv2 */
                    n = (csd[5] & 15) + ((csd[10] & 128) >> 7) + ((csd[9] & 3) << 1) + 2;
                    csize = (csd[8] >> 6) + ((WORD) csd[7] << 2) + ((WORD) (csd[6] & 3) << 10) + 1;
                    *(DWORD*) pParam = (DWORD) csize << (n - 9);
                }
                res = RES_OK;
            }
            break;

        case GET_SECTOR_SIZE: /* Get sectors on the disk (WORD) */
            *(WORD*) pParam = 512;
            res = RES_OK;
            break;

        case GET_BLOCK_SIZE: /* Get erase block size in unit of sectors (DWORD) */
            if (CardType & CT_SD2)
            { /* SDv2? */
                if (send_cmd(ACMD13, 0) == 0)
                { /* Read SD status */
                    rcvr_spi();
                    if (rcvr_datablock(csd, 16))
                    { /* Read partial block */
                        for (n = 64 - 16; n; n--) rcvr_spi(); /* Purge trailing data */
                        *(DWORD*) pParam = 16UL << (csd[10] >> 4);
                        res = RES_OK;
                    }
                }
            }
            else
            { /* SDv1 or MMCv3 */
                if ((send_cmd(CMD9, 0) == 0) && rcvr_datablock(csd, 16))
                { /* Read CSD */
                    if (CardType & CT_SD1)
                    { /* SDv1 */
                        *(DWORD*) pParam = (((csd[10] & 63) << 1) +
                                ((WORD) (csd[11] & 128) >> 7) + 1) << ((csd[13] >> 6) - 1);
                    }
                    else
                    { /* MMCv3 */
                        *(DWORD*) pParam = ((WORD) ((csd[10] & 124) >> 2) + 1) *
                                (((csd[11] & 3) << 3) + ((csd[11] & 224) >> 5) + 1);
                    }
                    res = RES_OK;
                }
            }
            break;

        case MMC_GET_TYPE: /* Get card type flags (1 byte) */
            *ptr = CardType;
            res = RES_OK;
            break;

        case MMC_GET_CSD: /* Receive CSD as a data block (16 bytes) */
            if ((send_cmd(CMD9, 0) == 0) /* READ_CSD */
                    && rcvr_datablock((BYTE *)pParam, 16))
                res = RES_OK;
            break;

        case MMC_GET_CID: /* Receive CID as a data block (16 bytes) */
            if ((send_cmd(CMD10, 0) == 0) /* READ_CID */
                    && rcvr_datablock((BYTE *)pParam, 16))
                res = RES_OK;
            break;

        case MMC_GET_OCR: /* Receive OCR as an R3 resp (4 bytes) */
            if (send_cmd(CMD58, 0) == 0)
            { /* READ_OCR */
                for (n = 0; n < 4; n++)
                    *((BYTE*) pParam + n) = rcvr_spi();
                res = RES_OK;
            }
            break;

        case MMC_GET_SDSTAT: /* Receive SD statsu as a data block (64 bytes) */
            if (send_cmd(ACMD13, 0) == 0)
            { /* SD_STATUS */
                rcvr_spi();
                if (rcvr_datablock((BYTE *)pParam, 64))
                    res = RES_OK;
            }
            break;

        default:
            res = RES_PARERR;
    }

    deselect();

    return res;
}

/**
 * Read Sector(s)
 * @param pDiskDev Device pointer
 * @param pBuf Pointer to the data buffer to store read data
 * @param pSector Start sector number (LBA)
 * @param pCount Sector count (1..255)
 * @return
 */
static retcode
lldPic32SDMMCRead(struct hldDiskDevice *pDiskDev, UINT8 *pBuf, UINT32 pSector,
    UINT8 pCount)
{
    if (Stat & STA_NOINIT) return RES_NOTRDY;

    if (!(CardType & CT_BLOCK)) pSector *= 512; /* Convert to byte address if needed */

    if (pCount == 1)
    { /* Single block read */
        if ((send_cmd(CMD17, pSector) == 0) /* READ_SINGLE_BLOCK */
                && rcvr_datablock(pBuf, 512))
            pCount = 0;
    }
    else
    { /* Multiple block read */
        if (send_cmd(CMD18, pSector) == 0)
        { /* READ_MULTIPLE_BLOCK */
            do
            {
                if (!rcvr_datablock(pBuf, 512)) break;
                pBuf += 512;
            }
            while (--pCount);
            send_cmd(CMD12, 0); /* STOP_TRANSMISSION */
        }
    }
    deselect();

    return pCount ? RES_ERROR : RES_OK;
}

/**
 * Write Sector(s)
 * @param pDiskDev Drive device pointer
 * @param pBuf Pointer to the data to be written
 * @param pSector Start sector number (LBA)
 * @param pCount Sector count (1..255)
 * @return File system return code
 */
static retcode
lldPic32SDMMCWrite(struct hldDiskDevice *pDiskDev, const UINT8 *pBuf,
    UINT32 pSector, UINT8 pCount)
{
    if (Stat & STA_NOINIT) return RES_NOTRDY;
    if (Stat & STA_PROTECT) return RES_WRPRT;

    /* Convert to byte address if needed */
    if (!(CardType & CT_BLOCK)) pSector *= 512;

    if (pCount == 1)
    { /* Single block write */
        if ((send_cmd(CMD24, pSector) == 0) /* WRITE_BLOCK */
                && xmit_datablock(pBuf, 0xFE))
            pCount = 0;
    }
    else
    { /* Multiple block write */
        if (CardType & CT_SDC) send_cmd(ACMD23, pCount);
        if (send_cmd(CMD25, pSector) == 0)
        { /* WRITE_MULTIPLE_BLOCK */
            do
            {
                if (!xmit_datablock(pBuf, 0xFC)) break;
                pBuf += 512;
            }
            while (--pCount);
            if (!xmit_datablock(0, 0xFD)) /* STOP_TRAN token */
                pCount = 1;
        }
    }
    deselect();

    return pCount ? RES_ERROR : RES_OK;
}

/**
 * Read disk status
 * @param pDiskDev Device pointer
 * @return Disk status
 */
static diskstatus
lldPic32SDMMCStatus(struct hldDiskDevice *pDiskDev)
{
    return Stat;
}
