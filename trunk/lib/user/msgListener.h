/**
 * @file msgListener.h
 * @brief Listener of system messages.
 * @author Daniel Szot
 */

#ifndef _LIB_MSG_LISTENER_H
#define _LIB_MSG_LISTENER_H

#include "os/queue.h"
#include "os/task.h"
#include "lib/list.h"
#include "lib/user/message.h"
//#include "lib/user/msgDispatcher.h"

#define MSG_LISTENER_TIMEOUT 10

struct msgListener
{
    struct list_head list;
    xQueueHandle msgQueue;
    xTaskHandle task;
};

struct msgListener *msgListenerCreate(UINT16 pQueueSize);
void msgListenerDelete(struct msgListener *pListener);

retcode msgListenerPost(struct msgListener *pListener, struct msg *pMsg);

BOOL msgListenerGet(struct msgListener *pListener, struct msg *pMsg,
        struct window *pWnd, UINT32 pFilterMin, UINT32 pFilterMax);

BOOL msgListenerPeek(struct msgListener *pListener, struct msg *pMsg,
        struct window *pWnd, UINT32 pFilterMin, UINT32 pFilterMax, BOOL pRemoveMsg);

#endif
