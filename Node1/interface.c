#ifndef INCLUDES_H
#include "includes.h"
#endif

#define highscore_address 0x400 // Starting right after oled saved data
#define num_highscores 3
#define num_highscore_char 3
#define menu_offset 30
#define highscore_offset 30
#define joystickMenuTreshold 50

char letters[4] = {'A', 'A', 'A', '\0'};

uint8_t menuSelected;
uint8_t letterSelected;

uint8_t lastJoystickYVal = 0;
uint8_t lastJoystickXVal = 0;
uint8_t lowHighScore = 0;

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
void bootStartMenu(void) {
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

		updateController();
	
		if (abs(controller.y_val) > joystickMenuTreshold && abs(lastJoystickYVal) < joystickMenuTreshold) {
			moveArrow();
		}
		
		// Menu selection 
		if (bitIsSet(PIND, PD4)) {
			break;
			_delay_ms(100);
		}
		
		// update last joystick value
		lastJoystickYVal = controller.y_val;
	}
}

// Selecting behavior for the buttons on the main menu
void menuSelection(void) {
	switch(menuSelected){
		case(0):
		game.game_status = 1;
		game.score = 0;
		
		mode_select_menu();
		
		CAN_send_game_status();
		_delay_ms(100);
		playMenu();
		_delay_ms(200);
		check_score();
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
void playMenu(void) {
	oled_reset();
	oled_pos(3,0);
	
	oled_print_centered("GOGOGO");
	
	// Playing the game, Break on back button or game
	while (!bitIsSet(PIND, PD3)) { //  || game.game_status
		updateController();
		
		CAN_send_inputData();
		_delay_ms(30);
		CAN_decode_message();
		_delay_ms(30);

		if(!game.game_status){
			printf("Game Over\n\r");
			break;
		}
	}
}

void check_score(void) {
	printf("Score: %d\n\r", game.score);
	if(game.score > lowHighScore) {
		input_highscore(game.score);
	}
}

// Highscore
// Initierer til AAA 0 poeng
void initHighscore(void) {
	for (uint8_t i = 0; i < num_highscores*4; i = i + 4) {
		writeSRAM(highscore_address+i, 0b00000000);
		for (uint8_t j = 1; j <= num_highscore_char; j++) {
			writeSRAM(highscore_address+i+j, 'A');
		}
	}
}

void highscore(void) {
	// Start music
	CAN_send_music_status(1,1);
	_delay_ms(20);
	
	// Screen candy
	oled_reset();
	oled_set_font(LARGE);
	oled_home();
	oled_print_centered("High Score");
	oled_pos(1,0);
	oled_draw_hline(128,0b00111100);
	
	for (uint8_t i = 0; i < num_highscores; i++) {
		char high_score[10];
		for (uint8_t j = 1; j <= num_highscore_char; j++) {
			high_score[j-1] = readSRAM(highscore_address+i*4+j);
		}
		high_score[num_highscore_char] = ' ';
		
		uint8_t score_dec = readSRAM(highscore_address+i*4);
		
		char score_char[3];
		sprintf(score_char, "%d", (uint8_t)score_dec); 
		high_score[num_highscore_char+1] = score_char[0];
		high_score[num_highscore_char+2] = score_char[1];
		high_score[num_highscore_char+3] = score_char[2];
		high_score[num_highscore_char+4] = '\0';
		oled_pos(i+2,0);
		oled_print_left(high_score, highscore_offset);
	}
		
	oled_pos(7,0);
	oled_print("back");
	while (!bitIsSet(PIND, PD3)) {
	}
	CAN_send_music_status(0,1);
	_delay_ms(200);
}

void input_highscore(uint8_t score) {
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
		
		updateController();
		if (abs(controller.x_val) > joystickMenuTreshold && abs(lastJoystickXVal) < joystickMenuTreshold) {
			changeLetter();
		}
		if (abs(controller.y_val) > joystickMenuTreshold && abs(lastJoystickYVal) < joystickMenuTreshold) {
			changeChar();
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
		
		lastJoystickXVal = controller.x_val;
		lastJoystickYVal = controller.y_val;
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
		printf("Name: %c%c%c Score: %d\n\r",names[i][0],names[i][1],names[i][2], highscore[i]);
	}
	
	// Reorder highscore list for new highscore
	
	for (int8_t i = num_highscores-1; i >= 0; i--) {
		// printf("highscore[i]: %d, new value: %d\n\r", highscore[i], value);
		if ((uint8_t)highscore[i]<value) {
			if (i==num_highscores-1) {			// If last name in high score discard old "last" place
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

	// Updates highscore threshold
    lowHighScore = highscore[num_highscores-1];                                                                                                                                                                                                                                                                  
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
	_delay_ms(200);
}

// Determine movement direction
void moveArrow(void) {
	if(controller.y_val > 0 && menuSelected>0) {
		menuSelected--;
	}
	else if(controller.y_val < 0 && menuSelected<2) {
		menuSelected++;
	}
}

// Change the selected letter in highscore
void changeLetter(void) {
	if(controller.x_val > 0 && letterSelected<2) {
		letterSelected++;
	}
	else if(controller.x_val < 0 && letterSelected>0) {
		letterSelected--;
	}
}

// Change the selected letters character
void changeChar(void) {
	if(controller.y_val > 0 && letters[letterSelected]>'A') {
		letters[letterSelected]--;
	}
	else if(controller.y_val < 0 && letters[letterSelected]<'Z') {
		letters[letterSelected]++;
	}
}


void mode_select_menu() {
	
	
}