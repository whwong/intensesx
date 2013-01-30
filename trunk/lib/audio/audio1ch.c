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

// Dobule audio buffer
// Input buffer - always buffer[cb] - playing buffer
// Output bufer - always buffer[cb-1] - loading buffer
static UINT8 *buffer[2];
// Current index in current buffer
static UINT32 bufIdx = 0;
// Number of samples (bytes) in output buffer
static UINT32 osamples = 0;
// Number of samples (bytes) in input buffer
static UINT32 isamples = 0;
// Currently playing audio file sample rate
static UINT32 sampleRate = 0;
// Currently playing audio file bits per one sample on one channel
static UINT8 bits = 0;
// Currently playing audio file number of channels
static UINT8 channels = 0;
// Current buffer index (1 or 0 possible)
static UINT8 cb = 0;
// Is sound is playing (1) or paused (0)
static UINT8 playing = 0;
// Handle to the playing task
static xTaskHandle playingTask = NULL;
// Semaphore to synchronize data loading from file to output buffer
static xSemaphoreHandle audio1chSem = NULL;
// Mutex to mutualy exclisive audio playing and stopping routines
static xSemaphoreHandle audio1chPlayMutex = NULL;
// Device used for playing
static struct hldAudioDevice *currentDevice = NULL;
// Currently playing audio file handle
static struct audioFile *currentAudioFile = NULL;
// Playing end callback
// It is executed after play stop only when sound was previously successfully
// played
static void (*audioPlayingEndCallback)();
// Ending flag. Any task can set it to 1, then playing task will end end set it
// back to 0
// static UINT8 endingFlag;
// 1 If playing task is running, 0 if not
// static UINT8 playingTaskRunning = 0;
// Currently playing sound flsgs
static UINT32 currentAudioFlags = 0;

void audioSetPlayingEndCallback(void (*pCB)())
{
    audioPlayingEndCallback = pCB;
}

/**
 * Checks that specified filename extension is equal to pExt
 * @param pFn File name to compare
 * @param pExt Extension to compare
 * @return Boolean value like:
 * @retval TRUE - extesions is equal
 * @retval FALSE - extension is different
 */
static BOOL checkExtension(const char *pFn, const char *pExt)
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

/**
 * Playing task function
 * @param pvParameter Task parameter in this case this is always equal
 * to currentAudioFile
 */
static void audioSoundProc(void *pvParameter)
{
    retcode ret;
    UINT32 rd = 0;

    while(1)
    {
        if ((audio1chSem == NULL) || (audio1chPlayMutex == NULL))
        {
            audio1chStopSound();
            break;
        }

        xSemaphoreTake(audio1chSem, portMAX_DELAY);

        if (currentAudioFlags & SND_CLOSING)
        {
            playing = 0;
            audio1chStopSound();
            break;
        }

        if (currentAudioFile->type == DECODER_WAVE)
        {
            if (osamples == 0)
            {
                ret = audioWaveLoadBuffer((struct audioWaveFile *)currentAudioFile,
                        buffer[1-cb], &rd);

                if ((rd == 0) || (ret != SUCCESS))
                {
                    playing = 0;
                    audio1chStopSound();
                    break;
                }
                osamples = rd;
                playing = 1;
            }
        }
        else
        {
            // Unsupported ;(
            playing = 0;
            audio1chStopSound();
            break;
        }
    }
    
    audioPlayingEndCallback();

    // To be absolutelly sure that we never get out this function when it is
    // task routine
    if ((playingTask != NULL) && ((currentAudioFlags & SND_SYNC) == 0))
    {
        playingTask = NULL;
        vTaskDelete(NULL);
    }
}

/**
 * Configure audio devices to play given sound file
 * @param pAudioFileHandle Sound file
 * @return Result code
 * @retval ERR_NOT_SUPPORTED - not supported format
 * @retval SUCCESS - successfully configured
 */
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

/**
 * Play file by specified filename
 * @param pFileName Specify filename of sound to play. Decoder is selected
 * based on file extension.
 * @param pFlags Playing flags:
 * SND_SYNC - Synchronous play. Wait until sound will end.
 * SND_ASYNC - Asynchronous play. Play sound in another task.
 * @return Result code
 */
