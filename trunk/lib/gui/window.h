/**
 * @file window.h
 * @brief Common window definitions
 * @author Daniel Szot
 */

#ifndef _LIB_GUI_WINDOW_H
#define _LIB_GUI_WINDOW_H

#include "lib/common.h"

struct window
{
    UINT32 wnd;
};

struct window *windowGetFocused();
struct window *windowGetFocusedRoot();

#endif
