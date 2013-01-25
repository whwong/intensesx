/**
 * @file graphics.h
 * @brief Graphics library
 * @author Daniel Szot
 */

#ifndef _GRAPHICS_LIBRARY_H
#define _GRAPHICS_LIBRARY_H

#define GRAPH_TESTS

#include "lib/common.h"
#include "hal/hld/lcd.h"

extern struct hldLcdDevice *graphLcdDev;

// OS Porting
#define graphDelayMs vTaskDelay

#define GRAPH_FONT_ORIENT_HOR 0
#define GRAPH_FONT_ORIENT_VER 1

// Font style
// mutually exclusive
#define FS_ALIGN_MASK     0x00000003
#define FS_ALIGN_LEFT     0x00000000
#define FS_ALIGN_CENTER   0x00000001
#define FS_ALIGN_RIGHT    0x00000002

// mutually exclusive
#define FS_VALIGN_MASK    0x0000000C
#define FS_VALIGN_CENTER  0x00000000
#define FS_VALIGN_TOP     0x00000004
#define FS_VALIGN_BOTTOM  0x00000008

// Mid colors are calculated only once while rendering each character. This is ideal for rendering text over a constant background.
#define ANTIALIAS_OPAQUE        0

// Mid values are calculated for every necessary pixel. This feature is useful when rendering text over an image
// or when the background is not one flat color.
#define ANTIALIAS_TRANSLUCENT   1

#define ANTIALIAS_METHOD ANTIALIAS_OPAQUE

// Common graphics library funkctions
void graphSetLcdDevice(struct hldLcdDevice *pLcdDev);
retcode graphSetDrawingColor(UINT8 pA, UINT8 pR, UINT8 pG, UINT8 pB);
retcode graphClearScreen();

// Drawing primitives
retcode graphDrawLine(UINT16 pX1, UINT16 pY1, UINT16 pX2, UINT16 pY2);
retcode graphDrawRect(UINT16 pX1, UINT16 pY1, UINT16 pX2, UINT16 pY2);
retcode graphDrawCircle(UINT16 pX, UINT16 pY, UINT16 pRadius);
retcode graphDrawRoundedRect();

// Tests of features
#ifdef GRAPH_TESTS
void graphTestDrawEdgeMesh();
void graphTestDrawRand1kLines();
void graphTestDrawRand10kLines();
void graphTestDrawRand30Circles();
#endif

enum graphFontType
{
    GRAPH_FONT_BITMAP,
    GRAPH_FONT_OUTLINE, // Not supported yet
};

struct graphFont
{
    // Font type
    enum graphFontType type;

    // Font image address
    const UINT8 *address;
};

retcode graphDrawChar(UINT16 pX, UINT16 pY, UINT16 *pCharWidth,
        UINT16 pChar, struct graphFont *pFont);
void graphDrawText(UINT16 pX, UINT16 pY, UINT16 pW, UINT16 pH,
        char * pText, struct graphFont *pFont, UINT32 pStyle);

void graphGetTextSize(UINT16 *pW, UINT16 *pH, char * pText, struct graphFont *pFont);

struct graphBitmapFontHeader
{
    UINT8 fontID;                     // User assigned value
    UINT8 extendedGlyphEntry : 1;     // Extended Glyph entry flag. When set font has extended glyph feature enabled.
    UINT8 res1               : 1;     // Reserved for future use  (must be set to 0)
    UINT8 bpp                : 2;     // Actual BPP = 2^bpp
    UINT8 orientation        : 2;     // Orientation of the character glyphs (0,90,180,270 degrees)
                                            //   - 00 - Normal
                                            //   - 01 - Characters rotated 270 degrees clockwise
                                            //   - 10 - Characters rotated 180 degrees
                                            //   - 11 - Characters rotated 90 degrees clockwise
                                            // Note: Rendering DO NOT rotate the characters. The table contains rotated characters
                                            //       and will be rendered as is.
    UINT8 res2               : 2;     // Reserved for future use (must be set to 0).
    UINT16 firstChar;                 // Character code of first character (e.g. 32).
    UINT16 lastChar;                  // Character code of last character in font (e.g. 3006).
    UINT16 height;                    // Font characters height in pixels.
};

//Overview: Structures describing the glyph entry.
struct graphFontGlyphEntry
{
    UINT8 width;                      // width of the glyph
    UINT8 offsetLSB;                  // Least Significant UINT8 of the glyph location offset
    UINT16 offsetMSB;                 // Most Significant (2) UINT8s of the glyph location offset
};

struct graphFontGlyphEntryEx
{
    UINT32 offset;                    // Offset Order: LSW_LSB LSW_MSB MSW_MSB MSW_MSB
    UINT16 cursorAdvance;             // x-value by which cursor has to advance after rendering the glyph
    UINT16 glyphWidth;                // width of the glyph
    INT16 xAdjust;                    // x-position is adjusted as per this signed number
    INT16 yAdjust;                    // y-position is adjusted as per this signed number
};

// Structure for character information when rendering the character.
struct graphFontOutCharParam
{
    struct graphFontGlyphEntry *pChTable;
    struct graphFontGlyphEntryEx *pChTableExtended;

    UINT8 bpp;
    SHORT chGlyphWidth;
    UINT8 *pChImage;
    SHORT xAdjust;
    SHORT yAdjust;
    SHORT xWidthAdjust;
    SHORT heightOvershoot;
};

#endif
