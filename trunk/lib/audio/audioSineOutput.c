/**
 * @file audioSineOutput.c
 * @brief One of the sound output modules. It could output sine wave at any freq
 * @note Default frequency after init is 100Hz. It is able to change by
 * function audioSineSetFreq(UINT32);
 * @author Daniel Szot
 */

#include "audioSineOutput.h"

static struct hldAudioConfig audioSineCfg;
static UINT8 audioSineReady = 0;
static double audioSineSampleDiv = 0.0f;
static UINT32 audioSineFreq = 100;

void audioSineSetFreq(UINT32 pFreq)
{
    audioSineFreq = pFreq;
    audioSineSampleDiv = (double)(audioSineFreq*AUDIO_SIN_LUT_SIZE)/
            (double)audioSineCfg.sampleRate;

    DONE("Audio Sine Output set at %dHz", audioSineFreq);
}

static void inline audioSineConsoleUsage()
{
    LOG("\tusage: 'asetsinf'");
    LOG("\tType 'asetsinf 1000' to set output freq at 1000Hz");
}

static void audioSineSetFreqConsole(UINT8 argc, const char *argv[])
{
    if (argc == 2 && (strcmp(argv[1], "-h") == 0))
    {
        LOG("\tThis command sets audio line out sine wave frequency");
        audioSineConsoleUsage();
    }
    else if (argc == 2)
    {
        UINT32 freq = atoi(argv[1]);
        audioSineSetFreq(freq);
    }
    else
    {
        audioSineConsoleUsage();
    }
}

retcode audioSineConfig(struct hldAudioConfig *pCfg)
{
    if (pCfg->bits != 16)
    {
        ERROR("Audio Sine Output supports only 16 bits audio devices");
        return ERR;
    }

    if (pCfg->channels != 2)
    {
        ERROR("Audio Sine Output supports only stereo devices");
        return ERR;
    }

    if (pCfg->mode != AM_SINE)
    {
        ERROR("This device is not configured to use sine output");
        return ERR;
    }

    if (audioSineReady != 1)
        consoleRegisterCmd("asetsinf", &audioSineSetFreqConsole);

    audioSineCfg = *pCfg;
    audioSineReady = 1;
    audioSineSetFreq(100);

    return SUCCESS;
}

INT32 audioSineGetSample()
{
    static UINT32 time = 0;
    static double count = 0;
    static union audioSineSample s;
    if (audioSineReady != 1)
        return 0;
    
    count += audioSineSampleDiv;
    if (count >= 1)
    {
        s.left = audioSinLut(time*128);
        s.right = s.left;
        
        time += (UINT32)count;
        if (time > 512)
            time = 0;
        count = 0;
    }

    return s.sample;
}
