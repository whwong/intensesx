#include <p32xxxx.h>
#include <sys/asm.h>
#include "ISR_Support.h"

	.set	nomips16
 	.set 	noreorder

 	.extern pxCurrentTCB
 	.extern vTaskSwitchContext

 	.extern lldPic32IRIntHandler
	.extern xISRStackTop
 	.global	lldPic32IRIntWrapper

	.set		noreorder
	.set 		noat
	.ent		lldPic32IRIntWrapper

lldPic32IRIntWrapper:

	portSAVE_CONTEXT
	jal lldPic32IRIntHandler
	nop
	portRESTORE_CONTEXT

	.end		lldPic32IRIntWrapper
