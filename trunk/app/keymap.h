/**
 * @file keymap.h
 * @brief Keymap for Intense S project
 * @author Daniel Szot
 */

#ifndef _APP_KEYMAP_H
#define _APP_KEYMAP_H

#include "lib/input/keyCodes.h"
#include "lib/input/inputTask.h"

const struct inputKeyMap intenseKeymap[] =
{
    {0x20dfe01f,V_KEY_LEFT},
    {0x20df02fd,V_KEY_UP},
    {0x20df609f,V_KEY_RIGHT},
    {0x20df827d,V_KEY_DOWN},
    {0x20df08f7,V_KEY_0},
    {0x20df8877,V_KEY_1},
    {0x20df48b7,V_KEY_2},
    {0x20dfc837,V_KEY_3},
    {0x20df28d7,V_KEY_4},
    {0x20dfa857,V_KEY_5},
    {0x20df6897,V_KEY_6},
    {0x20dfe817,V_KEY_7},
    {0x20df18e7,V_KEY_8},
    {0x20df9867,V_KEY_9},
    {0x20df22dd,V_KEY_ENTER},
    {0x20dfc23d,V_KEY_MENU},
    {0x20dfda25,V_KEY_EXIT},
    {0x20df14eb,V_KEY_BACK},
    {0x20df55aa,V_KEY_INFO},
    {0x20df0df2,V_KEY_PLAY},
    {0x20df5da2,V_KEY_PAUSE},
    {0x20df8d72,V_KEY_STOP},
    {0x20dfc03f,V_KEY_V_DOWN},
    {0x20df40bf,V_KEY_V_UP},
};

#define INTENSE_KEYMAP_SIZE (sizeof(intenseKeymap)/sizeof(struct inputKeyMap))

#endif
