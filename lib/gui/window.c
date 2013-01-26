/**
 * @file window.c
 * @brief Common window definitions
 * @author Daniel Szot
 */

#include "window.h"
#include "gui.h"
#include "lib/graphics/graphics.h"
#include "lib/input/keyCodes.h"

#define WND_DEBUG
#if defined(WND_DEBUG)
#define WND_LOG(frm, ...) LOG("wnd: "frm, ##__VA_ARGS__)
#else
#define WND_LOG(...)
#endif

/**
 * Return focused window in given control. Returns NULL if given control
 * is different than WT_PANEL
 * @param pWnd Pointer to parent window
 * @return Returns focused child window
 * @remark Used only when focus inheritance enabled. Probablly will be deleted
 * in future
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

    for (wnd = pWnd->firstChild; wnd; wnd = wnd->next)
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
    struct guiWindow *focusedChild = NULL;

    wnd = (struct guiWindow*)guiGetCurrentMainWindow();
    if (wnd != NULL)
    {
        focusedChild = guiGetWindowById(wnd, wnd->focusId);
    }
    
    return focusedChild;
}
/* Old version with focus inheritance probably to delete in future
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
}*/

/**
 * Sets pWnd.id as pWnd.mainWin focusId. This function do not send
 * MSG_SETFOCUS and MSG_KILLFOCUS messages so it will not redraw control
 * with focused style.
 * @param pWnd Pointer to window which will be focused
 */
