#
# Generated Makefile - do not edit!
#
# Edit the Makefile in the project folder instead (../Makefile). Each target
# has a -pre and a -post target defined where you can add customized code.
#
# This makefile implements configuration specific macros and targets.


# Include project Makefile
include Makefile

# Environment
SHELL=cmd.exe
# Adding MPLAB X bin directory to path
PATH:=C:/Program Files (x86)/Microchip/MPLABX/mplab_ide/mplab_ide/modules/../../bin/:$(PATH)
MKDIR=gnumkdir -p
RM=rm -f 
MV=mv 
CP=cp 

# Macros
CND_CONF=default
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
IMAGE_TYPE=debug
OUTPUT_SUFFIX=elf
DEBUGGABLE_SUFFIX=elf
FINAL_IMAGE=dist/${CND_CONF}/${IMAGE_TYPE}/IntenseS.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
else
IMAGE_TYPE=production
OUTPUT_SUFFIX=hex
DEBUGGABLE_SUFFIX=elf
FINAL_IMAGE=dist/${CND_CONF}/${IMAGE_TYPE}/IntenseS.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
endif

# Object Directory
OBJECTDIR=build/${CND_CONF}/${IMAGE_TYPE}

# Distribution Directory
DISTDIR=dist/${CND_CONF}/${IMAGE_TYPE}

# Object Files Quoted if spaced
OBJECTFILES_QUOTED_IF_SPACED=${OBJECTDIR}/app/main.o ${OBJECTDIR}/boards/PIC32MX7MMB/board.o ${OBJECTDIR}/hal/hld/hld.o ${OBJECTDIR}/hal/lld/platforms/microchip/gpio.o ${OBJECTDIR}/hal/lld/platforms/microchip/ili9163.o ${OBJECTDIR}/hal/lld/platforms/microchip/hx8347.o ${OBJECTDIR}/hal/lld/platforms/microchip/pic32UART.o ${OBJECTDIR}/hal/lld/platforms/microchip/pic32UARTIsr.o ${OBJECTDIR}/hal/lld/platforms/microchip/pic32ADC.o ${OBJECTDIR}/hal/lld/platforms/microchip/pic32ADCIsr.o ${OBJECTDIR}/hal/lld/platforms/microchip/pic32IR.o ${OBJECTDIR}/hal/lld/platforms/microchip/pic32IRIsr.o ${OBJECTDIR}/hal/lld/platforms/microchip/pic32IRTimerIsr.o ${OBJECTDIR}/hal/lld/platforms/microchip/wm8731.o ${OBJECTDIR}/hal/lld/platforms/microchip/wm8731ISR.o ${OBJECTDIR}/hal/lld/platforms/microchip/pic32SDMMC.o ${OBJECTDIR}/hal/lld/resistiveTouch.o ${OBJECTDIR}/lib/audio/decoders/wave.o ${OBJECTDIR}/lib/audio/audioSineOutput.o ${OBJECTDIR}/lib/audio/audioSinLUT.o ${OBJECTDIR}/lib/audio/audioConsole.o ${OBJECTDIR}/lib/audio/audio1ch.o ${OBJECTDIR}/lib/console/serialConsole.o ${OBJECTDIR}/lib/fatfs/option/syscall.o ${OBJECTDIR}/lib/fatfs/option/unicode.o ${OBJECTDIR}/lib/fatfs/diskio.o ${OBJECTDIR}/lib/fatfs/ff.o ${OBJECTDIR}/lib/fatfs/fatfsConsole.o ${OBJECTDIR}/lib/graphics/graphics.o ${OBJECTDIR}/lib/graphics/primitives.o ${OBJECTDIR}/lib/graphics/tests.o ${OBJECTDIR}/lib/gui/window.o ${OBJECTDIR}/lib/input/inputEvent.o ${OBJECTDIR}/lib/input/inputTask.o ${OBJECTDIR}/lib/ir/nec.o ${OBJECTDIR}/lib/user/msgListener.o ${OBJECTDIR}/lib/user/msgDispatcher.o ${OBJECTDIR}/lib/log.o ${OBJECTDIR}/lib/digitalFilter.o ${OBJECTDIR}/lib/time.o ${OBJECTDIR}/os/port/MPLAB/PIC32MX/port.o ${OBJECTDIR}/os/port/MPLAB/PIC32MX/port_asm.o ${OBJECTDIR}/os/croutine.o ${OBJECTDIR}/os/heap_2.o ${OBJECTDIR}/os/list.o ${OBJECTDIR}/os/queue.o ${OBJECTDIR}/os/tasks.o ${OBJECTDIR}/os/timers.o
POSSIBLE_DEPFILES=${OBJECTDIR}/app/main.o.d ${OBJECTDIR}/boards/PIC32MX7MMB/board.o.d ${OBJECTDIR}/hal/hld/hld.o.d ${OBJECTDIR}/hal/lld/platforms/microchip/gpio.o.d ${OBJECTDIR}/hal/lld/platforms/microchip/ili9163.o.d ${OBJECTDIR}/hal/lld/platforms/microchip/hx8347.o.d ${OBJECTDIR}/hal/lld/platforms/microchip/pic32UART.o.d ${OBJECTDIR}/hal/lld/platforms/microchip/pic32UARTIsr.o.d ${OBJECTDIR}/hal/lld/platforms/microchip/pic32ADC.o.d ${OBJECTDIR}/hal/lld/platforms/microchip/pic32ADCIsr.o.d ${OBJECTDIR}/hal/lld/platforms/microchip/pic32IR.o.d ${OBJECTDIR}/hal/lld/platforms/microchip/pic32IRIsr.o.d ${OBJECTDIR}/hal/lld/platforms/microchip/pic32IRTimerIsr.o.d ${OBJECTDIR}/hal/lld/platforms/microchip/wm8731.o.d ${OBJECTDIR}/hal/lld/platforms/microchip/wm8731ISR.o.d ${OBJECTDIR}/hal/lld/platforms/microchip/pic32SDMMC.o.d ${OBJECTDIR}/hal/lld/resistiveTouch.o.d ${OBJECTDIR}/lib/audio/decoders/wave.o.d ${OBJECTDIR}/lib/audio/audioSineOutput.o.d ${OBJECTDIR}/lib/audio/audioSinLUT.o.d ${OBJECTDIR}/lib/audio/audioConsole.o.d ${OBJECTDIR}/lib/audio/audio1ch.o.d ${OBJECTDIR}/lib/console/serialConsole.o.d ${OBJECTDIR}/lib/fatfs/option/syscall.o.d ${OBJECTDIR}/lib/fatfs/option/unicode.o.d ${OBJECTDIR}/lib/fatfs/diskio.o.d ${OBJECTDIR}/lib/fatfs/ff.o.d ${OBJECTDIR}/lib/fatfs/fatfsConsole.o.d ${OBJECTDIR}/lib/graphics/graphics.o.d ${OBJECTDIR}/lib/graphics/primitives.o.d ${OBJECTDIR}/lib/graphics/tests.o.d ${OBJECTDIR}/lib/gui/window.o.d ${OBJECTDIR}/lib/input/inputEvent.o.d ${OBJECTDIR}/lib/input/inputTask.o.d ${OBJECTDIR}/lib/ir/nec.o.d ${OBJECTDIR}/lib/user/msgListener.o.d ${OBJECTDIR}/lib/user/msgDispatcher.o.d ${OBJECTDIR}/lib/log.o.d ${OBJECTDIR}/lib/digitalFilter.o.d ${OBJECTDIR}/lib/time.o.d ${OBJECTDIR}/os/port/MPLAB/PIC32MX/port.o.d ${OBJECTDIR}/os/port/MPLAB/PIC32MX/port_asm.o.d ${OBJECTDIR}/os/croutine.o.d ${OBJECTDIR}/os/heap_2.o.d ${OBJECTDIR}/os/list.o.d ${OBJECTDIR}/os/queue.o.d ${OBJECTDIR}/os/tasks.o.d ${OBJECTDIR}/os/timers.o.d

# Object Files
OBJECTFILES=${OBJECTDIR}/app/main.o ${OBJECTDIR}/boards/PIC32MX7MMB/board.o ${OBJECTDIR}/hal/hld/hld.o ${OBJECTDIR}/hal/lld/platforms/microchip/gpio.o ${OBJECTDIR}/hal/lld/platforms/microchip/ili9163.o ${OBJECTDIR}/hal/lld/platforms/microchip/hx8347.o ${OBJECTDIR}/hal/lld/platforms/microchip/pic32UART.o ${OBJECTDIR}/hal/lld/platforms/microchip/pic32UARTIsr.o ${OBJECTDIR}/hal/lld/platforms/microchip/pic32ADC.o ${OBJECTDIR}/hal/lld/platforms/microchip/pic32ADCIsr.o ${OBJECTDIR}/hal/lld/platforms/microchip/pic32IR.o ${OBJECTDIR}/hal/lld/platforms/microchip/pic32IRIsr.o ${OBJECTDIR}/hal/lld/platforms/microchip/pic32IRTimerIsr.o ${OBJECTDIR}/hal/lld/platforms/microchip/wm8731.o ${OBJECTDIR}/hal/lld/platforms/microchip/wm8731ISR.o ${OBJECTDIR}/hal/lld/platforms/microchip/pic32SDMMC.o ${OBJECTDIR}/hal/lld/resistiveTouch.o ${OBJECTDIR}/lib/audio/decoders/wave.o ${OBJECTDIR}/lib/audio/audioSineOutput.o ${OBJECTDIR}/lib/audio/audioSinLUT.o ${OBJECTDIR}/lib/audio/audioConsole.o ${OBJECTDIR}/lib/audio/audio1ch.o ${OBJECTDIR}/lib/console/serialConsole.o ${OBJECTDIR}/lib/fatfs/option/syscall.o ${OBJECTDIR}/lib/fatfs/option/unicode.o ${OBJECTDIR}/lib/fatfs/diskio.o ${OBJECTDIR}/lib/fatfs/ff.o ${OBJECTDIR}/lib/fatfs/fatfsConsole.o ${OBJECTDIR}/lib/graphics/graphics.o ${OBJECTDIR}/lib/graphics/primitives.o ${OBJECTDIR}/lib/graphics/tests.o ${OBJECTDIR}/lib/gui/window.o ${OBJECTDIR}/lib/input/inputEvent.o ${OBJECTDIR}/lib/input/inputTask.o ${OBJECTDIR}/lib/ir/nec.o ${OBJECTDIR}/lib/user/msgListener.o ${OBJECTDIR}/lib/user/msgDispatcher.o ${OBJECTDIR}/lib/log.o ${OBJECTDIR}/lib/digitalFilter.o ${OBJECTDIR}/lib/time.o ${OBJECTDIR}/os/port/MPLAB/PIC32MX/port.o ${OBJECTDIR}/os/port/MPLAB/PIC32MX/port_asm.o ${OBJECTDIR}/os/croutine.o ${OBJECTDIR}/os/heap_2.o ${OBJECTDIR}/os/list.o ${OBJECTDIR}/os/queue.o ${OBJECTDIR}/os/tasks.o ${OBJECTDIR}/os/timers.o


CFLAGS=
ASFLAGS=
LDLIBSOPTIONS=

# Path to java used to run MPLAB X when this makefile was created
MP_JAVA_PATH="C:\Program Files (x86)\Java\jre6/bin/"
OS_CURRENT="$(shell uname -s)"
############# Tool locations ##########################################
# If you copy a project from one host to another, the path where the  #
# compiler is installed may be different.                             #
# If you open this project with MPLAB X in the new host, this         #
# makefile will be regenerated and the paths will be corrected.       #
#######################################################################
MP_CC="C:\Program Files (x86)\Microchip\MPLAB C32 Suite\bin\pic32-gcc.exe"
# MP_BC is not defined
MP_AS="C:\Program Files (x86)\Microchip\MPLAB C32 Suite\bin\pic32-as.exe"
MP_LD="C:\Program Files (x86)\Microchip\MPLAB C32 Suite\bin\pic32-ld.exe"
MP_AR="C:\Program Files (x86)\Microchip\MPLAB C32 Suite\bin\pic32-ar.exe"
DEP_GEN=${MP_JAVA_PATH}java -jar "C:/Program Files (x86)/Microchip/MPLABX/mplab_ide/mplab_ide/modules/../../bin/extractobjectdependencies.jar" 
# fixDeps replaces a bunch of sed/cat/printf statements that slow down the build
FIXDEPS=fixDeps
MP_CC_DIR="C:\Program Files (x86)\Microchip\MPLAB C32 Suite\bin"
# MP_BC_DIR is not defined
MP_AS_DIR="C:\Program Files (x86)\Microchip\MPLAB C32 Suite\bin"
MP_LD_DIR="C:\Program Files (x86)\Microchip\MPLAB C32 Suite\bin"
MP_AR_DIR="C:\Program Files (x86)\Microchip\MPLAB C32 Suite\bin"
# MP_BC_DIR is not defined

.build-conf:  ${BUILD_SUBPROJECTS}
	${MAKE}  -f nbproject/Makefile-default.mk dist/${CND_CONF}/${IMAGE_TYPE}/IntenseS.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}

