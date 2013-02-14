/**
 * @file primitives.c
 * @brief Part of Graphics library. Drawing primitives like lines, circles etc.
 * @author Daniel Szot
 */

#include "graphics.h"

retcode graphDrawLine(UINT16 pX1, UINT16 pY1, UINT16 pX2, UINT16 pY2)
{
    INT16 deltaX, deltaY;
    INT16 error, stepErrorLT, stepErrorGE;
    INT16 stepX, stepY;
    INT16 steep;
    INT16 temp;
    INT16 style, type;

    if (graphLcdDev == NULL)
        return DEVICE_NOT_SET;

    if (pX1 == pX2)
    {
        if (pY1 > pY2)
        {
            temp = pY1;
            pY1 = pY2;
            pY2 = temp;
        }

        style = 0;
        type = 1;
        for (temp = pY1; temp < pY2 + 1; temp++)
        {
            graphLcdDev->drawPixel(graphLcdDev, pX1, temp);
        }

        return (1);
    }

    if (pY1 == pY2)
    {
        if (pX1 > pX2)
        {
            temp = pX1;
            pX1 = pX2;
            pX2 = temp;
        }

        style = 0;
        type = 1;
        for (temp = pX1; temp < pX2 + 1; temp++)
        {
            graphLcdDev->drawPixel(graphLcdDev, temp, pY1);
        }

        return (1);
    }

    stepX = 0;
    deltaX = pX2 - pX1;
    if (deltaX < 0)
    {
        deltaX = -deltaX;
        --stepX;
    }
    else
    {
        ++stepX;
    }

    stepY = 0;
    deltaY = pY2 - pY1;
    if (deltaY < 0)
    {
        deltaY = -deltaY;
        --stepY;
    }
    else
    {
        ++stepY;
    }

    steep = 0;
    if (deltaX < deltaY)
    {
        ++steep;
        temp = deltaX;
        deltaX = deltaY;
        deltaY = temp;
        temp = pX1;
        pX1 = pY1;
        pY1 = temp;
        temp = stepX;
        stepX = stepY;
        stepY = temp;
        graphLcdDev->drawPixel(graphLcdDev, pY1, pX1);
    }
    else
    {
        graphLcdDev->drawPixel(graphLcdDev, pX1, pY1);
    }

    // If the current error greater or equal zero
    stepErrorGE = deltaX << 1;

    // If the current error less than zero
    stepErrorLT = deltaY << 1;

    // Error for the first pixel
    error = stepErrorLT - deltaX;

    style = 0;
    type = 1;

    while (--deltaX >= 0)
    {
        if (error >= 0)
        {
            pY1 += stepY;
            error -= stepErrorGE;
        }

        pX1 += stepX;
        error += stepErrorLT;


        if (steep)
        {
            graphLcdDev->drawPixel(graphLcdDev, pY1, pX1);
        }
        else
        {
            graphLcdDev->drawPixel(graphLcdDev, pX1, pY1);
        }
    }

    return SUCCESS;
}

retcode graphDrawRect(UINT16 pX1, UINT16 pY1, UINT16 pX2, UINT16 pY2)
{
    if (graphLcdDev == NULL)
        return DEVICE_NOT_SET;

    graphLcdDev->fill(graphLcdDev, pX1, pY1, pX2, pY2);

    return SUCCESS;
}

retcode graphDrawCircle(UINT16 pX, UINT16 pY, UINT16 pRadius)
{
    if (graphLcdDev == NULL)
        return DEVICE_NOT_SET;

    INT16 r = (INT16) pRadius;
    INT16 x = -r, y = 0, err = 2 - 2 * r; /* II. Quadrant */
    do
    {
        graphLcdDev->drawPixel(graphLcdDev, pX - x, pY + y); /*   I. Quadrant */
        graphLcdDev->drawPixel(graphLcdDev, pX - y, pY - x); /*  II. Quadrant */
        graphLcdDev->drawPixel(graphLcdDev, pX + x, pY - y); /* III. Quadrant */
        graphLcdDev->drawPixel(graphLcdDev, pX + y, pY + x); /*  IV. Quadrant */
        r = err;

        if (r > x)
            err += ++x * 2 + 1; /* e_xy+e_x > 0 */
        if (r <= y)
            err += ++y * 2 + 1; /* e_xy+e_y < 0 */

    }
    while (x < 0);

    return SUCCESS;
}

retcode graphDrawRoundedRect()
{
    // Not supported yet
    return ERR_NOT_SUPPORTED;
}

