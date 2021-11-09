#ifndef INCLUDES_H
#include "includes.h"
#endif

//Utelizes the Cortex-M3 processor system timer

#define f_clock 84 //MHz


void init_timer_us(uint8_t unit){
	//Sets the reload value for the SysTick clock. Loads N=unit*f_clk[MHz] cycles. 
	SysTick->LOAD = ((int)(unit*f_clock)&SysTick_LOAD_RELOAD_Msk);
	//Resets the current value register
	SysTick->VAL = 0;
	
	//Sets clock source to MCK
	SysTick->CTRL = (1<<SysTick_CTRL_CLKSOURCE_Pos)&SysTick_CTRL_CLKSOURCE_Msk;
	//Enables SysTick exception request
	SysTick->CTRL |= (1<<SysTick_CTRL_TICKINT_Pos)&SysTick_CTRL_TICKINT_Msk;
	//Enable counter
	SysTick->CTRL |= (1<<SysTick_CTRL_ENABLE_Pos)&SysTick_CTRL_ENABLE_Msk;
	
}


void delay_us(uint16_t delay_time){
	init_timer_us(delay_time); //Sets the timer
	while(!((SysTick->CTRL)&SysTick_CTRL_CLKSOURCE_Msk));
}