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

void guiRegisterButtonClass();
INT32 guiDefButtonProc(struct guiWindow *pWnd, UINT32 pMsg,
        UINT32 pParam1, UINT32 pParam2);

#endif
