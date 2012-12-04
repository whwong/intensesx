/**
 * @file pic32IR.h
 * @brief IR driver for PIC32. Uses Inpput Compare peripherial and
 * timer 2 functionality
 * @author Daniel Szot
 */

#include "lib/common.h"
#include "task.h"
#include "semphr.h"
#include "hal/hld/hld.h"
#include "pic32IR.h"

#include "lib/input/inputEvent.h"

static char lldPic32IrName[HLD_DEVICE_MAX_NAME_LEN] = "PIC32 IR Driver (uses IC2)";

// Let's set function lldPic32IRIntWrapper as a handler to IC interrupt vector.
//
// The IC interrupt handler.  As this uses the FreeRTOS assembly interrupt
// entry point the IPL setting in the following prototype has no effect.  The
// interrupt priority is set in device opening function
void __attribute__( (interrupt(ipl1), vector(_INPUT_CAPTURE_2_VECTOR)))
lldPic32IRIntWrapper( void );
void __attribute__( (interrupt(ipl1), vector(_TIMER_2_VECTOR)))
lldPic32IRTimerIntWrapper( void );

static UINT32 lastPulseLen = 0;
static UINT32 lastCode = 0;
static union hldIrFlags ir_flags;

/**
 * @brief Attach Pic32 IR device
 *
 * @return Result value
 */
retcode lldPic32IRAttach(struct hldIrConfig *pCfg)
{
    struct hldIrDevice *dev;

    assert(pCfg->decode != NULL);

    if (hldDeviceGetByType(NULL, HLD_DEVICE_TYPE_IR) != NULL)
        return ERR_HLD_DEVICE_TYPE_EXISTS;

    // Only one device could be opened - this is guaranteed by unique name
    // requirement
    dev = hldDeviceAlloc(lldPic32IrName, HLD_DEVICE_TYPE_IR,
            sizeof(struct hldIrDevice));

    if (dev == NULL)
        return ERR_NO_MEMMORY;

    dev->config = *pCfg;

    dev->attach =   lldPic32IRAttach;
    dev->open =     lldPic32IROpen;
    dev->close =    lldPic32IRClose;
    dev->read =     lldPic32IRRead;

    retcode result = hldDeviceRegister(dev);
    if (result != SUCCESS)
    {
        hldDeviceFree(dev);
    }
    else
    {
        dev->head.state |= HLD_DEVICE_STATE_TURNED_ON;
    }

    return result;
}

static void lldPic32IRReset()
{
    ir_flags.edge = 0;
    ir_flags.timeout = 0;
    ir_flags.first = 1;
}

static void lldPic32IREnable()
{
    lldPic32IRReset();
    ir_flags.decoded = 0;
    mIC2IntEnable(1);
}

static void lldPic32IRDisable()
{
    mIC2IntEnable(0);
    IRTimerDisable();
}

/**
 * @brief Open pic32 input capture peripherial
 *
 * @param pIrDev Pointer to device which will be opened
 * @return Result value
 */
static retcode lldPic32IROpen(struct hldIrDevice *pIrDev)
{
    assert(pIrDev != NULL);

    // Configure all needed things but leave timer off and
    // IC interrupts also OFF

    // Configure Timer2 to use with IC2
    OpenTimer2(T2_OFF | T2_SOURCE_INT | T2_PS_1_16, 0xffff);
    // TODO: dodac priorytet do configa
    ConfigIntTimer2(T2_INT_ON | T2_INT_PRIOR_3 | T2_INT_SUB_PRIOR_0);
    WriteTimer2(T2_RELOAD);

    // Configure IC2
    OpenCapture2(IC_ON | IC_TIMER2_SRC | IC_EVERY_FALL_EDGE);
    ConfigIntCapture2(IC_INT_OFF | IC_INT_PRIOR_3 |T2_INT_SUB_PRIOR_0);

    // Init ir_flags data
    lldPic32IRReset();
    
    // Now call lldPic32IREnable to enable IC2 interrupt
    // Timer will be enabled after first edge will be detected
    lldPic32IREnable();

    pIrDev->head.state |= HLD_DEVICE_STATE_RUNNING;

    return SUCCESS;
}

