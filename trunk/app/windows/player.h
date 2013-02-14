/**
 * @file player.h
 * @brief Player application header file
 * @author Daniel Szot
 */

#ifndef _APP_WINDOWS_PLAYER_H
#define _APP_WINDOWS_PLAYER_H

#include "lib/common.h"
#include "lib/log.h"
#include "lib/gui/gui.h"
#include "lib/user/msgDispatcher.h"
#include "lib/audio/audio1ch.h"

#include "app/rsc/rsc.h"

#include "app/lcdInit.h"

// Control indexes
#define IDC_WND_PLAYER  1
#define IDC_BTN_RPT     2
#define IDC_BTN_PREV    3
#define IDC_BTN_PLAY    4
#define IDC_BTN_NEXT    5
#define IDC_BTN_LIST    6
#define IDC_BTN_SET     7
#define IDC_BTN_VOL     8
#define IDC_PB_POS      9
#define IDC_PB_VOL      10
#define IDC_ST_TITLE    11
#define IDC_ST_DETAILS  12
#define IDC_ST_TITLE_BAR 13

// Size and position coeficients
#ifdef LCD_FPGA
#define SS_COEF_X (2.5f)
#define SS_COEF_Y (2)
#else
#define SS_COEF_X (1)
#define SS_COEF_Y (1)
#endif

// Window position and size
#define WND_PLAYER_X (0 * SS_COEF_X)
#define WND_PLAYER_Y (0 * SS_COEF_Y)
#define WND_PLAYER_W (320 * SS_COEF_X)
#define WND_PLAYER_H (240 * SS_COEF_Y)

// Control positions and sizes
#define BTN_RPT_X (12 * SS_COEF_X)
#define BTN_RPT_Y (166 * SS_COEF_Y)
#define BTN_RPT_W (54 * SS_COEF_X)
#define BTN_RPT_H (23 * SS_COEF_Y)

#define BTN_PREV_X (73 * SS_COEF_X)
#define BTN_PREV_Y (166 * SS_COEF_Y)
#define BTN_PREV_W (54 * SS_COEF_X)
#define BTN_PREV_H (31 * SS_COEF_Y)

#define BTN_PLAY_X (134 * SS_COEF_X)
#define BTN_PLAY_Y (166 * SS_COEF_Y)
#define BTN_PLAY_W (55 * SS_COEF_X)
#define BTN_PLAY_H (42 * SS_COEF_Y)

#define BTN_NEXT_X (195 * SS_COEF_X)
#define BTN_NEXT_Y (166 * SS_COEF_Y)
#define BTN_NEXT_W (54 * SS_COEF_X)
#define BTN_NEXT_H (31 * SS_COEF_Y)

#define BTN_LIST_X (255 * SS_COEF_X)
#define BTN_LIST_Y (166 * SS_COEF_Y)
#define BTN_LIST_W (54 * SS_COEF_X)
#define BTN_LIST_H (23 * SS_COEF_Y)

#define BTN_VOL_X (271 * SS_COEF_X)
#define BTN_VOL_Y (83 * SS_COEF_Y)
#define BTN_VOL_W (38 * SS_COEF_X)
#define BTN_VOL_H (23 * SS_COEF_Y)

#define BTN_SET_X (12 * SS_COEF_X)
#define BTN_SET_Y (83 * SS_COEF_Y)
#define BTN_SET_W (38 * SS_COEF_X)
#define BTN_SET_H (23 * SS_COEF_Y)

#define PB_POS_X (12 * SS_COEF_X)
#define PB_POS_Y (112 * SS_COEF_Y)
#define PB_POS_W (297 * SS_COEF_X)
#define PB_POS_H (23 * SS_COEF_Y)

#define PB_VOL_X (291 * SS_COEF_X)
#define PB_VOL_Y (12 * SS_COEF_Y)
#define PB_VOL_W (18 * SS_COEF_X)
#define PB_VOL_H (71 * SS_COEF_Y)

#define ST_TITLE_X (56 * SS_COEF_X)
#define ST_TITLE_Y (48 * SS_COEF_Y)
#define ST_TITLE_W (209 * SS_COEF_X)
#define ST_TITLE_H (35 * SS_COEF_Y)

#define ST_DETAILS_X (94 * SS_COEF_X)
#define ST_DETAILS_Y (138 * SS_COEF_Y)
#define ST_DETAILS_W (215 * SS_COEF_X)
#define ST_DETAILS_H (15 * SS_COEF_Y)

#define ST_TITLE_BAR_X (0 * SS_COEF_X)
#define ST_TITLE_BAR_Y (0 * SS_COEF_Y)
#define ST_TITLE_BAR_W (320 * SS_COEF_X)
#define ST_TITLE_BAR_H (20 * SS_COEF_Y)

void appPlayerShow();
retcode appPlayerInit();

#endif
