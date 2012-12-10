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
static UINT8 *fileBuffer;
static UINT32 fileBufSamples;

static UINT32 bufIdx = 0;
static UINT32 osamples;
static UINT32 isamples;
static UINT32 sampleRate;
static UINT8 bits;
static UINT8 channels;
static UINT8 cb;
static UINT8 playing = 0;
static xTaskHandle playingTask;
static xSemaphoreHandle audio1chMutex = NULL;

static struct hldAudioDevice *currentDevice = NULL;
static struct audioFile *currentAudioFile = NULL;

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

static void audioSoundProc(void *pvParameter)
{
    struct audioFile *audioFileHandle;
    retcode ret;
    UINT32 rd = 0;
    audioFileHandle = (struct audioFile *)pvParameter;
    static UINT32 lasttime = 0;
    while(1)
    {
        //while(1) vTaskDelay(100);

        //while (xSemaphoreTake(audio1chMutex, portMAX_DELAY) != pdTRUE);

        while(fileBufSamples > 0)
        {
            audio1chLoadBuff();
            vTaskDelay(1);
        }

        if (audioFileHandle->type == DECODER_WAVE)
        {
            if (fileBufSamples == 0)
            {
                lasttime = xTaskGetTickCount();
                //LOG(".");
                ret = audioWaveLoadBuffer((struct audioWaveFile *)audioFileHandle, fileBuffer, &rd);
                //LOG("r: %d", rd);
                if ((rd == 0) || (ret != SUCCESS))
                {
                    //LOG("Err: %d, %d", ret, rd);
                    vTaskDelete(playingTask);
                }
                fileBufSamples = rd;
                playing = 1;
                audio1chLoadBuff();
            }
        }
        else
        {
            // Unsupported ;(
            vTaskDelete(playingTask);
        }
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

retcode audio1chPlaySound(const char *pFileName, UINT32 pFlags)
{
    LED0 = 1;
    INT32 ret;
    FIL *f;
    struct audioFile *audioFileHandle;

    assert(pFileName != NULL);

    LOG("audio1chPlaySound");

    audio1chStopSound(NULL);

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
            if (ret == ERR_NOT_SUPPORTED)
                AUDIO_LOG("Not supported mode. Sorry :(, I can not support everything.");
            else
                AUDIO_LOG("Audio device not started");

            audio1chStopSound(audioFileHandle);
            return ret;
        }

        vSemaphoreCreateBinary(audio1chMutex);
        if (audio1chMutex == NULL)
        {
            AUDIO_LOG("Error while creating mutex");
            audio1chStopSound(audioFileHandle);
            return ERR_MUTEX;
        }

        // Alloc buffer memmory
        buffer[0] = pvPortMalloc(audioFileHandle->neededBufLen);
        buffer[1] = pvPortMalloc(audioFileHandle->neededBufLen);
        fileBuffer = pvPortMalloc(audioFileHandle->neededBufLen);

        LOG("fb: %p", fileBuffer);

        if ((buffer[0] == NULL) || (buffer[1] == NULL) || (fileBuffer == NULL))
        {
            AUDIO_LOG("Buffers allocation error");
            audio1chStopSound(audioFileHandle);
            return ERR_NO_MEMMORY;
        }
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
            1024, (void *)audioFileHandle, SND_PLAY_TASK_PRIORITY, &playingTask);

        if (ret != pdPASS)
        {
            AUDIO_LOG("Playing task creation error");
            audio1chStopSound(audioFileHandle);
            return ERR_NO_MEMMORY;
        }

        AUDIO_LOG("Playing task created");
    }

    currentAudioFile = audioFileHandle;
    return SUCCESS;
}

/**
 * Stop playing specyfied file
 * @param pAudioFile Audio file handle to stop play. Stop current if NULL
 */