MP_PROCESSOR_OPTION=32MX795F512L
MP_LINKER_FILE_OPTION=
# ------------------------------------------------------------------------------------
# Rules for buildStep: assemble
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
else
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: assembleWithPreprocess
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
${OBJECTDIR}/hal/lld/platforms/microchip/pic32UARTIsr.o: hal/lld/platforms/microchip/pic32UARTIsr.S  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/hal/lld/platforms/microchip 
	@${RM} ${OBJECTDIR}/hal/lld/platforms/microchip/pic32UARTIsr.o.d 
	@${RM} ${OBJECTDIR}/hal/lld/platforms/microchip/pic32UARTIsr.o.ok ${OBJECTDIR}/hal/lld/platforms/microchip/pic32UARTIsr.o.err 
	@${FIXDEPS} "${OBJECTDIR}/hal/lld/platforms/microchip/pic32UARTIsr.o.d" "${OBJECTDIR}/hal/lld/platforms/microchip/pic32UARTIsr.o.asm.d" -t $(SILENT) -c ${MP_CC} $(MP_EXTRA_AS_PRE)  -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1 -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"E:/Projekty/Inz/IntenseS.X/os/port/MPLAB/PIC32MX/" -MMD -MF "${OBJECTDIR}/hal/lld/platforms/microchip/pic32UARTIsr.o.d"  -o ${OBJECTDIR}/hal/lld/platforms/microchip/pic32UARTIsr.o hal/lld/platforms/microchip/pic32UARTIsr.S  -Wa,--defsym=__MPLAB_BUILD=1$(MP_EXTRA_AS_POST),-MD="${OBJECTDIR}/hal/lld/platforms/microchip/pic32UARTIsr.o.asm.d",--defsym=__MPLAB_DEBUG=1,--defsym=__ICD2RAM=1,--gdwarf-2,--defsym=__DEBUG=1,--defsym=__MPLAB_DEBUGGER_ICD3=1,-I"E:/Projekty/Inz/IntenseS.X/os/",-ah
	
${OBJECTDIR}/hal/lld/platforms/microchip/pic32ADCIsr.o: hal/lld/platforms/microchip/pic32ADCIsr.S  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/hal/lld/platforms/microchip 
	@${RM} ${OBJECTDIR}/hal/lld/platforms/microchip/pic32ADCIsr.o.d 
	@${RM} ${OBJECTDIR}/hal/lld/platforms/microchip/pic32ADCIsr.o.ok ${OBJECTDIR}/hal/lld/platforms/microchip/pic32ADCIsr.o.err 
	@${FIXDEPS} "${OBJECTDIR}/hal/lld/platforms/microchip/pic32ADCIsr.o.d" "${OBJECTDIR}/hal/lld/platforms/microchip/pic32ADCIsr.o.asm.d" -t $(SILENT) -c ${MP_CC} $(MP_EXTRA_AS_PRE)  -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1 -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"E:/Projekty/Inz/IntenseS.X/os/port/MPLAB/PIC32MX/" -MMD -MF "${OBJECTDIR}/hal/lld/platforms/microchip/pic32ADCIsr.o.d"  -o ${OBJECTDIR}/hal/lld/platforms/microchip/pic32ADCIsr.o hal/lld/platforms/microchip/pic32ADCIsr.S  -Wa,--defsym=__MPLAB_BUILD=1$(MP_EXTRA_AS_POST),-MD="${OBJECTDIR}/hal/lld/platforms/microchip/pic32ADCIsr.o.asm.d",--defsym=__MPLAB_DEBUG=1,--defsym=__ICD2RAM=1,--gdwarf-2,--defsym=__DEBUG=1,--defsym=__MPLAB_DEBUGGER_ICD3=1,-I"E:/Projekty/Inz/IntenseS.X/os/",-ah
	
${OBJECTDIR}/hal/lld/platforms/microchip/pic32IRIsr.o: hal/lld/platforms/microchip/pic32IRIsr.S  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/hal/lld/platforms/microchip 
	@${RM} ${OBJECTDIR}/hal/lld/platforms/microchip/pic32IRIsr.o.d 
	@${RM} ${OBJECTDIR}/hal/lld/platforms/microchip/pic32IRIsr.o.ok ${OBJECTDIR}/hal/lld/platforms/microchip/pic32IRIsr.o.err 
	@${FIXDEPS} "${OBJECTDIR}/hal/lld/platforms/microchip/pic32IRIsr.o.d" "${OBJECTDIR}/hal/lld/platforms/microchip/pic32IRIsr.o.asm.d" -t $(SILENT) -c ${MP_CC} $(MP_EXTRA_AS_PRE)  -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1 -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"E:/Projekty/Inz/IntenseS.X/os/port/MPLAB/PIC32MX/" -MMD -MF "${OBJECTDIR}/hal/lld/platforms/microchip/pic32IRIsr.o.d"  -o ${OBJECTDIR}/hal/lld/platforms/microchip/pic32IRIsr.o hal/lld/platforms/microchip/pic32IRIsr.S  -Wa,--defsym=__MPLAB_BUILD=1$(MP_EXTRA_AS_POST),-MD="${OBJECTDIR}/hal/lld/platforms/microchip/pic32IRIsr.o.asm.d",--defsym=__MPLAB_DEBUG=1,--defsym=__ICD2RAM=1,--gdwarf-2,--defsym=__DEBUG=1,--defsym=__MPLAB_DEBUGGER_ICD3=1,-I"E:/Projekty/Inz/IntenseS.X/os/",-ah
	
${OBJECTDIR}/hal/lld/platforms/microchip/pic32IRTimerIsr.o: hal/lld/platforms/microchip/pic32IRTimerIsr.S  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/hal/lld/platforms/microchip 
	@${RM} ${OBJECTDIR}/hal/lld/platforms/microchip/pic32IRTimerIsr.o.d 
	@${RM} ${OBJECTDIR}/hal/lld/platforms/microchip/pic32IRTimerIsr.o.ok ${OBJECTDIR}/hal/lld/platforms/microchip/pic32IRTimerIsr.o.err 
	@${FIXDEPS} "${OBJECTDIR}/hal/lld/platforms/microchip/pic32IRTimerIsr.o.d" "${OBJECTDIR}/hal/lld/platforms/microchip/pic32IRTimerIsr.o.asm.d" -t $(SILENT) -c ${MP_CC} $(MP_EXTRA_AS_PRE)  -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1 -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"E:/Projekty/Inz/IntenseS.X/os/port/MPLAB/PIC32MX/" -MMD -MF "${OBJECTDIR}/hal/lld/platforms/microchip/pic32IRTimerIsr.o.d"  -o ${OBJECTDIR}/hal/lld/platforms/microchip/pic32IRTimerIsr.o hal/lld/platforms/microchip/pic32IRTimerIsr.S  -Wa,--defsym=__MPLAB_BUILD=1$(MP_EXTRA_AS_POST),-MD="${OBJECTDIR}/hal/lld/platforms/microchip/pic32IRTimerIsr.o.asm.d",--defsym=__MPLAB_DEBUG=1,--defsym=__ICD2RAM=1,--gdwarf-2,--defsym=__DEBUG=1,--defsym=__MPLAB_DEBUGGER_ICD3=1,-I"E:/Projekty/Inz/IntenseS.X/os/",-ah
	
${OBJECTDIR}/hal/lld/platforms/microchip/wm8731ISR.o: hal/lld/platforms/microchip/wm8731ISR.S  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/hal/lld/platforms/microchip 
	@${RM} ${OBJECTDIR}/hal/lld/platforms/microchip/wm8731ISR.o.d 
	@${RM} ${OBJECTDIR}/hal/lld/platforms/microchip/wm8731ISR.o.ok ${OBJECTDIR}/hal/lld/platforms/microchip/wm8731ISR.o.err 
	@${FIXDEPS} "${OBJECTDIR}/hal/lld/platforms/microchip/wm8731ISR.o.d" "${OBJECTDIR}/hal/lld/platforms/microchip/wm8731ISR.o.asm.d" -t $(SILENT) -c ${MP_CC} $(MP_EXTRA_AS_PRE)  -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1 -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"E:/Projekty/Inz/IntenseS.X/os/port/MPLAB/PIC32MX/" -MMD -MF "${OBJECTDIR}/hal/lld/platforms/microchip/wm8731ISR.o.d"  -o ${OBJECTDIR}/hal/lld/platforms/microchip/wm8731ISR.o hal/lld/platforms/microchip/wm8731ISR.S  -Wa,--defsym=__MPLAB_BUILD=1$(MP_EXTRA_AS_POST),-MD="${OBJECTDIR}/hal/lld/platforms/microchip/wm8731ISR.o.asm.d",--defsym=__MPLAB_DEBUG=1,--defsym=__ICD2RAM=1,--gdwarf-2,--defsym=__DEBUG=1,--defsym=__MPLAB_DEBUGGER_ICD3=1,-I"E:/Projekty/Inz/IntenseS.X/os/",-ah
	
${OBJECTDIR}/os/port/MPLAB/PIC32MX/port_asm.o: os/port/MPLAB/PIC32MX/port_asm.S  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/os/port/MPLAB/PIC32MX 
	@${RM} ${OBJECTDIR}/os/port/MPLAB/PIC32MX/port_asm.o.d 
	@${RM} ${OBJECTDIR}/os/port/MPLAB/PIC32MX/port_asm.o.ok ${OBJECTDIR}/os/port/MPLAB/PIC32MX/port_asm.o.err 
	@${FIXDEPS} "${OBJECTDIR}/os/port/MPLAB/PIC32MX/port_asm.o.d" "${OBJECTDIR}/os/port/MPLAB/PIC32MX/port_asm.o.asm.d" -t $(SILENT) -c ${MP_CC} $(MP_EXTRA_AS_PRE)  -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1 -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"E:/Projekty/Inz/IntenseS.X/os/port/MPLAB/PIC32MX/" -MMD -MF "${OBJECTDIR}/os/port/MPLAB/PIC32MX/port_asm.o.d"  -o ${OBJECTDIR}/os/port/MPLAB/PIC32MX/port_asm.o os/port/MPLAB/PIC32MX/port_asm.S  -Wa,--defsym=__MPLAB_BUILD=1$(MP_EXTRA_AS_POST),-MD="${OBJECTDIR}/os/port/MPLAB/PIC32MX/port_asm.o.asm.d",--defsym=__MPLAB_DEBUG=1,--defsym=__ICD2RAM=1,--gdwarf-2,--defsym=__DEBUG=1,--defsym=__MPLAB_DEBUGGER_ICD3=1,-I"E:/Projekty/Inz/IntenseS.X/os/",-ah
	
else
${OBJECTDIR}/hal/lld/platforms/microchip/pic32UARTIsr.o: hal/lld/platforms/microchip/pic32UARTIsr.S  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/hal/lld/platforms/microchip 
	@${RM} ${OBJECTDIR}/hal/lld/platforms/microchip/pic32UARTIsr.o.d 
	@${RM} ${OBJECTDIR}/hal/lld/platforms/microchip/pic32UARTIsr.o.ok ${OBJECTDIR}/hal/lld/platforms/microchip/pic32UARTIsr.o.err 
	@${FIXDEPS} "${OBJECTDIR}/hal/lld/platforms/microchip/pic32UARTIsr.o.d" "${OBJECTDIR}/hal/lld/platforms/microchip/pic32UARTIsr.o.asm.d" -t $(SILENT) -c ${MP_CC} $(MP_EXTRA_AS_PRE)  -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"E:/Projekty/Inz/IntenseS.X/os/port/MPLAB/PIC32MX/" -MMD -MF "${OBJECTDIR}/hal/lld/platforms/microchip/pic32UARTIsr.o.d"  -o ${OBJECTDIR}/hal/lld/platforms/microchip/pic32UARTIsr.o hal/lld/platforms/microchip/pic32UARTIsr.S  -Wa,--defsym=__MPLAB_BUILD=1$(MP_EXTRA_AS_POST),-MD="${OBJECTDIR}/hal/lld/platforms/microchip/pic32UARTIsr.o.asm.d",--gdwarf-2,-I"E:/Projekty/Inz/IntenseS.X/os/",-ah
	
${OBJECTDIR}/hal/lld/platforms/microchip/pic32ADCIsr.o: hal/lld/platforms/microchip/pic32ADCIsr.S  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/hal/lld/platforms/microchip 
	@${RM} ${OBJECTDIR}/hal/lld/platforms/microchip/pic32ADCIsr.o.d 
	@${RM} ${OBJECTDIR}/hal/lld/platforms/microchip/pic32ADCIsr.o.ok ${OBJECTDIR}/hal/lld/platforms/microchip/pic32ADCIsr.o.err 
	@${FIXDEPS} "${OBJECTDIR}/hal/lld/platforms/microchip/pic32ADCIsr.o.d" "${OBJECTDIR}/hal/lld/platforms/microchip/pic32ADCIsr.o.asm.d" -t $(SILENT) -c ${MP_CC} $(MP_EXTRA_AS_PRE)  -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"E:/Projekty/Inz/IntenseS.X/os/port/MPLAB/PIC32MX/" -MMD -MF "${OBJECTDIR}/hal/lld/platforms/microchip/pic32ADCIsr.o.d"  -o ${OBJECTDIR}/hal/lld/platforms/microchip/pic32ADCIsr.o hal/lld/platforms/microchip/pic32ADCIsr.S  -Wa,--defsym=__MPLAB_BUILD=1$(MP_EXTRA_AS_POST),-MD="${OBJECTDIR}/hal/lld/platforms/microchip/pic32ADCIsr.o.asm.d",--gdwarf-2,-I"E:/Projekty/Inz/IntenseS.X/os/",-ah
	
