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

retcode audio1chPlaySound(const char *pFileName, UINT32 pFlags);
UINT32 audio1chGetSample(signed portBASE_TYPE *pHigherPriorityTaskWoken);
void audio1chLoadBuff();
void audio1chStopSound(struct audioFile *pAudioFile);

#else
#define audio1chGetSample(...) 0
#endif

#endif
