/*
 * TTK4155-Byggern-G17.c
 *
 * Created: 31/08/2021 14:03:31
 * Author : henri
 */ 

#include <avr/io.h>
#include "uart.h"
#include "functions.h"

int main(void) {

    initUart();

    /* Replace with your application code */
    while(1){
		transmitByte(0x1);
	}
}