${OBJECTDIR}/hal/lld/platforms/microchip/pic32IRIsr.o: hal/lld/platforms/microchip/pic32IRIsr.S  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/hal/lld/platforms/microchip 
	@${RM} ${OBJECTDIR}/hal/lld/platforms/microchip/pic32IRIsr.o.d 
	@${RM} ${OBJECTDIR}/hal/lld/platforms/microchip/pic32IRIsr.o.ok ${OBJECTDIR}/hal/lld/platforms/microchip/pic32IRIsr.o.err 
	@${FIXDEPS} "${OBJECTDIR}/hal/lld/platforms/microchip/pic32IRIsr.o.d" "${OBJECTDIR}/hal/lld/platforms/microchip/pic32IRIsr.o.asm.d" -t $(SILENT) -c ${MP_CC} $(MP_EXTRA_AS_PRE)  -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"E:/Projekty/Inz/IntenseS.X/os/port/MPLAB/PIC32MX/" -MMD -MF "${OBJECTDIR}/hal/lld/platforms/microchip/pic32IRIsr.o.d"  -o ${OBJECTDIR}/hal/lld/platforms/microchip/pic32IRIsr.o hal/lld/platforms/microchip/pic32IRIsr.S  -Wa,--defsym=__MPLAB_BUILD=1$(MP_EXTRA_AS_POST),-MD="${OBJECTDIR}/hal/lld/platforms/microchip/pic32IRIsr.o.asm.d",--gdwarf-2,-I"E:/Projekty/Inz/IntenseS.X/os/",-ah
	
${OBJECTDIR}/hal/lld/platforms/microchip/pic32IRTimerIsr.o: hal/lld/platforms/microchip/pic32IRTimerIsr.S  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/hal/lld/platforms/microchip 
	@${RM} ${OBJECTDIR}/hal/lld/platforms/microchip/pic32IRTimerIsr.o.d 
	@${RM} ${OBJECTDIR}/hal/lld/platforms/microchip/pic32IRTimerIsr.o.ok ${OBJECTDIR}/hal/lld/platforms/microchip/pic32IRTimerIsr.o.err 
	@${FIXDEPS} "${OBJECTDIR}/hal/lld/platforms/microchip/pic32IRTimerIsr.o.d" "${OBJECTDIR}/hal/lld/platforms/microchip/pic32IRTimerIsr.o.asm.d" -t $(SILENT) -c ${MP_CC} $(MP_EXTRA_AS_PRE)  -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"E:/Projekty/Inz/IntenseS.X/os/port/MPLAB/PIC32MX/" -MMD -MF "${OBJECTDIR}/hal/lld/platforms/microchip/pic32IRTimerIsr.o.d"  -o ${OBJECTDIR}/hal/lld/platforms/microchip/pic32IRTimerIsr.o hal/lld/platforms/microchip/pic32IRTimerIsr.S  -Wa,--defsym=__MPLAB_BUILD=1$(MP_EXTRA_AS_POST),-MD="${OBJECTDIR}/hal/lld/platforms/microchip/pic32IRTimerIsr.o.asm.d",--gdwarf-2,-I"E:/Projekty/Inz/IntenseS.X/os/",-ah
	
${OBJECTDIR}/hal/lld/platforms/microchip/wm8731ISR.o: hal/lld/platforms/microchip/wm8731ISR.S  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/hal/lld/platforms/microchip 
	@${RM} ${OBJECTDIR}/hal/lld/platforms/microchip/wm8731ISR.o.d 
	@${RM} ${OBJECTDIR}/hal/lld/platforms/microchip/wm8731ISR.o.ok ${OBJECTDIR}/hal/lld/platforms/microchip/wm8731ISR.o.err 
	@${FIXDEPS} "${OBJECTDIR}/hal/lld/platforms/microchip/wm8731ISR.o.d" "${OBJECTDIR}/hal/lld/platforms/microchip/wm8731ISR.o.asm.d" -t $(SILENT) -c ${MP_CC} $(MP_EXTRA_AS_PRE)  -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"E:/Projekty/Inz/IntenseS.X/os/port/MPLAB/PIC32MX/" -MMD -MF "${OBJECTDIR}/hal/lld/platforms/microchip/wm8731ISR.o.d"  -o ${OBJECTDIR}/hal/lld/platforms/microchip/wm8731ISR.o hal/lld/platforms/microchip/wm8731ISR.S  -Wa,--defsym=__MPLAB_BUILD=1$(MP_EXTRA_AS_POST),-MD="${OBJECTDIR}/hal/lld/platforms/microchip/wm8731ISR.o.asm.d",--gdwarf-2,-I"E:/Projekty/Inz/IntenseS.X/os/",-ah
	
${OBJECTDIR}/os/port/MPLAB/PIC32MX/port_asm.o: os/port/MPLAB/PIC32MX/port_asm.S  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/os/port/MPLAB/PIC32MX 
	@${RM} ${OBJECTDIR}/os/port/MPLAB/PIC32MX/port_asm.o.d 
	@${RM} ${OBJECTDIR}/os/port/MPLAB/PIC32MX/port_asm.o.ok ${OBJECTDIR}/os/port/MPLAB/PIC32MX/port_asm.o.err 
	@${FIXDEPS} "${OBJECTDIR}/os/port/MPLAB/PIC32MX/port_asm.o.d" "${OBJECTDIR}/os/port/MPLAB/PIC32MX/port_asm.o.asm.d" -t $(SILENT) -c ${MP_CC} $(MP_EXTRA_AS_PRE)  -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"E:/Projekty/Inz/IntenseS.X/os/port/MPLAB/PIC32MX/" -MMD -MF "${OBJECTDIR}/os/port/MPLAB/PIC32MX/port_asm.o.d"  -o ${OBJECTDIR}/os/port/MPLAB/PIC32MX/port_asm.o os/port/MPLAB/PIC32MX/port_asm.S  -Wa,--defsym=__MPLAB_BUILD=1$(MP_EXTRA_AS_POST),-MD="${OBJECTDIR}/os/port/MPLAB/PIC32MX/port_asm.o.asm.d",--gdwarf-2,-I"E:/Projekty/Inz/IntenseS.X/os/",-ah
	
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: compile
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
${OBJECTDIR}/app/main.o: app/main.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/app 
	@${RM} ${OBJECTDIR}/app/main.o.d 
	@${FIXDEPS} "${OBJECTDIR}/app/main.o.d" $(SILENT) -c ${MP_CC} $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1 -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"E:/Projekty/Inz/IntenseS.X/os/port/MPLAB/PIC32MX" -I"E:/Projekty/Inz/IntenseS.X/os" -I"E:/Projekty/Inz/IntenseS.X/boards/PIC32MX7MMB" -I"E:/Projekty/Inz/IntenseS.X" -O1 -MMD -MF "${OBJECTDIR}/app/main.o.d" -o ${OBJECTDIR}/app/main.o app/main.c  
	
${OBJECTDIR}/boards/PIC32MX7MMB/board.o: boards/PIC32MX7MMB/board.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/boards/PIC32MX7MMB 
	@${RM} ${OBJECTDIR}/boards/PIC32MX7MMB/board.o.d 
	@${FIXDEPS} "${OBJECTDIR}/boards/PIC32MX7MMB/board.o.d" $(SILENT) -c ${MP_CC} $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1 -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"E:/Projekty/Inz/IntenseS.X/os/port/MPLAB/PIC32MX" -I"E:/Projekty/Inz/IntenseS.X/os" -I"E:/Projekty/Inz/IntenseS.X/boards/PIC32MX7MMB" -I"E:/Projekty/Inz/IntenseS.X" -O1 -MMD -MF "${OBJECTDIR}/boards/PIC32MX7MMB/board.o.d" -o ${OBJECTDIR}/boards/PIC32MX7MMB/board.o boards/PIC32MX7MMB/board.c  
	
${OBJECTDIR}/hal/hld/hld.o: hal/hld/hld.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/hal/hld 
	@${RM} ${OBJECTDIR}/hal/hld/hld.o.d 
	@${FIXDEPS} "${OBJECTDIR}/hal/hld/hld.o.d" $(SILENT) -c ${MP_CC} $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1 -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"E:/Projekty/Inz/IntenseS.X/os/port/MPLAB/PIC32MX" -I"E:/Projekty/Inz/IntenseS.X/os" -I"E:/Projekty/Inz/IntenseS.X/boards/PIC32MX7MMB" -I"E:/Projekty/Inz/IntenseS.X" -O1 -MMD -MF "${OBJECTDIR}/hal/hld/hld.o.d" -o ${OBJECTDIR}/hal/hld/hld.o hal/hld/hld.c  
	
${OBJECTDIR}/hal/lld/platforms/microchip/gpio.o: hal/lld/platforms/microchip/gpio.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/hal/lld/platforms/microchip 
	@${RM} ${OBJECTDIR}/hal/lld/platforms/microchip/gpio.o.d 
	@${FIXDEPS} "${OBJECTDIR}/hal/lld/platforms/microchip/gpio.o.d" $(SILENT) -c ${MP_CC} $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1 -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"E:/Projekty/Inz/IntenseS.X/os/port/MPLAB/PIC32MX" -I"E:/Projekty/Inz/IntenseS.X/os" -I"E:/Projekty/Inz/IntenseS.X/boards/PIC32MX7MMB" -I"E:/Projekty/Inz/IntenseS.X" -O1 -MMD -MF "${OBJECTDIR}/hal/lld/platforms/microchip/gpio.o.d" -o ${OBJECTDIR}/hal/lld/platforms/microchip/gpio.o hal/lld/platforms/microchip/gpio.c  
	
${OBJECTDIR}/hal/lld/platforms/microchip/ili9163.o: hal/lld/platforms/microchip/ili9163.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/hal/lld/platforms/microchip 
	@${RM} ${OBJECTDIR}/hal/lld/platforms/microchip/ili9163.o.d 
	@${FIXDEPS} "${OBJECTDIR}/hal/lld/platforms/microchip/ili9163.o.d" $(SILENT) -c ${MP_CC} $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1 -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"E:/Projekty/Inz/IntenseS.X/os/port/MPLAB/PIC32MX" -I"E:/Projekty/Inz/IntenseS.X/os" -I"E:/Projekty/Inz/IntenseS.X/boards/PIC32MX7MMB" -I"E:/Projekty/Inz/IntenseS.X" -O1 -MMD -MF "${OBJECTDIR}/hal/lld/platforms/microchip/ili9163.o.d" -o ${OBJECTDIR}/hal/lld/platforms/microchip/ili9163.o hal/lld/platforms/microchip/ili9163.c  
	
${OBJECTDIR}/hal/lld/platforms/microchip/hx8347.o: hal/lld/platforms/microchip/hx8347.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/hal/lld/platforms/microchip 
	@${RM} ${OBJECTDIR}/hal/lld/platforms/microchip/hx8347.o.d 
	@${FIXDEPS} "${OBJECTDIR}/hal/lld/platforms/microchip/hx8347.o.d" $(SILENT) -c ${MP_CC} $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1 -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"E:/Projekty/Inz/IntenseS.X/os/port/MPLAB/PIC32MX" -I"E:/Projekty/Inz/IntenseS.X/os" -I"E:/Projekty/Inz/IntenseS.X/boards/PIC32MX7MMB" -I"E:/Projekty/Inz/IntenseS.X" -O1 -MMD -MF "${OBJECTDIR}/hal/lld/platforms/microchip/hx8347.o.d" -o ${OBJECTDIR}/hal/lld/platforms/microchip/hx8347.o hal/lld/platforms/microchip/hx8347.c  
	
${OBJECTDIR}/hal/lld/platforms/microchip/pic32UART.o: hal/lld/platforms/microchip/pic32UART.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/hal/lld/platforms/microchip 
	@${RM} ${OBJECTDIR}/hal/lld/platforms/microchip/pic32UART.o.d 
	@${FIXDEPS} "${OBJECTDIR}/hal/lld/platforms/microchip/pic32UART.o.d" $(SILENT) -c ${MP_CC} $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1 -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"E:/Projekty/Inz/IntenseS.X/os/port/MPLAB/PIC32MX" -I"E:/Projekty/Inz/IntenseS.X/os" -I"E:/Projekty/Inz/IntenseS.X/boards/PIC32MX7MMB" -I"E:/Projekty/Inz/IntenseS.X" -O1 -MMD -MF "${OBJECTDIR}/hal/lld/platforms/microchip/pic32UART.o.d" -o ${OBJECTDIR}/hal/lld/platforms/microchip/pic32UART.o hal/lld/platforms/microchip/pic32UART.c  
	
${OBJECTDIR}/hal/lld/platforms/microchip/pic32ADC.o: hal/lld/platforms/microchip/pic32ADC.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/hal/lld/platforms/microchip 
	@${RM} ${OBJECTDIR}/hal/lld/platforms/microchip/pic32ADC.o.d 
	@${FIXDEPS} "${OBJECTDIR}/hal/lld/platforms/microchip/pic32ADC.o.d" $(SILENT) -c ${MP_CC} $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1 -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"E:/Projekty/Inz/IntenseS.X/os/port/MPLAB/PIC32MX" -I"E:/Projekty/Inz/IntenseS.X/os" -I"E:/Projekty/Inz/IntenseS.X/boards/PIC32MX7MMB" -I"E:/Projekty/Inz/IntenseS.X" -O1 -MMD -MF "${OBJECTDIR}/hal/lld/platforms/microchip/pic32ADC.o.d" -o ${OBJECTDIR}/hal/lld/platforms/microchip/pic32ADC.o hal/lld/platforms/microchip/pic32ADC.c  
	
