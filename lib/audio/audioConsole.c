/**
 * @file audioConsole.h
 * @brief Interface between console and audio drivers
 * @note At this moment we have only setting volume function, but if we
 * make more sophisticated driver we could write some more
 * useful console cmds :)
 * @author Daniel Szot
 */

#include "audioConsole.h"
#include "audio1ch.h"

static void audioConsoleSetv(UINT8 argc, const char *argv[])
{
    if (argc == 2 && (strcmp(argv[1], "-h") == 0))
    {
        LOG("\tThis command sets audio driver volume level");
        LOG("\tusage: 'asetv devid volume'");
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

static void audioConsolePause(UINT8 argc, const char *argv[])
{
    if (argc == 2 && (strcmp(argv[1], "-h") == 0))
    {
        LOG("\tThis command can play audio file");
        LOG("\tusage: 'aplay paused_state'");
        LOG("\tpaused_state: 1 - pause, 0 - resume");
    }
    else if (argc == 2)
    {
        audio1chPause(atoi(argv[1]));
    }
    else
    {
        LOG("\tThis command can play audio file");
        LOG("\tusage: 'aplay paused_state'");
        LOG("\tpaused_state: 1 - pause, 0 - resume");
    }
}

static void audioConsolePlay(UINT8 argc, const char *argv[])
{
    if (argc == 2 && (strcmp(argv[1], "-h") == 0))
    {
        LOG("\tThis command can play audio file");
        LOG("\tusage: 'aplay filename flags'");
    }
    else if (argc == 2 || argc == 3)
    {
        // Default parameters, need to flags always be qual to 0
        UINT32 flags = SND_ASYNC;

        if (argc == 3)
        {
            UINT8 i;
            for (i = 0; i < strlen(argv[2]); i++)
            {
                // And then if some flag is specified modify flags by binary
                // adding unstandard flags.
                if (argv[2][i] == 's')
                    flags |= SND_SYNC;
            }
        }

        retcode ret = audio1chPlaySound(argv[1], flags);

        if (ret == SUCCESS)
            DONE("audio1chPlaySound");
        else
            ERROR("audio1chPlaySound");
    }
    else
    {
        LOG("\tThis command can play audio file");
        LOG("\tusage: 'aplay filename flags'");
    }
}

static void audioConsoleStop(UINT8 argc, const char *argv[])
{
    if (argc == 2 && (strcmp(argv[1], "-h") == 0))
    {
        LOG("\tThis command stop playing audio file");
        LOG("\tusage: 'astop'");
    }
    else if (argc == 1)
    {
        audio1chStopSound(NULL);
        LOG("Sound stopped...");
    }
    else
    {
        LOG("\tThis command stop playing audio file");
        LOG("\tusage: 'astop'");
    }
}

/**
 * Execute just after console init
 */
void audioConsoleRegisterCmds()
{
    consoleRegisterCmd("asetv", &audioConsoleSetv);
    consoleRegisterCmd("aplay", &audioConsolePlay);
    consoleRegisterCmd("apause", &audioConsolePause);
    consoleRegisterCmd("astop", &audioConsoleStop);
}
