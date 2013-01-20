/**
 * @file gui.h
 * @brief GUI Lib main header file
 * @author Daniel Szot
 */

#ifndef _LIB_GUI_H_
#define _LIB_GUI_H_

#include "lib/common.h"
#include "guidefs.h"
#include "window.h"
#include "wndclass.h"

#define guiSetColor(c) graphSetDrawingColor(c.a, c.r, c.g, c.b)
#define guiDrawRect(c) graphDrawRect(c.l, c.t, c.l+c.w, c.t+c.h)

retcode guiInit();

void guiBeginPaint();
void guiEndPaint();

void guiSetWinstyle(const struct guiWinStyle *pWinstyle);
void guiSetCurrentMainWindow(struct guiMainWindow *pMainWindow);
struct guiMainWindow *guiGetCurrentMainWindow();
void guiSetDefaultFont(const struct graphFont *pFont);
struct graphFont *guiGetDefaultFont();

BOOL guiPointInRect(struct guiPoint *pPoint, struct guiRect *pRect);
BOOL guiXYInRect(UINT16 pX, UINT16 pY, struct guiRect *pRect);

struct guiWinStyle *guiGetStyle(UINT8 pIndex);
void drawStyleFrame(UINT8 pStyleIdx, struct guiRect *pRect);

#endif
