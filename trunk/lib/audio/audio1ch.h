/**
 * @file audio1ch.h
 * @brief Audio one channel output
 * @author Daniel Szot
 */

#ifndef _AUDIO_1CH_H
#define _AUDIO_1CH_H

#include "lib/common.h"
#include "hal/hld/audio.h"

#if defined(USE_ONE_CHANNEL_AUDIO)

#define AUDIO_1CH_BUFLEN 512

#else
#define audio1chGetSample(...) 0
#endif

#endif
