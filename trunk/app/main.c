/**
 * @file main.c
 * @brief Intense S application main file
 * @author Daniel Szot
 */
#include "lib/graphics/graphics.h"
#include "lib/input/inputTask.h"


#include "lib/common.h"
#include "hal/hld/hld.h"
#include "hal/hld/lcd.h"
#include "lib/user/msgListener.h"
#include "hal/hld/uart.h"
#include "hal/hld/adc.h"
#include "hal/hld/touch.h"
#include "hal/hld/audio.h"

#include "lib/log.h"

#include "hal/lld/platforms/microchip/pic32UART.h"
#include "hal/lld/platforms/microchip/pic32ADC.h"
#include "hal/lld/platforms/microchip/pic32IR.h"
#include "hal/lld/resistiveTouch.h"
#include "hal/lld/platforms/microchip/wm8731.h"
#include "hal/lld/platforms/microchip/pic32SDMMC.h"
#include "lib/ir/nec.h"

#include "lib/input/inputEvent.h"
#include "keymap.h"

#include "lib/user/msgDispatcher.h"
#include "lib/audio/audioSineOutput.h"

#include "lib/audio/audioConsole.h"
#include "lib/fatfs/fatfsConsole.h"
#include "lib/audio/audio1ch.h"

#include "rsc/fonts/fonts.h"
#include "rsc/winstyle/winstyle.h"

int main(void);

struct hldUartDevice *gUartDevice;
struct hldLcdDevice *gLcdDevice;
FATFS fat;

static UINT32 dec(UINT32 d, union hldIrFlags *pFlags)
{
    return 0;
}

static char fatBenchmarkBuf[512] = "Tutaj zapisujemy dokladnie 64 znaki. Testujemy testujemy a co!\n\rTutaj zapisujemy dokladnie 64 znaki. Testujemy testujemy a co!\n\rTutaj zapisujemy dokladnie 64 znaki. Testujemy testujemy a co!\n\rTutaj zapisujemy dokladnie 64 znaki. Testujemy testujemy a co!\n\r";

