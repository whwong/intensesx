/**
 * @file audio1ch.h
 * @brief Audio one channel output
 * @author Daniel Szot
 */

#include "audio1ch.h"

#if defined(USE_ONE_CHANNEL_AUDIO)

// Dobule buffor 512 bytes, for stereo signed 16 bit wave it gives 128 samples
static UINT8 buffor[2][AUDIO_1CH_BUFLEN];

static UINT32 osamples;
static UINT32 isamples;
static UINT32 sampleRate;
static UINT8 bits;
static UINT8 stereo;
static UINT8 cb;

static hldAudioDevice *currentDevice = NULL;

void audio1chCreate();
void audio1chGetSample();
void audio1chWriteBuf();

#endif