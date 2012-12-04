/**
 * @file adc.h
 * @brief ADC High Level Driver - ADC HLD structure contains mutex
 * which can be used while implementing new driver to mutual
 * exclusive access right to device
 * @author Daniel Szot
 */

#ifndef _HLD_ADC_H_
#define _HLD_ADC_H_

#include "hld.h"
#include "lib/common.h"
#include "os/semphr.h"

struct hldAdcConfig
{
    UINT32 aquisitionTime; // in ns, != 0 && > adcClockPeriod
    UINT32 adcClockPeriod; // in ns, != 0

    // Priority of ADC interrupt
    UINT8 intPriority;
};

struct hldAdcDevice
{
    struct hldDevice head;
    struct hldAdcConfig config;

    retcode (*attach)(struct hldAdcConfig *pCfg);
    retcode (*open)(struct hldAdcDevice *pAdcDev, UINT8 channel);
    retcode (*close)(struct hldAdcDevice *pAdcDev, UINT8 channel);

    retcode (*read)(struct hldAdcDevice *pAdcDev, UINT8 pChannel, UINT32 *pValue);

    BOOL (*isChannelOpened)(struct hldAdcDevice *pAdcDev, UINT8 channel);
};

#endif
