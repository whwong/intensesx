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
#include "lib/gui/progressbar.h"
#include "hal/lld/platforms/microchip/pic32ADC.h"
#include "hal/lld/platforms/microchip/pic32IR.h"
#include "hal/lld/resistiveTouch.h"
#include "hal/lld/platforms/microchip/wm8731.h"
#include "hal/lld/platforms/microchip/pic32SDMMC.h"
#include "lib/ir/nec.h"

#include "lib/input/inputEvent.h"

#include "lib/user/msgDispatcher.h"
#include "lib/audio/audioSineOutput.h"

#include "lib/audio/audioConsole.h"
#include "lib/fatfs/fatfsConsole.h"
#include "lib/audio/audio1ch.h"

#include "rsc/rsc.h"
#include "lib/gui/statictext.h"


#include "init.h"
#include "windows/player.h"
#include "lib/input/keyCodes.h"

static FATFS fat;
static BOOL showCursor = FALSE;
int main(void);

void overflowme()
{
    volatile UINT32 t[100];
    volatile UINT32 tab[1024];

    taskYIELD();

    for (t[0] = 1; t[0] < 100; t[0]++)
    {
        tab[t[0]] = t[0];
    }

    tab[10] = 0;
    return;
}

static void appInitErrHandler()
{
    while(1)
    {
        LED0 = 0;
        vTaskDelay(500);
        LED0 = 1;
        vTaskDelay(500);
    }
}

void showCursorProc(struct msg *m)
{
struct hldLcdDevice *lcd;
    static INT32 x = 100,y = 100, ox=100, oy=100;

    lcd = hldDeviceGetById(HLD_DEVICE_TYPE_LCD, 0);

    switch(m->message)
    {
        case MSG_POINTERMOVE:
            x = GET_X_PARAM2(m->param2);
            y = GET_Y_PARAM2(m->param2);

            lcd->setColor(lcd, 0, 255, 255, 255);
            graphDrawLine(x,y,ox,oy);

            lcd->setColor(lcd, 0, 30, 30, 255);
            lcd->drawPixel(lcd, x,y);
            lcd->drawPixel(lcd, ox,oy);

            ox = x;
            oy = y;
            break;

        case MSG_POINTERDOWN:
            x = GET_X_PARAM2(m->param2);
            y = GET_Y_PARAM2(m->param2);

            lcd->setColor(lcd, 0, 100, 100, 100);
            lcd->drawPixel(lcd, x,y);
            graphDrawCircle(x, y, 3);

            ox = x;
            oy = y;
            break;

        case MSG_POINTERUP:
            x = GET_X_PARAM2(m->param2);
            y = GET_Y_PARAM2(m->param2);

            lcd->setColor(lcd, 0, 100, 100, 100);
            lcd->drawPixel(lcd, x,y);
            graphDrawCircle(x, y, 3);

            //spped
            lcd->setColor(lcd, 0, 128, 0, 0);
            graphDrawLine(x,y,x+GET_DX_PARAM1(m->param1),y+GET_DY_PARAM1(m->param1));

            ox = x;
            oy = y;
            break;
    }
}

void vApplicationStackOverflowHook();
    UINT32 *fugi = NULL;
