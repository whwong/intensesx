/**
 * @file fpgaGPU.c
 * @brief Driver for LCD fpga based graphics controller
 * Screen resolution is 800x480
 * Color format is RGB565
 * @author Daniel Szot
 */

#include "lib/common.h"
#include "task.h"
#include "hal/hld/hld.h"
#include "hal/hld/i2c.h"
#include "FpgaGpu.h"

static char lldFpgaGpuName[HLD_DEVICE_MAX_NAME_LEN] = "FPGA GPU Driver";

static struct hldI2CDevice *i2c;

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

    return ret;
}

static inline void __attribute__ ((always_inline))
lldFpgaGpuOn()
{
    // Set PSC0 to 1/152s period
    pca9532WriteReg(i2c, PSC0, 0x00);
    // Set PWM0 duty cycle to 0%
    pca9532WriteReg(i2c, PWM0, 0xff);

    // Set LED8 (backlight) at PWM0 freq
    pca9532WriteReg(i2c, LS2, 0x02);

    // Set voltages on LCD ON
    pca9532WriteReg(i2c, LS0, 0x05);

    vTaskDelay(120);
}

static inline void __attribute__ ((always_inline))
lldFpgaGpuOff()
{
    //Backlight off
    pca9532WriteReg(i2c, PWM0, 255);
    // Set voltages on LCD OFF
    pca9532WriteReg(i2c, LS0, 0x00);
    // Set LED8 to HI-Z
    pca9532WriteReg(i2c, LS2, 0x00);
    vTaskDelay(120);
}

static inline void __attribute__ ((always_inline))
lldFpgaGpuReset()
{
    lldFpgaGpuOff();
    lldFpgaGpuOn();
}

static inline void __attribute__ ((always_inline))
lldFpgaGpuSetBacklight(BOOL pState)
{
    if (pState == TRUE)
        pca9532WriteReg(i2c, PWM0, 150);
    else
        pca9532WriteReg(i2c, PWM0, 255);
}

static inline void __attribute__ ((always_inline))
lldFpgaGpuSetCommand()
{
    lldGpioWriteState(LCD_RS_GPIO, 0);
}

static inline void __attribute__ ((always_inline))
lldFpgaGpuSetData()
{
    lldGpioWriteState(LCD_RS_GPIO, 1);
}

static inline void __attribute__ ((always_inline))
lldFpgaGpuSetCS()
{
    lldGpioWriteState(LCD_CS_GPIO, 0);
}

static inline void __attribute__ ((always_inline))
lldFpgaGpuRstCS()
{
    lldGpioWriteState(LCD_CS_GPIO, 1);
}

static inline void __attribute__ ((always_inline))
lldFpgaGpuWrite(UINT16 pData)
{
    PMDIN1 = pData;
    PMPWaitBusy();
}

static inline UINT16 __attribute__ ((always_inline))
lldFpgaGpuRead()
{
    WORD value;

    lldFpgaGpuSetData();
    value = PMDIN1;
    PMPWaitBusy();
    //PMCONbits.PMPEN = 0; // disable PMP
    value = PMDIN1;
    PMPWaitBusy();
    //PMCONbits.PMPEN = 1; // enable  PMP
    return value;
}

static inline retcode __attribute__ ((always_inline))
lldFpgaGpuSetColor(struct hldLcdDevice *pLcdDev, UINT8 pA, UINT8 pR, UINT8 pG, UINT8 pB)
{
    pLcdDev->drawingColor = RGB888TORGB565(pR, pG, pB);

    lldFpgaGpuSetCS();
    lldFpgaGpuSetCommand();
    lldFpgaGpuWrite(0x7363);
    lldFpgaGpuSetData();
    lldFpgaGpuWrite(pLcdDev->drawingColor);
    lldFpgaGpuRstCS();

   // LOG("FPGA: Set color RGB888 (%d,%d,%d) RGB565 (0x%x)", pR, pG, pB, pLcdDev->drawingColor);

    return SUCCESS;
}

