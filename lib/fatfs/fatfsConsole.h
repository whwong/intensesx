/**
 * @file fatfscmd.c
 * @brief Console onterface for fatfs file system
 * @author Daniel Szot
 */

#include "lib/common.h"
#include "lib/fatfs/ff.h"
#include "lib/console/serialConsole.h"

void fatfsConsoleRegisterCmds();

static void fatfsCdCmd(UINT8 argc, const char *argv[]);
