/**
 * @file graphics.h
 * @brief Graphics library
 * @author Daniel Szot
 */

#ifndef _GRAPHICS_LIBRARY_H
#define _GRAPHICS_LIBRARY_H

#define GRAPH_TESTS

#include "lib/common.h"
#include "hal/hld/lcd.h"

extern struct hldLcdDevice *graphLcdDev;

// OS Porting
#define graphDelayMs vTaskDelay

// Common graphics library funkctions
void graphSetLcdDevice(struct hldLcdDevice *pLcdDev);
retcode graphSetDrawingColor(UINT8 pA, UINT8 pR, UINT8 pG, UINT8 pB);

// Drawing primitives
retcode graphDrawLine(UINT16 pX1, UINT16 pY1, UINT16 pX2, UINT16 pY2);
retcode graphDrawRect(UINT16 pX1, UINT16 pY1, UINT16 pX2, UINT16 pY2);
retcode graphDrawCircle(UINT16 pX, UINT16 pY, UINT16 pRadius);
retcode graphDrawRoundedRect();

// Tests of features
#ifdef GRAPH_TESTS
void graphTestDrawEdgeMesh();
void graphTestDrawRand1kLines();
void graphTestDrawRand10kLines();
void graphTestDrawRand30Circles();
#endif

#endif
