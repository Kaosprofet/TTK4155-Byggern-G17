#ifndef INCLUDES_H
#include "includes.h"
#endif

void playGame(void) {
	// Initialize and allocate controller
	initGame();
	
	bootStartupScreen();
	_delay_ms(2000);
	
	while(1) {	
		// Starting in menu
		bootStartMenu();
		// Go to selected part of game
		menuSelection();
	}
}

// Initialise all parts needed for game
void initGame() {
	initButton();
	initADC();
	initUart();
	initSRAM();
	initOLED();
	initHighscore();
	CAN_controller_init(CAN_NORMAL);
	
	calibrateJoystick();
}