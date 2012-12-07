/**
 * @file audio1ch.h
 * @brief Audio one channel output
 * @author Daniel Szot
 */

#include "audio1ch.h"
#include "lib/fatfs/ff.h"
#include "decoders/decoder.h"
#include "decoders/wave.h"

#if defined(USE_ONE_CHANNEL_AUDIO)

#define AUDIO_DEBUG
#if defined(AUDIO_DEBUG)
#define AUDIO_LOG(frm, ...) LOG("audio: "frm, ##__VA_ARGS__)
#else
#define AUDIO_LOG(...)
#endif

// Dobule buffer
static UINT8 *buffer[2];

static UINT32 osamples;
static UINT32 isamples;
static UINT32 sampleRate;
static UINT8 bits;
static UINT8 channels;
static UINT8 cb;
static xTaskHandle playingTask;

static struct hldAudioDevice *currentDevice = NULL;

static BOOL checkExtension(char *pFn, char *pExt)
{
    UINT32 i, dotpos;

    // Find last dot
    for (i = strlen(pFn); i > 0; i--)
    {
        if (pFn[i - 1] == '.')
        {
            dotpos = i;
            break;
        }
    }

    // Check ext
    if (strcmp(pFn+dotpos, pExt) == 0)
        return TRUE;
    else
        return FALSE;
}

static void audioSoundProc(void *pvParameter)
{
    while(1)
    {
        vTaskDelay(100);
    }
}

static retcode audio1chConfig(struct audioFile *pAudioFileHandle)
{
    retcode ret = SUCCESS;
    //adjust device driver
    sampleRate = pAudioFileHandle->sampleRate;
    channels = pAudioFileHandle->channels;
    bits = pAudioFileHandle->bits;

    struct hldAudioDevice *devNode;
    devNode = (struct hldAudioDevice*)hldDeviceGetFirst();

    // Loop by all devices
    while (devNode != NULL)
    {
        // But we are interested only about AUDIO devices
        if (devNode->head.type == HLD_DEVICE_TYPE_AUDIO)
        {
            ret = devNode->ioctl(devNode, AC_SET_SAMPLE, sampleRate);
            if (ret != SUCCESS) return ret;
            devNode->ioctl(devNode, AC_SET_CHANNELS, channels);
            if (ret != SUCCESS) return ret;
            devNode->ioctl(devNode, AC_SET_BITS, bits);
            if (ret != SUCCESS) return ret;
        }
        devNode = (struct hldAudioDevice*)devNode->head.next;
    }

    return SUCCESS;
}

retcode audio1chPlaySound(char *pFileName, UINT32 pFlags)
{
    INT32 ret;
    FIL *f;
    struct audioFile *audioFileHandle;

    assert(pFileName != NULL);

    // Alloc FatFS file handle
    f = pvPortMalloc(sizeof(FIL));
    if (f == NULL)
        return ERR_NO_MEMMORY;

    // Open FatFS file
    ret = f_open(f, pFileName, FA_OPEN_EXISTING | FA_READ);
    if (ret != FR_OK)
    {
        AUDIO_LOG("File open error");
        return ERR_FILE;
    }

    // Check extensions
    if (checkExtension(pFileName, "wav"))
    {
        AUDIO_LOG("WAVE file. Parsing header...");
        audioFileHandle = (struct audioFile*)audioWaveReadHeaders(f);

        if (audioFileHandle == NULL)
        {
            AUDIO_LOG("Error while parsing header");
            f_close(f);
            vPortFree(f);
            return ERR_FILE;
        }

        AUDIO_LOG("WAVE Header parsed");
        AUDIO_LOG("Configurating devices...");

        ret = audio1chConfig(audioFileHandle);
        if (ret != SUCCESS)
        {
            AUDIO_LOG("Not supported mode. Sorry :(, I can not support everything.");
            audio1chStopSound(audioFileHandle);
            return ret;
        }

        //ok
    }
    else if (checkExtension(pFileName, "flac"))
    {
        AUDIO_LOG("FLAC files will be supported ASAP");
        f_close(f);
        return ERR_NOT_SUPPORTED;
    }

    // Check playing mode
    if (pFlags & SND_SYNC)
    {
        // Play sync start playing loop
        // TODO: do this loop :)
        AUDIO_LOG("SND_SYNC Selected");
        audio1chStopSound(audioFileHandle);
        return ERR_NOT_SUPPORTED;
    }
    else
    {
        // Play async - create task for playing
        AUDIO_LOG("SND_ASYNC Selected");
        ret = xTaskCreate( audioSoundProc, ( const signed char * const ) "1ch",
            1024, NULL, SND_PLAY_TASK_PRIORITY, &playingTask);

        if (ret != pdPASS)
        {
            AUDIO_LOG("Playing task creation error");
            audio1chStopSound(audioFileHandle);
            return ERR_NO_MEMMORY;
        }

        AUDIO_LOG("Playing task created");
    }

    return SUCCESS;
}

void audio1chStopSound(struct audioFile *pAudioFile)
{
    if (playingTask != NULL)
    {
        vTaskDelete(playingTask);
        playingTask = NULL;
    }
    
    osamples = 0;
    isamples = 0;
    cb = 0;
    f_close(pAudioFile->fileHandle);
    vPortFree(pAudioFile->fileHandle);
    vPortFree(pAudioFile);
}

UINT32 audio1chGetSample()
{
    return 0;
}

#endif
