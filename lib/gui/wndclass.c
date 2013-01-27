/**
 * @file wndclass.c
 * @brief Windows class manager
 * @author Daniel Szot
 */

#include "wndclass.h"
#include "lib/common.h"
#include "list.h"
#include "button.h"

#define WND_CLASS_DEBUG
#if defined(WND_CLASS_DEBUG)
#define WND_CLASS_LOG(frm, ...) LOG("wnd: "frm, ##__VA_ARGS__)
#else
#define WND_CLASS_LOG(...)
#endif

static struct list_head clist;
static xSemaphoreHandle wndClassMutex = NULL;

retcode guiInitWindowClasses()
{
    if (wndClassMutex == NULL)
        wndClassMutex = xSemaphoreCreateMutex();

    if (wndClassMutex == NULL)
    {
        return ERR_NO_MEMMORY;
    }

    if (xSemaphoreTake(wndClassMutex, portMAX_DELAY))
    {
        INIT_LIST_HEAD(&clist);
        xSemaphoreGive(wndClassMutex);
    }
    else
    {
        MUTEX_ERROR();
        return ERR_MUTEX;
    }

    WND_CLASS_LOG("Creating default window classes...");

    guiRegisterButtonClass();
    guiRegisterProgressBarClass();
    guiRegisterStaticTextClass();

    return SUCCESS;
}

retcode guiRegisterWindowClass(struct guiWndClassInfo *pWndClassInfo)
{
    struct guiWndClass *wclass;
    struct guiWndClass *newWndClass;

    assert(pWndClassInfo != NULL);
    
    while (xSemaphoreTake(wndClassMutex, portMAX_DELAY) != pdTRUE);

    list_for_each_entry(wclass, &clist, list)
    {
        if (strcmp(wclass->className, pWndClassInfo->className) == 0)
        {
            WND_CLASS_LOG("Window class name already exists (\"%s\")",
                    newWndClass->className);
            return ERR_INVALID_NAME;
        }
    }

    xSemaphoreGive(wndClassMutex);

    newWndClass = pvPortMalloc(sizeof(struct guiWndClass));

    if (newWndClass == NULL)
    {
        WND_CLASS_LOG("Out of memory to register new window class (\"%s\")",
                newWndClass->className);
        return ERR_NO_MEMMORY;
    }

    newWndClass->windowStyle = pWndClassInfo->windowStyle;
    newWndClass->windowProc = pWndClassInfo->windowProc;
    newWndClass->colorStyle = pWndClassInfo->colorStyle;

    strcpy(newWndClass->className, pWndClassInfo->className);

    while (xSemaphoreTake(wndClassMutex, portMAX_DELAY) != pdTRUE);
    list_add(&(newWndClass->list), &clist);
    xSemaphoreGive(wndClassMutex);
    WND_CLASS_LOG("New window class registered (\"%s\")", newWndClass->className);

    return SUCCESS;
}

retcode guiUnregisterWindowClass(const char *pClassName)
{
    struct guiWndClass *wclass;

    assert(pClassName != NULL);
    
    while (xSemaphoreTake(wndClassMutex, portMAX_DELAY) != pdTRUE);

    list_for_each_entry(wclass, &clist, list)
    {
        if (strcmp(wclass->className, pClassName) == 0)
        {
            WND_CLASS_LOG("Window class unregistered (\"%s\")", wclass->className);
            list_del(&(wclass->list));
            xSemaphoreGive(wndClassMutex);
            vPortFree(wclass);
            return SUCCESS;
        }
    }
    
    xSemaphoreGive(wndClassMutex);
    return ERR_INVALID_NAME;
}

struct guiWndClass *guiGetWindowClass(const char *pClassName)
{
    struct guiWndClass *wclass;

    assert(pClassName != NULL);

    while (xSemaphoreTake(wndClassMutex, portMAX_DELAY) != pdTRUE);

    list_for_each_entry(wclass, &clist, list)
    {
        if (strcmp(wclass->className, pClassName) == 0)
        {
            xSemaphoreGive(wndClassMutex);
            return wclass;
        }
    }

    xSemaphoreGive(wndClassMutex);
    return NULL;
}
