#ifndef CAN_CONTROLLER_H_
#define CAN_CONTROLLER_H_

void can_controller_read(void);
void can_controller_write(void);
void can_controller_request_to_send(void);
void can_controller_read_status(void);
void can_controller_bit_modify(void);
void can_controller_reset(void);

#endif