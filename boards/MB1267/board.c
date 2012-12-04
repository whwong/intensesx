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
#pragma config FPLLMUL  = MUL_15        // PLL Multiplier
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
#pragma config ICESEL   = ICS_PGx1      // ICE/ICD Comm Channel Select
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
    TRISA = 0x0000000;
    LCD_BACKLIGHT_GPIO = 0;
    PORTSetPinsDigitalOut(IOPORT_A, BIT_0 | BIT_1 | BIT_2 | BIT_3 | BIT_4 |
            BIT_5 | BIT_7 | BIT_8 | BIT_9 | BIT_10);
    PORTSetPinsDigitalOut(IOPORT_B,  BIT_0 | BIT_1 | BIT_8 | BIT_9 | BIT_10 |
            BIT_4 | BIT_6 | BIT_12 | BIT_13 | BIT_14);
    PORTSetPinsDigitalOut(IOPORT_C, BIT_1 | BIT_2 | BIT_3 | BIT_4 | BIT_14 |
            BIT_12);
    PORTSetPinsDigitalOut(IOPORT_D, BIT_11 | BIT_4 | BIT_5 | BIT_1| BIT_3);
    PORTSetPinsDigitalOut(IOPORT_E, BIT_9 | BIT_8);
    PORTSetPinsDigitalOut(IOPORT_F, BIT_12);
    PORTSetPinsDigitalOut(IOPORT_G, BIT_9 | BIT_15 | BIT_12 | BIT_13 | BIT_14);

    //INPUT
    PORTSetPinsDigitalIn(IOPORT_A, BIT_6);
    PORTSetPinsDigitalIn(IOPORT_B, BIT_11 | BIT_15 | BIT_5 | BIT_2 |
            BIT_3 | BIT_4);
    PORTSetPinsDigitalIn(IOPORT_D, BIT_15);
    PORTSetPinsDigitalIn(IOPORT_G, BIT_7);
    PORTSetPinsDigitalIn(IOPORT_F, BIT_4);

    PORTSetPinsAnalogIn(IOPORT_B, BIT_7);

    CNPUEbits.CNPUE7 = 1;
    DDPCON = 0x00000003;
}