/*********************************************************************
 * Function:  void OutCharGetInfoFlash (XCHAR ch, OUTCHAR_PARAM *pParam)
 *
 * PreCondition: none
 *
 * Input: ch - character code
 *        pParam - pointer to character information structure.
 *
 * Output: none
 *
 * Side Effects: none
 *
 * Overview: Gathers the parameters for the specified character of the
 *           currently set font from flash memory. This is a step performed
 *           before the character is rendered.
 *
 * Note: Application should not call this function. This function is for
 *       versatility of implementing hardware accelerated text rendering
 *       only.
 *
 ********************************************************************/
static void
graphGetCharInfo(UINT16 pCh, struct graphFontOutCharParam *pParam,
        struct graphFont *pFont)
{
    struct graphFontGlyphEntryEx *pChTableExtended;
    struct graphFontGlyphEntry *pChTable;
    struct graphBitmapFontHeader *fontHead;

    fontHead = (struct graphBitmapFontHeader*) pFont->address;

    pParam->pChTable = NULL;
    pParam->pChTableExtended = NULL;
    pParam->xAdjust = 0;
    pParam->yAdjust = 0;
    pParam->xWidthAdjust = 0;
    pParam->heightOvershoot = 0;

    // set color depth of font,
    // based on 2^bpp where bpp is the color depth setting in the FONT_HEADER
    pParam->bpp = 1 << fontHead->bpp;

    if (fontHead->extendedGlyphEntry)
    {
        pChTableExtended = (struct graphFontGlyphEntryEx *) (pFont->address +
                sizeof (struct graphBitmapFontHeader)) + ((UINT16) pCh -
                (UINT16) (fontHead->firstChar));

        pParam->pChImage = (UINT8 *) (pFont->address + pChTableExtended->offset);
        pParam->chGlyphWidth = pChTableExtended->glyphWidth;

        pParam->xWidthAdjust = pChTableExtended->glyphWidth -
                pChTableExtended->cursorAdvance;

        pParam->xAdjust = pChTableExtended->xAdjust;
        pParam->yAdjust = pChTableExtended->yAdjust;

        if (pParam->yAdjust > 0)
        {
            pParam->heightOvershoot = pParam->yAdjust;
        }
    }
    else
    {
        pChTable = (struct graphFontGlyphEntry *) ((pFont)->address +
                sizeof (struct graphBitmapFontHeader)) + ((UINT16) pCh -
                (UINT16) (fontHead->firstChar));

        pParam->pChImage = (UINT8 *) ((pFont)->address +
                ((UINT32) (pChTable->offsetMSB) << 8) + pChTable->offsetLSB);

        pParam->chGlyphWidth = pChTable->width;
    }
}

static UINT16 _fgcolor100;
static UINT16 _fgcolor25;
static UINT16 _fgcolor75;
static UINT16 _bgcolor100;
static UINT16 _bgcolor25;
static UINT16 _bgcolor75;

/**
 * Calculates mid values of colors
 */
static void __attribute__((always_inline)) calculateColors()
{
    UINT16 _fgcolor50;
    UINT16 _bgcolor50;

    _fgcolor50 = (_fgcolor100 & 0b1111011111011110u) >> 1;
    _fgcolor25 = (_fgcolor50 & 0b1111011111011110u) >> 1;
    _fgcolor75 = _fgcolor50 + _fgcolor25;

    _bgcolor50 = (_bgcolor100 & 0b1111011111011110u) >> 1;
    _bgcolor25 = (_bgcolor50 & 0b1111011111011110u) >> 1;
    _bgcolor75 = _bgcolor50 + _bgcolor25;

    _fgcolor25 += _bgcolor75;
    _fgcolor75 += _bgcolor25;
}

static retcode graphCharRender(UINT16 pX, UINT16 pY,
        struct graphFontOutCharParam *pParam,
        struct graphBitmapFontHeader *pFontHead)
{
    assert(pParam != NULL);
    assert(pFontHead != NULL);

    UINT8 temp = 0;
    UINT8 mask;
    UINT8 restoremask;
    INT16 xCnt, yCnt, x = 0, y;

    UINT8 val, shift;
    UINT16 bgcolor;

    if (pParam->bpp == 1)
    {
        restoremask = 0x01;
    }
    else
    {
        if (pParam->bpp == 2)
        {
            restoremask = 0x03;
        }
        else
        {
            return ERR_NOT_SUPPORTED; // BPP > 2 are not yet supported
        }

        bgcolor = graphLcdDev->getPixel(graphLcdDev,
                pX + (pParam->chGlyphWidth >> 1), pY + (pFontHead->height >> 1));

        if ((_fgcolor100 != graphLcdDev->drawingColor) || (_bgcolor100 != bgcolor))
        {
            _fgcolor100 = graphLcdDev->drawingColor;
            _bgcolor100 = bgcolor;
            calculateColors();
        }
    }

