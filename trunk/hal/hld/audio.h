/**
 * @file audio.h
 * @brief DAC/ADC for audio High Level Driver
 * @author Daniel Szot
 */

#ifndef _HLD_AUDIO_H_
#define _HLD_AUDIO_H_

#include "hld.h"
#include "lib/common.h"

enum hldAudioMode
{
    AM_NONE = 0x00,
    AM_KMIXER,
    AM_SINE,
    AM_ONECHANNEL
};

// IOCTL Commands
#define AC_SET_VOLUME       0x00
#define AC_SET_VOLUME_SOFT  0x01
#define AC_GET_VOLUME       0x02
#define AC_DISABLE          0x03
#define AC_ENABLE           0x04
#define AC_SET_SAMPLE       0x05
#define AC_SET_BITS         0x06
#define AC_SET_CHANNELS         0x07

// IOCTL Params
#define AUDIO_LINEIN    0x01
#define AUDIO_MICIN     0x02
#define AUDIO_LINEOUT   0x04
#define AUDIO_OUTAMP    0x08

struct hldAudioConfig
{
    // In Hz, eg. 44100, 48000, 96000
    UINT32 sampleRate;
    //eg. 1,2
    UINT8 channels;
    //eg. 8, 16, 24, 32
    UINT8 bits;
    //eg. AUDIO_LINEIN | AUDIO_LINEOUT
    UINT8 enable;
    UINT8 intPriority;
    enum hldAudioMode mode;
};

struct hldAudioDevice
{
    struct hldDevice head;
    struct hldAudioConfig config;

    retcode (*attach)(struct hldAudioConfig *pCfg);
    retcode (*open)(struct hldAudioDevice *pAudioDev);
    retcode (*close)(struct hldAudioDevice *pAudioDev);

    retcode (*ioctl)(struct hldAudioDevice *pAudioDev, UINT32 pCmd, UINT32 pParam);
};

#endif
