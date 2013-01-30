/**
 * @file fpgaGPU.h
 * @brief Driver for LCD fpga based graphics controller
 * @author Daniel Szot
 */

#ifndef _FPGA_GPU_H
#define _FPGA_GPU_H

#include <p32xxxx.h>
#include <stdlib.h>
#include <plib.h>
#include "lib/common.h"
#include "gpio.h"
#include "board.h"
#include "hal/hld/lcd.h"

#ifdef __PIC32MX__
#define PMDIN1 PMDIN
#endif

// Defines below are reserved for PCA9532 which is mounted on LCD board,
// FPGA GPU is only bridge so we need to suppprt it at our side.
#define DEV_FIXED_ADDR  0xC0
#define DEV_HW_ADDR     0x08

#define DEV_ADDR   (DEV_FIXED_ADDR | DEV_HW_ADDR)

#define AUTO_INC    0x10

#define INPUT0  0x00
#define INPUT1  0x01
#define PSC0    0x02
#define PWM0    0x03
#define PSC1    0x04
#define PWM1    0x05
#define LS0     0x06
#define LS1     0x07
#define LS2     0x08
#define LS3     0x09
// End

#define PMPWaitBusy()   while(PMMODEbits.BUSY);
#define RGB888TORGB565(r, g, b) (((r >> 3) << 11) | ((g >> 2) << 5) | ((b >> 3)))

retcode lldFpgaGpuAttach();
static retcode lldFpgaGpuFill(struct hldLcdDevice *pLcdDev,
    UINT16 pX1, UINT16 pY1, UINT16 pX2, UINT16 pY2);
static retcode lldFpgaGpuOpen(struct hldLcdDevice *pLcdDev);
static retcode lldFpgaGpuClose(struct hldLcdDevice *pLcdDev);

static inline retcode __attribute__ ((always_inline))
lldFpgaGpuSetColor(struct hldLcdDevice *pLcdDev, UINT8 pA, UINT8 pR, UINT8 pG, UINT8 pB);

static retcode lldFpgaGpuDrawPixel(struct hldLcdDevice *pLcdDev, UINT16 pX, UINT16 pY);
static UINT16 lldFpgaGpuGetPixel(struct hldLcdDevice *pLcdDev, UINT16 pX, UINT16 pY);

static retcode lldFpgaGpuFlush(struct hldLcdDevice *pLcdDev);

static inline UINT32 __attribute__ ((always_inline))
lldFpgaGpuGetMaxX(struct hldLcdDevice *pLcdDev);

static inline UINT32 __attribute__ ((always_inline))
lldFpgaGpuGetMaxY(struct hldLcdDevice *pLcdDev);

#endif
