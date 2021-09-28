#ifndef INCLUDES_H
#include "includes.h"
#endif

uint8_t menuSelected;
uint8_t menuElements = 4;
uint8_t highscore_address = 0x400; // Starting right after oled saved data
uint8_t num_highscores = 5;

// Print startup screen
void bootStartupScreen(void) {
	
}


// Print start menu
void bootStartMenu(void) {
	
}

// Determine movement direction
void moveArrow(struct controllers *controller) {
	if(controller->y_val > 0) {
		arrowUp();
	}
	else if(controller->y_val < 0) {
		arrowDown();
	}
}

// Move the arrow up on the menu
void arrowUp(void) {
	if (menuSelected>0) {
		
		menuSelected--;
	}
}

// Move the arrow down on the menu
void arrowDown(void) {
	if (menuSelected<menuElements) {
		
		menuSelected++;
	}
}

// Selecting behavior for the buttons on the main menu
void menuSelection(struct controllers *controller) {
	switch(menuSelected){
		case(0):
		playMenu(controller);
		break;
		case(1):
		highscore();
		break;
		case(2):
		resetGame();
		break;
	}
}

// The game segment
void playMenu(struct controllers *controller) {
	// 
	
	// Playing the game
	while (1) {
		
	}
}

// Highscore
void highscore(void) {
	//print highscore
	while (1) {
		// wait on highscore page
		
		// Break on back button
		if (bitIsSet(PORTD, PD3)) {
			break;
		}
	}
}

void set_highscore(uint8_t value) {
	uint8_t highscore[num_highscores];
	for (uint8_t i = 0; i < num_highscores; i++) {
		highscore[i] = readSRAM(highscore_address+i);
	}
	for (uint8_t i = num_highscores-1; i >= 0; i--) {
		if (highscore[i]<value) {
			if (i==num_highscores) {
				highscore[i] = value;
			}
			else {
				highscore[i+1] = highscore[i];
				highscore[i] = value;
			}
		}
	}
	for (uint8_t i = 0; i < num_highscores; i++) {
		writeSRAM(highscore_address+i, highscore[i]);
	}
}

// Reset highscore
void resetGame(void) {
	for (uint8_t address = highscore_address; address < highscore_address+num_highscores; address++) {
		writeSRAM(address, 0x00000000);
	}
}
