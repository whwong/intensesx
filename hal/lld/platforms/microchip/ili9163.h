/**
 * @file lcd.h
 * @brief ILI9163 LCD controller driver for Microchip PIC32 processors family
 * Screen resolution is 128x160
 * Color format is RGB565
 * @author Daniel Szot
 */

#ifndef _ILI9163_H
#define _ILI9163_H

#include <p32xxxx.h>
#include <stdlib.h>
#include <plib.h>
#include "lib/common.h"
#include "gpio.h"
#include "board.h"
#include "hal/hld/lcd.h"

#if !defined(LCD_BACKLIGHT_GPIO) || !defined(LCD_RESET_GPIO)
    #error "Please define LCD_BACKLIGHT_GPIO and LCD_RESET_GPIO macros in \
           board.h before using ILI9163 LCD driver."
#endif

//#define ILI9163_DOUBLE_BUFFERED_DRIVER
#define ILI9163_DMA_CHANNEL 1

#define RGB888TORGB565(r, g, b) (((r >> 3) << 11) | ((g >> 2) << 5) | ((b >> 3)))

#define CONTROL (PMP_ON | PMP_IDLE_CON | PMP_MUX_OFF | PMP_ST | PMP_READ_WRITE_EN |\
                 PMP_CS2_CS1_EN | PMP_LATCH_POL_LO | PMP_CS2_POL_LO | PMP_CS1_POL_HI |\
                 PMP_WRITE_POL_LO | PMP_READ_POL_LO)

#define MODE    (PMP_IRQ_READ_WRITE | PMP_AUTO_ADDR_OFF | PMP_DATA_BUS_8 | PMP_MODE_MASTER2 |\
                 PMP_WAIT_BEG_3 | PMP_WAIT_MID_7 | PMP_WAIT_END_3)

#define PORT    (PMP_PEN_0 | PMP_PEN_14 | PMP_PEN_15)

#define INTERRUPT (PMP_INT_OFF)

#define DATA_ADDRESS 	0x8001
#define COMMAND_ADDRESS 0x8000
#define LEDS_ADDRESS	0x4000

#define lcdCommand(cmd) (PMPSetAddress(COMMAND_ADDRESS), PMPMasterWrite((cmd)))
#define lcdData(data)   (PMPSetAddress(DATA_ADDRESS),               \
                            PMPMasterWrite((data & 0xFF00) >> 8),   \
                            PMPMasterWrite((data & 0x00FF)))
#define lcdData8(data) (PMPSetAddress(DATA_ADDRESS), PMPMasterWrite(data))

#define pmpAddress(addr) (PMPSetAddress((addr)))
#define pmpWrite(data) (PMPMasterWrite((data & 0xFF00) >> 8),       \
                            PMPMasterWrite((data & 0x00FF)))
#define pmpRead() (PMPMasterRead())

#define lcdOn() (LCD_RESET = 1)
#define lcdOff() (LCD_RESET = 0)

retcode lldIli9163Attach();
retcode lldIli9163Fill(struct hldLcdDevice *pLcdDev,
    UINT16 pX1, UINT16 pY1, UINT16 pX2, UINT16 pY2);
retcode lldIli9163Open(struct hldLcdDevice *pLcdDev);
retcode lldIli9163Close(struct hldLcdDevice *pLcdDev);
retcode lldIli9163SetColor(struct hldLcdDevice *pLcdDev, UINT8 pA, UINT8 pR,
        UINT8 pG, UINT8 pB);
retcode lldIli9163DrawPixel(struct hldLcdDevice *pLcdDev, UINT16 pX, UINT16 pY);
retcode lldIli9163Flush(struct hldLcdDevice *pLcdDev);
inline UINT32 lldIli9163GetMaxX(struct hldLcdDevice *pLcdDev);
inline UINT32 lldIli9163GetMaxY(struct hldLcdDevice *pLcdDev);

#endif
