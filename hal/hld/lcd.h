/**
 * @file lcd.h
 * @brief LCD High Level Driver
 * @author Daniel Szot
 */

#ifndef _HLD_LCD_H_
#define _HLD_LCD_H_

#include "hld.h"
#include "lib/common.h"

struct hldLcdDevice
{
    struct hldDevice head;

    UINT32 drawingColor;    // Drawing color in color format of LCD

    retcode (*attach)();
    retcode (*open)(struct hldLcdDevice *pLcdDev);
    retcode (*close)(struct hldLcdDevice *pLcdDev);

    retcode (*setColor)(struct hldLcdDevice *pLcdDev, UINT8 pA, UINT8 pR,
        UINT8 pG, UINT8 pB);
    retcode (*setColorRaw)(struct hldLcdDevice *pLcdDev, UINT32 pColor);
    retcode (*drawPixel)(struct hldLcdDevice *pLcdDev, UINT16 pX, UINT16 pY);
    UINT16 (*getPixel)(struct hldLcdDevice *pLcdDev, UINT16 pX, UINT16 pY);
    retcode (*fill)(struct hldLcdDevice *pLcdDev,
        UINT16 pX1, UINT16 pY1, UINT16 pX2, UINT16 pY2);
    
    UINT32 (*getMaxX)();
    UINT32 (*getMaxY)();
};

#endif
