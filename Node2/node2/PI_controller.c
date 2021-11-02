#ifndef INCLUDES_H
#include "includes.h"
#endif

uint16_t PI_controller(uint16_t r, uint16_t y){
	uint16_t e = r - y;
	//Shifting the array of error values
	for(int i = 0; i<N-1;i++){ e_vec[i+1] = e_vec[i];}
	e_vec[0] = e; //Adding the last value;
	uint16_t e_sum = 0;
	//Calculation the sum
	for(int j=0; j<N;j++){ e_sum += e_vec[j];}
	return Kp*e+Ti*e_sum;
}
