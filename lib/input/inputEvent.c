/**
 * @file inputEvent.c
 * @brief Input library - it is responsible for getting events from
 * input devices, processing it and passing to listeners
 * @author Daniel Szot
 */

#include "lib/common.h"
#include "lib/input/inputEvent.h"

xQueueHandle inputEventsQueue;

/**
 * Init input manager, run it always before start any input device drivers
 * @param pQueueSize Input queue size
 * @return ERR_NO_MEMMORY when can't create input queue
 * SUCCESS if queue created successfully
 */
retcode inputEventInit(UINT16 pQueueSize)
{
    assert(pQueueSize != 0);
    assert(sizeof(struct inputEvent) == sizeof(struct inputTouchEvent));
    assert(sizeof(struct inputEvent) == sizeof(struct inputKeyEvent));
    assert(sizeof(struct inputEvent) == sizeof(struct inputCharEvent));

    inputEventsQueue = xQueueCreate( pQueueSize,
            (unsigned portBASE_TYPE) sizeof(struct inputEvent));

    if (inputEventsQueue == NULL)
        return ERR_NO_MEMMORY;

    vQueueAddToRegistry(inputEventsQueue, "inp");

    return SUCCESS;
}

/**
 * Place event in queue
 * @param pEvent Event to be placed
 */
retcode inputEventNotify(struct inputEvent *pEvent)
{
    assert(pEvent != NULL);

    if(xQueueSend(inputEventsQueue, pEvent, INPUT_EVENT_TIMEOUT))
        return SUCCESS;
    else
        return ERR_QUEUE_FULL;
}

/**
 * Place touch event in queue
 * @param pAction Event action
 * @param pPositionX Touch X position
 * @param pPositionY Touch Y position
 * @param pSpeedX Touch X speed
 * @param pSpeedY Touch Y speed
 */
retcode inputTouchEventNotify(UINT8 pAction, UINT16 pPositionX, UINT16 pPositionY,
        INT16 pSpeedX, INT16 pSpeedY)
{
    struct inputTouchEvent event;

    event.action = pAction;
    event.positionX = pPositionX;
    event.positionY = pPositionY;
    event.speedX = pSpeedX;
    event.speedY = pSpeedY;
    event.type = EVENT_TOUCH;
    event.timestamp = time();

    return inputEventNotify((struct inputEvent*)&event);
}

/**
 * Place touch event in queue
 * @param pAction Event action
 * @param pCode Key code
 * @param pRepeatCount Key repetition number
 */
retcode inputKeyEventNotify(UINT8 pAction, UINT32 pCode, UINT16 pRepeatCount)
{
    struct inputKeyEvent event;

    event.action = pAction;
    event.code = pCode;
    event.type = EVENT_KEY;
    event.timestamp = time();

    return inputEventNotify((struct inputEvent*)&event);
}

/**
 * Place RCU event in queue
 * @param pAction Event action
 * @param pCode Received 32 bit code
 */
retcode inputRcuEventNotify(UINT8 pAction, UINT32 pCode)
{
    struct inputRcuEvent event;

    event.action = pAction;
    event.code = pCode;
    event.param2 = 0;
    event.type = EVENT_RCU;
    event.timestamp = time();

    return inputEventNotify((struct inputEvent*)&event);
}

/**
 * Place char event in queue
 * @param pAction Event action
 * @param pCharacter Character code
 */
retcode inputCharEventNotify(UINT8 pAction, UINT8 pCharacter)
{
    struct inputCharEvent event;

    event.action = pAction;
    event.character = pCharacter;
    event.type = EVENT_CHAR;
    event.timestamp = time();

    return inputEventNotify((struct inputEvent*)&event);
}

/**
 * Place disk event in queue
 * @param pAction Event action
 * @param pCharacter Character code
 */
retcode inputDiskEventNotify(UINT8 pAction, UINT32 pDevId)
{
    struct inputDiskEvent event;

    event.action = pAction;
    event.devId = pDevId;
    event.type = EVENT_DISK;
    event.timestamp = time();

    return inputEventNotify((struct inputEvent*)&event);
}

/**
 * Place event in queue (from ISR)
 * @param pEvent Event to be placed
 */
