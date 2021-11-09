#ifndef INTERFACE_H
#define INTERFACE_H

#include <avr/io.h>

void bootStartupScreen(void);	// Print startup screen
void bootStartMenu(void);		// Print start menu

void moveArrow(void);			// Determine movement direction

void changeLetter(void);
void changeChar(void);

void menuSelection(void);		// Selecting behavior for the buttons on the main menu

void playMenu(void);			// The game segment
void initHighscore(void);
void highscore(void);			// Highscore
void input_highscore(uint8_t score);
void set_highscore(char name[], uint8_t value);
void resetGame(void);									

#endif