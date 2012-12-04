/**
 * @file lcd.h
 * @brief ILI9163 LCD controller driver for Microchip PIC32 processors family
 * Screen resolution is 128x160
 * Color format is RGB565
 * @author Daniel Szot
 */

#include "lib/common.h"
#include "task.h"
#include "hal/hld/hld.h"
#include "ili9163.h"

static char ili9163Name[HLD_DEVICE_MAX_NAME_LEN] = "ILI9163 128x160 LCD Driver";

#ifdef ILI9163_DOUBLE_BUFFERED_DRIVER
    UINT16 ili9163FrameBuffer[160*132];
#endif

static void lldIli9163On()
{
    lldGpioWriteState(LCD_RESET_GPIO, 1);
    vTaskDelay(120);
}

static void lldIli9163Off()
{
    lldGpioWriteState(LCD_RESET_GPIO, 0);
    vTaskDelay(120);
}

static void lldIli9163Reset()
{
    lldIli9163Off();
    lldIli9163On();
}

static void lldIli9163SetBacklight(BOOL pState)
{
    lldGpioWriteState(LCD_BACKLIGHT_GPIO, pState);
}

retcode lldIli9163Attach()
{
    struct hldLcdDevice *dev;
    dev = hldDeviceAlloc(ili9163Name, HLD_DEVICE_TYPE_LCD,
            sizeof(struct hldLcdDevice));
    
    if (dev == NULL)
        return ERR_NO_MEMMORY;

    dev->attach =       lldIli9163Attach;
    dev->open =         lldIli9163Open;
    dev->close =        lldIli9163Close;
    dev->drawPixel =    lldIli9163DrawPixel;
    dev->setColor =     lldIli9163SetColor;
    dev->fill =         lldIli9163Fill;
    dev->getMaxX =      lldIli9163GetMaxX;
    dev->getMaxY =      lldIli9163GetMaxY;

    retcode result = hldDeviceRegister(dev);
    if (result != SUCCESS)
    {
        hldDeviceFree(dev);
    }

    return result;
}

static void lldIli9163Window(UINT16 pX1, UINT16 pY1, UINT16 pX2, UINT16 pY2)
{
    lcdCommand(0x2A);
    lcdData8(0);
    lcdData8(pX1);
    lcdData8(0x00);
    lcdData8(pX2);

    lcdCommand(0x2B);
    lcdData8(0);
    lcdData8(131-pY2);
    lcdData8(0x00);
    lcdData8(131-pY1);
//    lcdCommand(0x2B);
//    lcdData8(0);
//    lcdData8(pY1);
//    lcdData8(0x00);
//    lcdData8(pY2);

    lcdCommand(0x2C);
}

retcode lldIli9163Fill(struct hldLcdDevice *pLcdDev,
    UINT16 pX1, UINT16 pY1, UINT16 pX2, UINT16 pY2)
{
#ifndef ILI9163_DOUBLE_BUFFERED_DRIVER
    UINT16 i,k;
    lldIli9163Window(pX1, pY1, pX2, pY2);

    pmpAddress(DATA_ADDRESS);

    for(i = pX1; i <= pX2; i++)
    {
        for (k = pY1; k <= pY2; k++)
        {
            pmpWrite((UINT16)pLcdDev->drawingColor);
        }
    }
#else
    UINT32 i;
    for(i = 0; i <= 159*131; i++)
    {
        ili9163FrameBuffer[i] = pLcdDev->drawingColor;
    }
#endif

    return SUCCESS;
}

retcode lldIli9163Open(struct hldLcdDevice *pLcdDev)
{
    lldIli9163Reset();
    pLcdDev->head.state |= HLD_DEVICE_STATE_TURNED_ON;

    mPMPOpen(CONTROL, MODE, PORT, INTERRUPT);
    vTaskDelay(3);

    lcdCommand(0x11); 					//exit sleep mode
    vTaskDelay(100);
    lcdCommand(0x26); lcdData8(0x04);                   //set gamma curve 3
    lcdCommand(0xF2); lcdData8(0x00);                   //gamma adjustment enabled
    lcdCommand(0xB1); lcdData8(0x0A);	lcdData8(0x14);	//frame rate
    lcdCommand(0xC0); lcdData8(0x0A);	lcdData8(0x00);	//power control 1
    lcdCommand(0xC1); lcdData8(0x02);                   //power control 2
    lcdCommand(0xC5); lcdData8(0x2F);	lcdData8(0x3E);	//VCOM control 1
    lcdCommand(0xC7); lcdData8(0x40);			//VCOM offset control
    lcdCommand(0x2A);			//column address set
    lcdData8(0x00);
    lcdData8(0x00);			//start column 0x0000
    lcdData8(0x00);
    lcdData8(0x7F);			//end column 0x007F = 127
    lcdCommand(0x2B);			//page address set
    lcdData8(0x00);
    lcdData8(0x00);			//start line 0x0000
    lcdData8(0x00);
    lcdData8(0x9F); 			//end line 0x009F = 159
//#ifdef ILI9163_DOUBLE_BUFFERED_DRIVER
///    lcdCommand(0x36); lcdData8(0x00);	//Memory access control
//#else
    lcdCommand(0x36); lcdData8(0x20);	//Memory access control
//#endif
    lcdCommand(0x3A); lcdData8(0xC5);	//Pixel format = 16bit/pixel
    lcdCommand(0x29);	//display on
    lcdCommand(0x20);	//color inversion on - for compatybility to old lcd screen...
    lcdCommand(0x2C);	//write to frame memory(begin writing data)

    pLcdDev->drawingColor = 0x00000000;
    lldIli9163Fill(pLcdDev, 0, 0, 160, 132);
    vTaskDelay(50);
    lldIli9163SetBacklight(1);

    pLcdDev->head.state |= HLD_DEVICE_STATE_RUNNING;
    return SUCCESS;
}

