/**
 * @file gui.c
 * @brief Common GUI source
 * @author Daniel Szot
 */

#include "gui.h"
#include "lib/graphics/graphics.h"

#define GUI_DEBUG
#if defined(GUI_DEBUG)
#define GUI_LOG(frm, ...) LOG("gui: "frm, ##__VA_ARGS__)
#else
#define GUI_LOG(...)
#endif

static struct guiWinStyle *currentWinstyle = NULL;
static struct guiMainWindow *currentMainWindow = NULL;
static struct graphFont *defaultFont = NULL;
static xSemaphoreHandle guiDrawMutex = NULL;

/**
 * Initialize GUI library
 * @return Result code
 */
retcode guiInit()
{
    retcode ret;
    
    if (guiDrawMutex == NULL)
        guiDrawMutex = xSemaphoreCreateMutex();

    if (guiDrawMutex == NULL)
    {
        return ERR_NO_MEMMORY;
    }
    
    ret = guiInitWindowClasses();
    if (ret != SUCCESS)
    {
        return ret;
    }
    
    return SUCCESS;
}

/**
 * Execute always before start drawing in MSG_PAINT message.
 * This function locks drawing library access for other tasks
 */
void guiBeginPaint()
{
    while (xSemaphoreTake(guiDrawMutex, portMAX_DELAY) != pdTRUE);
}

/**
 * Execute always after end drawing in MSG_PAINT message.
 * This function releases drawing library access for other tasks
 */
void guiEndPaint()
{
    xSemaphoreGive(guiDrawMutex);
}

/**
 * Sets current winstyle
 * @param pWinstyle Pointer to winstyle table
 */
void guiSetWinstyle(const struct guiWinStyle *pWinstyle)
{
    currentWinstyle = (struct guiWinStyle*)pWinstyle;
}

/**
 * Get current windowstyle
 * @return Winstyle pointer
 */
struct guiWinStyle *guiGetWinstyle()
{
    return currentWinstyle;
}

/**
 * Sets current main window
 * @param pMainWindow Main window which will be set
 */
void guiSetCurrentMainWindow(struct guiMainWindow *pMainWindow)
{
    currentMainWindow = pMainWindow;
}

/**
 * Gets current active and showing on screen pointer to main window
 * @return Pointer to main window
 */
struct guiMainWindow *guiGetCurrentMainWindow()
{
    return currentMainWindow;
}

/**
 * Sets default font
 * @param pFont Pointer to font
 */
void guiSetDefaultFont(const struct graphFont *pFont)
{
    defaultFont = (struct graphFont *)pFont;
}

/**
 * Get current windowstyle
 * @return Winstyle pointer
 */
struct graphFont *guiGetDefaultFont()
{
    return defaultFont;
}

/**
 * Checks that given point is in given rect
 * @param pPoint Point to check
 * @param pRect Rectangle to check
 * @return Comparation result
 * @retval TRUE Given point is in given rect
 * @retval FALSE Given point is not in given rect
 */
BOOL guiPointInRect(struct guiPoint *pPoint, struct guiRect *pRect)
{
    if ((pPoint->x >= pRect->l) &&
            (pPoint->x < (pRect->l + pRect->w)) &&
            (pPoint->y >= pRect->t) &&
            (pPoint->y < (pRect->t + pRect->h)))
        return TRUE;
    else
        return FALSE;
}

/**
 * Checks that given x,y coordinates are in given rect
 * @param pX Coordinate X
 * @param pY Coordinate Y
 * @param pRect Rectangle to check
 * @return Comparation result
 * @retval TRUE Given point is in given rect
 * @retval FALSE Given point is not in given rect
 */
BOOL guiXYInRect(UINT16 pX, UINT16 pY, struct guiRect *pRect)
{
    if ((pX >= pRect->l) &&
            (pX < (pRect->l + pRect->w)) &&
            (pY >= pRect->t) &&
            (pY < (pRect->t + pRect->h)))
        return TRUE;
    else
        return FALSE;
}

struct guiWinStyle *guiGetStyle(UINT8 pIndex)
{
    return &(currentWinstyle[pIndex]);
}

void guiDrawStyleFrame(UINT8 pStyleIdx, struct guiRect *pRect)
{
    struct guiWinStyle *ws;
    ws = guiGetStyle(pStyleIdx);

    if (ws->bgStyle == CS_SOLID)
    {
        guiSetColor(ws->bgColor);
        guiDrawRect((*pRect));
    }

    if (ws->leftLineStyle == CS_SOLID)
    {
        guiSetColor(ws->leftLineColor);
        graphDrawRect(pRect->l,
            pRect->t,
            pRect->l + ws->leftLineWidth - 1,
            pRect->t + pRect->h);
    }

    if (ws->rightLineStyle == CS_SOLID)
    {
        guiSetColor(ws->leftLineColor);
        graphDrawRect(pRect->l + pRect->w - ws->rightLineWidth + 1,
            pRect->t,
            pRect->l + pRect->w,
            pRect->t + pRect->h);
    }

    if (ws->topLineStyle == CS_SOLID)
    {
        guiSetColor(ws->leftLineColor);
        graphDrawRect(pRect->l,
            pRect->t,
            pRect->l + pRect->w,
            pRect->t + ws->topLineWidth - 1);
    }

    if (ws->bottomLineStyle == CS_SOLID)
    {
        guiSetColor(ws->leftLineColor);
        graphDrawRect(pRect->l,
            pRect->t + pRect->h - ws->bottomLineWidth + 1,
            pRect->l + pRect->w,
            pRect->t + pRect->h);
    }
}

void guiDrawText(struct guiRect *pRect, char *pText, struct graphFont *pFont, UINT32 pStyle)
{
    graphDrawText(pRect->l, pRect->t, pRect->w, pRect->h, pText, pFont, pStyle);
}
