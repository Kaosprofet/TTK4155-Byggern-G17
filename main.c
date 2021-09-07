/*
 * TTK4155-Byggern-G17.c
 *
 * Created: 31/08/2021 14:03:31
 *
 */ 

#ifndef F_CPU // In case F_CPU not defined
#define F_CPU 4915200 // 4.9152 MHz
#endif

#include <avr/io.h>
#include "uart.h"
#include "functions.h"

void ex1() {
  initUart();
  while(1){
		transmitByte(0b00000001);
  }
}

int main(void) {

  ex1();
    
	}
}