${OBJECTDIR}/hal/lld/platforms/microchip/pic32IR.o: hal/lld/platforms/microchip/pic32IR.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/hal/lld/platforms/microchip 
	@${RM} ${OBJECTDIR}/hal/lld/platforms/microchip/pic32IR.o.d 
	@${FIXDEPS} "${OBJECTDIR}/hal/lld/platforms/microchip/pic32IR.o.d" $(SILENT) -c ${MP_CC} $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1 -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"E:/Projekty/Inz/IntenseS.X/os/port/MPLAB/PIC32MX" -I"E:/Projekty/Inz/IntenseS.X/os" -I"E:/Projekty/Inz/IntenseS.X/boards/PIC32MX7MMB" -I"E:/Projekty/Inz/IntenseS.X" -O1 -MMD -MF "${OBJECTDIR}/hal/lld/platforms/microchip/pic32IR.o.d" -o ${OBJECTDIR}/hal/lld/platforms/microchip/pic32IR.o hal/lld/platforms/microchip/pic32IR.c  
	
${OBJECTDIR}/hal/lld/platforms/microchip/wm8731.o: hal/lld/platforms/microchip/wm8731.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/hal/lld/platforms/microchip 
	@${RM} ${OBJECTDIR}/hal/lld/platforms/microchip/wm8731.o.d 
	@${FIXDEPS} "${OBJECTDIR}/hal/lld/platforms/microchip/wm8731.o.d" $(SILENT) -c ${MP_CC} $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1 -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"E:/Projekty/Inz/IntenseS.X/os/port/MPLAB/PIC32MX" -I"E:/Projekty/Inz/IntenseS.X/os" -I"E:/Projekty/Inz/IntenseS.X/boards/PIC32MX7MMB" -I"E:/Projekty/Inz/IntenseS.X" -O1 -MMD -MF "${OBJECTDIR}/hal/lld/platforms/microchip/wm8731.o.d" -o ${OBJECTDIR}/hal/lld/platforms/microchip/wm8731.o hal/lld/platforms/microchip/wm8731.c  
	
${OBJECTDIR}/hal/lld/platforms/microchip/pic32SDMMC.o: hal/lld/platforms/microchip/pic32SDMMC.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/hal/lld/platforms/microchip 
	@${RM} ${OBJECTDIR}/hal/lld/platforms/microchip/pic32SDMMC.o.d 
	@${FIXDEPS} "${OBJECTDIR}/hal/lld/platforms/microchip/pic32SDMMC.o.d" $(SILENT) -c ${MP_CC} $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1 -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"E:/Projekty/Inz/IntenseS.X/os/port/MPLAB/PIC32MX" -I"E:/Projekty/Inz/IntenseS.X/os" -I"E:/Projekty/Inz/IntenseS.X/boards/PIC32MX7MMB" -I"E:/Projekty/Inz/IntenseS.X" -O1 -MMD -MF "${OBJECTDIR}/hal/lld/platforms/microchip/pic32SDMMC.o.d" -o ${OBJECTDIR}/hal/lld/platforms/microchip/pic32SDMMC.o hal/lld/platforms/microchip/pic32SDMMC.c  
	
${OBJECTDIR}/hal/lld/resistiveTouch.o: hal/lld/resistiveTouch.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/hal/lld 
	@${RM} ${OBJECTDIR}/hal/lld/resistiveTouch.o.d 
	@${FIXDEPS} "${OBJECTDIR}/hal/lld/resistiveTouch.o.d" $(SILENT) -c ${MP_CC} $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1 -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"E:/Projekty/Inz/IntenseS.X/os/port/MPLAB/PIC32MX" -I"E:/Projekty/Inz/IntenseS.X/os" -I"E:/Projekty/Inz/IntenseS.X/boards/PIC32MX7MMB" -I"E:/Projekty/Inz/IntenseS.X" -O1 -MMD -MF "${OBJECTDIR}/hal/lld/resistiveTouch.o.d" -o ${OBJECTDIR}/hal/lld/resistiveTouch.o hal/lld/resistiveTouch.c  
	
${OBJECTDIR}/lib/audio/decoders/wave.o: lib/audio/decoders/wave.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/lib/audio/decoders 
	@${RM} ${OBJECTDIR}/lib/audio/decoders/wave.o.d 
	@${FIXDEPS} "${OBJECTDIR}/lib/audio/decoders/wave.o.d" $(SILENT) -c ${MP_CC} $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1 -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"E:/Projekty/Inz/IntenseS.X/os/port/MPLAB/PIC32MX" -I"E:/Projekty/Inz/IntenseS.X/os" -I"E:/Projekty/Inz/IntenseS.X/boards/PIC32MX7MMB" -I"E:/Projekty/Inz/IntenseS.X" -O1 -MMD -MF "${OBJECTDIR}/lib/audio/decoders/wave.o.d" -o ${OBJECTDIR}/lib/audio/decoders/wave.o lib/audio/decoders/wave.c  
	
${OBJECTDIR}/lib/audio/audioSineOutput.o: lib/audio/audioSineOutput.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/lib/audio 
	@${RM} ${OBJECTDIR}/lib/audio/audioSineOutput.o.d 
	@${FIXDEPS} "${OBJECTDIR}/lib/audio/audioSineOutput.o.d" $(SILENT) -c ${MP_CC} $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1 -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"E:/Projekty/Inz/IntenseS.X/os/port/MPLAB/PIC32MX" -I"E:/Projekty/Inz/IntenseS.X/os" -I"E:/Projekty/Inz/IntenseS.X/boards/PIC32MX7MMB" -I"E:/Projekty/Inz/IntenseS.X" -O1 -MMD -MF "${OBJECTDIR}/lib/audio/audioSineOutput.o.d" -o ${OBJECTDIR}/lib/audio/audioSineOutput.o lib/audio/audioSineOutput.c  
	
${OBJECTDIR}/lib/audio/audioSinLUT.o: lib/audio/audioSinLUT.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/lib/audio 
	@${RM} ${OBJECTDIR}/lib/audio/audioSinLUT.o.d 
	@${FIXDEPS} "${OBJECTDIR}/lib/audio/audioSinLUT.o.d" $(SILENT) -c ${MP_CC} $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1 -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"E:/Projekty/Inz/IntenseS.X/os/port/MPLAB/PIC32MX" -I"E:/Projekty/Inz/IntenseS.X/os" -I"E:/Projekty/Inz/IntenseS.X/boards/PIC32MX7MMB" -I"E:/Projekty/Inz/IntenseS.X" -O1 -MMD -MF "${OBJECTDIR}/lib/audio/audioSinLUT.o.d" -o ${OBJECTDIR}/lib/audio/audioSinLUT.o lib/audio/audioSinLUT.c  
	
${OBJECTDIR}/lib/audio/audioConsole.o: lib/audio/audioConsole.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/lib/audio 
	@${RM} ${OBJECTDIR}/lib/audio/audioConsole.o.d 
	@${FIXDEPS} "${OBJECTDIR}/lib/audio/audioConsole.o.d" $(SILENT) -c ${MP_CC} $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1 -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"E:/Projekty/Inz/IntenseS.X/os/port/MPLAB/PIC32MX" -I"E:/Projekty/Inz/IntenseS.X/os" -I"E:/Projekty/Inz/IntenseS.X/boards/PIC32MX7MMB" -I"E:/Projekty/Inz/IntenseS.X" -O1 -MMD -MF "${OBJECTDIR}/lib/audio/audioConsole.o.d" -o ${OBJECTDIR}/lib/audio/audioConsole.o lib/audio/audioConsole.c  
	
${OBJECTDIR}/lib/audio/audio1ch.o: lib/audio/audio1ch.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/lib/audio 
	@${RM} ${OBJECTDIR}/lib/audio/audio1ch.o.d 
	@${FIXDEPS} "${OBJECTDIR}/lib/audio/audio1ch.o.d" $(SILENT) -c ${MP_CC} $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1 -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"E:/Projekty/Inz/IntenseS.X/os/port/MPLAB/PIC32MX" -I"E:/Projekty/Inz/IntenseS.X/os" -I"E:/Projekty/Inz/IntenseS.X/boards/PIC32MX7MMB" -I"E:/Projekty/Inz/IntenseS.X" -O1 -MMD -MF "${OBJECTDIR}/lib/audio/audio1ch.o.d" -o ${OBJECTDIR}/lib/audio/audio1ch.o lib/audio/audio1ch.c  
	
${OBJECTDIR}/lib/console/serialConsole.o: lib/console/serialConsole.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/lib/console 
	@${RM} ${OBJECTDIR}/lib/console/serialConsole.o.d 
	@${FIXDEPS} "${OBJECTDIR}/lib/console/serialConsole.o.d" $(SILENT) -c ${MP_CC} $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1 -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"E:/Projekty/Inz/IntenseS.X/os/port/MPLAB/PIC32MX" -I"E:/Projekty/Inz/IntenseS.X/os" -I"E:/Projekty/Inz/IntenseS.X/boards/PIC32MX7MMB" -I"E:/Projekty/Inz/IntenseS.X" -O1 -MMD -MF "${OBJECTDIR}/lib/console/serialConsole.o.d" -o ${OBJECTDIR}/lib/console/serialConsole.o lib/console/serialConsole.c  
	
${OBJECTDIR}/lib/fatfs/option/syscall.o: lib/fatfs/option/syscall.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/lib/fatfs/option 
	@${RM} ${OBJECTDIR}/lib/fatfs/option/syscall.o.d 
	@${FIXDEPS} "${OBJECTDIR}/lib/fatfs/option/syscall.o.d" $(SILENT) -c ${MP_CC} $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1 -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"E:/Projekty/Inz/IntenseS.X/os/port/MPLAB/PIC32MX" -I"E:/Projekty/Inz/IntenseS.X/os" -I"E:/Projekty/Inz/IntenseS.X/boards/PIC32MX7MMB" -I"E:/Projekty/Inz/IntenseS.X" -O1 -MMD -MF "${OBJECTDIR}/lib/fatfs/option/syscall.o.d" -o ${OBJECTDIR}/lib/fatfs/option/syscall.o lib/fatfs/option/syscall.c  
	
${OBJECTDIR}/lib/fatfs/option/unicode.o: lib/fatfs/option/unicode.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/lib/fatfs/option 
	@${RM} ${OBJECTDIR}/lib/fatfs/option/unicode.o.d 
	@${FIXDEPS} "${OBJECTDIR}/lib/fatfs/option/unicode.o.d" $(SILENT) -c ${MP_CC} $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1 -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"E:/Projekty/Inz/IntenseS.X/os/port/MPLAB/PIC32MX" -I"E:/Projekty/Inz/IntenseS.X/os" -I"E:/Projekty/Inz/IntenseS.X/boards/PIC32MX7MMB" -I"E:/Projekty/Inz/IntenseS.X" -O1 -MMD -MF "${OBJECTDIR}/lib/fatfs/option/unicode.o.d" -o ${OBJECTDIR}/lib/fatfs/option/unicode.o lib/fatfs/option/unicode.c  
	
${OBJECTDIR}/lib/fatfs/diskio.o: lib/fatfs/diskio.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/lib/fatfs 
	@${RM} ${OBJECTDIR}/lib/fatfs/diskio.o.d 
	@${FIXDEPS} "${OBJECTDIR}/lib/fatfs/diskio.o.d" $(SILENT) -c ${MP_CC} $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1 -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"E:/Projekty/Inz/IntenseS.X/os/port/MPLAB/PIC32MX" -I"E:/Projekty/Inz/IntenseS.X/os" -I"E:/Projekty/Inz/IntenseS.X/boards/PIC32MX7MMB" -I"E:/Projekty/Inz/IntenseS.X" -O1 -MMD -MF "${OBJECTDIR}/lib/fatfs/diskio.o.d" -o ${OBJECTDIR}/lib/fatfs/diskio.o lib/fatfs/diskio.c  
	
${OBJECTDIR}/lib/fatfs/ff.o: lib/fatfs/ff.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/lib/fatfs 
	@${RM} ${OBJECTDIR}/lib/fatfs/ff.o.d 
	@${FIXDEPS} "${OBJECTDIR}/lib/fatfs/ff.o.d" $(SILENT) -c ${MP_CC} $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1 -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"E:/Projekty/Inz/IntenseS.X/os/port/MPLAB/PIC32MX" -I"E:/Projekty/Inz/IntenseS.X/os" -I"E:/Projekty/Inz/IntenseS.X/boards/PIC32MX7MMB" -I"E:/Projekty/Inz/IntenseS.X" -O1 -MMD -MF "${OBJECTDIR}/lib/fatfs/ff.o.d" -o ${OBJECTDIR}/lib/fatfs/ff.o lib/fatfs/ff.c  
	
