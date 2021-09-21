#include "game.h"
#include <avr/io.h>
#include <stdio.h>
#include <stdlib.h>
#include "joystick.h"
#include "functions.h"


void initButton(void) {
	clearBit(DDRD,PD3);
	clearBit(DDRD,PD4);
}

volatile struct controllers* controller = NULL;


void playGame(void) {
	// Initialise and allocate controller
	controller = (struct controllers*) malloc(sizeof(struct controllers));
	controller->offset = 30;
	
	// Initialise button
	initButton();

	while(1) {
		updateController(controller);
		printController(controller);
	}

	
	
	
	
}