static inline retcode __attribute__ ((always_inline))
lldFpgaGpuSetColorRaw(struct hldLcdDevice *pLcdDev, UINT32 pColor)
{
    pLcdDev->drawingColor = pColor;

    /*lldFpgaGpuSetCS();
    lldFpgaGpuSetCommand();
    lldFpgaGpuWrite(0x7363);
    lldFpgaGpuSetData();
    lldFpgaGpuWrite(pLcdDev->drawingColor);
    lldFpgaGpuRstCS();*/

   // LOG("FPGA: Set color RGB888 (%d,%d,%d) RGB565 (0x%x)", pR, pG, pB, pLcdDev->drawingColor);

    return SUCCESS;
}

static inline UINT32 __attribute__ ((always_inline))
lldFpgaGpuGetMaxX(struct hldLcdDevice *pLcdDev)
{
    return 800;
}

static inline UINT32 __attribute__ ((always_inline))
lldFpgaGpuGetMaxY(struct hldLcdDevice *pLcdDev)
{
    return 480;
}

retcode lldFpgaGpuIoctl(struct hldLcdDevice *pLcdDev, UINT32 pCmd, UINT32 pParam)
{
    switch(pCmd)
    {
        case LC_ENABLE_DOUBLE_BUFFER:
            if (pParam == TRUE)
            {
                // Enable double buffering
                lldFpgaGpuSetCS();
                lldFpgaGpuSetCommand();
                lldFpgaGpuWrite(0x6666);
                lldFpgaGpuRstCS();
                return SUCCESS;
            }
            else
            {
                // Disable double buffering
                lldFpgaGpuSetCS();
                lldFpgaGpuSetCommand();
                lldFpgaGpuWrite(0x666F);
                lldFpgaGpuRstCS();
                return SUCCESS;
                
            }
            break;

        case LC_CLEAR_SCREEN:
            lldFpgaGpuSetCS();
            lldFpgaGpuSetCommand();
            lldFpgaGpuWrite(0x6373);
            lldFpgaGpuRstCS();
            break;

        default:
            return ERR_NOT_SUPPORTED;
    }
}

