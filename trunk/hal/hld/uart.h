/**
 * @file uart.h
 * @brief UART High Level Driver - UART Hi level driver structure contains
 * queues which can be used by low level driver to implement buffered
 * version of driver - but it is not necessarily.
 * @author Daniel Szot
 */

#ifndef _HLD_UART_H_
#define _HLD_UART_H_

#include "hld.h"
#include "uart.h"
#include "char.h"
#include "lib/common.h"
#include "queue.h"

enum hldUartConfigDataSize
{
    UDS_8_BITS = 0x0000,
    UDS_9_BITS = 0x0006
};

enum hldUartConfigParity
{
    UP_ODD = 0x0004,
    UP_EVEN = 0x0002,
    UP_NONE = 0x0000
};

enum hldUartConfigStopBits
{
    USB_1,
    USB_2
};

// Configuration structure for UART device
struct hldUartConfig
{
    UINT8 portNumber;
    // Baud rate in bps like 115200
    UINT32 baudRate;
    enum hldUartConfigDataSize dataSize;
    enum hldUartConfigParity parity;
    enum hldUartConfigStopBits stopBits;

    // Size of receive queue
    UINT8 rxQueueSize;
    // Size of sending bytes queue
    UINT8 txQueueSize;

    // Priority of UART interrupt
    UINT8 intPriority;

    // Enables UART TX-to-RX Loop-back mode.
    UINT8 enableLoopback;
};


struct hldUartDevice
{
    struct hldCharDevice charHead;
    struct hldUartConfig config;

    // Queues to implement by lld used to communicate ISR with tasks
    // If you are implementing simple driver which is not using interrupts
    // and data buffering you needn't to use this but you have to provide
    // some method of synchronization
    xQueueHandle rxedCharsQueue;
    xQueueHandle charsForTxQueue;

    retcode (*attach)();
    retcode (*open)(struct hldUartDevice *pUartDev, struct hldUartConfig *pCfg);
    retcode (*close)(struct hldUartDevice *pUartDev);
    
};

#endif
