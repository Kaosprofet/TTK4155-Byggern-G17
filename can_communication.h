#ifndef CAN_COMMUNICATIONS_H
#define CAN_COMMUNICATIONS_H

typedef struct{uint8_t ID; uint8_t length; uint8_t data[8];} can_message;



//void CAN_controller_init(can_modes mode); // Initializes using loop-back mode**
void CAN_com_send(uint8_t ID, uint8_t length, uint8_t data); // sends data**
uint8_t CAN_com_recieve(void); //receives data **
#endif