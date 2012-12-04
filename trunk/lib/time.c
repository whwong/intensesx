/**
 * @file time.c
 * @brief Time library - responsible for provide RTC functionallity
 * Now supports only interfaces to getting time for future use wchich now
 * returns just ticks count.
 * @author Daniel Szot
 */

#include "time.h"
#include "lib/common.h"
#include "task.h"

/**
 * Gets timestamp from ISR.
 * Not implemented yet, now returns just os ticks count
 * @return Seconds elapsed from 1 jan 1970
 */
UINT32 timeISR()
{
    return xTaskGetTickCountFromISR();
}

/**
 * Gets timestamp.
 * Not implemented yet, now returns just os ticks count
 * @return Seconds elapsed from 1 jan 1970
 */
UINT32 time()
{
    return xTaskGetTickCount();
}
