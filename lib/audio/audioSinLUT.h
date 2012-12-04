/**
 * @file audioSinLUT.h
 * @brief Sinus Look Up Table with 512 entries
 * @author Daniel Szot
 */

#ifndef _AUDIO_SIN_LUT_H
#define _AUDIO_SIN_LUT_H

#include "lib/common.h"

#define AUDIO_SIN_LUT_SIZE 512

extern const INT16 gAudioSinLut[];

/**
 * Get sin value from look up table
 * @param pTheta Range is [0, 0xFFFF] for [0,2pi)
 * @return Sine value
 */
static inline INT32 audioSinLut(INT32 pTheta)
{
    return gAudioSinLut[(pTheta >> 7) & 0x1FF];
}

/**
 * Get cos value from look up table
 * @param pTheta Range is [0, 0xFFFF] for [0,2pi)
 * @return Cos value
 */
static inline INT32 audioCosLut(INT32 pTheta)
{
    return gAudioSinLut[((pTheta >> 7) + 128) & 0x1FF];
}

#endif
