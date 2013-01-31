/**
 * @file board.h
 * @brief Definitions dedicated to MB1267 board
 * @author Daniel Szot
 */

#ifndef _BOARD_H_
#define _BOARD_H_

//******************************************************************************
// BOARD Configuration
//******************************************************************************
#define LCD_ORIENTATION 90
#define LCD_FPGA
#define LCD_FPGA_TEST

//******************************************************************************
// GPIO Definitions
//******************************************************************************

// LCD
#ifdef LCD_FPGA
#define LCD_BACKLIGHT_GPIO LATDbits.LATD2 // not used
#define LCD_RESET_GPIO LATCbits.LATC1 // not used
#define LCD_RS_GPIO LATBbits.LATB5
#define LCD_CS_GPIO LATAbits.LATA10
#else
#define LCD_BACKLIGHT_GPIO LATDbits.LATD2
#define LCD_RESET_GPIO LATCbits.LATC1
#define LCD_RS_GPIO LATBbits.LATB15
#define LCD_CS_GPIO LATBbits.LATB14
#endif

// TOUCH SCREEN
// Required if you would like to use lld/resistiveTouch.c driver
#define TOUCH_ADC_CH_XPOS     12
#define TOUCH_ADC_CH_YPOS     13
#define TOUCH_ADC_CH_YNEG     10
// Define as MAX + 1 (2^resolution_in_bits)
#define TOUCH_ADC_MAX         1024
// X port definitions
#define TOUCH_LAT_XNEG        LATBbits.LATB11
#define TOUCH_LAT_XPOS        LATBbits.LATB13
#define TOUCH_TRIS_XNEG       TRISBbits.TRISB11
#define TOUCH_TRIS_XPOS       TRISBbits.TRISB13
// Y port definitions
#define TOUCH_LAT_YNEG        LATBbits.LATB10
#define TOUCH_LAT_YPOS        LATBbits.LATB12
#define TOUCH_TRIS_YNEG       TRISBbits.TRISB10
#define TOUCH_TRIS_YPOS       TRISBbits.TRISB12

// LEDS
#define LED0 LATAbits.LATA0
#define LED1 LATAbits.LATA1
//define LED2 LATDbits.LATD9

//******************************************************************************
// Fnctions Declarations
//******************************************************************************
void boardInit();

#endif
