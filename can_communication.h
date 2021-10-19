#ifndef CAN_COMMUNICATIONS_H
#define CAN_COMMUNICATIONS_H

#include <avr/io.h>

extern volatile uint8_t can_interrupt_flag;

typedef struct{unsigned int ID; uint8_t length; uint8_t data[8];} can_message;
void CAN_test(void);
void CAN_sendmessage(can_message* message); // sends data**
can_message CAN_recieve_message(void); //receives data **


#endif