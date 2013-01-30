/**
 * @file i2c.h
 * @brief I2C High Level Driver
 * @author Daniel Szot
 */

#ifndef _HLD_I2C_H_
#define _HLD_I2C_H_

#include "hld.h"
#include "char.h"
#include "lib/common.h"

#define HLD_I2C_CLK_FREQ 100000

// Ioctl Commands
#define I2C_START   0x00
#define I2C_STOP    0x01

struct hldI2CDevice
{
    struct hldCharDevice charHead;

    retcode (*attach)();
    retcode (*open)(struct hldI2CDevice *pI2CDev);
    retcode (*close)(struct hldI2CDevice *pI2CDev);
    
    retcode (*ioctl)(struct hldI2CDevice *pI2CDev, UINT32 pCmd, UINT32 pParam);
};

#endif
