/**
 * @file hld.h
 * @brief High Level Driver manager
 * @author Daniel Szot
 */

#ifndef _HLD_H_
#define _HLD_H_

#include "lib/common.h"
#include "lib/log.h"

// Maxumum length of device name
#define HLD_DEVICE_MAX_NAME_LEN     32
// Maximum number of defices at all
#define HLD_DEVICE_MAX_NUMBER       32

// If defined function hldDeviceRemove compares devices by names
// if not - it uses pointers to compare devices. Not defined is quicker method
//#define HLD_DEVICE_REMOVE_BY_NAME

// Device type
// If you would like to add another type of device fell free to add
// something there
typedef enum
{
    HLD_DEVICE_TYPE_LCD      = 0x0001,
    HLD_DEVICE_TYPE_DAC      = 0x0002,
    HLD_DEVICE_TYPE_LED      = 0x0004,
    HLD_DEVICE_TYPE_KEYBOARD = 0x0008,

    // To check that smth is CHAR device check:
    // if ((type & HLD_DEVICE_TYPE_CHAR) != 0)
    HLD_DEVICE_TYPE_CHAR     = 0x0010,
    HLD_DEVICE_TYPE_UART     = 0x0010 | 0x0020,

    HLD_DEVICE_TYPE_ADC      = 0x0040,
    HLD_DEVICE_TYPE_TOUCH    = 0x0080,
    HLD_DEVICE_TYPE_IR       = 0x0100,
    HLD_DEVICE_TYPE_AUDIO    = 0x0200,
    HLD_DEVICE_TYPE_DISK    = 0x0400,
} hldDeviceType;

// Currect device state
typedef enum
{
    HLD_DEVICE_STATE_TURNED_ON  = 0x00010001,   // Device have supply
    HLD_DEVICE_STATE_RUNNING    = 0x00010002,   // Device is running
} hldDeviceState;

struct hldDevice
{
    struct hldDevice *next;
    hldDeviceType type;
    UINT16 id;
    INT8 name[HLD_DEVICE_MAX_NAME_LEN];
    hldDeviceState state;
    
    // Private data of each device
    // If you would like to use it in your lld you need to allocate
    // memmory for this using pvPortMalloc
    void *priv;
};

struct hldDevice * hldDeviceGetFirst();
void *hldDeviceGetByName(INT8 *pName);
void *hldDeviceGetByType(void *pStartNode, hldDeviceType pType);
void *hldDeviceGetById(hldDeviceType pType, UINT16 pId);

void *hldDeviceAlloc(INT8 *pName, hldDeviceType pType, UINT32 pSize);
retcode hldDeviceRegister(void *pDevice);
void  hldDeviceFree(void *pDevice);

#endif
