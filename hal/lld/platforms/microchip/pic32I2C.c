/**
 * @file pic32I2C.c
 * @brief pic32I2C Driver for PIC32
 * @author Daniel Szot
 */

#include "lib/common.h"
#include "task.h"
#include "queue.h"
#include "hal/hld/hld.h"
#include "pic32I2C.h"
#include "hal/hld/char.h"
#include "lib/input/inputEvent.h"

static char lldPic32I2CName[HLD_DEVICE_MAX_NAME_LEN] = "PIC32 I2C Driver";

/**
 * @brief Attach Pic32 I2C device
 *
 * @return Result value
 */
retcode lldPic32I2CAttach()
{
    struct hldI2CDevice *dev;

    dev = hldDeviceAlloc(lldPic32I2CName, HLD_DEVICE_TYPE_I2C,
            sizeof(struct hldI2CDevice));

    if (dev == NULL)
        return ERR_NO_MEMMORY;

    dev->charHead.head.priv = NULL;
    dev->attach =   lldPic32I2CAttach;
    dev->open =     lldPic32I2COpen;
    dev->close =    lldPic32I2CClose;
    dev->ioctl =    lldPic32I2CIoctl;
    dev->charHead.write =  lldPic32I2CWriteByte;
    dev->charHead.read =   lldPic32I2CReadByte;
    
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
 * @brief Open device
 *
 * @param pI2CDev Pointer to device which will be opened
 * @return Result value
 */
static retcode
lldPic32I2COpen(struct hldI2CDevice *pI2CDev)
{
    CloseI2C2();
    OpenI2C2(I2C_EN | I2C_IDLE_CON | I2C_7BIT_ADD | I2C_STR_EN,
            (configPERIPHERAL_CLOCK_HZ / (2 * HLD_I2C_CLK_FREQ) - 2));
    // Wait to complete
    IdleI2C2();
    vTaskDelay(1);

    pI2CDev->charHead.head.state |= HLD_DEVICE_STATE_RUNNING;
    
    DONE("LLD PIC 32 I2C (#2) device opened:");
    LOG("    CLK Freq: %d", HLD_I2C_CLK_FREQ);
    
    return SUCCESS;
}

/**
 * @brief Close device, disable and delete everything :)
 *
 * @param pI2CDev Pointer to device which will be closed
 * @return Result value
 */
static retcode lldPic32I2CClose(struct hldI2CDevice *pI2CDev)
{
    pI2CDev->charHead.head.state &= ~(HLD_DEVICE_STATE_RUNNING |
            HLD_DEVICE_STATE_TURNED_ON);

    CloseI2C2();

    LOG("LLD PIC 32 I2C (#2) device closed");
    return SUCCESS;
}

/**
 * @brief Send single byte to device
 *
 * @param pI2CDev Pointer to device which will be used
 * @return Result value
 */
static retcode 
lldPic32I2CWriteByte(struct hldCharDevice *pCharDev, UINT8 pByte, UINT32 pTimeout)
{
    if (MasterWriteI2C2(pByte) != 0)
    {
        return ERR_BUS; // failed
    }
    IdleI2C2(); // Wait to complete

    if (I2C2STATbits.BCL)
        I2C2STATbits.BCL = 0;
    
    return SUCCESS;
}

/**
 * @brief Read single byte from device
 *
 * @param I2C Pointer to device which will be used
 * @return Result value
 */
static retcode
lldPic32I2CReadByte(struct hldCharDevice *pCharDev, UINT8 *pByte, UINT32 pTimeout)
{
    (*pByte) = MasterReadI2C2();

    return SUCCESS;
}

static retcode
lldPic32I2CIoctl(struct hldI2CDevice *pI2CDev, UINT32 pCmd, UINT32 pParam)
{
    switch (pCmd)
    {
        case I2C_START:
            StartI2C2(); // Send the Start Bit
            IdleI2C2(); // Wait to complete
            break;

        case I2C_STOP:
            StopI2C2(); // Send the Start Bit
            IdleI2C2(); // Wait to complete
            break;
    }
}
