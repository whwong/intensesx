/**
 * @file primitives.c
 * @brief Part of Graphics library. Drawing primitives like lines, circles etc.
 * @author Daniel Szot
 */

#include "graphics.h"

retcode graphDrawLine(UINT16 pX1, UINT16 pY1, UINT16 pX2, UINT16 pY2)
{
    INT16   deltaX, deltaY;
    INT16   error, stepErrorLT, stepErrorGE;
    INT16   stepX, stepY;
    INT16   steep;
    INT16   temp;
    INT16   style, type;

    if (graphLcdDev == NULL)
        return DEVICE_NOT_SET;
    
    if(pX1 == pX2)
    {
        if(pY1 > pY2)
        {
            temp = pY1;
            pY1 = pY2;
            pY2 = temp;
        }

        style = 0;
        type = 1;
        for(temp = pY1; temp < pY2 + 1; temp++)
        {
            graphLcdDev->drawPixel(graphLcdDev, pX1, temp);
        }

        return (1);
    }

    if(pY1 == pY2)
    {
        if(pX1 > pX2)
        {
            temp = pX1;
            pX1 = pX2;
            pX2 = temp;
        }

        style = 0;
        type = 1;
        for(temp = pX1; temp < pX2 + 1; temp++)
        {
            graphLcdDev->drawPixel(graphLcdDev, temp, pY1);
        }

        return (1);
    }

    stepX = 0;
    deltaX = pX2 - pX1;
    if(deltaX < 0)
    {
        deltaX = -deltaX;
        --stepX;
    }
    else
    {
        ++stepX;
    }

    stepY = 0;
    deltaY = pY2 - pY1;
    if(deltaY < 0)
    {
        deltaY = -deltaY;
        --stepY;
    }
    else
    {
        ++stepY;
    }

    steep = 0;
    if(deltaX < deltaY)
    {
        ++steep;
        temp = deltaX;
        deltaX = deltaY;
        deltaY = temp;
        temp = pX1;
        pX1 = pY1;
        pY1 = temp;
        temp = stepX;
        stepX = stepY;
        stepY = temp;
        graphLcdDev->drawPixel(graphLcdDev, pY1, pX1);
    }
    else
    {
        graphLcdDev->drawPixel(graphLcdDev, pX1, pY1);
    }

    // If the current error greater or equal zero
    stepErrorGE = deltaX << 1;

    // If the current error less than zero
    stepErrorLT = deltaY << 1;

    // Error for the first pixel
    error = stepErrorLT - deltaX;

    style = 0;
    type = 1;

    while(--deltaX >= 0)
    {
        if(error >= 0)
        {
            pY1 += stepY;
            error -= stepErrorGE;
        }

        pX1 += stepX;
        error += stepErrorLT;


        if(steep)
        {
            graphLcdDev->drawPixel(graphLcdDev, pY1, pX1);
        }
        else
        {
            graphLcdDev->drawPixel(graphLcdDev, pX1, pY1);
        }
    }

    return SUCCESS;
}

retcode graphDrawRect(UINT16 pX1, UINT16 pY1, UINT16 pX2, UINT16 pY2)
{
    if (graphLcdDev == NULL)
        return DEVICE_NOT_SET;

    graphLcdDev->fill(graphLcdDev, pX1, pY1, pX2, pY2);

    return SUCCESS;
}

retcode graphDrawCircle(UINT16 pX, UINT16 pY, UINT16 pRadius)
{
    if (graphLcdDev == NULL)
        return DEVICE_NOT_SET;

    INT16 r = (INT16)pRadius;
    INT16 x = -r, y = 0, err = 2 - 2 * r; /* II. Quadrant */
    do
    {
        graphLcdDev->drawPixel(graphLcdDev, pX - x, pY + y); /*   I. Quadrant */
        graphLcdDev->drawPixel(graphLcdDev, pX - y, pY - x); /*  II. Quadrant */
        graphLcdDev->drawPixel(graphLcdDev, pX + x, pY - y); /* III. Quadrant */
        graphLcdDev->drawPixel(graphLcdDev, pX + y, pY + x); /*  IV. Quadrant */
        r = err;

        if (r > x)
            err += ++x * 2 + 1; /* e_xy+e_x > 0 */
        if (r <= y)
            err += ++y * 2 + 1; /* e_xy+e_y < 0 */
        
    } while (x < 0);

    return SUCCESS;
}

retcode graphDrawRoundedRect();
