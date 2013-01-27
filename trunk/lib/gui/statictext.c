/**
 * @file statictext.c
 * @brief Static Text control
 * @author Daniel Szot
 */

#include "statictext.h"
#include "gui.h"
#include "lib/graphics/graphics.h"
#include "lib/input/keyCodes.h"

void guiRegisterStaticTextClass()
{
    struct guiWndClassInfo wci;

    wci.className = "statictext";
    wci.windowStyle = WS_CHILD | WS_VISIBLE;
    wci.colorStyle.shIdx = WSTL_STATICTEXT_SH;
    wci.colorStyle.hlIdx = WSTL_STATICTEXT_SH;
    wci.colorStyle.selIdx = WSTL_STATICTEXT_SH;
    wci.colorStyle.gryIdx = WSTL_STATICTEXT_GRY;
    wci.windowProc = guiDefStaticTextProc;

    guiRegisterWindowClass(&wci);
}

INT32 guiDefStaticTextProc(struct guiWindow *pWnd, UINT32 pMsg,
        UINT32 pParam1, UINT32 pParam2)
{
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
                pWnd->addData2 &= ~SS_DISABLED;
            else
                pWnd->addData2 |= SS_DISABLED;

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
                if (pWnd->windowStyle & SS_DISABLED)
                    idx = pWnd->colorStyle.gryIdx;
                else
                    idx = pWnd->colorStyle.shIdx;

                guiDrawStyleFrame(idx, &pWnd->clientFrame);

                ws = guiGetStyle(idx);
                if (ws->fgStyle == CS_SOLID)
                {
                    guiSetColor(ws->fgColor);

                    if (pWnd->windowStyle & SS_RIGHT)
                    {
                        guiDrawText(&pWnd->clientFrame, pWnd->caption, pWnd->font,
                                FS_ALIGN_RIGHT | FS_VALIGN_CENTER);
                    }
                    else if (pWnd->windowStyle & SS_CENTER)
                    {
                        guiDrawText(&pWnd->clientFrame, pWnd->caption, pWnd->font,
                                FS_ALIGN_CENTER | FS_VALIGN_CENTER);
                    }
                    else
                    {
                        guiDrawText(&pWnd->clientFrame, pWnd->caption, pWnd->font,
                                FS_ALIGN_LEFT | FS_VALIGN_CENTER);
                    }
                }
            }

            guiEndPaint();
            break;
    }
    
    return guiDefWindowProc(pWnd, pMsg, pParam1, pParam2);
}        
