/**
 * @file digitalFilter.h
 * @brief Linear digital filter implementation
 * @author Daniel Szot
 */

#ifndef _DIGITAL_FILTER_H
#define _DIGITAL_FILTER_H

struct digitalFilter
{
    INT32 *data;
    UINT32 windowSize;
    UINT32 bufferPosition;
    UINT32 samplesCount;
};

struct digitalFilter *filterCreate(UINT32 windowSize);
void filterDelete(struct digitalFilter *pFilter);

void filterAddSample(struct digitalFilter *pFilter, INT32 pValue);
BOOL filterIsReady(struct digitalFilter *pFilter);
INT32 filterGetValue(struct digitalFilter *pFilter);
void filterResetSamples(struct digitalFilter *pFilter);

INT32 filterGetRejectMinMax(struct digitalFilter *pFilter);
INT32 filterGetMax(struct digitalFilter *pFilter);
INT32 filterGetMin(struct digitalFilter *pFilter);

#endif
