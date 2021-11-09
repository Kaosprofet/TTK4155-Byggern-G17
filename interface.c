#ifndef INCLUDES_H
#include "includes.h"
#endif

#define highscore_address 0x400 // Starting right after oled saved data
#define num_highscores 3
#define num_highscore_char 3
#define menu_offset 30
#define highscore_offset 30
#define joystickMenuTreshold 50

uint8_t menuSelected;
uint8_t letterSelected;

char letters[4] = {'A', 'A', 'A', '\0'};

uint8_t lastJoystickYVal = 0;
uint8_t lastJoystickXVal = 0;


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
		//CAN_send_inputData(controller);
		updateController(controller);
		printController(controller);
	
		if (abs(controller->y_val) > joystickMenuTreshold && abs(lastJoystickYVal) < joystickMenuTreshold) {
			moveArrow(controller);
		}
		
		// Menu selection 
		if (bitIsSet(PIND, PD4)) {break;}
		
		// update last joystick value
		lastJoystickYVal = controller->y_val;
	}
	
}


// Selecting behavior for the buttons on the main menu
void menuSelection(struct controllers *controller) {
	switch(menuSelected){
		case(0):
		playMenu(controller);
		//input_highscore(controller, score);
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
	oled_print_centered("GOGOGO");
	// Playing the game, Break on back button
	while (!bitIsSet(PIND, PD3)) {
		updateController(controller);
		printController(controller);
		CAN_send_inputData(controller);
		_delay_ms(50);
	}
	// Test scoring system
	uint8_t score = 10;
	input_highscore(controller, score);
}

// Highscore
// Initierer til AAA 0 poeng, b�r ha en test som sjekker om vi ikke har non volatile minne og kan lagre mellom kj�ringer
void initHighscore(void) {
		for (uint8_t i = 0; i < num_highscores*4; i = i + 4) {
			writeSRAM(highscore_address+i, 0);
			for (uint8_t j = 1; j <= num_highscore_char; j++) {
				writeSRAM(highscore_address+i+j, 'A');
			}
		}
}

void highscore(void) {
	// Screen candy
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
		oled_pos(i+2,0);
		oled_print_left(high_score, highscore_offset);
	}
		
	oled_pos(7,0);
	oled_print("back");
	while (!bitIsSet(PIND, PD3)) {
	}
}

void input_highscore(struct controllers *controller, uint8_t score) {
	letterSelected = 0;
	for (uint8_t i = 0; i<3 ;i++) {letters[i] = 'A';};
	
	// Screen candy
	oled_reset();
	oled_set_font(LARGE);
	oled_pos(1,0);
	oled_draw_hline(128,0b00111100);
	oled_pos(3,0);
	oled_print_centered("NEW HIGHSCORE!!!");
	oled_set_font(NORMAL);
	oled_pos(4,0);
	oled_print_centered("Congratulation");
	oled_pos(6,0);
	oled_draw_hline(128,0b00111100);
	
	_delay_ms(2000);
	
	oled_reset();
	oled_set_font(LARGE);
	oled_home();
	oled_print_centered("Enter your name");
	oled_pos(1,0);
	oled_draw_hline(128,0b00111100);
	
	oled_set_font(NORMAL);
	oled_pos(7,90);
	oled_print("select");
	oled_set_font(LARGE);
	
	// Name input 
	while(!bitIsSet(PIND, PD4)) {
		oled_pos(3,0);
		oled_print_centered(letters);
		
		updateController(controller);
		if (abs(controller->x_val) > joystickMenuTreshold && abs(lastJoystickXVal) < joystickMenuTreshold) {
			changeLetter(controller);
		}
		if (abs(controller->y_val) > joystickMenuTreshold && abs(lastJoystickYVal) < joystickMenuTreshold) {
			changeChar(controller);
		}
		
		// Indicator printing
		oled_pos(4,0);
		switch(letterSelected) {
			case(0):
			oled_print_centered("^  ");
			break;
			case(1):
			oled_print_centered(" ^ ");
			break;
			case(2):
			oled_print_centered("  ^");
			break;
		}
		
		lastJoystickXVal = controller->x_val;
		lastJoystickYVal = controller->y_val;
	}
	oled_pos(4,0);
	oled_print_centered("   ");
	
	set_highscore(letters, score);
}

// Stores new highscore to memory
void set_highscore(char name[], uint8_t value) {
	uint8_t highscore[num_highscores];
	char names[num_highscores][num_highscore_char];
	
	// Collecting highscore from SRAM
	
	for (uint8_t i = 0; i < num_highscores; i++) {
		highscore[i] = readSRAM(highscore_address+i*4);
		for (uint8_t j = 1; j <= num_highscore_char; j++) {
			names[i][j-1] = readSRAM(highscore_address+i*4+j);
		}
		printf("Name: %s Score: %d\n\r",names[i], highscore[i]);
	}
	
	// Reorder highscore list for new highscore
	for (uint8_t i = num_highscores-1; i >= 0; i--) {
		if (highscore[i]<value) {
			if (i==num_highscores) {			// If last name in high score discard old "last" place
				highscore[i] = value;
				for (uint8_t j = 0; j < num_highscore_char; j++) {
					names[i][j] = name[j];
				}
			}
			else {
				// Push old highscore down 
				highscore[i+1] = highscore[i];
				for (uint8_t j = 0; j < num_highscore_char; j++) {
					names[i+1][j] = names[i][j];
				}
				// Add new highscore
				highscore[i] = value;
				for (uint8_t j = 0; j < num_highscore_char; j++) {
					names[i][j] = name[j];
				}
			}
		}
	}
	
	// Write new highscore to SRAM
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

// Determine movement direction
void moveArrow(struct controllers *controller) {
	if(controller->y_val > 0 && menuSelected>0) {
		menuSelected--;
	}
	else if(controller->y_val < 0 && menuSelected<2) {
		menuSelected++;
	}
}

// Change the selected letter in highscore
void changeLetter(struct controllers *controller) {
	if(controller->x_val > 0 && letterSelected<2) {
		letterSelected++;
	}
	else if(controller->x_val < 0 && letterSelected>0) {
		letterSelected--;
	}
}

// Change the selected letters character
void changeChar(struct controllers *controller) {
	if(controller->y_val > 0 && letters[letterSelected]>'A') {
		letters[letterSelected]--;
	}
	else if(controller->y_val < 0 && letters[letterSelected]<'Z') {
		letters[letterSelected]++;
	}
}
