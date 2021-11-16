#ifndef CAN_COMMUNICATIONS_H
#define CAN_COMMUNICATIONS_H

#include <avr/io.h>

extern volatile uint8_t can_interrupt_flag;

//Defining IDs
#define CAN_ID_GameStatus 20
#define CAN_ID_ManInputData 30
#define CAN_ID_PID_paramters 40
#define CAN_ID_Music 1

typedef struct{
	unsigned int ID; 
	uint8_t length; 
	uint8_t data[8];
} can_message;
	

void CAN_send_message(can_message* message); // sends message-struct
can_message CAN_receive_message(void); //receives data
void CAN_send_game_status(void);


// Test functions
void CAN_test(void); //Sends a message in LOOPBACK mode
void CAN_test_normal(void); //Sends an arbitrary message for tests with node 2
void CAN_test_recieve(void);

#endif