static void prvTestTask1( void *pvParameters )
{
    UINT32 c=0, t;
    UINT16 cw = 0;
    struct hldLcdDevice *lcd;
    struct hldUartConfig uartConfig;
    struct hldUartDevice *uart;
    struct hldAdcDevice *adc;
    struct hldAdcConfig adcConfig;
    struct hldTouchDevice *touch;
    struct hldTouchConfig touchConfig;
    struct hldIrDevice *ir;
    struct hldIrConfig irConfig;
    struct hldAudioDevice *audio;
    struct hldAudioConfig audioConfig;
    struct hldDiskDevice *disk;
    UINT32 volume = 50;

    if (inputEventInit(200) != SUCCESS)
    {
        ERROR("Can't create input layer");
        while(1);
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

    if (lldPic32UARTAttach() == SUCCESS)
    {
        uart = hldDeviceGetById(HLD_DEVICE_TYPE_UART, 0);
        if (uart != NULL)
        {
            logSetUartDevice(uart);
            uart->open(uart, &uartConfig);
            gUartDevice = uart;
        }
    }

    // 1kHz sample clock
    adcConfig.adcClockPeriod    = (3905);
    adcConfig.aquisitionTime    = (3905)*4;
    adcConfig.intPriority       = configKERNEL_INTERRUPT_PRIORITY + 2;

    if (lldPic32ADCAttach(&adcConfig) == SUCCESS)
    {
        adc = hldDeviceGetById(HLD_DEVICE_TYPE_ADC, 0);
        if (adc != NULL)
        {
            adc->open(adc, 8);
            adc->open(adc, 12);
        }
    }

    if (lldHx8347Attach() == SUCCESS)
    {
        lcd = hldDeviceGetById(HLD_DEVICE_TYPE_LCD, 0);
        if (lcd != NULL)
        {
            lcd->open(lcd);
            graphSetLcdDevice(lcd);
        }
    }

    touchConfig.sampleTime      = 18;
    touchConfig.taskPriority    = tskIDLE_PRIORITY + 1;
    touchConfig.yMinCal = 67;
    touchConfig.yMaxCal = 920;
    touchConfig.xMinCal = 73;
    touchConfig.xMaxCal = 927;
    touchConfig.screenResolutionX = 240;
    touchConfig.screenResolutionY = 320;

    if (lldResTouchAttach(&touchConfig) == SUCCESS)
    {
        touch = hldDeviceGetById(HLD_DEVICE_TYPE_TOUCH, 0);
        if (touch != NULL)
        {
            touch->open(touch);
        }
    }

    irConfig.decode = &irNecDecode;

    if (lldPic32IRAttach(&irConfig) == SUCCESS)
    {
        ir = hldDeviceGetById(HLD_DEVICE_TYPE_IR, 0);
        if (ir != NULL)
        {
            ir->open(ir);
        }
    }

    audioConfig.bits = 16;
    audioConfig.channels = 2;
    audioConfig.enable = AUDIO_LINEOUT;
    audioConfig.mode = AM_ONECHANNEL;
    audioConfig.sampleRate = 48000;
    audioConfig.intPriority = configKERNEL_INTERRUPT_PRIORITY + 2;
    
    
    if (lldWm8731Attach(&audioConfig) == SUCCESS)
    {
        audio = hldDeviceGetById(HLD_DEVICE_TYPE_AUDIO, 0);
        if (audio != NULL)
        {
            // In production software need to try a few times and check that
            // device started properly
            audio->open(audio);
            audio->ioctl(audio, AC_ENABLE, AUDIO_OUTAMP);
            audio->ioctl(audio, AC_SET_VOLUME, volume);
        }
    }

    if (lldPic32SDMMCAttach() == SUCCESS)
    {
        disk = hldDeviceGetById(HLD_DEVICE_TYPE_DISK, 0);
        if (disk != NULL)
        {
            diskstatus ds = disk->open(disk);
            LOG("Disk status: %x", ds);
        }
    }
    INT32 fr = f_mount(0, &fat);
/* Benchmark
    FIL f;
    UINT numread;
    LOG("f_mount: %d", fr);

    // Write benchmark
    fr = f_open(&f, "dupa.txt", FA_CREATE_ALWAYS | FA_WRITE);
    LOG("f_open: %x", fr);


    UINT32 i;
    UINT32 lastTick = time();
    for (i = 0; i < 4096; i++)
    {
        fr = f_write(&f, fatBenchmarkBuf, 512, &numread);
        //if (numread!=64) ERROR("f_write: %x, writed: %d", fr, numread);
    }
    UINT32 delta = time()-lastTick;
    LOG("Writing %d bytes takes: %d ms it gives speed %dkB/s", 512*(i+1), delta, 512*(i+1)/delta);

    fr = f_close(&f);
    LOG("f_close: %x", fr);

    //Read benchmark
    fr = f_open(&f, "dupa.txt", FA_OPEN_EXISTING | FA_READ);
    LOG("f_open: %x", fr);

    lastTick = time();
    for (i = 0; i < 4096; i++)
    {
        fr = f_read(&f, fatBenchmarkBuf, 512, &numread);
        //if (numread!=64) ERROR("f_write: %x, writed: %d", fr, numread);
    }
    delta = time()-lastTick;
    LOG("Reading %d bytes takes: %d ms it gives speed %dkB/s", 512*(i+1), delta, 512*(i+1)/delta);

    fr = f_close(&f);
    LOG("f_close: %x", fr);
*/
    msgDispatcherInit();
    if (inputTaskInit(&intenseKeymap[0], INTENSE_KEYMAP_SIZE) == SUCCESS)
    {
        DONE("Input task created");
    }
    else
    {
        ERROR("Input task creation error");
        while(1);
    }

    consoleStart();
    audioConsoleRegisterCmds();
    fatfsConsoleRegisterCmds();
    audioSineConfig(&audioConfig);
    audioSineSetFreq(500);

    audio1chPlaySound("asaf2.wav", 0);
    
    struct msgListener *list;
    struct msg m;
    INT32 x = 100,y = 100, ox=100, oy=100;
    INT32 charx = 10;
    INT32 charxh = 10;
    INT32 charxha = 10;

    // GUI Init
    guiSetDefaultFont(&g_DroidSans15);
    
    list = msgListenerCreate(200);
    while(msgListenerGet(list, &m, NULL, 0, 0))
    {
        diskstatus ds;
        switch(m.message)
        {
            case MSG_DISKREMOVED:
                ds = disk->close(disk);
                LOG("Disk status: %x", ds);
                break;

            case MSG_DISKINSERTED:
                ds = disk->open(disk);
                LOG("Disk status: %x", ds);
                break;

            case MSG_KEYDOWN:
                if (m.param1 == V_KEY_LEFT)
                    x += 5;
                else if (m.param1 == V_KEY_RIGHT)
                    x -= 5;
                else if (m.param1 == V_KEY_UP)
                    y += 5;
                else if (m.param1 == V_KEY_DOWN)
                    y -= 5;

                if (m.param1 == V_KEY_1)
                    audio1chPlaySound("asaf2.wav", SND_ASYNC);
                else if (m.param1 == V_KEY_2)
                    audio1chPlaySound("satellit.wav", SND_ASYNC);
                else if (m.param1 == V_KEY_3)
                    audio1chPlaySound("k.wav", SND_SYNC);
                else if (m.param1 == V_KEY_STOP)
                    audio1chStopSound();
                else if (m.param1 == V_KEY_PLAY)
                    audio1chPause(0);
                else if (m.param1 == V_KEY_PAUSE)
                    audio1chPause(1);
                else if (m.param1 == V_KEY_V_UP)
                {
                    if (volume < 100)
                    {
                        volume++;
                    }
                    
                    audio->ioctl(audio, AC_SET_VOLUME_SOFT, volume);
                }
                else if (m.param1 == V_KEY_V_DOWN)
                {
                    if (volume > 0)
                    {
                        volume--;
                    }
                    
                    audio->ioctl(audio, AC_SET_VOLUME_SOFT, volume);
                }

                lcd->setColor(lcd, 0, 255, 255, 255);
                graphDrawLine(x,y,ox,oy);

                lcd->setColor(lcd, 0, 0, 255, 0);
                lcd->drawPixel(lcd, x,y);
                graphDrawCircle(x, y, 2);

                ox = x;
                oy = y;
                break;

            case MSG_KEYUP:
                lcd->setColor(lcd, 0, 255, 0, 0);
                graphDrawCircle(x, y, 2);
                break;

            case MSG_POINTERMOVE:
                x = GET_X_PARAM2(m.param2);
                y = GET_Y_PARAM2(m.param2);

                lcd->setColor(lcd, 0, 255, 255, 255);
                graphDrawLine(x,y,ox,oy);

                lcd->setColor(lcd, 0, 30, 30, 255);
                lcd->drawPixel(lcd, x,y);
                lcd->drawPixel(lcd, ox,oy);

                ox = x;
                oy = y;
                break;

            case MSG_POINTERDOWN:
                x = GET_X_PARAM2(m.param2);
                y = GET_Y_PARAM2(m.param2);

                lcd->setColor(lcd, 0, 100, 100, 100);
                lcd->drawPixel(lcd, x,y);
                graphDrawCircle(x, y, 3);

                ox = x;
                oy = y;
                break;

            case MSG_POINTERUP:
                x = GET_X_PARAM2(m.param2);
                y = GET_Y_PARAM2(m.param2);

                lcd->setColor(lcd, 0, 100, 100, 100);
                lcd->drawPixel(lcd, x,y);
                graphDrawCircle(x, y, 3);

                //spped
                lcd->setColor(lcd, 0, 128, 0, 0);
                graphDrawLine(x,y,x+GET_DX_PARAM1(m.param1),y+GET_DY_PARAM1(m.param1));

                ox = x;
                oy = y;
                break;

            case MSG_CHAR:
                lcd->setColor(lcd, 0, 255, 255, 255);
                graphDrawChar(charx, 10, &cw, m.param1, &g_DroidSans15);
                charx += cw;
                graphDrawChar(charxh, 50, &cw, m.param1, &g_DroidSans22);
                charxh += cw;
                graphDrawChar(charxha, 100, &cw, m.param1, &g_DroidSans29);
                charxha += cw;
                break;
        }
    }
    msgListenerDelete(list);

    struct inputEvent evn;
    while(0)
    {
        struct inputTouchEvent *tevn = (struct inputTouchEvent*)&evn;
        struct inputKeyEvent *kevn = (struct inputKeyEvent*)&evn;
        struct inputCharEvent *cevn = (struct inputCharEvent*)&evn;
        struct inputRcuEvent *revn = (struct inputRcuEvent*)&evn;

        inputEventGet(&evn);

        switch(evn.type)
        {
            case EVENT_KEY:
                LOG("Key event:\n\taction: %d\n\tcode: 0x%08x\n\ttime: %d",
                        kevn->action, kevn->code, kevn->timestamp);
                break;
            
            case EVENT_RCU:
                LOG("RCU event:\n\taction: %d\n\tcode: 0x%08x\n\ttime: %d",
                        revn->action, revn->code, revn->timestamp);

                if (revn->code == 0x20dfe01f)
                {
                    x += 10;
                }
                else if (revn->code == 0x20df609f)
                {
                    x -= 10;
                }
                else if (revn->code == 0x20df02fd)
                {
                    y += 10;
                }
                else if (revn->code == 0x20df827d)
                {
                    y -= 10;
                }

                lcd->setColor(lcd, 0, 255, 255, 255);
                graphDrawLine(x,y,ox,oy);

                lcd->setColor(lcd, 0, 0, 255, 0);
                lcd->drawPixel(lcd, x,y);
                graphDrawCircle(x, y, 3);

                ox = x;
                oy = y;
                break;

            case EVENT_TOUCH:
                LOG("Touch event:\n\taction: %d\n\tpos: %d,%d\n\tspd: %d,%d\n\ttime: %d",
                        tevn->action, tevn->positionX, tevn->positionY, tevn->speedX,
                        tevn->speedY, tevn->timestamp);

                x = tevn->positionX;
                y = tevn->positionY;

                if ((tevn->action == EVENT_TOUCH_DOWN) || (tevn->action == EVENT_TOUCH_UP))
                {
                    lcd->setColor(lcd, 0, 100, 100, 100);
                    lcd->drawPixel(lcd, x,y);
                    graphDrawCircle(x, y, 3);
                    if (tevn->action == EVENT_TOUCH_UP)
                    {
                        lcd->setColor(lcd, 0, 128, 0, 0);
                        graphDrawLine(x,y,x+tevn->speedX,y+tevn->speedY);
                    }
                }
                else
                {
                    lcd->setColor(lcd, 0, 255, 255, 255);
                    graphDrawLine(x,y,ox,oy);

                    lcd->setColor(lcd, 0, 30, 30, 255);
                    lcd->drawPixel(lcd, x,y);
                    lcd->drawPixel(lcd, ox,oy);
                }

                ox = x;
                oy = y;

                break;

            case EVENT_CHAR:
                LOG("Char event:\n\taction: %d\n\tchar: 0x%02x (%d '%c')\n\ttime: %d",
                        cevn->action, cevn->character, cevn->character,
                        cevn->character, kevn->timestamp);
                break;
        }
    }

    for( ;; )
    {
        //UINT32 ticks = xTaskGetTickCount();

        //graphTestDrawRand30Circles();
        //lldIli9163Flush(lcd);
        //graphTestDrawEdgeMesh();
        //lldIli9163Flush(lcd);
        lcd->drawingColor = 0xffff;
        lcd->drawPixel(lcd, 0,0);
        //LOG("Clear");
        //lcd->drawPixel(lcd, 0,lcd->getMaxY());
        //lcd->drawPixel(lcd, lcd->getMaxX(),0);
        //lcd->drawPixel(lcd, lcd->getMaxX(),lcd->getMaxY());
        //UINT16 c1 = lcd->getPixel(lcd, 0,2);
        UINT16 c1 = lcd->getPixel(lcd, 0,0);
        c1 = lcd->getPixel(lcd, 1,1);
       // c1 = lcd->getPixel(lcd, 2,1);
        //c1 = lcd->getPixel(lcd, 2,0);
        //vTaskDelay(30000);

        INT32 lastIr = 0, irv = 0;

        while(1)
        {
            touch->read(touch, &x, &y);
            //lcd->drawPixel(lcd, x,y);
            if (x >= 0 && y >= 0)
            {
                if (ox != -1 && oy != -1)
                {
                    graphDrawLine(x,y,ox,oy);

                    lcd->setColor(lcd, 0, 255, 0, 0);
                    lcd->drawPixel(lcd, x,y);
                    lcd->drawPixel(lcd, ox,oy);
                    lcd->setColor(lcd, 0, 255, 255, 255);
                }
                else
                    lcd->drawPixel(lcd, x,y);
                
               // LOG("%d,%d", (INT32)x,(INT32)y);
            }
            ox = x;
            oy = y;

        }

        graphTestDrawRand1kLines();
        int i,j,k;
            //printf("dupa blada");
        for (k = 0; k < 10; k++)
        {
        //LOG("Kernel first log k = %d", k);
        //ERROR("No i sie posypalo");
        //adc->read(adc, 8, &t);
        //DEBUG("Ambient temp: %d",  t);
            for ( i = 0; i < lcd->getMaxX(); i++)
            {
                for ( j = 0; j <= lcd->getMaxY(); j++)
                {
                    UINT16 color = lcd->getPixel(lcd, i,j);
                    lcd->drawingColor = color;
                    //if (color != 0x0000)
                      lcd->drawPixel(lcd, i,j);
                }
            }
        }

        //ticks = xTaskGetTickCount() - ticks;

        graphSetDrawingColor(0,0,0,0);
        graphDrawRect(0,0,240,320);
    }
}
/*-----------------------------------------------------------*/
#include "math.h"
static void prvTestTask2( void *pvParameters )
{
	for( ;; )
	{
            vTaskDelay(500);
	}
}

int main(void) {
    boardInit();

    xTaskCreate( prvTestTask1, ( const signed char * const ) "Ts1",
            1024, NULL, tskIDLE_PRIORITY, NULL );
    xTaskCreate( prvTestTask2, ( const signed char * const ) "Ts2",
            configMINIMAL_STACK_SIZE+50, NULL, tskIDLE_PRIORITY, NULL );
    /* Finally start the scheduler. */
    vTaskStartScheduler();

    while(1);
}

void vApplicationStackOverflowHook( void )
{
	/* Look at pxCurrentTCB to see which task overflowed its stack. */
        graphSetDrawingColor(0,255,0,0);
        graphDrawRect(0,0,240,320);
	for( ;; );
}
/*-----------------------------------------------------------*/

void _general_exception_handler( unsigned long ulCause, unsigned long ulStatus )
{
	/* This overrides the definition provided by the kernel.  Other exceptions
	should be handled here. */
        graphSetDrawingColor(0,0,0,255);
        graphDrawRect(0,0,240,320);
	for( ;; );
}



