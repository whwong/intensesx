/**
 * @file guidefs.h
 * @brief Common GUI types and definitions
 * @author Daniel Szot
 */

#ifndef _LIB_GUI_DEFS_H_
#define _LIB_GUI_DEFS_H_

struct guiPoint
{
    UINT16 x;
    UINT16 y;
};

struct guiRect
{
    UINT16 l;
    UINT16 t;
    UINT16 w;
    UINT16 h;
};

struct guiColorStyle
{
    // Normal showing element winstyle index
    UINT8 shIdx;
    // Highlighted element winstyle index
    UINT8 hlIdx;
    // Selected element winstyle index
    UINT8 selIdx;
    // Diisabled element winstyle index
    UINT8 gryIdx;
};

struct guiColor
{
    UINT8 r;
    UINT8 g;
    UINT8 b;
    UINT8 a;
};

// Line styles definitions
#define LS_NOSHOW   0x00
#define LS_SOLID    0x01

// Client color styles
#define CS_NOSHOW   0x0000
#define CS_SOLID    0x0001

struct guiWinStyle
{
    // Left line
    UINT8 leftLineStyle;
    UINT8 leftLineWidth;
    struct guiColor leftLineColor;

    // Top line
    UINT8 topLineStyle;
    UINT8 topLineWidth;
    struct guiColor topLineColor;

    // Right line
    UINT8 rightLineStyle;
    UINT8 rightLineWidth;
    struct guiColor rightLineColor;

    // Bottom line
    UINT8 bottomLineStyle;
    UINT8 bottomLineWidth;
    struct guiColor bottomLineColor;

    // Client colors
    UINT16 fgStyle;
    struct guiColor fgColor; // Frontground color
    UINT16 bgStyle;
    struct guiColor bgColor; // Background color
};

#endif
