/**
 * @file wndclass.c
 * @brief Windows class manager
 * @author Daniel Szot
 */

#include "wndclass.h"
#include "lib/common.h"
#include "list.h"

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

    // Register default classes
    // TODO: o tutaj register ;)

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
            return ERR_INVALID_NAME;
    }

    xSemaphoreGive(wndClassMutex);

    newWndClass = pvPortMalloc(sizeof(struct guiWndClass));

    if (newWndClass == NULL)
    {
        return ERR_NO_MEMMORY;
    }

    newWndClass.windowStyle = pWndClassInfo.windowStyle;
    newWndClass.windowProc = pWndClassInfo.windowProc;

    strcpy(newWndClass.className, pWndClassInfo.className);

    while (xSemaphoreTake(wndClassMutex, portMAX_DELAY) != pdTRUE);
    list_add(&(newWndClass->list), &clist);
    xSemaphoreGive(wndClassMutex);
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