${OBJECTDIR}/lib/fatfs/fatfsConsole.o: lib/fatfs/fatfsConsole.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/lib/fatfs 
	@${RM} ${OBJECTDIR}/lib/fatfs/fatfsConsole.o.d 
	@${FIXDEPS} "${OBJECTDIR}/lib/fatfs/fatfsConsole.o.d" $(SILENT) -c ${MP_CC} $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1 -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"E:/Projekty/Inz/IntenseS.X/os/port/MPLAB/PIC32MX" -I"E:/Projekty/Inz/IntenseS.X/os" -I"E:/Projekty/Inz/IntenseS.X/boards/PIC32MX7MMB" -I"E:/Projekty/Inz/IntenseS.X" -O1 -MMD -MF "${OBJECTDIR}/lib/fatfs/fatfsConsole.o.d" -o ${OBJECTDIR}/lib/fatfs/fatfsConsole.o lib/fatfs/fatfsConsole.c  
	
${OBJECTDIR}/lib/graphics/graphics.o: lib/graphics/graphics.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/lib/graphics 
	@${RM} ${OBJECTDIR}/lib/graphics/graphics.o.d 
	@${FIXDEPS} "${OBJECTDIR}/lib/graphics/graphics.o.d" $(SILENT) -c ${MP_CC} $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1 -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"E:/Projekty/Inz/IntenseS.X/os/port/MPLAB/PIC32MX" -I"E:/Projekty/Inz/IntenseS.X/os" -I"E:/Projekty/Inz/IntenseS.X/boards/PIC32MX7MMB" -I"E:/Projekty/Inz/IntenseS.X" -O1 -MMD -MF "${OBJECTDIR}/lib/graphics/graphics.o.d" -o ${OBJECTDIR}/lib/graphics/graphics.o lib/graphics/graphics.c  
	
${OBJECTDIR}/lib/graphics/primitives.o: lib/graphics/primitives.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/lib/graphics 
	@${RM} ${OBJECTDIR}/lib/graphics/primitives.o.d 
	@${FIXDEPS} "${OBJECTDIR}/lib/graphics/primitives.o.d" $(SILENT) -c ${MP_CC} $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1 -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"E:/Projekty/Inz/IntenseS.X/os/port/MPLAB/PIC32MX" -I"E:/Projekty/Inz/IntenseS.X/os" -I"E:/Projekty/Inz/IntenseS.X/boards/PIC32MX7MMB" -I"E:/Projekty/Inz/IntenseS.X" -O1 -MMD -MF "${OBJECTDIR}/lib/graphics/primitives.o.d" -o ${OBJECTDIR}/lib/graphics/primitives.o lib/graphics/primitives.c  
	
${OBJECTDIR}/lib/graphics/tests.o: lib/graphics/tests.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/lib/graphics 
	@${RM} ${OBJECTDIR}/lib/graphics/tests.o.d 
	@${FIXDEPS} "${OBJECTDIR}/lib/graphics/tests.o.d" $(SILENT) -c ${MP_CC} $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1 -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"E:/Projekty/Inz/IntenseS.X/os/port/MPLAB/PIC32MX" -I"E:/Projekty/Inz/IntenseS.X/os" -I"E:/Projekty/Inz/IntenseS.X/boards/PIC32MX7MMB" -I"E:/Projekty/Inz/IntenseS.X" -O1 -MMD -MF "${OBJECTDIR}/lib/graphics/tests.o.d" -o ${OBJECTDIR}/lib/graphics/tests.o lib/graphics/tests.c  
	
${OBJECTDIR}/lib/gui/window.o: lib/gui/window.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/lib/gui 
	@${RM} ${OBJECTDIR}/lib/gui/window.o.d 
	@${FIXDEPS} "${OBJECTDIR}/lib/gui/window.o.d" $(SILENT) -c ${MP_CC} $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1 -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"E:/Projekty/Inz/IntenseS.X/os/port/MPLAB/PIC32MX" -I"E:/Projekty/Inz/IntenseS.X/os" -I"E:/Projekty/Inz/IntenseS.X/boards/PIC32MX7MMB" -I"E:/Projekty/Inz/IntenseS.X" -O1 -MMD -MF "${OBJECTDIR}/lib/gui/window.o.d" -o ${OBJECTDIR}/lib/gui/window.o lib/gui/window.c  
	
${OBJECTDIR}/lib/input/inputEvent.o: lib/input/inputEvent.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/lib/input 
	@${RM} ${OBJECTDIR}/lib/input/inputEvent.o.d 
	@${FIXDEPS} "${OBJECTDIR}/lib/input/inputEvent.o.d" $(SILENT) -c ${MP_CC} $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1 -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"E:/Projekty/Inz/IntenseS.X/os/port/MPLAB/PIC32MX" -I"E:/Projekty/Inz/IntenseS.X/os" -I"E:/Projekty/Inz/IntenseS.X/boards/PIC32MX7MMB" -I"E:/Projekty/Inz/IntenseS.X" -O1 -MMD -MF "${OBJECTDIR}/lib/input/inputEvent.o.d" -o ${OBJECTDIR}/lib/input/inputEvent.o lib/input/inputEvent.c  
	
${OBJECTDIR}/lib/input/inputTask.o: lib/input/inputTask.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/lib/input 
	@${RM} ${OBJECTDIR}/lib/input/inputTask.o.d 
	@${FIXDEPS} "${OBJECTDIR}/lib/input/inputTask.o.d" $(SILENT) -c ${MP_CC} $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1 -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"E:/Projekty/Inz/IntenseS.X/os/port/MPLAB/PIC32MX" -I"E:/Projekty/Inz/IntenseS.X/os" -I"E:/Projekty/Inz/IntenseS.X/boards/PIC32MX7MMB" -I"E:/Projekty/Inz/IntenseS.X" -O1 -MMD -MF "${OBJECTDIR}/lib/input/inputTask.o.d" -o ${OBJECTDIR}/lib/input/inputTask.o lib/input/inputTask.c  
	
${OBJECTDIR}/lib/ir/nec.o: lib/ir/nec.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/lib/ir 
	@${RM} ${OBJECTDIR}/lib/ir/nec.o.d 
	@${FIXDEPS} "${OBJECTDIR}/lib/ir/nec.o.d" $(SILENT) -c ${MP_CC} $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1 -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"E:/Projekty/Inz/IntenseS.X/os/port/MPLAB/PIC32MX" -I"E:/Projekty/Inz/IntenseS.X/os" -I"E:/Projekty/Inz/IntenseS.X/boards/PIC32MX7MMB" -I"E:/Projekty/Inz/IntenseS.X" -O1 -MMD -MF "${OBJECTDIR}/lib/ir/nec.o.d" -o ${OBJECTDIR}/lib/ir/nec.o lib/ir/nec.c  
	
${OBJECTDIR}/lib/user/msgListener.o: lib/user/msgListener.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/lib/user 
	@${RM} ${OBJECTDIR}/lib/user/msgListener.o.d 
	@${FIXDEPS} "${OBJECTDIR}/lib/user/msgListener.o.d" $(SILENT) -c ${MP_CC} $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1 -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"E:/Projekty/Inz/IntenseS.X/os/port/MPLAB/PIC32MX" -I"E:/Projekty/Inz/IntenseS.X/os" -I"E:/Projekty/Inz/IntenseS.X/boards/PIC32MX7MMB" -I"E:/Projekty/Inz/IntenseS.X" -O1 -MMD -MF "${OBJECTDIR}/lib/user/msgListener.o.d" -o ${OBJECTDIR}/lib/user/msgListener.o lib/user/msgListener.c  
	
${OBJECTDIR}/lib/user/msgDispatcher.o: lib/user/msgDispatcher.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/lib/user 
	@${RM} ${OBJECTDIR}/lib/user/msgDispatcher.o.d 
	@${FIXDEPS} "${OBJECTDIR}/lib/user/msgDispatcher.o.d" $(SILENT) -c ${MP_CC} $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1 -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"E:/Projekty/Inz/IntenseS.X/os/port/MPLAB/PIC32MX" -I"E:/Projekty/Inz/IntenseS.X/os" -I"E:/Projekty/Inz/IntenseS.X/boards/PIC32MX7MMB" -I"E:/Projekty/Inz/IntenseS.X" -O1 -MMD -MF "${OBJECTDIR}/lib/user/msgDispatcher.o.d" -o ${OBJECTDIR}/lib/user/msgDispatcher.o lib/user/msgDispatcher.c  
	
${OBJECTDIR}/lib/log.o: lib/log.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/lib 
	@${RM} ${OBJECTDIR}/lib/log.o.d 
	@${FIXDEPS} "${OBJECTDIR}/lib/log.o.d" $(SILENT) -c ${MP_CC} $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1 -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"E:/Projekty/Inz/IntenseS.X/os/port/MPLAB/PIC32MX" -I"E:/Projekty/Inz/IntenseS.X/os" -I"E:/Projekty/Inz/IntenseS.X/boards/PIC32MX7MMB" -I"E:/Projekty/Inz/IntenseS.X" -O1 -MMD -MF "${OBJECTDIR}/lib/log.o.d" -o ${OBJECTDIR}/lib/log.o lib/log.c  
	
${OBJECTDIR}/lib/digitalFilter.o: lib/digitalFilter.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/lib 
	@${RM} ${OBJECTDIR}/lib/digitalFilter.o.d 
	@${FIXDEPS} "${OBJECTDIR}/lib/digitalFilter.o.d" $(SILENT) -c ${MP_CC} $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1 -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"E:/Projekty/Inz/IntenseS.X/os/port/MPLAB/PIC32MX" -I"E:/Projekty/Inz/IntenseS.X/os" -I"E:/Projekty/Inz/IntenseS.X/boards/PIC32MX7MMB" -I"E:/Projekty/Inz/IntenseS.X" -O1 -MMD -MF "${OBJECTDIR}/lib/digitalFilter.o.d" -o ${OBJECTDIR}/lib/digitalFilter.o lib/digitalFilter.c  
	
${OBJECTDIR}/lib/time.o: lib/time.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/lib 
	@${RM} ${OBJECTDIR}/lib/time.o.d 
	@${FIXDEPS} "${OBJECTDIR}/lib/time.o.d" $(SILENT) -c ${MP_CC} $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1 -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"E:/Projekty/Inz/IntenseS.X/os/port/MPLAB/PIC32MX" -I"E:/Projekty/Inz/IntenseS.X/os" -I"E:/Projekty/Inz/IntenseS.X/boards/PIC32MX7MMB" -I"E:/Projekty/Inz/IntenseS.X" -O1 -MMD -MF "${OBJECTDIR}/lib/time.o.d" -o ${OBJECTDIR}/lib/time.o lib/time.c  
	
${OBJECTDIR}/os/port/MPLAB/PIC32MX/port.o: os/port/MPLAB/PIC32MX/port.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/os/port/MPLAB/PIC32MX 
	@${RM} ${OBJECTDIR}/os/port/MPLAB/PIC32MX/port.o.d 
	@${FIXDEPS} "${OBJECTDIR}/os/port/MPLAB/PIC32MX/port.o.d" $(SILENT) -c ${MP_CC} $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1 -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"E:/Projekty/Inz/IntenseS.X/os/port/MPLAB/PIC32MX" -I"E:/Projekty/Inz/IntenseS.X/os" -I"E:/Projekty/Inz/IntenseS.X/boards/PIC32MX7MMB" -I"E:/Projekty/Inz/IntenseS.X" -O1 -MMD -MF "${OBJECTDIR}/os/port/MPLAB/PIC32MX/port.o.d" -o ${OBJECTDIR}/os/port/MPLAB/PIC32MX/port.o os/port/MPLAB/PIC32MX/port.c  
	
${OBJECTDIR}/os/croutine.o: os/croutine.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/os 
	@${RM} ${OBJECTDIR}/os/croutine.o.d 
	@${FIXDEPS} "${OBJECTDIR}/os/croutine.o.d" $(SILENT) -c ${MP_CC} $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1 -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"E:/Projekty/Inz/IntenseS.X/os/port/MPLAB/PIC32MX" -I"E:/Projekty/Inz/IntenseS.X/os" -I"E:/Projekty/Inz/IntenseS.X/boards/PIC32MX7MMB" -I"E:/Projekty/Inz/IntenseS.X" -O1 -MMD -MF "${OBJECTDIR}/os/croutine.o.d" -o ${OBJECTDIR}/os/croutine.o os/croutine.c  
	
${OBJECTDIR}/os/heap_2.o: os/heap_2.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/os 
	@${RM} ${OBJECTDIR}/os/heap_2.o.d 
	@${FIXDEPS} "${OBJECTDIR}/os/heap_2.o.d" $(SILENT) -c ${MP_CC} $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1 -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"E:/Projekty/Inz/IntenseS.X/os/port/MPLAB/PIC32MX" -I"E:/Projekty/Inz/IntenseS.X/os" -I"E:/Projekty/Inz/IntenseS.X/boards/PIC32MX7MMB" -I"E:/Projekty/Inz/IntenseS.X" -O1 -MMD -MF "${OBJECTDIR}/os/heap_2.o.d" -o ${OBJECTDIR}/os/heap_2.o os/heap_2.c  
	
${OBJECTDIR}/os/list.o: os/list.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/os 
	@${RM} ${OBJECTDIR}/os/list.o.d 
	@${FIXDEPS} "${OBJECTDIR}/os/list.o.d" $(SILENT) -c ${MP_CC} $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1 -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"E:/Projekty/Inz/IntenseS.X/os/port/MPLAB/PIC32MX" -I"E:/Projekty/Inz/IntenseS.X/os" -I"E:/Projekty/Inz/IntenseS.X/boards/PIC32MX7MMB" -I"E:/Projekty/Inz/IntenseS.X" -O1 -MMD -MF "${OBJECTDIR}/os/list.o.d" -o ${OBJECTDIR}/os/list.o os/list.c  
	
