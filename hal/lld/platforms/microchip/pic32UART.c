/**
 * @file pic32UART.c
 * @brief RS232 Driver for PIC32 UART module
 * Driver based on PIC32 serial demo from FreeRTOS.
 * @author Daniel Szot
 */

#include "lib/common.h"
#include "task.h"
#include "queue.h"
#include "hal/hld/hld.h"
#include "pic32UART.h"
#include "hal/hld/char.h"
#include "lib/input/inputEvent.h"

static char lldPic32UARTName[HLD_DEVICE_MAX_NAME_LEN] = "PIC32 UART Driver";

// Let's set function lldPic32UARTIntWrapper as a handler to all uart
// interrupt vectors.
//
// The UART interrupt handler.  As this uses the FreeRTOS assembly interrupt
// entry point the IPL setting in the following prototype has no effect.  The
// interrupt priority is set by the call to  INTSetVectorPriority() in
// lldPic32UARTOpen().
void __attribute__( (interrupt(ipl1), vector(_UART_1_VECTOR))) lldPic32UARTIntWrapper( void );
//void __attribute__( (interrupt(ipl1), vector(_UART_2_VECTOR))) lldPic32UARTIntWrapper( void );

/**
 * @brief Attach Pic32 UART device
 *
 * @return Result value
 */
retcode lldPic32UARTAttach()
{
    struct hldUartDevice *dev;
    char devName[HLD_DEVICE_MAX_NAME_LEN];
    static UINT8 devNumber = 0;
    struct lldPic32UARTPrivateData *priv;

    sprintf(devName, "%s #%d", lldPic32UARTName, devNumber);
    devNumber++;

    dev = hldDeviceAlloc(devName, HLD_DEVICE_TYPE_UART,
            sizeof(struct hldUartDevice));

    if (dev == NULL)
        return ERR_NO_MEMMORY;

    priv = pvPortMalloc(sizeof(struct lldPic32UARTPrivateData));

    if (priv == NULL)
    {
        hldDeviceFree(dev);
        return ERR_NO_MEMMORY;
    }
    
    priv->txHasEnded = TRUE;
    dev->charHead.head.priv = priv;
    dev->attach =   lldPic32UARTAttach;
    dev->open =     lldPic32UARTOpen;
    dev->close =    lldPic32UARTClose;
    dev->charHead.write =  lldPic32UARTWriteByte;
    dev->charHead.read =   lldPic32UARTReadByte;
    
    retcode result = hldDeviceRegister(dev);
    if (result != SUCCESS)
    {
        hldDeviceFree(dev);
    }
    else
    {
        dev->charHead.head.state |= HLD_DEVICE_STATE_TURNED_ON;
    }

    return result;
}

/**
 * @brief Open device, enable interrupts, create queues etc.
 *
 * @param pUartDev Pointer to device which will be opened
 * @return Result value
 */
