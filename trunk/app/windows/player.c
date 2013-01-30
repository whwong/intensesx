/**
 * @file player.c
 * @brief Player application source file
 * @author Daniel Szot
 */

#include "player.h"

// Max dir len
#define PWD_MAX_LEN 64

/* Timer period - should be smth about 100-2000ms */
#define TIMER_PERIOD_MS 500
/* Just random value :) */
#define TIMER_ID 122
/* RTOS timeout to add/del timer from timer queue */
#define TIMER_TIMEOUT 1000

#define MSG_PLAY_END (MSG_USER+0x0010)

static struct guiMainWindow *wndPlayer;

extern retcode pca9532WriteReg(struct hldI2CDevice *i2c, UINT8 pReg, UINT8 pVal);

// Main buttons
static struct guiWindow *btnRpt;
static struct guiWindow *btnPrev;
static struct guiWindow *btnPlay;
static struct guiWindow *btnNext;
static struct guiWindow *btnList;

// Additional buttons
static struct guiWindow *btnSet;
static struct guiWindow *btnVol;

// Progress bars
static struct guiWindow *pbVol;
static struct guiWindow *pbPos;

// Static texts
static struct guiWindow *stTitle;
static struct guiWindow *stDetails;
static struct guiWindow *stTitleBar;

// State variables
static UINT32 currentFileIndex = 1;
static UINT32 currentSamplesCount = 0;
static UINT32 currentSample = 0;
static BOOL stopPosUpdating = 0;
static UINT32 rptState = 0;
static xTimerHandle hTimer;

static void appPlayerTimerProc(xTimerHandle pxTimer);
static void appPbVolChanged();

static UINT32 getPwdPlayableCount()
{
    FRESULT res;
    FILINFO fno;
    DIR dir;
    char *fn;
    UINT32 fnlen, counter;
    UINT8 buf[PWD_MAX_LEN];

    counter = 0;
    f_getcwd(buf, PWD_MAX_LEN);
    res = f_opendir(&dir, buf);
    if (res == FR_OK)
    {
        for (;;)
        {
            res = f_readdir(&dir, &fno);
            if (res != FR_OK || fno.fname[0] == 0) break;
            if (fno.fname[0] == '.') continue; // Ignore dot entry
            if (fno.fattrib & AM_DIR) continue;
            fn = fno.fname;
            fnlen = strlen(fn);
            
            if ((fn[fnlen-4] == '.') && (fn[fnlen-3] == 'W') &&
                    (fn[fnlen-2] == 'A') && (fn[fnlen-1] == 'V'))
            {
                counter++;
            }
        }
    }

    return counter;
}

static BOOL getPwdPlayableIdxName(UINT32 pIdx, char *pBuf)
{
    FRESULT res;
    FILINFO fno;
    DIR dir;
    char *fn;
    UINT32 fnlen, counter;
    UINT8 buf[PWD_MAX_LEN];

    counter = 0;
    f_getcwd(buf, PWD_MAX_LEN);
    res = f_opendir(&dir, buf);
    if (res == FR_OK)
    {
        for (;;)
        {
            res = f_readdir(&dir, &fno);
            if (res != FR_OK || fno.fname[0] == 0) break;
            if (fno.fname[0] == '.') continue; // Ignore dot entry
            if (fno.fattrib & AM_DIR) continue;
            fn = fno.fname;
            fnlen = strlen(fn);

            if ((fn[fnlen-4] == '.') && (fn[fnlen-3] == 'W') &&
                    (fn[fnlen-2] == 'A') && (fn[fnlen-1] == 'V'))
            {
                counter++;
                if (counter == pIdx)
                {
                    UINT32 buflen = strlen(buf);
                    strcpy(pBuf, buf);
                    if (pBuf[buflen-1] != '/')
                    {
                        strcpy(pBuf + buflen, "/");
                        strcpy(pBuf + buflen + 1, fn);
                    }
                    else
                    {
                        strcpy(pBuf + buflen, fn);
                    }
                    return TRUE;
                }
            }
        }
    }
    
    return FALSE;
}

