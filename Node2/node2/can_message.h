#ifndef CAN_MESSAGE_H_
#define CAN_MESSAGE_H_

#include <stdint.h>

#define music_id 1
#define controller_id 30
#define status_id 20
#define pid_id 40



void init_can(void);
void can_encode_message(uint8_t ID);
void can_decode_message(void);

#endif