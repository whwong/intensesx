#include <p32xxxx.h>
#include <sys/asm.h>
#include "ISR_Support.h"

	.set	nomips16
 	.set 	noreorder

 	.extern pxCurrentTCB
 	.extern vTaskSwitchContext

 	.extern lldPic32UARTIntHandler
	.extern xISRStackTop
 	.global	lldPic32UARTIntWrapper

	.set		noreorder
	.set 		noat
	.ent		lldPic32UARTIntWrapper

lldPic32UARTIntWrapper:

	portSAVE_CONTEXT
	jal lldPic32UARTIntHandler
	nop
	portRESTORE_CONTEXT

	.end		lldPic32UARTIntWrapper
