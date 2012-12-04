/**
 * @file pic32ADC.h
 * @brief Analog digital converter Driver for PIC32 ADC module
 * Driver based on RetroBSD ADC driver for PIC32. Thanks Serge!
 * @author Daniel Szot
 */

#include "lib/common.h"
#include "task.h"
#include "semphr.h"
#include "hal/hld/hld.h"
#include "pic32ADC.h"

static char lldPic32ADCName[HLD_DEVICE_MAX_NAME_LEN] = "PIC32 ADC Driver";

// In this table we will hold all readed values from all adc channels.
// Table size is fixed from performance reasons. It is better just to
// have space reserved for all channels than do it dynamically. Even when we
// are using only a few channels.
static struct lldPic32ADCChanInfo adcInfo[LLD_PIC32_ADC_MAX+1];
static UINT8 openedChannels;

// Let's set function lldPic32ADCntWrapper as a handler to adc interrupt vector.
//
// The ADC interrupt handler.  As this uses the FreeRTOS assembly interrupt
// entry point the IPL setting in the following prototype has no effect.  The
// interrupt priority is set by the call to  ConfigIntUART2() in
// xSerialPortInitMinimal().
void __attribute__( (interrupt(ipl1), vector(_ADC_VECTOR))) lldPic32ADCIntWrapper( void );

/**
 * @brief Attach Pic32 ADC device
 *
 * @return Result value
 */
