/**
 * @file inputEvent.h
 * @brief Input library - it is responsible for getting events from
 * input devices, processing it and passing to listeners
 * @author Daniel Szot
 */

#ifndef _LIB_INPUT_EVENT_H
#define _LIB_INPUT_EVENT_H

#include "lib/common.h"
#include "lib/log.h"
#include "queue.h"
#include "task.h"

#define EVENT_TOUCH     0x01
#define EVENT_KEY       0x02
#define EVENT_CHAR      0x03
#define EVENT_RCU       0x04
#define EVENT_DISK    0x05

#define EVENT_TOUCH_DOWN    0x01
#define EVENT_TOUCH_UP      0x02
#define EVENT_TOUCH_MOVE    0x03

#define EVENT_KEY_DOWN      0x11
#define EVENT_KEY_UP        0x12

#define EVENT_CHAR_GET      0x21

#define EVENT_RCU_CODE_RECEIVED 0x31

#define EVENT_DISK_INSERTED     0x41
#define EVENT_DISK_REMOVED      0x42

#define INPUT_EVENT_TIMEOUT     10

struct inputEvent
{
    UINT8 type;
    UINT32 timestamp;
    UINT8 action;
    UINT32 param1;
    UINT32 param2;
};

struct inputTouchEvent
{
    UINT8 type;
    UINT32 timestamp;
    UINT8 action;

    // param 1
    UINT16 positionX;
    UINT16 positionY;

    // param 2
    INT16 speedX;
    INT16 speedY;
};

struct inputKeyEvent
{
    UINT8 type;
    UINT32 timestamp;
    UINT8 action;

    // param 1
    UINT32 code;

    // param 2
    UINT32 reserved;
};

struct inputRcuEvent
{
    UINT8 type;
    UINT32 timestamp;
    UINT8 action;

    // param 1
    UINT32 code;

    // param 2
    UINT32 param2;
};

struct inputCharEvent
{
    UINT8 type;
    UINT32 timestamp;
    UINT8 action;

    // param 1
    UINT32 character : 8;
    UINT32 reserved : 24;

    UINT32 param2;
};

struct inputDiskEvent
{
    UINT8 type;
    UINT32 timestamp;
    UINT8 action;

    // param 1
    UINT32 devId;

    UINT32 param2;
};

retcode inputEventInit(UINT16 pQueueSize);
retcode inputEventNotify(struct inputEvent *pEvent);

retcode inputTouchEventNotify(UINT8 pAction, UINT16 pPositionX, UINT16 pPositionY,
        INT16 pSpeedX, INT16 pSpeedY);

retcode inputKeyEventNotify(UINT8 pAction, UINT32 pCode, UINT16 pRepeatCount);
retcode inputRcuEventNotify(UINT8 pAction, UINT32 pCode);
retcode inputCharEventNotify(UINT8 pAction, UINT8 pCharacter);
retcode inputDiskEventNotify(UINT8 pAction, UINT32 pDevId);
retcode inputEventNotifyISR(struct inputEvent *pEvent, INT32 *pHiPriorTaskWoken);

retcode inputTouchEventNotifyISR(UINT8 pAction, UINT16 pPositionX, UINT16 pPositionY,
        INT16 pSpeedX, INT16 pSpeedY, INT32 *pHiPriorTaskWoken);

retcode inputKeyEventNotifyISR(UINT8 pAction, UINT32 pCode, UINT16 pRepeatCount,
        INT32 *pHiPriorTaskWoken);

retcode inputRcuEventNotifyISR(UINT8 pAction, UINT32 pCode, INT32 *pHiPriorTaskWoken);

retcode inputCharEventNotifyISR(UINT8 pAction, UINT8 pCharacter, INT32 *pHiPriorTaskWoken);
retcode inputDiskEventNotifyISR(UINT8 pAction, UINT32 pDevId, INT32 *pHiPriorTaskWoken);
void inputEventGet(struct inputEvent *pEvent);
BOOL inputEventPeek(struct inputEvent *pEvent, UINT32 pWaitForMessage);

#endif
