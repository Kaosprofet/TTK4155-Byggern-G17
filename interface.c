#ifndef INCLUDES_H
#include "includes.h"
#endif

#define menuElements 3
#define highscore_address 0x400 // Starting right after oled saved data
#define num_highscores 3
#define num_highscore_char 3
#define menu_offset 30
#define highscore_offset 30
#define joystickMenuTreshold 50

uint8_t menuSelected;
uint8_t lastJoystickVal = 0;

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
	menuSelected = 0;
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
			oled_pos(4,0);
			oled_print_left("  High Score", menu_offset);
			oled_pos(5,0);
			oled_print_left("  Reset", menu_offset);
			break;
			case(1):
			oled_print_left("  Play Game", menu_offset);
			oled_pos(4,0);
			oled_print_left("> High Score", menu_offset);
			oled_pos(5,0);
			oled_print_left("  Reset", menu_offset);
			break;
			case(2):
			oled_print_left("  Play Game", menu_offset);
			oled_pos(4,0);
			oled_print_left("  High Score", menu_offset);
			oled_pos(5,0);
			oled_print_left("> Reset", menu_offset);
			break;
		}
		
		oled_pos(7,100);
		oled_print("select");
		
		updateController(controller);
		printf("Menu selected: %d\n\r", menuSelected);
		//printController(controller);
		if (abs(controller->y_val) > joystickMenuTreshold && abs(lastJoystickVal) < joystickMenuTreshold) {
			moveArrow(controller);
		}
		
		// Menu selection 
		if (bitIsSet(PIND, PD4)) {break;}
		
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
	if (menuSelected<menuElements-1) {
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
	oled_reset();
	oled_pos(3,0);
	// Playing the game
	while (1) {
		oled_print("For ever a looser");
		// Break on back button
		if (bitIsSet(PIND, PD3)) {
			break;
		}
	}
}

// Highscore
// Initierer til AAA 0 poeng, bør ha en test som sjekker om vi ikke har non volatile minne og kan lagre mellom kjøringer
void initHighscore(void) {
		for (uint8_t i = 0; i < num_highscores*4; i = i + 4) {
			writeSRAM(highscore_address+i, 4);
			for (uint8_t j = 1; j <= num_highscore_char; j++) {
				writeSRAM(highscore_address+i+j, 'A');
			}
		}
}

void highscore(void) {
	oled_reset();
	oled_set_font(LARGE);
	oled_home();
	oled_print_centered("High Score");
	oled_pos(1,0);
	oled_draw_hline(128,0b00111100);
	
	for (uint8_t i = 0; i < num_highscores; i++) {
		char high_score[10] = "";
		for (uint8_t j = 1; j <= num_highscore_char; j++) {
			high_score[j-1] = readSRAM(highscore_address+i*4+j);
		}
		high_score[num_highscore_char] = ' ';
		high_score[num_highscore_char+1] = readSRAM(highscore_address+i*4)+'0';
		high_score[num_highscore_char+2] = '\0';
		//printf("%s\n\r", high_score);	//debug
		oled_pos(i+2,0);
		oled_print_left(high_score, highscore_offset);
	}
	
	oled_pos(7,0);
	oled_print("back");
	while (!bitIsSet(PIND, PD3)) {

	}
}

void input_highscore(void) {
	while(1) {
		
	}
}

void set_highscore(char name[], uint8_t value) {
	uint8_t highscore[num_highscores];
	char names[num_highscores][num_highscore_char];
	
	for (uint8_t i = 0; i < num_highscores; i++) {
		highscore[i] = readSRAM(highscore_address+i*4);
		for (uint8_t j = 1; j <= num_highscore_char; j++) {
			names[i][j-1] = readSRAM(highscore_address+i*4+j);
		}
	}

	for (uint8_t i = num_highscores-1; i >= 0; i--) {
		if (highscore[i]<value) {
			if (i==num_highscores) {			// If last name in high score
				highscore[i] = value;
				for (uint8_t j = 0; j < num_highscore_char; j++) {
					names[i][j] = name[j];
				}
			}
			else {
				highscore[i+1] = highscore[i];
				for (uint8_t j = 0; j < num_highscore_char; j++) {
					names[i+1][j] = names[i][j];
				}
				highscore[i] = value;
				for (uint8_t j = 0; j < num_highscore_char; j++) {
					names[i][j] = name[j];
				}
			}
		}
	}
	for (uint8_t i = 0; i < num_highscores; i++) {
		writeSRAM(highscore_address+i*4, highscore[i]);
		for (uint8_t j = 1; j <= num_highscore_char; j++) {
			writeSRAM(highscore_address+i*4+j, names[i][j-1]);
		}
	}
}

// Reset highscore
void resetGame(void) {
	initHighscore();
}
