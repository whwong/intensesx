/**
 * @file wm8731.c
 * @brief Driver for WM8731 codec specialised for PIC32 on SPI3A (SPI4) and I2C2
 * @author Daniel Szot
 */

#include "lib/common.h"
#include "task.h"
#include "semphr.h"
#include "hal/hld/hld.h"
#include "hal/hld/audio.h"
#include "lib/audio/audioSineOutput.h"

#include "wm8731.h"

static char lldWM8731Name[HLD_DEVICE_MAX_NAME_LEN] =
    "WM8731 Audio Codec";

void __attribute__( (interrupt(ipl2), vector(_SPI_4_VECTOR))) lldWm8731IntWrapper( void );

static xSemaphoreHandle audioMutex = NULL;
static UINT8 currentVolume = 0;

static retcode lldWm8731Init(struct hldAudioConfig *pCfg);
static void Wm8731DisableOutAmp(void);
static void Wm8731EnableOutAmp(void);
static void Wm8731Volume(UINT8 pLevel, UINT8 pZCEN);
static retcode WmMasterWrite(UINT8 pRegAdd, UINT16 pCmd);
static struct hldAudioDevice *currentDevice = NULL;

/**
 * @brief Attach WM8731 device
 *
 * @return Result value
 */
retcode lldWm8731Attach(struct hldAudioConfig *pCfg)
{
    struct hldAudioDevice *dev;

    assert(pCfg->sampleRate != 0)

    // Only one device could be opened - this is guaranteed by unique name
    // requirement
    dev = hldDeviceAlloc(lldWM8731Name, HLD_DEVICE_TYPE_AUDIO,
            sizeof(struct hldAudioDevice));

    if (dev == NULL)
        return ERR_NO_MEMMORY;

    dev->config = *pCfg;

    dev->attach =   lldWm8731Attach;
    dev->open =     lldWm8731Open;
    dev->close =    lldWm8731Close;
    dev->ioctl =    lldWm8731Ioctl;

    retcode result = hldDeviceRegister(dev);
    if (result != SUCCESS)
    {
        hldDeviceFree(dev);
    }
    else
    {
        dev->head.state |= HLD_DEVICE_STATE_TURNED_ON;
    }

    currentDevice = dev;

    return result;
}

static char *lldWm8731ModeToStr(enum hldAudioMode pMode)
{
    switch(pMode)
    {
        case AM_KMIXER:
            return "kmixer";
            break;
        case AM_NONE:
            return "none";
            break;
        case AM_SINE:
            return "sine";
            break;
        case AM_ONECHANNEL:
            return "onechannel";
            break;
    }
}

/**
 * @brief Open WM8731 device
 *
 * @param pAudioDev Pointer to device which will be opened
 * @return Result value
 */
static retcode lldWm8731Open(struct hldAudioDevice *pAudioDev)
{
    retcode ret;
    assert(pAudioDev != NULL);

    // Open there
    audioMutex = xSemaphoreCreateMutex();
    if (audioMutex == NULL)
    {
        MUTEX_ERROR();
        return ERR_MUTEX;
    }
    currentVolume = 0;

    if (ret = lldWm8731Init(&pAudioDev->config) != SUCCESS)
    {
        vSemaphoreDelete(audioMutex);
        ERROR("Bus error in %s device driver", lldWM8731Name);
        return ret;
    }

    // Open SPI and register interrupt
    SpiChnOpen( 4, SPI_CON_SLVEN|SPI_CON_CKP|SPI_CON_MODE32|
	                SPI_CON_ON|SPI_CON_FRMPOL|SPI_CON_FRMSYNC|
	                SPI_CON_FRMEN, configPERIPHERAL_CLOCK_HZ/20000000L);

    // Interrupts
    SpiChnClrTxIntFlag(4);

    INTSetVectorPriority(INT_VECTOR_SPI(4), pAudioDev->config.intPriority);
    INTSetVectorSubPriority(INT_VECTOR_SPI(4), INT_SUB_PRIORITY_LEVEL_1);
    INTEnable(INT_SOURCE_SPI_TX(4), INT_ENABLED);

    // force the 1st txfer
    SPI4BUF = 0;

    pAudioDev->head.state |= HLD_DEVICE_STATE_RUNNING;

    DONE("%s device opened:", lldWM8731Name);
    LOG("    Sample rate: %d", pAudioDev->config.sampleRate);
    LOG("    Channels: %d", pAudioDev->config.channels);
    LOG("    Bits: %d", pAudioDev->config.bits);
    LOG("    Mode: %s", lldWm8731ModeToStr(pAudioDev->config.mode));
    LOG("    Enable: %08x", pAudioDev->config.enable);

    return SUCCESS;
}

/**
 * @brief Close WM8731 device
 *
 * @param pAudioDev Pointer to device which channel will be closed
 * @return Result value
 */
