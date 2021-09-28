#ifndef INCLUDES_H
#include "includes.h"
#endif



struct controllers* controller = NULL;

void playGame(void) {
	// Initialize and allocate controller
	controller = (struct controllers*) malloc(sizeof(struct controllers));
	calibrateJoystick(controller);

	
	// Initialize all functions
	initGame();

	while(1) {
		// update and print controller each loop
		updateController(controller);
		printController(controller);
	}
}

void initGame() {
	initButton();
	initADC();
	initUart();
	initSRAM();
	initOLED();
}