retcode inputEventNotifyISR(struct inputEvent *pEvent, INT32 *pHiPriorTaskWoken)
{
    assert(pEvent != NULL);

    if(xQueueSendFromISR(inputEventsQueue, pEvent, pHiPriorTaskWoken))
        return SUCCESS;
    else
        return ERR_QUEUE_FULL;
}

/**
 * Place touch event in queue from ISR
 * @param pAction Event action
 * @param pPositionX Touch X position
 * @param pPositionY Touch Y position
 * @param pSpeedX Touch X speed
 * @param pSpeedY Touch Y speed
 */
retcode inputTouchEventNotifyISR(UINT8 pAction, UINT16 pPositionX, UINT16 pPositionY,
        INT16 pSpeedX, INT16 pSpeedY, INT32 *pHiPriorTaskWoken)
{
    struct inputTouchEvent event;

    event.action = pAction;
    event.positionX = pPositionX;
    event.positionY = pPositionY;
    event.speedX = pSpeedX;
    event.speedY = pSpeedY;
    event.type = EVENT_TOUCH;
    event.timestamp = timeISR();

    return inputEventNotifyISR((struct inputEvent*)&event, pHiPriorTaskWoken);
}

/**
 * Place touch event in queue from ISR
 * @param pAction Event action
 * @param pCode Key code
 * @param pRepeatCount Key repetition number
 */
retcode inputKeyEventNotifyISR(UINT8 pAction, UINT32 pCode, UINT16 pRepeatCount,
        INT32 *pHiPriorTaskWoken)
{
    struct inputKeyEvent event;

    event.action = pAction;
    event.code = pCode;
    event.type = EVENT_KEY;
    event.timestamp = timeISR();

    return inputEventNotifyISR((struct inputEvent*)&event, pHiPriorTaskWoken);
}

/**
 * Place RCU event in queue from ISR
 * @param pAction Event action
 * @param pCode Received 32 bit code
 */
retcode inputRcuEventNotifyISR(UINT8 pAction, UINT32 pCode, INT32 *pHiPriorTaskWoken)
{
    struct inputRcuEvent event;

    event.action = pAction;
    event.code = pCode;
    event.param2 = 0;
    event.type = EVENT_RCU;
    event.timestamp = timeISR();

    return inputEventNotifyISR((struct inputEvent*)&event, pHiPriorTaskWoken);
}

/**
 * Place char event in queue from ISR
 * @param pAction Event action
 * @param pCharacter Character code
 */
retcode inputCharEventNotifyISR(UINT8 pAction, UINT8 pCharacter, INT32 *pHiPriorTaskWoken)
{
    struct inputCharEvent event;

    event.action = pAction;
    event.character = pCharacter;
    event.type = EVENT_CHAR;
    event.timestamp = timeISR();

    return inputEventNotifyISR((struct inputEvent*)&event, pHiPriorTaskWoken);
}

/**
 * Place disk event in queue from ISR
 * @param pAction Event action
 * @param pCharacter Character code
 */
retcode inputDiskEventNotifyISR(UINT8 pAction, UINT32 pDevId, INT32 *pHiPriorTaskWoken)
{
    struct inputDiskEvent event;

    event.action = pAction;
    event.devId = pDevId;
    event.type = EVENT_DISK;
    event.timestamp = timeISR();

    return inputEventNotifyISR((struct inputEvent*)&event, pHiPriorTaskWoken);
}

/**
 * Get event from queue and wait until any event will be avaliable to receive
 * If you wouldn't like to wait use inputEventPeek
 *
 * @param pEvent Buffer for event to be received
 */
void inputEventGet(struct inputEvent *pEvent)
{
    BOOL received;

    assert(pEvent != NULL);

    do
    {
        received = xQueueReceive(inputEventsQueue, pEvent, portMAX_DELAY);
    }
    while(received == FALSE);
}

/**
 * Get event from queue but do not wait if there is no events in queue
 * If you would like to wait until any event will be avaliable to get
 * use inputEventGet
 *
 * @param pEvent Buffer for event to be received
 * @param pWaitForMessage Wait for message time in ms
 * @return TRUE if event was received, FALSE when no events in queue
 */
BOOL inputEventPeek(struct inputEvent *pEvent, UINT32 pWaitForMessage)
{
    assert(pEvent != NULL);
    return xQueueReceive(inputEventsQueue, pEvent, pWaitForMessage);
}
