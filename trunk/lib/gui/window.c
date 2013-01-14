/**
 * @file window.c
 * @brief Common window definitions
 * @author Daniel Szot
 */

#include "window.h"
#include "gui.h"

/**
 * Return focused window in given control. Returns NULL if given control
 * is different than WT_PANEL
 * @param pWnd Pointer to parent window
 * @return Returns focused child window
 */
static struct guiWindow *guiControlGetFocusedChild(struct guiWindow *pWnd)
{
    // We would like to find last one in list child which
    // is focused because of drawing order from begin to end
    // so we wish to get focused element on top of others
    struct guiWindow *wnd;
    struct guiWindow *ret = NULL;

    if (pWnd == NULL)
        return NULL;

    for (wnd = pWnd->firstChild; (wnd && wnd->next); wnd = wnd->next)
    {
        if ((pWnd->focusId != 0) && (wnd->id == pWnd->focusId))
        {
            // Ok got it
            ret = wnd;
            // But we are continue to return last one
            // I know that this is not best idea from optimalisation point
            // of view, maybe one day I will modify it :)
        }
    }

    return ret;
}

/**
 * Gets focused window, this could be root or child window as well
 * @return Window pointer. NULL if there is no any focused window
 */
struct guiWindow *guiWindowGetFocused()
{
    // Just one main window on screen supported at this moment so check
    // only current main window children
    struct guiWindow *wnd;
    struct guiWindow *focusedChild;

    focusedChild = (struct guiWindow*)guiGetCurrentMainWindow();
    do
    {
        wnd = focusedChild;
        focusedChild = guiControlGetFocusedChild(wnd);
    }
    while(focusedChild != NULL);

    return wnd;
}

/**
 * Gets focused root window. If focused is child window get parent of window
 * and parent of next etc until we catch root window.
 * @return Root window pointer. NULL if there is no any focused window
 */
struct guiMainWindow *guiWindowGetFocusedMain()
{
    // For now we support onlly one main window visible on the screen so we
    // just return pointer to current main window
    // In future this should checks which main window is focused and return
    // pointer to it.
    return guiGetCurrentMainWindow();
}

/**
 * Return child window of given parent in pX, pY point
 * @param pWnd Pointer to parent window
 * @param pX Coordinate X
 * @param pY Coordinate Y
 * @return Returns focused child window
 */
static struct guiWindow *guiControlGetXYChild(struct guiWindow *pWnd, UINT16 pX, UINT16 pY)
{
    // We would like to find last one in list child which
    // is focused because of drawing order from begin to end
    // so we wish to get focused element on top of others
    struct guiWindow *wnd;
    struct guiWindow *ret = NULL;

    if (pWnd == NULL)
        return NULL;

    for (wnd = pWnd->firstChild; (wnd && wnd->next); wnd = wnd->next)
    {
        if (guiXYInRect(pX, pY, &wnd->frame) == TRUE)
        {
            // Ok got it
            ret = wnd;
            // But we are continue to return last one
            // I know that this is not best idea from optimalisation point
            // of view, maybe one day I will modify it :)
        }
    }

    return ret;
}

/**
 * Gets window under given x,y coordinates
 * @param pX Coordinate X
 * @param pY Coordinate Y
 * @return Pointer to picked window
 */
struct guiWindow *guiWindowAtXY(UINT16 pX, UINT16 pY)
{
    // Just one main window on screen supported at this moment so check
    // only current main window children
    struct guiWindow *wnd;
    struct guiWindow *focusedChild;

    focusedChild = (struct guiWindow*)guiGetCurrentMainWindow();
    do
    {
        wnd = focusedChild;
        focusedChild = guiControlGetXYChild(wnd, pX, pY);
    }
    while(focusedChild != NULL);

    return wnd;
}

/**
 * Gets root window at pX,pY position.
 * @param pX Coordinate X
 * @param pY Coordinate Y
 * @return Root window pointer. NULL if there is no any focused window
 */
