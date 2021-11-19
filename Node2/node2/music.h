#ifndef MUSIC_H
#define MUSIC_H


#ifndef INCLUDES_H
#include "includes.h"
#endif

void play_music(const int* melody, int tempo, int amount_of_notes);

void song_select(void);

void game_over_music(void);

#endif