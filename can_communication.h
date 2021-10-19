#ifndef CAN_COMMUNICATIONS_H
#define CAN_COMMUNICATIONS_H

#include <avr/io.h>

extern volatile uint8_t can_interrupt_flag;

//Defining IDs
#define CAN_ID_ManInputData 20
#define CAN_ID_PID_paramters 21

typedef struct{unsigned int ID; uint8_t length; uint8_t data[8];} can_message;
void CAN_test(void); //Exercise 5 test
void CAN_sendmessage(can_message* message); // sends data
can_message CAN_recieve_message(void); //receives data
void CAN_test_normal(void); //Sends an arbitrary message for tests with node 2

#endif