retcode lldPic32ADCAttach(struct hldAdcConfig *pCfg)
{
    struct hldAdcDevice *dev;

    assert(pCfg->adcClockPeriod != 0);
    assert(pCfg->aquisitionTime != 0);
    assert(pCfg->adcClockPeriod < pCfg->aquisitionTime);

    // Only one device could be opened - this is guaranteed by unique name
    // requirement
    dev = hldDeviceAlloc(lldPic32ADCName, HLD_DEVICE_TYPE_ADC,
            sizeof(struct hldAdcDevice));

    if (dev == NULL)
        return ERR_NO_MEMMORY;

    dev->config = *pCfg;

    openedChannels = 0;
    dev->attach =   lldPic32ADCAttach;
    dev->open =     lldPic32ADCOpen;
    dev->close =    lldPic32ADCClose;
    dev->read =     lldPic32ADCRead;
    dev->isChannelOpened = lldPic32ADCIsChannelOpened;

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

/**
 * @brief If opened channels is equal to 0 let's open device, enable interrupts,
 * create mutex etc. If opened channels is greater than 0 just open another
 * channel.
 *
 * @param pUartDev Pointer to device which will be opened
 * @param pChannel Channel to open
 * @return Result value
 */
static retcode lldPic32ADCOpen(struct hldAdcDevice *pAdcDev, UINT8 pChannel)
{
    UINT32 convReg, aqReg, scanFreq;
    assert(pAdcDev != NULL);
    
    if (pChannel > LLD_PIC32_ADC_MAX)
        return ERR_HLD_DEVICE_ADC_CHANNEL_NOT_EXISTS;

    AD1PCFG &= ~(1 << pChannel);
    adcInfo[pChannel].active = 1;
    if(openedChannels == 0)
    {
        // Enable and configure the ADC here
        AD1CSSL = 0xFFFF;
        AD1CON2 = 0b0000010000111100;
        
        convReg = (pAdcDev->config.adcClockPeriod * 
                (configPERIPHERAL_CLOCK_HZ/1000000UL));
        
        convReg /= (2UL*1000UL);
        convReg--;
        aqReg = pAdcDev->config.aquisitionTime / pAdcDev->config.adcClockPeriod;
        AD1CON3 = (convReg & 0xff) | ((aqReg & 0xff) << 8);

        AD1CON1 = 0b1000000011100110;

        DONE("LLD PIC 32 ADC device opened");
        scanFreq = 1000000000UL/(pAdcDev->config.adcClockPeriod *
                (12+aqReg) * (LLD_PIC32_ADC_MAX+1));

        LOG("LLD PIC 32 ADC Scanning frequency is %d Hz", scanFreq);
        
        // TODO: Add priority setting
        INTSetVectorPriority(INT_ADC_VECTOR, pAdcDev->config.intPriority);
        INTSetVectorSubPriority(INT_ADC_VECTOR, INT_SUB_PRIORITY_LEVEL_0);
        IEC1SET = 0x0002;
    }
    openedChannels++;

    pAdcDev->head.state |= HLD_DEVICE_STATE_RUNNING;

    return SUCCESS;
}

/**
 * @brief Close channel, if opened channels reach 0, close whole device.
 *
 * @param pAdcDev Pointer to device which channel will be closed
 * @param pChannel Channel to close
 * @return Result value
 */
static retcode lldPic32ADCClose(struct hldAdcDevice *pAdcDev, UINT8 pChannel)
{
    assert(pAdcDev != NULL);
    
    pAdcDev->head.state &= ~(HLD_DEVICE_STATE_RUNNING |
            HLD_DEVICE_STATE_TURNED_ON);
    
    if (pChannel > LLD_PIC32_ADC_MAX)
        return ERR_HLD_DEVICE_ADC_CHANNEL_NOT_EXISTS;

    AD1PCFG |= (1 << pChannel);
    adcInfo[pChannel].active = 0;
    openedChannels--;

    LOG("LLD PIC 32 ADC channel AD%d closed", pChannel);

    if(openedChannels == 0)
    {
        // Switch off the ADC here.
        AD1CSSL = 0x0000;
        AD1CON1 = 0x0000;
        asm volatile("NOP");
        IEC1CLR = 0x0002;
        LOG("LLD PIC 32 ADC device closed");
    }

    return SUCCESS;
}

/**
 * @brief Read single value from device
 *
 * @param pAdcDev Pointer to device which will be used
 * @return Result value
 */
static inline retcode __attribute__ ((always_inline))
lldPic32ADCRead(struct hldAdcDevice *pAdcDev, UINT8 pChannel, UINT32 *pValue)
{
    assert(pAdcDev != NULL);
    assert(pChannel <= LLD_PIC32_ADC_MAX);
    
    *pValue = adcInfo[pChannel].lastReading;

    assert(pValue != NULL);
    
    return SUCCESS;
}

static inline BOOL __attribute__ ((always_inline))
lldPic32ADCIsChannelOpened(struct hldAdcDevice *pAdcDev, UINT8 pChannel)
{
    assert(pChannel <= LLD_PIC32_ADC_MAX);
    return adcInfo[pChannel].active;
}

/**
 * @brief ADC device interrupt handler
 * @note interrupt is asserted once per 16 conversions
 */
void lldPic32ADCIntHandler()
{
    // Zadeklarowane jako static aby zminimalizować użycie stosu
    static portBASE_TYPE higherPriorityTaskWoken;
    higherPriorityTaskWoken = FALSE;

    adcInfo[0].lastReading = ADC1BUF0;
    adcInfo[1].lastReading = ADC1BUF1;
    adcInfo[2].lastReading = ADC1BUF2;
    adcInfo[3].lastReading = ADC1BUF3;
    adcInfo[4].lastReading = ADC1BUF4;
    adcInfo[5].lastReading = ADC1BUF5;
    adcInfo[6].lastReading = ADC1BUF6;
    adcInfo[7].lastReading = ADC1BUF7;
    adcInfo[8].lastReading = ADC1BUF8;
    adcInfo[9].lastReading = ADC1BUF9;
    adcInfo[10].lastReading = ADC1BUFA;
    adcInfo[11].lastReading = ADC1BUFB;
    adcInfo[12].lastReading = ADC1BUFC;
    adcInfo[13].lastReading = ADC1BUFD;
    adcInfo[14].lastReading = ADC1BUFE;
    adcInfo[15].lastReading = ADC1BUFF;

    IFS1CLR = 0x0002;

    /* If sending or receiving necessitates a context switch, then switch now. */
    portEND_SWITCHING_ISR( higherPriorityTaskWoken );
}
