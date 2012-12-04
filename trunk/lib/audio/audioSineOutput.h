/**
 * @file audioSineOutput.h
 * @brief One of the sound output modules. It could output sine wave at any freq
 * @note Default frequency after init is 100Hz. It is able to change by
 * function audioSineSetFreq(UINT32);
 * @author Daniel Szot
 */

#ifndef _AUDIO_SINE_OUTPUT_H
#define _AUDIO_SINE_OUTPUT_H

#include "audioSinLUT.h"
#include "hal/hld/audio.h"
#include "lib/console/serialConsole.h"

union audioSineSample
{
    INT32 sample;

    struct
    {
        INT16 left;
        INT16 right;
    };
};

void audioSineSetFreq(UINT32 pFreq);
retcode audioSineConfig(struct hldAudioConfig *pCfg);
INT32 audioSineGetSample();

#endif
