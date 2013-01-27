/**
 * @file audio1ch.h
 * @brief Audio one channel output
 * @author Daniel Szot
 */

#ifndef _AUDIO_1CH_H
#define _AUDIO_1CH_H

#include "lib/common.h"
#include "decoders/decoder.h"
#include "hal/hld/audio.h"

#if defined(USE_ONE_CHANNEL_AUDIO)

#define SND_PLAY_TASK_PRIORITY DEFAULT_USER_TASK_PRIORITY

#define SND_ASYNC   0x00000000
#define SND_SYNC    0x00000001
#define SND_CLOSING 0x00000004

retcode audio1chPlaySound(const char *pFileName, UINT32 pFlags);
UINT32 audio1chGetSample(signed portBASE_TYPE *pHigherPriorityTaskWoken);
void audio1chStopSound();
void audio1chPause(BOOL pPause);
void audio1chSetCurrentSample(UINT32 pCurrentSample);
UINT32 audio1chGetCurrentSample();
UINT32 audio1chGetSamplesCount();

#else
#define audio1chGetSample(...) 0
#endif

#endif
