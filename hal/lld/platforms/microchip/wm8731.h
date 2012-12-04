/**
 * @file wm8731.h
 * @brief Driver for WM8731 codec specialised for PIC32 on SPI3A and I2C2
 * @note Driver based on Lucio Di Jasio driver for PIC32MX4 MMB 
 * @author Daniel Szot
 */

#ifndef _PIC32_WM8731_H
#define _PIC32_WM8731_H

#include "lib/common.h"
#include "board.h"
#include "hal/hld/audio.h"

// Low level wm8731 interface defines
#define	WM8731_ADDRESS              0x1a

#define	WM8731_REG_LLINE_IN         0x0
#define	WM8731_REG_RLINE_IN         0x1
#define	WM8731_REG_LHPHONE_OUT      0x2
#define	WM8731_REG_RHPHONE_OUT      0x3
#define	WM8731_REG_ANALOG_PATH      0x4
#define	WM8731_REG_DIGITAL_PATH     0x5
#define	WM8731_REG_PDOWN_CTRL       0x6
#define	WM8731_REG_DIGITAL_IF       0x7
#define	WM8731_REG_SAMPLING_CTRL    0x8
#define	WM8731_REG_ACTIVE_CTRL      0x9
#define	WM8731_REG_RESET            0xf

#define WM8731_I2C_CLOCK            100000  // standard I2C bus frequency

retcode lldWm8731Attach(struct hldAudioConfig *pCfg);

static retcode lldWm8731Open(struct hldAudioDevice *pAudioDev);
static retcode lldWm8731Close(struct hldAudioDevice *pAudioDev);

static retcode
lldWm8731Ioctl(struct hldAudioDevice *pAudioDev, UINT32 pCmd, UINT32 pParam);

#endif
