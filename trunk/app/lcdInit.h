/**
 * @file lcdInit.h
 * @brief Temporary module to test functionality of big 800x480 lcd screen
 * @author Daniel Szot
 */

#ifndef _BIG_SCREEN_LCD_H
#define _BIG_SCREEN_LCD_H

#include "lib/common.h"
#include "hal/hld/hld.h"
#include "hal/hld/i2c.h"

#define DEV_FIXED_ADDR  0xC0
#define DEV_HW_ADDR     0x08

#define DEV_ADDR   (DEV_FIXED_ADDR | DEV_HW_ADDR)

#define AUTO_INC    0x10

#define INPUT0  0x00
#define INPUT1  0x01
#define PSC0    0x02
#define PWM0    0x03
#define PSC1    0x04
#define PWM1    0x05
#define LS0     0x06
#define LS1     0x07
#define LS2     0x08
#define LS3     0x09

void bigScreenInit();

#endif
