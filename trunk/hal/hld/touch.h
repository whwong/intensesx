/**
 * @file touch.h
 * @brief Touch Screen High Level Driver
 * @author Daniel Szot
 */

#ifndef _HLD_TOUCH_H_
#define _HLD_TOUCH_H_

#include "hld.h"
#include "lib/common.h"

struct hldTouchConfig
{
    UINT16 taskPriority;
    UINT32 sampleTime; // in ticks

    // Calibration
    UINT16 yMinCal;
    UINT16 yMaxCal;
    UINT16 xMinCal;
    UINT16 xMaxCal;

    //Screen
    UINT16 screenResolutionX;
    UINT16 screenResolutionY;
};

struct hldTouchDevice
{
    struct hldDevice head;
    struct hldTouchConfig config;

    retcode (*attach)(struct hldTouchConfig *pCfg);
    retcode (*open)(struct hldTouchDevice *pTouchDev);
    retcode (*close)(struct hldTouchDevice *pTouchDev);

    retcode (*read)(struct hldTouchDevice *pTouchDev, INT32 *pX, INT32 *pY);
};

#endif
