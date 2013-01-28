/**
 * @file wave.h
 * @brief Wave "decoder" support
 * @author Daniel Szot
 */

#include "wave.h"

#define WAVE_DEBUG
#if defined(WAVE_DEBUG)
#define WAVE_LOG(frm, ...) LOG("wave: "frm, ##__VA_ARGS__)
#else
#define WAVE_LOG(...)
#endif

/**
 * Reads header from opened wave file
 * @param pFile File handle. File need to be previously opened by f_open
 * @return Pointer to readed header
 */
struct audioWaveFile *audioWaveReadHeaders(FIL *pFile)
{
    UINT32 rd;
    INT32 fr;
    struct audioWaveFile *af;
    struct audioWaveChunk chunkHead;
    struct audioWaveSubchunk subchunkHead;
    struct audioWaveChunkFormat formatChunk;

    // Load file header
    fr = f_read(pFile, &chunkHead, sizeof(struct audioWaveChunk), (UINT*)&rd);
    if ((fr != FR_OK) || (rd != sizeof(struct audioWaveChunk)))
        return NULL;

    // Check that this is supported
    if ((chunkHead.chunkID != RIFF_DWORD) || (chunkHead.format != WAVE_DWORD))
    {
        WAVE_LOG("Unsupported WAVE file");
        return NULL;
    }

    // Seek for format chunk
    fr = f_read(pFile, &subchunkHead, sizeof(struct audioWaveSubchunk), (UINT*)&rd);
    if ((fr != FR_OK) || (rd != sizeof(struct audioWaveSubchunk)))
        return NULL;

    if (subchunkHead.subchunkID != FMT_DWORD)
    {
        WAVE_LOG("Can't find format chunk");
        return NULL;
    }

    // Load format chunk
    fr = f_read(pFile, &formatChunk, sizeof(struct audioWaveChunkFormat), (UINT*)&rd);
    if ((fr != FR_OK) || (rd != sizeof(struct audioWaveChunkFormat)))
        return NULL;

    // Find data chunk
    subchunkHead.subchunkSize = 0;
    do
    {
        fr = f_lseek(pFile, f_tell(pFile) + subchunkHead.subchunkSize);
        if (fr != FR_OK)
            return NULL;

        fr = f_read(pFile, &subchunkHead, sizeof(struct audioWaveSubchunk), (UINT*)&rd);
        if ((fr != FR_OK) || (rd != sizeof(struct audioWaveSubchunk)))
        {
            WAVE_LOG("Can't find data chunk or data chunk read error");
            return NULL;
        }
        LOG("size: %d, data: 0x%x", subchunkHead.subchunkSize, subchunkHead.subchunkID);
    } while(subchunkHead.subchunkID != DATA_DWORD);

    af = pvPortMalloc(sizeof(struct audioWaveFile));
    af->head.fileHandle = pFile;
    af->head.type = DECODER_WAVE;

    af->head.sampleRate = formatChunk.sampleRate;
    af->head.channels = formatChunk.numChannels;
    af->head.bits = formatChunk.bitsPerSample;
    af->head.neededBufLen = WAVE_NEEDED_BUF_LEN;


    af->dataStartIndex = f_tell(pFile);
    af->samplesCount = subchunkHead.subchunkSize / formatChunk.numChannels /
            (formatChunk.bitsPerSample / 8);

    LOG("wave start: %d, all %d", af->dataStartIndex, af->samplesCount);

    return af;
}

/**
 * Reads next buffer from file. Reads neededBufLen bytes from file.
 * @param pReadedHeader Wave header struct from which we will be reading
 * @param pBuf Pointer to allocated buffer
 * @return Result code
 */
retcode
audioWaveLoadBuffer(struct audioWaveFile *pWaveFile, UINT8 *pBuf, UINT32 *pLoaded)
{
    INT32 fr;
    UINT rd;

    //LOG("%d, %d", pWaveFile->head.neededBufLen, *pLoaded);
    fr = f_read(pWaveFile->head.fileHandle, pBuf, pWaveFile->head.neededBufLen, &rd);
    if ((fr != FR_OK))// || (rd != sizeof(struct audioWaveChunkFormat)))
        return ERR_FILE;

    *pLoaded = rd;
    //LOG("OK %d, %d", pWaveFile->head.neededBufLen, *pLoaded);
    return SUCCESS;
}

UINT32 audioWaveGetSamplesCount(struct audioWaveFile *pWaveFile)
{
    return pWaveFile->samplesCount;
}

UINT32 audioWaveGetCurrentSample(struct audioWaveFile *pWaveFile)
{
    UINT32 curadr, progress;
    curadr = f_tell(pWaveFile->head.fileHandle);
    curadr -= pWaveFile->dataStartIndex;
    progress = curadr / pWaveFile->head.channels / (pWaveFile->head.bits / 8);
    return progress;
}

void audioWaveSetCurrentSample(struct audioWaveFile *pWaveFile, UINT32 pCurrentSample)
{
    UINT32 curadr = pWaveFile->dataStartIndex;
    curadr += pCurrentSample * pWaveFile->head.channels * (pWaveFile->head.bits / 8);

    f_lseek(pWaveFile->head.fileHandle, curadr);
}
