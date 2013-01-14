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

void guiSetWinstyle(struct guiWinStyle *pWinstyle);
void guiSetCurrentMainWindow(struct guiMainWindow *pMainWindow);
struct guiMainWindow *guiGetCurrentMainWindow();
void guiSetDefaultFont(struct graphFont *pFont);
struct graphFont *guiGetDefaultFont();

BOOL guiPointInRect(struct guiPoint *pPoint, struct guiRect *pRect);
BOOL guiXYInRect(UINT16 pX, UINT16 pY, struct guiRect *pRect);

#endif
