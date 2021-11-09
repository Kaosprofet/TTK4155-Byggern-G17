#ifndef INCLUDES_H
#include "includes.h"
#endif

void playGame(void) {
	// Initialize and allocate controller
	initGame();
	controllers controller;
	calibrateJoystick(&controller);
	
	bootStartupScreen();
	_delay_ms(2000);
	
	while(1) {	
		// Starting in menu
		bootStartMenu(&controller);
		// Go to selected part of game
		menuSelection(&controller);
	}
}

void initGame() {
	initButton();
	initADC();
	initUart();
	initSRAM();
	initOLED();
	initHighscore();
	can_controller_init(CAN_NORMAL);
}