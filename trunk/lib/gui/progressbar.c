/**
 * @file progressbar.c
 * @brief Progress Bar control
 * @author Daniel Szot
 */

#include "progressbar.h"
#include "gui.h"
#include "lib/graphics/graphics.h"
#include "lib/input/keyCodes.h"
#include "lib/graphics/graphics.h"

#define PB_DEBUG
#if defined(PB_DEBUG)
#define PB_LOG(frm, ...) LOG("pb: "frm, ##__VA_ARGS__)
#else
#define PB_LOG(...)
#endif

#define PB_DATA(w) ((struct guiProgressBarData*)w->addData2)

struct guiProgressBarData
{
    INT32 min;
    INT32 max;
    INT32 pos;
};

void guiRegisterProgressBarClass()
{
    struct guiWndClassInfo wci;

    wci.className = "progressbar";
    wci.windowStyle = WS_CHILD | WS_VISIBLE;
    wci.colorStyle.shIdx = WSTL_PROGRESSBAR_SH;
    // For progress bar hl style is bar style
    wci.colorStyle.hlIdx = WSTL_PROGRESSBAR_BAR_SH;
    wci.colorStyle.selIdx = WSTL_PROGRESSBAR_SH;
    wci.colorStyle.gryIdx = WSTL_PROGRESSBAR_SH;
    wci.windowProc = guiDefProgressBarProc;

    guiRegisterWindowClass(&wci);
}

static void guiProgressBarPaint(struct guiWindow *pWnd)
{
    struct guiWinStyle *ws;
    struct guiRect barframe, bgframe;
    INT32 pos, barW;

    if (pWnd->windowStyle & WS_VISIBLE)
    {
        guiDrawStyleFrameBorders(pWnd->colorStyle.shIdx, &pWnd->clientFrame);
        ws = guiGetStyle(pWnd->colorStyle.shIdx);

        barframe = pWnd->clientFrame;
        barframe.l += ws->leftLineWidth;
        barframe.w -= ws->leftLineWidth + ws->rightLineWidth;
        barframe.t += ws->topLineWidth;
        barframe.h -= ws->topLineWidth + ws->bottomLineWidth;
        bgframe = barframe;

        pos = PB_DATA(pWnd)->pos;

        if (pWnd->windowStyle & PBS_VERTICAL)
        {
            barW = (pos - PB_DATA(pWnd)->min) * barframe.h;
            barW /= (PB_DATA(pWnd)->max - PB_DATA(pWnd)->min);
            barframe.t += (barframe.h - barW);
            barframe.h = barW;
            bgframe.h -= barW;
        }
        else
        {
            barW = (pos - PB_DATA(pWnd)->min) * barframe.w;
            barW /= (PB_DATA(pWnd)->max - PB_DATA(pWnd)->min);
            barframe.w = barW;
            bgframe.l += barW;
            bgframe.w -= barW;
        }

        guiDrawStyleFrameBg(pWnd->colorStyle.shIdx, &bgframe, TRUE);
        guiDrawStyleFrameBg(pWnd->colorStyle.hlIdx, &barframe, FALSE);

        if (ws->fgStyle == CS_SOLID)
        {
            guiSetColor(ws->fgColor);
            guiDrawText(&pWnd->clientFrame, pWnd->caption, pWnd->font,
                    FS_ALIGN_CENTER | FS_VALIGN_CENTER);
        }
    }
}

static void guiProgressBarNormalize(struct guiWindow *pWnd, INT32 pPos, BOOL pNotify)
{
    if (pPos >= PB_DATA(pWnd)->max)
    {
        PB_DATA(pWnd)->pos = PB_DATA(pWnd)->max;
        if (pNotify)
            PROGRESSBAR_NOTIFY_PARENT(pWnd, PBN_REACHMAX);
    }

    if (pPos <= PB_DATA(pWnd)->min)
    {
        PB_DATA(pWnd)->pos = PB_DATA(pWnd)->min;
        if (pNotify)
            PROGRESSBAR_NOTIFY_PARENT(pWnd, PBN_REACHMIN);
    }
}

static void guiProgressBarSetPos(struct guiWindow *pWnd, UINT32 pPos)
{
    if (PB_DATA(pWnd)->pos != pPos)
    {
        PB_DATA(pWnd)->pos = pPos;
        guiProgressBarNormalize(pWnd, (INT32)pPos, (pWnd->windowStyle & PBS_NOTIFY));
        msgSend(pWnd, MSG_PAINT, 0, 0);
    }
}

static void guiProgressBarDeltaPos(struct guiWindow *pWnd, UINT32 pDelta)
{
    if (pDelta != 0)
    {
        PB_DATA(pWnd)->pos += (INT32)pDelta;
        guiProgressBarNormalize(pWnd, PB_DATA(pWnd)->pos,
            (pWnd->windowStyle & PBS_NOTIFY));
        msgSend(pWnd, MSG_PAINT, 0, 0);
    }
}

