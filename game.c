#include "game.h"
#include <avr/io.h>
#include "joystick.h"

volatile struct controllers {
	volatile uint8_t joystick_x;
	volatile uint8_t joystick_y;
	volatile uint8_t slider1;
	volatile uint8_t slider2;
};

void fetchController(void) {
	//controller.joystick_x = fetchJoystick('x');
	//controller.joystick_y = fetchJoystick('y');
	//controller.slider1 = fetchSlider(1);
	//controller.slider2 = fetchSlider(2);
}

void playGame(void) {
	volatile struct controllers controller;
	//controller.joystick_x = fetchJoystick('x');
	//controller.joystick_y = fetchJoystick('y');
	//controller.slider1 = fetchSlider(1);
	//controller.slider2 = fetchSlider(2);
}