static retcode
lldPic32UARTOpen(struct hldUartDevice *pUartDev, struct hldUartConfig *pCfg)
{
    UINT8 portNumber;
    struct hldUartDevice * devNode = NULL;

    // Setting up config for device
    pUartDev->config = *pCfg;

    // Checking that selected port number is not currently used
    devNode = (struct hldUartDevice*)hldDeviceGetFirst();
    while (devNode != NULL)
    {
        if ((devNode->charHead.head.type == HLD_DEVICE_TYPE_UART) &&
                (devNode != pUartDev) &&
                (devNode->config.portNumber == pUartDev->config.portNumber) &&
                ((devNode->charHead.head.state & HLD_DEVICE_STATE_RUNNING) != 0))
        {
            ERROR("LLD PIC32 UART port #%d already in use", pUartDev->config.portNumber);
            return ERR_HLD_DEVICE_UART_PORT_IN_USE;
        }

        devNode = (struct hldUartDevice*)devNode->charHead.head.next;
    }

    // Creating queues
    pUartDev->rxedCharsQueue = xQueueCreate( pUartDev->config.rxQueueSize,
            ( unsigned portBASE_TYPE ) sizeof( UINT8 ) );
    pUartDev->charsForTxQueue = xQueueCreate( pUartDev->config.txQueueSize,
            ( unsigned portBASE_TYPE ) sizeof( UINT8 ) );

    portNumber = pUartDev->config.portNumber-1;

    // UART Peripherial
    UARTConfigure(portNumber, UART_ENABLE_PINS_TX_RX_ONLY ); // | UART_RTS_WHEN_TX_NOT_EMPTY
    UARTSetFifoMode(portNumber, UART_INTERRUPT_ON_TX_NOT_FULL | UART_INTERRUPT_ON_RX_NOT_EMPTY);

    UARTSetLineControl(portNumber, pUartDev->config.dataSize |
            pUartDev->config.parity |
            pUartDev->config.stopBits);

    UARTSetDataRate(portNumber, configPERIPHERAL_CLOCK_HZ, pUartDev->config.baudRate);

    UARTEnable(portNumber, UART_ENABLE_FLAGS(UART_PERIPHERAL | UART_RX | UART_TX));

    // Interrupts
    INTEnable(INT_SOURCE_UART_RX(portNumber), INT_ENABLED);
    INTEnable(INT_SOURCE_UART_TX(portNumber), INT_ENABLED);

    INTSetVectorPriority(INT_VECTOR_UART(portNumber), pUartDev->config.intPriority);
    INTSetVectorSubPriority(INT_VECTOR_UART(portNumber), INT_SUB_PRIORITY_LEVEL_0);

    pUartDev->charHead.head.state |= HLD_DEVICE_STATE_RUNNING;
    
    DONE("LLD PIC 32 UART port #%d opened:", pUartDev->config.portNumber);
    LOG("    Baud rate: %d", pUartDev->config.baudRate);
    LOG("    Data size: %s", (pUartDev->config.dataSize == UDS_8_BITS) ?
        "UDS_8_BITS" : "UDS_9_BITS");
    
    switch(pUartDev->config.parity)
    {
        case UP_ODD:
            LOG("    Parity: UP_ODD");
            break;
            
        case UP_EVEN:
            LOG("    Parity: UP_EVEN");
            break;
        
        default:
            LOG("    Parity: UP_NONE");
    }
    
    LOG("    Stop bits: %s", ((pUartDev->config.stopBits == USB_1) ?
        "USB_1" : "USB_2"));
    LOG("    Interrupt priority: %d", pUartDev->config.intPriority);
    LOG("    RX queue size: %d", pUartDev->config.rxQueueSize);
    LOG("    TX queue size: %d", pUartDev->config.txQueueSize);
    LOG("    Loopback: %d", pUartDev->config.enableLoopback);
    
    return SUCCESS;
}

/**
 * @brief Close device, disable and delete everything :)
 *
 * @param pUartDev Pointer to device which will be closed
 * @return Result value
 */
static retcode lldPic32UARTClose(struct hldUartDevice *pUartDev)
{
    UINT8 portNumber;
    pUartDev->charHead.head.state &= ~(HLD_DEVICE_STATE_RUNNING |
            HLD_DEVICE_STATE_TURNED_ON);

    portNumber = pUartDev->config.portNumber-1;
    UARTEnable(portNumber, UART_DISABLE_FLAGS(UART_PERIPHERAL | UART_RX | UART_TX));

    vQueueDelete(pUartDev->rxedCharsQueue);
    vQueueDelete(pUartDev->charsForTxQueue);

    LOG("LLD PIC 32 UART port #%d closed", pUartDev->config.portNumber);
    return SUCCESS;
}

/**
 * @brief Send single byte to device
 *
 * @param pUartDev Pointer to device which will be used
 * @return Result value
 */
static retcode 
lldPic32UARTWriteByte(struct hldCharDevice *pCharDev, UINT8 pByte, UINT32 pTimeout)
{
    struct lldPic32UARTPrivateData *priv;
    priv = (struct lldPic32UARTPrivateData *)(pCharDev->head.priv);

    // Return error if after time pTimeout there is still no empty space in queue
    if(xQueueSend(((struct hldUartDevice *)pCharDev)->charsForTxQueue, &pByte, pTimeout) != pdPASS)
    {
        return ERR_HLD_DEVICE_UART_TX_QUEUE_FULL;
    }

    // If we are currently not sending nothing lets force sending process
    if(priv->txHasEnded == TRUE)
    {
        priv->txHasEnded = FALSE;
        // just by enabling interrupt. Interrupt flag is set automatically
        // when hardware sending buffer have at least one empty slot.
        //INTSetFlag(INT_SOURCE_UART_TX(pUartDev->config.portNumber - 1));
        INTEnable(INT_SOURCE_UART_TX(
                ((struct hldUartDevice *)pCharDev)->config.portNumber - 1),
                INT_ENABLED);
    }

    return SUCCESS;
}