retcode lldIli9163Close(struct hldLcdDevice *pLcdDev)
{
    pLcdDev->head.state &= ~(HLD_DEVICE_STATE_RUNNING |
            HLD_DEVICE_STATE_TURNED_ON);
    
    lldIli9163SetBacklight(0);
    mPMPClose();
    lldIli9163Off();
    return SUCCESS;
}

retcode lldIli9163SetColor(struct hldLcdDevice *pLcdDev, UINT8 pA, UINT8 pR,
        UINT8 pG, UINT8 pB)
{
    pLcdDev->drawingColor = RGB888TORGB565(pR, pG, pB);
    return SUCCESS;
}

static void lldIli9163GoToXY(UINT16 pX, UINT16 pY)
{
    lcdCommand(0x2A);
    lcdData8(0); 
    lcdData8(pX);
    lcdData8(0x00);
    lcdData8(pX+1);

    lcdCommand(0x2B);
    lcdData8(0);
    lcdData8(128-pY);
    lcdData8(0x00);
    lcdData8(128-pY+1);

    lcdCommand(0x2C);
}

retcode lldIli9163DrawPixel(struct hldLcdDevice *pLcdDev, UINT16 pX, UINT16 pY)
{
#ifndef ILI9163_DOUBLE_BUFFERED_DRIVER
    lldIli9163GoToXY(pX, pY);
    lcdData((UINT16)pLcdDev->drawingColor);
#else
    ili9163FrameBuffer[pX+((131-pY)*160)] = (UINT16)pLcdDev->drawingColor;
#endif
    return SUCCESS;
}

retcode lldIli9163GetPixel(struct hldLcdDevice *pLcdDev, UINT16 pX, UINT16 pY,
    UINT8 *r, UINT8 *g, UINT8 *b)
{
#ifndef ILI9163_DOUBLE_BUFFERED_DRIVER
    *r = (pLcdDev->drawingColor & 0b1111100000000000) >> 11;
    *g = (pLcdDev->drawingColor & 0b0000011111100000) >> 5;
    *b = (pLcdDev->drawingColor & 0b0000000000011111);
#else
    UINT32 xy = pX+((131-pY)*160);
    *r = (UINT8)((ili9163FrameBuffer[xy] & 0b1111100000000000) >> 11);
    *g = (UINT8)((ili9163FrameBuffer[xy] & 0b0000011111100000) >> 5);
    *b = (UINT8)(ili9163FrameBuffer[xy] & 0b0000000000011111);
#endif

    return SUCCESS;
}

retcode lldIli9163Flush(struct hldLcdDevice *pLcdDev)
{
    lldIli9163GoToXY(0, 0);
#ifdef ILI9163_DOUBLE_BUFFERED_DRIVER
    /*DmaChnSetTxfer(ILI9163_DMA_CHANNEL, (void*)&ili9163FrameBuffer,
                    (void*)&PMDIN, 160*132*2, 1, 1);
    //DmaChnSetEvEnableFlags(DMA_SPI, DMA_EV_BLOCK_DONE);
    //DmaChnIntEnable(DMA_SPI);
    DmaChnEnable(ILI9163_DMA_CHANNEL);*/
    UINT32 i;
    UINT8 *data = (UINT8*)ili9163FrameBuffer;
    //lldIli9163Window(0, 0, 160, 132);

    pmpAddress(DATA_ADDRESS);

    for(i = 0; i < 160*132*2; i++)
    {
        pmpWrite(*data);
        data++;
    }
#endif

    return SUCCESS;
}

inline UINT32 lldIli9163GetMaxX(struct hldLcdDevice *pLcdDev)
{
    return 160;
}

inline UINT32 lldIli9163GetMaxY(struct hldLcdDevice *pLcdDev)
{
    return 128;
}
