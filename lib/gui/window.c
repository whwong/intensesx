/**
 * @file window.c
 * @brief Common window definitions
 * @author Daniel Szot
 */

#include "window.h"

/**
 * Gets focused window, this could be root or child window as well
 * @return Window pointer. NULL if there is no any focused window
 */
struct window *windowGetFocused()
{
    return NULL;
}

/**
 * Gets focused root window. If focused is child window get parent of window
 * and parent of next etc until we catch root window.
 * @return Root window pointer. NULL if there is no any focused window
 */
struct window *windowGetFocusedRoot()
{
    return NULL;
}


