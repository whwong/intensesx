/**
 * @file inputTask.c
 * @brief Input task gets messages from inputEvent and passes it to message
 * dispatcher
 * @author Daniel Szot
 */

#include "lib/common.h"
#include "lib/input/inputEvent.h"
#include "lib/input/inputTask.h"
#include "lib/user/msgDispatcher.h"
#include "os/task.h"
#include "lib/input/keyCodes.h"
#include "lib/gui/window.h"
#include "lib/gui/gui.h"

static xTaskHandle inputTaskHandler;
static const struct inputKeyMap *keymap;
static UINT32 keymapSize;

static void inputTask(void *pvParameters);

/**
 * Init input task and start it
 * @param pKeymap Pointer to user defined keymap table
 * @param pKeymapSize Number of keymap elements
 * @return
 */
retcode inputTaskInit(const struct inputKeyMap *pKeymap, UINT32 pKeymapSize)
{
    UINT32 ret;

    assert(inputTaskHandler == NULL);

    if (inputTaskHandler == NULL)
    {
        ret = xTaskCreate(inputTask, (const signed char * const) "INP",
            INPUT_TASK_STACK_SIZE, NULL, INPUT_TASK_PRIORITY, &inputTaskHandler);

        DEBUG_INPUT("Input task creation result: %d", ret);

        if (ret == pdPASS)
        {
            keymap = pKeymap;
            keymapSize = pKeymapSize;
            return SUCCESS;
        }

        return ERR_NO_MEMMORY;
    }

    return ERR_ALREADY_STARTED;
}

/**
 * Stop and delete input task
 */
inline void inputTaskDelete()
{
    vTaskDelete(inputTaskHandler);
}

/**
 * Suspend input task
 */
inline void inputTaskSuspend()
{
    vTaskSuspend(inputTaskHandler);
}

/**
 * Resume input task
 */
inline void inputTaskResume()
{
    vTaskResume(inputTaskHandler);
}

/**
 * Check that specified Virtual Key is Char key
 * This function ptobably will be moved to msgDispatcher subsystem in future
 * @param pVkey Virtual Key to check
 * @return TRUE if specified Virtual Key is a Char key, FALSE if not
 */
static BOOL isCharKey(UINT32 pVkey)
{
    if (((pVkey >= V_KEY_0) && (pVkey <= V_KEY_9)) ||
        ((pVkey >= 0x41) && (pVkey <= 0x5A)))
    {
        return TRUE;
    }

    return FALSE;
}

/**
 * Get next repetition repeat time. By this funtion you can specify function to
 * calculate next repetition time in function of current repetition count.
 * There is for example specified liner decreasing function and static
 * (selected by defines), you can add your own if you need.
 * @param pRepetition Repetition count
 * @return Repeat time for next key pressed event
 */
static inline UINT32 inputGetRepeatTime(UINT32 pRepetition)
{
#ifdef REPEAT_LINEAR_SPEED_UP
    return max((INT32)INPUT_MIN_REPEAT, INPUT_REPEAT_BASE-((INT32)pRepetition*INPUT_REPEAT_COEF));
#else
    return INPUT_REPEAT;
#endif
}

/**
 * Send key type message. Private shortcut function.
 * @param pMsg Message code
 * @param scan_code Scan code (device dependend)
 * @param repeat_count Repetition count
 */
static void sendKeyMessage(UINT32 pMsg, UINT32 scan_code, UINT16 repeat_count)
{
    UINT32 param2, i;

    for (i = 0; i < keymapSize; i++)
    {
        if (keymap[i].scan_code == scan_code)
        {
            param2 = repeat_count | (scan_code << 16);
            msgPost(guiWindowGetFocused(), pMsg, keymap[i].vkey, param2);
        }
    }
}

/**
 * Input task is responsible for getting input events from all devices,
 * process repeat events and passing all this events as messages to
 * message dispatcher. This task is i/o layer between input devices subsystem
 * and messages subsystem. This is the only one place when you should use
 * functions from this two subsystems mixed together.
 * @param pvParameters Always NULL
 */
