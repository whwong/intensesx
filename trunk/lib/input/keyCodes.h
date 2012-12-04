/**
 * @file keyCodes.h
 * @brief Definition of all key codes in system
 * @author Daniel Szot
 */

#ifndef _LIB_KEYCODES_H
#define _LIB_KEYCODES_H

#define V_KEY_NULL  0x0000

// Number keys
#define V_KEY_0     0x0030
#define V_KEY_1     0x0031
#define V_KEY_2     0x0032
#define V_KEY_3     0x0033
#define V_KEY_4     0x0034
#define V_KEY_5     0x0035
#define V_KEY_6     0x0036
#define V_KEY_7     0x0037
#define V_KEY_8     0x0038
#define V_KEY_9     0x0039

// 0x41 - 0x5A Reserved for char keys A - Z

// Navigation
#define V_KEY_LEFT  0x0025
#define V_KEY_RIGHT 0x0027
#define V_KEY_UP    0x0026
#define V_KEY_DOWN  0x0028
#define V_KEY_ENTER 0x000D
#define V_KEY_CLEAR 0x000C

#define V_KEY_P_UP      0x0021
#define V_KEY_P_DOWN    0x0022

#define V_KEY_POWER     0x005F
#define V_KEY_MENU      0x0012

#define V_KEY_EXIT      0x0100
#define V_KEY_BACK      0x0125
#define V_KEY_INFO      0x0126

#define V_KEY_MUTE      0x0101
#define V_KEY_PAUSE     0x0102
#define V_KEY_TVRADIO   0x0103
#define V_KEY_RECALL    0x0104
#define V_KEY_AUDIO     0x0105

// Extended navigation
#define V_KEY_C_UP      0x0106
#define V_KEY_C_DOWN    0x0107
#define V_KEY_V_UP      0x0108
#define V_KEY_V_DOWN    0x0109
#define V_KEY_F_UP      0x010A
#define V_KEY_F_DOWN    0x010B

#define V_KEY_EPG       0x010C
#define V_KEY_TEXT      0x010D
#define V_KEY_SUBTITLE  0x010E

#define V_KEY_FAV   0x010F
#define V_KEY_SLEEP 0x0110
#define V_KEY_FIND  0x0111

// Color keys
#define V_KEY_RED       0x0112
#define V_KEY_GREEN     0x0113
#define V_KEY_YELLOW    0x0114
#define V_KEY_BLUE      0x0115
#define V_KEY_WHITE     0x0116

// Recording
#define V_KEY_RECORD    0x0117
#define V_KEY_STOP      0x0118
#define V_KEY_TSHIFT    0x0119
#define V_KEY_PLAY      0x011A
#define V_KEY_SEARCH    0x011B
#define V_KEY_PREV      0x011C
#define V_KEY_NEXT      0x011D
#define V_KEY_FB        0x011E
#define V_KEY_FF        0x011F
#define V_KEY_B_SLOW    0x0120
#define V_KEY_STEP      0x0121
#define V_KEY_SLOW      0x0122
#define V_KEY_TVMODE    0x0123
#define V_KEY_HDMI      0x0124

#endif
