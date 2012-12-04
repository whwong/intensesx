#include <p32xxxx.h>
#include <sys/asm.h>
#include "ISR_Support.h"

	.set	nomips16
 	.set 	noreorder

 	.extern pxCurrentTCB
 	.extern vTaskSwitchContext

 	.extern lldPic32IRTimerIntHandler
	.extern xISRStackTop
 	.global	lldPic32IRTimerIntWrapper

	.set		noreorder
	.set 		noat
	.ent		lldPic32IRTimerIntWrapper

lldPic32IRTimerIntWrapper:

	portSAVE_CONTEXT
	jal lldPic32IRTimerIntHandler
	nop
	portRESTORE_CONTEXT

	.end		lldPic32IRTimerIntWrapper
