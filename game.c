#ifndef INCLUDES_H
#include "includes.h"
#endif



struct controllers* controller = NULL;

void playGame(void) {
	// Initialize and allocate controller
	controller = (struct controllers*) malloc(sizeof(struct controllers));
	calibrateJoystick(controller);

	
	// Initialize button
	initButton();

	while(1) {
		// update controller each loop
		updateController(controller);
		printController(controller);
	}
}