/**
 * @file board.c
 * @brief Functions related to MB1267A board
 * @author Daniel Szot
 */

#include <plib.h>
#include "FreeRTOSConfig.h"
#include "board.h"

//******************************************************************************
// Set configuration bits
//******************************************************************************
#pragma config FPLLMUL  = MUL_20        // PLL Multiplier
#pragma config FPLLIDIV = DIV_2         // PLL Input Divider
#pragma config FPLLODIV = DIV_1         // PLL Output Divider
#pragma config FPBDIV   = DIV_1         // Peripheral Clock divisor
#pragma config FWDTEN   = OFF           // Watchdog Timer
//#pragma config WDTPS    = PS1         // Watchdog Timer Postscale
#pragma config FCKSM    = CSDCMD        // Clk Switching & Fail Safe Clk Monitor
#pragma config OSCIOFNC = OFF           // CLKO Enable
#pragma config POSCMOD  = OFF           // Primary Oscillator
//#pragma config IESO     = ON          // Internal/External Switch-over
#pragma config FSOSCEN  = OFF           // Secondary Osc Enable (KLO was off)
#pragma config FNOSC    = FRCPLL        // Oscillator Selection
#pragma config CP       = OFF           // Code Protect
#pragma config BWP      = OFF           // Boot Flash Write Protect
#pragma config PWP      = OFF           // Program Flash Write Protect
#pragma config ICESEL   = ICS_PGx2      // ICE/ICD Comm Channel Select
#pragma config DEBUG    = OFF           // Background Debugger Enable
#pragma config FSRSSEL  = PRIORITY_7
/**
 * @brief Board initialization
 */
void boardInit()
{
    // Turn on the interrupts
    INTEnableSystemMultiVectoredInt();
    SYSTEMConfig(configCPU_CLOCK_HZ, SYS_CFG_ALL);
    SYSTEMConfigPerformance(configCPU_CLOCK_HZ);

    //DDPCONbits.JTAGEN = 0;
    //DDPCONbits.TROEN = 0;

    DDPCON = 0x00000003;

    //Setup ports directions
    //OUTPUT
    PORTSetPinsDigitalOut(IOPORT_D, BIT_2);
    
#ifndef LCD_FPGA
    LCD_BACKLIGHT_GPIO = 1;
#endif
    PORTSetPinsDigitalOut(IOPORT_A, BIT_0 | BIT_1 | BIT_10);
    PORTSetPinsDigitalOut(IOPORT_B, BIT_4 | BIT_5 | BIT_14 | BIT_15);
    PORTSetPinsDigitalOut(IOPORT_C, BIT_1);
    PORTSetPinsDigitalOut(IOPORT_D, BIT_12 | BIT_13 | BIT_6 | BIT_7);
    PORTSetPinsDigitalOut(IOPORT_E, BIT_0 | BIT_1 | BIT_2 | BIT_3 |
            BIT_4 | BIT_5 | BIT_6 | BIT_7);
    PORTSetPinsDigitalOut(IOPORT_F, BIT_0 | BIT_1);
    PORTSetPinsDigitalOut(IOPORT_G, BIT_0 | BIT_1);

    LED0 = 1;
    LED1 = 1;

    //INPUT
    //PORTSetPinsDigitalIn(IOPORT_A, BIT_10);
    PORTSetPinsDigitalIn(IOPORT_B, BIT_0 | BIT_1 | BIT_2 | BIT_3);
    PORTSetPinsDigitalIn(IOPORT_D, BIT_15 | BIT_9);
    PORTSetPinsDigitalIn(IOPORT_G, BIT_7);
    PORTSetPinsDigitalIn(IOPORT_F, BIT_4);

    PORTSetPinsAnalogIn(IOPORT_B, BIT_8);
    PORTSetPinsAnalogIn(IOPORT_B, BIT_10);
    PORTSetPinsAnalogIn(IOPORT_B, BIT_12);
    PORTSetPinsAnalogIn(IOPORT_B, BIT_13);

    DDPCON = 0x00000003;
    AD1PCFG = 0xFFFF;   // all PORTB inputs digital
}
