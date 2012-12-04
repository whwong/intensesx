/**
 * @file pic32ADC.h
 * @brief Analog digital converter Driver for PIC32 ADC module
 * Driver based on RetroBSD ADC driver for PIC32. Thanks Serge!
 * @author Daniel Szot
 */

#ifndef _PIC32ADC_H
#define _PIC32ADC_H

#include <p32xxxx.h>
#include <stdlib.h>
#include <plib.h>
#include "lib/common.h"
#include "board.h"
#include "hal/hld/adc.h"

#define LLD_PIC32_ADC_MAX 15

struct lldPic32ADCChanInfo
{
    UINT32 lastReading;
    UINT8 active;
};

retcode lldPic32ADCAttach(struct hldAdcConfig *pCfg);
static retcode lldPic32ADCOpen(struct hldAdcDevice *pAdcDev, UINT8 pChannel);
static retcode lldPic32ADCClose(struct hldAdcDevice *pAdcDev, UINT8 pChannel);

static inline retcode __attribute__ ((always_inline))
lldPic32ADCRead(struct hldAdcDevice *pAdcDev, UINT8 pChannel, UINT32 *pValue);

static inline BOOL __attribute__ ((always_inline))
lldPic32ADCIsChannelOpened(struct hldAdcDevice *pAdcDev, UINT8 pChannel);

#endif
