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
    BOOL isPointerInRect;
    UINT32 idx;
    struct guiWinStyle *ws;

    // Jeszcze ukrywanie przycisku trzeba zrobic
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

            msgSend(pWnd, MSG_PAINT, 0, 0);
            break;
            
        case MSG_NCPAINT:
            // Paint the window's non-client area.
            break;
            
        case MSG_PAINT:
            // Paint the window's client area.
            guiBeginPaint();

            if (pWnd->windowStyle & WS_VISIBLE)
            {
                // While button is disabled styles like hilite or pressed are
                // updated for button while touch down and touch up messages.
                // There we are always firstly check that button is disabled,
                // if it is just draw disabled button no matters on other states.
                if (pWnd->addData2 & BS_DISABLED)
                    idx = pWnd->colorStyle.gryIdx;
                else if (pWnd->addData2 & BS_PRESSED)
                    idx = pWnd->colorStyle.selIdx;
                else if (pWnd->addData2 & BS_HILITE)
                    idx = pWnd->colorStyle.hlIdx;
                else
                    idx = pWnd->colorStyle.shIdx;

                guiDrawStyleFrame(idx, &pWnd->clientFrame);

                ws = guiGetStyle(idx);
                if (ws->fgStyle == CS_SOLID)
                {
                    guiSetColor(ws->fgColor);
                    guiDrawText(&pWnd->clientFrame, pWnd->caption, pWnd->font,
                            FS_ALIGN_CENTER | FS_VALIGN_CENTER);
                }
            }

            guiEndPaint();
            break;

        case MSG_POINTERHOVER:
            if (pParam1 == 1)
                pWnd->addData2 |= BS_PRESSED;
            
            if (!(pWnd->addData2 & BS_DISABLED))
                msgSend(pWnd, MSG_PAINT, 0, 0);
            break;

        case MSG_POINTERLEAVE:
            if (pParam1 == 1)
                pWnd->addData2 &= ~BS_PRESSED;
                
            if (!(pWnd->addData2 & BS_DISABLED))
                msgSend(pWnd, MSG_PAINT, 0, 0);
            break;

        case MSG_POINTERDOWN:
            pWnd->addData2 |= BS_PRESSED;
            
            if (!(pWnd->addData2 & BS_DISABLED))
                msgSend(pWnd, MSG_PAINT, 0, 0);
            break;
            
        case MSG_POINTERUP:
            isPointerInRect = guiXYInRect((pParam2 & 0xFFFF),
                    ((pParam2 >> 16) & 0xFFFF), &pWnd->clientFrame);
            
            if ((pWnd->addData2 & BS_PRESSED) && isPointerInRect &&
                    (!(pWnd->addData2 & BS_DISABLED)))
            {
                BUTTON_NOTIFY_PARENT(pWnd, BN_CLICKED);
                pWnd->addData2 &= ~BS_PRESSED;
                msgSend(pWnd, MSG_PAINT, 0, 0);
            }
            else
            {
                pWnd->addData2 &= ~BS_PRESSED;
            }
            break;

        case MSG_SETFOCUS:
            if (!(pWnd->addData2 & BS_DISABLED))
            {
                pWnd->addData2 |= BS_HILITE;
                msgSend(pWnd, MSG_PAINT, 0, 0);
            }
            break;

        case MSG_KILLFOCUS:
            if (!(pWnd->addData2 & BS_DISABLED))
            {
                pWnd->addData2 &= ~BS_HILITE;
                msgSend(pWnd, MSG_PAINT, 0, 0);
            }
            break;
    }
    
    return guiDefWindowProc(pWnd, pMsg, pParam1, pParam2);
}        