    if (pFontHead->orientation == GRAPH_FONT_ORIENT_HOR)
    {
        y = pY + pParam->yAdjust;
        for (yCnt = 0; yCnt < pFontHead->height + pParam->heightOvershoot; yCnt++)
        {
            x = pX + pParam->xAdjust;
            mask = 0;

            shift = 0;

            for (xCnt = 0; xCnt < pParam->chGlyphWidth; xCnt++)
            {
                if (mask == 0)
                {
                    temp = *(pParam->pChImage)++;
                    mask = restoremask;

                    shift = 0;
                }

                if (pParam->bpp == 1)
                {
                    if (temp & mask)
                    {
                        graphLcdDev->drawPixel(graphLcdDev, x, y);
                    }
                }
                else
                {
                    val = (temp & mask) >> shift;
                    if (val)
                    {
#if (ANTIALIAS_METHOD == ANTIALIAS_TRANSLUCENT)
                        bgcolor = graphLcdDev->getPixel(graphLcdDev, x, y);
                        if (_bgcolor100 != bgcolor)
                        {
                            _bgcolor100 = bgcolor;
                            calculateColors();
                        }
#endif
                        switch (val)
                        {
                            case 1: graphLcdDev->setColorRaw(graphLcdDev, _fgcolor25);
                                break;

                            case 2: graphLcdDev->setColorRaw(graphLcdDev, _fgcolor75);
                                break;

                            case 3: graphLcdDev->setColorRaw(graphLcdDev, _fgcolor100);
                        }

                        graphLcdDev->drawPixel(graphLcdDev, x, y);
                    }
                }

                mask <<= pParam->bpp;
                shift += pParam->bpp;

                x++;
            }
            y++;
        }
    }
    else // If extended glyph is used, then vertical alignment may not be rendered properly and hence users must position the texts properly
    {
        y = pX + pParam->xAdjust;
        for (yCnt = 0; yCnt < pFontHead->height + pParam->heightOvershoot; yCnt++)
        {
            x = pY + pParam->yAdjust;
            mask = 0;

            shift = 0;

            for (xCnt = 0; xCnt < pParam->chGlyphWidth; xCnt++)
            {
                if (mask == 0)
                {
                    temp = *(pParam->pChImage)++;
                    mask = restoremask;

                    shift = 0;
                }

                if (pParam->bpp == 1)
                {
                    if (temp & mask)
                    {
                        graphLcdDev->drawPixel(graphLcdDev, x, y);
                    }
                }
                else
                {
                    val = (temp & mask) >> shift;
                    if (val)
                    {

#if (ANTIALIAS_METHOD == ANTIALIAS_TRANSLUCENT)
                        bgcolor = graphLcdDev->getPixel(graphLcdDev, x, y);
                        if (_bgcolor100 != bgcolor)
                        {
                            _bgcolor100 = bgcolor;
                            calculateColors();
                        }
#endif

                        switch (val)
                        {
                            case 1: graphLcdDev->setColorRaw(graphLcdDev, _fgcolor25);
                                break;

                            case 2: graphLcdDev->setColorRaw(graphLcdDev, _fgcolor75);
                                break;

                            case 3: graphLcdDev->setColorRaw(graphLcdDev, _fgcolor100);
                        }

                        graphLcdDev->drawPixel(graphLcdDev, x, y);
                    }
                }

                mask <<= pParam->bpp;
                shift += pParam->bpp;

                x--;
            }
            y++;
        }
    }

    // restore color
    if (pParam->bpp > 1)
    {
        graphLcdDev->drawingColor = _fgcolor100;
    }
    return SUCCESS;

}

static retcode graphDrawBitmapChar(UINT16 pX, UINT16 pY, UINT16 *pCharWidth, UINT16 pChar, struct graphFont *pFont)
{
    struct graphBitmapFontHeader *fontHead;
    struct graphFontOutCharParam cparam;

    assert(pFont != NULL);

    if (graphLcdDev == NULL)
        return DEVICE_NOT_SET;

    DEBUG("graphDrawBitmapChar");

    fontHead = (struct graphBitmapFontHeader*) pFont->address;

    if (pChar < fontHead->firstChar)
        return ERR_CHAR_NOT_IN_FONT;
    if (pChar > fontHead->lastChar)
        return ERR_CHAR_NOT_IN_FONT;

    graphGetCharInfo(pChar, &cparam, pFont);

    (*pCharWidth) = cparam.chGlyphWidth - cparam.xAdjust - cparam.xWidthAdjust;
    graphCharRender(pX, pY, &cparam, fontHead);

    return SUCCESS;
}

