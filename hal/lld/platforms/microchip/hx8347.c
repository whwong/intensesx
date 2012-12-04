/**
 * @file hx8347.c
 * @brief hx8347 LCD controller driver for Microchip PIC32 processors family
 * Screen resolution is 320x240
 * Color format is RGB565
 * Driver based on Microchip Module for Microchip Graphics Library
 * @author Daniel Szot
 */

#include "lib/common.h"
#include "task.h"
#include "hal/hld/hld.h"
#include "hx8347.h"

static char lldHx8347Name[HLD_DEVICE_MAX_NAME_LEN] = "HX8347 320x240 LCD Driver";

// Clipping region control
static BOOL _clipRgn;

// Clipping region borders
static INT16 _clipLeft;
static INT16 _clipTop;
static INT16 _clipRight;
static INT16 _clipBottom;

static inline void __attribute__ ((always_inline))
lldHx8347On()
{
    lldGpioWriteState(LCD_RESET_GPIO, 1);
    vTaskDelay(120);
}

static inline void __attribute__ ((always_inline))
lldHx8347Off()
{
    lldGpioWriteState(LCD_RESET_GPIO, 0);
    vTaskDelay(120);
}

static inline void __attribute__ ((always_inline))
lldHx8347Reset()
{
    lldHx8347Off();
    lldHx8347On();
}

static inline void __attribute__ ((always_inline))
lldHx8347SetBacklight(BOOL pState)
{
    lldGpioWriteState(LCD_BACKLIGHT_GPIO, pState);
}

static inline void __attribute__ ((always_inline))
lldHx8347SetCommand()
{
    lldGpioWriteState(LCD_RS_GPIO, 0);
}

static inline void __attribute__ ((always_inline))
lldHx8347SetData()
{
    lldGpioWriteState(LCD_RS_GPIO, 1);
}

static inline void __attribute__ ((always_inline))
lldHx8347SetCS()
{
    lldGpioWriteState(LCD_CS_GPIO, 0);
}

static inline void __attribute__ ((always_inline))
lldHx8347RstCS()
{
    lldGpioWriteState(LCD_CS_GPIO, 1);
}

static inline void __attribute__ ((always_inline))
lldHx8347Write(UINT16 pData)
{
    PMDIN1 = pData;
    PMPWaitBusy();
}

static inline void __attribute__ ((always_inline))
lldHx8347WritePixel(UINT16 pData)
{
    lldHx8347SetData();
    lldHx8347Write(pData);
}

static inline UINT16 __attribute__ ((always_inline))
lldHx8347Read()
{
    WORD value;

    lldHx8347SetData();
    value = PMDIN1;
    PMPWaitBusy();
    //PMCONbits.PMPEN = 0; // disable PMP
    value = PMDIN1;
    PMPWaitBusy();
    //PMCONbits.PMPEN = 1; // enable  PMP
    return value;
}

static inline retcode __attribute__ ((always_inline))
lldHx8347SetColor(struct hldLcdDevice *pLcdDev, UINT8 pA, UINT8 pR, UINT8 pG, UINT8 pB)
{
    pLcdDev->drawingColor = RGB888TORGB565(pR, pG, pB);
    return SUCCESS;
}


static inline UINT32 __attribute__ ((always_inline))
lldHx8347GetMaxX(struct hldLcdDevice *pLcdDev)
{
    #if (DISP_ORIENTATION == 90) || (DISP_ORIENTATION == 270)
        return (320 - 1);
    #elif (DISP_ORIENTATION == 0) || (DISP_ORIENTATION == 180)
        return (240 - 1);
    #endif
}

static inline UINT32 __attribute__ ((always_inline))
lldHx8347GetMaxY(struct hldLcdDevice *pLcdDev)
{
    #if (DISP_ORIENTATION == 90) || (DISP_ORIENTATION == 270)
        return (240 - 1);
    #elif (DISP_ORIENTATION == 0) || (DISP_ORIENTATION == 180)
        return (320 - 1);
    #endif
}


static void lldHx8347SetAddress(INT16 x, INT16 y)
{
    lldHx8347SetCommand();
    lldHx8347Write(0x02);
    lldHx8347SetData();
    lldHx8347Write(((WORD_VAL) (UINT16) x).v[1]);
    lldHx8347SetCommand();
    lldHx8347Write(0x03);
    lldHx8347SetData();
    lldHx8347Write(((WORD_VAL) (UINT16) x).v[0]);
    lldHx8347SetCommand();
    lldHx8347Write(0x06);
    lldHx8347SetData();
    lldHx8347Write(((WORD_VAL) (UINT16) y).v[1]);
    lldHx8347SetCommand();
    lldHx8347Write(0x07);
    lldHx8347SetData();
    lldHx8347Write(((WORD_VAL) (UINT16) y).v[0]);
    lldHx8347SetCommand();
    lldHx8347Write(0x22);
}

