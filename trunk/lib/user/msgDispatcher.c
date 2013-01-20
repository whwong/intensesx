/**
 * @file msgDispatcher.c
 * @brief Listener of system messages.
 * @author Daniel Szot
 */

#include "lib/common.h"
#include "lib/user/msgDispatcher.h"
#include "lib/log.h"
#include "message.h"
#include "semphr.h"
#include "lib/gui/window.h"

static struct msgDispatcher dispatcher;
static xSemaphoreHandle dispatcherMutex = NULL;

/**
 * Message dispatcher initialisation. Run it before running inputTaskInit().
 */
retcode msgDispatcherInit()
{
    dispatcher.curPointerPosX = 0;
    dispatcher.curPointerPosY = 0;

    if (dispatcherMutex == NULL)
        dispatcherMutex = xSemaphoreCreateMutex();

    if (dispatcherMutex == NULL)
    {
        return ERR_NO_MEMMORY;
    }

    if (xSemaphoreTake(dispatcherMutex, portMAX_DELAY))
    {
        INIT_LIST_HEAD(&dispatcher.listenersList);
        xSemaphoreGive(dispatcherMutex);
    }
    else
    {
        MUTEX_ERROR();
        return ERR_MUTEX;
    }

    return SUCCESS;
}

/**
 * Add message listener. This should be used only by msg subsystem.
 * Shared function to register and create listener is msgListenerCreate - use
 * this one in your application.
 * @param pListener
 */
void msgAddListener(struct msgListener* pListener)
{
    while (xSemaphoreTake(dispatcherMutex, portMAX_DELAY) != pdTRUE);
    list_add(&(pListener->list), &dispatcher.listenersList);
    dispatcher.listenersCount++;
    xSemaphoreGive(dispatcherMutex);
}

/**
 * Delete message listener. This should be used only by msg subsystem.
 * Shared function to register and create listener is msgListenerCreate - use
 * this one in your application.
 * @param pListener
 */
void msgDelListener(struct msgListener* pListener)
{
    while (xSemaphoreTake(dispatcherMutex, portMAX_DELAY) != pdTRUE);
    list_del(&(pListener->list));
    dispatcher.listenersCount--;
    xSemaphoreGive(dispatcherMutex);
}

/**
 * Post message to all listeners
 *
 * This should checks that message is directed to one of the listeners windows
 * and send it only if it is. But this is not implemented yet.
 *
 * @param pWnd Pointer to window to which the message is directed.
 * Not implemented yet!
 * @param pMessage Message code
 * @param pParam1 Parameter 1
 * @param pParam2 Parameter 2
 */
void msgPost(struct guiWindow *pWnd, UINT32 pMessage, UINT32 pParam1, UINT32 pParam2)
{
    struct msg m;
    struct msgListener *l;
    UINT8 listenersCount = 0;

    while (xSemaphoreTake(dispatcherMutex, portMAX_DELAY) != pdTRUE);
    if (pMessage >= MSG_POINTERFIRST && pMessage <= MSG_POINTERLAST)
    {
        dispatcher.curPointerPosX = pParam1 & 0xFFFF;
        dispatcher.curPointerPosY = (pParam1 >> 16) & 0xFFFF;
    }

    m.wnd = pWnd;
    m.message = pMessage;
    m.param1 = pParam1;
    m.param2 = pParam2;
    m.ptX = dispatcher.curPointerPosX;
    m.ptY = dispatcher.curPointerPosY;
    m.timestamp = xTaskGetTickCount();

    list_for_each_entry(l, &dispatcher.listenersList, list)
    {
        listenersCount++;
        msgListenerPost(l, &m);
    }
    xSemaphoreGive(dispatcherMutex);

    DEBUG("Message dispatched: 0x%08x, p1: 0x%x, p2: 0x%x to %d listeners",
            pMessage, pParam1, pParam2, listenersCount);
}

