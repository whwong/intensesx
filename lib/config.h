/**
 * @file config.h
 * @brief Library configuration file
 * @author Daniel Szot
 */

#ifndef _LIBRARY_CONFIG_H_
#define _LIBRARY_CONFIG_H_

//#define NDEBUG

#if !defined(NDEBUG)
#define LOG_ENABLED
//#define LOG_DEBUG
#define ASSERT_LOGING
#endif

#define DEFAULT_USER_TASK_PRIORITY 1

// You always should provide any console interface
// We need to implement just 4 functions:
// 1. retcode consoleStart();
// 2. void consoleStop();
// 3. struct consoleCmdHandler *
//    consoleRegisterCmd(char *pCommand, void (*pHandler)(UINT8 argc, const char* argv[]));
// 4. void consoleUnregisterCmd(struct consoleCmdHandler *pHandler);
#define USE_SERIAL_CONSOLE

// Use one channel audio output
#define USE_ONE_CHANNEL_AUDIO

#endif