static retcode lldWm8731Close(struct hldAudioDevice *pAudioDev)
{
    assert(pAudioDev != NULL);

    pAudioDev->head.state &= ~(HLD_DEVICE_STATE_RUNNING |
            HLD_DEVICE_STATE_TURNED_ON);

    vSemaphoreDelete(audioMutex);

    DONE("%s device closed", lldWM8731Name);
    return SUCCESS;
}

static retcode
lldWm8731Ioctl(struct hldAudioDevice *pAudioDev, UINT32 pCmd, UINT32 pParam)
{
    switch(pCmd)
    {
        case AC_SET_VOLUME:
            currentVolume = (UINT8)pParam;
            Wm8731Volume(currentVolume, 0);
            break;

        case AC_SET_VOLUME_SOFT:
            currentVolume = (UINT8)pParam;
            Wm8731Volume(currentVolume, 1);
            break;

        case AC_GET_VOLUME:
            *(UINT8*)pParam = currentVolume;
            break;

        case AC_DISABLE:
            if (pParam == AUDIO_OUTAMP)
            {
                Wm8731DisableOutAmp();
            }
            break;

        case AC_ENABLE:
            if (pParam == AUDIO_OUTAMP)
            {
                Wm8731EnableOutAmp();
            }
            break;

        case AC_SET_SAMPLE:
            return lldWm8731SetSampleRate(&pAudioDev->config, pParam);
            break;

        case AC_SET_BITS:
            return lldWm8731SetBits(&pAudioDev->config, pParam);
            break;

        case AC_SET_CHANNELS:
            if (pParam != 2)
                return ERR_NOT_SUPPORTED;
            else
                return SUCCESS;
            break;

        default:
            DEBUG("Unknown IOCTL command for %s device", lldWM8731Name);
    }
    return SUCCESS;
}

/**
 * @brief SPI device interrupt handler
 */

void lldWm8731IntHandler()
{
    // Zadeklarowane jako static aby zminimalizować użycie stosu
    static portBASE_TYPE higherPriorityTaskWoken;
    higherPriorityTaskWoken = FALSE;
    static UINT32 i;

    // 1. clear interrupt flag
    SpiChnClrTxIntFlag(4);
    i = SPI4BUF;

    switch(currentDevice->config.mode)
    {
        case AM_NONE:
            SPI4BUF = 0;
            break;
        case AM_KMIXER:
            break;
        case AM_ONECHANNEL:
            //SPI4BUF = audio1chGetSample();
            break;
        case AM_SINE:
            SPI4BUF = audioSineGetSample();
            break;
    }
    
    // If sending or receiving necessitates a context switch, then switch now.
    portEND_SWITCHING_ISR( higherPriorityTaskWoken );
}

static retcode lldWm8731SetEnabledBlocks(UINT32 pEnabledFlags)
{
    UINT8 tmpVal = 0x00;

    if ((pEnabledFlags & AUDIO_LINEIN) == 0)
        tmpVal |= 0x05; // ADCPD & MICPD
    if ((pEnabledFlags & AUDIO_MICIN) == 0)
        tmpVal |= 0x06; // ADCPD & LINEINPD
    if ((pEnabledFlags & AUDIO_LINEOUT) == 0)
        tmpVal |= 0x18; // DACPD & OUTPD

    return WmMasterWrite(WM8731_REG_PDOWN_CTRL, (UINT16)tmpVal);
}

static retcode lldWm8731SetBits(struct hldAudioConfig *pCfg, UINT32 pBits)
{
    // DSP_MODE|LRP|MS
    UINT8 tmpVal = 0x53;
    pCfg->bits = pBits;
    switch(pBits)
    {
        case 16:
            break;
        case 20:
            tmpVal |= 0x40;
            break;
        case 24:
            tmpVal |= 0x80;
            break;
        case 32:
            tmpVal |= 0xC0;
            break;

        default:
            return ERR_NOT_SUPPORTED;
    }

    return WmMasterWrite(WM8731_REG_DIGITAL_IF, tmpVal);
}

static retcode lldWm8731SetSampleRate(struct hldAudioConfig *pCfg, UINT32 pSampleRate)
{
    // Normal mode|!BOSR
    UINT8 tmpVal = 0x02;
    pCfg->sampleRate = pSampleRate;
    
    switch (pSampleRate)
    {
        case 8000:
            tmpVal |= (0x03 << 2);
            break;
        case 32000:
            tmpVal |= (0x06 << 2);
            break;
        case 48000:
            // Leave 00 at bits 2:5
            break;
        case 96000:
            tmpVal |= (0x07 << 2);
            break;
        default:
            return ERR_NOT_SUPPORTED;
    }

    return WmMasterWrite(WM8731_REG_SAMPLING_CTRL, tmpVal);
}

