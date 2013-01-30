/**
 * @file pic32I2C.h
 * @brief I2C Driver for PIC32
 * @author Daniel Szot
 */

#ifndef _PIC32I2C_H
#define _PIC32I2C_H

#include <p32xxxx.h>
#include <stdlib.h>
#include <plib.h>
#include "lib/common.h"
#include "board.h"
#include "hal/hld/i2c.h"

retcode lldPic32I2CAttach();

static retcode lldPic32I2COpen(struct hldI2CDevice *pI2CDev);
static retcode lldPic32I2CClose(struct hldI2CDevice *pI2CDev);

static retcode 
lldPic32I2CWriteByte(struct hldCharDevice *pCharDev, UINT8 pByte, UINT32 pTimeout);

static retcode
lldPic32I2CReadByte(struct hldCharDevice *pCharDev, UINT8 *pByte, UINT32 pTimeout);

static retcode
lldPic32I2CIoctl(struct hldI2CDevice *pI2CDev, UINT32 pCmd, UINT32 pParam);

#endif
