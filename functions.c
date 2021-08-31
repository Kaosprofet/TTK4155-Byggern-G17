#include <avr/io.h>
#include "functions.h"

uint8_t bitIsSet(uint8_t reg, uint8_t bit) {
    return reg & (1<<bit);
}

uint8_t bitIsClear(uint8_t reg, uint8_t bit) {
    return !( reg & (1<<bit));
}

void waitBitIsSet(uint8_t reg, uint8_t bit) {
    while(bitIsClear(reg,bit));
}

void waitBitIsClear(uint8_t reg, uint8_t bit) {
    while(bitIsSet(reg,bit));
}