void guiWindowSetFocused(struct guiWindow *pWnd)
{
    WND_LOG("Change focus from id: %d, to id: %d", pWnd->mainWin->head.focusId, pWnd->id);
    pWnd->mainWin->head.focusId = pWnd->id;
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

    for (wnd = pWnd->firstChild; wnd; wnd = wnd->next)
    {
        if ((guiXYInRect(pX, pY, &wnd->frame) == TRUE) &&
                (wnd->windowStyle & WS_VISIBLE))
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

    if (focusedChild == NULL)
        return NULL;

    if ((guiXYInRect(pX, pY, &focusedChild->frame) == TRUE) &&
                (focusedChild->windowStyle & WS_VISIBLE))
    {
        do
        {
            wnd = focusedChild;
            focusedChild = guiControlGetXYChild(wnd, pX, pY);
        }
        while(focusedChild != NULL);
    }
    else
    {
        wnd = NULL;
    }

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

struct guiWindow *guiGetWindowById(struct guiWindow *pRoot, UINT16 pId)
{
    struct guiWindow *wnd, *tw;

    if (pRoot->id == pId)
        return pRoot;

    for (wnd = pRoot->firstChild; wnd; wnd = wnd->next)
    {
        tw = guiGetWindowById(wnd, pId);

        if (tw != NULL)
        {
            return tw;
        }
    }

    return NULL;
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

static retcode guiDelChildFromParent(struct guiWindow *pParent, struct guiWindow *pCtrl)
{
    struct guiWindow *firstCtrl;
    BOOL found = FALSE;

    assert(pParent != NULL);
    assert(pCtrl != NULL);

    firstCtrl = pParent->firstChild;

    if (!firstCtrl)
        return ERR_NOT_FOUND;
    else
    {
        if (firstCtrl == pCtrl)
        {
            pParent->firstChild = pCtrl->next;
            found = TRUE;
        }
        else
        {
            while (firstCtrl->next)
            {
                if (firstCtrl->next == pCtrl)
                {
                    firstCtrl->next = pCtrl->next;
                    found = TRUE;
                    break;
                }

                firstCtrl = firstCtrl->next;
            }
        }
    }

    if (found == TRUE)
        return SUCCESS;
    else
        return ERR_NOT_FOUND;
}

struct guiWindow *guiCreateWindow (const char* pClassName,
        const char* pCaption, UINT32 pStyle,
        UINT16 pId, UINT16 pIdLeft, UINT16 pIdRight, UINT16 pIdTop, UINT16 pIdBottom,
        UINT16 pX, UINT16 pY, UINT16 pW, UINT16 pH,
        struct guiWindow *pParentWnd, UINT32 pAddData)
{
    struct guiMainWindow *mainWin;
    struct guiWndClass *cci;
    struct guiWindow *newCtrl;

    if (!(mainWin = guiGetMainWindowPtrOfControl(pParentWnd)))
        return NULL;

    cci = guiGetWindowClass(pClassName);

    if (!cci)
    {
        WND_LOG("Invalid window class name (\"%s\")", pClassName);
        return NULL;
    }

    newCtrl = pvPortMalloc(sizeof(struct guiWindow));

    if (!newCtrl)
    {
        WND_LOG("Can not allocate memmory for new window");
        return NULL;
    }

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
    newCtrl->colorStyle = cci->colorStyle;

    newCtrl->id = pId;
    newCtrl->idLeft = pIdLeft;
    newCtrl->idRight = pIdRight;
    newCtrl->idTop = pIdTop;
    newCtrl->idBottom = pIdBottom;
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
        WND_LOG("Given parent window is invalid (%p)", pParentWnd);
        free(newCtrl->caption);
        free(newCtrl);
        return NULL;
    }

    if (msgSend(newCtrl, MSG_NCCREATE, 0, pAddData))
    {
        guiDelChildFromParent(pParentWnd, newCtrl);
        free(newCtrl->caption);
        free(newCtrl);
        return NULL;
    }

    if (msgSend(newCtrl, MSG_CREATE, (UINT32)pParentWnd, pAddData))
    {
        guiDelChildFromParent(pParentWnd, newCtrl);
        free(newCtrl->caption);
        free(newCtrl);
        return NULL;
    }

    // TODO: Size changeing messages should be added there if some day will
    // be supported

    if ((newCtrl->parent->windowStyle & WS_VISIBLE) &&
            (newCtrl->windowStyle & WS_VISIBLE))
    {
        // TODO: InvalidateRect when damaging will be supported
        if (newCtrl->mainWin->head.focusId == newCtrl->id)
            msgSend(newCtrl, MSG_SETFOCUS, 0, 0);
        
        msgSend(newCtrl, MSG_PAINT, 0, (UINT32)&newCtrl->clientFrame);
        msgSend(newCtrl, MSG_NCPAINT, 0, 0);
    }

    // Dodac klase i dodac okno zobaczyc jak leca message.

    return newCtrl;
}

struct guiMainWindow *guiCreateMainWindow (const char* pClassName,
        const char* pCaption, UINT32 pStyle,
        UINT16 pId, UINT16 pFocusedId, UINT16 pX, UINT16 pY, UINT16 pW, UINT16 pH)
{
    struct guiWndClass *cci;
    struct guiMainWindow *newWnd;

    cci = guiGetWindowClass(pClassName);

    if (!cci)
    {
        WND_LOG("Invalid window class name (\"%s\")", pClassName);
        return NULL;
    }

    newWnd = pvPortMalloc(sizeof(struct guiMainWindow));

    if (!newWnd)
    {
        WND_LOG("Can not allocate memmory for new window");
        return NULL;
    }

    newWnd->head.type = WT_MAIN;

    newWnd->head.frame.l = pX;
    newWnd->head.frame.t = pY;
    newWnd->head.frame.w = pW;
    newWnd->head.frame.h = pH;

    // When we would like to support WS_BORDER or WS_CAPTION this
    // frames will be different
    newWnd->head.clientFrame = newWnd->head.frame;

    if (pCaption)
    {
        UINT16 len = strlen(pCaption);

        newWnd->head.caption = pvPortMalloc(len + 1);
        if (len > 0)
            strcpy(newWnd->head.caption, pCaption);
    }
    else
        newWnd->head.caption = "";

    newWnd->head.windowStyle = pStyle | cci->windowStyle;
    newWnd->head.colorStyle = cci->colorStyle;

    newWnd->head.id = pId;
    newWnd->head.idLeft = pId;
    newWnd->head.idRight = pId;
    newWnd->head.idTop = pId;
    newWnd->head.idBottom = pId;
    newWnd->head.focusId = pFocusedId;
    newWnd->head.addData = 0;
    newWnd->head.addData2 = 0;
    newWnd->head.windowProc = cci->windowProc;
    newWnd->head.font = guiGetDefaultFont();
    newWnd->head.mainWin = newWnd;
    newWnd->head.parent = NULL;
    newWnd->head.next = NULL;

    newWnd->head.wndClass = cci;

    if (msgSend((struct guiWindow*)newWnd, MSG_NCCREATE, 0, 0))
    {
        free(newWnd->head.caption);
        free(newWnd);
        return NULL;
    }

    if (msgSend((struct guiWindow*)newWnd, MSG_CREATE, 0, 0))
    {
        free(newWnd->head.caption);
        free(newWnd);
        return NULL;
    }

    // TODO: Size changeing messages should be added there if some day will
    // be supported

    if (newWnd->head.windowStyle & WS_VISIBLE)
    {
        // TODO: InvalidateRect when damaging will be supported
        guiSetCurrentMainWindow(newWnd);
        msgSend ((struct guiWindow*)newWnd, MSG_PAINT, 0, (UINT32)&newWnd->head.clientFrame);
        msgSend ((struct guiWindow*)newWnd, MSG_NCPAINT, 0, 0);
    }

    return newWnd;
}

/**
 * Default key down procedure for controls and main windows
 * @param pWnd Window pointer
 * @param pMsg Message code
 * @param pParam1
 * @param pParam2
 * @return Control proc result, depends from message
 */
static INT32 guiDefKeyDownProc(struct guiWindow *pWnd, UINT32 pMsg,
        UINT32 pParam1, UINT32 pParam2)
{
    struct guiWindow *wnd = NULL;
    struct guiWindow *oldFocusedWnd = NULL;

    assert(pWnd != NULL);

    if ((pParam1 == V_KEY_LEFT) || (pParam1 == V_KEY_RIGHT) ||
            (pParam1 == V_KEY_UP) || (pParam1 == V_KEY_DOWN))
    {
        oldFocusedWnd = guiWindowGetFocused();

        if (pParam1 == V_KEY_LEFT)
        {
            if (pWnd->idLeft != 0)
            {
                wnd = guiGetWindowById((struct guiWindow *)pWnd->mainWin, pWnd->idLeft);
                if (wnd != NULL)
                {
                    while ((wnd != NULL) && ((wnd->windowStyle & WS_VISIBLE) == 0))
                    {
                        wnd = guiGetWindowById((struct guiWindow *)wnd->mainWin, wnd->idLeft);
                    }
                }
            }
            else
            {
                // TODO: Obtain focused window automatically based on window positions
                // There we need to check WS_FOCUSSTOP
                // DO NOT SUPPORTED YET!
            }
        }
        else if (pParam1 == V_KEY_RIGHT)
        {
            if (pWnd->idRight != 0)
            {
                wnd = guiGetWindowById((struct guiWindow *)pWnd->mainWin, pWnd->idRight);
                if (wnd != NULL)
                {
                    while ((wnd != NULL) && ((wnd->windowStyle & WS_VISIBLE) == 0))
                    {
                        wnd = guiGetWindowById((struct guiWindow *)wnd->mainWin, wnd->idRight);
                    }
                }
            }
            else
            {
                // TODO: Obtain focused window automatically based on window positions
                // There we need to check WS_FOCUSSTOP
                // DO NOT SUPPORTED YET!
            }
        }
        else if (pParam1 == V_KEY_UP)
        {
            if (pWnd->idTop != 0)
            {
                wnd = guiGetWindowById((struct guiWindow *)pWnd->mainWin, pWnd->idTop);
                if (wnd != NULL)
                {
                    while ((wnd != NULL) && ((wnd->windowStyle & WS_VISIBLE) == 0))
                    {
                        wnd = guiGetWindowById((struct guiWindow *)wnd->mainWin, wnd->idTop);
                    }
                }
            }
            else
            {
                // TODO: Obtain focused window automatically based on window positions
                // There we need to check WS_FOCUSSTOP
                // DO NOT SUPPORTED YET!
            }
        }
        else if (pParam1 == V_KEY_DOWN)
        {
            if (pWnd->idBottom != 0)
            {
                wnd = guiGetWindowById((struct guiWindow *)pWnd->mainWin, pWnd->idBottom);
                if (wnd != NULL)
                {
                    while ((wnd != NULL) && ((wnd->windowStyle & WS_VISIBLE) == 0))
                    {
                        wnd = guiGetWindowById((struct guiWindow *)wnd->mainWin, wnd->idBottom);
                    }
                }
            }
            else
            {
                // TODO: Obtain focused window automatically based on window positions
                // There we need to check WS_FOCUSSTOP
                // DO NOT SUPPORTED YET!
            }
        }

        if (wnd != oldFocusedWnd)
        {
            if (oldFocusedWnd != NULL)
            {
                msgSend(oldFocusedWnd, MSG_KILLFOCUS, (UINT32)wnd, 0);
            }

            if (wnd != NULL)
            {
                guiWindowSetFocused(wnd);
                msgSend(wnd, MSG_SETFOCUS, (UINT32)oldFocusedWnd, 0);
            }
        }
    }
}

static INT32 guiDefMainWindowProc(struct guiWindow *pWnd, UINT32 pMsg,
        UINT32 pParam1, UINT32 pParam2)
{
    struct guiWindow *wnd;
    
    switch (pMsg)
    {
        case MSG_CREATE:
            pWnd->addData2 = pWnd->colorStyle.shIdx;
            break;

        case MSG_NCPAINT:
            // Paint the window's non-client area.
            break;

        case MSG_PAINT:
            // Paint the window's client area.
            guiBeginPaint();
            guiDrawStyleFrame(pWnd->addData2, &pWnd->clientFrame);
            guiEndPaint();

            for (wnd = pWnd->firstChild; wnd; wnd = wnd->next)
            {
                msgSend(wnd, MSG_PAINT, pParam1, pParam2);
            }
            break;

        case MSG_SETFOCUS:
            pWnd->addData2 = pWnd->colorStyle.hlIdx;
            msgSend(pWnd, MSG_PAINT, 0, 0);
            break;

        case MSG_KILLFOCUS:
            pWnd->addData2 = pWnd->colorStyle.shIdx;
            msgSend(pWnd, MSG_PAINT, 0, 0);
            break;

        case MSG_KEYDOWN:
            guiDefKeyDownProc(pWnd, pMsg, pParam1, pParam2);
            break;
    }

    return 0;
}

static INT32 guiDefConrolProc(struct guiWindow *pWnd, UINT32 pMsg,
        UINT32 pParam1, UINT32 pParam2)
{
    struct guiWindow *wnd;
    UINT32 len;
    
    switch (pMsg)
    {
        case MSG_SETFONT:
            if ((struct graphFont *)pParam1 != NULL)
            {
                pWnd->font = (struct graphFont *)pParam1;

                if ((pParam2 & 0xFF) != 0x00)
                    msgSend(pWnd, MSG_PAINT, 0, 0);
            }
            else
                return -1;
            break;

        case MSG_SETTEXT:
            if ((char *)pParam2 != NULL)
            {
                vPortFree(pWnd->caption);
                len = strlen((char *)pParam2);

                pWnd->caption = pvPortMalloc(len + 1);
                if (len > 0)
                    strcpy(pWnd->caption, (char *)pParam2);
            }
            else
                return -1;
            break;

        case MSG_GETTEXT:
            len = min(strlen((char *)pWnd->caption), pParam1);

            if (len > 0)
                strcpy((char *)pParam2, pWnd->caption);
            break;
            
        case MSG_KEYDOWN:
            guiDefKeyDownProc(pWnd, pMsg, pParam1, pParam2);
            break;

/*        case MSG_POINTERDOWN:
            wnd = guiWindowGetFocused();
            msgSend(wnd, MSG_KILLFOCUS, (UINT32)pWnd, 0);
            pWnd->mainWin->head.focusId = pWnd->id;
            msgSend(pWnd, MSG_SETFOCUS, (UINT32)wnd, 0);
            break;*/
    }

    return 0;
}

INT32 guiDefWindowProc(struct guiWindow *pWnd, UINT32 pMsg,
        UINT32 pParam1, UINT32 pParam2)
{
    if (pWnd->type == WT_MAIN)
        return guiDefMainWindowProc(pWnd, pMsg, pParam1, pParam2);
    else if (pWnd->type == WT_CONTROL)
        return guiDefConrolProc(pWnd, pMsg, pParam1, pParam2);
    else
        return 0;
}

BOOL guiShowWindow(struct guiWindow *pWnd, UINT32 pCmdShow)
{
    BOOL ret;

    assert(pWnd != NULL);
 
    ret = ((pWnd->windowStyle & WS_VISIBLE) == WS_VISIBLE);

    if (pWnd->type == WT_MAIN)
    {
        switch (pCmdShow)
        {
            case SW_SHOW:
                guiSetCurrentMainWindow((struct guiMainWindow *)pWnd);
                msgPost(pWnd, MSG_NCPAINT, 0 ,0);
                msgPost(pWnd, MSG_PAINT, 0 ,0);
                break;

            case SW_HIDE:
                guiSetCurrentMainWindow(NULL);
                graphClearScreen();
                break;
        }
    }
    else if (pWnd->type == WT_CONTROL)
    {
        switch (pCmdShow)
        {
            case SW_SHOW:
                pWnd->windowStyle |= WS_VISIBLE;
                msgPost(pWnd, MSG_NCPAINT, 0 ,0);
                msgPost(pWnd, MSG_PAINT, 0 ,0);
                break;

            case SW_HIDE:
                pWnd->windowStyle &= ~WS_VISIBLE;
                if (pWnd->parent != NULL)
                    msgPost(pWnd->parent, MSG_PAINT, 0 ,0);
                break;
        }
    }

    return ret;
}
