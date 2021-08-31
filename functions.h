#include <avr/io.h>

uint8_t bitIsSet(uint8_t reg, uint8_t bit);
uint8_t bitIsClear(uint8_t reg, uint8_t bit);
void waitBitIsSet(uint8_t reg, uint8_t bit);
void waitBitIsClear(uint8_t reg, uint8_t bit);