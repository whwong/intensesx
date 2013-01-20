/**
 * @file wndclass.h
 * @brief Windows class manager
 * @author Daniel Szot
 */

#ifndef _LIB_GUI_WND_CLASS_H
#define _LIB_GUI_WND_CLASS_H

#include "lib/common.h"
#include "guidefs.h"

#define MAX_CLASSNAME_LEN 15

struct guiWndClass
{
    struct list_head list;
    struct guiColorStyle colorStyle;
    UINT32 windowStyle;
    INT32(*windowProc)(struct guiWindow *, UINT32, UINT32, UINT32);
    char className[MAX_CLASSNAME_LEN];
};

struct guiWndClassInfo
{
    struct guiColorStyle colorStyle;
    UINT32 windowStyle;
    INT32(*windowProc)(struct guiWindow *, UINT32, UINT32, UINT32);
    char *className;
};

retcode guiInitWindowClasses();
retcode guiRegisterWindowClass(struct guiWndClassInfo *pWndClassInfo);
retcode guiUnregisterWindowClass(const char *pClassName);
struct guiWndClass *guiGetWindowClass(const char *pClassName);

#endif
