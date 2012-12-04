#include <p32xxxx.h>
#include <sys/asm.h>
#include "ISR_Support.h"

	.set	nomips16
 	.set 	noreorder

 	.extern pxCurrentTCB
 	.extern vTaskSwitchContext

 	.extern lldWm8731IntHandler
	.extern xISRStackTop
 	.global	lldWm8731IntWrapper

	.set		noreorder
	.set 		noat
	.ent		lldWm8731IntWrapper

lldWm8731IntWrapper:

	portSAVE_CONTEXT
	jal lldWm8731IntHandler
	nop
	portRESTORE_CONTEXT

	.end		lldWm8731IntWrapper
