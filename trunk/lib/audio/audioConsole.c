/**
 * @file audioConsole.h
 * @brief Interface between console and audio drivers
 * @note At this moment we have only setting volume function, but if we
 * make more sophisticated driver we could write some more
 * useful console cmds :)
 * @author Daniel Szot
 */

#include "audioConsole.h"

static void audioConsoleSetv(UINT8 argc, const char *argv[])
{
    if (argc == 2 && (strcmp(argv[1], "-h") == 0))
    {
        LOG("\tThis command sets audio driver volume level");
        LOG("\tusage: 'asetv devid volume soft'");
    }
    else if (argc == 3 || argc == 4)
    {
        struct hldAudioDevice *aud;
        UINT16 devid = atoi(argv[1]);
        INT32 vol = atoi(argv[2]);
        aud = (struct hldAudioDevice*)hldDeviceGetById(HLD_DEVICE_TYPE_AUDIO, devid);

        if (aud != NULL)
        {
            if (vol > 100)
            {
                LOG("\tMaximum acceptable value is 100");
                return;
            }

            if (vol < 0)
            {
                LOG("\tMainimum acceptable value is 0");
                return;
            }

            if (argc == 4)
            {
                UINT16 soft = atoi(argv[3]);
                soft = (soft == 0) ? AC_SET_VOLUME : AC_SET_VOLUME_SOFT;
                aud->ioctl(aud, soft, vol);
            }
            else
                aud->ioctl(aud, AC_SET_VOLUME, vol);
        }
        else
            LOG("\tNo audio device with given ID found");
    }
    else
    {
        LOG("\tThis command sets audio driver volume level");
        LOG("\tusage: 'asetv devid volume'");
    }
}

/**
 * Execute just after console init
 */
void audioConsoleRegisterCmds()
{
    consoleRegisterCmd("asetv", &audioConsoleSetv);
}
