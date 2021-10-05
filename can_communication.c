#ifndef INCLUDES_H
#include "includes.h"
#include "can_communication.h"
#endif



typedef enum{CAN_NORMAL, CAN_SLEEP, CAN_LOOPBACK, CAN_LISTEN, CAN_CONFIG} can_modes;

void CAN_controller_init(can_modes mode){
	
}

void CAN_CTRL(can_modes mode){
	uint8_t byte = 0;
	switch(mode){
		case(CAN_NORMAL): byte = 0b00000000; break;
		case(CAN_SLEEP): byte = 0b00100000; break;
		case(CAN_LOOPBACK): byte = 0b01000000; break;
		case(CAN_LISTEN): byte = 0b01100000; break;
		case(CAN_CONFIG): byte = 0b10000000; break;
	}
	can_controller_write(0xF, byte);
}
//CANCTRL (ADDRESS: XFh)