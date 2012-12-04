/**
 * @file audioConsole.h
 * @brief Interface between console and audio drivers and library
 * @author Daniel Szot
 */

#ifndef _AUDIO_CONSOLE_H
#define _AUDIO_CONSOLE_H

#include "lib/common.h"
#include "audioSinLUT.h"
#include "hal/hld/audio.h"

void audioConsoleRegisterCmds();

#endif
