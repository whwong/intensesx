/**
 * @file pic32IR.h
 * @brief IR driver for PIC32. Uses Inpput Compare peripherial and
 * timer 2 functionality
 * @author Daniel Szot
 */

#ifndef _PIC32IR_H
#define _PIC32IR_H

#include <p32xxxx.h>
#include <stdlib.h>
#include <plib.h>
#include "lib/common.h"
#include "board.h"
#include "hal/hld/ir.h"

#define T2_RELOAD 0

#define IRTimerEnable() T2CONbits.TON = 1;
#define IRTimerDisable() T2CONbits.TON = 0;

retcode lldPic32IRAttach(struct hldIrConfig *pCfg);
static retcode lldPic32IROpen(struct hldIrDevice *pIrDev);
static retcode lldPic32IRClose(struct hldIrDevice *pIrDev);

static inline retcode __attribute__ ((always_inline))
lldPic32IRRead(struct hldIrDevice *pIrDev, UINT32 *pLastCode);

#endif
