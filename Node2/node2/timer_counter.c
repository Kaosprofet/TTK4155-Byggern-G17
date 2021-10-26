#ifndef INCLUDES_H
#include "includes.h"
#endif

void init_timer(void){
    // TC_CMRx has to be sett to waveform mode
    // Make sure TIOA is output (should be while Tc 0 is in wave mode)
    // PIOC->PIO_ABSR |= PIO_PC19B_PWMH5; //Selects TI0A to use pin 46
    // TC0->TC_WPMR &= 0xFFFFFFFE;//Disables the write protecion 
    // TC0->WAVE |= 0x00008000; //Sets TC0 as a wave unit
    // TC0->TC_CMR &= 0x00000007; //Sets the counter frequency to be MCK/2 
    //TC0->REG_TC0_CMR0 |= 0x00002000; //Sets the timer to increase until RC, then reset from 0. 
	// Tc->TC_CMR |= 0x00020000; //Sets ACPC to 01, which sets RC compare
    // Tc->TC_RC |= 0x01000000; //Henrik bruk disse for å sette duty cycle 
    // Tc->TC_RA |= 0x00000111; 
    //May have to clear WPEN first to be able to write to TC_CMRx
    //May have to check that WPKEY = 0x54494D
}