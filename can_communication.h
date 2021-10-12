#ifndef CAN_COMMUNICATIONS_H
#define CAN_COMMUNICATIONS_H


typedef struct{unsigned int ID; uint8_t length; uint8_t data[8];} can_message;
void CAN_test(void);	
void CAN_com_init(uint8_t can_mode); // Initialization
void CAN_sendmessage(can_message* message); // sends data**
can_message CAN_recieve_message(void); //receives data **


#endif