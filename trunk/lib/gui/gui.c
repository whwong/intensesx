/**
 * @file gui.c
 * @brief Common GUI source
 * @author Daniel Szot
 */

#include "gui.h"

static struct guiWinStyle *currentWinstyle = NULL;
static struct guiMainWindow *currentMainWindow = NULL;
static struct graphFont *defaultFont = NULL;

/**
 * Sets current winstyle
 * @param pWinstyle Pointer to winstyle table
 */
void guiSetWinstyle(struct guiWinStyle *pWinstyle)
{
    currentWinstyle = pWinstyle;
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
void guiSetDefaultFont(struct graphFont *pFont)
{
    defaultFont = pFont;
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
