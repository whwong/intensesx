/**
 * @file retcode.h
 * @brief Definitions of return codes
 * @author Daniel Szot
 */

#ifndef _RETCODE_H_
#define _RETCODE_H_

/// Values: < 0 - errors
///         = 0 - success
///         > 0 - warnings
typedef enum
{
    SUCCESS = 0,

    ERR_HLD_DEVICE_TYPE_EXISTS = -101,
    ERR_HLD_DEVICE_NAME_EXISTS = -100,
    WAR_HLD_DEVICE_NOT_FOUND = 100,

    ERR_HLD_DEVICE_UART_PORT_IN_USE = -101,
    ERR_HLD_DEVICE_UART_RX_QUEUE_EMPTY = -102,
    ERR_HLD_DEVICE_UART_TX_QUEUE_FULL = -103,

    ERR_HLD_DEVICE_ADC_CHANNEL_NOT_EXISTS = -104,
    ERR_ADC_DEVICE_NOT_FOUND = -105,

    ERR_NO_MEMMORY = -200,
    ERR_MUTEX = -201,
    ERR_QUEUE_FULL = -203,
    ERR_ALREADY_STARTED = -204,
    ERR_BUS = -205,
    ERR = -206, // Unknown or module specyfic error
    ERR_NOT_SUPPORTED = -207,
    ERR_FILE = -208,
    ERR_DEVICE_NOT_STARTED = -209,
    ERR_CHAR_NOT_IN_FONT = -210,
    ERR_INVALID_NAME = -211,
    ERR_NOT_FOUND = -212,
    ERR_FILE_SYSTEM = -213,
            
    // Retcodes for FatFS file system
    RES_OK = 0,         /* 0: Successful */
    RES_ERROR = -401,   /* 1: R/W Error */
    RES_WRPRT = -402,   /* 2: Write Protected */
    RES_NOTRDY = -403,  /* 3: Not Ready */
    RES_PARERR = -404,  /* 4: Invalid Parameter */

    DEVICE_NOT_SET = -300,
} retcode;

#endif
