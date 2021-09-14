#include "joystick.h"
#include <avr/io.h>

#include "adc.h"

uint8_t fetchJoystick(char xy) {
	if (xy == 'x') {
		return adcData.channel1;
	}
}

uint8_t fetchSlider(uint8_t num) {
	if (num == 1) {
		return adcData.channel3;
	}
}