void audio1chStopSound(struct audioFile *pAudioFile)
{
    struct audioFile *af = pAudioFile;
    if (af == NULL)
    {
        af = currentAudioFile;
        LOG("currentAudioFile");
    } else
        LOG("pAudioFile");

    if (af == NULL)
    {
        LOG("NULL total");
        return;
    }

    if (playingTask != NULL)
    {
        vTaskDelete(playingTask);
        playingTask = NULL;
    }

    if (audio1chMutex != NULL)
    {
        vSemaphoreDelete(audio1chMutex);
        audio1chMutex = NULL;
    }

    playing = 0;
    osamples = 0;
    isamples = 0;
    cb = 0;
    f_close(af->fileHandle);
    vPortFree(buffer[0]);
    vPortFree(buffer[1]);
    vPortFree(fileBuffer);
    vPortFree(af->fileHandle);
    vPortFree(af);
    af = NULL;
    pAudioFile = NULL;
    currentAudioFile = NULL;
}

UINT32 audio1chGetSample(signed portBASE_TYPE *pHigherPriorityTaskWoken)
{
    static UINT32 sample=0;

 /*   if (audio1chMutex != NULL)
    {
       if (xQueueIsQueueFullFromISR(audio1chMutex) == pdFALSE)
            xSemaphoreGiveFromISR(audio1chMutex, pHigherPriorityTaskWoken);
    }
            return 0;*/
    static UINT32 toggleFlag = 1;
    toggleFlag = (toggleFlag == 0) ? 1 : 0;
    //LED1 = toggleFlag;

    if ((isamples != 0) && (playing == 1))
    {
        LED1 = 0;
        // 4. load the new samples from the current buffer
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

        // 6. check if current buffer emptied
        if (isamples == 0)
        {
            // swap input/output buffers
            cb = 1 - cb;
            // restart counting input samples
            isamples = osamples;
            bufIdx = 0;
            osamples = 0;

            // Zmiana buforów bo odtwarzajacy sie skonczyl
            // mozna zaladowac dane do output z bufora pliku
            // ale nie będziemy tego robic tutaj
        }
    }
    else
    {
        //static UINT32 toggleFlag = 1;
        //toggleFlag = (toggleFlag == 0) ? 1 : 0;
        LED1 = 1;
        sample = 0;
    }

    //else if (playing == 0)
    //    sample = 0;

    return sample;
}

void audio1chLoadBuff()
{
    UINT32 i;

    

    if ((fileBufSamples > 0) && (osamples == 0))
    {
        LED0 = 0;
        // move all samples available into the current RAM buffer
        //memcpy( (void*)audioBuf[1-ACfg.cb][OCount],
        for(i = 0; fileBufSamples > 0; i++, fileBufSamples--)
        {
            buffer[1 - cb][osamples] = fileBuffer[i];
            osamples++;
        }

        // Cala zawartosc bufora pliku zostala zaladowana do bufora odtwarzania
        // mozna ju WYZWOLIC task ladujacy
     /*   if (audio1chMutex != NULL)
        {
           if (xQueueIsQueueFullFromISR(audio1chMutex) == pdFALSE)
                xSemaphoreGiveFromISR(audio1chMutex, pHigherPriorityTaskWoken);
        }  */

        if (isamples == 0)
        {
            LED0 = 0;
            // swap input/output buffers
            cb = 1 - cb;
            // restart counting input samples
            isamples = osamples;
            bufIdx = 0;
            osamples = 0;

            // Jezeli bufor odtwarzania byl wyczerpany to zaraz po zaladowaniu
            // sampli do bufora drugiego, zmieniamy bufor i rozpoczynamy odtwarzanie
        }

        LED0 = 1;
    }
    /*else
    {
        if (audio1chMutex != NULL)
        {
           if (xQueueIsQueueFullFromISR(audio1chMutex) == pdFALSE)
                xSemaphoreGiveFromISR(audio1chMutex, pHigherPriorityTaskWoken);
        }
    }*/

}

#endif
