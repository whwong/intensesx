/**
 * @file fatfsConsole.c
 * @brief Console onterface for fatfs file system
 * @author Daniel Szot
 */

#include "fatfsConsole.h"

#define PWD_MAX_LEN 64

static void fatfsConsoleError(FRESULT pErr)
{
    switch(pErr)
    {
        case FR_OK:
            LOG("\t(0) Succeeded"); break;
        case FR_DISK_ERR:
            LOG("\t(1) A hard error occured in the low level disk I/O layer"); break;
        case FR_INT_ERR:
            LOG("\t(2) Assertion failed"); break;
        case FR_NOT_READY:
            LOG("\t(3) The physical drive cannot work"); break;
        case FR_NO_FILE:
            LOG("\t(4) Could not find the file"); break;
        case FR_NO_PATH:
            LOG("\t(5) Could not find the path"); break;
        case FR_INVALID_NAME:
            LOG("\t(6) The path name format is invalid"); break;
        case FR_DENIED:
            LOG("\t(7) Acces denied due to prohibited access or directory full"); break;
        case FR_EXIST:
            LOG("\t(8) Acces denied due to prohibited access"); break;
        case FR_INVALID_OBJECT:
            LOG("\t(9) The file/directory object is invalid"); break;
        case FR_WRITE_PROTECTED:
            LOG("\t(10) The physical drive is write protected"); break;
        case FR_INVALID_DRIVE:
            LOG("\t(11) The logical drive number is invalid"); break;
        case FR_NOT_ENABLED:
            LOG("\t(12) The volume has no work area"); break;
        case FR_NO_FILESYSTEM:
            LOG("\t(13) There is no valid FAT volume"); break;
        case FR_MKFS_ABORTED:
            LOG("\t(14) The f_mkfs() aborted due to any parameter error"); break;
        case FR_TIMEOUT:
            LOG("\t(15) Could not get a grant to access the volume within defined period"); break;
        case FR_LOCKED:
            LOG("\t(16) The operation is rejected according to the file shareing policy"); break;
        case FR_NOT_ENOUGH_CORE:
            LOG("\t(17) LFN working buffer could not be allocated"); break;
        case FR_TOO_MANY_OPEN_FILES:
            LOG("\t(18) Number of open files > _FS_SHARE"); break;
        case FR_INVALID_PARAMETER:
            LOG("\t(19) Given parameter is invalid"); break;
    }
}

static void fatfsCdCmdUsage()
{
    LOG("\tThis command sets given dir as current");
    LOG("\tusage: 'cd path'");
}

static void fatfsCdCmd(UINT8 argc, const char *argv[])
{
    if (argc == 2 && (strcmp(argv[1], "-h") == 0))
    {
        fatfsCdCmdUsage();
    }
    else if (argc == 2)
    {
        FRESULT r = f_chdir(argv[1]);
        if (r == FR_OK)
        {
            UINT8 buf[PWD_MAX_LEN];
            f_getcwd(buf, PWD_MAX_LEN);
            LOG("\t%s", buf);
        }
        else
        {
            fatfsConsoleError(r);
        }
    }
    else
    {
        fatfsCdCmdUsage();
    }
}

static void fatfsPwdCmdUsage()
{
    LOG("\tThis command prints current directory");
    LOG("\tusage: 'pwd'");
}

static void fatfsPwdCmd(UINT8 argc, const char *argv[])
{
    if (argc == 2 && (strcmp(argv[1], "-h") == 0))
    {
        fatfsPwdCmdUsage();
    }
    else if (argc == 1)
    {
        UINT8 buf[PWD_MAX_LEN];
        FRESULT r = f_getcwd(buf, PWD_MAX_LEN);

        if (r == FR_OK)
            LOG("\t%s", buf);
        else
            fatfsConsoleError(r);
    }
    else
    {
        fatfsPwdCmdUsage();
    }
}

static void fatfsLsCmdUsage()
{
    LOG("\tThis command prints all elements list from current directory");
    LOG("\tusage: 'ls'");
}

static void fatfsLsCmd(UINT8 argc, const char *argv[])
{
    if (argc == 2 && (strcmp(argv[1], "-h") == 0))
    {
        fatfsLsCmdUsage();
    }
    else if (argc == 1)
    {
        FRESULT res;
        FILINFO fno;
        DIR dir;
        int i;
        char *fn;
        UINT8 buf[PWD_MAX_LEN];

        f_getcwd(buf, PWD_MAX_LEN);
        res = f_opendir(&dir, buf);
        if (res == FR_OK)
        {
            i = strlen(buf);
            for (;;)
            {
                res = f_readdir(&dir, &fno);
                if (res != FR_OK || fno.fname[0] == 0) break;
                if (fno.fname[0] == '.') continue; // Ignore dot entry
                fn = fno.fname;
                if (fno.fattrib & AM_DIR)
                {
                    // It is a directory
                    LOG("\t[%s]", fn);
                }
                else
                {
                    // It is a file.
                    LOG("\t%s", fn);
                }
            }
        }
    }
    else
    {
        fatfsLsCmdUsage();
    }
}

void fatfsConsoleRegisterCmds()
{
    consoleRegisterCmd("cd", &fatfsCdCmd);
    consoleRegisterCmd("pwd", &fatfsPwdCmd);
    consoleRegisterCmd("ls", &fatfsLsCmd);
}
