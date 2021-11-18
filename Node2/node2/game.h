#ifndef GAME_H
#define GAME_H

#include <stdint.h>

void start_game(void);
uint32_t run_game(void);
uint8_t calculate_score(uint32_t raw_score);

#endif