// Low level wm8731 interface
static retcode lldWm8731Init(struct hldAudioConfig *pCfg)
{
    retcode fail = SUCCESS;
    retcode sup = SUCCESS;

    CloseI2C2();
    OpenI2C2(I2C_EN | I2C_IDLE_CON | I2C_7BIT_ADD | I2C_STR_EN,
            (configPERIPHERAL_CLOCK_HZ / (2 * WM8731_I2C_CLOCK) - 2));
    // Wait to complete
    IdleI2C2();
    vTaskDelay(1);

    fail |= WmMasterWrite(WM8731_REG_RESET, 0x00); // reset device

    // Mute line in (left and right) at this moment we do not support line in
    // LRINBOTH|LINMUTE|(-34.5dB); use just the left input channel
    fail |= WmMasterWrite(WM8731_REG_LLINE_IN, 0x180);

    // Set default value for volume (mute) for both left and right channls
    // LRHPBOTH|!LZCEN|MUTE; use the left headphone commands
    fail |= WmMasterWrite(WM8731_REG_LHPHONE_OUT, 0x12F);

    // (SIDEATT=-15dB)|!SIDETONE|DACSEL|!BYPASS;
    fail |= WmMasterWrite(WM8731_REG_ANALOG_PATH, 0xd0);

     // !DACMU|(DEEMP=48KHz);
    fail |= WmMasterWrite(WM8731_REG_DIGITAL_PATH, 0x06);

    fail |= lldWm8731SetEnabledBlocks(pCfg->enable);
    
    sup = lldWm8731SetBits(pCfg, pCfg->bits);
    if (sup == ERR_NOT_SUPPORTED)
    {
        pCfg->bits = 16;
        fail |= lldWm8731SetBits(pCfg, pCfg->sampleRate);
        WARNING("Selected data format not supported. "
                    "Driver data format set to 16 bits");
    }
    else
        fail |= sup;
    
    sup = lldWm8731SetSampleRate(pCfg, pCfg->sampleRate);
    if (sup == ERR_NOT_SUPPORTED)
    {
        pCfg->sampleRate = 16;
        fail |= lldWm8731SetSampleRate(pCfg, pCfg->sampleRate);
        WARNING("Selected sample rate not supported. "
                    "Driver sample rate set to 48000Hz");
    }
    else
        fail |= sup;

    if (pCfg->channels != 2)
    {
        pCfg->channels = 2;
        WARNING("Selected channels number is not supported. "
                    "Driver channels set to 2");
    }

    return fail;
}

static void Wm8731DisableOutAmp(void)
{
    // ACTIVE; turn off
    WmMasterWrite(WM8731_REG_ACTIVE_CTRL, 0x00);
    LOG("ioctl: wm8731 OUT_AMP disabled");
}

static void Wm8731EnableOutAmp(void)
{
    // ACTIVE; turn on
    WmMasterWrite(WM8731_REG_ACTIVE_CTRL, 0x01);
    LOG("ioctl: wm8731 OUT_AMP enabled");
}

/**
 * Change device volume for both 2 channels
 * @param pLevel Volume level from 0 to 100
 */
static void Wm8731Volume(UINT8 pLevel, UINT8 pZCEN)
{
    UINT16 tmpVal = (pZCEN != 0) ? 0x1AF : 0x12F;
    // LRHPBOTH|LZCEN| 00-2F = Mute, 7F max
    WmMasterWrite(WM8731_REG_LHPHONE_OUT, tmpVal + (pLevel*80/100));
    LOG("ioctl: wm8731 OUT_AMP volume changed to: %d, soft: %d", pLevel, pZCEN);
}

/**
 * Write register to WM8731
 * @param pRegAdd Register address
 * @param pCmd Data to write
 * @return Return code
 * @retval SUCCESS If everything goes right
 * @retval ERR_BUS When bus error occured
 */
static retcode WmMasterWrite(UINT8 pRegAdd, UINT16 pCmd)
{
    UINT8 buff[4]; // store the words here
    UINT16 wmWord;
    retcode fail = SUCCESS;

    while (xSemaphoreTake(audioMutex, portMAX_DELAY) != pdTRUE);
    // reg address is 7 bits, command is 9 bits
    wmWord = ((UINT16) pRegAdd << 9) | (pCmd & 0x1ff);
    // issue a write command with WM8731 address
    buff[0] = (WM8731_ADDRESS << 1) | 0;
    buff[1] = (UINT8) (wmWord >> 8);
    buff[2] = (UINT8) (wmWord);

    // now transmit
    {
        int ix;

        StartI2C2(); // Send the Start Bit
        IdleI2C2(); // Wait to complete

        for (ix = 0; ix < 3; ix++)
        {
            if (MasterWriteI2C2(buff[ix]) != 0)
            {
                fail = ERR_BUS; // failed
                break;
            }
            IdleI2C2(); // Wait to complete

            if (I2C2STATbits.BCL)
                I2C2STATbits.BCL = 0;
            //while(1);
        }


        StopI2C2(); // Send the Stop condition
        IdleI2C2(); // Wait to complete
    }
    xSemaphoreGive(audioMutex);

    return fail;
}