${OBJECTDIR}/os/queue.o: os/queue.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/os 
	@${RM} ${OBJECTDIR}/os/queue.o.d 
	@${FIXDEPS} "${OBJECTDIR}/os/queue.o.d" $(SILENT) -c ${MP_CC} $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1 -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"E:/Projekty/Inz/IntenseS.X/os/port/MPLAB/PIC32MX" -I"E:/Projekty/Inz/IntenseS.X/os" -I"E:/Projekty/Inz/IntenseS.X/boards/PIC32MX7MMB" -I"E:/Projekty/Inz/IntenseS.X" -O1 -MMD -MF "${OBJECTDIR}/os/queue.o.d" -o ${OBJECTDIR}/os/queue.o os/queue.c  
	
${OBJECTDIR}/os/tasks.o: os/tasks.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/os 
	@${RM} ${OBJECTDIR}/os/tasks.o.d 
	@${FIXDEPS} "${OBJECTDIR}/os/tasks.o.d" $(SILENT) -c ${MP_CC} $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1 -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"E:/Projekty/Inz/IntenseS.X/os/port/MPLAB/PIC32MX" -I"E:/Projekty/Inz/IntenseS.X/os" -I"E:/Projekty/Inz/IntenseS.X/boards/PIC32MX7MMB" -I"E:/Projekty/Inz/IntenseS.X" -O1 -MMD -MF "${OBJECTDIR}/os/tasks.o.d" -o ${OBJECTDIR}/os/tasks.o os/tasks.c  
	
${OBJECTDIR}/os/timers.o: os/timers.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/os 
	@${RM} ${OBJECTDIR}/os/timers.o.d 
	@${FIXDEPS} "${OBJECTDIR}/os/timers.o.d" $(SILENT) -c ${MP_CC} $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1 -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"E:/Projekty/Inz/IntenseS.X/os/port/MPLAB/PIC32MX" -I"E:/Projekty/Inz/IntenseS.X/os" -I"E:/Projekty/Inz/IntenseS.X/boards/PIC32MX7MMB" -I"E:/Projekty/Inz/IntenseS.X" -O1 -MMD -MF "${OBJECTDIR}/os/timers.o.d" -o ${OBJECTDIR}/os/timers.o os/timers.c  
	
else
${OBJECTDIR}/app/main.o: app/main.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/app 
	@${RM} ${OBJECTDIR}/app/main.o.d 
	@${FIXDEPS} "${OBJECTDIR}/app/main.o.d" $(SILENT) -c ${MP_CC} $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"E:/Projekty/Inz/IntenseS.X/os/port/MPLAB/PIC32MX" -I"E:/Projekty/Inz/IntenseS.X/os" -I"E:/Projekty/Inz/IntenseS.X/boards/PIC32MX7MMB" -I"E:/Projekty/Inz/IntenseS.X" -O1 -MMD -MF "${OBJECTDIR}/app/main.o.d" -o ${OBJECTDIR}/app/main.o app/main.c  
	
${OBJECTDIR}/boards/PIC32MX7MMB/board.o: boards/PIC32MX7MMB/board.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/boards/PIC32MX7MMB 
	@${RM} ${OBJECTDIR}/boards/PIC32MX7MMB/board.o.d 
	@${FIXDEPS} "${OBJECTDIR}/boards/PIC32MX7MMB/board.o.d" $(SILENT) -c ${MP_CC} $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"E:/Projekty/Inz/IntenseS.X/os/port/MPLAB/PIC32MX" -I"E:/Projekty/Inz/IntenseS.X/os" -I"E:/Projekty/Inz/IntenseS.X/boards/PIC32MX7MMB" -I"E:/Projekty/Inz/IntenseS.X" -O1 -MMD -MF "${OBJECTDIR}/boards/PIC32MX7MMB/board.o.d" -o ${OBJECTDIR}/boards/PIC32MX7MMB/board.o boards/PIC32MX7MMB/board.c  
	
${OBJECTDIR}/hal/hld/hld.o: hal/hld/hld.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/hal/hld 
	@${RM} ${OBJECTDIR}/hal/hld/hld.o.d 
	@${FIXDEPS} "${OBJECTDIR}/hal/hld/hld.o.d" $(SILENT) -c ${MP_CC} $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"E:/Projekty/Inz/IntenseS.X/os/port/MPLAB/PIC32MX" -I"E:/Projekty/Inz/IntenseS.X/os" -I"E:/Projekty/Inz/IntenseS.X/boards/PIC32MX7MMB" -I"E:/Projekty/Inz/IntenseS.X" -O1 -MMD -MF "${OBJECTDIR}/hal/hld/hld.o.d" -o ${OBJECTDIR}/hal/hld/hld.o hal/hld/hld.c  
	
${OBJECTDIR}/hal/lld/platforms/microchip/gpio.o: hal/lld/platforms/microchip/gpio.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/hal/lld/platforms/microchip 
	@${RM} ${OBJECTDIR}/hal/lld/platforms/microchip/gpio.o.d 
	@${FIXDEPS} "${OBJECTDIR}/hal/lld/platforms/microchip/gpio.o.d" $(SILENT) -c ${MP_CC} $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"E:/Projekty/Inz/IntenseS.X/os/port/MPLAB/PIC32MX" -I"E:/Projekty/Inz/IntenseS.X/os" -I"E:/Projekty/Inz/IntenseS.X/boards/PIC32MX7MMB" -I"E:/Projekty/Inz/IntenseS.X" -O1 -MMD -MF "${OBJECTDIR}/hal/lld/platforms/microchip/gpio.o.d" -o ${OBJECTDIR}/hal/lld/platforms/microchip/gpio.o hal/lld/platforms/microchip/gpio.c  
	
${OBJECTDIR}/hal/lld/platforms/microchip/ili9163.o: hal/lld/platforms/microchip/ili9163.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/hal/lld/platforms/microchip 
	@${RM} ${OBJECTDIR}/hal/lld/platforms/microchip/ili9163.o.d 
	@${FIXDEPS} "${OBJECTDIR}/hal/lld/platforms/microchip/ili9163.o.d" $(SILENT) -c ${MP_CC} $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"E:/Projekty/Inz/IntenseS.X/os/port/MPLAB/PIC32MX" -I"E:/Projekty/Inz/IntenseS.X/os" -I"E:/Projekty/Inz/IntenseS.X/boards/PIC32MX7MMB" -I"E:/Projekty/Inz/IntenseS.X" -O1 -MMD -MF "${OBJECTDIR}/hal/lld/platforms/microchip/ili9163.o.d" -o ${OBJECTDIR}/hal/lld/platforms/microchip/ili9163.o hal/lld/platforms/microchip/ili9163.c  
	
${OBJECTDIR}/hal/lld/platforms/microchip/hx8347.o: hal/lld/platforms/microchip/hx8347.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/hal/lld/platforms/microchip 
	@${RM} ${OBJECTDIR}/hal/lld/platforms/microchip/hx8347.o.d 
	@${FIXDEPS} "${OBJECTDIR}/hal/lld/platforms/microchip/hx8347.o.d" $(SILENT) -c ${MP_CC} $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"E:/Projekty/Inz/IntenseS.X/os/port/MPLAB/PIC32MX" -I"E:/Projekty/Inz/IntenseS.X/os" -I"E:/Projekty/Inz/IntenseS.X/boards/PIC32MX7MMB" -I"E:/Projekty/Inz/IntenseS.X" -O1 -MMD -MF "${OBJECTDIR}/hal/lld/platforms/microchip/hx8347.o.d" -o ${OBJECTDIR}/hal/lld/platforms/microchip/hx8347.o hal/lld/platforms/microchip/hx8347.c  
	
${OBJECTDIR}/hal/lld/platforms/microchip/pic32UART.o: hal/lld/platforms/microchip/pic32UART.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/hal/lld/platforms/microchip 
	@${RM} ${OBJECTDIR}/hal/lld/platforms/microchip/pic32UART.o.d 
	@${FIXDEPS} "${OBJECTDIR}/hal/lld/platforms/microchip/pic32UART.o.d" $(SILENT) -c ${MP_CC} $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"E:/Projekty/Inz/IntenseS.X/os/port/MPLAB/PIC32MX" -I"E:/Projekty/Inz/IntenseS.X/os" -I"E:/Projekty/Inz/IntenseS.X/boards/PIC32MX7MMB" -I"E:/Projekty/Inz/IntenseS.X" -O1 -MMD -MF "${OBJECTDIR}/hal/lld/platforms/microchip/pic32UART.o.d" -o ${OBJECTDIR}/hal/lld/platforms/microchip/pic32UART.o hal/lld/platforms/microchip/pic32UART.c  
	
${OBJECTDIR}/hal/lld/platforms/microchip/pic32ADC.o: hal/lld/platforms/microchip/pic32ADC.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/hal/lld/platforms/microchip 
	@${RM} ${OBJECTDIR}/hal/lld/platforms/microchip/pic32ADC.o.d 
	@${FIXDEPS} "${OBJECTDIR}/hal/lld/platforms/microchip/pic32ADC.o.d" $(SILENT) -c ${MP_CC} $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"E:/Projekty/Inz/IntenseS.X/os/port/MPLAB/PIC32MX" -I"E:/Projekty/Inz/IntenseS.X/os" -I"E:/Projekty/Inz/IntenseS.X/boards/PIC32MX7MMB" -I"E:/Projekty/Inz/IntenseS.X" -O1 -MMD -MF "${OBJECTDIR}/hal/lld/platforms/microchip/pic32ADC.o.d" -o ${OBJECTDIR}/hal/lld/platforms/microchip/pic32ADC.o hal/lld/platforms/microchip/pic32ADC.c  
	
${OBJECTDIR}/hal/lld/platforms/microchip/pic32IR.o: hal/lld/platforms/microchip/pic32IR.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/hal/lld/platforms/microchip 
	@${RM} ${OBJECTDIR}/hal/lld/platforms/microchip/pic32IR.o.d 
	@${FIXDEPS} "${OBJECTDIR}/hal/lld/platforms/microchip/pic32IR.o.d" $(SILENT) -c ${MP_CC} $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"E:/Projekty/Inz/IntenseS.X/os/port/MPLAB/PIC32MX" -I"E:/Projekty/Inz/IntenseS.X/os" -I"E:/Projekty/Inz/IntenseS.X/boards/PIC32MX7MMB" -I"E:/Projekty/Inz/IntenseS.X" -O1 -MMD -MF "${OBJECTDIR}/hal/lld/platforms/microchip/pic32IR.o.d" -o ${OBJECTDIR}/hal/lld/platforms/microchip/pic32IR.o hal/lld/platforms/microchip/pic32IR.c  
	
${OBJECTDIR}/hal/lld/platforms/microchip/wm8731.o: hal/lld/platforms/microchip/wm8731.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/hal/lld/platforms/microchip 
	@${RM} ${OBJECTDIR}/hal/lld/platforms/microchip/wm8731.o.d 
	@${FIXDEPS} "${OBJECTDIR}/hal/lld/platforms/microchip/wm8731.o.d" $(SILENT) -c ${MP_CC} $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"E:/Projekty/Inz/IntenseS.X/os/port/MPLAB/PIC32MX" -I"E:/Projekty/Inz/IntenseS.X/os" -I"E:/Projekty/Inz/IntenseS.X/boards/PIC32MX7MMB" -I"E:/Projekty/Inz/IntenseS.X" -O1 -MMD -MF "${OBJECTDIR}/hal/lld/platforms/microchip/wm8731.o.d" -o ${OBJECTDIR}/hal/lld/platforms/microchip/wm8731.o hal/lld/platforms/microchip/wm8731.c  
	
${OBJECTDIR}/hal/lld/platforms/microchip/pic32SDMMC.o: hal/lld/platforms/microchip/pic32SDMMC.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/hal/lld/platforms/microchip 
	@${RM} ${OBJECTDIR}/hal/lld/platforms/microchip/pic32SDMMC.o.d 
	@${FIXDEPS} "${OBJECTDIR}/hal/lld/platforms/microchip/pic32SDMMC.o.d" $(SILENT) -c ${MP_CC} $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"E:/Projekty/Inz/IntenseS.X/os/port/MPLAB/PIC32MX" -I"E:/Projekty/Inz/IntenseS.X/os" -I"E:/Projekty/Inz/IntenseS.X/boards/PIC32MX7MMB" -I"E:/Projekty/Inz/IntenseS.X" -O1 -MMD -MF "${OBJECTDIR}/hal/lld/platforms/microchip/pic32SDMMC.o.d" -o ${OBJECTDIR}/hal/lld/platforms/microchip/pic32SDMMC.o hal/lld/platforms/microchip/pic32SDMMC.c  
	
${OBJECTDIR}/hal/lld/resistiveTouch.o: hal/lld/resistiveTouch.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/hal/lld 
	@${RM} ${OBJECTDIR}/hal/lld/resistiveTouch.o.d 
	@${FIXDEPS} "${OBJECTDIR}/hal/lld/resistiveTouch.o.d" $(SILENT) -c ${MP_CC} $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"E:/Projekty/Inz/IntenseS.X/os/port/MPLAB/PIC32MX" -I"E:/Projekty/Inz/IntenseS.X/os" -I"E:/Projekty/Inz/IntenseS.X/boards/PIC32MX7MMB" -I"E:/Projekty/Inz/IntenseS.X" -O1 -MMD -MF "${OBJECTDIR}/hal/lld/resistiveTouch.o.d" -o ${OBJECTDIR}/hal/lld/resistiveTouch.o hal/lld/resistiveTouch.c  
	
