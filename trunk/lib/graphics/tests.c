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

#endif
#endif
