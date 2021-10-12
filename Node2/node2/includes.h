#ifndef INCLUDES_H
#define INCLUDES_H

#define F_CPU 84000000
#define BAUD 9600 

#define BAUD_PRESCALE (F_CPU / (BAUD * 16UL))

#include <stdint.h>

#include "sam.h"
#include "functions.h"
#include "uart.h"


#endif