/**
 * Send sync message to all listeners. In difference to msgPost this
 * function do not queue message and dispatch it by message listeners
 * but just call windowProc from given window pointer
 *
 * @param pWnd Pointer to window to which the message is directed.
 * @param pMessage Message code
 * @param pParam1 Parameter 1
 * @param pParam2 Parameter 2
 * @return Return value is message dependend
 */
INT32 msgSend(struct guiWindow *pWnd, UINT32 pMessage, UINT32 pParam1, UINT32 pParam2)
{
    struct msg m;
    UINT32 ret = 0;

    while (xSemaphoreTake(dispatcherMutex, portMAX_DELAY) != pdTRUE);
    if (pMessage >= MSG_POINTERFIRST && pMessage <= MSG_POINTERLAST)
    {
        dispatcher.curPointerPosX = pParam1 & 0xFFFF;
        dispatcher.curPointerPosY = (pParam1 >> 16) & 0xFFFF;
    }

    m.wnd = pWnd;
    m.message = pMessage;
    m.param1 = pParam1;
    m.param2 = pParam2;
    m.ptX = dispatcher.curPointerPosX;
    m.ptY = dispatcher.curPointerPosY;
    m.timestamp = xTaskGetTickCount();
    xSemaphoreGive(dispatcherMutex);

    if ((pWnd != NULL) && (pWnd->windowProc != NULL))
    {
        ret = pWnd->windowProc(pWnd, m.message, pParam1, pParam2);

        DEBUG("Message sent: 0x%08x, p1: 0x%x, p2: 0x%x to wnd: %p",
            pMessage, pParam1, pParam2, pWnd);
    }
    else
        ret = -1;
    
    return ret;
}

/**
 * Post thread message to specified thread
 *
 * @param pTaskHandle Task handle to which the message is directed.
 * @param pMessage Message code
 * @param pParam1 Parameter 1
 * @param pParam2 Parameter 2
 */
void msgPostTask(xTaskHandle pTaskHandle, UINT32 pMessage, UINT32 pParam1, UINT32 pParam2)
{
    struct msg m;
    struct msgListener *l;
    UINT8 listenersCount = 0, listenersDispatched = 0;

    while (xSemaphoreTake(dispatcherMutex, portMAX_DELAY) != pdTRUE);
    if (pMessage >= MSG_POINTERFIRST && pMessage <= MSG_POINTERLAST)
    {
        dispatcher.curPointerPosX = pParam1 & 0xFFFF;
        dispatcher.curPointerPosY = (pParam1 >> 16) & 0xFFFF;
    }

    m.wnd = NULL;
    m.message = pMessage;
    m.param1 = pParam1;
    m.param2 = pParam2;
    m.ptX = dispatcher.curPointerPosX;
    m.ptY = dispatcher.curPointerPosY;
    m.timestamp = xTaskGetTickCount();

    list_for_each_entry(l, &dispatcher.listenersList, list)
    {
        if (l->task == pTaskHandle)
        {
            listenersCount++;
            if (msgListenerPost(l, &m) == SUCCESS)
                listenersDispatched++;
        }
    }
    xSemaphoreGive(dispatcherMutex);

    DEBUG("Message dispatched: 0x%08x, p1: 0x%x, p2: 0x%x to %d/%d listeners",
            pMessage, pParam1, pParam2, listenersDispatched, listenersCount);
}

/**
 * Dispatch message synchronously to desired window
 * @param pMsg Message to dispatch
 * @return Window proc result
 */
INT32 msgDispatch(struct msg *pMsg)
{
    INT32 ret = -1;

    assert(pMsg != NULL);

    DEBUG("Message will be dispatched to window");

    if (pMsg->wnd == NULL)
        return ret;
    
    if (pMsg->wnd->windowProc == NULL)
        return ret;

    ret = (*pMsg->wnd->windowProc)(pMsg->wnd, pMsg->message,
        pMsg->param1, pMsg->param2);

    DEBUG("Message executed: 0x%08x, p1: 0x%x, p2: 0x%x to wnd: 0x%x ret: %d",
        pMsg->message, pMsg->param1, pMsg->param2, pMsg->wnd, ret);

    return ret;
}
