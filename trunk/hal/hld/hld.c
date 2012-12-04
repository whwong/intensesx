/**
 * @file hld.c
 * @brief High Level Driver manager
 * @author Daniel Szot
 */

#include "hld.h"
#include "semphr.h"

static struct hldDevice *hldFirstDevice = NULL;
static xSemaphoreHandle hldMutex =  NULL;

/**
 * @brief Gets pointer to first device in list
 *
 * @return Pointer to first device on list. NULL - no devices.
 */
struct hldDevice * hldDeviceGetFirst()
{
    return hldFirstDevice;
}

/**
 * @brief Gets device pointer by device name
 *
 * @param pName Name of schearching device
 * @return Pointer to founded device. NULL when not found.
 */
void *hldDeviceGetByName(INT8 *pName)
{
    struct hldDevice *dev;

    assert(hldMutex != NULL);

    if (xSemaphoreTakeRecursive(hldMutex, portMAX_DELAY))
    {
        // Loop by all devices
        for (dev = hldFirstDevice; dev != NULL; dev = dev->next)
        {
            if (strcmp(dev->name, pName) == 0)
            {
                xSemaphoreGiveRecursive(hldMutex);
                // Device founded, return
                return dev;
            }
        }

        xSemaphoreGiveRecursive(hldMutex);
    }
    else
    {
        MUTEX_ERROR();
        return NULL;
    }

    return NULL;
}

/**
 * @brief Gets first founded device with specified type
 *
 * @param pStartNode Start node, when NULL starts from the begining
 * @param pType Type of schearching device
 * @return Pointer to founded device. NULL when not found.
 */
void *hldDeviceGetByType(void *pStartNode, hldDeviceType pType)
{
    struct hldDevice *dev;

    assert(hldMutex != NULL);

    if (xSemaphoreTakeRecursive(hldMutex, portMAX_DELAY))
    {
        // If pStartNode = NULL start searching from begin of the list
        dev = (pStartNode == NULL) ? hldFirstDevice : pStartNode;

        // Loop by all devices from dev
        for (; dev != NULL; dev = dev->next)
        {
            if (dev->type == pType)
            {
                xSemaphoreGiveRecursive(hldMutex);
                // Device founded, return
                return dev;
            }
        }
        
        xSemaphoreGiveRecursive(hldMutex);
    }
    else
    {
        MUTEX_ERROR();
        return NULL;
    }

    return NULL;
}

/**
 * @brief Gets first founded device with specified type and id
 * @param pType Type of schearching device
 * @param pId Id of schearching device
 * @return Pointer to founded device. NULL when not found.
 */
void *hldDeviceGetById(hldDeviceType pType, UINT16 pId)
{
    struct hldDevice *dev;

    assert(hldMutex != NULL);

    if (xSemaphoreTakeRecursive(hldMutex, portMAX_DELAY))
    {
        // Loop by all devices
        for (dev = hldFirstDevice; dev != NULL; dev = dev->next)
        {
            if ((dev->type == pType) && (dev->id == pId))
            {
                xSemaphoreGiveRecursive(hldMutex);
                // Device founded, return
                return dev;
            }
        }
        
        xSemaphoreGiveRecursive(hldMutex);
    }
    else
    {
        MUTEX_ERROR();
        return NULL;
    }

    return NULL;
}

/**
 * @brief Adds device to device list
 *
 * @param pDev Device to add
 * @return Return value
 */
static retcode hldDeviceAdd(struct hldDevice *pDev)
{
    volatile struct hldDevice *dp;

    if (hldFirstDevice == NULL)
    {
        if (hldMutex == NULL)
            hldMutex = xSemaphoreCreateMutex();

        if ((hldMutex != NULL) && (xSemaphoreTake(hldMutex, portMAX_DELAY)))
        {
            // If we have not any device just set this one as first
            hldFirstDevice = pDev;
            pDev->next = NULL;
            xSemaphoreGive(hldMutex);
            DONE("HLD Device %s added", pDev->name);
            return SUCCESS;
        }
        else
        {
            MUTEX_ERROR();
            return ERR_MUTEX;
        }
    }

    assert(hldMutex != NULL);

    /*// Check taht name is unique
    if (hldDeviceGetByName(pDev->name) != NULL)
    {
        xSemaphoreGive(hldMutex);
        ERROR("HLD Device named %s already exists", pDev->name);
        return ERR_HLD_DEVICE_NAME_EXISTS;
    }*/

    if (xSemaphoreTakeRecursive(hldMutex, portMAX_DELAY))
    {
        // Check taht name is unique
        if (hldDeviceGetByName(pDev->name) != NULL)
        {
            xSemaphoreGiveRecursive(hldMutex);
            ERROR("HLD Device named %s already exists", pDev->name);
            return ERR_HLD_DEVICE_NAME_EXISTS;
        }
        
        // Find pointer to last device
        dp = hldFirstDevice;
        while (dp->next != NULL)
        {
            dp = dp->next;
        }

        // Add device to device list
        dp->next = pDev;
        pDev->next = NULL;

        xSemaphoreGiveRecursive(hldMutex);
        DONE("HLD Device %s added", pDev->name);
        return SUCCESS;
    }
    else
    {
        MUTEX_ERROR();
        return ERR_NO_MEMMORY;
    }
}

