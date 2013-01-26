/**
 * @file progressbar.h
 * @brief Progress Bar control
 * @author Daniel Szot
 */

#ifndef _LIB_GUI_PROGRESSBAR_H
#define _LIB_GUI_PROGRESSBAR_H

#include "lib/common.h"
#include "guidefs.h"
#include "lib/user/message.h"

/**
 * @brief Notifies the parent window.
 *
 * Sends window notification messages to the parent when
 * the user clicks the control.
 */
#define PBS_NOTIFY 0x0001

/**
 * @brief Allows dragging by user using pointer device
 */
#define PBS_ALLOW_DRAGGING 0x0002

/**
 * @brief Makes progress bar vertical
 */
#define PBS_VERTICAL 0x0004

/**
 * @brief Identifies that PB is pressed
 *
 * Internal state for progressbar to identify that it was pressed by user
 */
#define PBS_PRESSED 0x0100

/**
 * @brief Sets range of PB max and min
 *
 * Sets the upper and lower limits of the progress bar control's range,
 * and redraws the bar to show the new ranges.
 *
 * @param pParam1 Lower limit of the progress bar.
 * @param pParam2 Upper limit of the progress bar.
 */
#define PBM_SETRANGE (MSG_USER + 1)

/**
 * @brief Sets the progress bar current position.
 *
 * Sets the progress bar current position,
 * and redraw the bar to show the new position.
 *
 * @param pParam1 The progress bar current position.
 */
#define PBM_SETPOS (MSG_USER + 2)

/**
 * @brief Advances the progress bar current position.
 *
 * Advances the progress bar current position as specified by posInc,
 * and redraw the bar to reflect the new position.
 *
 * @param pParam1 Progress bar position increment
 */
#define PBM_DELTAPOS (MSG_USER + 3)

/**
 * @brief Gets the progress bar current position.
 *
 * @param pParam1 Output parameter. Give there pointer to INT32. There
 * will be saved current position of PB
 */
#define PBM_GETPOS (MSG_USER + 4)

/**
 * @brief Notifies reach of maximum limit.
 *
 * The PBN_REACHMAX notification code is sent when the progressbar reachs its maximum limit.
 */
#define PBN_REACHMAX 1

/**
 * @brief Notifies reach of minimum limit.
 * 
 * The PBN_REACHMIN notification code is sent when the progressbar reachs its minimum limit.
 */
#define PBN_REACHMIN 2

/**
 * @brief Notifies clicking by user
 * 
 * The PBN_CLICKED notification code is sent when the user clicks on PB
 */
#define PBN_CLICKED 3

/**
 * @brief Notifies pos changed
 *
 * The PBN_CHANGED notification code is sent when PB pos was changed by dragging
 */
#define PBN_CHANGED 4

#define PROGRESSBAR_NOTIFY_PARENT(wnd, code) msgPost((struct guiWindow *)wnd->mainWin, \
        MSG_COMMAND, ((code << 16) | (wnd->id & 0xFFFF)), (UINT32)wnd)

void guiRegisterProgressBarClass();
INT32 guiDefProgressBarProc(struct guiWindow *pWnd, UINT32 pMsg,
        UINT32 pParam1, UINT32 pParam2);

#endif