static UINT32 getPwdIdxByName(char *pFN)
{
    FRESULT res;
    FILINFO fno;
    DIR dir;
    char *fn;
    UINT32 fnlen, counter;
    UINT8 buf[PWD_MAX_LEN];

    counter = 0;
    f_getcwd(buf, PWD_MAX_LEN);
    res = f_opendir(&dir, buf);
    if (res == FR_OK)
    {
        for (;;)
        {
            res = f_readdir(&dir, &fno);
            if (res != FR_OK || fno.fname[0] == 0) break;
            if (fno.fname[0] == '.') continue; // Ignore dot entry
            if (fno.fattrib & AM_DIR) continue;
            fn = fno.fname;
            fnlen = strlen(fn);

            if ((fn[fnlen-4] == '.') && (fn[fnlen-3] == 'W') &&
                    (fn[fnlen-2] == 'A') && (fn[fnlen-1] == 'V'))
            {
                counter++;
                LOG("f1 %s f2 %s", pFN, fn);
                if (strcmp(pFN, fn) == 0)
                    return counter;
            }
        }
    }

    return 1;
}

// TODO: I know that below 2 functiions are ugly and not optimal. I will need to
// change it in future
static void pathRemoveFN(char *pPath)
{
    UINT32 pathlen = strlen(pPath);
    INT32 i;

    for (i = pathlen - 1; i >= 0; i--)
    {
        if (pPath[i] == '/')
        {
            if ((i > 0) && (pPath[i-1] == ':'))
                pPath[i+1] = '\0';
            else
                pPath[i] = '\0';

            return;
        }
    }

    if ((i < 0) && (pathlen > 0))
        pPath[0] == '\0';
}

static char * pathSkipPath(char *pPath)
{
    UINT32 pathlen = strlen(pPath);
    INT32 i;

    for (i = pathlen - 1; i >= 0; i--)
    {
        if (pPath[i] == '/')
        {
            return pPath + i + 1;
        }
    }

    return pPath;
}

static void playUpdateIndexByFN(char *pFN)
{
    FRESULT res;
    char path[256];
    
    strcpy(path, pFN);
    pathRemoveFN(path);
    res = f_chdir(path);
    if (res == FR_OK)
    {
        currentFileIndex = getPwdIdxByName(pathSkipPath(pFN));
    }
}

void playError()
{
    msgSend(stTitle, MSG_SETTEXT, 0, (UINT32)"Can't open the file");
    msgPost(stTitle, MSG_PAINT, 0, 0);

    msgSend(stDetails, MSG_SETTEXT, 0, (UINT32)"");
    msgPost(stDetails, MSG_PAINT, 0, 0);

    msgSend(btnPlay, MSG_SETTEXT, 0, (UINT32)"PLAY");
    msgPost(btnPlay, MSG_PAINT, 0, 0);
}

static void appUpdatePosPB()
{
    UINT32 secPassed;
    UINT32 minPassed;
    UINT32 sampleRate;
    char buf[32];

    currentSample = audio1chGetCurrentSample()/1000;

    sampleRate = audio1chGetSampleRate();
    if (sampleRate == 0)
        secPassed = 0;
    else
        secPassed = (currentSample * 1000) / sampleRate;

    minPassed = secPassed / 60;
    secPassed %= 60;

    sprintf(buf, "%d:%02d", minPassed, secPassed);
    msgSend(pbPos, MSG_SETTEXT, 0, (UINT32)buf);

    if (stopPosUpdating == FALSE)
    {
        msgPost(pbPos, PBM_SETPOS, currentSample, 0);
    }
    else
    {
        msgPost(pbPos, MSG_PAINT, 0, 0);
    }
}

void skipForward()
{
    UINT32 seed = xTaskGetTickCount();
    UINT32 playableCount = getPwdPlayableCount();
    if ((rptState == 0) || (rptState == 1))
    {
        if (currentFileIndex < playableCount)
            currentFileIndex++;
        else
            currentFileIndex = 1;
    }
    else
    {
        currentFileIndex = rand_r((UINT *)&seed) % playableCount;
    }
}

void skipBackward()
{
    UINT32 seed = xTaskGetTickCount();
    UINT32 playableCount = getPwdPlayableCount();
    UINT32 sec = 0;

    if (audio1chGetSampleRate() > 0)
        sec = audio1chGetCurrentSample() / audio1chGetSampleRate();

    LOG("S: %d", sec);

    if (sec < 10)
    {
        if ((rptState == 0) || (rptState == 1))
        {
            if (currentFileIndex > 0)
                currentFileIndex--;
            else
                currentFileIndex = playableCount;
        }
        else
        {
            currentFileIndex = rand_r((UINT *)&seed) % playableCount;
        }
    }
}

