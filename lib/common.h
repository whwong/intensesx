/**
 * @file common.h
 * @brief Common include file
 * @author Daniel Szot
 */

#ifndef _COMMON_H_
#define _COMMON_H_

// Common & OS
#include "board.h"
#include "os/FreeRTOS.h"
#include "os/task.h"
#include "os/timers.h"
#include "GenericTypeDefs.h"
#include "retcode.h"
#include "config.h"
#include "assert.h"

// Modules
//#include "lib/log.h"
//#include "lib/graphics/graphics.h"
#include "lib/user/msgDispatcher.h"

#if defined(USE_SERIAL_CONSOLE)
#include "lib/console/serialConsole.h"
#else
#define consoleStart(...) do{}while(0)
#define consoleStop(...) do{}while(0)
#define consoleRegisterCmd(...) do{}while(0)
#define consoleUnregisterCmd(...) do{}while(0)
#endif

// File system
#include "lib/fatfs/ff.h"

// LIBC
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <stddef.h>

#define max(a,b) \
   ({ __typeof__ (a) _a = (a); \
       __typeof__ (b) _b = (b); \
     _a > _b ? _a : _b; })

#define min(a,b) \
   ({ __typeof__ (a) _a = (a); \
       __typeof__ (b) _b = (b); \
     _a < _b ? _a : _b; })

#endif
