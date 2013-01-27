/**
 * @file statictext.h
 * @brief Static text control
 * @author Daniel Szot
 */

#ifndef _LIB_GUI_STATICTEXT_H
#define _LIB_GUI_STATICTEXT_H

#include "lib/common.h"
#include "guidefs.h"

#define SS_NORMAL   0x00
#define SS_DISABLED 0x04
#define SS_RIGHT    0x08
#define SS_LEFT     0x00
#define SS_CENTER   0x10

void guiRegisterStaticTextClass();
INT32 guiDefStaticTextProc(struct guiWindow *pWnd, UINT32 pMsg,
        UINT32 pParam1, UINT32 pParam2);

#endif
