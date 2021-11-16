
#ifndef F_CPU // In case F_CPU not defined
#define F_CPU 4915200 // 4.9152 MHz
#endif

#ifndef BAUD
#define BAUD 9600 //Baudrate for UART comunication
#endif

// All external libraries used in node 1
#include <avr/interrupt.h>
#include <avr/io.h>
#include <stdio.h>
#include <util/delay.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>

// All internal libraries
#include "functions.h"
#include "uart.h"
#include "sram.h"
#include "adc.h"
#include "joystick.h"
#include "game.h"
#include "oled.h"
#include "interface.h"
#include "spi.h"
#include "can_controller.h"
#include "can_communication.h"

// Joystick directions
enum directions {
	UP, DOWN, LEFT, RIGHT, NEUTRAL
};

// Struct for controller values
typedef struct Controller {											
	signed int x_zero;
	signed int y_zero;
	volatile signed int org_x_val;
	volatile signed int org_y_val;
	volatile signed int x_val;
	volatile signed int y_val;
	volatile enum directions dir;
	volatile uint8_t slider1_val;
	volatile uint8_t slider2_val;
} Controller;

Controller controller;

// Struct for game values
typedef struct Game {
	uint8_t game_status;
	uint8_t score;
} Game;

Game game;
