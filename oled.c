#include "functions.h"
#include <avr/io.h>

#ifndef OLED_CMD
#define OLED_CMD 0x1000 //The address room for OLED command
#endif

#ifndef OLED_DATA
#define OLED_DATA 0x1200 //The address room for OLED data
#endif

#define height 64
#define width 128


volatile char * OLED_CMD_val = (char *)OLED_CMD; 
volatile char * OLED_DATA_val = (char *)OLED_DATA;

void writeCMD(uint8_t cmd){OLED_CMD_val[0] = cmd;} //Writes the input command to the command register of the OLED
void writeDATA(uint8_t data){OLED_DATA_val[0] = data;} //Wrtites the input data to the data register of the OLED






void initOLED(void){
	uint8_t RecInitCommands[] = { //From the OLED manual page 15 (Recommended initialization commands)
		0xae, //Display off
		0xa1, //Segment remap
		0xda, //Common pads hardware: alternative
		0x12,
		0xc8, //common output scan direction:com63~com0
		0xa8, //multiplex ration mode:63
		0x3f,
		0xd5, //display divide ratio/osc. freq. mode
		0x80,
		0x81, //contrast control
		0x50,
		0xd9, //set pre-charge period
		0x21, 
		0x20, //Set Memory Addressing Mode
		0x02,
		0xdb, //VCOM deselect level mode
		0x30,
		0xad, //master configuration
		0x00, 
		0xa4, //out follows RAM content 
		0xa6, //set normal display
		0xaf, // display on
	};
	uint8_t num_commands = 21;
	for(uint8_t i=0; i <(num_commands -1); i++){ //Writes all the commands
		writeCMD(RecInitCommands[i]);
	}	
}

void printOled(char inString[]){
	
}
