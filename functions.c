#include <avr/io.h>
#include "functions.h"

bool bitIsSet(uint8_t reg, uint8_t bit) {
    return reg & (1<<bit);
}

bool bitIsClear(uint8_t reg, uint8_t bit) {
    return !( reg & (1<<bit));
}

void whaitBitIsSet(uint8_t reg, uint8_t bit) {
    while(bitIsClear(reg,bit))
}

void whaitBitIsClear(uint8_t reg, uint8_t bit) {
    while(bitIsSet(reg,bit))
}