retcode lldHx8347Attach()
{
    struct hldLcdDevice *dev;
    dev = hldDeviceAlloc(lldHx8347Name, HLD_DEVICE_TYPE_LCD,
            sizeof(struct hldLcdDevice));

    if (dev == NULL)
        return ERR_NO_MEMMORY;

    _clipRgn = 0;

    dev->attach =       lldHx8347Attach;
    dev->open =         lldHx8347Open;
    dev->close =        lldHx8347Close;
    dev->drawPixel =    lldHx8347DrawPixel;
    dev->getPixel =     lldHx8347GetPixel;
    dev->setColor =     lldHx8347SetColor;
    dev->fill =         lldHx8347Fill;
    dev->getMaxX =      lldHx8347GetMaxX;
    dev->getMaxY =      lldHx8347GetMaxY;

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

static void lldHx8347SetReg(BYTE index, BYTE value)
{
    lldHx8347SetCS();
    lldHx8347SetCommand();
    lldHx8347Write(index);
    lldHx8347SetData();
    lldHx8347Write(value);
    lldHx8347RstCS();
}

static void lldHx8347PMPOpen()
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

static retcode lldHx8347Open(struct hldLcdDevice *pLcdDev)
{
    lldHx8347Reset();

    lldHx8347Off();
    lldHx8347RstCS();
    lldHx8347SetBacklight(0);

    lldHx8347PMPOpen();
    lldHx8347On();

    vTaskDelay(1);

    // Driving ability setting
    lldHx8347SetReg(0xEA,0x00);   // PTBA[15:8]
    lldHx8347SetReg(0xEB,0x20);   // PTBA[7:0]
    lldHx8347SetReg(0xEC,0x0C);   // STBA[15:8]
    lldHx8347SetReg(0xED,0xC4);   // STBA[7:0]
    lldHx8347SetReg(0xE8,0x40);   // OPON[7:0]
    lldHx8347SetReg(0xE9,0x38);   // OPON1[7:0]
    lldHx8347SetReg(0xF1,0x01);   // OTPS1B
    lldHx8347SetReg(0xF2,0x10);   // GEN
    lldHx8347SetReg(0x27,0xA3);   //

    // Gamma 2.8 setting
    lldHx8347SetReg(0x40,0x00);   //
    lldHx8347SetReg(0x41,0x00);   //
    lldHx8347SetReg(0x42,0x01);   //
    lldHx8347SetReg(0x43,0x13);   //
    lldHx8347SetReg(0x44,0x10);   //
    lldHx8347SetReg(0x45,0x26);   //
    lldHx8347SetReg(0x46,0x08);   //
    lldHx8347SetReg(0x47,0x51);   //
    lldHx8347SetReg(0x48,0x02);   //
    lldHx8347SetReg(0x49,0x12);   //
    lldHx8347SetReg(0x4A,0x18);   //
    lldHx8347SetReg(0x4B,0x19);   //
    lldHx8347SetReg(0x4C,0x14);   //

    lldHx8347SetReg(0x50,0x19);   //
    lldHx8347SetReg(0x51,0x2F);   //
    lldHx8347SetReg(0x52,0x2C);   //
    lldHx8347SetReg(0x53,0x3E);   //
    lldHx8347SetReg(0x54,0x3F);   //
    lldHx8347SetReg(0x55,0x3F);   //
    lldHx8347SetReg(0x56,0x2E);   //
    lldHx8347SetReg(0x57,0x77);   //
    lldHx8347SetReg(0x58,0x0B);   //
    lldHx8347SetReg(0x59,0x06);   //
    lldHx8347SetReg(0x5A,0x07);   //
    lldHx8347SetReg(0x5B,0x0D);   //
    lldHx8347SetReg(0x5C,0x1D);   //
    lldHx8347SetReg(0x5D,0xCC);   //

    // Window setting
    #if (DISP_ORIENTATION == 0)
    lldHx8347SetReg(0x04,0x00);
    lldHx8347SetReg(0x05,0xEF);
    lldHx8347SetReg(0x08,0x01);
    lldHx8347SetReg(0x09,0x3F);
    #else
    lldHx8347SetReg(0x04,0x01);
    lldHx8347SetReg(0x05,0x3F);
    lldHx8347SetReg(0x08,0x00);
    lldHx8347SetReg(0x09,0xEF);
    #endif

    // Display Setting
    //lldHx8347SetReg(0x01,0x06);   // IDMON=0, INVON=1, NORON=1, PTLON=0

    #if (DISP_ORIENTATION == 0)
    lldHx8347SetReg(0x16,0x08);   // MY=0, MX=0, MV=0, BGR=1
    #else
    lldHx8347SetReg(0x16,0x68);   // MY=0, MX=1, MV=1, BGR=1
    #endif


    // Power Voltage Setting
    lldHx8347SetReg(0x1B,0x1B);   // VRH = 4.65
    lldHx8347SetReg(0x1A,0x01);   // BT
    lldHx8347SetReg(0x24,0x2F);   // VMH
    lldHx8347SetReg(0x25,0x57);   // VML

    // Vcom offset
    lldHx8347SetReg(0x23,0x8D);   // FLICKER ADJUST

    // Power ON Setting
    lldHx8347SetReg(0x18,0x36);   //
    lldHx8347SetReg(0x19,0x01);   //
    lldHx8347SetReg(0x01,0x00);   //
    lldHx8347SetReg(0x1F,0x88);   //
    vTaskDelay(5);
    lldHx8347SetReg(0x1F,0x80);   //
    vTaskDelay(5);
    lldHx8347SetReg(0x1F,0x90);   //
    vTaskDelay(5);
    lldHx8347SetReg(0x1F,0xD0);   //
    vTaskDelay(5);

    // 65K Color Selection
    lldHx8347SetReg(0x17,0x05);   //

    // Set Panel
    lldHx8347SetReg(0x36,0x00);   //

    // Display ON Setting
    lldHx8347SetReg(0x28,0x38);   //
    vTaskDelay(40);
    lldHx8347SetReg(0x28,0x3C);   //
    vTaskDelay(1);

    pLcdDev->drawingColor = 0x00000000;
    lldHx8347Fill(pLcdDev, 0, 0, pLcdDev->getMaxX(), pLcdDev->getMaxY());
    vTaskDelay(10);
    lldHx8347SetBacklight(1);

    pLcdDev->head.state |= HLD_DEVICE_STATE_RUNNING;
    return SUCCESS;
}

static retcode lldHx8347Fill(struct hldLcdDevice *pLcdDev,
    UINT16 pX1, UINT16 pY1, UINT16 pX2, UINT16 pY2)
{
    register SHORT  x, y;

    if(_clipRgn)
    {
        if(pX1 < _clipLeft)
            pX1 = _clipLeft;
        if(pX2 > _clipRight)
            pX2 = _clipRight;
        if(pY1 < _clipTop)
            pY1 = _clipTop;
        if(pY2 > _clipBottom)
            pY2 = _clipBottom;
    }

    lldHx8347SetCS();
    for(y = pY1; y < pY2 + 1; y++)
    {
        lldHx8347SetAddress(pX1, y);
        for(x = pX1; x < pX2 + 1; x++)
        {
            lldHx8347WritePixel(pLcdDev->drawingColor);
        }
    }

    lldHx8347RstCS();
    return SUCCESS;
}


static retcode lldHx8347Close(struct hldLcdDevice *pLcdDev)
{
    pLcdDev->head.state &= ~(HLD_DEVICE_STATE_RUNNING |
            HLD_DEVICE_STATE_TURNED_ON);

    lldHx8347SetBacklight(0);
    PMCONbits.PMPEN = 0;
    lldHx8347Off();
    lldHx8347RstCS();
    return SUCCESS;
}

static retcode lldHx8347DrawPixel(struct hldLcdDevice *pLcdDev, UINT16 pX, UINT16 pY)
{
    if(_clipRgn)
    {
        if(pX < _clipLeft)
            return;
        if(pX > _clipRight)
            return;
        if(pY < _clipTop)
            return;
        if(pY > _clipBottom)
            return;
    }

    lldHx8347SetCS();
    lldHx8347SetAddress(pX, pY);
    lldHx8347WritePixel(pLcdDev->drawingColor);
    lldHx8347RstCS();
}

static UINT16 lldHx8347GetPixel(struct hldLcdDevice *pLcdDev, UINT16 pX, UINT16 pY)
{
    if(_clipRgn)
    {
        if(pX < _clipLeft)
            return 0;
        if(pX > _clipRight)
            return 0;
        if(pY < _clipTop)
            return 0;
        if(pY > _clipBottom)
            return 0;
    }

    WORD val;
    BYTE r,g,b;

    lldHx8347SetCS();
    lldHx8347SetAddress(pX,pY);

    //lldHx8347SetCommand();
   // lldHx8347Write(0x22);

  //  val = lldHx8347Read();  // Dummy Read for PMP read acces
    lldHx8347Read();  // Dummy Read for TFT GRAM read acces
    val = lldHx8347Read();
    b = (BYTE)((val >> 8) & 0xFF);
    //val = lldHx8347Read();
    r = (BYTE)((val >> 0 ) & 0xFF);
    //b = (BYTE)((val << 4) & 0x30);
    //lldHx8347Read();  // Dummy Read for TFT GRAM read acces
    val = lldHx8347Read();
    g = (BYTE)((val >> 0) & 0xFF);
    lldHx8347RstCS();
    return  (WORD)RGB888TORGB565(r,g,b);
}

static retcode lldHx8347Flush(struct hldLcdDevice *pLcdDev)
{
    return SUCCESS;
}
