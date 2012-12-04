/**
 * @file board.h
 * @brief Definitions dedicated to MB1267 board
 * @author Daniel Szot
 */

#ifndef _BOARD_H_
#define _BOARD_H_

//******************************************************************************
// GPIO definitions
//******************************************************************************
#define LCD_BACKLIGHT_GPIO LATDbits.LATD1
#define LCD_RESET_GPIO LATCbits.LATC14

#define LED66 LATEbits.LATE9

//******************************************************************************
// Fnctions declarations
//******************************************************************************
void boardInit();

#endif
