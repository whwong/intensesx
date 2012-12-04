/**
 * @file hx8347.h
 * @brief hx8347 LCD controller driver for Microchip PIC32 processors family
 * Screen resolution is 320x240
 * Color format is RGB565
 * Driver based on Microchip Module for MGL
 * @author Daniel Szot
 */

#ifndef _HX8347_H
#define _HX8347_H

#include <p32xxxx.h>
#include <stdlib.h>
#include <plib.h>
#include "lib/common.h"
#include "gpio.h"
#include "board.h"
#include "hal/hld/lcd.h"

#if !defined(LCD_BACKLIGHT_GPIO) || !defined(LCD_RESET_GPIO)
    #error "Please define LCD_BACKLIGHT_GPIO and LCD_RESET_GPIO macros in \
           board.h before using HX8347 LCD driver."
#endif

#if !defined(LCD_ORIENTATION)
    #warning "LCD_ORIENTATION macro is not defined in \
           board.h. HX8347 LCD driver set this macro to default value 0 deg."

    #define LCD_ORIENTATION 0
#endif

#ifdef __PIC32MX__
#define PMDIN1 PMDIN
#endif

#define PMPWaitBusy()   while(PMMODEbits.BUSY);
#define RGB888TORGB565(r, g, b) (((r >> 3) << 11) | ((g >> 2) << 5) | ((b >> 3)))
//#define RGB888TORGB565(r, g, b) (((r & 0xF8) << 7) | ((g & 0xFC) << 3) | (((b >> 3) & 0x1F)))

retcode lldHx8347Attach();
static retcode lldHx8347Fill(struct hldLcdDevice *pLcdDev,
    UINT16 pX1, UINT16 pY1, UINT16 pX2, UINT16 pY2);
static retcode lldHx8347Open(struct hldLcdDevice *pLcdDev);
static retcode lldHx8347Close(struct hldLcdDevice *pLcdDev);

static inline retcode __attribute__ ((always_inline))
lldHx8347SetColor(struct hldLcdDevice *pLcdDev, UINT8 pA, UINT8 pR, UINT8 pG, UINT8 pB);

static retcode lldHx8347DrawPixel(struct hldLcdDevice *pLcdDev, UINT16 pX, UINT16 pY);
static UINT16 lldHx8347GetPixel(struct hldLcdDevice *pLcdDev, UINT16 pX, UINT16 pY);

static retcode lldHx8347Flush(struct hldLcdDevice *pLcdDev);

static inline UINT32 __attribute__ ((always_inline))
lldHx8347GetMaxX(struct hldLcdDevice *pLcdDev);

static inline UINT32 __attribute__ ((always_inline))
lldHx8347GetMaxY(struct hldLcdDevice *pLcdDev);

#endif
