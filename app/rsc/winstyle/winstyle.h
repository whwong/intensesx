

#ifndef _WINSTYLE_H_
#define _WINSTYLE_H_

#include "lib/gui/gui.h"

#define WINSTYLE_BLUE       {0x00, 0x84, 0xff, 0xff}
#define WINSTYLE_HL_BLUE    {0x59, 0xaf, 0xff, 0xff}
#define WINSTYLE_BG_BLUE    {0x25, 0x3a, 0x47, 0xff}
#define WINSTYLE_BLACK      {0x00, 0x00, 0x00, 0xff}
#define WINSTYLE_WHITE      {0xff, 0xff, 0xff, 0xff}
#define WINSTYLE_BTN_FRAME  {0x7f, 0x7f, 0x7f, 0xff}
#define WINSTYLE_PB_FRAME   {0xcc, 0xcc, 0xcc, 0xff}
#define WINSTYLE_GRAY       {0x55, 0x55, 0x55, 0xff}
#define WINSTYLE_GRAYHL     {0x55, 0x77, 0x99, 0xff}

enum intenseWinstyleIndexes
{
    WSTL_USER = WSTL_LAST_DEFAULT,

    // Do not change nothing above
    // User styles defined below
};

const struct guiWinStyle intenseWinstyle[] =
{
    // *************************************************************************
    // * WINDOW Style
    // *************************************************************************
    // Window SH
    {LS_NOSHOW, 0, WINSTYLE_BLACK,   // Left line
     LS_NOSHOW, 0, WINSTYLE_BLACK,   // Top line
     LS_NOSHOW, 0, WINSTYLE_BLACK,   // Right line
     LS_NOSHOW, 0, WINSTYLE_BLACK,   // Bottom line
     CS_SOLID, WINSTYLE_WHITE,    // FG Color
     CS_SOLID, WINSTYLE_BLACK},   // BG Color

    // Window HL
    {CS_SOLID, 1, WINSTYLE_BTN_FRAME,   // Left line
     CS_SOLID, 2, WINSTYLE_BTN_FRAME,   // Top line
     CS_SOLID, 3, WINSTYLE_BTN_FRAME,   // Right line
     CS_SOLID, 4, WINSTYLE_BTN_FRAME,   // Bottom line
     CS_SOLID, WINSTYLE_WHITE,    // FG Color
     CS_SOLID, WINSTYLE_GRAYHL},   // BG Color

    // *************************************************************************
    // * STATIC TEXT Style
    // *************************************************************************
    // Static text SH
    {LS_NOSHOW, 0, WINSTYLE_BLACK,   // Left line
     LS_NOSHOW, 0, WINSTYLE_BLACK,   // Top line
     LS_NOSHOW, 0, WINSTYLE_BLACK,   // Right line
     LS_NOSHOW, 0, WINSTYLE_BLACK,   // Bottom line
     CS_SOLID, WINSTYLE_WHITE,    // FG Color
     CS_SOLID, WINSTYLE_BLACK},   // BG Color

    // Static text GRY
    {LS_NOSHOW, 0, WINSTYLE_BLACK,   // Left line
     LS_NOSHOW, 0, WINSTYLE_BLACK,   // Top line
     LS_NOSHOW, 0, WINSTYLE_BLACK,   // Right line
     LS_NOSHOW, 0, WINSTYLE_BLACK,   // Bottom line
     CS_SOLID, WINSTYLE_GRAY,    // FG Color
     CS_SOLID, WINSTYLE_BLACK},   // BG Color

    // *************************************************************************
    // * BUTTON Style
    // *************************************************************************
    // Button SH
    {LS_SOLID, 1, WINSTYLE_BTN_FRAME,   // Left line
     LS_SOLID, 1, WINSTYLE_BTN_FRAME,   // Top line
     LS_SOLID, 1, WINSTYLE_BTN_FRAME,   // Right line
     LS_SOLID, 1, WINSTYLE_BTN_FRAME,   // Bottom line
     CS_SOLID, WINSTYLE_WHITE,    // FG Color
     CS_SOLID, WINSTYLE_BLACK},   // BG Color

    // Button HL
    {LS_SOLID, 3, WINSTYLE_WHITE,   // Left line
     LS_SOLID, 1, WINSTYLE_WHITE,   // Top line
     LS_SOLID, 3, WINSTYLE_WHITE,   // Right line
     LS_SOLID, 1, WINSTYLE_WHITE,   // Bottom line
     CS_SOLID, WINSTYLE_WHITE,    // FG Color
     CS_SOLID, WINSTYLE_HL_BLUE},   // BG Color

    // Button SEL
    {LS_SOLID, 0, WINSTYLE_BTN_FRAME,   // Left line
     LS_SOLID, 0, WINSTYLE_BTN_FRAME,   // Top line
     LS_SOLID, 0, WINSTYLE_BTN_FRAME,   // Right line
     LS_SOLID, 0, WINSTYLE_BTN_FRAME,   // Bottom line
     CS_SOLID, WINSTYLE_WHITE,    // FG Color
     CS_SOLID, WINSTYLE_BLUE},   // BG Color

    // Button GRY
    {LS_SOLID, 1, WINSTYLE_GRAY,   // Left line
     LS_SOLID, 1, WINSTYLE_GRAY,   // Top line
     LS_SOLID, 1, WINSTYLE_GRAY,   // Right line
     LS_SOLID, 1, WINSTYLE_GRAY,   // Bottom line
     CS_SOLID, WINSTYLE_GRAY,    // FG Color
     CS_SOLID, WINSTYLE_BLACK},   // BG Color

    // *************************************************************************
    // * PROGRESS BAR Style
    // *************************************************************************
    // Progress Bar SH
    {LS_SOLID, 1, WINSTYLE_PB_FRAME,   // Left line
     LS_SOLID, 1, WINSTYLE_PB_FRAME,   // Top line
     LS_SOLID, 1, WINSTYLE_PB_FRAME,   // Right line
     LS_SOLID, 1, WINSTYLE_PB_FRAME,   // Bottom line
     CS_SOLID, WINSTYLE_WHITE,    // FG Color
     CS_SOLID, WINSTYLE_BG_BLUE},   // BG Color

    // Progress Bar - Bar style
    {LS_NOSHOW, 0, WINSTYLE_BTN_FRAME,   // Left line
     LS_NOSHOW, 0, WINSTYLE_BTN_FRAME,   // Top line
     LS_NOSHOW, 0, WINSTYLE_BTN_FRAME,   // Right line
     LS_NOSHOW, 0, WINSTYLE_BTN_FRAME,   // Bottom line
     LS_NOSHOW, WINSTYLE_WHITE,    // FG Color
     CS_SOLID, WINSTYLE_BLUE},   // BG Color
};

#endif
