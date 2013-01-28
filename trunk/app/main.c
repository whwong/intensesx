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

static FATFS fat;

int main(void);

INT32 firstWindowProc(struct guiWindow *pWnd, UINT32 pMsg, UINT32 pParam1, UINT32 pParam2)
{
    struct hldLcdDevice *lcd;
    static INT32 x = 100,y = 100, ox=100, oy=100;

    lcd = hldDeviceGetById(HLD_DEVICE_TYPE_LCD, 0);

    switch(pMsg)
    {
        case MSG_POINTERMOVE:
            x = GET_X_PARAM2(pParam2);
            y = GET_Y_PARAM2(pParam2);

            lcd->setColor(lcd, 0, 255, 255, 255);
            graphDrawLine(x,y,ox,oy);

            lcd->setColor(lcd, 0, 30, 30, 255);
            lcd->drawPixel(lcd, x,y);
            lcd->drawPixel(lcd, ox,oy);

            ox = x;
            oy = y;
            break;

        case MSG_POINTERDOWN:
            x = GET_X_PARAM2(pParam2);
            y = GET_Y_PARAM2(pParam2);

            lcd->setColor(lcd, 0, 100, 100, 100);
            lcd->drawPixel(lcd, x,y);
            graphDrawCircle(x, y, 3);

            ox = x;
            oy = y;
            break;

        case MSG_POINTERUP:
            x = GET_X_PARAM2(pParam2);
            y = GET_Y_PARAM2(pParam2);

            lcd->setColor(lcd, 0, 100, 100, 100);
            lcd->drawPixel(lcd, x,y);
            graphDrawCircle(x, y, 3);

            //spped
            lcd->setColor(lcd, 0, 128, 0, 0);
            graphDrawLine(x,y,x+GET_DX_PARAM1(pParam1),y+GET_DY_PARAM1(pParam1));

            ox = x;
            oy = y;
            break;
    }
    
    return guiDefWindowProc(pWnd, pMsg, pParam1, pParam2);
}

void overflowme()
{
    UINT32 t[100];
    UINT32 tab[1024];

    taskYIELD();

    for (t[0] = 1; t[0] = 60; t[0]++)
    {
        t[t[0]] = t[0];
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

static void appMainTask( void *pvParameters )
{
    struct msgListener *list;
    struct hldDiskDevice *disk;
    struct msg m;

    // Init HW and SW
    if (appInit() != SUCCESS)
        appInitErrHandler();

    // Create windows
    if (appPlayerInit() != SUCCESS)
        appInitErrHandler();
    
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
        }
           
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

void vApplicationStackOverflowHook( void )
{
    char buf[30];
    char taskNameBuf[10];
    
    graphSetDrawingColor(255, 255, 0, 0);
    graphDrawRect(0, 0, 240, 320);

    graphSetDrawingColor(255, 255, 255, 255);
    graphDrawRect(10, 10, 220, 30);

    graphSetDrawingColor(255, 255, 0, 0);
    graphDrawText(10, 10, 220, 30, "IntenseOS", guiGetDefaultFont(),
            FS_ALIGN_CENTER | FS_VALIGN_TOP);

    graphSetDrawingColor(255, 255, 255, 255);

    graphDrawText(10, 50, 220, 30, "Stack overflow detected",
            guiGetDefaultFont(), FS_ALIGN_LEFT);
    graphDrawText(10, 100, 220, 30, "Details:", guiGetDefaultFont(), FS_ALIGN_LEFT);

    vTaskGetName(taskNameBuf);

    sprintf(buf, "Task name = \"%s\"", taskNameBuf);
    graphDrawText(10, 130, 220, 30, buf, guiGetDefaultFont(), FS_ALIGN_LEFT);
    sprintf(buf, "Stack = 0x%08x", uxTaskGetStackAddr());
    graphDrawText(10, 160, 220, 30, buf, guiGetDefaultFont(), FS_ALIGN_LEFT);
    sprintf(buf, "Stack top = 0x%08x", uxTaskGetStackTopAddr());
    graphDrawText(10, 190, 220, 30, buf, guiGetDefaultFont(), FS_ALIGN_LEFT);

    graphDrawText(10, 240, 220, 30, "Dev. need to be restarted",
            guiGetDefaultFont(), FS_ALIGN_LEFT);

    for (;;);
}

void _general_exception_handler( unsigned long ulCause, unsigned long ulStatus )
{
    char buf[20];
    
    graphSetDrawingColor(255, 0, 0, 255);
    graphDrawRect(0, 0, 240, 320);

    graphSetDrawingColor(255, 255, 255, 255);
    graphDrawRect(10, 10, 220, 30);

    graphSetDrawingColor(255, 0, 0, 255);
    graphDrawText(10, 10, 220, 30, "IntenseOS", guiGetDefaultFont(),
            FS_ALIGN_CENTER | FS_VALIGN_TOP);

    graphSetDrawingColor(255, 255, 255, 255);

    graphDrawText(10, 50, 220, 30, "Fatal exception occured",
            guiGetDefaultFont(), FS_ALIGN_LEFT);
    graphDrawText(10, 100, 220, 30, "Details:", guiGetDefaultFont(), FS_ALIGN_LEFT);

    sprintf(buf, "CAUSE = 0x%08x", ulCause);
    graphDrawText(10, 130, 220, 30, buf, guiGetDefaultFont(), FS_ALIGN_LEFT);
    sprintf(buf, "STATUS = 0x%08x", ulStatus);
    graphDrawText(10, 160, 220, 30, buf, guiGetDefaultFont(), FS_ALIGN_LEFT);
    sprintf(buf, "EPC = 0x%08x", _CP0_GET_EPC());
    graphDrawText(10, 190, 220, 30, buf, guiGetDefaultFont(), FS_ALIGN_LEFT);

    graphDrawText(10, 240, 220, 30, "Dev. need to be restarted",
            guiGetDefaultFont(), FS_ALIGN_LEFT);

    for (;;);
}



