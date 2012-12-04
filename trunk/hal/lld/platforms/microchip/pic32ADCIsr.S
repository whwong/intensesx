#include <p32xxxx.h>
#include <sys/asm.h>
#include "ISR_Support.h"

	.set	nomips16
 	.set 	noreorder

 	.extern pxCurrentTCB
 	.extern vTaskSwitchContext

 	.extern lldPic32ADCIntHandler
	.extern xISRStackTop
 	.global	lldPic32ADCIntWrapper

	.set		noreorder
	.set 		noat
	.ent		lldPic32ADCIntWrapper

lldPic32ADCIntWrapper:

	portSAVE_CONTEXT
	jal lldPic32ADCIntHandler
	nop
	portRESTORE_CONTEXT

	.end		lldPic32ADCIntWrapper
