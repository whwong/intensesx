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
    UINT16 idx=0;
    for (idx = 0; idx < 320; idx+=4)
    {
        graphSetDrawingColor(0, 255,0,0);
        UINT16 x1,y1,x2,y2;
        x1=0;
        y1=320-idx;
        x2=idx;
        y2=0;
        graphDrawLine(x1,y1,x2,y2);
        graphDelayMs(15);
    }
    graphDelayMs(1000);
    for (idx = 0; idx < 320; idx+=4)
    {
        graphSetDrawingColor(0, 0,255,0);
        UINT16 x1,y1,x2,y2;
        x1=240;
        y1=idx;
        x2=240-idx;
        y2=320;
        graphDrawLine(x1,y1,x2,y2);
        graphDelayMs(15);
    }
    graphDelayMs(1000);
}

void graphTestDrawRand1kLines()
{
    UINT16 i;
    srand(0xbadf00d);
    for (i = 0; i < 1000; i++)
    {
        UINT16 x1,y1,x2,y2;
        graphSetDrawingColor(0, rand()%255, rand()%255, rand()%255);
        x1=rand()%240;
        y1=rand()%320;
        x2=rand()%240;
        y2=rand()%320;
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
    UINT16 i;
    srand(0xbadf00d);
    for (i = 0; i < 30; i++)
    {
        graphSetDrawingColor(0, 0, 0, rand()%255);
        graphDrawCircle(rand()%128+16, rand()%100+16, rand()%16);
    }
}

#endif
#endif
