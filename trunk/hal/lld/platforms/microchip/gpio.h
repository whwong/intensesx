/**
 * @file gpio.h
 * @brief GPIO driver for Michrochip PIC uC
 * @author Daniel Szot
 */

#ifndef _MICROCHIP_PIC_GPIO_H_
#define _MICROCHIP_PIC_GPIO_H_

#include "lib/common.h"
#include <peripheral/ports.h>

// Particular bits
#define BIT_31                       (1 << 31)
#define BIT_30                       (1 << 30)
#define BIT_29                       (1 << 29)
#define BIT_28                       (1 << 28)
#define BIT_27                       (1 << 27)
#define BIT_26                       (1 << 26)
#define BIT_25                       (1 << 25)
#define BIT_24                       (1 << 24)
#define BIT_23                       (1 << 23)
#define BIT_22                       (1 << 22)
#define BIT_21                       (1 << 21)
#define BIT_20                       (1 << 20)
#define BIT_19                       (1 << 19)
#define BIT_18                       (1 << 18)
#define BIT_17                       (1 << 17)
#define BIT_16                       (1 << 16)
#define BIT_15                       (1 << 15)
#define BIT_14                       (1 << 14)
#define BIT_13                       (1 << 13)
#define BIT_12                       (1 << 12)
#define BIT_11                       (1 << 11)
#define BIT_10                       (1 << 10)
#define BIT_9                        (1 << 9)
#define BIT_8                        (1 << 8)
#define BIT_7                        (1 << 7)
#define BIT_6                        (1 << 6)
#define BIT_5                        (1 << 5)
#define BIT_4                        (1 << 4)
#define BIT_3                        (1 << 3)
#define BIT_2                        (1 << 2)
#define BIT_1                        (1 << 1)
#define BIT_0                        (1 << 0)

/// Describes uC ports
typedef enum
{
    #ifdef _PORTA
        GPIO_PORT_A,
    #endif
    #ifdef _PORTB
        GPIO_PORT_B,
    #endif
    #ifdef _PORTC
        GPIO_PORT_C,
    #endif
    #ifdef _PORTD
        GPIO_PORT_D,
    #endif
    #ifdef _PORTE
        GPIO_PORT_E,
    #endif
    #ifdef _PORTF
        GPIO_PORT_F,
    #endif
    #ifdef _PORTG
        GPIO_PORT_G,
    #endif
	GPIO_PORT_NUM
} lldGpioPorts;

/// Sets bits b in port p as a digital input
#define lldGpioSetDigitalIn(p, b)   PORTSetPinsDigitalIn(p, b)
/// Sets bits b in port p as a digital output
#define lldGpioSetDigitalOut(p, b)  PORTSetPinsDigitalOut(p, b)
/// Sets bits b in port p as a analog input
#define lldGpioSetAnalogIn(p, b)    PORTSetPinsAnalogIn(p, b)
/// Sets bits b in port p as a analog output
#define lldGpioSetAnalogOut(p, b)   PORTSetPinsAnalogOut(p, b)

/// Reads value of port p
#define lldGpioReadPort(p)      PORTRead(p)
/// Writes value b to port p
#define lldGpioWritePort(p, b)  PORTWrite(p, b)

/// Set bits b from port p
#define lldGpioSetBits(p, b)    PORTSetBits(p, b)
/// Clear bits b from port p
#define lldGpioClearBits(p, b)  PORTClearBits(p, b)
/// Toggle bits b from port p
#define lldGpioToggleBits(p, b) PORTToggleBits(p, b)

// Write state s to gpio g
#define lldGpioWriteState(g, s) g = s
// Reads state from gpio g
#define lldGpioReadState(g)     g

/// GPIO module configuration struct
typedef struct
{
    #ifdef _PORTA
        UINT32 portADir;
    #endif
    #ifdef _PORTB
        UINT32 portBDir;
    #endif
    #ifdef _PORTC
        UINT32 portCDir;
    #endif
    #ifdef _PORTD
        UINT32 portDDir;
    #endif
    #ifdef _PORTE
        UINT32 portEDir;
    #endif
    #ifdef _PORTF
        UINT32 portFDir;
    #endif
    #ifdef _PORTG
        UINT32 portGDir;
    #endif
} lldGpioConfig;

void lldGpioInit(lldGpioConfig *cfg);

#endif
