/**
 * @file wave.h
 * @brief Wave "decoder" support
 * @author Daniel Szot
 */

#ifndef _AUDIO_DECODER_WAVE_H
#define _AUDIO_DECODER_WAVE_H

#include "lib/common.h"
#include "decoder.h"
#include "lib/fatfs/ff.h"

#define WAVE_NEEDED_BUF_LEN 512

struct audioWaveFile
{
    struct audioFile head;
    UINT8 format;
};

// Data format in file
#define  RIFF_DWORD  0x46464952UL
#define  WAVE_DWORD  0x45564157UL
#define  DATA_DWORD  0x61746164UL
#define  FMT_DWORD   0x20746d66UL
#define  WAV_DWORD   0x00564157UL

// Chunk ID struct
struct audioWaveChunk
{
    // Contains the letters "RIFF" in ASCII form
    UINT32 chunkID;
    
    // 36 + SubChunk2Size, or more precisely:
    // 4 + (8 + SubChunk1Size) + (8 + SubChunk2Size)
    // This is the size of the rest of the chunk
    // following this number.  This is the size of the
    // entire file in bytes minus 8 bytes for the
    // two fields not included in this count:
    // ChunkID and ChunkSize.
    UINT32 chunkSize;

    // Contains the letters "WAVE"
    UINT32 format;
};

// Subchunk ID struct
struct audioWaveSubchunk
{
    // Contains the letters "fmt " or "data"
    UINT32 subchunkID;
    // 16 for PCM.  This is the size of the
    // rest of the Subchunk which follows this number.
    UINT32 subchunkSize;
};

// Format Chunk struct
struct audioWaveChunkFormat
{
    // PCM = 1 (i.e. Linear quantization)
    // Values other than 1 indicate some
    // form of compression.
    UINT16 audioFormat;

    // Mono = 1, Stereo = 2, etc.
    UINT16 numChannels;

    // 8000, 44100, etc.
    UINT32 sampleRate;

    // SampleRate * NumChannels * BitsPerSample/8
    UINT32 byteRate;

    // NumChannels * BitsPerSample/8
    // The number of bytes for one sample including
    // all channels.
    UINT16 blockAlign;

    // 8 bits = 8, 16 bits = 16, etc.
    UINT16 bitsPerSample;

    // Support only PCM so this is the end
};

struct audioWaveFile *audioWaveReadHeaders(FIL *pFile);

retcode
audioWaveLoadBuffer(struct audioWaveFile *pWaveFile, UINT8 *pBuf, UINT32 *pLoaded);

#endif
