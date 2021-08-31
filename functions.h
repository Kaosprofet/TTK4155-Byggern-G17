#include <avr/io.h>

bool bitIsSet(uint8_t reg, uint8_t bit);
bool bitIsClear(uint8_t reg, uint8_t bit);
void whaitBitIsSet(uint8_t reg, uint8_t bit);
void whaitBitIsClear(uint8_t reg, uint8_t bit);