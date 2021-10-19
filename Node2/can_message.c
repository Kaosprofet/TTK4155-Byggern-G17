#ifndef INCLUDES_H
#include "includes.h"
#endif

void init_can() {
	uint32_t br;
	br = 125 << CAN_BR_BRP_Pos;
	br |= 1 << CAN_BR_SJW_Pos;
	br |= 3 << CAN_BR_PHASE1_Pos;
	br |= 3 << CAN_BR_PROPAG;
	br |= 4 << CAN_BR_PHASE2_Pos;
	uint8_t test = can_init_def_tx_rx_mb(br);
	
	if(test) {
		printf("CAN initialization failed");
	}
}