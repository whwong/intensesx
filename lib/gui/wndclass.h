/**
 * @file wndclass.h
 * @brief Windows class manager
 * @author Daniel Szot
 */

#include "guidefs.h"

#define MAX_CLASSNAME_LEN 15

struct guiWndClass
{
    struct list_head list;
    guiColorStyle colorStyle;
    UINT32 windowStyle;
    retcode(*windowProc)(struct guiWindow *, struct msg *, UINT32, UINT32);
    char className[MAX_CLASSNAME_LEN];
};

struct guiWndClassInfo
{
    guiColorStyle colorStyle;
    UINT32 windowStyle;
    retcode(*windowProc)(struct guiWindow *, struct msg *, UINT32, UINT32);
    char className[MAX_CLASSNAME_LEN];
};

retcode guiInitWindowClasses();
retcode guiRegisterWindowClass(struct guiWndClassInfo *pWndClassInfo);
retcode guiUnregisterWindowClass(const char *pClassName);
struct guiWndClass *guiGetWindowClass(const char *pClassName);
