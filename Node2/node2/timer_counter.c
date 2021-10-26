#ifndef INCLUDES_H
#include "includes.h"
#endif

void init_timer(void){
    // TC_CMRx has to be sett to waveform mode
    // Make sure TIOA is output (should be while Tc 0 is in wave mode)
    clearBit(TC_WPMR, WPEN) //Disables the write protecion 
    setBit(TC_CMR0,WAVE); //Sets TC0 as a wave unit
    TC_CMR0 &= 0x00000007; // Sets the counter frequency to be MCK/2 
    TC_CMR0 |= 0x00002000; //Sets the timer to increase until RC, then reset from 0. 
    TC_CMR0 |= 0x00020000; //Sets ACPC to 01, which sets RC compare
    TC_RC0 |= 0x00000000; //Henrik bruk disse for å sette duty cycle 
    TC_RA0 |= 0x00000000; 
    //May have to clear WPEN first to be able to write to TC_CMRx
    //May have to check that WPKEY = 0x54494D
}