static void appMainTask( void *pvParameters )
{
    struct msgListener *list;
    struct hldDiskDevice *disk;
    struct hldLcdDevice *lcd;
    struct msg m;

    // Init HW and SW
    if (appInit() != SUCCESS)
        appInitErrHandler();

#ifdef LCD_FPGA_TEST
    lcd = hldDeviceGetByType(NULL, HLD_DEVICE_TYPE_LCD);

    LOG("****************************");
    LOG("* !THIS IS FPGA TEST MODE! *");
    LOG("****************************");
    LOG("Lcd dev: %p", lcd);

    UINT8 r = 0,g = 0,b = 0;
    UINT16 x,y;
    UINT32 counter = 0;
    //srand(xTaskGetTickCount());
    
    graphSetDrawingColor(0xff, 0x00, 0x00, 0x00);
    graphDrawRect(0, 0, lcd->getMaxX(), lcd->getMaxY());

    UINT32 startMs = xTaskGetTickCount();
    while(1)
    {
        if (counter < 1000)
        {
            counter++;
        }
        else
        {
            break;
        }
        
        r = rand() % 255;
        g = rand() % 255;
        b = rand() % 255;
        lcd->setColor(lcd, 0xff, r, g, b);

        x = rand() % (lcd->getMaxX()-100);
        y = rand() % (lcd->getMaxY()-100);

        lcd->fill(lcd, x, y, x+100, y+100);

        //vTaskDelay(100);
    }

    LOG("%d 100x100 rects in %dms",counter, xTaskGetTickCount()-startMs);

    while(1);

#endif
    // Create windows
    if (appPlayerInit() != SUCCESS)
        appInitErrHandler();

    appInitScreenWait();

    // Show first window
    appPlayerShow();
    list = msgListenerCreate(DEF_MSG_QUEUE_SIZE);
    while(msgListenerGet(list, &m, NULL, 0, 0))
    {
        diskstatus ds;
        switch(m.message)
        {
            case MSG_DISKREMOVED:
                disk = hldDeviceGetById(HLD_DEVICE_TYPE_DISK, 0);
                if (disk != NULL)
                {
                    ds = disk->close(disk);
                    LOG("Disk status: %x", ds);
                }
                break;

            case MSG_DISKINSERTED:
                disk = hldDeviceGetById(HLD_DEVICE_TYPE_DISK, 0);
                if (disk != NULL)
                {
                    ds = disk->open(disk);
                    LOG("Disk status: %x", ds);
                }
                break;

            case MSG_KEYUP:
                switch(m.param1)
                {
                    case V_KEY_5:
                        m.param2 = *fugi;
                        break;
                    case V_KEY_6:
                        vApplicationStackOverflowHook();
                        break;
                    case V_KEY_1:
                        showCursor = 1 - showCursor;
                        break;
                    case V_KEY_2:
                        graphClearScreen();
                        graphTestDrawRand1kLines();
                        break;
                    case V_KEY_3:
                        graphClearScreen();
                        graphTestDrawRand30Circles();
                        break;
                    case V_KEY_4:
                        graphClearScreen();
                        graphTestDrawEdgeMesh();
                        break;
                    case V_KEY_0:   
                        appPlayerShow();
                        break;


                }
                break;
        }

        if (showCursor == TRUE)
            showCursorProc(&m);
        msgDispatch(&m);
    }
    msgListenerDelete(list);
}

int main(void)
{
    boardInit();

    xTaskCreate( appMainTask, ( const signed char * const ) "App",
            4096, NULL, tskIDLE_PRIORITY, NULL );
    
    vTaskStartScheduler();

    while(1);
}

#ifdef LCD_FPGA
#define SCALE_X 2.5f
#define SCALE_Y 2
#else
#define SCALE_X 1
#define SCALE_Y 1s
#endif

void fpgaDelay()
{
#ifdef LCD_FPGA
    volatile UINT32 v;
    for (v = 0; v < 500000; v++);
#endif
}