void playActual(BOOL pSkipForward)
{
    UINT32 errCount = 0;
    char buf[256];
    UINT32 sr, ch, b, sec, min;

    while(1)
    {
        if (getPwdPlayableIdxName(currentFileIndex, buf))
        {
            if (audio1chPlaySound(buf, SND_ASYNC) != SUCCESS)
            {
                if ((errCount++) < 10)
                {
                    if (pSkipForward)
                        skipForward();
                    else
                        skipBackward();
                    continue;
                }
                else
                {
                    playError();
                    return;
                }
            }

            msgSend(stTitle, MSG_SETTEXT, 0, (UINT32)pathSkipPath(buf));
            msgPost(stTitle, MSG_PAINT, 0, 0);

            currentSamplesCount = audio1chGetSamplesCount() / 1000;
            currentSample = 0;

            msgPost(pbPos, PBM_SETPOS, 0, 0);
            msgPost(pbPos, PBM_SETRANGE, 0, currentSamplesCount);

            sr = audio1chGetSampleRate();
            ch = audio1chGetChannels();
            b = audio1chGetBits();

            if (sr == 0)
                return;

            sec = (currentSamplesCount * 1000) / sr;
            min = sec / 60;
            sec %= 60;

            sprintf(buf, "%dkHz, %dch, %dbit, %d:%02d", sr/1000, ch, b, min, sec);
            msgSend(stDetails, MSG_SETTEXT, 0, (UINT32)buf);
            msgPost(stDetails, MSG_PAINT, 0, 0);

            appUpdatePosPB();
            return;
        }
        else
        {
            if ((errCount++) < 10)
            {
                if (pSkipForward)
                    skipForward();
                else
                    skipBackward();

                continue;
            }
            else
            {
                playError();
                return;
            }
        }
    }
}

void playNext()
{
    skipForward();
    playActual(TRUE);
}

void playPrev()
{
    skipBackward();
    playActual(FALSE);
}

void playFilename(char *pFN)
{
    playUpdateIndexByFN(pFN);
    playActual(TRUE);
}

static retcode appPlayerCreate()
{
    hTimer = xTimerCreate("PLR", TIMER_PERIOD_MS / portTICK_RATE_MS,
                pdTRUE, (void *)TIMER_ID, appPlayerTimerProc);

    if (hTimer == NULL)
    {
        return ERR;
    }
    else
    {
        if (xTimerStart(hTimer, TIMER_TIMEOUT) != pdPASS)
        {
            xTimerDelete(hTimer, TIMER_TIMEOUT);
            return ERR;
        }
    }

    return SUCCESS;
}

void appPlayingEndCallback()
{
    msgPost((struct guiWindow*)wndPlayer, MSG_PLAY_END, 0, 0);
}

void appPlayerShow()
{
    if (appPlayerCreate() == SUCCESS)
    {
        // We can't use guiShowWindow because we want to show window before creating
        // message listener for this (guiShowWindow executes msgPost)
        wndPlayer->head.windowStyle |= WS_VISIBLE;
        guiSetCurrentMainWindow((struct guiMainWindow *)wndPlayer);
        msgSend((struct guiWindow *)wndPlayer, MSG_NCPAINT, 0 ,0);
        msgSend((struct guiWindow *)wndPlayer, MSG_PAINT, 0 ,0);
        msgSend(btnPlay, MSG_SETFOCUS, 0, 0);
        audioSetPlayingEndCallback(&appPlayingEndCallback);
    }
}

static void appPlayerTimerProc(xTimerHandle pxTimer)
{
    appUpdatePosPB();
}

// *****************************************************************************
// * BUTTONS CALLBACKS
// *****************************************************************************

static void appBtnRptClicked()
{
    if (rptState < 2)
        rptState++;
    else
        rptState = 0;

    switch(rptState)
    {
        case 0: msgSend(btnRpt, MSG_SETTEXT, 0, (UINT32)"->"); break;
        case 1: msgSend(btnRpt, MSG_SETTEXT, 0, (UINT32)"RPT"); break;
        case 2: msgSend(btnRpt, MSG_SETTEXT, 0, (UINT32)"SHUFF"); break;
    }

    msgPost(btnRpt, MSG_PAINT, 0, 0);
}

static void appBtnPrevClicked()
{
    playPrev();
}

