/**
 * @file log.c
 * @author Daniel Szot
 */

#include "hal/hld/hld.h"
#include "hal/hld/uart.h"
#include "lib/common.h"
#include "lib/log.h"
#include "semphr.h"

#if defined(LOG_ENABLED)

static struct hldCharDevice *logCharDevice;
static xSemaphoreHandle logMutex = NULL;

inline void __attribute__ ((always_inline))
logSetUartDevice(struct hldCharDevice *pCharDev)
{
    logCharDevice = pCharDev;

    if (logMutex == NULL)
    {
        logMutex = xSemaphoreCreateMutex();
    }
}

void log_print(char * pBuf)
{
    UINT32 i = 0;
    
    if (logCharDevice != NULL)
    {
        while (pBuf[i] != '\0')
        {
            logCharDevice->write(logCharDevice, pBuf[i], LOG_MAX_LINE_DELAY);
            i++;
        }
    }
}

void log_putc(char c)
{
    if (logCharDevice != NULL)
    {
        logCharDevice->write(logCharDevice, c, LOG_MAX_LINE_DELAY);
    }
}

void log_output(enum logMessageType pType, const char* pFormat, ...)
{
    static char buf[LOG_MAX_CHARS_PER_LINE + 1];

    if (logMutex == NULL)
    {
        return;
    }
    else if (xSemaphoreTake(logMutex, LOG_MAX_LINE_DELAY))
    {
        va_list paramList;
        va_start(paramList, pFormat);

        switch(pType)
        {
            case LOG_ERROR:
                log_print("[ERROR]: ");
                break;

            case LOG_WARNING:
                log_print("[WARNING]: ");
                break;

            case LOG_SUCCESS:
                log_print("[SUCCESS]: ");
                break;

            case LOG_DEBUG_INFO:
                log_print("[DEBUG]: ");
                break;

            default:
                break;
        }

        vsnprintf(buf, LOG_MAX_CHARS_PER_LINE, pFormat, paramList);

        log_print(buf);
        if (logCharDevice != NULL)
            logCharDevice->write(logCharDevice, '\n', LOG_MAX_LINE_DELAY);

        va_end(paramList);
        xSemaphoreGive(logMutex);
    }
}

#endif