retcode lldFpgaGpuAttach()
{
    struct hldLcdDevice *dev;

    i2c = hldDeviceGetByType(NULL, HLD_DEVICE_TYPE_I2C);

    if (i2c == NULL)
    {
        ERROR("%s need at least one i2c driver inited", lldFpgaGpuName);
        return ERR_DEVICE_NOT_STARTED;
    }

    dev = hldDeviceAlloc(lldFpgaGpuName, HLD_DEVICE_TYPE_LCD,
            sizeof(struct hldLcdDevice));

    if (dev == NULL)
        return ERR_NO_MEMMORY;

    dev->attach =       lldFpgaGpuAttach;
    dev->open =         lldFpgaGpuOpen;
    dev->close =        lldFpgaGpuClose;
    dev->drawPixel =    lldFpgaGpuDrawPixel;
    dev->getPixel =     lldFpgaGpuGetPixel;
    dev->setColor =     lldFpgaGpuSetColor;
    dev->setColorRaw =  lldFpgaGpuSetColorRaw;
    dev->fill =         lldFpgaGpuFill;
    dev->getMaxX =      lldFpgaGpuGetMaxX;
    dev->getMaxY =      lldFpgaGpuGetMaxY;
    dev->enableCursor = lldFpgaGpuEnableCursor;
    dev->updateCursorPos = lldFpgaGpuUpdateCursorPos;
    dev->flush =        lldFpgaGpuFlush;
    dev->ioctl =        lldFpgaGpuIoctl;
    
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

static void lldFpgaGpuPMPOpen()
{
    // PMP setup
    PMMODE = 0;
    PMAEN = 0;
    PMCON = 0;
    PMMODEbits.MODE = 2;                // Intel 80 master interface

    PMMODEbits.WAITB = 3;
    PMMODEbits.WAITM = 10;
    PMMODEbits.WAITE = 3;

    PMMODEbits.MODE16 = 1;              // 16 bit mode

    PMCONbits.CSF    = 0;
    PMCONbits.PTRDEN = 1;               // enable RD line
    PMCONbits.PTWREN = 1;               // enable WR line
    PMCONbits.PMPEN = 1;                // enable PMP
}

static retcode lldFpgaGpuOpen(struct hldLcdDevice *pLcdDev)
{
    lldFpgaGpuOff();
    lldFpgaGpuRstCS();

    lldFpgaGpuPMPOpen();
    lldFpgaGpuOn();

    vTaskDelay(100);

    pLcdDev->drawingColor = 0x00000000;
    lldFpgaGpuFill(pLcdDev, 0, 0, pLcdDev->getMaxX(), pLcdDev->getMaxY());
    //vTaskDelay(10);


    vTaskDelay(200);
    lldFpgaGpuSetBacklight(1);
    vTaskDelay(500);

    pLcdDev->head.state |= HLD_DEVICE_STATE_RUNNING;
    return SUCCESS;
}

static retcode lldFpgaGpuFill(struct hldLcdDevice *pLcdDev,
    UINT16 pX1, UINT16 pY1, UINT16 pX2, UINT16 pY2)
{
    lldFpgaGpuSetCS();
    lldFpgaGpuSetCommand();
    lldFpgaGpuWrite(0x6672);
    lldFpgaGpuSetData();
    lldFpgaGpuWrite(pX1);
    lldFpgaGpuWrite(pY1);
    lldFpgaGpuWrite(pX2);
    lldFpgaGpuWrite(pY2);
    lldFpgaGpuRstCS();

   // LOG("FPGA: Fill Rect (x1: %d, y1: %d, x2: %d, y2: %d)", pX1, pY1, pX2, pY2);
    return SUCCESS;
}


static retcode lldFpgaGpuClose(struct hldLcdDevice *pLcdDev)
{
    pLcdDev->head.state &= ~(HLD_DEVICE_STATE_RUNNING |
            HLD_DEVICE_STATE_TURNED_ON);

    lldFpgaGpuSetBacklight(0);
    PMCONbits.PMPEN = 0;
    lldFpgaGpuOff();
    lldFpgaGpuRstCS();
    return SUCCESS;
}

static retcode lldFpgaGpuDrawPixel(struct hldLcdDevice *pLcdDev, UINT16 pX, UINT16 pY)
{
    lldFpgaGpuSetCS();
    lldFpgaGpuSetCommand();
    lldFpgaGpuWrite(0x7370);
    lldFpgaGpuSetData();
    lldFpgaGpuWrite(pX);
    lldFpgaGpuWrite(pY);
    lldFpgaGpuRstCS();

    //LOG("FPGA: Set Pixel (x: %d, y: %d)", pX, pY);
    return SUCCESS;
}

static UINT16 lldFpgaGpuGetPixel(struct hldLcdDevice *pLcdDev, UINT16 pX, UINT16 pY)
{
    return  (WORD)RGB888TORGB565(0x20,0x20,0x20);
}

static retcode lldFpgaGpuFlush(struct hldLcdDevice *pLcdDev)
{
    lldFpgaGpuSetCS();
    lldFpgaGpuSetCommand();
    lldFpgaGpuWrite(0x6666);
    lldFpgaGpuRstCS();
    return SUCCESS;
}

static BOOL cursorVisible = FALSE;
static UINT16 cursorPosX = 0;
static UINT16 cursorPosY = 0;

static void lldFpgaGpuUpdateCursor()
{
    lldFpgaGpuSetCS();
    lldFpgaGpuSetCommand();
    lldFpgaGpuWrite(0x6472);
    lldFpgaGpuSetData();
    lldFpgaGpuWrite(cursorPosX);
    lldFpgaGpuWrite(cursorPosY);
    lldFpgaGpuWrite((UINT16)cursorVisible);
    lldFpgaGpuRstCS();

    LOG("FPGA: dc: %d, %d, %x", cursorPosX, cursorPosY, cursorVisible);
}

retcode lldFpgaGpuUpdateCursorPos(struct hldLcdDevice *pLcdDev, UINT16 pX, UINT16 pY)
{
    cursorPosX = pX;
    cursorPosY = pY;
    lldFpgaGpuUpdateCursor();

    LOG("FPGA: Update Cursor Pos (x: %d, y: %d)", pX, pY);
    return SUCCESS;
}

retcode lldFpgaGpuEnableCursor(struct hldLcdDevice *pLcdDev, BOOL pEnable)
{
    cursorVisible = pEnable;
    lldFpgaGpuUpdateCursor();

    LOG("FPGA: Enable Cursor (en: %d)", pEnable);
    return SUCCESS;
}
