/**
 * @file button.c
 * @brief Button control
 * @author Daniel Szot
 */

#include "button.h"
#include "gui.h"
#include "lib/graphics/graphics.h"

#define BTN_DEBUG
#if defined(BTN_DEBUG)
#define BTN_LOG(frm, ...) LOG("wnd: "frm, ##__VA_ARGS__)
#else
#define BTN_LOG(...)
#endif

void guiRegisterButtonClass()
{
    struct guiWndClassInfo wci;

    wci.className = "button";
    wci.windowStyle = WS_CHILD | WS_VISIBLE;
    wci.colorStyle.shIdx = WSTL_BUTTON_SH;
    wci.colorStyle.hlIdx = WSTL_BUTTON_HL;
    wci.colorStyle.selIdx = WSTL_BUTTON_SEL;
    wci.colorStyle.gryIdx = WSTL_BUTTON_GRY;
    wci.windowProc = guiDefButtonProc;

    guiRegisterWindowClass(&wci);
}

INT32 guiDefButtonProc(struct guiWindow *pWnd, UINT32 pMsg,
        UINT32 pParam1, UINT32 pParam2)
{
    UINT32 len, idx;
    
    switch (pMsg)
    {
        case MSG_CREATE:
            pWnd->addData2 = 0;
            break;

        case MSG_ENABLE:
            if (pParam1 == TRUE)
                pWnd->addData2 &= ~BS_DISABLED;
            else
                pWnd->addData2 |= BS_DISABLED;
            break;

        case MSG_SETTEXT:
            vPortFree(pWnd->caption);
            len = strlen((char *)pParam2);

            pWnd->caption = pvPortMalloc(len + 1);
            if (len > 0)
                strcpy(pWnd->caption, (char *)pParam2);
            break;

        case MSG_GETTEXT:
            len = min(strlen((char *)pWnd->caption), pParam1);

            if (len > 0)
                strcpy((char *)pParam2, pWnd->caption);
            break;
            
        case MSG_NCPAINT:
            // Paint the window's non-client area.
            break;
            
        case MSG_PAINT:
            // Paint the window's client area.
            guiBeginPaint();

            if (pWnd->addData2 & BS_DISABLED)
                idx = pWnd->colorStyle.gryIdx;
            else if (pWnd->addData2 & BS_PRESSED)
                idx = pWnd->colorStyle.selIdx;
            else if (pWnd->addData2 & BS_HILITE)
                idx = pWnd->colorStyle.hlIdx;
            else
                idx = pWnd->colorStyle.shIdx;
            
            guiDrawStyleFrame(idx, &pWnd->clientFrame);

            // TODO: cos slabo rozpoznaje kollory tla i jak jest hl to i tak miksuje do sh
            // TODO: no i zla pozycja + rysowanie childow jakos inaczej zrobic w window.c
            guiDrawText(&pWnd->clientFrame, pWnd->caption, pWnd->font);

            guiEndPaint();
            break;

        case MSG_POINTERHOVER:
            pWnd->addData2 |= BS_HILITE;
            msgSend(pWnd, MSG_PAINT, 0, 0);
            break;

        case MSG_POINTERLEAVE:
            pWnd->addData2 &= ~BS_HILITE;
            msgSend(pWnd, MSG_PAINT, 0, 0);
            break;

        case MSG_POINTERDOWN:
            pWnd->addData2 |= BS_PRESSED;
            msgSend(pWnd, MSG_PAINT, 0, 0);
            break;
            
        case MSG_POINTERUP:
            pWnd->addData2 &= ~BS_PRESSED;
            msgSend(pWnd, MSG_PAINT, 0, 0);
            break;
    }
    
    return 0;
}        
