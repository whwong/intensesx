

#ifndef _WINSTYLE_H_
#define _WINSTYLE_H_

#include "lib/gui/gui.h"

#define WINSTYLE_BLUE       {0x00, 0x84, 0xff, 0xff}
#define WINSTYLE_HL_BLUE    {0x59, 0xaf, 0xff, 0xff}
#define WINSTYLE_BLACK      {0x00, 0x00, 0x00, 0xff}
#define WINSTYLE_WHITE      {0xff, 0xff, 0xff, 0xff}
#define WINSTYLE_BTN_FRAME  {0xaa, 0xaa, 0xaa, 0xff}
#define WINSTYLE_GRAY       {0x55, 0x55, 0x55, 0xff}

enum intenseWinstyleIndexes
{
    WSTL_STATIC_TEXT_SH = 0,
    WSTL_STATIC_TEXT_HL,
    WSTL_STATIC_TEXT_SEL,
    WSTL_STATIC_TEXT_GRY,

    WSTL_BUTTON_SH,
    WSTL_BUTTON_HL,
    WSTL_BUTTON_SEL,
    WSTL_BUTTON_GRY,
};

const struct guiWinStyle intenseWinstyle[] =
{
    // *************************************************************************
    // * STATIC TEXT Style
    // *************************************************************************
    // Static text SH
    {LS_NOSHOW, 0, WINSTYLE_BLACK,   // Left line
     LS_NOSHOW, 0, WINSTYLE_BLACK,   // Top line
     LS_NOSHOW, 0, WINSTYLE_BLACK,   // Right line
     LS_NOSHOW, 0, WINSTYLE_BLACK,   // Bottom line
     CS_SOLID, WINSTYLE_WHITE,    // FG Color
     CS_NOSHOW, WINSTYLE_BLACK},   // BG Color

    // Static text HL
    {LS_NOSHOW, 0, WINSTYLE_BLACK,   // Left line
     LS_NOSHOW, 0, WINSTYLE_BLACK,   // Top line
     LS_NOSHOW, 0, WINSTYLE_BLACK,   // Right line
     LS_NOSHOW, 0, WINSTYLE_BLACK,   // Bottom line
     CS_SOLID, WINSTYLE_WHITE,    // FG Color
     CS_SOLID, WINSTYLE_HL_BLUE},   // BG Color

    // Static text SEL
    {LS_NOSHOW, 0, WINSTYLE_BLACK,   // Left line
     LS_NOSHOW, 0, WINSTYLE_BLACK,   // Top line
     LS_NOSHOW, 0, WINSTYLE_BLACK,   // Right line
     LS_NOSHOW, 0, WINSTYLE_BLACK,   // Bottom line
     CS_SOLID, WINSTYLE_WHITE,    // FG Color
     CS_SOLID, WINSTYLE_BLUE},   // BG Color

    // Static text GRY
    {LS_NOSHOW, 0, WINSTYLE_BLACK,   // Left line
     LS_NOSHOW, 0, WINSTYLE_BLACK,   // Top line
     LS_NOSHOW, 0, WINSTYLE_BLACK,   // Right line
     LS_NOSHOW, 0, WINSTYLE_BLACK,   // Bottom line
     CS_SOLID, WINSTYLE_GRAY,    // FG Color
     CS_NOSHOW, WINSTYLE_BLACK},   // BG Color

    // *************************************************************************
    // * BUTTON Style
    // *************************************************************************
    // Button SH
    {LS_SOLID, 2, WINSTYLE_BTN_FRAME,   // Left line
     LS_SOLID, 1, WINSTYLE_BTN_FRAME,   // Top line
     LS_SOLID, 2, WINSTYLE_BTN_FRAME,   // Right line
     LS_SOLID, 1, WINSTYLE_BTN_FRAME,   // Bottom line
     CS_SOLID, WINSTYLE_WHITE,    // FG Color
     CS_SOLID, WINSTYLE_BLACK},   // BG Color

    // Button HL
    {LS_SOLID, 2, WINSTYLE_BTN_FRAME,   // Left line
     LS_SOLID, 1, WINSTYLE_BTN_FRAME,   // Top line
     LS_SOLID, 2, WINSTYLE_BTN_FRAME,   // Right line
     LS_SOLID, 1, WINSTYLE_BTN_FRAME,   // Bottom line
     CS_SOLID, WINSTYLE_WHITE,    // FG Color
     CS_SOLID, WINSTYLE_HL_BLUE},   // BG Color

    // Button SEL
    {LS_SOLID, 2, WINSTYLE_BTN_FRAME,   // Left line
     LS_SOLID, 1, WINSTYLE_BTN_FRAME,   // Top line
     LS_SOLID, 2, WINSTYLE_BTN_FRAME,   // Right line
     LS_SOLID, 1, WINSTYLE_BTN_FRAME,   // Bottom line
     CS_SOLID, WINSTYLE_WHITE,    // FG Color
     CS_SOLID, WINSTYLE_BLUE},   // BG Color

    // Button GRY
    {LS_SOLID, 2, WINSTYLE_GRAY,   // Left line
     LS_SOLID, 1, WINSTYLE_GRAY,   // Top line
     LS_SOLID, 2, WINSTYLE_GRAY,   // Right line
     LS_SOLID, 1, WINSTYLE_GRAY,   // Bottom line
     CS_SOLID, WINSTYLE_GRAY,    // FG Color
     CS_SOLID, WINSTYLE_BLACK},   // BG Color
};

#endif
