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

Controller controller;

#include "sam.h"
#include "functions.h"
#include "uart.h"
#include "can_controller.h"
#include "can_interrupt.h"
#include "can_message.h"
#include "printf.h"
#include "IR_driver.h"
#include "servo.h"
#include "PI_controller.h"

#endif