retcode audio1chPlaySound(const char *pFileName, UINT32 pFlags)
{
    INT32 ret;
    FIL *f;

    assert(pFileName != NULL);

    if (audio1chPlayMutex == NULL)
        audio1chPlayMutex = xSemaphoreCreateMutex();

    if (audio1chPlayMutex == NULL)
    {
        AUDIO_LOG("Error while creating playing mutex");
        audio1chStopSound();
        return ERR_MUTEX;
    }

    audio1chStopSound();
    currentAudioFlags = pFlags;

    xSemaphoreTake(audio1chPlayMutex, portMAX_DELAY);

    // Alloc FatFS file handle
    f = pvPortMalloc(sizeof(FIL));
    if (f == NULL)
    {
        xSemaphoreGive(audio1chPlayMutex);
        return ERR_NO_MEMMORY;
    }

    // Open FatFS file
    ret = f_open(f, pFileName, FA_OPEN_EXISTING | FA_READ);
    if (ret != FR_OK)
    {
        AUDIO_LOG("File open error #%d", ret);
        vPortFree(f);
        xSemaphoreGive(audio1chPlayMutex);
        return ERR_FILE;
    }

    // Check extensions
    if (checkExtension(pFileName, "wav") || checkExtension(pFileName, "WAV"))
    {
        AUDIO_LOG("WAVE file. Parsing header...");
        currentAudioFile = (struct audioFile*)audioWaveReadHeaders(f);

        if (currentAudioFile == NULL)
        {
            AUDIO_LOG("Error while parsing header");
            f_close(f);
            vPortFree(f);
            xSemaphoreGive(audio1chPlayMutex);
            return ERR_FILE;
        }

        AUDIO_LOG("WAVE Header parsed");
        AUDIO_LOG("Configurating devices...");

        ret = audio1chConfig(currentAudioFile);
        if (ret != SUCCESS)
        {
            if (ret == ERR_NOT_SUPPORTED)
                AUDIO_LOG("Not supported mode. Sorry :(, I can not support everything.");
            else
                AUDIO_LOG("Audio device not started");

            f_close(f);
            vPortFree(f);
            vPortFree(currentAudioFile);
            currentAudioFile = NULL;
            xSemaphoreGive(audio1chPlayMutex);
            return ret;
        }

        vSemaphoreCreateBinary(audio1chSem);
        if (audio1chSem == NULL)
        {
            AUDIO_LOG("Error while creating loading semaphore");
            xSemaphoreGive(audio1chPlayMutex);
            audio1chStopSound();
            return ERR_MUTEX;
        }

        // Alloc buffer memmory
        buffer[0] = pvPortMalloc(currentAudioFile->neededBufLen);
        buffer[1] = pvPortMalloc(currentAudioFile->neededBufLen);

        if ((buffer[0] == NULL) || (buffer[1] == NULL))
        {
            AUDIO_LOG("Buffers allocation error");
            xSemaphoreGive(audio1chPlayMutex);
            audio1chStopSound();
            return ERR_NO_MEMMORY;
        }
    }
    else if (checkExtension(pFileName, "flac"))
    {
        AUDIO_LOG("FLAC files will be supported ASAP");
        f_close(f);
        xSemaphoreGive(audio1chPlayMutex);
        return ERR_NOT_SUPPORTED;
    }
    else
    {
        AUDIO_LOG("Can not open %s file. Not supported extension", pFileName);
        f_close(f);
        xSemaphoreGive(audio1chPlayMutex);
        return ERR_NOT_SUPPORTED;
    }

    // Check playing mode
    if (pFlags & SND_SYNC)
    {
        xSemaphoreGive(audio1chPlayMutex);
        // Play sync start playing loop
        AUDIO_LOG("SND_SYNC Selected");
        audioSoundProc((void *)currentAudioFile);
        return SUCCESS;
    }
    else
    {
        // Play async - create task for playing
        AUDIO_LOG("SND_ASYNC Selected");
        ret = xTaskCreate( audioSoundProc, ( const signed char * const ) "1ch",
            2048, (void *)currentAudioFile, SND_PLAY_TASK_PRIORITY, &playingTask);

        if (ret != pdPASS)
        {
            AUDIO_LOG("Playing task creation error");
            audio1chStopSound();
            return ERR_NO_MEMMORY;
        }

        AUDIO_LOG("Playing task created");
    }

    xSemaphoreGive(audio1chPlayMutex);
    return SUCCESS;
}

/**
 * Stop currently playing sound
 */
