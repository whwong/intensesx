/**
 * @file time.h
 * @brief Time library - responsible for provide RTC functionallity
 * Now supports only interfaces to getting time for future use wchich now
 * returns just ticks count.
 * @author Daniel Szot
 */

#ifndef _LIB_TIME_H
#define _LIB_TIME_H

#include "lib/common.h"

UINT32 time();
UINT32 timeISR();

#endif
