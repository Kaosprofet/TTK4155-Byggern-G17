#ifndef CAN_COMMUNICATIONS_H
#define CAN_COMMUNICATIONS_H

void CAN_com_INIT(void); // Initializas using loop-back mode**
void CAN_com_send(uint8_t ID, uint8_t length, uint8_t data);
uint8_t CAN_com_recieve(void){;
#endif