static void appBtnPlayClicked()
{
    if (audio1chIsPlaying() == TRUE)
    {
        audio1chPause(1);
        msgSend(btnPlay, MSG_SETTEXT, 0, (UINT32)"PLAY");
        msgPost(btnPlay, MSG_PAINT, 0, 0);
    }
    else
    {
        msgSend(btnPlay, MSG_SETTEXT, 0, (UINT32)"PAUSE");
        msgPost(btnPlay, MSG_PAINT, 0, 0);

        if (audio1chIsFileLoaded())
        {
            audio1chPause(0);
        }
        else
        {
            playActual(TRUE);
        }
    }
}

static void appBtnNextClicked()
{
    playNext();
}

static void appBtnListClicked()
{

}

static void appBtnSetClicked()
{
    bigScreenInit();
}

static void appBtnVolClicked()
{
    static BOOL hidden = TRUE;

    if (hidden == TRUE)
    {
        guiShowWindow(pbVol, SW_SHOW);
        appPbVolChanged();
    }
    else
    {
        guiShowWindow(pbVol, SW_HIDE);
        msgSend(btnVol, MSG_SETTEXT, 0, (UINT32)"VOL");
        msgSend(btnVol, MSG_PAINT, 0, 0);
    }

    hidden = 1 - hidden;
}

// *****************************************************************************
// * PROGRESS BARS CALLBACKS
// *****************************************************************************

static void appPbPosDraggingEnd()
{
    UINT32 p;
    msgSend(pbPos, PBM_GETPOS, (UINT32)&p, 0);
    audio1chSetCurrentSample(p*1000);
    stopPosUpdating = FALSE;
}

static void appPbPosDraggingStart()
{
    stopPosUpdating = TRUE;
}

static void appPbVolDraggingEnd()
{
    appBtnVolClicked();
}

static void appPbPosChanged()
{
    UINT32 pos;
    msgSend(pbPos, PBM_GETPOS, (UINT32)&pos, 0);
    LOG("Pos: %d", pos);
}
#include "app/lcdInit.h"
static void appPbVolChanged()
{
    struct hldAudioDevice *audio;
    UINT32 pos;
    char buf[5];
    
    msgSend(pbVol, PBM_GETPOS, (UINT32)&pos, 0);
    sprintf(buf, "%d", pos);
    msgSend(btnVol, MSG_SETTEXT, 0, (UINT32)buf);
    msgSend(btnVol, MSG_PAINT, 0, 0);

    // Set volume with zero crossing detection
    audio = hldDeviceGetByType(NULL, HLD_DEVICE_TYPE_AUDIO);
    audio->ioctl(audio, AC_SET_VOLUME_SOFT, pos);
}

// *****************************************************************************
// * MAIN WINDOW FUNCTIONS
// *****************************************************************************

static INT32 appPlayerWndProc(struct guiWindow *pWnd, UINT32 pMsg, UINT32 pParam1, UINT32 pParam2)
{
    UINT32 notify, id;
    switch(pMsg)
    {
        case MSG_CREATE:
            break;

        case MSG_PLAY_END:
            if (rptState == 1)
                playActual(TRUE);
            else
                playNext();
            break;

        case MSG_COMMAND:
            
            notify = pParam1 >> 16;
            id = pParam1 & 0xFFFF;
            
            if (notify == BN_CLICKED)
            {
                switch(id)
                {
                    case IDC_BTN_RPT: appBtnRptClicked(); break;
                    case IDC_BTN_PREV: appBtnPrevClicked(); break;
                    case IDC_BTN_PLAY: appBtnPlayClicked(); break;
                    case IDC_BTN_NEXT: appBtnNextClicked(); break;
                    case IDC_BTN_LIST: appBtnListClicked(); break;
                    case IDC_BTN_SET: appBtnSetClicked(); break;
                    case IDC_BTN_VOL: appBtnVolClicked(); break;
                }
            }
            else if (notify == PBN_REACHMAX)
            {
                
            }
            else if (notify == PBN_REACHMIN)
            {

            }
            else if (notify == PBN_CLICKED)
            {

            }
            else if (notify == PBN_DRAGGINGSTART)
            {
                switch(id)
                {
                    case IDC_PB_POS: appPbPosDraggingStart(); break;
                }
            }
            else if (notify == PBN_DRAGGINGEND)
            {
                switch(id)
                {
                    case IDC_PB_POS: appPbPosDraggingEnd(); break;
                    case IDC_PB_VOL: appPbVolDraggingEnd(); break;
                }
            }
            else if (notify == PBN_CHANGED)
            {
                switch(id)
                {
                    case IDC_PB_POS: appPbPosChanged(); break;
                    case IDC_PB_VOL: appPbVolChanged(); break;
                }
            }
            break;
    }
    return guiDefWindowProc(pWnd, pMsg, pParam1, pParam2);
}

