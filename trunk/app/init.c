/**
 * @file init.c
 * @brief Hw and Sw init procedures
 * @author Daniel Szot
 */

#include "init.h"

#include "keymap.h"
#include "rsc/winstyle/winstyle.h"

static FATFS fat;

static void drawInitScreen()
{
    struct hldLcdDevice *lcd = hldDeviceGetByType(NULL, HLD_DEVICE_TYPE_LCD);
    UINT32 x1, x2, y1, y2;

    x1 = 0;
    y1 = 0;

    x2 = lcd->getMaxX();
    y2 = lcd->getMaxY()-22;

    graphSetDrawingColor(0xff, 0xff, 0xff, 0xff);

    //graphDrawRect(x1, y1, x2, y2);
    graphDrawText(x1, y1, x2, y2, "IntenseOS", &g_FontHaveltica26AA,
            FS_VALIGN_CENTER | FS_ALIGN_CENTER);


    graphSetDrawingColor(0xff, 0xaa, 0xaa, 0xaa);
    graphDrawRect(0, y2, lcd->getMaxX(), lcd->getMaxY());
    graphSetDrawingColor(0xff, 0x55, 0x55, 0x55);
    graphDrawText(0, y2, lcd->getMaxX(), 22,
            "ZUT - Engineer's Thesis (Szot, Zamolski)", &g_DroidSans15,
            FS_VALIGN_CENTER | FS_ALIGN_RIGHT);
}

void appInitScreenWait()
{
    UINT32 x, y, c = 0;
    struct hldTouchDevice *touch = hldDeviceGetById(HLD_DEVICE_TYPE_TOUCH, 0);
    struct hldLcdDevice *lcd = hldDeviceGetById(HLD_DEVICE_TYPE_LCD, 0);

    graphSetDrawingColor(0xff, 0xff, 0xff, 0xff);
    graphDrawText(0, lcd->getMaxY()/2, lcd->getMaxX(), 30,
            "(Tap the screen to continue)", &g_DroidSans15,
            FS_VALIGN_CENTER | FS_ALIGN_CENTER);

    do
    {
        c++;
        touch->read(touch, &x, &y);
        vTaskDelay(5);

        if (c == 600)
            break;
    } while (x == -1 && y == -1);
}

