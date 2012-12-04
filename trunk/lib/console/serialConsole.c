/**
 * @file serialConsole.c
 * @brief Serial port console using LOG as an output
 * @author Daniel Szot
 */

#include "lib/console/serialConsole.h"

static void serialConsoleTask(void *pvParameters);
static xTaskHandle consoleTaskHandler;
static UINT8 exitFlag = 0;

static char cbuf[CONSOLE_LINE_MAX_LEN];
static const char *argv[CONSOLE_COMMAND_MAX_PARAMS];
static UINT8 argc = 0;
static UINT8 cidx = 0;
static UINT8 argvidx = 0;

/** Define this to see some debug information about serial console */
#define SERIAL_CONSOLE_DEBUG
#if defined(SERIAL_CONSOLE_DEBUG)
#define DEBUG_CONSOLE DEBUG
#else
#define DEBUG_CONSOLE(...) do{}while(0)
#endif

/* COMMAND HANDLERS SUPPORT */
static struct list_head cmdHandlersList;
static xSemaphoreHandle consoleMutex = NULL;

/**
 * Register command on console commands list
 * @param pCommand Command name
 * @param pHandler Command handler
 * @return Pointer to structure which you can use to unregister handler by
 * calling consoleUnregisterCmd
 */
struct consoleCmdHandler *
consoleRegisterCmd(char *pCommand, void (*pHandler)(UINT8 argc, const char* argv[]))
{
    struct consoleCmdHandler *h;
    
    assert(consoleMutex != NULL);
    assert(pHandler != NULL);
    assert(pCommand != NULL);

    h = pvPortMalloc(sizeof(struct consoleCmdHandler));
    if (h == NULL)
        return NULL;

    h->command = pvPortMalloc(strlen(pCommand)+1);

    if (h->command == NULL)
    {
        vPortFree(h);
        return NULL;
    }

    strcpy(h->command, pCommand);
    h->handler = pHandler;

    while (xSemaphoreTake(consoleMutex, portMAX_DELAY) != pdTRUE);
    list_add(&(h->list), &(cmdHandlersList));
    xSemaphoreGive(consoleMutex);

    return h;
}

/**
 * Delete command from console commands list
 * @param pHandler command handler returned by consoleRegisterCmd
 */
void consoleUnregisterCmd(struct consoleCmdHandler *pHandler)
{
    assert(pHandler != NULL);

    while (xSemaphoreTake(consoleMutex, portMAX_DELAY) != pdTRUE);
    list_del(&(pHandler->list));
    xSemaphoreGive(consoleMutex);

    vPortFree(pHandler->command);
    vPortFree(pHandler);
}

/* DEFAULT COMMANDS */
static void consoleCmdExit(UINT8 argc, const char *argv[])
{
    if (argc == 1)
    {
        LOG("\tStopping serial console...");
        consoleStop();
    }
    else if (argc == 2 && (strcmp(argv[1], "-h") == 0))
    {
        LOG("\tThis command do not get any parameters yet.");
        LOG("\tYou can just delete console task by typing 'exit'");
    }
    else
    {
        LOG("\tusage: 'exit'");
        LOG("\tType 'exit -h' to get help about this command");
    }
}

void consoleCmdHelp(UINT8 argc, const char *argv[])
{
    struct consoleCmdHandler *h;
    
    LOG("\tThis is simple serial console version 0.0.1");
    LOG("\tYou can use simple commands separated by whitespaces");
    LOG("\tQuotation marks are not parse yet.");
    LOG("\tAuthor: Daniel Szot (szotdaniel@gmail.com)");
    LOG("");
    LOG("\tAvaliable commands:");

    while (xSemaphoreTake(consoleMutex, portMAX_DELAY) != pdTRUE);
    list_for_each_entry(h, &cmdHandlersList, list)
    {
        LOG("\t\t%s", h->command);
    }
    xSemaphoreGive(consoleMutex);
}

/* CONSOLE SUPPORT*/

/**
 * Wake serial console to work :)
 * @return Return code
 * @note Console always uses default LOG device as an output
 */
retcode consoleStart()
{
    UINT32 ret;

    assert(consoleTaskHandler == NULL);

    if (consoleTaskHandler == NULL)
    {
        if (consoleMutex == NULL)
            consoleMutex = xSemaphoreCreateMutex();

        if (consoleMutex == NULL)
        {
            return ERR_NO_MEMMORY;
        }
        
        ret = xTaskCreate(serialConsoleTask, (const signed char * const) "STT",
            CONSOLE_TASK_STACK_SIZE, NULL, DEFAULT_USER_TASK_PRIORITY, &consoleTaskHandler);

        DEBUG_CONSOLE("Console task creation result: %d, task: %p", ret, consoleTaskHandler);

        if (ret == pdPASS)
        {
            exitFlag = 0;
            cidx = 0;
            memset(&cbuf[0], 0, CONSOLE_LINE_MAX_LEN);

            while (xSemaphoreTake(consoleMutex, portMAX_DELAY) != pdTRUE);
            INIT_LIST_HEAD(&cmdHandlersList);
            xSemaphoreGive(consoleMutex);

            consoleRegisterCmd("exit", &consoleCmdExit);
            consoleRegisterCmd("help", &consoleCmdHelp);

            DONE("Serial console created");
            return SUCCESS;
        }

        vSemaphoreDelete(consoleMutex);
        return ERR_NO_MEMMORY;
    }

    return ERR_ALREADY_STARTED;
}

