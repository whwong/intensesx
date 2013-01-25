/**
 * @file graphics.c
 * @brief Graphics library
 * @author Daniel Szot
 */

#include "graphics.h"
#include "hal/hld/lcd.h"

struct hldLcdDevice *graphLcdDev;

void graphSetLcdDevice(struct hldLcdDevice *pLcdDev)
{
    graphLcdDev = pLcdDev;
}

retcode graphSetDrawingColor(UINT8 pA, UINT8 pR, UINT8 pG, UINT8 pB)
{
    if (graphLcdDev != NULL)
    {
        graphLcdDev->setColor(graphLcdDev, pA, pR, pG, pB);
        return SUCCESS;
    }
    else
        return DEVICE_NOT_SET;
}

retcode graphClearScreen()
{
    if (graphLcdDev != NULL)
    {
        graphLcdDev->setColor(graphLcdDev, 0xff, 0x00, 0x00, 0x00);
        graphDrawRect(0,0,graphLcdDev->getMaxX(), graphLcdDev->getMaxY());
        return SUCCESS;
    }
    else
        return DEVICE_NOT_SET;
}
