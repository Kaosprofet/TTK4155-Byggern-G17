#ifndef INCLUDES_H
#include "includes.h"
#endif

uint8_t lastJoystickVal = 0;
uint8_t joystickMenuTreshold = 70;


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
		
		// Starting in menu
		bootStartMenu();
		while(1) {
			updateController(controller);
			if (abs(controller->y_val) > joystickMenuTreshold && !(lastJoystickVal > joystickMenuTreshold)) {
				moveArrow(controller);
			}
			
			if (bitIsSet(PORTD, PD4)) {
				
				break;
			}
			
			lastJoystickVal = controller->y_val;
		}
		menuSelection(controller);
		
		
	}
}

void initGame() {
	initButton();
	initADC();
	initUart();
	initSRAM();
	initOLED();
}