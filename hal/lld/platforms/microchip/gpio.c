/**
 * @file gpio.c
 * @brief GPIO driver for Michrochip PIC uC
 * @author Daniel Szot
 */

#include "gpio.h"

/**
 * @brief GPIO peripherial initialization
 * @param cfg Configuration struct (describes ports directions)
 */
void lldGpioInit(lldGpioConfig *cfg)
{
    #ifdef _PORTA
        TRISA = cfg->portADir;
    #endif
    #ifdef _PORTB
        TRISB = cfg->portBDir;
    #endif
    #ifdef _PORTC
        TRISC = cfg->portCDir;
    #endif
    #ifdef _PORTD
        TRISD = cfg->portDDir;
    #endif
    #ifdef _PORTE
        TRISE = cfg->portEDir;
    #endif
    #ifdef _PORTF
        TRISF = cfg->portFDir;
    #endif
    #ifdef _PORTG
        TRISG = cfg->portGDir;
    #endif
}
