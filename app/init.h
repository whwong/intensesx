/**
 * @file init.h
 * @brief Hw and Sw init procedures
 * @author Daniel Szot
 */

#ifndef _APP_INIT_H
#define _APP_INIT_H

#include "lib/common.h"

// Hardware abstraction layer
#include "hal/hld/hld.h"
#include "hal/hld/lcd.h"
#include "hal/hld/uart.h"
#include "hal/hld/i2c.h"
#include "hal/hld/adc.h"
#include "hal/hld/touch.h"
#include "hal/hld/audio.h"

// Low level drivers
#include "hal/lld/platforms/microchip/pic32UART.h"
#include "hal/lld/platforms/microchip/pic32I2C.h"
#include "hal/lld/platforms/microchip/pic32ADC.h"
#include "hal/lld/platforms/microchip/pic32IR.h"
#include "hal/lld/resistiveTouch.h"
#include "hal/lld/platforms/microchip/wm8731.h"
#include "hal/lld/platforms/microchip/pic32SDMMC.h"
#include "hal/lld/platforms/microchip/fpgaGPU.h"

// Libraries
#include "lib/graphics/graphics.h"
#include "lib/input/inputTask.h"
#include "lib/user/msgListener.h"
#include "lib/log.h"
#include "lib/ir/nec.h"
#include "lib/input/inputEvent.h"
#include "lib/user/msgDispatcher.h"
#include "lib/audio/audioConsole.h"
#include "lib/fatfs/fatfsConsole.h"
#include "lib/audio/audio1ch.h"

// App
#include "rsc/rsc.h"

retcode appInit();

#endif
