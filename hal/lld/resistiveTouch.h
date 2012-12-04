/**
 * @file resistiveTouch.h
 * @brief 4-Wire Resistive Touch Screen Driver
 * Driver based on Microchip TouchScreen.c driver
 * @author Daniel Szot
 */

#ifndef _RESISTIVE_TOUCH_H
#define _RESISTIVE_TOUCH_H

#include "lib/common.h"
#include "board.h"
#include "hal/hld/adc.h"

#if !defined(TOUCH_ADC_CH_XPOS) || !defined(TOUCH_ADC_CH_YPOS) || \
    !defined(TOUCH_LAT_XNEG) || !defined(TOUCH_LAT_XPOS) || \
    !defined(TOUCH_TRIS_XNEG) || !defined(TOUCH_TRIS_XPOS) || \
    !defined(TOUCH_LAT_YNEG) || !defined(TOUCH_LAT_YPOS) || \
    !defined(TOUCH_TRIS_YNEG) || !defined(TOUCH_TRIS_YPOS) || \
    !defined(TOUCH_ADC_CH_YNEG) || !defined(TOUCH_ADC_MAX)
    #error "Please define TOUCH_ADC_CH_X, TOUCH_ADC_CH_Y, TOUCH_LAT_XNEG, \
TOUCH_LAT_XPOS, TOUCH_TRIS_XNEG, TOUCH_TRIS_XPOS, TOUCH_LAT_YNEG, TOUCH_LAT_YPOS \
TOUCH_TRIS_YNEG and TOUCH_TRIS_YPOS macros in board.h before using 4-Wire \
resistive touch screen driver."
#endif

/** Define this macro to get pointer move speed value in px/10ms unit */
//#define SPEED_IN_PXIN10MS
#define TOUCH_FILTER_WINDOW_SIZE 3

#define LLD_RES_TOUCH_STACK_SIZE (configMINIMAL_STACK_SIZE+50)

enum lldResTouchTaskStates
{
    GENERATE_EVENT,
    SET_X,
    GET_X,
    SET_Y,
    GET_Y,
    SET_VALUES
};

retcode lldResTouchAttach(struct hldTouchConfig *pCfg);

static retcode lldResTouchOpen(struct hldTouchDevice *pTouchDev);
static retcode lldResTouchClose(struct hldTouchDevice *pTouchDev);

static inline retcode __attribute__ ((always_inline))
lldResTouchRead(struct hldTouchDevice *pTouchDev, INT32 *pX, INT32 *pY);
static void lldResTouchTask(void *pvParameters);

#endif