static void graphDrawBitmapText(UINT16 pX, UINT16 pY, UINT16 pW, UINT16 pH,
        char * pText, struct graphFont *pFont, UINT32 pStyle)
{
    char c;
    UINT16 tw, th;
    UINT32 charWidth, x = 0;
    struct graphBitmapFontHeader *fontHead;
    struct graphFontOutCharParam cparam;

    assert(pFont != NULL);

    if (graphLcdDev == NULL)
        return;

    fontHead = (struct graphBitmapFontHeader*) pFont->address;

    if (fontHead->height > pH)
        return;

    if ((pStyle & FS_ALIGN_MASK) == FS_ALIGN_CENTER)
    {
        graphGetTextSize(&tw, &th, pText, pFont);
        if (tw < pW)
            pX += (pW - tw) / 2;
    } 
    else if ((pStyle & FS_ALIGN_MASK) == FS_ALIGN_RIGHT)
    {
        graphGetTextSize(&tw, &th, pText, pFont);
        if (tw < pW)
            pX += (pW - tw);
    }

    if ((pStyle & FS_VALIGN_MASK) == FS_VALIGN_BOTTOM)
    {
        // We do not need to check fontHead->height < pH condition because
        // we actually do it at the begining of function and return when
        // condition is false
        pY += (pH - fontHead->height);
    }
    else if ((pStyle & FS_VALIGN_MASK) == FS_VALIGN_CENTER)
    {
        // Same case, see above comment
        pY += (pH - fontHead->height)/2;
    }

    while ((c = *(pText++)) != 0)
    {
        if (c < fontHead->firstChar)
            continue;
        if (c > fontHead->lastChar)
            continue;

        graphGetCharInfo(c, &cparam, pFont);

        charWidth = cparam.chGlyphWidth - cparam.xAdjust - cparam.xWidthAdjust;

        if ((x + charWidth) > pW)
            break;

        graphCharRender(pX+x, pY, &cparam, fontHead);
        x += charWidth;
    }
}

static retcode graphDrawOutlineChar(UINT16 pX, UINT16 pY, UINT16 *pCharWidth, UINT16 pChar, struct graphFont *pFont)
{
    // Not supported yet
    return ERR_NOT_SUPPORTED;
}

static void graphDrawOutlineText(UINT16 pX, UINT16 pY, UINT16 pW, UINT16 pH,
        char * pText, struct graphFont *pFont, UINT32 pStyle)
{
    return;
}

/**
 * Draw character at specified position using specified font
 * @param pX Character draw Y position
 * @param pY Character draw X position
 * @param pCharWidth Out param, start drawing next char after pCharWidth pixels
 * @param pChar Character to draw
 * @param pFont Pointer to font which will be used
 * @return
 */
retcode graphDrawChar(UINT16 pX, UINT16 pY, UINT16 *pCharWidth, UINT16 pChar, struct graphFont *pFont)
{
    if (pFont->type == GRAPH_FONT_BITMAP)
        return graphDrawBitmapChar(pX, pY, pCharWidth, pChar, pFont);
    else if (pFont->type == GRAPH_FONT_OUTLINE)
        return graphDrawOutlineChar(pX, pY, pCharWidth, pChar, pFont);
    else
        return ERR_NOT_SUPPORTED;
}

void graphDrawText(UINT16 pX, UINT16 pY, UINT16 pW, UINT16 pH,
        char * pText, const struct graphFont *pFont, UINT32 pStyle)
{
    if (pFont->type == GRAPH_FONT_BITMAP)
        graphDrawBitmapText(pX, pY, pW, pH, pText, pFont, pStyle);
    else if (pFont->type == GRAPH_FONT_OUTLINE)
        graphDrawOutlineText(pX, pY, pW, pH, pText, pFont, pStyle);
    else
        return;
}

static void graphGetOutlineTextSize(UINT16 *pW, UINT16 *pH, char * pText, struct graphFont *pFont)
{

}

static void graphGetBitmapTextSize(UINT16 *pW, UINT16 *pH, char * pText, struct graphFont *pFont)
{
    char c;
    UINT32 charWidth, w = 0;
    struct graphBitmapFontHeader *fontHead;
    struct graphFontOutCharParam cparam;

    assert(pFont != NULL);

    fontHead = (struct graphBitmapFontHeader*) pFont->address;

    while ((c = *(pText++)) != 0)
    {
        if (c < fontHead->firstChar)
            continue;
        if (c > fontHead->lastChar)
            continue;

        graphGetCharInfo(c, &cparam, pFont);

        charWidth = cparam.chGlyphWidth - cparam.xAdjust - cparam.xWidthAdjust;

        w += charWidth;
    }

    *pH = fontHead->height;
    *pW = w;
}

void graphGetTextSize(UINT16 *pW, UINT16 *pH, char * pText, struct graphFont *pFont)
{
    if (pFont->type == GRAPH_FONT_BITMAP)
        graphGetBitmapTextSize(pW, pH, pText, pFont);
    else if (pFont->type == GRAPH_FONT_OUTLINE)
        graphGetOutlineTextSize(pW, pH, pText, pFont);
    else
        return;
}

