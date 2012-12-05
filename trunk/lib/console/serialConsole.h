/**
 * @file serialConsole.c
 * @brief Serial port console using LOG as an output
 * @author Daniel Szot
 */
#ifndef _LIB_CONSOLE_SERIAL_H
#define _LIB_CONSOLE_SERIAL_H

#include "os/FreeRTOS.h"
#include "os/task.h"
#include "GenericTypeDefs.h"
#include "lib/retcode.h"
#include "lib/config.h"
#include "lib/assert.h"
#include "lib/user/msgDispatcher.h"
#include "lib/log.h"
#include "os/task.h"
#include "lib/list.h"
#include "os/semphr.h"

#define CONSOLE_TASK_STACK_SIZE (configMINIMAL_STACK_SIZE+512)
#define CONSOLE_TASK_MSG_QUEUE_SIZE 20
#define CONSOLE_LINE_MAX_LEN 100
#define CONSOLE_COMMAND_MAX_LEN 100
#define CONSOLE_COMMAND_MAX_PARAMS 20

struct consoleCmdHandler
{
    struct list_head list;
    char *command;
    void (*handler)(UINT8 argc, const char* argv[]);
};

retcode consoleStart();
void consoleStop();

struct consoleCmdHandler *
consoleRegisterCmd(char *pCommand, void (*pHandler)(UINT8 argc, const char* argv[]));

void consoleUnregisterCmd(struct consoleCmdHandler *pHandler);

#endif