static retcode appHardwareInit()
{
    retcode ret = SUCCESS;
    struct hldLcdDevice *lcd;
    struct hldUartConfig uartConfig;
    struct hldUartDevice *uart;
    struct hldI2CDevice *i2c;
    struct hldAdcDevice *adc;
    struct hldAdcConfig adcConfig;
    struct hldTouchDevice *touch;
    struct hldTouchConfig touchConfig;
    struct hldIrDevice *ir;
    struct hldIrConfig irConfig;
    struct hldAudioDevice *audio;
    struct hldAudioConfig audioConfig;
    struct hldDiskDevice *disk;

    ret = inputEventInit(INPUT_QUEUE_SIZE);
    if (ret != SUCCESS)
    {
        ERROR("Can not create input layer (#%d)", ret);
        return ret;
    }
    else
    {
        DONE("Input layer created");
    }

    uartConfig.baudRate     = 115200;
    uartConfig.dataSize     = UDS_8_BITS;
    uartConfig.parity       = UP_EVEN;
    uartConfig.stopBits     = USB_1;
    uartConfig.portNumber   = 1;
    uartConfig.txQueueSize  = 10;
    uartConfig.rxQueueSize  = 10;
    uartConfig.intPriority  = (configKERNEL_INTERRUPT_PRIORITY + 1);
    uartConfig.enableLoopback = 0;

    ret = lldPic32UARTAttach();
    if (ret == SUCCESS)
    {
        uart = hldDeviceGetById(HLD_DEVICE_TYPE_UART, 0);
        if (uart != NULL)
        {
            logSetUartDevice(uart);
            uart->open(uart, &uartConfig);
        }
    }
    else
    {
        ERROR("Can not attach UART device (#%d)", ret);
        return ret;
    }

    ret = lldPic32I2CAttach();
    if (ret == SUCCESS)
    {
        i2c = hldDeviceGetById(HLD_DEVICE_TYPE_I2C, 0);
        if (i2c != NULL)
        {
            i2c->open(i2c);
        }
    }
    else
    {
        ERROR("Can not attach I2C device (#%d)", ret);
        return ret;
    }

    // 1kHz sample clock
    adcConfig.adcClockPeriod    = (3905);
    adcConfig.aquisitionTime    = (3905)*4;
    adcConfig.intPriority       = configKERNEL_INTERRUPT_PRIORITY + 2;

    ret = lldPic32ADCAttach(&adcConfig);
    if (ret == SUCCESS)
    {
        adc = hldDeviceGetById(HLD_DEVICE_TYPE_ADC, 0);
        if (adc != NULL)
        {
            adc->open(adc, 8);
            adc->open(adc, 12);
        }
    }
    else
    {
        ERROR("Can not attach ADC device (#%d)", ret);
        return ret;
    }

#ifdef LCD_FPGA
    ret = lldFpgaGpuAttach();
#else
    ret = lldHx8347Attach();
#endif
    if (ret == SUCCESS)
    {
        lcd = hldDeviceGetById(HLD_DEVICE_TYPE_LCD, 0);
        if (lcd != NULL)
        {
            lcd->open(lcd);
            graphSetLcdDevice(lcd);
#ifndef LCD_FPGA_TEST
            drawInitScreen();
#endif
        }
    }
    else
    {
        ERROR("Can not attach Hx8347 lcd driver device (#%d)", ret);
        return ret;
    }

    touchConfig.sampleTime      = 18;
    touchConfig.taskPriority    = tskIDLE_PRIORITY + 1;
    touchConfig.yMinCal = 67;
    touchConfig.yMaxCal = 920;
    touchConfig.xMinCal = 73;
    touchConfig.xMaxCal = 927;
    touchConfig.screenResolutionX = lcd->getMaxX();
    touchConfig.screenResolutionY = lcd->getMaxY();

    ret = lldResTouchAttach(&touchConfig);
    if (ret == SUCCESS)
    {
        touch = hldDeviceGetById(HLD_DEVICE_TYPE_TOUCH, 0);
        if (touch != NULL)
        {
            touch->open(touch);
        }
    }
    else
    {
        ERROR("Can not attach TOUCH input device (#%d)", ret);
        return ret;
    }

    irConfig.decode = &irNecDecode;

    ret = lldPic32IRAttach(&irConfig);
    if (ret == SUCCESS)
    {
        ir = hldDeviceGetById(HLD_DEVICE_TYPE_IR, 0);
        if (ir != NULL)
        {
            ir->open(ir);
        }
    }
    else
    {
        ERROR("Can not attach IR device (#%d)", ret);
        return ret;
    }

    audioConfig.bits = 16;
    audioConfig.channels = 2;
    audioConfig.enable = AUDIO_LINEOUT;
    audioConfig.mode = AM_ONECHANNEL;
    audioConfig.sampleRate = 48000;
    audioConfig.intPriority = configKERNEL_INTERRUPT_PRIORITY + 2;

    ret = lldWm8731Attach(&audioConfig);
    if (ret == SUCCESS)
    {
        audio = hldDeviceGetById(HLD_DEVICE_TYPE_AUDIO, 0);
        if (audio != NULL)
        {
            // In production software need to try a few times and check that
            // device started properly
            audio->open(audio);
            audio->ioctl(audio, AC_ENABLE, AUDIO_OUTAMP);
            audio->ioctl(audio, AC_SET_VOLUME, 50);
        }
    }
    else
    {
        ERROR("Can not attach Wm8731 device (#%d)", ret);
        return ret;
    }

    ret = lldPic32SDMMCAttach();
    if (ret == SUCCESS)
    {
        disk = hldDeviceGetById(HLD_DEVICE_TYPE_DISK, 0);
        if (disk != NULL)
        {
            diskstatus ds = disk->open(disk);
            LOG("Disk status: %x", ds);
        }
    }
    else
    {
        ERROR("Can not attach SDMMC device (#%d)", ret);
        return ret;
    }

    return SUCCESS;
}

retcode appSoftwareInit()
{
    INT32 fr;
    retcode ret;
    
    fr = f_mount(0, &fat);
    if (fr != FR_OK)
    {
        ERROR("Can not init file system (#%d)", fr);
        return ERR_FILE_SYSTEM;
    }

    ret = msgDispatcherInit();
    if (ret == SUCCESS)
    {
        DONE("Message dispatcher inited");
    }
    else
    {
        ERROR("Can not init message dispatcher (#%d)", ret);
        return ret;
    }

    ret = inputTaskInit(&intenseKeymap[0], INTENSE_KEYMAP_SIZE);
    if (ret == SUCCESS)
    {
        DONE("Input task created");
    }
    else
    {
        ERROR("Input task creation error (#%d)", ret);
        return ret;
    }

    ret = consoleStart();
    if (ret == SUCCESS)
    {
        DONE("Console started");
    }
    else
    {
        ERROR("Console creation error (#%d)", ret);
        return ret;
    }
    
    audioConsoleRegisterCmds();
    fatfsConsoleRegisterCmds();

    ret = guiInit();
    if (ret == SUCCESS)
    {
        DONE("GUI library inited");
    }
    else
    {
        ERROR("GUI init error (#%d)", ret);
        return ret;
    }
    
    guiSetDefaultFont(&g_DroidSans22);
    guiSetWinstyle(intenseWinstyle);

    return SUCCESS;
}

retcode appInit()
{
    retcode ret;

    ret = appHardwareInit();
    if (ret != SUCCESS)
        return ret;

    ret = appSoftwareInit();
    if (ret != SUCCESS)
        return ret;
}
