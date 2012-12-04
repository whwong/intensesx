/**
 * @file resistiveTouch.h
 * @brief 4-Wire Resistive Touch Screen Driver
 * Driver based on Microchip TouchScreen.c driver
 * @author Daniel Szot
 */

#include "lib/common.h"
#include "task.h"
#include "semphr.h"
#include "hal/hld/hld.h"
#include "hal/hld/touch.h"
#include "hal/hld/adc.h"
#include "lib/digitalFilter.h"

#include "resistiveTouch.h"

static char lldResTouchName[HLD_DEVICE_MAX_NAME_LEN] =
    "4-Wire Resistive Touch Screen";

static const signed char lldResTouchTaskName[configMAX_TASK_NAME_LEN] = "4wt";

static xSemaphoreHandle touchMutex = NULL;
static xTaskHandle touchTaskHandle = NULL;
static struct hldAdcDevice *touchAdcDev;
static volatile INT32 adcX = -1;
static volatile INT32 adcY = -1;
static volatile enum lldResTouchTaskStates taskState = SET_TOUCH;
static struct digitalFilter *filter;

/**
 * @brief Attach 4-Wire Resistive Touch Screen device
 *
 * @return Result value
 */
retcode lldResTouchAttach(struct hldTouchConfig *pCfg)
{
    struct hldTouchDevice *dev;

    assert(pCfg->sampleTime != 0);

    touchAdcDev = (struct hldAdcDevice *)hldDeviceGetByType(NULL,
            HLD_DEVICE_TYPE_ADC);

    if ((touchAdcDev == NULL) ||
            ((touchAdcDev->head.state & HLD_DEVICE_STATE_RUNNING) != HLD_DEVICE_STATE_RUNNING))
    {
        ERROR("Touch screen driver requires attached ADC device to run.");
        return ERR_ADC_DEVICE_NOT_FOUND;
    }

    // Only one device could be opened - this is guaranteed by unique name
    // requirement
    dev = hldDeviceAlloc(lldResTouchName, HLD_DEVICE_TYPE_TOUCH,
            sizeof(struct hldTouchDevice));

    if (dev == NULL)
        return ERR_NO_MEMMORY;

    dev->config = *pCfg;

    dev->attach =   lldResTouchAttach;
    dev->open =     lldResTouchOpen;
    dev->close =    lldResTouchClose;
    dev->read =     lldResTouchRead;

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

/**
 * @brief Open touch screen device
 * 
 * @param pTouchDev Pointer to device which will be opened
 * @return Result value
 */
static retcode lldResTouchOpen(struct hldTouchDevice *pTouchDev)
{
    BOOL tcRes;
    assert(pTouchDev != NULL);

    touchMutex = xSemaphoreCreateMutex();
    if (touchMutex == NULL)
    {
        MUTEX_ERROR();
        return ERR_MUTEX;
    }

    tcRes = xTaskCreate(lldResTouchTask, lldResTouchTaskName,
            configMINIMAL_STACK_SIZE*3, (void *)pTouchDev,
            pTouchDev->config.taskPriority, touchTaskHandle);

    if (tcRes == FALSE)
    {
        vSemaphoreDelete(touchMutex);
        ERROR("Failed to create %s device task", lldResTouchName);
        return ERR_NO_MEMMORY;
    }

    filter = filterCreate(TOUCH_FILTER_WINDOW_SIZE);

    pTouchDev->head.state |= HLD_DEVICE_STATE_RUNNING;

    DONE("%s device opened:", lldResTouchName);
    LOG("    Sample time: %d", pTouchDev->config.sampleTime);
    LOG("    Task priority: %d", pTouchDev->config.taskPriority);

    return SUCCESS;
}

/**
 * @brief Close touch screen device
 *
 * @param pTouchDev Pointer to device which channel will be closed
 * @return Result value
 */
static retcode lldResTouchClose(struct hldTouchDevice *pTouchDev)
{
    assert(pTouchDev != NULL);
    
    pTouchDev->head.state &= ~(HLD_DEVICE_STATE_RUNNING |
            HLD_DEVICE_STATE_TURNED_ON);

    vSemaphoreDelete(touchMutex);
    vTaskDelete(touchTaskHandle);
    filterDelete(filter);
    LOG("Resistive Touch Screen device closed");

    return SUCCESS;
}

static inline INT32 __attribute__ ((always_inline))
lldResTouchGetX(struct hldTouchDevice *pTouchDev)
{
    INT32 res;

#if (LCD_ORIENTATION == 90) || (LCD_ORIENTATION == 270)
    result = adcY;
#else
    res = adcX;
#endif

    if(res >= 0)
    {
#if (LCD_ORIENTATION == 90) || (LCD_ORIENTATION == 270)
        result = (pTouchDev->config.screenResolutionX * (res - pTouchDev->config.yMinCal)) /
                (pTouchDev->config.yMaxCal - pTouchDev->config.yMinCal);
#else
        res = (pTouchDev->config.screenResolutionX * (res - pTouchDev->config.xMinCal)) /
                (pTouchDev->config.xMaxCal - pTouchDev->config.xMinCal);
#endif

#if (LCD_ORIENTATION == 0) || (LCD_ORIENTATION == 360)
        res = pTouchDev->config.screenResolutionX - res;
#endif
    }
    
    return res;
}

static inline INT32 __attribute__ ((always_inline))
lldResTouchGetY(struct hldTouchDevice *pTouchDev)
{
    INT32 res;

#if (LCD_ORIENTATION == 90) || (LCD_ORIENTATION == 270)
    result = adcX;
#else
    res = adcY;
#endif

    if(res >= 0)
    {
#if (LCD_ORIENTATION == 90) || (LCD_ORIENTATION == 270)
        result = (pTouchDev->config.screenResolutionY * (res - pTouchDev->config.xMinCal)) /
                (pTouchDev->config.xMaxCal - pTouchDev->config.xMinCal);
#else
        res = (pTouchDev->config.screenResolutionY * (res - pTouchDev->config.yMinCal)) /
                (pTouchDev->config.yMaxCal - pTouchDev->config.yMinCal);
#endif

#if (LCD_ORIENTATION == 0) || (LCD_ORIENTATION == 360)
        res = pTouchDev->config.screenResolutionY - res;
#endif
    }

    return res;
}

/**
 * @brief Read single sample from device
 *
 * @param pTouchDev Pointer to device which will be used
 * @param pValue Pointer to save readed value. Value is is format like:
 * Xpos = (pValue & 0x00FF);
 * Ypos = (pValue & 0xFF00) >> 16;
 * @return Result value
 */
static inline retcode __attribute__ ((always_inline))
lldResTouchRead(struct hldTouchDevice *pTouchDev, INT32 *pX, INT32 *pY)
{
    assert(pTouchDev != NULL);

    *pX = lldResTouchGetX(pTouchDev);
    *pY = lldResTouchGetY(pTouchDev);

    assert(pX != NULL);
    assert(pY != NULL);
    
    return SUCCESS;
}

static void lldResTouchTask(void *pvParameters)
{
    struct hldTouchDevice *dev = (struct hldTouchDevice *)pvParameters;
    static INT32 tempX, lastTempX, tempY, lastTempY, toggle = 0;
    UINT16 temp;

    while(1)
    {
        switch(taskState)
        {
            case SET_X:
                TOUCH_TRIS_XNEG = 0;
                TOUCH_TRIS_XPOS = 0;
                TOUCH_TRIS_YNEG = 1;
                TOUCH_TRIS_YPOS = 1;

                TOUCH_LAT_XNEG = 0;
                TOUCH_LAT_XPOS = 1;
                taskState = GET_X;
                break;
                
            case GET_X:
                touchAdcDev->read(touchAdcDev, TOUCH_ADC_CH_XPOS, &tempX);
                if ((tempX >= dev->config.xMinCal) && (tempX <= dev->config.xMaxCal))
                {
                    //LOG("tX");

                    if (lastTempX != -1)
                        tempX = (tempX+lastTempX)/2;

                    taskState = SET_Y;
                }
                else
                {
                    adcX = -1;
                    adcY = -1;
                    tempX = -1;
                    //LOG("n");
                    taskState = SET_X;
                }
                lastTempX = tempX;
                break;

            case SET_Y:
                TOUCH_TRIS_XNEG = 1;
                TOUCH_TRIS_XPOS = 1;
                TOUCH_TRIS_YNEG = 0;
                TOUCH_TRIS_YPOS = 0;

                TOUCH_LAT_YNEG = 0;
                TOUCH_LAT_YPOS = 1;
                taskState = GET_Y;
                break;

            case GET_Y:
                touchAdcDev->read(touchAdcDev, TOUCH_ADC_CH_YPOS, &tempY);
                if ((tempY >= dev->config.yMinCal) && (tempY <= dev->config.yMaxCal))
                {
                    //LOG("tY");
                    if (lastTempY != -1)
                        tempY = (tempY+lastTempY)/2;

                    taskState = SET_VALUES;
                }
                else
                {
                    adcX = -1;
                    adcY = -1;
                    tempY = -1;
                    //LOG("n");
                    taskState = SET_X;
                }
                lastTempY = tempY;
                break;

            case SET_VALUES:
                adcX = tempX;
                adcY = tempY;
                //LOG("Touch: %d, %d", adcX, adcY);
                taskState = SET_X;
                break;

            default:
                taskState = SET_X;
        }

        // TODO: remove it
        LED2 = toggle;
        toggle = (toggle == 0) ? 1 : 0;

        vTaskDelay(dev->config.sampleTime/4);
    }
}