/**
 * @brief Read single byte from device
 *
 * @param pUartDev Pointer to device which will be used
 * @return Result value
 */
static retcode
lldPic32UARTReadByte(struct hldCharDevice *pCharDev, UINT8 *pByte, UINT32 pTimeout)
{
    // Get next char from queue
    // Return error when no characters to get in time pTimeout
    if(xQueueReceive(((struct hldUartDevice *)pCharDev)->rxedCharsQueue, pByte, pTimeout))
    {
        return SUCCESS;
    }
    else
    {
        return ERR_HLD_DEVICE_UART_RX_QUEUE_EMPTY;
    }
}

/**
 * @brief UART device interrupt handler
 */
void lldPic32UARTIntHandler()
{
    // Zadeklarowane jako static aby zminimalizować użycie stosu
    static UINT8 rxtxChar, portIndex;
    static struct hldUartDevice * devNode;
    static portBASE_TYPE higherPriorityTaskWoken;
    static struct lldPic32UARTPrivateData *priv;

    higherPriorityTaskWoken = FALSE;
    devNode = (struct hldUartDevice*)hldDeviceGetFirst();

    // Loop by all devices
    while (devNode != NULL)
    {
        // But we are interested only about UART devices
        if (devNode->charHead.head.type == HLD_DEVICE_TYPE_UART)
        {
            // Check int flag by port number
            portIndex = devNode->config.portNumber - 1;
            if (INTGetFlag(INT_SOURCE_UART_RX(portIndex)))
            {
                // receive all avaliable data
                while (UARTReceivedDataIsAvailable(UART1 + portIndex) == TRUE)
                {
                    // and put it on the queue
                    rxtxChar = UARTGetDataByte(UART1 + portIndex);
                    xQueueSendFromISR( devNode->rxedCharsQueue, &rxtxChar,
                            &higherPriorityTaskWoken );

                    inputCharEventNotifyISR(EVENT_CHAR_GET, rxtxChar,
                            &higherPriorityTaskWoken);

                    if (devNode->config.enableLoopback == 1)
                    {
                        xQueueSendFromISR( devNode->charsForTxQueue, &rxtxChar,
                                &higherPriorityTaskWoken );
                    }
                }
                
                INTClearFlag(INT_SOURCE_UART_RX(portIndex));
            }

            // Check that we have some place for data to send
            // If interrupt is asserted it means that we enable it
            // so we have some data to send
            if (INTGetFlag(INT_SOURCE_UART_TX(portIndex)))
            {
                // Loop until we have some free space in hardware queue
                //if (UARTTransmissionHasCompleted(UART1 + portIndex) == TRUE)

                if (UARTTransmitterIsReady(UART1 + portIndex))
                {
                    // Get one byte from queue and send it
                    if( xQueueReceiveFromISR( devNode->charsForTxQueue,
                            &rxtxChar, &higherPriorityTaskWoken ) == TRUE )
                    {
                        // Send the next character queued for Tx.
                        UARTSendDataByte(UART1 + portIndex, rxtxChar);
                    }
                    else
                    {
                        // Let's get private device data
                        priv = (struct lldPic32UARTPrivateData *)
                                (devNode->charHead.head.priv);

                        assert(priv != NULL);

                        // Queue empty, nothing to send.
                        priv->txHasEnded = TRUE;
                        // Disable tx interrupt - it will be enabled again by
                        // sending function
                        INTEnable(INT_SOURCE_UART_TX(portIndex), INT_DISABLED);
                    }
                }

                // Clear TX interrupt flag
                INTClearFlag(INT_SOURCE_UART_TX(portIndex));
            }
        }
        devNode = (struct hldUartDevice*)devNode->charHead.head.next;
    }

    /* If sending or receiving necessitates a context switch, then switch now. */
    portEND_SWITCHING_ISR( higherPriorityTaskWoken );
}
