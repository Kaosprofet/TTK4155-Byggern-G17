#ifndef INCLUDES_H
#define INCLUDES_H

#define F_CPU 84000000
#define BAUD 9600 

#define BAUD_PRESCALE (F_CPU / (BAUD * 16UL))

#include <stdint.h>
#include <stdio.h>
#include <stdarg.h>

typedef struct coords {
	int8_t x,y;
} coords;


#include "sam.h"
#include "functions.h"
#include "uart.h"
#include "can_controller.h"
#include "can_interrupt.h"
#include "can_message.h"
#include "printf.h"


#endif