void audio1chStopSound()
{
    if (audio1chPlayMutex == NULL)
        audio1chPlayMutex = xSemaphoreCreateMutex();

    if (audio1chPlayMutex == NULL)
    {
        AUDIO_LOG("Error while creating playing mutex");
        return;
    }

    xSemaphoreTake(audio1chPlayMutex, portMAX_DELAY);

    if (currentAudioFile != NULL)
    {
        currentAudioFlags |= SND_CLOSING;
        f_close(currentAudioFile->fileHandle);

        if (audio1chSem != NULL)
            xSemaphoreGive(audio1chSem);

        if ((playingTask != NULL) && (xTaskGetCurrentTaskHandle() != playingTask) &&
                ((currentAudioFlags & SND_SYNC) == 0))
        {
            AUDIO_LOG("Deleting playing task (%p) from another task...", playingTask);
            vTaskDelete(playingTask);
            playingTask = NULL;
        }

        AUDIO_LOG("Deleting semaphore...");
        if (audio1chSem != NULL)
        {
            vSemaphoreDelete(audio1chSem);
            audio1chSem = NULL;
        }

        AUDIO_LOG("Freeing memory...");
        playing = 0;
        osamples = 0;
        isamples = 0;
        cb = 0;
        vPortFree(buffer[0]);
        vPortFree(buffer[1]);
        vPortFree(currentAudioFile->fileHandle);
        vPortFree(currentAudioFile);
        currentAudioFile = NULL;
    }
    
    xSemaphoreGive(audio1chPlayMutex);
}

/**
 * Get one sample from playing buffer. One sample means one sample for one channel.
 * If file have more than one channel you will need to get another sample for each
 * channel. Execute only from ISR.
 * @param pHigherPriorityTaskWoken Standard ISR freeRTOS parameter
 * @return Sample in format specified by file format
 */
UINT32 audio1chGetSample(signed portBASE_TYPE *pHigherPriorityTaskWoken)
{
    static UINT32 sample = 0;

    if ((isamples != 0) && (playing == 1))
    {
        // Load new sample from the current buffer
        switch(bits)
        {
            case 8:
                sample = buffer[cb][bufIdx++];
                isamples--;
                break;
                
            case 16:
                sample = ((UINT16*)(buffer[cb]))[bufIdx++];
                isamples = isamples - 2;
                break;

            default:
                sample = ((UINT16*)(buffer[cb]))[bufIdx++];
                isamples = isamples - 2;
        }

        // Check if current buffer emptied
        if ((isamples == 0) && (osamples != 0))
        {
            // Swap input/output buffers
            cb = 1 - cb;
            // Restart counting input samples
            isamples = osamples;
            bufIdx = 0;
            osamples = 0;

            // Let's load new data to output buffer
            if (audio1chSem != NULL)
                xSemaphoreGiveFromISR(audio1chSem, pHigherPriorityTaskWoken);
        }
        else if (isamples == 0)
        {
            sample = 0;
        }
    }
    else if ((playing == 1) && (osamples != 0))
    {
        // swap input/output buffers
        cb = 1 - cb;
        // restart counting input samples
        isamples = osamples;
        bufIdx = 0;
        osamples = 0;

        // Let's load new data to output buffer
        if (audio1chSem != NULL)
            xSemaphoreGiveFromISR(audio1chSem, pHigherPriorityTaskWoken);
    }
    else
    {
        sample = 0;
    }

    return sample;
}

void audio1chPause(BOOL pPause)
{
    if (currentAudioFile != NULL)
        playing = 1-pPause;
}

void audio1chSetCurrentSample(UINT32 pCurrentSample)
{
    if (currentAudioFile != NULL)
    {
        if (currentAudioFile->type == DECODER_WAVE)
        {
            audioWaveSetCurrentSample((struct audioWaveFile *)currentAudioFile,
                    pCurrentSample);
        }
    }
}

UINT32 audio1chGetCurrentSample()
{
    if (currentAudioFile != NULL)
    {
        if (currentAudioFile->type == DECODER_WAVE)
        {
            return audioWaveGetCurrentSample((struct audioWaveFile *)currentAudioFile);
        }
        else
        {
            // Unsupported ;(
            return 0;
        }
    }
    else
        return 0;
}

UINT32 audio1chGetSamplesCount()
{
    if (currentAudioFile != NULL)
    {
        if (currentAudioFile->type == DECODER_WAVE)
        {
            return audioWaveGetSamplesCount((struct audioWaveFile *)currentAudioFile);
        }
        else
        {
            // Unsupported ;(
            return 0;
        }
    }
    else
        return 0;
}

UINT32 audio1chGetBits()
{
    if (currentAudioFile != NULL)
    {
        return currentAudioFile->bits;
    }
    else
        return 0;
}

UINT32 audio1chGetChannels()
{
    if (currentAudioFile != NULL)
    {
        return currentAudioFile->channels;
    }
    else
        return 0;
}

UINT32 audio1chGetSampleRate()
{
    if (currentAudioFile != NULL)
    {
        return currentAudioFile->sampleRate;
    }
    else
        return 0;
}

BOOL audio1chIsPlaying()
{
    return (BOOL)playing;
}

BOOL audio1chIsFileLoaded()
{
    if (currentAudioFile != NULL)
        return TRUE;
    else
        return FALSE;
}

#endif
