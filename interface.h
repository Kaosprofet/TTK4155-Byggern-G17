#ifndef INTERFACE_H
#define INTERFACE_H

#include <avr/io.h>

void bootStartupScreen(void);							// Print startup screen
void bootStartMenu(struct controllers *controller);								// Print start menu
void moveArrow(struct controllers *controller);			// Determine movement direction
void arrowUp(void);										// Move the arrow up on the menu
void arrowDown(void);									// Move the arrow down on the menu

void menuSelection(struct controllers *controller);		// Selecting behavior for the buttons on the main menu

void playMenu(struct controllers *controller);			// The game segment
void initHighscore(void);
void highscore(void);									// Highscore
void set_highscore(char name[], uint8_t value);
void resetGame(void);									

#endif