retcode appPlayerInit()
{
    struct guiWndClassInfo wci;
    wci.className = "wndplayer";
    wci.windowStyle = WS_BORDER;
    wci.colorStyle.shIdx = WSTL_WINDOW_SH;
    wci.colorStyle.hlIdx = WSTL_WINDOW_HL;
    wci.colorStyle.selIdx = WSTL_WINDOW_SH;
    wci.colorStyle.gryIdx = WSTL_WINDOW_SH;
    wci.windowProc = appPlayerWndProc;

    guiRegisterWindowClass(&wci);
    
    wndPlayer = guiCreateMainWindow("wndplayer",
        "Music Player",
        WS_NONE,
        IDC_WND_PLAYER, IDC_BTN_PLAY,
        WND_PLAYER_X,
        WND_PLAYER_Y,
        WND_PLAYER_W,
        WND_PLAYER_H);

    btnRpt = guiCreateWindow("button",
        "->",
        WS_VISIBLE | WS_FOCUSSTOP,
        IDC_BTN_RPT, 
        IDC_BTN_LIST, IDC_BTN_PREV,
        IDC_BTN_SET, IDC_BTN_RPT, // Need mod
        BTN_RPT_X,
        BTN_RPT_Y,
        BTN_RPT_W,
        BTN_RPT_H,
        (struct guiWindow *)wndPlayer, 0);

    btnPrev = guiCreateWindow("button",
        "PREV",
        WS_VISIBLE | WS_FOCUSSTOP,
        IDC_BTN_PREV,
        IDC_BTN_RPT, IDC_BTN_PLAY,
        IDC_BTN_SET, IDC_BTN_PREV, // Need mod
        BTN_PREV_X,
        BTN_PREV_Y,
        BTN_PREV_W,
        BTN_PREV_H,
        (struct guiWindow *)wndPlayer, 0);

    btnPlay = guiCreateWindow("button",
        "PLAY",
        WS_VISIBLE | WS_FOCUSSTOP,
        IDC_BTN_PLAY,
        IDC_BTN_PREV, IDC_BTN_NEXT,
        IDC_BTN_VOL, IDC_BTN_PLAY, // Need mod
        BTN_PLAY_X,
        BTN_PLAY_Y,
        BTN_PLAY_W,
        BTN_PLAY_H,
        (struct guiWindow *)wndPlayer, 0);

    btnNext = guiCreateWindow("button",
        "NEXT",
        WS_VISIBLE | WS_FOCUSSTOP,
        IDC_BTN_NEXT,
        IDC_BTN_PLAY, IDC_BTN_LIST,
        IDC_BTN_VOL, IDC_BTN_NEXT, // Need mod
        BTN_NEXT_X,
        BTN_NEXT_Y,
        BTN_NEXT_W,
        BTN_NEXT_H,
        (struct guiWindow *)wndPlayer, 0);

    btnList = guiCreateWindow("button",
        "LIST",
        WS_VISIBLE | WS_FOCUSSTOP,
        IDC_BTN_LIST,
        IDC_BTN_NEXT, IDC_BTN_RPT,
        IDC_BTN_VOL, IDC_BTN_LIST, // Need mod
        BTN_LIST_X,
        BTN_LIST_Y,
        BTN_LIST_W,
        BTN_LIST_H,
        (struct guiWindow *)wndPlayer, 0);

    btnSet = guiCreateWindow("button",
        "SET",
        WS_VISIBLE | WS_FOCUSSTOP,
        IDC_BTN_SET,
        IDC_BTN_VOL, IDC_BTN_VOL,
        IDC_BTN_SET, IDC_BTN_RPT, // Need mod
        BTN_SET_X,
        BTN_SET_Y,
        BTN_SET_W,
        BTN_SET_H,
        (struct guiWindow *)wndPlayer, 0);

    btnVol = guiCreateWindow("button",
        "VOL",
        WS_VISIBLE | WS_FOCUSSTOP,
        IDC_BTN_VOL,
        IDC_BTN_SET, IDC_BTN_SET,
        IDC_BTN_VOL, IDC_BTN_LIST, // Need mod
        BTN_VOL_X,
        BTN_VOL_Y,
        BTN_VOL_W,
        BTN_VOL_H,
        (struct guiWindow *)wndPlayer, 0);
    
    msgSend(btnRpt, MSG_SETFONT, (UINT32)&g_DroidSans15, 0);
    msgSend(btnPrev, MSG_SETFONT, (UINT32)&g_DroidSans15, 0);
    msgSend(btnPlay, MSG_SETFONT, (UINT32)&g_DroidSans15, 0);
    msgSend(btnNext, MSG_SETFONT, (UINT32)&g_DroidSans15, 0);
    msgSend(btnList, MSG_SETFONT, (UINT32)&g_DroidSans15, 0);
    msgSend(btnSet, MSG_SETFONT, (UINT32)&g_DroidSans15, 0);
    msgSend(btnVol, MSG_SETFONT, (UINT32)&g_DroidSans15, 0);

    pbPos = guiCreateWindow("progressbar",
        "0:00",
        WS_VISIBLE | PBS_ALLOW_DRAGGING | PBS_NOTIFY,
        IDC_PB_POS, IDC_PB_POS, IDC_PB_POS, IDC_PB_POS, IDC_PB_POS,
        PB_POS_X,
        PB_POS_Y,
        PB_POS_W,
        PB_POS_H,
        (struct guiWindow *) wndPlayer, 0);

    pbVol = guiCreateWindow("progressbar",
        "",
        PBS_ALLOW_DRAGGING | PBS_NOTIFY | PBS_VERTICAL,
        IDC_PB_VOL, IDC_PB_VOL, IDC_PB_VOL, IDC_PB_VOL, IDC_PB_VOL,
        PB_VOL_X,
        PB_VOL_Y,
        PB_VOL_W,
        PB_VOL_H,
        (struct guiWindow *) wndPlayer, 0);

    msgSend(pbVol, PBM_SETRANGE, 0, 100);
    struct hldAudioDevice *audio;
    UINT32 vol;
    audio = hldDeviceGetByType(NULL, HLD_DEVICE_TYPE_AUDIO);
    audio->ioctl(audio, AC_GET_VOLUME, (UINT32)&vol);
    msgSend(pbVol, PBM_SETPOS, vol, 0);

    stTitle = guiCreateWindow("statictext",
        "Press play",
        WS_VISIBLE | SS_CENTER,
        IDC_ST_TITLE, IDC_ST_TITLE, IDC_ST_TITLE, IDC_ST_TITLE, IDC_ST_TITLE,
        ST_TITLE_X,
        ST_TITLE_Y,
        ST_TITLE_W,
        ST_TITLE_H,
        (struct guiWindow *) wndPlayer, 0);

    stDetails = guiCreateWindow("statictext",
        "Press play to hear a sound...",
        WS_VISIBLE | SS_RIGHT,
        IDC_ST_DETAILS, IDC_ST_DETAILS, IDC_ST_DETAILS, IDC_ST_DETAILS, IDC_ST_DETAILS,
        ST_DETAILS_X,
        ST_DETAILS_Y,
        ST_DETAILS_W,
        ST_DETAILS_H,
        (struct guiWindow *) wndPlayer, 0);


    stTitleBar = guiCreateWindow("statictext",
        " Audio player",
        WS_VISIBLE | SS_LEFT,
        IDC_ST_TITLE_BAR, IDC_ST_TITLE_BAR, IDC_ST_TITLE_BAR,
        IDC_ST_TITLE_BAR, IDC_ST_TITLE_BAR,
        ST_TITLE_BAR_X,
        ST_TITLE_BAR_Y,
        ST_TITLE_BAR_W,
        ST_TITLE_BAR_H,
        (struct guiWindow *) wndPlayer, 0);

    stTitleBar->colorStyle.shIdx = WSTL_TITLE_BAR;
    stTitleBar->colorStyle.gryIdx = WSTL_TITLE_BAR;
    msgSend(stDetails, MSG_ENABLE, FALSE, 0);

    msgSend(stTitle, MSG_SETFONT, (UINT32)&g_DroidSans29, 0);
    msgSend(stDetails, MSG_SETFONT, (UINT32)&g_DroidSans15, 0);
    msgSend(stTitleBar, MSG_SETFONT, (UINT32)&g_DroidSans15, 0);

    return SUCCESS;
}
