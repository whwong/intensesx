/**
 * @file tests.h
 * @brief Graphics library Tests
 * @author Daniel Szot
 */

#ifndef _GRAPHICS_LIBRARY_TESTS_H
#define _GRAPHICS_LIBRARY_TESTS_H

#include "graphics.h"

#ifdef GRAPH_TESTS

void graphTestDrawEdgeMesh()
{
    struct hldLcdDevice *lcd = hldDeviceGetById(HLD_DEVICE_TYPE_LCD, 0);
    UINT16 idx=0;
    for (idx = 0; idx < lcd->getMaxY(); idx+=4)
    {
        graphSetDrawingColor(0, 255,0,0);
        UINT16 x1,y1,x2,y2;
        x1=0;
        y1=lcd->getMaxY()-idx;
        x2=idx;
        y2=0;
        graphDrawLine(x1,y1,x2,y2);
        graphDelayMs(15);
    }
    graphDelayMs(1000);
    for (idx = 0; idx < lcd->getMaxY(); idx+=4)
    {
        graphSetDrawingColor(0, 0,255,0);
        UINT16 x1,y1,x2,y2;
        x1=lcd->getMaxX();
        y1=idx;
        x2=lcd->getMaxX()-idx;
        y2=lcd->getMaxY();
        graphDrawLine(x1,y1,x2,y2);
        graphDelayMs(15);
    }
    graphDelayMs(1000);
}

void graphTestDrawRand1kLines()
{
    struct hldLcdDevice *lcd = hldDeviceGetById(HLD_DEVICE_TYPE_LCD, 0);
    UINT16 i;
    srand(0xbadf00d);
    for (i = 0; i < 1000; i++)
    {
        UINT16 x1,y1,x2,y2;
        graphSetDrawingColor(0, rand()%255, rand()%255, rand()%255);
        x1=rand()%lcd->getMaxX();
        y1=rand()%lcd->getMaxY();
        x2=rand()%lcd->getMaxX();
        y2=rand()%lcd->getMaxY();
        graphDrawLine(x1,y1,x2,y2);
    }
}

void graphTestDrawRand10kLines()
{
    UINT8 i;
    for (i = 0; i < 10; i++)
    {
        graphTestDrawRand1kLines();
    }
}

void graphTestDrawRand30Circles()
{
    struct hldLcdDevice *lcd = hldDeviceGetById(HLD_DEVICE_TYPE_LCD, 0);
    UINT16 i;
    //srand(0xbadf00d);
    for (i = 0; i < 30; i++)
    {
        graphSetDrawingColor(0, rand()%255, rand()%255, rand()%255);
        graphDrawCircle(rand()%lcd->getMaxY()+16, rand()%lcd->getMaxX()+16, rand()%16);
    }
}

void graphTestDrawRand300Circles()
{
    UINT8 i;
    for (i = 0; i < 10; i++)
    {
        graphTestDrawRand30Circles();
    }
}

void graphTestDraw1kRects()
{
    struct hldLcdDevice *lcd = hldDeviceGetById(HLD_DEVICE_TYPE_LCD, 0);
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
}

void graphTestDraw100VLines()
{
    struct hldLcdDevice *lcd = hldDeviceGetById(HLD_DEVICE_TYPE_LCD, 0);
    UINT8 r = 0,g = 0,b = 0;
    UINT16 x,y,h;
    UINT32 counter = 0;
    //srand(xTaskGetTickCount());

    graphSetDrawingColor(0xff, 0x00, 0x00, 0x00);
    graphDrawRect(0, 0, lcd->getMaxX(), lcd->getMaxY());

    UINT32 startMs = xTaskGetTickCount();
    while(1)
    {
        if (counter < 100)
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

        h = rand() % (lcd->getMaxY());
        x = rand() % (lcd->getMaxX()-1);
        y = rand() % (lcd->getMaxY()-h);

        lcd->fill(lcd, x, y, x+1, y+h);

        //vTaskDelay(100);
    }
}

void graphTestDraw100HLines()
{
    struct hldLcdDevice *lcd = hldDeviceGetById(HLD_DEVICE_TYPE_LCD, 0);
    UINT8 r = 0,g = 0,b = 0;
    UINT16 x,y,h;
    UINT32 counter = 0;
    //srand(xTaskGetTickCount());

    graphSetDrawingColor(0xff, 0x00, 0x00, 0x00);
    graphDrawRect(0, 0, lcd->getMaxX(), lcd->getMaxY());

    UINT32 startMs = xTaskGetTickCount();
    while (1)
    {
        if (counter < 100)
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

        h = rand() % (lcd->getMaxX());
        x = rand() % (lcd->getMaxX() - h);
        y = rand() % (lcd->getMaxY() - 2);

        lcd->fill(lcd, x, y, x + h, y + 1);

        //vTaskDelay(100);
    }
}

void graphTestDooubleBuffer(BOOL pDouble)
{
    struct hldLcdDevice *lcd = hldDeviceGetById(HLD_DEVICE_TYPE_LCD, 0);
    INT16 x = 100,y = 100;
    UINT16 w = 100, h = 100;
    INT16 dx = 10, dy = 10;
    UINT32 counter = 0;
    //srand(xTaskGetTickCount());


    while(1)
    {
        counter++;

        if (counter == 100)
            break;

        if (((x+w+dx) >= lcd->getMaxX()) || (x+dx < 0))
            dx = -dx;

        if (((y+h+dy) >= lcd->getMaxY()) || (y+dy < 0))
            dy = -dy;

        x += dx;
        y += dy;

        graphSetDrawingColor(0xff, 0xff, 0xff, 0xff);
        //graphDrawRect(0, 0, lcd->getMaxX(), lcd->getMaxY());
        lcd->ioctl(lcd, LC_CLEAR_SCREEN, 0);

        graphSetDrawingColor(0xff, 0xFF, 0x00, 0x00);
        graphDrawRect(x, y, x+w, y+h);

        if (pDouble)
            lcd->flush(lcd);

    }

    lcd->ioctl(lcd, LC_ENABLE_DOUBLE_BUFFER, 0);
}

#endif
#endif