/**
 * @brief Removes device from device list
 * 
 * @param pDev Device to remove
 * @return Return value
 */
static retcode hldDeviceRemove(struct hldDevice *pDev)
{
    struct hldDevice *dp;

    assert(hldMutex != NULL);

    if (xSemaphoreTakeRecursive(hldMutex, portMAX_DELAY))
    {
#ifdef HLD_DEVICE_REMOVE_BY_NAME
        // Check that we have some devices
        if (hldFirstDevice != NULL)
        {
            // Check that we need to remove first device
            if (strcmp(hldFirstDevice->name, pDev->name) == 0)
            {
                hldFirstDevice = pDev->next;
            }
            else
            {
                // Find device to remove
                for (dp = hldFirstDevice; dp->next != NULL; dp = dp->next)
                {
                    if (strcmp(dp->next->name, pDev->name) == 0)
                    {
                        // Device found, remove.
                        dp->next = pDev->next;
                        xSemaphoreGiveRecursive(hldMutex);
                        return SUCCESS;
                    }
                }
            }
        }
#else
        if (hldFirstDevice != NULL)
        {
            // Check that we need to remove first device
            if (hldFirstDevice == pDev)
            {
                hldFirstDevice = pDev->next;
            }
            else
            {
                // Find device to remove
                for (dp = hldFirstDevice; dp->next != NULL; dp = dp->next)
                {
                    if (dp->next == pDev)
                    {
                        // Device found, remove.
                        dp->next = pDev->next;
                        xSemaphoreGiveRecursive(hldMutex);
                        DONE("HLD Device %s removed", pDev->name);
                        return SUCCESS;
                    }
                }
            }
        }

        xSemaphoreGiveRecursive(hldMutex);
#endif
    }
    else
    {
        MUTEX_ERROR();
        return ERR_NO_MEMMORY;
    }

    WARNING("HLD Device %s not found", pDev->name);
    return WAR_HLD_DEVICE_NOT_FOUND;
}

/**
 * @brief Allocates memmory for new device structure
 *
 * @param pName New device name
 * @param pType New device type
 * @param pSize Device structure size
 * @return Pointer to allocated memmory
 */
void *hldDeviceAlloc(INT8 *pName, hldDeviceType pType, UINT32 pSize)
{
    struct hldDevice *dev;
    struct hldDevice *dp;
    UINT16 id;

    if (hldMutex == NULL)
        hldMutex = xSemaphoreCreateMutex();

    if (hldMutex != NULL && xSemaphoreTake(hldMutex, portMAX_DELAY))
    {
        dev = (struct hldDevice *)pvPortMalloc(pSize);

        // If alloc fail return NULL
        if (dev == NULL)
        {
            xSemaphoreGive(hldMutex);
            ERROR("HLD Device memmory alocation failed");
            return NULL;
        }

        // Calculate device id
        for (id = 0, dp = hldFirstDevice; dp != NULL; dp = dp->next)
        {
            // Commented because we are checking name unique in hldDeviceRegister
            /*if (strcmp(dp->name, pName) == 0)
            {
                // Device found
                vPortFree(dev);
                ERROR("HLD Device name \"%s\" already exists. Device name must be unique", pName);
                return NULL;
            }*/

            // Calculate id
            if (dp->type == pType)
                id++;
        }

        /* Clear device structure */
        memset((UINT8 *)dev, 0, pSize);

        dev->type = pType;
        dev->id = id;
        strcpy(dev->name, pName);
        
        xSemaphoreGive(hldMutex);
    }
    else
    {
        MUTEX_ERROR();
        return NULL;
    }

    return dev;
}

/**
 * @brief Registers new device in HAL
 *
 * @param pDevice Pointer to device which will be registered
 * @return Result value
 */
retcode hldDeviceRegister(void *pDevice)
{
    return hldDeviceAdd((struct hldDevice*)pDevice);
}

/**
 * @brief Frees device
 * @param pDevice Pointer to device
 */
void  hldDeviceFree(void *pDevice)
{
    if (pDevice != NULL)
    {
        hldDeviceRemove((struct hldDevice *)pDevice);
        vPortFree(((struct hldDevice *)pDevice)->priv);
        vPortFree(pDevice);
    }

    return;
}