static INT32 guiProgressBarSetRange(struct guiWindow *pWnd, UINT32 pMin, UINT32 pMax)
{
    if (pMin < pMax)
    {
        PB_DATA(pWnd)->min = (INT32)pMin;
        PB_DATA(pWnd)->max = (INT32)pMax;
        guiProgressBarNormalize(pWnd, PB_DATA(pWnd)->pos,
            (pWnd->windowStyle & PBS_NOTIFY));
        msgSend(pWnd, MSG_PAINT, 0, 0);
        return 0;
    }
    else
        return -1;
}

static INT32 guiProgressBarPointerMove(struct guiWindow *pWnd, UINT32 pMsg,
        UINT32 pParam1, UINT32 pParam2)
{
    INT32 pos, bw, pointer;

    // if pointer dev button is holded
    if ((pWnd->windowStyle & PBS_ALLOW_DRAGGING) && (pParam1 & 0xffff) == 0x01)
    {
        // get pointer x position
        // TODO: get y when vertical
        if (pWnd->windowStyle & PBS_VERTICAL)
        {
            pointer = (INT32)((pParam2 >> 16) & 0xffff);
            bw = (pWnd->clientFrame.t + pWnd->clientFrame.h) - pointer;
            pos = (bw * (PB_DATA(pWnd)->max - PB_DATA(pWnd)->min));
            pos /= pWnd->clientFrame.h;
            pos += PB_DATA(pWnd)->min;
        }
        else
        {
            pointer = (INT32)(pParam2 & 0xffff);
            bw = pointer - pWnd->clientFrame.l;
            pos = (bw * (PB_DATA(pWnd)->max - PB_DATA(pWnd)->min));
            pos /= pWnd->clientFrame.w;
            pos += PB_DATA(pWnd)->min;
        }

        if (pos != PB_DATA(pWnd)->pos)
        {
            msgSend(pWnd, PBM_SETPOS, pos, 0);
            if ((pWnd->windowStyle & PBS_NOTIFY))
                PROGRESSBAR_NOTIFY_PARENT(pWnd, PBN_CHANGED);
        }
    }
}

INT32 guiDefProgressBarProc(struct guiWindow *pWnd, UINT32 pMsg,
        UINT32 pParam1, UINT32 pParam2)
{
    BOOL isPointerInRect;

    // Jeszcze ukrywanie przycisku trzeba zrobic
    switch (pMsg)
    {
        case MSG_CREATE:
            pWnd->addData2 = (UINT32)pvPortMalloc(sizeof(struct guiProgressBarData));

            if ((struct guiProgressBarData *)pWnd->addData2 == NULL)
                return -1;
            
            PB_DATA(pWnd)->max = 100;
            PB_DATA(pWnd)->min = 0;
            PB_DATA(pWnd)->pos = 0;
            break;
            
        case MSG_DESTROY:
            vPortFree(PB_DATA(pWnd));
            break;
            
        case MSG_NCPAINT:
            // Paint the window's non-client area.
            break;
            
        case MSG_PAINT:
            // Paint the window's client area.
            guiBeginPaint();
            guiProgressBarPaint(pWnd);
            guiEndPaint();
            break;

        case MSG_POINTERDOWN:
            pWnd->windowStyle |= PBS_PRESSED;
            break;
            
        case MSG_POINTERUP:
            isPointerInRect = guiXYInRect((pParam2 & 0xFFFF),
                    ((pParam2 >> 16) & 0xFFFF), &pWnd->clientFrame);
            
            if ((pWnd->windowStyle & PBS_PRESSED) && isPointerInRect)
            {
                if ((pWnd->windowStyle & PBS_NOTIFY))
                    PROGRESSBAR_NOTIFY_PARENT(pWnd, PBN_CLICKED);
                pWnd->windowStyle &= ~PBS_PRESSED;
            }
            else
            {
                pWnd->windowStyle &= ~PBS_PRESSED;
            }
            break;

        case MSG_POINTERMOVE:
            guiProgressBarPointerMove(pWnd, pMsg, pParam1, pParam2);
            break;

        case PBM_SETPOS:
            guiProgressBarSetPos(pWnd, pParam1);
            return 0;
            break;

        case PBM_GETPOS:
            (*(UINT32*)pParam1) = PB_DATA(pWnd)->pos;
            return 0;
            break;

        case PBM_DELTAPOS:
            guiProgressBarDeltaPos(pWnd, pParam1);
            return 0;
            break;

        case PBM_SETRANGE:
            return guiProgressBarSetRange(pWnd, pParam1, pParam2);
            break;
    }
    
    return guiDefWindowProc(pWnd, pMsg, pParam1, pParam2);
}        
