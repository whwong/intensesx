/**
 * @file msgDispatcher.h
 * @brief Message dispatcher. Main file of messages library.
 * @author Daniel Szot
 */

#ifndef _LIB_MSG_DISPATCHER_H
#define _LIB_MSG_DISPATCHER_H

#include "os/queue.h"
#include "lib/list.h"
#include "lib/user/msgListener.h"

struct msgDispatcher
{
    struct list_head listenersList;
    UINT32 listenersCount;
    
    UINT32 curPointerPosX;
    UINT32 curPointerPosY;
};

retcode msgDispatcherInit();
void msgAddListener(struct msgListener* pListener);
void msgDelListener(struct msgListener* pListener);

void msgPost(struct window *pWnd, UINT32 pMessage, UINT32 pParam1, UINT32 pParam2);
void msgPostTask(xTaskHandle pTaskHandle, UINT32 pMessage, UINT32 pParam1, UINT32 pParam2);

#endif
