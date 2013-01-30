/**
 * @file lcdInit.c
 * @brief Temporary module to test functionality of big 800x480 lcd screen
 * @author Daniel Szot
 */

#include "lcdInit.h"

static retcode pca9532WriteReg(struct hldI2CDevice *i2c, UINT8 pReg, UINT8 pVal)
{
    retcode ret = SUCCESS;

    i2c->ioctl(i2c, I2C_START, 0);
    
    ret |= i2c->charHead.write(i2c, DEV_ADDR, 0);
    ret |= i2c->charHead.write(i2c, pReg, 0);
    ret |= i2c->charHead.write(i2c, pVal, 0);

    i2c->ioctl(i2c, I2C_STOP, 0);

    if (ret != SUCCESS)
        ERROR("PCA9532 write(0x%x, 0x%x)", pReg, pVal);
    else
        DONE("PCA9532 write(0x%x, 0x%x)", pReg, pVal);
}

void bigScreenInit()
{
    struct hldI2CDevice *i2c = hldDeviceGetByType(NULL, HLD_DEVICE_TYPE_I2C);

    if (i2c != NULL)
        LOG("Ok mamy device");

    // Set 0 on LED0 and LED1, leave LED2 and LED3 at hi-impedance
    pca9532WriteReg(i2c, LS0, 0x05);
    // Set LED8 at PWM0 freq
    pca9532WriteReg(i2c, LS2, 0x02);
    // Set PSC0 to 1/152s period
    pca9532WriteReg(i2c, PSC0, 0x00);
    // Set PWM0 duty cycle to 50%
    pca9532WriteReg(i2c, PWM0, 50);

    i2c->ioctl(i2c, I2C_STOP, 0);
}