${OBJECTDIR}/lib/audio/decoders/wave.o: lib/audio/decoders/wave.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/lib/audio/decoders 
	@${RM} ${OBJECTDIR}/lib/audio/decoders/wave.o.d 
	@${FIXDEPS} "${OBJECTDIR}/lib/audio/decoders/wave.o.d" $(SILENT) -c ${MP_CC} $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"E:/Projekty/Inz/IntenseS.X/os/port/MPLAB/PIC32MX" -I"E:/Projekty/Inz/IntenseS.X/os" -I"E:/Projekty/Inz/IntenseS.X/boards/PIC32MX7MMB" -I"E:/Projekty/Inz/IntenseS.X" -O1 -MMD -MF "${OBJECTDIR}/lib/audio/decoders/wave.o.d" -o ${OBJECTDIR}/lib/audio/decoders/wave.o lib/audio/decoders/wave.c  
	
${OBJECTDIR}/lib/audio/audioSineOutput.o: lib/audio/audioSineOutput.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/lib/audio 
	@${RM} ${OBJECTDIR}/lib/audio/audioSineOutput.o.d 
	@${FIXDEPS} "${OBJECTDIR}/lib/audio/audioSineOutput.o.d" $(SILENT) -c ${MP_CC} $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"E:/Projekty/Inz/IntenseS.X/os/port/MPLAB/PIC32MX" -I"E:/Projekty/Inz/IntenseS.X/os" -I"E:/Projekty/Inz/IntenseS.X/boards/PIC32MX7MMB" -I"E:/Projekty/Inz/IntenseS.X" -O1 -MMD -MF "${OBJECTDIR}/lib/audio/audioSineOutput.o.d" -o ${OBJECTDIR}/lib/audio/audioSineOutput.o lib/audio/audioSineOutput.c  
	
${OBJECTDIR}/lib/audio/audioSinLUT.o: lib/audio/audioSinLUT.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/lib/audio 
	@${RM} ${OBJECTDIR}/lib/audio/audioSinLUT.o.d 
	@${FIXDEPS} "${OBJECTDIR}/lib/audio/audioSinLUT.o.d" $(SILENT) -c ${MP_CC} $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"E:/Projekty/Inz/IntenseS.X/os/port/MPLAB/PIC32MX" -I"E:/Projekty/Inz/IntenseS.X/os" -I"E:/Projekty/Inz/IntenseS.X/boards/PIC32MX7MMB" -I"E:/Projekty/Inz/IntenseS.X" -O1 -MMD -MF "${OBJECTDIR}/lib/audio/audioSinLUT.o.d" -o ${OBJECTDIR}/lib/audio/audioSinLUT.o lib/audio/audioSinLUT.c  
	
${OBJECTDIR}/lib/audio/audioConsole.o: lib/audio/audioConsole.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/lib/audio 
	@${RM} ${OBJECTDIR}/lib/audio/audioConsole.o.d 
	@${FIXDEPS} "${OBJECTDIR}/lib/audio/audioConsole.o.d" $(SILENT) -c ${MP_CC} $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"E:/Projekty/Inz/IntenseS.X/os/port/MPLAB/PIC32MX" -I"E:/Projekty/Inz/IntenseS.X/os" -I"E:/Projekty/Inz/IntenseS.X/boards/PIC32MX7MMB" -I"E:/Projekty/Inz/IntenseS.X" -O1 -MMD -MF "${OBJECTDIR}/lib/audio/audioConsole.o.d" -o ${OBJECTDIR}/lib/audio/audioConsole.o lib/audio/audioConsole.c  
	
${OBJECTDIR}/lib/audio/audio1ch.o: lib/audio/audio1ch.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/lib/audio 
	@${RM} ${OBJECTDIR}/lib/audio/audio1ch.o.d 
	@${FIXDEPS} "${OBJECTDIR}/lib/audio/audio1ch.o.d" $(SILENT) -c ${MP_CC} $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"E:/Projekty/Inz/IntenseS.X/os/port/MPLAB/PIC32MX" -I"E:/Projekty/Inz/IntenseS.X/os" -I"E:/Projekty/Inz/IntenseS.X/boards/PIC32MX7MMB" -I"E:/Projekty/Inz/IntenseS.X" -O1 -MMD -MF "${OBJECTDIR}/lib/audio/audio1ch.o.d" -o ${OBJECTDIR}/lib/audio/audio1ch.o lib/audio/audio1ch.c  
	
${OBJECTDIR}/lib/console/serialConsole.o: lib/console/serialConsole.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/lib/console 
	@${RM} ${OBJECTDIR}/lib/console/serialConsole.o.d 
	@${FIXDEPS} "${OBJECTDIR}/lib/console/serialConsole.o.d" $(SILENT) -c ${MP_CC} $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"E:/Projekty/Inz/IntenseS.X/os/port/MPLAB/PIC32MX" -I"E:/Projekty/Inz/IntenseS.X/os" -I"E:/Projekty/Inz/IntenseS.X/boards/PIC32MX7MMB" -I"E:/Projekty/Inz/IntenseS.X" -O1 -MMD -MF "${OBJECTDIR}/lib/console/serialConsole.o.d" -o ${OBJECTDIR}/lib/console/serialConsole.o lib/console/serialConsole.c  
	
${OBJECTDIR}/lib/fatfs/option/syscall.o: lib/fatfs/option/syscall.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/lib/fatfs/option 
	@${RM} ${OBJECTDIR}/lib/fatfs/option/syscall.o.d 
	@${FIXDEPS} "${OBJECTDIR}/lib/fatfs/option/syscall.o.d" $(SILENT) -c ${MP_CC} $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"E:/Projekty/Inz/IntenseS.X/os/port/MPLAB/PIC32MX" -I"E:/Projekty/Inz/IntenseS.X/os" -I"E:/Projekty/Inz/IntenseS.X/boards/PIC32MX7MMB" -I"E:/Projekty/Inz/IntenseS.X" -O1 -MMD -MF "${OBJECTDIR}/lib/fatfs/option/syscall.o.d" -o ${OBJECTDIR}/lib/fatfs/option/syscall.o lib/fatfs/option/syscall.c  
	
${OBJECTDIR}/lib/fatfs/option/unicode.o: lib/fatfs/option/unicode.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/lib/fatfs/option 
	@${RM} ${OBJECTDIR}/lib/fatfs/option/unicode.o.d 
	@${FIXDEPS} "${OBJECTDIR}/lib/fatfs/option/unicode.o.d" $(SILENT) -c ${MP_CC} $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"E:/Projekty/Inz/IntenseS.X/os/port/MPLAB/PIC32MX" -I"E:/Projekty/Inz/IntenseS.X/os" -I"E:/Projekty/Inz/IntenseS.X/boards/PIC32MX7MMB" -I"E:/Projekty/Inz/IntenseS.X" -O1 -MMD -MF "${OBJECTDIR}/lib/fatfs/option/unicode.o.d" -o ${OBJECTDIR}/lib/fatfs/option/unicode.o lib/fatfs/option/unicode.c  
	
${OBJECTDIR}/lib/fatfs/diskio.o: lib/fatfs/diskio.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/lib/fatfs 
	@${RM} ${OBJECTDIR}/lib/fatfs/diskio.o.d 
	@${FIXDEPS} "${OBJECTDIR}/lib/fatfs/diskio.o.d" $(SILENT) -c ${MP_CC} $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"E:/Projekty/Inz/IntenseS.X/os/port/MPLAB/PIC32MX" -I"E:/Projekty/Inz/IntenseS.X/os" -I"E:/Projekty/Inz/IntenseS.X/boards/PIC32MX7MMB" -I"E:/Projekty/Inz/IntenseS.X" -O1 -MMD -MF "${OBJECTDIR}/lib/fatfs/diskio.o.d" -o ${OBJECTDIR}/lib/fatfs/diskio.o lib/fatfs/diskio.c  
	
${OBJECTDIR}/lib/fatfs/ff.o: lib/fatfs/ff.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/lib/fatfs 
	@${RM} ${OBJECTDIR}/lib/fatfs/ff.o.d 
	@${FIXDEPS} "${OBJECTDIR}/lib/fatfs/ff.o.d" $(SILENT) -c ${MP_CC} $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"E:/Projekty/Inz/IntenseS.X/os/port/MPLAB/PIC32MX" -I"E:/Projekty/Inz/IntenseS.X/os" -I"E:/Projekty/Inz/IntenseS.X/boards/PIC32MX7MMB" -I"E:/Projekty/Inz/IntenseS.X" -O1 -MMD -MF "${OBJECTDIR}/lib/fatfs/ff.o.d" -o ${OBJECTDIR}/lib/fatfs/ff.o lib/fatfs/ff.c  
	
${OBJECTDIR}/lib/fatfs/fatfsConsole.o: lib/fatfs/fatfsConsole.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/lib/fatfs 
	@${RM} ${OBJECTDIR}/lib/fatfs/fatfsConsole.o.d 
	@${FIXDEPS} "${OBJECTDIR}/lib/fatfs/fatfsConsole.o.d" $(SILENT) -c ${MP_CC} $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"E:/Projekty/Inz/IntenseS.X/os/port/MPLAB/PIC32MX" -I"E:/Projekty/Inz/IntenseS.X/os" -I"E:/Projekty/Inz/IntenseS.X/boards/PIC32MX7MMB" -I"E:/Projekty/Inz/IntenseS.X" -O1 -MMD -MF "${OBJECTDIR}/lib/fatfs/fatfsConsole.o.d" -o ${OBJECTDIR}/lib/fatfs/fatfsConsole.o lib/fatfs/fatfsConsole.c  
	
${OBJECTDIR}/lib/graphics/graphics.o: lib/graphics/graphics.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/lib/graphics 
	@${RM} ${OBJECTDIR}/lib/graphics/graphics.o.d 
	@${FIXDEPS} "${OBJECTDIR}/lib/graphics/graphics.o.d" $(SILENT) -c ${MP_CC} $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"E:/Projekty/Inz/IntenseS.X/os/port/MPLAB/PIC32MX" -I"E:/Projekty/Inz/IntenseS.X/os" -I"E:/Projekty/Inz/IntenseS.X/boards/PIC32MX7MMB" -I"E:/Projekty/Inz/IntenseS.X" -O1 -MMD -MF "${OBJECTDIR}/lib/graphics/graphics.o.d" -o ${OBJECTDIR}/lib/graphics/graphics.o lib/graphics/graphics.c  
	
${OBJECTDIR}/lib/graphics/primitives.o: lib/graphics/primitives.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/lib/graphics 
	@${RM} ${OBJECTDIR}/lib/graphics/primitives.o.d 
	@${FIXDEPS} "${OBJECTDIR}/lib/graphics/primitives.o.d" $(SILENT) -c ${MP_CC} $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"E:/Projekty/Inz/IntenseS.X/os/port/MPLAB/PIC32MX" -I"E:/Projekty/Inz/IntenseS.X/os" -I"E:/Projekty/Inz/IntenseS.X/boards/PIC32MX7MMB" -I"E:/Projekty/Inz/IntenseS.X" -O1 -MMD -MF "${OBJECTDIR}/lib/graphics/primitives.o.d" -o ${OBJECTDIR}/lib/graphics/primitives.o lib/graphics/primitives.c  
	
${OBJECTDIR}/lib/graphics/tests.o: lib/graphics/tests.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/lib/graphics 
	@${RM} ${OBJECTDIR}/lib/graphics/tests.o.d 
	@${FIXDEPS} "${OBJECTDIR}/lib/graphics/tests.o.d" $(SILENT) -c ${MP_CC} $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"E:/Projekty/Inz/IntenseS.X/os/port/MPLAB/PIC32MX" -I"E:/Projekty/Inz/IntenseS.X/os" -I"E:/Projekty/Inz/IntenseS.X/boards/PIC32MX7MMB" -I"E:/Projekty/Inz/IntenseS.X" -O1 -MMD -MF "${OBJECTDIR}/lib/graphics/tests.o.d" -o ${OBJECTDIR}/lib/graphics/tests.o lib/graphics/tests.c  
	
${OBJECTDIR}/lib/gui/window.o: lib/gui/window.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/lib/gui 
	@${RM} ${OBJECTDIR}/lib/gui/window.o.d 
	@${FIXDEPS} "${OBJECTDIR}/lib/gui/window.o.d" $(SILENT) -c ${MP_CC} $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"E:/Projekty/Inz/IntenseS.X/os/port/MPLAB/PIC32MX" -I"E:/Projekty/Inz/IntenseS.X/os" -I"E:/Projekty/Inz/IntenseS.X/boards/PIC32MX7MMB" -I"E:/Projekty/Inz/IntenseS.X" -O1 -MMD -MF "${OBJECTDIR}/lib/gui/window.o.d" -o ${OBJECTDIR}/lib/gui/window.o lib/gui/window.c  
	
