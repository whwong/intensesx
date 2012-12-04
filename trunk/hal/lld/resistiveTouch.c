/**
 * @file resistiveTouch.c
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
#include "lib/input/inputEvent.h"

#include "resistiveTouch.h"

static char lldResTouchName[HLD_DEVICE_MAX_NAME_LEN] =
    "4-Wire Resistive Touch Screen";

static const signed char lldResTouchTaskName[configMAX_TASK_NAME_LEN] = "4wt";

static xSemaphoreHandle touchMutex = NULL;
static xTaskHandle touchTaskHandle;
static struct hldAdcDevice *touchAdcDev;
static volatile INT32 adcX = -1;
static volatile INT32 adcY = -1;
static volatile enum lldResTouchTaskStates taskState = SET_X;
static struct digitalFilter *filterX;
static struct digitalFilter *filterY;

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
            LLD_RES_TOUCH_STACK_SIZE, (void *)pTouchDev,
            pTouchDev->config.taskPriority, &touchTaskHandle);

    if (tcRes == FALSE)
    {
        vSemaphoreDelete(touchMutex);
        ERROR("Failed to create %s device task", lldResTouchName);
        return ERR_NO_MEMMORY;
    }

    filterX = filterCreate(TOUCH_FILTER_WINDOW_SIZE);
    filterY = filterCreate(TOUCH_FILTER_WINDOW_SIZE);

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
    filterDelete(filterX);
    filterDelete(filterY);
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

    adcX = -1;
    adcY = -1;

    if (xSemaphoreTake(touchMutex, portMAX_DELAY))
    {
        if  (filterIsReady(filterX) && filterIsReady(filterY))
        {
            adcX = filterGetValue(filterX);
            adcY = filterGetValue(filterY);
        }

        xSemaphoreGive(touchMutex);
    }
    
    *pX = lldResTouchGetX(pTouchDev);
    *pY = lldResTouchGetY(pTouchDev);

    assert(pX != NULL);
    assert(pY != NULL);
    
    return SUCCESS;
}

static void lldResTouchNotifyEvent(struct hldTouchDevice *pDev)
{
    static UINT16 isTouch = 0, lastIsTouch = 0;
    static struct inputTouchEvent event;
    event.type = EVENT_TOUCH;
    event.timestamp = time();

    if (xSemaphoreTake(touchMutex, portMAX_DELAY))
    {
        if  (filterIsReady(filterX) && filterIsReady(filterY))
        {
            adcX = filterGetValue(filterX);
            adcY = filterGetValue(filterY);
            isTouch = 1;
        }
        else
            isTouch = 0;

        xSemaphoreGive(touchMutex);
    }

    if (isTouch != lastIsTouch)
    {
        // There was a change so we have a touch down or up event
        if (isTouch == 1)
        {
            //down
            event.action = EVENT_TOUCH_DOWN;
            event.positionX = lldResTouchGetX(pDev);
            event.positionY = lldResTouchGetY(pDev);

            event.speedX = 0;
            event.speedY = 0;
            inputEventNotify((struct inputEvent*)&event);
        }
        else
        {
            //up
            event.action = EVENT_TOUCH_UP;
            inputEventNotify((struct inputEvent*)&event);
        }
    }
    else if (isTouch == 1)
    {
        // We have a touch and we are moving
        event.action = EVENT_TOUCH_MOVE;
        event.speedX = event.positionX;
        event.speedY = event.positionY;

        event.positionX = lldResTouchGetX(pDev);
        event.positionY = lldResTouchGetY(pDev);

        event.speedX = (INT32)event.positionX - event.speedX;
        event.speedY = (INT32)event.positionY - event.speedY;

        // If we multiply speed by 10/dev->config.sampleTime
        // we will get speed in pixels in 10ms
#ifdef SPEED_IN_PXIN10MS
        event.speedX *= 10/pDev->config.sampleTime;
        event.speedY *= 10/pDev->config.sampleTime;
#endif

        if (event.speedX  != 0 || event.speedY != 0)
            inputEventNotify((struct inputEvent*)&event);
    }

    lastIsTouch = isTouch;
}

static void lldResTouchTask(void *pvParameters)
{
    struct hldTouchDevice *dev = (struct hldTouchDevice *)pvParameters;
    static INT32 tempX, tempY;

    while(1)
    {
        switch(taskState)
        {
            case GENERATE_EVENT:
                lldResTouchNotifyEvent(dev);
                taskState = SET_X;
                break;

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
                    taskState = SET_Y;
                }
                else
                {
                    if (xSemaphoreTake(touchMutex, portMAX_DELAY))
                    {
                        filterResetSamples(filterX);
                        xSemaphoreGive(touchMutex);
                    }
                    
                    taskState = GENERATE_EVENT;
                }
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
                    taskState = SET_VALUES;
                }
                else
                {
                    if (xSemaphoreTake(touchMutex, portMAX_DELAY))
                    {
                        filterResetSamples(filterY);
                        xSemaphoreGive(touchMutex);
                    }
                    
                    taskState = GENERATE_EVENT;
                }
                break;

            case SET_VALUES:
                if (xSemaphoreTake(touchMutex, portMAX_DELAY))
                {
                    filterAddSample(filterX, tempX);
                    filterAddSample(filterY, tempY);

                    xSemaphoreGive(touchMutex);
                }

                taskState = GENERATE_EVENT;
                break;

            default:
                taskState = GENERATE_EVENT;
        }

        vTaskDelay(dev->config.sampleTime/6);
    }
}