void vApplicationStackOverflowHook( void )
{
    char buf[30];
    char taskNameBuf[10];
    struct hldLcdDevice *lcd = hldDeviceGetById(HLD_DEVICE_TYPE_LCD, 0);

#ifdef LCD_FPGA
    guiSetDefaultFont(&g_DroidSans29);
#endif
    
    graphSetDrawingColor(255, 255, 0, 0);
    graphDrawRect(0, 0, lcd->getMaxX(), lcd->getMaxY());

    graphSetDrawingColor(255, 255, 255, 255);
    graphDrawRect(10*SCALE_X, 10*SCALE_Y, 300*SCALE_X, 30*SCALE_Y);

    fpgaDelay();

    graphSetDrawingColor(255, 255, 0, 0);
    graphDrawText(10*SCALE_X, 10*SCALE_Y, 300*SCALE_X, 30*SCALE_Y, "IntenseOS", guiGetDefaultFont(),
            FS_ALIGN_CENTER | FS_VALIGN_TOP);

    fpgaDelay();

    graphSetDrawingColor(255, 255, 255, 255);

    graphDrawText(10*SCALE_X, 50*SCALE_Y, 220*SCALE_X, 30*SCALE_Y, "Stack overflow detected",
            guiGetDefaultFont(), FS_ALIGN_LEFT);

    fpgaDelay();

    vTaskGetName(taskNameBuf);

    sprintf(buf, "Task name = \"%s\"", taskNameBuf);
    graphDrawText(10*SCALE_X, 80*SCALE_Y, 220*SCALE_X, 30*SCALE_Y, buf, guiGetDefaultFont(), FS_ALIGN_LEFT);
    sprintf(buf, "Stack = 0x%08x", uxTaskGetStackAddr());
    fpgaDelay();
    graphDrawText(10*SCALE_X, 110*SCALE_Y, 220*SCALE_X, 30*SCALE_Y, buf, guiGetDefaultFont(), FS_ALIGN_LEFT);

    fpgaDelay();
    
    sprintf(buf, "Stack top = 0x%08x", uxTaskGetStackTopAddr());
    graphDrawText(10*SCALE_X, 140*SCALE_Y, 220*SCALE_X, 30*SCALE_Y, buf, guiGetDefaultFont(), FS_ALIGN_LEFT);

    fpgaDelay();
    graphDrawText(10*SCALE_X, 200*SCALE_Y, 220*SCALE_X, 30*SCALE_Y, "Dev. need to be restarted",
            guiGetDefaultFont(), FS_ALIGN_LEFT);

    for (;;);
}

void _general_exception_handler( unsigned long ulCause, unsigned long ulStatus )
{
    char buf[20];
    struct hldLcdDevice *lcd = hldDeviceGetById(HLD_DEVICE_TYPE_LCD, 0);

#ifdef LCD_FPGA
    guiSetDefaultFont(&g_DroidSans29);
#endif
    
    graphSetDrawingColor(255, 0, 0, 255);
    graphDrawRect(0, 0, lcd->getMaxX(), lcd->getMaxY());

    graphSetDrawingColor(255, 255, 255, 255);
    graphDrawRect(10*SCALE_X, 10*SCALE_Y, 300*SCALE_X, 30*SCALE_Y);

    graphSetDrawingColor(255, 0, 0, 255);
    graphDrawText(10*SCALE_X, 10*SCALE_Y, 300*SCALE_X, 30*SCALE_Y, "IntenseOS", guiGetDefaultFont(),
            FS_ALIGN_CENTER | FS_VALIGN_TOP);

    fpgaDelay();
    
    graphSetDrawingColor(255, 255, 255, 255);

    graphDrawText(10*SCALE_X, 50*SCALE_Y, 220*SCALE_X, 30*SCALE_Y, "Fatal exception occured",
            guiGetDefaultFont(), FS_ALIGN_LEFT);

    sprintf(buf, "CAUSE = 0x%08x", ulCause);
    graphDrawText(10*SCALE_X, 80*SCALE_Y, 220*SCALE_X, 30*SCALE_Y, buf, guiGetDefaultFont(), FS_ALIGN_LEFT);
    fpgaDelay();
    sprintf(buf, "STATUS = 0x%08x", ulStatus);
    graphDrawText(10*SCALE_X, 110*SCALE_Y, 220*SCALE_X, 30*SCALE_Y, buf, guiGetDefaultFont(), FS_ALIGN_LEFT);

    fpgaDelay();
    
    sprintf(buf, "EPC = 0x%08x", _CP0_GET_EPC());
    graphDrawText(10*SCALE_X, 140*SCALE_Y, 220*SCALE_X, 30*SCALE_Y, buf, guiGetDefaultFont(), FS_ALIGN_LEFT);

    fpgaDelay();
    graphDrawText(10*SCALE_X, 200*SCALE_Y, 220*SCALE_X, 30*SCALE_Y, "Dev. need to be restarted",
            guiGetDefaultFont(), FS_ALIGN_LEFT);

    for (;;);
}



