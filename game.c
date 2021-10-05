#ifndef INCLUDES_H
#include "includes.h"
#endif


struct controllers* controller = NULL;

void playGame(void) {
	// Initialize and allocate controller
	initGame();
	controller = (struct controllers*) malloc(sizeof(struct controllers));
	calibrateJoystick(controller);
	
	bootStartupScreen();
	_delay_ms(2000);
	
	while(1) {
		// update and print controller each loop
		//updateController(controller);
		//printController(controller);
		
		// Starting in menu
		bootStartMenu(controller);
		// Go to selected part of game
		menuSelection(controller);
	}
}

void initGame() {
	initButton();
	initADC();
	initUart();
	initSRAM();
	initOLED();
	initHighscore();
}