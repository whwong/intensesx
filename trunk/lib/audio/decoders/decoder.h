/**
 * @file decoder.h
 * @brief Common header part for all decoders
 * @author Daniel Szot
 */

#ifndef _AUDIO_DECODER_H
#define _AUDIO_DECODER_H

#include "lib/common.h"

#define DECODER_WAVE 0x01
#define DECODER_FLAC 0x02

struct audioFile
{
    FIL *fileHandle;
    UINT32 type;
    UINT32 sampleRate;
    UINT16 neededBufLen;
    UINT8 channels;
    UINT8 bits;
};

#endif
