#ifndef CAN_CONTROLLER_H_
#define CAN_CONTROLLER_H_

uint8_t can_controller_read(uint8_t address);
void can_controller_write(uint8_t address, uint8_t data);
void can_controller_request_to_send(uint8_t address);
void can_controller_read_status(uint8_t address);
void can_controller_bit_modify(uint8_t address);
void can_controller_reset(uint8_t address);

#endif