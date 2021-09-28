#ifndef INCLUDES_H
#include "includes.h"
#endif

uint8_t menuSelected = 0;
uint8_t menuElements = 3;
uint8_t highscore_address = 0x400; // Starting right after oled saved data
uint8_t num_highscores = 5;
uint8_t menu_offset = 30;
uint8_t lastJoystickVal = 0;
uint8_t joystickMenuTreshold = 70;

// Print startup screen
void bootStartupScreen(void) {
	oled_reset();
	oled_set_font(LARGE);
	oled_pos(1,0);
	oled_draw_hline(128,0b00111100);
	oled_pos(3,0);
	oled_print_centered("Kult spill 5000");
	oled_set_font(SMALL);
	oled_pos(4,0);
	oled_print_centered("An ok adventure");
	oled_pos(6,0);
	oled_draw_hline(128,0b00111100);
}


// Print start menu
void bootStartMenu(struct controllers *controller) {
	oled_reset();
	oled_set_font(LARGE);
	oled_home();
	oled_print_centered("Main Menu");
	oled_pos(1,0);
	oled_draw_hline(128,0b00111100);
	while (1) {
		oled_set_font(NORMAL);
		oled_pos(3,0);
		switch(menuSelected) {
			case(0):
			oled_print_left("> Play Game", menu_offset);
			oled_print_left("  High Score", menu_offset);
			oled_print_left("  Reset", menu_offset);
			break;
			case(1):
			oled_print_left("  Play Game", menu_offset);
			oled_print_left("> High Score", menu_offset);
			oled_print_left("  Reset", menu_offset);
			break;
			case(2):
			oled_print_left("  Play Game", menu_offset);
			oled_print_left("  High Score", menu_offset);
			oled_print_left("> Reset", menu_offset);
			break;
		}
		
		
		
		oled_pos(7,100);
		oled_print("select");
		
		updateController(controller);
		if (abs(controller->y_val) > joystickMenuTreshold && !(lastJoystickVal > joystickMenuTreshold)) {
			moveArrow(controller);
		}
		
		// Menu selection 
		if (bitIsSet(PORTD, PD4)) {break;}
		
		// update last joystick value
		lastJoystickVal = controller->y_val;
	}
	
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
