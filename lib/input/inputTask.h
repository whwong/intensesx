/**
 * @file inputTask.h
 * @brief Input task gets messages from inputEvent and passes it to message
 * dispatcher
 * @author Daniel Szot
 */

#ifndef _LIB_INPUT_TASK_H
#define _LIB_INPUT_TASK_H

#include "lib/common.h"
#include "lib/log.h"
#include "task.h"

#define INPUT_TASK_PRIORITY 1
#if defined(LOG_DEBUG)
#define INPUT_TASK_STACK_SIZE (configMINIMAL_STACK_SIZE+320)
#else
#define INPUT_TASK_STACK_SIZE (configMINIMAL_STACK_SIZE+300)
#endif

#define INPUT_TASK_MAX_SCAN_TIME 10
#define INPUT_FIRST_REPEAT 250

// Define this to config repetition method as linear spped up from defined
// repetition base time to defined repetition time limit by defined coefficient
#define REPEAT_LINEAR_SPEED_UP
#ifdef REPEAT_LINEAR_SPEED_UP
// equation is:
// max(INPUT_MIN_REPEAT, INPUT_REPEAT_BASE-(repetition*INPUT_REPEAT_COEF);
    #define INPUT_MIN_REPEAT    50
    #define INPUT_REPEAT_BASE   150
    #define INPUT_REPEAT_COEF   4
#else
#define INPUT_REPEAT 50
#endif

/** Define this to see some debug information about input task */
#define INPUT_TASK_DEBUG
#ifdef INPUT_TASK_DEBUG
#define DEBUG_INPUT DEBUG
#else
#define DEBUG_INPUT(...) do{}while(0)
#endif

/**
 Single element of keymap table
 scan_code is device key code
 vkey is virtual key which represents this particular scan_code
 */
struct inputKeyMap
{
    UINT32 scan_code;
    UINT32 vkey;
};

retcode inputTaskInit(const struct inputKeyMap *pKeymap, UINT32 pKeymapSize);
inline void inputTaskDelete();
inline void inputTaskSuspend();
inline void inputTaskResume();
static void inputTask(void *pvParameters);

#endif
