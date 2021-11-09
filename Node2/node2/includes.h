#ifndef INCLUDES_H
#define INCLUDES_H

#define F_CPU 84000000
#define BAUD 9600 

#define BAUD_PRESCALE (F_CPU / (BAUD * 16UL))

#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdarg.h>


typedef struct Controller {
	int8_t x;
	int8_t y;
	uint8_t button_state;
	uint8_t slider_1_val;
	uint8_t slider_2_val;
} Controller;

volatile Controller controller;

typedef struct Game {
	uint8_t game_status;
	uint8_t score; 
} Game;

Game game;

#include "sam.h"
#include "functions.h"
#include "uart.h"
#include "can_controller.h"
#include "can_interrupt.h"
#include "can_message.h"
#include "printf.h"
#include "servo.h"
#include "PI_controller.h"
#include "solenoid.h"
#include "game.h"
#include "game_board_driver.h"
#include "DAC.h"
#include "motor_driver.h"

#endif