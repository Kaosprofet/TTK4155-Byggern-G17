
#ifndef F_CPU // In case F_CPU not defined
#define F_CPU 4915200 // 4.9152 MHz
#endif

#ifndef BAUD
#define BAUD 9600
#endif

#include <avr/interrupt.h>
#include <avr/io.h>
#include <stdio.h>
#include <util/delay.h>
#include <stdlib.h>
#include <stdbool.h>


#include "functions.h"
#include "uart.h"
#include "sram.h"
#include "adc.h"
#include "joystick.h"
#include "game.h"