struct guiMainWindow *guiWindowAtXYMain(UINT16 pX, UINT16 pY)
{
    // For now we support onlly one main window visible on the screen so we
    // just return pointer to current main window
    // In future this should checks which main window is at cursor pos and return
    // pointer to it.
    return guiGetCurrentMainWindow();
}

struct guiMainWindow *guiGetMainWindowPtrOfControl(struct guiWindow *pWnd)
{
    if (pWnd == NULL)
        return NULL;

    return pWnd->mainWin;
}

static void guiAddChildToParent(struct guiWindow *pParent, struct guiWindow *pNewCtrl)
{
    struct guiWindow *firstCtrl;
    struct guiWindow *lastCtrl;

    assert(pParent != NULL);
    assert(pNewCtrl != NULL);

    firstCtrl = pParent->firstChild;
    pNewCtrl->next = NULL;

    if (firstCtrl == NULL)
    {
        pParent->firstChild = pNewCtrl;
    }
    else
    {
        lastCtrl = firstCtrl;

        while (lastCtrl->next != NULL)
            lastCtrl = lastCtrl->next;

        lastCtrl->next = pNewCtrl;
    }
}

struct guiWindow *guiCreateWindow (const char* pClassName,
        const char* pCaption, DWORD pStyle,
        UINT16 pId, UINT16 pX, UINT16 pY, UINT16 pW, UINT16 pH, 
        struct guiWindow *pParentWnd, UINT32 pAddData)
{
    struct guiMainWindow *mainWin;
    struct guiWndClass *cci;
    struct guiWindow *newCtrl;

    if (!(mainWin = guiGetMainWindowPtrOfControl(pParentWnd)))
        return NULL;

    cci = guiGetWindowClass(pClassName);

    if (!cci)
        return NULL;

    newCtrl = pvPortMalloc(sizeof(struct guiWindow));

    if (!newCtrl)
        return NULL;

    newCtrl->type = WT_CONTROL;

    newCtrl->frame.l = pX;
    newCtrl->frame.t = pY;
    newCtrl->frame.w = pW;
    newCtrl->frame.h = pH;

    // When we would like to support WS_BORDER or WS_CAPTION this
    // frames will be different
    newCtrl->clientFrame = newCtrl->frame;

    if (pCaption)
    {
        UINT16 len = strlen(pCaption);

        newCtrl->caption = pvPortMalloc(len + 1);
        if (len > 0)
            strcpy(newCtrl->caption, pCaption);
    }
    else
        newCtrl->caption = "";

    newCtrl->windowStyle = pStyle | WS_CHILD | cci->windowStyle;

    newCtrl->id = pId;
    newCtrl->addData = pAddData;
    newCtrl->addData2 = 0;
    newCtrl->windowProc = cci->windowProc;
    newCtrl->font = guiGetDefaultFont();
    newCtrl->mainWin = mainWin;
    newCtrl->parent = pParentWnd;
    newCtrl->next = NULL;

    newCtrl->wndClass = cci;

    // Add control to parent child list
    if (pParentWnd != NULL)
    {
        guiAddChildToParent(pParentWnd, newCtrl);
    }
    else
    {
        free(newCtrl->caption);
        free(newCtrl);
        return NULL;
    }
// Dorobic te message do tworzenia i rysowania okna
    if (msgSend(newCtrl, MSG_NCCREATE, 0, pAddData))
    {
        //TODO: Delete from parent
        free(newCtrl->caption);
        free(newCtrl);
        return NULL;
    }

    if (msgSend (newCtrl, MSG_CREATE, pParentWnd, pAddData))
    {
        //TODO: Delete from parent
        free(newCtrl->caption);
        free(newCtrl);
        return NULL;
    }

    // Size changeing messages should be added there if some day will
    // be supported

    if ((newCtrl->parent->windowStyle & WS_VISIBLE) &&
            (newCtrl->windowStyle & WS_VISIBLE))
    {
        //TODO: Draw window
        //InvalidateRect ((HWND)newCtrl, NULL, TRUE);
        //SendAsyncMessage ((HWND)newCtrl, MSG_NCPAINT, 0, 0);
    }

    return newCtrl;
}
