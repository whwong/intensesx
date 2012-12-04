/**
 * @file pic32UART.h
 * @brief RS232 Driver for PIC32 UART module
 * Driver based on PIC32 serial demo from FreeRTOS.
 * @author Daniel Szot
 */

#ifndef _PIC32UART_H
#define _PIC32UART_H

#include <p32xxxx.h>
#include <stdlib.h>
#include <plib.h>
#include "lib/common.h"
#include "board.h"
#include "hal/hld/uart.h"

struct lldPic32UARTPrivateData
{
    // Transmition status
    // After adding one or more chars this pointer is set to false
    // and interrupt flag of TX interrupt is being set. In ISR all chars
    // from queue are sending until queue will be empty. In this moment value
    // of this variable is set to true. With this feature all chars will be sent
    // at once until queue will be empty. See code to understand :).
    BOOL txHasEnded;
};

retcode lldPic32UARTAttach();

static retcode lldPic32UARTOpen(struct hldUartDevice *pUartDev, struct hldUartConfig *pCfg);
static retcode lldPic32UARTClose(struct hldUartDevice *pUartDev);

static retcode 
lldPic32UARTWriteByte(struct hldCharDevice *pCharDev, UINT8 pByte, UINT32 pTimeout);

static retcode
lldPic32UARTReadByte(struct hldCharDevice *pCharDev, UINT8 *pByte, UINT32 pTimeout);

#endif
