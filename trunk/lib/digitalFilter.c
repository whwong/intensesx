/**
 * @file digitalFilter.c
 * @brief Linear digital filter implementation
 * @author Daniel Szot
 */

#include "lib/common.h"
#include "lib/log.h"
#include "digitalFilter.h"

struct digitalFilter *filterCreate(UINT32 windowSize)
{
    struct digitalFilter *filter;
    UINT32 size = sizeof(struct digitalFilter);

    assert(windowSize != 0);

    filter = (struct digitalFilter *)pvPortMalloc(size);
    assert(filter != NULL);

    filter->data = (INT32*)pvPortMalloc(sizeof(INT32)*windowSize);
    assert(filter->data != NULL);

    filter->windowSize = windowSize;
    filter->samplesCount = 0;
    filter->bufferPosition = 0;
    return filter;
}

void filterDelete(struct digitalFilter *pFilter)
{
    assert(pFilter != NULL);
    
    vPortFree(pFilter->data);
    vPortFree(pFilter);
}

void filterAddSample(struct digitalFilter *pFilter, INT32 pValue)
{
    assert(pFilter != NULL);
    
    pFilter->data[pFilter->bufferPosition++] = pValue;
    pFilter->bufferPosition %= pFilter->windowSize;
    pFilter->samplesCount++;
}

BOOL filterIsReady(struct digitalFilter *pFilter)
{
    assert(pFilter != NULL);

    return (pFilter->samplesCount >= pFilter->windowSize);
}

INT32 filterGetValue(struct digitalFilter *pFilter)
{
    UINT32 i = 0;
    INT32 value = 0;

    assert(pFilter != NULL);

    for (i = 0; i < pFilter->windowSize; i++)
    {
        value += pFilter->data[i];
    }

    value /= pFilter->windowSize;
    return value;
}

void filterResetSamples(struct digitalFilter *pFilter)
{
    UINT32 i = 0;

    assert(pFilter != NULL);

    for (i = 0; i < pFilter->windowSize; i++)
    {
        pFilter->data[i] = 0;
    }

    pFilter->samplesCount = 0;
    pFilter->bufferPosition = 0;
}

INT32 filterGetRejectMinMax(struct digitalFilter *pFilter)
{
    UINT32 i = 0, minIdx = 0, maxIdx = 0;
    INT32 value = 0, min = -65535, max = 65535;

    assert(pFilter != NULL);
    assert(pFilter->windowSize >= 3);

    for (i = 0; i < pFilter->windowSize; i++)
    {
        if (pFilter->data[i] > max)
        {
            max = pFilter->data[i];
            maxIdx = i;
        }

        if (pFilter->data[i] < min)
        {
            min = pFilter->data[i];
            minIdx = i;
        }
    }

    for (i = 0; i < pFilter->windowSize; i++)
    {
        if ((i != minIdx) && (i != maxIdx))
            value += pFilter->data[i];
    }

    value /= (pFilter->windowSize - 2);
    return value;
}

INT32 filterGetMax(struct digitalFilter *pFilter)
{
    UINT32 i = 0;
    INT32 max = -65535;

    assert(pFilter != NULL);

    for (i = 0; i < pFilter->windowSize; i++)
    {
        if (pFilter->data[i] > max)
            max = pFilter->data[i];
    }

    return max;
}

INT32 filterGetMin(struct digitalFilter *pFilter)
{
    UINT32 i = 0;
    INT32 min = 65535;

    assert(pFilter != NULL);

    for (i = 0; i < pFilter->windowSize; i++)
    {
        if (pFilter->data[i] < min)
            min = pFilter->data[i];
    }

    return min;
}