/**
 * Stop serial console task
 */
void consoleStop()
{
    msgPostTask(consoleTaskHandler, MSG_QUIT, 0, 0);
}

/**
 * Check that character "c" is blank
 * @param c Character to check
 * @return TRUE - blank, FALSE - normal character
 */
static inline BOOL isBlank(char c)
{
    return ((c == ' ') || (c == '\t')) ? TRUE : FALSE;
}

/**
 * Skip blank characters in pLine buffer starting from pStartIdx index. It
 * takes start index by 2nd param and look for next normal character. After
 * find normal (non-blank) char, it returns index of this char.
 * @param pLine Buffer to scan
 * @param pStartIdx Start index
 * @return Index of next non-blank character
 */
static UINT32 skipBlank(char *pLine, UINT32 pStartIdx)
{
    UINT32 i = pStartIdx;
    while((pLine[i] != '\0') && (isBlank(pLine[i]) == TRUE))
    {
        i++;
    }

    return i;
}

/**
 * Gets pointer to the next parameter in command line buffer
 * @return Pointer to next param
 */
static const char *getNextParamPointer()
{
    UINT32 startIdx;
    assert(cbuf != NULL);

    argvidx = skipBlank(cbuf, argvidx);
    startIdx = argvidx;

    if (cbuf[argvidx] == '\0')
        return NULL;
    
    while(cbuf[argvidx] != '\0')
    {
        if (isBlank(cbuf[argvidx]) == TRUE)
            break;
        
        argvidx++;
    }

    if (cbuf[argvidx] != '\0')
        cbuf[argvidx++] = '\0';
    
    return &cbuf[startIdx];
}

/**
 * Parse serial console command
 * This functions sets argc and argv variables appropriate for given
 * command and parameters. After that if founds command handlers in
 * handlers list and executes appropriate.
 */
static void serialConsoleCommand()
{
    UINT32 commandsExecuted = 0;
    const char **argp;
    struct consoleCmdHandler *h;

    argc = 0;
    argp = argv;
    argvidx = 0;
    while ((*argp++ = getNextParamPointer()))
        argc++;

#if defined(SERIAL_CONSOLE_DEBUG)
    UINT8 i;
    for (i = 0; i < argc; i++)
    {
        DEBUG_CONSOLE("\t Cmd param %d: %s", i, argv[i]);
    }
#endif

    while (xSemaphoreTake(consoleMutex, portMAX_DELAY) != pdTRUE);
    list_for_each_entry(h, &cmdHandlersList, list)
    {
        xSemaphoreGive(consoleMutex);
        if (strcmp(h->command, argv[0]) == 0)
        {
            (h->handler)(argc, argv);
            commandsExecuted++;
        }
        while (xSemaphoreTake(consoleMutex, portMAX_DELAY) != pdTRUE);
    }
    xSemaphoreGive(consoleMutex);

    if (commandsExecuted == 0)
    {
        LOG("\tUnknown command. Type 'help' to see complete commands list.");
    }
}

/**
 * Process return key pressed event
 */
static inline void processRetun()
{
    LOG_PUTC('\n');
    if (cidx > 0)
    {
        cbuf[cidx] = 0;
        cidx = 0;
        serialConsoleCommand();
        DEBUG_CONSOLE("Serial console command received: %s", cbuf);
    }
}

/**
 * Process backspace key pressed event
 */
static inline void processBackspace()
{
    if (cidx > 0)
    {
        cbuf[cidx] = '\0';
        cidx--;
        LOG_PUTC(0x7f);
    }
}

/**
 * Process any other key pressed event
 */
static inline void processChar(char c)
{
    if (cidx < (CONSOLE_LINE_MAX_LEN-1))
    {
        cbuf[cidx] = c;
        cidx++;
        LOG_PUTC(c);
    }
    else
    {
        DEBUG_CONSOLE("Serial console command too long. "
                "Max command lenght is: %d", CONSOLE_LINE_MAX_LEN);
    }
}

/**
 * Serial taks main function. It is responsible for parsing commands and
 * executing appropriate handlers.
 * @param pvParameters
 */
static void serialConsoleTask(void *pvParameters)
{
    struct msgListener *list;
    struct msg m;

    list = msgListenerCreate(CONSOLE_TASK_MSG_QUEUE_SIZE);
    while(msgListenerGet(list, &m, NULL, 0, 0))
    {
        if (m.message == MSG_CHAR)
        {
            switch(m.param1)
            {
                case '\r':
                case '\n':
                    processRetun();
                    break;

                case '\b':
                // DEL
                case 0x7f:
                    processBackspace();
                    break;

                default:
                    processChar((char)m.param1);

            }
        }
    }

    msgListenerDelete(list);
    DONE("Serial console deleted");
    vTaskDelete(consoleTaskHandler);
}
