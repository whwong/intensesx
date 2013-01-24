/**
 * @file button.h
 * @brief Button control
 * @author Daniel Szot
 */

#ifndef _LIB_GUI_BUTTON_H
#define _LIB_GUI_BUTTON_H

#include "lib/common.h"
#include "guidefs.h"

#define BS_NORMAL   0x00
#define BS_PRESSED  0x01
#define BS_HILITE   0x02
#define BS_DISABLED 0x04

#define BN_CLICKED 0
#define BN_DISABLE 4
#define BN_KILLFOCUS 7
#define BN_PAINT 1
#define BN_PUSHED 2
#define BN_SETFOCUS 6
#define BN_UNPUSHED 3

#define BUTTON_NOTIFY_PARENT(wnd, code) msgPost(wnd->mainWin, MSG_COMMAND, \
        ((code << 16) | (wnd->id & 0xFFFF)), (UINT32)wnd)

void guiRegisterButtonClass();
INT32 guiDefButtonProc(struct guiWindow *pWnd, UINT32 pMsg,
        UINT32 pParam1, UINT32 pParam2);

#endif