${OBJECTDIR}/lib/input/inputEvent.o: lib/input/inputEvent.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/lib/input 
	@${RM} ${OBJECTDIR}/lib/input/inputEvent.o.d 
	@${FIXDEPS} "${OBJECTDIR}/lib/input/inputEvent.o.d" $(SILENT) -c ${MP_CC} $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"E:/Projekty/Inz/IntenseS.X/os/port/MPLAB/PIC32MX" -I"E:/Projekty/Inz/IntenseS.X/os" -I"E:/Projekty/Inz/IntenseS.X/boards/PIC32MX7MMB" -I"E:/Projekty/Inz/IntenseS.X" -O1 -MMD -MF "${OBJECTDIR}/lib/input/inputEvent.o.d" -o ${OBJECTDIR}/lib/input/inputEvent.o lib/input/inputEvent.c  
	
${OBJECTDIR}/lib/input/inputTask.o: lib/input/inputTask.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/lib/input 
	@${RM} ${OBJECTDIR}/lib/input/inputTask.o.d 
	@${FIXDEPS} "${OBJECTDIR}/lib/input/inputTask.o.d" $(SILENT) -c ${MP_CC} $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"E:/Projekty/Inz/IntenseS.X/os/port/MPLAB/PIC32MX" -I"E:/Projekty/Inz/IntenseS.X/os" -I"E:/Projekty/Inz/IntenseS.X/boards/PIC32MX7MMB" -I"E:/Projekty/Inz/IntenseS.X" -O1 -MMD -MF "${OBJECTDIR}/lib/input/inputTask.o.d" -o ${OBJECTDIR}/lib/input/inputTask.o lib/input/inputTask.c  
	
${OBJECTDIR}/lib/ir/nec.o: lib/ir/nec.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/lib/ir 
	@${RM} ${OBJECTDIR}/lib/ir/nec.o.d 
	@${FIXDEPS} "${OBJECTDIR}/lib/ir/nec.o.d" $(SILENT) -c ${MP_CC} $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"E:/Projekty/Inz/IntenseS.X/os/port/MPLAB/PIC32MX" -I"E:/Projekty/Inz/IntenseS.X/os" -I"E:/Projekty/Inz/IntenseS.X/boards/PIC32MX7MMB" -I"E:/Projekty/Inz/IntenseS.X" -O1 -MMD -MF "${OBJECTDIR}/lib/ir/nec.o.d" -o ${OBJECTDIR}/lib/ir/nec.o lib/ir/nec.c  
	
${OBJECTDIR}/lib/user/msgListener.o: lib/user/msgListener.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/lib/user 
	@${RM} ${OBJECTDIR}/lib/user/msgListener.o.d 
	@${FIXDEPS} "${OBJECTDIR}/lib/user/msgListener.o.d" $(SILENT) -c ${MP_CC} $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"E:/Projekty/Inz/IntenseS.X/os/port/MPLAB/PIC32MX" -I"E:/Projekty/Inz/IntenseS.X/os" -I"E:/Projekty/Inz/IntenseS.X/boards/PIC32MX7MMB" -I"E:/Projekty/Inz/IntenseS.X" -O1 -MMD -MF "${OBJECTDIR}/lib/user/msgListener.o.d" -o ${OBJECTDIR}/lib/user/msgListener.o lib/user/msgListener.c  
	
${OBJECTDIR}/lib/user/msgDispatcher.o: lib/user/msgDispatcher.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/lib/user 
	@${RM} ${OBJECTDIR}/lib/user/msgDispatcher.o.d 
	@${FIXDEPS} "${OBJECTDIR}/lib/user/msgDispatcher.o.d" $(SILENT) -c ${MP_CC} $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"E:/Projekty/Inz/IntenseS.X/os/port/MPLAB/PIC32MX" -I"E:/Projekty/Inz/IntenseS.X/os" -I"E:/Projekty/Inz/IntenseS.X/boards/PIC32MX7MMB" -I"E:/Projekty/Inz/IntenseS.X" -O1 -MMD -MF "${OBJECTDIR}/lib/user/msgDispatcher.o.d" -o ${OBJECTDIR}/lib/user/msgDispatcher.o lib/user/msgDispatcher.c  
	
${OBJECTDIR}/lib/log.o: lib/log.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/lib 
	@${RM} ${OBJECTDIR}/lib/log.o.d 
	@${FIXDEPS} "${OBJECTDIR}/lib/log.o.d" $(SILENT) -c ${MP_CC} $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"E:/Projekty/Inz/IntenseS.X/os/port/MPLAB/PIC32MX" -I"E:/Projekty/Inz/IntenseS.X/os" -I"E:/Projekty/Inz/IntenseS.X/boards/PIC32MX7MMB" -I"E:/Projekty/Inz/IntenseS.X" -O1 -MMD -MF "${OBJECTDIR}/lib/log.o.d" -o ${OBJECTDIR}/lib/log.o lib/log.c  
	
${OBJECTDIR}/lib/digitalFilter.o: lib/digitalFilter.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/lib 
	@${RM} ${OBJECTDIR}/lib/digitalFilter.o.d 
	@${FIXDEPS} "${OBJECTDIR}/lib/digitalFilter.o.d" $(SILENT) -c ${MP_CC} $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"E:/Projekty/Inz/IntenseS.X/os/port/MPLAB/PIC32MX" -I"E:/Projekty/Inz/IntenseS.X/os" -I"E:/Projekty/Inz/IntenseS.X/boards/PIC32MX7MMB" -I"E:/Projekty/Inz/IntenseS.X" -O1 -MMD -MF "${OBJECTDIR}/lib/digitalFilter.o.d" -o ${OBJECTDIR}/lib/digitalFilter.o lib/digitalFilter.c  
	
${OBJECTDIR}/lib/time.o: lib/time.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/lib 
	@${RM} ${OBJECTDIR}/lib/time.o.d 
	@${FIXDEPS} "${OBJECTDIR}/lib/time.o.d" $(SILENT) -c ${MP_CC} $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"E:/Projekty/Inz/IntenseS.X/os/port/MPLAB/PIC32MX" -I"E:/Projekty/Inz/IntenseS.X/os" -I"E:/Projekty/Inz/IntenseS.X/boards/PIC32MX7MMB" -I"E:/Projekty/Inz/IntenseS.X" -O1 -MMD -MF "${OBJECTDIR}/lib/time.o.d" -o ${OBJECTDIR}/lib/time.o lib/time.c  
	
${OBJECTDIR}/os/port/MPLAB/PIC32MX/port.o: os/port/MPLAB/PIC32MX/port.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/os/port/MPLAB/PIC32MX 
	@${RM} ${OBJECTDIR}/os/port/MPLAB/PIC32MX/port.o.d 
	@${FIXDEPS} "${OBJECTDIR}/os/port/MPLAB/PIC32MX/port.o.d" $(SILENT) -c ${MP_CC} $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"E:/Projekty/Inz/IntenseS.X/os/port/MPLAB/PIC32MX" -I"E:/Projekty/Inz/IntenseS.X/os" -I"E:/Projekty/Inz/IntenseS.X/boards/PIC32MX7MMB" -I"E:/Projekty/Inz/IntenseS.X" -O1 -MMD -MF "${OBJECTDIR}/os/port/MPLAB/PIC32MX/port.o.d" -o ${OBJECTDIR}/os/port/MPLAB/PIC32MX/port.o os/port/MPLAB/PIC32MX/port.c  
	
${OBJECTDIR}/os/croutine.o: os/croutine.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/os 
	@${RM} ${OBJECTDIR}/os/croutine.o.d 
	@${FIXDEPS} "${OBJECTDIR}/os/croutine.o.d" $(SILENT) -c ${MP_CC} $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"E:/Projekty/Inz/IntenseS.X/os/port/MPLAB/PIC32MX" -I"E:/Projekty/Inz/IntenseS.X/os" -I"E:/Projekty/Inz/IntenseS.X/boards/PIC32MX7MMB" -I"E:/Projekty/Inz/IntenseS.X" -O1 -MMD -MF "${OBJECTDIR}/os/croutine.o.d" -o ${OBJECTDIR}/os/croutine.o os/croutine.c  
	
${OBJECTDIR}/os/heap_2.o: os/heap_2.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/os 
	@${RM} ${OBJECTDIR}/os/heap_2.o.d 
	@${FIXDEPS} "${OBJECTDIR}/os/heap_2.o.d" $(SILENT) -c ${MP_CC} $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"E:/Projekty/Inz/IntenseS.X/os/port/MPLAB/PIC32MX" -I"E:/Projekty/Inz/IntenseS.X/os" -I"E:/Projekty/Inz/IntenseS.X/boards/PIC32MX7MMB" -I"E:/Projekty/Inz/IntenseS.X" -O1 -MMD -MF "${OBJECTDIR}/os/heap_2.o.d" -o ${OBJECTDIR}/os/heap_2.o os/heap_2.c  
	
${OBJECTDIR}/os/list.o: os/list.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/os 
	@${RM} ${OBJECTDIR}/os/list.o.d 
	@${FIXDEPS} "${OBJECTDIR}/os/list.o.d" $(SILENT) -c ${MP_CC} $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"E:/Projekty/Inz/IntenseS.X/os/port/MPLAB/PIC32MX" -I"E:/Projekty/Inz/IntenseS.X/os" -I"E:/Projekty/Inz/IntenseS.X/boards/PIC32MX7MMB" -I"E:/Projekty/Inz/IntenseS.X" -O1 -MMD -MF "${OBJECTDIR}/os/list.o.d" -o ${OBJECTDIR}/os/list.o os/list.c  
	
${OBJECTDIR}/os/queue.o: os/queue.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/os 
	@${RM} ${OBJECTDIR}/os/queue.o.d 
	@${FIXDEPS} "${OBJECTDIR}/os/queue.o.d" $(SILENT) -c ${MP_CC} $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"E:/Projekty/Inz/IntenseS.X/os/port/MPLAB/PIC32MX" -I"E:/Projekty/Inz/IntenseS.X/os" -I"E:/Projekty/Inz/IntenseS.X/boards/PIC32MX7MMB" -I"E:/Projekty/Inz/IntenseS.X" -O1 -MMD -MF "${OBJECTDIR}/os/queue.o.d" -o ${OBJECTDIR}/os/queue.o os/queue.c  
	
${OBJECTDIR}/os/tasks.o: os/tasks.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/os 
	@${RM} ${OBJECTDIR}/os/tasks.o.d 
	@${FIXDEPS} "${OBJECTDIR}/os/tasks.o.d" $(SILENT) -c ${MP_CC} $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"E:/Projekty/Inz/IntenseS.X/os/port/MPLAB/PIC32MX" -I"E:/Projekty/Inz/IntenseS.X/os" -I"E:/Projekty/Inz/IntenseS.X/boards/PIC32MX7MMB" -I"E:/Projekty/Inz/IntenseS.X" -O1 -MMD -MF "${OBJECTDIR}/os/tasks.o.d" -o ${OBJECTDIR}/os/tasks.o os/tasks.c  
	
${OBJECTDIR}/os/timers.o: os/timers.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/os 
	@${RM} ${OBJECTDIR}/os/timers.o.d 
	@${FIXDEPS} "${OBJECTDIR}/os/timers.o.d" $(SILENT) -c ${MP_CC} $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"E:/Projekty/Inz/IntenseS.X/os/port/MPLAB/PIC32MX" -I"E:/Projekty/Inz/IntenseS.X/os" -I"E:/Projekty/Inz/IntenseS.X/boards/PIC32MX7MMB" -I"E:/Projekty/Inz/IntenseS.X" -O1 -MMD -MF "${OBJECTDIR}/os/timers.o.d" -o ${OBJECTDIR}/os/timers.o os/timers.c  
	
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: link
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
dist/${CND_CONF}/${IMAGE_TYPE}/IntenseS.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} dist/${CND_CONF}/${IMAGE_TYPE} 
	${MP_CC} $(MP_EXTRA_LD_PRE)  -mdebugger -D__MPLAB_DEBUGGER_ICD3=1 -mprocessor=$(MP_PROCESSOR_OPTION)  -o dist/${CND_CONF}/${IMAGE_TYPE}/IntenseS.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX} ${OBJECTFILES_QUOTED_IF_SPACED}        -Wl,--defsym=__MPLAB_BUILD=1$(MP_EXTRA_LD_POST)$(MP_LINKER_FILE_OPTION),--defsym=__MPLAB_DEBUG=1,--defsym=__ICD2RAM=1,--defsym=__DEBUG=1,--defsym=__MPLAB_DEBUGGER_ICD3=1 
else
dist/${CND_CONF}/${IMAGE_TYPE}/IntenseS.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} dist/${CND_CONF}/${IMAGE_TYPE} 
	${MP_CC} $(MP_EXTRA_LD_PRE)  -mprocessor=$(MP_PROCESSOR_OPTION)  -o dist/${CND_CONF}/${IMAGE_TYPE}/IntenseS.X.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX} ${OBJECTFILES_QUOTED_IF_SPACED}        -Wl,--defsym=__MPLAB_BUILD=1$(MP_EXTRA_LD_POST)$(MP_LINKER_FILE_OPTION)
	${MP_CC_DIR}\\pic32-bin2hex dist/${CND_CONF}/${IMAGE_TYPE}/IntenseS.X.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX}  
endif


# Subprojects
.build-subprojects:

# Clean Targets
.clean-conf:
	${RM} -r build/default
	${RM} -r dist/default

# Enable dependency checking
.dep.inc: .depcheck-impl

DEPFILES=$(shell mplabwildcard ${POSSIBLE_DEPFILES})
ifneq (${DEPFILES},)
include ${DEPFILES}
endif