static void inputTask(void *pvParameters)
{
    struct inputEvent evn;
    struct inputTouchEvent *tevn = (struct inputTouchEvent*)&evn;
    struct inputKeyEvent *kevn = (struct inputKeyEvent*)&evn;
    struct inputCharEvent *cevn = (struct inputCharEvent*)&evn;
    struct inputDiskEvent *devn = (struct inputDiskEvent*)&evn;
    struct inputRcuEvent *revn = (struct inputRcuEvent*)&evn;

    struct guiWindow *targetWnd = NULL;
    struct guiWindow *pointerWnd = NULL;
    struct guiWindow *oldFocusedWnd = NULL;
    UINT32 cursorAtTargetWnd = FALSE;

    UINT32 lastKey = 0, lastKeyRepeated = 0, lastKeyEventTime = 0;
    UINT32 lastRcu = 0, lastRcuRepeated = 0, lastRcuEventTime = 0;
    UINT32 lastRcuCodeReceived = 0;
    UINT32 timePassed = 0;
    
    while(1)
    {
        if (inputEventPeek(&evn, INPUT_TASK_MAX_SCAN_TIME) == TRUE)
        {
            switch(evn.type)
            {
                case EVENT_KEY:
                    DEBUG_INPUT("Key event:\n\taction: %d\n\tcode: 0x%08x\n\ttime: %d",
                            kevn->action, kevn->code, kevn->timestamp);

                    switch(kevn->action)
                    {
                        case EVENT_KEY_DOWN:
                            sendKeyMessage(MSG_KEYDOWN, kevn->code, 0);
                            lastKey = kevn->code;
                            lastKeyRepeated = 0;
                            lastKeyEventTime = kevn->timestamp;
                            break;

                        case EVENT_KEY_UP:
                            sendKeyMessage(MSG_KEYUP, kevn->code, 0);
                            lastKey = 0;
                            break;
                    }

                    break;

                case EVENT_RCU:
                    DEBUG_INPUT("RCU event:\n\taction: %d\n\tcode: 0x%08x\n\ttime: %d",
                            revn->action, revn->code, revn->timestamp);

                    if (revn->action == EVENT_RCU_CODE_RECEIVED)
                    {                        
                        if (revn->code != lastRcu)
                        {
                            sendKeyMessage(MSG_KEYDOWN, revn->code, 0);
                            lastRcu = revn->code;
                            lastRcuRepeated = 0;
                            lastRcuEventTime = revn->timestamp;
                            lastRcuCodeReceived = revn->timestamp;
                        }
                        else
                        {
                            lastRcuCodeReceived = revn->timestamp;
                        }
                    }

                    break;

                case EVENT_TOUCH:
                    DEBUG_INPUT("Touch event:\n\taction: %d\n\tpos: %d,%d\n\tspd: %d,%d\n\ttime: %d",
                            tevn->action, tevn->positionX, tevn->positionY, tevn->speedX,
                            tevn->speedY, tevn->timestamp);

                    pointerWnd = guiWindowAtXY(tevn->positionX, tevn->positionY);
                    oldFocusedWnd = guiWindowGetFocused();
                    
                    if ((pointerWnd != NULL) && (targetWnd == NULL) && (pointerWnd != oldFocusedWnd))
                    {
                        if (oldFocusedWnd != NULL)
                        {
                            msgPost(oldFocusedWnd, MSG_POINTERLEAVE, 0,
                                ((tevn->positionY << 16) | tevn->positionX));

                            msgPost(oldFocusedWnd, MSG_KILLFOCUS, (UINT32)pointerWnd, 0);
                        }

                        if (pointerWnd != NULL)
                        {
                            msgPost(pointerWnd, MSG_POINTERHOVER, 0,
                                ((tevn->positionY << 16) | tevn->positionX));

                            if (guiWindowSetFocused(pointerWnd))
                                msgPost(pointerWnd, MSG_SETFOCUS, (UINT32)oldFocusedWnd, 0);
                        }
                    }

                    if (targetWnd != NULL)
                    {
                        if (guiXYInRect(tevn->positionX, tevn->positionY, &targetWnd->clientFrame) == TRUE)
                        {
                            if (cursorAtTargetWnd == FALSE)
                            {
                                cursorAtTargetWnd = TRUE;
                                msgPost(targetWnd, MSG_POINTERHOVER, 1,
                                    ((tevn->positionY << 16) | tevn->positionX));
                            }
                        }
                        else
                        {
                            if (cursorAtTargetWnd == TRUE)
                            {
                                cursorAtTargetWnd = FALSE;
                                msgPost(targetWnd, MSG_POINTERLEAVE, 1,
                                    ((tevn->positionY << 16) | tevn->positionX));
                            }
                        }
                    }

                    switch(kevn->action)
                    {
                        case EVENT_TOUCH_DOWN:
                            // 0x0001 Passed as 1st parameter means that we just
                            // press button 1 down (touch the screen in this case)
                            // and move with speed = 0 because we just touched
                            // screen
                            if (pointerWnd != NULL)
                            {
                                targetWnd = pointerWnd;
                                cursorAtTargetWnd = TRUE;
                                msgPost(targetWnd, MSG_POINTERDOWN, 1,
                                        ((tevn->positionY << 16) | tevn->positionX));
                            }
                            break;

                        case EVENT_TOUCH_UP:
                            // Limit speed from -127 to 127 - this should be enought
                            // for all cases. We need to do this to pack all needed
                            // data in two 32 bits params in message system.
                            // If limit is not enought try to div this value in
                            // input system and then multiply it in all user apps
                            if (tevn->speedX > 127)
                                tevn->speedX = 127;
                            else if (tevn->speedX < -127)
                                tevn->speedX = -127;

                            if (tevn->speedY > 127)
                                tevn->speedY = 127;
                            else if (tevn->speedY < -127)
                                tevn->speedY = -127;

                            if (targetWnd != NULL)
                            {
                                // 1st param is only speed because we just release
                                // touch from screen so we set btn 1 as released
                                msgPost(targetWnd, MSG_POINTERUP,
                                        ((tevn->speedY << 24) | ((tevn->speedX & 0xFF) << 16) | 0),
                                        ((tevn->positionY << 16) | tevn->positionX));
                                pointerWnd = targetWnd;
                                targetWnd = NULL;
                            }
                            else if (pointerWnd != NULL)
                            {
                                msgPost(pointerWnd, MSG_POINTERUP,
                                        ((tevn->speedY << 24) | ((tevn->speedX & 0xFF) << 16) | 0),
                                        ((tevn->positionY << 16) | tevn->positionX));
                            }
                            break;

                        case EVENT_TOUCH_MOVE:
                            // Same limitation as above in EVENT_TOUCH_UP
                            if (tevn->speedX > 127)
                                tevn->speedX = 127;
                            else if (tevn->speedX < -127)
                                tevn->speedX = -127;

                            if (tevn->speedY > 127)
                                tevn->speedY = 127;
                            else if (tevn->speedY < -127)
                                tevn->speedY = -127;

                            if (targetWnd != NULL)
                            {
                                msgPost(targetWnd, MSG_POINTERMOVE,
                                        ((tevn->speedY << 24) | ((tevn->speedX & 0xFF) << 16) | 1),
                                        ((tevn->positionY << 16) | tevn->positionX));
                            }
                            else if (pointerWnd != NULL)
                            {
                                // 1st param is spd | 0x0001 because if we are
                                // touching screen we have to had btn1 pressed
                                // btn1 is equal to touch screen
                                msgPost(pointerWnd, MSG_POINTERMOVE,
                                        ((tevn->speedY << 24) | ((tevn->speedX & 0xFF) << 16) | 0),
                                        ((tevn->positionY << 16) | tevn->positionX));
                            }
                            break;
                    }
                    
                    break;

                case EVENT_CHAR:
                    DEBUG_INPUT("Char event:\n\taction: %d\n\tchar: 0x%02x (%d '%c')\n\ttime: %d",
                            cevn->action, cevn->character, cevn->character,
                            cevn->character, cevn->timestamp);

                    if (revn->action == EVENT_CHAR_GET)
                    {
                        msgPost(guiWindowGetFocused(), MSG_CHAR, cevn->character,
                                (0 | (cevn->character << 16)));
                    }

                    break;

                case EVENT_DISK:
                    
                    DEBUG_INPUT("Disk event:\n\taction: %d\n\tdevid: %d\n\ttime: %d",
                            devn->action, devn->devId, devn->timestamp);

                    if (devn->action == EVENT_DISK_INSERTED)
                    {
                        msgPost(NULL, MSG_DISKINSERTED, devn->devId, 0);
                    }
                    else if (devn->action == EVENT_DISK_REMOVED)
                    {
                        msgPost(NULL, MSG_DISKREMOVED, devn->devId, 0);
                    }
                    break;
            }
        }

        // Repeat processing
        timePassed = xTaskGetTickCount() - lastKeyEventTime;
        if (lastKey != 0)
        {
            // Some key is pressed
            if (((lastKeyRepeated == 0) && (timePassed >= INPUT_FIRST_REPEAT)) ||
                ((lastKeyRepeated > 0) && (timePassed >= inputGetRepeatTime(lastRcuRepeated))))
            {
                sendKeyMessage(MSG_KEYDOWN, lastKey, lastKeyRepeated);
                lastKeyRepeated++;
                lastKeyEventTime = xTaskGetTickCount();
            }
        }

        timePassed = xTaskGetTickCount() - lastRcuEventTime;
        if (lastRcu != 0)
        {
            // Some key is pressed
            if ((xTaskGetTickCount() - lastRcuCodeReceived) > 120)
            {
                // If there is no command refresh from RCU driver
                // post MSG_KEYUP
                sendKeyMessage(MSG_KEYUP, lastRcu, 1);
                lastRcu = 0;
            }

            if (((lastRcuRepeated == 0) && (timePassed >= INPUT_FIRST_REPEAT)) ||
                ((lastRcuRepeated > 0) && (timePassed >= inputGetRepeatTime(lastRcuRepeated))))
            {
                lastRcuRepeated++;
                sendKeyMessage(MSG_KEYDOWN, lastRcu, lastRcuRepeated);
                lastRcuEventTime = xTaskGetTickCount();
            }
        }
    }
}

