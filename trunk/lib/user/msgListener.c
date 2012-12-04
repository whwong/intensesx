/**
 * @file msgListener.c
 * @brief Listener of system messages.
 * @author Daniel Szot
 */

#include "lib/common.h"
#include "lib/user/msgListener.h"
#include "lib/log.h"
#include "queue.h"

/**
 * Create a message listener for task
 *
 * @param pQueueSize Messages queue size
 * @return Allocated and created msgListener structure pointer
 */
struct msgListener *msgListenerCreate(UINT16 pQueueSize)
{
    struct msgListener *ml;
    assert(pQueueSize > 0);

    ml = (struct msgListener *)pvPortMalloc(sizeof(struct msgListener));

    // If alloc fail return NULL
    if (ml == NULL)
    {
        ERROR("Message listener memmory alocation failed");
        return NULL;
    }

    ml->task = xTaskGetCurrentTaskHandle();
    ml->msgQueue = xQueueCreate(pQueueSize, sizeof(struct msg));

    if (ml->msgQueue != 0)
    {
        vQueueAddToRegistry(ml->msgQueue, "List");
        msgAddListener(ml);
    }
    else
    {
        vPortFree(ml);
        ERROR("Message queue create failed");
        return NULL;
    }
    
    return ml;
}

/**
 * Delete a message listener for task
 *
 * @param pListener Listener to delete
 */
void msgListenerDelete(struct msgListener *pListener)
{
    assert(pListener != NULL);

    msgDelListener(pListener);
    vQueueDelete(pListener->msgQueue);
    vPortFree(pListener);
}

/**
 * Post message to this particular pListener
 * @param pListener Listener to which message will be directed
 * @param pMsg Pointer to message structure which will be send
 */
retcode msgListenerPost(struct msgListener *pListener, struct msg *pMsg)
{
    BOOL sent;

    assert(pListener != NULL);

    sent = xQueueSend(pListener->msgQueue, (void *)pMsg, MSG_LISTENER_TIMEOUT);
    return (sent == pdTRUE) ? SUCCESS : ERR_QUEUE_FULL;
}

/**
 * Get message from listener. Unlike msgListenerGet, the msgListenerPeek
 * function does not wait for a message to be posted before returning.
 *
 * @param pListener Listener from message will be received
 * @param pMsg Pointer to message structure to which message will be saved
 * @param pWnd A handle to the window whose messages are to be retrieved.
 * The window must belong to the current thread. NOT IMPLEMENTED YET!
 * @param pFilterMax The integer value of the lowest message value to be 
 * retrieved. NOT IMPLEMENTED YET!
 * @param pFilterMin he integer value of the highest message value to be
 * retrieved. NOT IMPLEMENTED YET!
 * @return FALSE if received message is MSG_QUIT, TRUE if not.
 */
BOOL msgListenerGet(struct msgListener *pListener, struct msg *pMsg,
        struct window *pWnd, UINT32 pFilterMin, UINT32 pFilterMax)
{
    UINT32 sent;
    
    assert(pListener != NULL);
    assert(pMsg != NULL);

    do
    {
        sent = xQueueReceive(pListener->msgQueue, pMsg, portMAX_DELAY);
    }
    while (sent != pdPASS);

    if (pMsg->message == MSG_QUIT)
        return FALSE;
    else
        return TRUE;
}

/**
 * Get message from listener. Do not wait for messages. Return if message is
 * not avaliable.
 *
 * @param pListener Listener from message will be received
 * @param pMsg Pointer to message structure to which message will be saved
 * @param pWnd A handle to the window whose messages are to be retrieved.
 * The window must belong to the current thread. NOT IMPLEMENTED YET!
 * @param pFilterMax The integer value of the lowest message value to be
 * retrieved. NOT IMPLEMENTED YET!
 * @param pFilterMin he integer value of the highest message value to be
 * retrieved. NOT IMPLEMENTED YET!
 * @param pRemoveMsg TRUE to remove message from queue, FALSE to not remove.
 * @return TRUE if messasge received, FALSE if not.
 */
BOOL msgListenerPeek(struct msgListener *pListener, struct msg *pMsg,
        struct window *pWnd, UINT32 pFilterMin, UINT32 pFilterMax, BOOL pRemoveMsg)
{
    UINT32 sent;

    assert(pListener != NULL);
    assert(pMsg != NULL);

    if (pRemoveMsg == TRUE)
    {
        sent = xQueueReceive(pListener->msgQueue, pMsg, 0);
    }
    else
    {
        sent = xQueuePeek(pListener->msgQueue, pMsg, 0);
    }

    return sent;
}