/**
 * @brief Close pic32 Input Caoture device
 *
 * @param pAdcDev Pointer to device which will be closed
 * @return Result value
 */
static retcode lldPic32IRClose(struct hldIrDevice *pIrDev)
{
    assert(pIrDev != NULL);
    
    pIrDev->head.state &= ~(HLD_DEVICE_STATE_RUNNING |
            HLD_DEVICE_STATE_TURNED_ON);
    
    CloseTimer2();
    CloseCapture2();

    return SUCCESS;
}

/**
 * @brief Read single value from device
 *
 * @param pAdcDev Pointer to device which will be used
 * @return Result value
 */
static inline retcode __attribute__ ((always_inline))
lldPic32IRRead(struct hldIrDevice *pIrDev, UINT32 *pLastCode)
{
    assert(pIrDev != NULL);
    
    *pLastCode = lastCode;

    assert(pLastCode != NULL);
    
    return SUCCESS;
}

static void lldPic32IRInterrupt(struct hldIrDevice *pIrDev, UINT16 pPulseLen, 
        INT32 *pHiPriorTaskWoken)
{
    
    WriteTimer2(T2_RELOAD);

    if (ir_flags.edge)
    {
        OpenCapture2(IC_ON | IC_TIMER2_SRC | IC_EVERY_FALL_EDGE);
        ir_flags.edge = 0;
    }
    else
    {
        OpenCapture2(IC_ON | IC_TIMER2_SRC | IC_EVERY_RISE_EDGE);
        ir_flags.edge = 1;
    }

    // If this is the first edge. Start timer
    if (ir_flags.first)
        IRTimerEnable();

    lastCode = pIrDev->config.decode(pPulseLen, &ir_flags, xTaskGetTickCountFromISR());

    if (ir_flags.first)
        ir_flags.first = 0;

    if (lastCode != 0)
        inputRcuEventNotifyISR(EVENT_RCU_CODE_RECEIVED, lastCode, pHiPriorTaskWoken);
}

/**
 * @brief IR device interrupt handler
 */
void lldPic32IRIntHandler()
{
    // Zadeklarowane jako static aby zminimalizować użycie stosu
    static portBASE_TYPE higherPriorityTaskWoken;
    struct hldIrDevice *dev;
    
    higherPriorityTaskWoken = FALSE;

    static UINT32 toggleFlag = 1;
    toggleFlag = (toggleFlag == 0) ? 1 : 0;
    LED1 = toggleFlag;

    // Get pulse len from IC buffer
    lastPulseLen = mIC2ReadCapture();

    // We can just find first IR device because we assume that there
    // is only one IR device attached in system. This should be checked by
    // attach function
    dev = (struct hldIrDevice*)hldDeviceGetFirst();
    while(dev != NULL)
    {
        if (dev->head.type == HLD_DEVICE_TYPE_IR)
        {
            lldPic32IRInterrupt(dev, lastPulseLen, &higherPriorityTaskWoken);
            break;
        }
        dev = (struct hldIrDevice*)(dev->head.next);
    }
    
    // Clear interrupt flag
    IFS0bits.IC2IF = 0;

    /* If sending or receiving necessitates a context switch, then switch now. */
    portEND_SWITCHING_ISR( higherPriorityTaskWoken );
}

static void lldPic32IRTimeout()
{
    ir_flags.timeout = 1;

    IRTimerDisable();

    // reset capture edge
    OpenCapture2(IC_ON | IC_TIMER2_SRC | IC_EVERY_FALL_EDGE);

    lldPic32IRReset();
}

/**
 * @brief IR Timer interrupt handler
 */
void lldPic32IRTimerIntHandler()
{
    // Zadeklarowane jako static aby zminimalizować użycie stosu
    static portBASE_TYPE higherPriorityTaskWoken;
    higherPriorityTaskWoken = FALSE;

    lldPic32IRTimeout();
    INTClearFlag(INT_T2);

    /* If sending or receiving necessitates a context switch, then switch now. */
    portEND_SWITCHING_ISR( higherPriorityTaskWoken );
}
